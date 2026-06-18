//
//  Keyboard.cpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#include "Keyboard.hpp"

bool Keyboard::composeKeyEvent() {
    return CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, 0x3A); // option key 0x3A
}

// Gets the key state, problem is that macOS uses this key and intercepts it first but I'd fix it later.
bool Keyboard::getKeyState() {
    bool pressed {false} ;
    pressed = composeKeyEvent();
        if (pressed) {
            std::cout << "Pressed" << std::endl;
        }
    return pressed;
}


// Dumbest collector ever for now.
void Keyboard::collector(const bool& keyRef, char inputCharacter, KeyboardState &state) {
    if (keyRef) {
        state.registeredInputs.insert(inputCharacter);
    }
    for (const char x : state.registeredInputs) {
            std::cout << x << std::endl;
        }
}

