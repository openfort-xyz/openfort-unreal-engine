#include "OpenfortMac.h"
#include "Openfort/OpenfortOpenfortSDK.h"
#include "Openfort/OpenfortSubsystem.h"
#include "Engine/GameEngine.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

ASWebAuthenticationSession *_authSession;

@implementation OpenfortMac

- (instancetype)init
{
  self = [super init];
  return self;
}

+ (OpenfortMac *)instance
{
  static OpenfortMac *staticOpenfortMac;
  static dispatch_once_t once;
  dispatch_once(&once, ^{
    staticOpenfortMac = [[self alloc] init];
  });
  return staticOpenfortMac;
}

+ (UOpenfortOpenfortSDK *)getOpenfortSDK
{
  UWorld *World = nullptr;

#if WITH_EDITOR
  if (GEditor)
  {
    for (const auto &Context : GEditor->GetWorldContexts())
    {
      if (Context.WorldType == EWorldType::PIE && Context.World())
      {
        World = Context.World();
        break;
      }
    }
  }
#else
  if (UGameEngine *GameEngine = Cast<UGameEngine>(GEngine))
  {
    World = GameEngine->GetGameWorld();
  }
#endif

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

- (void)launchUrl:(const char *)url forRedirectUri:(const char *)redirectUri
{
  if (@available(macOS 10.15, *))
  {
    NSURL *URL =
        [NSURL URLWithString:[[NSString alloc] initWithUTF8String:url]];

    // Bundle identifier does not work like iOS, so using callback URL scheme
    // from redirect URI instead
    NSString *redirectUriString =
        [[NSString alloc] initWithUTF8String:redirectUri];
    NSString *callbackURLScheme =
        [[redirectUriString componentsSeparatedByString:@":"] objectAtIndex:0];

    ASWebAuthenticationSession *authSession =
        [[ASWebAuthenticationSession alloc]
                  initWithURL:URL
            callbackURLScheme:callbackURLScheme
            completionHandler:^(NSURL *_Nullable redirect_uri,
                                NSError *_Nullable error) {
              _authSession = nil;

              if (redirect_uri)
              {
                UOpenfortOpenfortSDK *openfortSDK = [OpenfortMac getOpenfortSDK];

                if (openfortSDK)
                {
                  openfortSDK->HandleDeepLink(redirect_uri.absoluteString);
                }
              }
              else
              {
                OPENFORT_ERR("redirect_uri is empty");
                return;
              }
            }];

    _authSession = authSession;
    _authSession.presentationContextProvider = self;
    [_authSession start];
  }
}

- (ASPresentationAnchor)presentationAnchorForWebAuthenticationSession:
    (ASWebAuthenticationSession *)session API_AVAILABLE(macos(10.15))
{
  return [[[NSApplication sharedApplication] windows] firstObject];
}

@end
