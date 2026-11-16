//
//  Composer.cpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#include "Composer.hpp"
#include "KeysymTable.hpp"

#include <cctype>
#include <filesystem>
#include <ranges>
#include <pwd.h>
#include <unistd.h>

/*

    This is the dumbest, laziest and horribly optimized parser ever.
    first thing I gotta optimize is the file loading each time we are getting a new list of
    characters. this has to be loaded directly into memory and be alr mapped so we don't need to find it
    each time a new input arrives.

    I wanna make this a struct instead in which we have the char sequence of the line saved into a vector
    for then be searched for each one of the vector list in memory instead of relaying on disk.

    Update: I did it, although there are like two global variables that I don't quite like but it should be fine for now.
    now what's left is adding more keywords to the charsMap and identify Unicode cases. also checking if the list of inputs is even possible
    instead of faking the hot check.

    Update 2: sequences now live in a trie (one node per key), so the hot check is just "is there a node for
    what was typed so far". Keysyms come from the X11 keysymdef.h table (KeysymTable.hpp) and <UXXXX> tokens
    are converted to UTF-8, so every Multi_key line in Compose.pre is usable.

 */

namespace {

// One node per key typed. A node with a composedChar is the end of a sequence.
struct ComposeNode {
    std::unordered_map<std::string, std::unique_ptr<ComposeNode>> children {};
    std::optional<std::string> composedChar {};
};

std::unordered_map<SequenceLayer, std::vector<ComposeSequence>> sequenceLayers {};
ComposeNode sequenceTree {};

constexpr SequenceLayer layerOrder[] { SequenceLayer::Default, SequenceLayer::UserFile, SequenceLayer::Custom };

// Keys where macOS sends something different from the keysym's Unicode value (or the keysym has none)
const std::unordered_map<std::string_view, std::string> keyOverrides {
    {"dead_acute", "'"},
    {"dead_grave", "`"},
    {"dead_circumflex", "^"},
    {"dead_tilde", "~"},
    {"dead_diaeresis", "¨"},
    {"dead_cedilla", "¸"},

    // NSEvent function key characters
    {"Up", "\uF700"},
    {"Down", "\uF701"},
    {"Left", "\uF702"},
    {"Right", "\uF703"}
};

std::string encodeUTF8(char32_t codePoint) {
    std::string encoded {};
    if (codePoint < 0x80) {
        encoded += static_cast<char>(codePoint);
    } else if (codePoint < 0x800) {
        encoded += static_cast<char>(0xC0 | (codePoint >> 6));
        encoded += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else if (codePoint < 0x10000) {
        encoded += static_cast<char>(0xE0 | (codePoint >> 12));
        encoded += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        encoded += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else if (codePoint < 0x110000) {
        encoded += static_cast<char>(0xF0 | (codePoint >> 18));
        encoded += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
        encoded += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        encoded += static_cast<char>(0x80 | (codePoint & 0x3F));
    }
    return encoded;
}

bool isHex(std::string_view text) {
    return !text.empty() && std::ranges::all_of(text, [](char c) { return std::isxdigit(static_cast<unsigned char>(c)); });
}

// Resolves a keysym name (no angle brackets) like "Cyrillic_a" or "U2192"
std::optional<std::string> resolveKeysym(std::string_view name) {
    if (auto found { keyOverrides.find(name) }; found != keyOverrides.end()) {
        return found->second;
    }
    if (auto found { keysymTable.find(name) }; found != keysymTable.end()) {
        return encodeUTF8(found->second);
    }
    // Unicode keysyms: <U2192>, <U1F600>
    if (name.size() >= 5 && name.size() <= 7 && name.front() == 'U' && isHex(name.substr(1))) {
        std::string encoded { encodeUTF8(static_cast<char32_t>(std::stoul(std::string(name.substr(1)), nullptr, 16))) };
        if (!encoded.empty()) {
            return encoded;
        }
    }
    return std::nullopt;
}

void skipSpaces(std::string_view &text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) {
        text.remove_prefix(1);
    }
}

// Reads a "quoted string" from the start of text handling \" \\ \n \t \r, octal (\123) and hex (\x41) escapes.
std::optional<std::string> parseQuoted(std::string_view &text) {
    if (!text.starts_with('"')) {
        return std::nullopt;
    }

    std::string result {};
    size_t i { 1 };

    while (i < text.size()) {
        char c { text[i] };

        if (c == '"') {
            text.remove_prefix(i + 1);
            return result;
        }

        if (c != '\\' || i + 1 >= text.size()) {
            result += c;
            ++i;
            continue;
        }

        char escaped { text[i + 1] };
        i += 2;

        if (escaped >= '0' && escaped <= '7') {
            int value { escaped - '0' };
            for (int digits { 1 }; digits < 3 && i < text.size() && text[i] >= '0' && text[i] <= '7'; ++digits, ++i) {
                value = value * 8 + (text[i] - '0');
            }
            result += static_cast<char>(value);
        } else if ((escaped == 'x' || escaped == 'X') && i < text.size() && std::isxdigit(static_cast<unsigned char>(text[i]))) {
            size_t length { (i + 1 < text.size() && std::isxdigit(static_cast<unsigned char>(text[i + 1]))) ? 2ul : 1ul };
            result += static_cast<char>(std::stoi(std::string(text.substr(i, length)), nullptr, 16));
            i += length;
        } else if (escaped == 'n') {
            result += '\n';
        } else if (escaped == 'r') {
            result += '\r';
        } else if (escaped == 't') {
            result += '\t';
        } else {
            // \" \\ and anything else is taken literally
            result += escaped;
        }
    }

    // never found the closing quote
    return std::nullopt;
}

// Parses a line like: <Multi_key> <minus> <space> : "~" asciitilde # TILDE
std::optional<ComposeSequence> parseSequenceLine(std::string_view line) {
    std::vector<std::string_view> keysyms {};

    while (line.starts_with('<')) {
        size_t close { line.find('>') };
        if (close == std::string_view::npos) {
            return std::nullopt;
        }
        keysyms.push_back(line.substr(1, close - 1));
        line.remove_prefix(close + 1);
        skipSpaces(line);
    }

    if (keysyms.empty() || !line.starts_with(':')) {
        return std::nullopt;
    }
    line.remove_prefix(1);
    skipSpaces(line);

    // we only care about Multi_key cases (compose key), we have no way to start a dead key sequence
    if (keysyms.size() < 2 || keysyms.front() != "Multi_key") {
        return std::nullopt;
    }

    ComposeSequence sequence {};

    for (std::string_view keysym : keysyms | std::views::drop(1)) {
        std::optional<std::string> token { resolveKeysym(keysym) };
        // pressing the compose key again restarts composition, and unknown keys can't be typed
        if (keysym == "Multi_key" || !token) {
            return std::nullopt;
        }
        sequence.listOfTokens.push_back(*token);
    }

    if (line.starts_with('"')) {
        std::optional<std::string> output { parseQuoted(line) };
        if (!output) {
            return std::nullopt;
        }
        sequence.composedChar = *output;
    } else {
        // the output can also be just a keysym, e.g. `: Cyrillic_a`
        std::optional<std::string> output { resolveKeysym(line.substr(0, line.find_first_of(" \t#"))) };
        if (!output) {
            return std::nullopt;
        }
        sequence.composedChar = *output;
    }

    if (sequence.composedChar.empty()) {
        return std::nullopt;
    }
    return sequence;
}

std::filesystem::path homeDirectory() {
    // HOME points inside the sandbox container, we want the real one
    if (const passwd *user { getpwuid(getuid()) }; user && user->pw_dir) {
        return user->pw_dir;
    }
    return std::getenv("HOME") ? std::getenv("HOME") : "";
}

void parseComposeFile(const std::filesystem::path &filePath, std::vector<ComposeSequence> &listOfSequences, int depth) {
    // stops include loops
    if (depth > 8) {
        return;
    }

    std::ifstream composerFile { filePath };

    if (!composerFile) {
        std::cerr << "Uh oh, compose file could not be opened for reading: " << filePath << "\n";
        return;
    }

    std::string composerRead {};

    while (std::getline(composerFile, composerRead)) {
        std::string_view line { composerRead };
        skipSpaces(line);

        if (line.starts_with("include")) {
            line.remove_prefix(7);
            skipSpaces(line);

            std::optional<std::string> includePath { parseQuoted(line) };
            if (!includePath) {
                continue;
            }

            // %L / %S point to the system Compose file, which is our bundled Compose.pre.
            // That one is always loaded as the base layer, so there's nothing to include.
            if (includePath->contains("%L") || includePath->contains("%S")) {
                continue;
            }

            if (size_t home { includePath->find("%H") }; home != std::string::npos) {
                includePath->replace(home, 2, homeDirectory().string());
            }

            std::filesystem::path resolvedPath { *includePath };
            if (resolvedPath.is_relative()) {
                resolvedPath = filePath.parent_path() / resolvedPath;
            }

            parseComposeFile(resolvedPath, listOfSequences, depth + 1);
            continue;
        }

        if (std::optional<ComposeSequence> sequence { parseSequenceLine(line) }) {
            listOfSequences.push_back(std::move(*sequence));
        }
    }
}

void insertSequence(const ComposeSequence &sequence) {
    if (sequence.listOfTokens.empty() || sequence.composedChar.empty()) {
        return;
    }

    ComposeNode *node { &sequenceTree };

    for (const std::string &token : sequence.listOfTokens) {
        // A shorter sequence ending here would make this one unreachable, newest definition wins (same as X11)
        node->composedChar.reset();

        std::unique_ptr<ComposeNode> &child { node->children[token] };
        if (!child) {
            child = std::make_unique<ComposeNode>();
        }
        node = child.get();
    }

    // Same the other way around, longer sequences starting with this one are replaced
    node->children.clear();
    node->composedChar = sequence.composedChar;
}

void rebuildSequenceTree() {
    sequenceTree = ComposeNode {};
    for (SequenceLayer layer : layerOrder) {
        for (const ComposeSequence &sequence : sequenceLayers[layer]) {
            insertSequence(sequence);
        }
    }
}

}

std::vector<ComposeSequence> Composer::getCharSequences(const std::string &filePath) {
    std::vector<ComposeSequence> listOfSequences {};
    parseComposeFile(filePath, listOfSequences, 0);
    return listOfSequences;
}

ComposeResult Composer::lookUpForChar(const std::vector<std::string> &inputs) {
    const ComposeNode *node { &sequenceTree };

    // walk down the tree one key at a time, if a key has no node no sequence can ever match
    for (const std::string &input : inputs) {
        auto found { node->children.find(input) };
        if (found == node->children.end()) {
            return { ComposeStatus::NoMatch, {} };
        }
        node = found->second.get();
    }

    if (node->composedChar) {
        return { ComposeStatus::Composed, *node->composedChar };
    }

    if (!node->children.empty()) {
        return { ComposeStatus::Pending, {} };
    }

    return { ComposeStatus::NoMatch, {} };
}

void Composer::setSequences(SequenceLayer layer, const std::vector<ComposeSequence> &listOfSequences) {
    sequenceLayers[layer] = listOfSequences;
    rebuildSequenceTree();
}

void Composer::clearSequences(SequenceLayer layer) {
    sequenceLayers.erase(layer);
    rebuildSequenceTree();
}

void Composer::setSavedSequences(const std::vector<ComposeSequence> &listOfSequences){
    setSequences(SequenceLayer::Default, listOfSequences);
}

// Adds a sequence typed in the GUI, tokens can be characters or keysym names
void Composer::addSequence(const ComposeSequence &sequence) {
    ComposeSequence resolved {};
    resolved.composedChar = sequence.composedChar;

    for (const std::string &token : sequence.listOfTokens) {
        std::optional<std::string> resolvedToken { resolveToken(token) };
        if (!resolvedToken) {
            std::cerr << "Skipping custom sequence, unknown token: " << token << "\n";
            return;
        }
        resolved.listOfTokens.push_back(*resolvedToken);
    }

    sequenceLayers[SequenceLayer::Custom].push_back(resolved);
    // custom is the last layer, so it can go straight into the tree
    insertSequence(resolved);
}

std::vector<ComposeSequence> Composer::getSavedSequences() {
    std::vector<ComposeSequence> allSequences {};
    for (SequenceLayer layer : layerOrder) {
        std::ranges::copy(sequenceLayers[layer], std::back_inserter(allSequences));
    }
    return allSequences;
}

std::optional<std::string> Composer::resolveToken(const std::string &token) {
    if (token.size() > 2 && token.starts_with('<') && token.ends_with('>')) {
        return resolveKeysym(std::string_view(token).substr(1, token.size() - 2));
    }
    if (std::optional<std::string> keysym { resolveKeysym(token) }) {
        return keysym;
    }
    // plain characters like "=" or "é"
    if (!token.empty()) {
        return token;
    }
    return std::nullopt;
}
