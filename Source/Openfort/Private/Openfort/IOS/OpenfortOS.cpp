#include "OpenfortIOS.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Engine/GameEngine.h"

API_AVAILABLE(ios(12.0))
ASWebAuthenticationSession *_authSession;

@implementation OpenfortIOS

- (instancetype)init
{
  self = [super init];
  return self;
}

+ (OpenfortIOS *)instance
{
  static OpenfortIOS *staticOpenfortIOS;
  static dispatch_once_t once;
  dispatch_once(&once, ^{
    staticOpenfortIOS = [[self alloc] init];
  });
  return staticOpenfortIOS;
}

+ (UOpenfortOpenfortSDK *)getOpenfortSDK
{
  UGameEngine *GameEngine = Cast<UGameEngine>(GEngine);

  if (!GameEngine)
  {
    return nil;
  }

  UWorld *World = GameEngine ? GameEngine->GetGameWorld() : NULL;

  if (!World)
  {
    return nil;
  }

  auto OpenfortSubsystem = World->GetGameInstance()->GetSubsystem<UOpenfortSubsystem>();

  if (!OpenfortSubsystem)
  {
    return nil;
  }

  auto OpenfortSDK = OpenfortSubsystem->GetOpenfortSDK();

  if (!OpenfortSDK.IsValid())
  {
    return nil;
  }

  return OpenfortSDK.Get();
}

- (void)launchUrl:(const char *)url
{
  NSURL *URL = [NSURL URLWithString:[[NSString alloc] initWithUTF8String:url]];
  NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];

  ASWebAuthenticationSession *authSession = [[ASWebAuthenticationSession alloc]
            initWithURL:URL
      callbackURLScheme:bundleIdentifier
      completionHandler:^(NSURL *_Nullable callbackURL,
                          NSError *_Nullable error) {
        _authSession = nil;
        if (callbackURL)
        {
          UOpenfortOpenfortSDK *openfortSDK = [OpenfortIOS getOpenfortSDK];

          if (openfortSDK)
          {
            openfortSDK->HandleDeepLink(callbackURL.absoluteString);
          }
        }
        else
        {
          return;
        }
      }];

  _authSession = authSession;
  _authSession.presentationContextProvider = (id)self;

  [_authSession start];
}

- (nonnull ASPresentationAnchor)presentationAnchorForWebAuthenticationSession:
    (nonnull ASWebAuthenticationSession *)session API_AVAILABLE(ios(13.0))
{
  return [[[UIApplication sharedApplication] windows] firstObject];
}

@end
