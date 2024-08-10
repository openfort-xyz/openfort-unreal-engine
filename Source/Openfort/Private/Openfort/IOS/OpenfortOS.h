#pragma once

#import <AuthenticationServices/ASWebAuthenticationSession.h>
#import <Foundation/Foundation.h>

@interface OpenfortIOS
    : NSObject <ASWebAuthenticationPresentationContextProviding>
+ (OpenfortIOS *)instance;
- (void)launchUrl:(const char *)url;
@end