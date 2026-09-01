//
//  Keyboard.cpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#include "Keyboard.hpp"

void Keyboard::setComposeKeyCode(const int &keyCode) {
    KeyboardState::GetInstance()->targetKeyCode.store(keyCode);
}

int Keyboard::getComposeKeyCode() {
    return KeyboardState::GetInstance()->targetKeyCode.load();
}

// Dumbest collector ever for now.
void Keyboard::collector(const bool &keyRef, const std::string &inputCharacter) {
    KeyboardState *kbpointer { KeyboardState::GetInstance() };
    
    if (keyRef) {
        kbpointer->addInput(inputCharacter);
    }
    for (const std::string &x : kbpointer->registeredInputs) {
            std::cout << x << std::endl;
        }
}

/*

// check how to get the keyboard keycode from the event and why is not getting my keypresses anymore

CGEventRef Keyboard::captureKeyEvent(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *keyState){
    
    if (type == kCGEventTapDisabledByTimeout || type == kCGEventTapDisabledByUserInput){
        std::cout << "Event tap was disabled, re-enabling..." << std::endl;
        CGEventTapEnable(eventTapper, true);
        return event;
    }
    
    if (type != kCGEventKeyDown) {
            if (type != kCGEventKeyUp) {
                if (type != kCGEventFlagsChanged) {
                    return event;
                }
            }
        }
    
    
    long long keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    std::cout << keycode << std::endl;
    
    std::atomic<bool>* keyStatePtr { static_cast<std::atomic<bool>*>(keyState) };
    
    if (keycode == targetKeyCode){
        std::cout << "Pressed" << std::endl;
        keyStatePtr->store(true);
    }
    
    return event;
}

void Keyboard::startKeyWatch(){
    
    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown)
                          | CGEventMaskBit(kCGEventKeyUp)
                          | CGEventMaskBit(kCGEventFlagsChanged);
    
    CFMachPortRef eventTap { CGEventTapCreate(kCGHIDEventTap, kCGHeadInsertEventTap, kCGEventTapOptionListenOnly, eventMask, captureKeyEvent, &KeyboardState::GetInstance()->pressedKey) };
    
    if (!eventTap) {
        std::cerr << "Failed to create event tap. Check Accessibility permissions." << std::endl;
        return;
    }
    
    eventTapper = eventTap;
    
    CFRunLoopSourceRef runLoopSource { CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0) };
    
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
        
    CGEventTapEnable(eventTap, true);
    
    CFRunLoopRun();
      
}
 
 */

/*

// Gets the key state
bool Keyboard::composeKeyEvent() {
    return CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, (CGKeyCode)targetKeyCode.load());
}

void Keyboard::getKeyState(KeyboardState &state) {
    // gotta save some cpu time somehow
    while (!state.wasPressed.load()) {
        bool pressed = composeKeyEvent();
        if (pressed) {
            state.wasPressed.store(true);
            std::cout << "Pressed" << std::endl;
            return;
        }
    }
}

 */
