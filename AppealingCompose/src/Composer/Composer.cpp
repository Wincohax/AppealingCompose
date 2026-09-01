//
//  Composer.cpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#include "Composer.hpp"

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

 */


// const std::vector<std::string> &inputs

std::vector<ComposeSequence> savedSequences {};
unsigned long maxSizeOfSequence {};

std::vector<ComposeSequence> Composer::getCharSequences(const std::string &filePath) {

    std::vector<ComposeSequence> listOfSequences {};
    ComposeSequence charSequence {};

    // optimize later, load once and get our tokens.
    std::ifstream composerFile{ filePath };

    if (!composerFile)
    {
        // Print an error and exit
        std::cerr << "Uh oh, compose file could not be opened for reading!\n";
        return listOfSequences;
    }

    std::regex matcher("<([^>]+)>|\"([^\"]+)\"");
    std::smatch matches;

    std::string composerRead {};
    std::vector<std::string> composerTokens {};

    bool lifeBeforeCompose { false };

    // In the current Line of the file we are parsing
    while (std::getline(composerFile, composerRead)) {
        lifeBeforeCompose = false;
        // we only care about Multi_Key cases (compose key)
        if (composerRead.contains("<Multi_key>")) {

            auto match_begin { std::sregex_iterator(composerRead.begin(), composerRead.end(), matcher) };
            auto match_end { std::sregex_iterator() };

            for (std::sregex_iterator &i = match_begin; i != match_end; ++i) {
                const std::smatch& match = *i;
                std::string match_str = match.str();
                // if the compose key is not the first thing to be pressed we have no way to tell the composing thus
                // we break the loop, and we don't add that line of tokens
                if ( match_str == "<Multi_key>" && !composerTokens.empty()) {
                    lifeBeforeCompose = true;
                    composerTokens.clear();
                    charSequence.listOfTokens.clear();
                    break;
                }
                // We don't need the compose key token
                if (match_str != "<Multi_key>") {
                    composerTokens.push_back(match_str);
                }
            }

            // goes to the next line if our line had smth before the compose key
            if (lifeBeforeCompose) {
                continue;
            }

            // token cleaner
            for (std::string &token : composerTokens) {
                if (charactersMap.contains(token)) {
                    if (auto findToken = charactersMap.find(token); findToken != charactersMap.end()) {
                      charSequence.listOfTokens.push_back(findToken->second);
                    }
                    continue;
                }
                token.replace(0, 1, "");
                token.replace(token.size()-1, 1, "");
                charSequence.listOfTokens.push_back(token);
            }

            // Sequence Builder
            charSequence.composedChar = charSequence.listOfTokens.back();
            charSequence.listOfTokens.pop_back();
            listOfSequences.push_back(charSequence);
            
            // we save the max possible size of a sequence
            if (maxSizeOfSequence == 0 || maxSizeOfSequence < charSequence.listOfTokens.size()){
                maxSizeOfSequence = charSequence.listOfTokens.size();
            }
            

            // clear everything for the next line of tokens
            composerTokens.clear();
            charSequence.listOfTokens.clear();
        }
    }

    return listOfSequences;
}

std::string Composer::lookUpForChar(const std::vector<std::string> &inputs) {
    KeyboardState *kbpointer { KeyboardState::GetInstance() };
    
    
    bool hotCheck { false };
    
    for (const ComposeSequence &sequence : savedSequences) {
    // We make a hotCheck for the first ever char sent
    if (inputs.size() <= 1) {
        if (inputs.at(0) == sequence.listOfTokens.at(0)) {
            hotCheck = true;
        }
    }

    // we search if there's a match for each of the sequences we have
    auto found { std::ranges::search(sequence.listOfTokens, inputs).begin()};
        if (found != sequence.listOfTokens.end() && inputs.size() == sequence.listOfTokens.size()) {
                return sequence.composedChar;
        }
    }
    // fix bug with max limit of possible tokens
    if (!hotCheck && (inputs.size() <= 1 || inputs.size() >= maxSizeOfSequence)) {
        kbpointer->clearInputs();
        return "Stop";
    }

    return "NotFound";
}

void Composer::setSavedSequences(const std::vector<ComposeSequence> &listOfSequences){
    savedSequences = listOfSequences;
}

void Composer::addSequence(const ComposeSequence &sequence) {
    savedSequences.push_back(sequence);
    if (sequence.listOfTokens.size() > maxSizeOfSequence) {
        maxSizeOfSequence = sequence.listOfTokens.size();
    }
}

std::vector<ComposeSequence> Composer::getSavedSequences() {
    return savedSequences;
}

