//
//  Composer.cpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#include "Composer.hpp"

// Rock Bottom implementation, it covers 99% of my use case but will expand later to the Wayland/X11 amount of chars.
std::string_view Composer::composer(const std::unordered_set<char> &inputs, KeyboardState &state) {
    
    if (inputs.contains('A') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "Á";
    }
    if (inputs.contains('E') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "É";
    }
    if (inputs.contains('I') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "Í";
    }
    if (inputs.contains('O') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "Ó";
    }
    if (inputs.contains('U') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "Ú";
    }
    if (inputs.contains('N') && inputs.contains(*"~")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "Ñ";
    }
    if (inputs.contains('a') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "á";
    }
    if (inputs.contains('e') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "é";
    }
    if (inputs.contains('i') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "í";
    }
    if (inputs.contains('o') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "ó";
    }
    if (inputs.contains('u') && inputs.contains(*"'")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "ú";
    }
    if (inputs.contains('n') && inputs.contains(*"~")) {
        state.registeredInputs.clear();
        state.tries = 0;
        return "ñ";
    }
    state.tries++;
    
    if (state.tries >= 2){
        state.tries = 0;
        state.registeredInputs.clear();
        return "stop";
    }
    
    return "notfound";
}
