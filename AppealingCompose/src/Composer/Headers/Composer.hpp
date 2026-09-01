//
//  Composer.hpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#ifndef Composer_hpp
#define Composer_hpp

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Keyboard.hpp"

struct ComposeSequence {
    std::vector<std::string> listOfTokens {};
    std::string composedChar {};
};


const std::unordered_map<std::string, std::string> charactersMap {
    // Basic ASCII punctuation and symbols
    
    {"<space>", " "},
    {"<Tab>", "\t"},
    {"<BackSpace>", "\b"},
    {"<Return>", "\n"},
    {"<Escape>", "\u001B"},

    {"<exclam>", "!"},
    {"<quotedbl>", "\""},
    {"<numbersign>", "#"},
    {"<dollar>", "$"},
    {"<percent>", "%"},
    {"<ampersand>", "&"},
    {"<apostrophe>", "'"},
    {"<parenleft>", "("},
    {"<parenright>", ")"},
    {"<asterisk>", "*"},
    {"<plus>", "+"},
    {"<comma>", ","},
    {"<minus>", "-"},
    {"<period>", "."},
    {"<slash>", "/"},

    // More punctuation
    {"<colon>", ":"},
    {"<semicolon>", ";"},
    {"<less>", "<"},
    {"<equal>", "="},
    {"<greater>", ">"},
    {"<question>", "?"},
    {"<at>", "@"},

    // More ASCII symbols
    {"<bracketleft>", "["},
    {"<backslash>", "\\"},
    {"<bracketright>", "]"},
    {"<asciicircum>", "^"},
    {"<underscore>", "_"},
    {"<grave>", "`"},
    {"<braceleft>", "{"},
    {"<bar>", "|"},
    {"<braceright>", "}"},
    {"<asciitilde>", "~"},

    // Common named tokens
    {"<dead_acute>", "'"},
    {"<dead_grave>", "`"},
    {"<dead_circumflex>", "^"},
    {"<dead_tilde>", "~"},
    {"<dead_diaeresis>", "\u00A8"},
    {"<dead_cedilla>", "\u00B8"},

    // Currency and misc
    {"<EuroSign>", "\u20AC"},
    {"<cent>", "\u00A2"},
    {"<sterling>", "\u00A3"},
    {"<yen>", "\u00A5"},

    // Arrows
    {"<Left>", "\u2190"},
    {"<Up>", "\u2191"},
    {"<Right>", "\u2192"},
    {"<Down>", "\u2193"}
};


class Composer {
    public:
    static std::vector<::ComposeSequence> getCharSequences(const std::string &filePath);
    static std::string lookUpForChar(const std::vector<std::string> &inputs);
    static void setSavedSequences (const std::vector<ComposeSequence> &listOfSequences);
    static void addSequence(const ComposeSequence &sequence);
    static std::vector<ComposeSequence> getSavedSequences();
};

#endif /* Composer_hpp */

