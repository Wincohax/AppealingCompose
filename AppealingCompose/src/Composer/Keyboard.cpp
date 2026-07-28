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
 
 
 Gotta check why Nither command key works.

 */



static std::atomic<int> targetKeyCode { 0x3A };

void Keyboard::setComposeKeyCode(int keyCode) {
    targetKeyCode.store(keyCode);
}

int Keyboard::getComposeKeyCode() {
    return targetKeyCode.load();
}

// Gets the key state
bool Keyboard::composeKeyEvent() {
    return CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, (CGKeyCode)targetKeyCode.load());
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
void Keyboard::collector(const bool& keyRef, std::string &inputCharacter, KeyboardState &state) {
    if (keyRef) {
        state.registeredInputs.push_back(inputCharacter);
    }
    for (const std::string &x : state.registeredInputs) {
            std::cout << x << std::endl;
        }
}


