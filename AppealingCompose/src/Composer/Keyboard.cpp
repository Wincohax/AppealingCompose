//
//  Keyboard.cpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#include "Keyboard.hpp"




/*
 
 This was a sad attempt to get it working in singlethread, I'd rather not.
 
CGEventRef Keyboard::nativeKeyListener(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refCon){
    int64_t keyNumber { CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode) };
    
    
    
    
    return <#expression#>;
}
CFMachPortRef Keyboard::nativeKeyTapper() {
    // will branch it later so it depends on the key choosen
    CGEventMask mask { kCGEventFlagsChanged };
    
    return CGEventTapCreate(kCGHIDEventTap, kCGHeadInsertEventTap, kCGEventTapOptionListenOnly, mask, <#CGEventTapCallBack  _Nonnull callback#>, <#void * _Nullable userInfo#>) // option key 0x3A
}

 */



// Gets the key state
bool Keyboard::composeKeyEvent() {
    return CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, 0x3A);
}

void Keyboard::getKeyState(KeyboardState &state) {
        while (!state.wasPressed) {
            state.wasPressed = composeKeyEvent();
            if (state.wasPressed) {
                std::cout << "Pressed" << std::endl;
                return;
            }
        }
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

