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
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <optional>

#include "Keyboard.hpp"

struct ComposeSequence {
    std::vector<std::string> listOfTokens {};
    std::string composedChar {};
};

// What the composer thinks about the keys typed so far
enum class ComposeStatus {
    Composed,   // the inputs are a full sequence, composedChar holds the result
    Pending,    // the inputs are the start of at least one sequence, keep listening
    NoMatch     // no sequence starts with these inputs, stop composing
};

struct ComposeResult {
    ComposeStatus status { ComposeStatus::NoMatch };
    std::string composedChar {};
};

// Where a group of sequences came from. Later layers override earlier ones.
enum class SequenceLayer {
    Default,    // bundled Compose.pre
    UserFile,   // XCompose file imported by the user
    Custom      // combinations made in the GUI
};

class Composer {
    public:
    // Parses a Compose/XCompose file (following its include lines) into sequences
    static std::vector<::ComposeSequence> getCharSequences(const std::string &filePath);
    static ComposeResult lookUpForChar(const std::vector<std::string> &inputs);

    static void setSequences(SequenceLayer layer, const std::vector<ComposeSequence> &listOfSequences);
    static void clearSequences(SequenceLayer layer);
    static void setSavedSequences(const std::vector<ComposeSequence> &listOfSequences);
    static void addSequence(const ComposeSequence &sequence);
    static std::vector<ComposeSequence> getSavedSequences();

    // Turns a token like "<Cyrillic_a>", "minus", "U2192" or "é" into the text macOS sends for that key.
    // Returns nullopt when the token can't be typed.
    static std::optional<std::string> resolveToken(const std::string &token);
};

#endif /* Composer_hpp */
