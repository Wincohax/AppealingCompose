//
//  Composer.hpp
//  AppealingCompose
//
//  Created by Wincohax on 12/06/26.
//

#ifndef Composer_hpp
#define Composer_hpp

#include <stdio.h>
#include <iostream>
#include <unordered_set>
#include "Keyboard.hpp"

class Composer {
    public:
    static std::string_view composer(const std::unordered_set<char> &inputs, KeyboardState &state);
};

#endif /* Composer_hpp */
