//
//  ComposerAdapter.m
//  AppealingCompose
//
//  Created by Wincohax on 27/07/26.
//

#import "ComposerAdapter.h"
#include "Composer.hpp"

NS_ASSUME_NONNULL_BEGIN

@implementation ComposerAdapter

- (void) loadSequences:(NSString *) nativeFilePath{
    std::string convertedPath { [nativeFilePath UTF8String] };
    Composer::setSavedSequences(Composer::getCharSequences(convertedPath));
}

@end



NS_ASSUME_NONNULL_END
