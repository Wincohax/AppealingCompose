//
//  InputServer.mm
//  AppealingCompose
//
//  Created by Wincohax on 11/06/26.
//

#import "InputServer.hpp"

NS_ASSUME_NONNULL_BEGIN

/*
 
 This was insane to make, after reading a lot of docs from Chinese authors and smashing my head to Apple docs I managed to crack the code.
 I wanna write this to advice anyone new who's trying to use the Apple IMK but doesn't know how to start.
 
 This file only handles the IMK interaction Logic, u must create this file and @interface InputServer : IMKInputController
 so you can interact with it. this works by overritting the base methods and adding your own logic or behavior to them.
 
 based on your use case inputText might be better but most of the things are not really clear in Apple Docs so
 try and smash your head to the wall until you find smth useful.
 
 */


@implementation InputServer

- (void) commitComposition:(id)sender recieved:(std::string_view)charRecieved
{
    
    // Takes the string of the composer and makes it an NSString so we can use it.
    NSString *convertedChar = [NSString stringWithUTF8String:charRecieved.data()];
    
    // sends the new char 
    [sender insertText:convertedChar replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
    
    
    KeyboardState::clearInputs();
    
   // threadState.store(false);
    
  //  [self callKeyListener];
    
}

// Everything good now.

/*
- (void) callKeyListener {
     if (!threadState){
        threadState.store(true);
        std::thread keyboardThread(&Keyboard::startKeyWatch, std::ref(kbState));
        keyboardThread.detach();
        return;
    }
}
 */


// both methods tells which window is currently on use, I'd rather not use them to do things as it can hang the window based on what u do.
- (void) activateServer:(id)sender
{
    
    NSLog(@"Server activated for %@", [sender bundleIdentifier]);
  
}

- (void) deactivateServer:(id)sender
{
    NSLog(@"Server deactivated for %@", [sender bundleIdentifier]);
}


- (void) checkComposeKeyPress: (nonnull NSEvent*) keyEvent {
    NSLog(@"code pressed: %hu", keyEvent.keyCode);
    
    if (keyEvent.keyCode == KeyboardState::GetInstance()->targetKeyCode.load()){
            KeyboardState::GetInstance()->turnOnKey();
            return;
    }
}


/*
- (BOOL)inputText:(NSString*)string client:(id)sender
{
    if (keypress){
        NSLog(@"Controller got: %@", string);
        return YES;
    }
    return NO;
}
 */


// Handles every input event it happens on the system, when it returns NO it doesn't intercept the keypress. when it
// returns YES it will intercept it.

- (BOOL) handleEvent:(NSEvent *) event client:(id) sender{
    
    KeyboardState *kbpointer { KeyboardState::GetInstance() };
    
    [self checkComposeKeyPress:event];
    
    if (event.type == NSEventTypeFlagsChanged){
        return NO;
    }
    
    // First we check if our key is pressed, state is def in the h file.
    if (!KeyboardState::getKeyStatus()){
        return NO;
    }
    
    // We get the char from it
    NSString *character = [event characters];
    NSLog(@"Controller got: %@", [event characters]);
    
    //We convert it to a C++ string and get only the char we need.
    std::string characterTyped {[character UTF8String]};
    
    std::cout << "THIS IS: " << characterTyped << std::endl;
    
    // we send it to the collector
    Keyboard::collector(KeyboardState::getKeyStatus(), characterTyped);
    
    // we save our result
    std::string result {Composer::lookUpForChar(kbpointer->registeredInputs)};
    
    std::cout << result;
    
    // if our composer says that no match was found.
    if (result == "Stop"){
        KeyboardState::turnOffKey();
      //  threadState.store(false);
       // [self callKeyListener];
        return YES;
    }
    
    // if our result is smth we expect we commit the comp and reset the key state.
    if (result != "NotFound"){
        KeyboardState::turnOffKey();
        [self commitComposition:sender recieved:result];
        return YES;
    }
    
    
    
    
    // NSString *convertedChar = [NSString stringWithUTF8String:Composer::composer(state.registeredInputs, state).data()];
    //NSLog(@"wtf: %@", convertedChar); IT WORKED LMAO
    // std::cout << Composer::composer(state.registeredInputs, state);
    
    return YES;
}


// Recognizes each keypress and (MOST) Modifier Keys
- (NSUInteger) recognizedEvents:(nonnull id)sender {
    return NSEventMaskKeyDown | NSEventMaskFlagsChanged;
}

@end


NS_ASSUME_NONNULL_END
