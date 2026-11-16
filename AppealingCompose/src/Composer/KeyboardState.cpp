//
//  KeyboardState.cpp
//  AppealingCompose
//
//  Created by Wincohax on 25/08/26.
//

#include "KeyboardState.h"

KeyboardState* KeyboardState::pinstance_{nullptr};
std::mutex KeyboardState::mutex_;
std::atomic<int> KeyboardState::targetKeyCode { 0x3A };

KeyboardState *KeyboardState::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new KeyboardState();
    }
    return pinstance_;
}


std::atomic<bool> KeyboardState::getKeyStatus(){
    KeyboardState *kbpointer { KeyboardState::GetInstance() };
    return kbpointer->pressedKey.load();
}

void KeyboardState::addInput(const std::string &input){
    KeyboardState *kbpointer { KeyboardState::GetInstance() };
    kbpointer->registeredInputs.push_back(input);
}

void KeyboardState::clearInputs(){
    KeyboardState *kbpointer { KeyboardState::GetInstance() };
    kbpointer->registeredInputs.clear();
}

void KeyboardState::turnOnKey(){
    KeyboardState *kbpointer { KeyboardState::GetInstance() };
    kbpointer->pressedKey.store(true);
}

void KeyboardState::turnOffKey(){
    KeyboardState *kbpointer { KeyboardState::GetInstance() };
    kbpointer->pressedKey.store(false);
}

