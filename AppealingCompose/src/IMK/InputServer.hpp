//
//  InputServer.h
//  AppealingCompose
//
//  Created by Wincohax on 11/06/26.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <InputMethodKit/InputMethodKit.h>

#import "Keyboard.hpp"
#include "Composer.hpp"

NS_ASSUME_NONNULL_BEGIN

KeyboardState kbState {};
std::atomic<bool> threadState { false };

@interface InputServer : IMKInputController

- (void)commitComposition:(id)sender recieved:(std::string_view)charRecieved;
- (void)activateServer:(id)sender;
- (BOOL) handleEvent:(NSEvent *) event client:(id) sender;

@end

NS_ASSUME_NONNULL_END
