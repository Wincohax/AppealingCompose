//
//  KeyboardState.hpp
//  AppealingCompose
//
//  Created by Wincohax on 25/08/26.
//

#ifndef KeyboardState_hpp
#define KeyboardState_hpp

#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <atomic>


class KeyboardState {
private:
    static KeyboardState * pinstance_;
    static std::mutex mutex_;
    
protected:
    KeyboardState():
    registeredInputs {}, pressedKey { false }
    {
    }
    ~KeyboardState() {};
    
public:
    std::vector<std::string> registeredInputs;
    std::atomic<bool> pressedKey;
    static void turnOnKey();
    static void turnOffKey();
    static void addInput(const std::string &input);
    static void clearInputs();
    static std::atomic<bool> getKeyStatus();
    static std::atomic<int> targetKeyCode;
    
    KeyboardState(KeyboardState &other) = delete;
    void operator=(const KeyboardState &) = delete;
    static KeyboardState *GetInstance();
    
};

#endif /* KeyboardState_hpp */

