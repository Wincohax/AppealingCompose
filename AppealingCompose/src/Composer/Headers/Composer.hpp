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

class Composer {
    public:
    static std::vector<::ComposeSequence> getCharSequences(std::string &filePath);
    static std::string lookUpForChar(const std::vector<std::string> &inputs, KeyboardState &state);
    static void setSavedSequences (const std::vector<ComposeSequence> &listOfSequences);
};

#endif /* Composer_hpp */
