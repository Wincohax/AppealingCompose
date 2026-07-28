//
//  Keyboard.hpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#ifndef Keyboard_hpp
#define Keyboard_hpp

#include <stdio.h>
#include <thread>
#include <iostream>
#include <unordered_set>
#include <functional>
#include <ApplicationServices/ApplicationServices.h>

struct KeyboardState {
    std::vector<std::string> registeredInputs {};
    std::atomic<bool> wasPressed { false };
    int tries { 0 };
};

class Keyboard {
    public:
    //static CFMachPortRef nativeKeyTapper();
   // static CGEventRef nativeKeyListener(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refCon);
    static bool composeKeyEvent();
    static void getKeyState(KeyboardState &state);
    static void collector(const bool& keyRef, std::string &inputCharacter, KeyboardState &state);
    static void setComposeKeyCode(int keyCode);
    static int getComposeKeyCode();
};


#endif /* Keyboard_hpp */
