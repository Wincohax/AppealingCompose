//
//  ComposerAdapter.m
//  AppealingCompose
//
//  Created by Wincohax on 27/07/26.
//

#import "ComposerAdapter.h"
#include "Composer.hpp"
#include "Keyboard.hpp"

NS_ASSUME_NONNULL_BEGIN

@implementation ComposerAdapter

- (void) loadSequences:(NSString *) nativeFilePath{
    std::string convertedPath { [nativeFilePath UTF8String] };
    Composer::setSavedSequences(Composer::getCharSequences(convertedPath));
}

+ (void) loadSequences:(NSString *) nativeFilePath{
    std::string convertedPath { [nativeFilePath UTF8String] };
    Composer::setSavedSequences(Composer::getCharSequences(convertedPath));
}

+ (void) setComposeKeyCode:(int) keyCode {
    Keyboard::setComposeKeyCode(keyCode);
}

+ (int) getComposeKeyCode {
    return Keyboard::getComposeKeyCode();
}

+ (void) addCustomSequenceTokens:(NSArray<NSString *> *)tokens outputChar:(NSString *)outputChar {
    ComposeSequence seq;
    for (NSString *tok in tokens) {
        seq.listOfTokens.push_back([tok UTF8String]);
    }
    seq.composedChar = [outputChar UTF8String];
    Composer::addSequence(seq);
}

+ (NSArray<NSDictionary *> *) getLoadedSequences {
    NSMutableArray *result = [NSMutableArray array];
    auto seqs = Composer::getSavedSequences();
    for (const auto &seq : seqs) {
        NSMutableArray *tokens = [NSMutableArray array];
        for (const auto &tok : seq.listOfTokens) {
            [tokens addObject:[NSString stringWithUTF8String:tok.c_str()]];
        }
        NSString *outChar = [NSString stringWithUTF8String:seq.composedChar.c_str()];
        [result addObject:@{
            @"tokens": tokens,
            @"composedChar": outChar
        }];
    }
    return result;
}

@end



NS_ASSUME_NONNULL_END
