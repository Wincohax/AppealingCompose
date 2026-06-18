//
//  main.m
//  AppealingCompose
//
//  Created by Wincohax on 11/06/26.
//
#import <Cocoa/Cocoa.h>
#import <InputMethodKit/InputMethodKit.h>

NSString* connectionName = @"AppealingCompose";
IMKServer* server;

int main(int argc, char * argv[]) {
  @autoreleasepool{
    server = [[IMKServer alloc] initWithName:(NSString*)connectionName bundleIdentifier:[[NSBundle mainBundle] bundleIdentifier]];
      [[NSApplication sharedApplication] run];
  }
  return 0;
}
