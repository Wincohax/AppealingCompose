//
//  InputServer.hpp
//  AppealingCompose
//
//  Created by Wincohax on 11/06/26.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <InputMethodKit/InputMethodKit.h>
#import <Carbon/Carbon.h>

#import "Keyboard.hpp"
#include "Composer.hpp"

NS_ASSUME_NONNULL_BEGIN


@interface InputServer : IMKInputController

- (void)commitComposition:(id)sender recieved:(std::string_view)charRecieved;
- (void)activateServer:(id)sender;
- (BOOL) checkComposeKeyPress: (nonnull NSEvent*) keyEvent;
- (void) stopComposing;
- (NSUInteger) recognizedEvents:(id) sender;
- (BOOL) handleEvent:(NSEvent *) event client:(id) sender;

@end

NS_ASSUME_NONNULL_END
