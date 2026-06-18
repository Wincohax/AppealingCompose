//
//  Keyboard.hpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#ifndef Keyboard_hpp
#define Keyboard_hpp

#include <stdio.h>
#include <iostream>
#include <unordered_set>
#include <ApplicationServices/ApplicationServices.h>

struct KeyboardState {
    std::unordered_set<char> registeredInputs {};
    bool wasPressed { false };
    int tries { 0 };
};

class Keyboard {
    public:
    static bool composeKeyEvent();
    static bool getKeyState();
    static void collector(const bool& keyRef, char inputCharacter, KeyboardState &state);
};


#endif /* Keyboard_hpp */
