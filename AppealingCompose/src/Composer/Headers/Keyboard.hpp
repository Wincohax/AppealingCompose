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

#include "KeyboardState.h"

class Keyboard {
    public:
    // legacy behavior
  //  static bool composeKeyEvent();
    
  //  static void getKeyState();
    static void collector(const bool &keyRef, const std::string &inputCharacter);
    static void setComposeKeyCode(const int &keyCode);
    static int getComposeKeyCode();
};


#endif /* Keyboard_hpp */
