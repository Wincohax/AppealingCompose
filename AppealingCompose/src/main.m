//
//  main.m
//  AppealingCompose
//
//  Created by Wincohax on 11/06/26.
//
#import <Cocoa/Cocoa.h>
#import <InputMethodKit/InputMethodKit.h>
#import "ComposerAdapter.h"
#import "AppealingCompose-Swift.h"

NSString* connectionName = @"AppealingCompose";
IMKServer* server;

int main(int argc, char * argv[]) {
    @autoreleasepool{
        server = [[IMKServer alloc] initWithName:(NSString*)connectionName bundleIdentifier:[[NSBundle mainBundle] bundleIdentifier]];
        NSString *ComposeFilePath = [[NSBundle mainBundle] pathForResource:@"Compose" ofType:@"pre"];
        ComposerAdapter *adapter = [[ComposerAdapter alloc] init];
        [adapter loadSequences:ComposeFilePath];
        
        [AppDelegateGUI initializeGUI];
        
        [[NSApplication sharedApplication] run];

  }
  return 0;
}
