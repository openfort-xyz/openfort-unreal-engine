#pragma once

#import <AuthenticationServices/ASWebAuthenticationSession.h>
#import <Foundation/Foundation.h>

@interface OpenfortMac
    : NSObject <ASWebAuthenticationPresentationContextProviding>
+ (OpenfortMac *)instance;
- (void)launchUrl:(const char *)url forRedirectUri:(const char *)redirectUri;
@end
