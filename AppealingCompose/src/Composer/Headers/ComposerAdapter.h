//
//  ComposerAdapter.h
//  AppealingCompose
//
//  Created by Wincohax on 27/07/26.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ComposerAdapter : NSObject

- (void) loadSequences:(NSString *) nativeFilePath;
+ (void) loadSequences:(NSString *) nativeFilePath;
+ (void) setComposeKeyCode:(int) keyCode;
+ (int) getComposeKeyCode;
+ (void) addCustomSequenceTokens:(NSArray<NSString *> *)tokens outputChar:(NSString *)outputChar;
+ (void) clearCustomSequences;
+ (NSInteger) loadUserComposeFile:(NSString *) nativeFilePath;
+ (void) clearUserComposeFile;
+ (NSArray<NSDictionary *> *) getLoadedSequences;

@end

NS_ASSUME_NONNULL_END
