#pragma once

#include "CoreMinimal.h"
#include "Misc/GeneratedTypeName.h"

OPENFORT_API DECLARE_LOG_CATEGORY_EXTERN(LogOpenfort, Log, All);

// Disable for shipping builds
#if NO_LOGGING
OPENFORT_API DECLARE_LOG_CATEGORY_EXTERN(LogClean, Log, All);
#else
// Direct implementation of the DECLARE_LOG_CATEGORY_EXTERN macro
OPENFORT_API extern struct FLogCategoryLogClean : public FLogCategory<ELogVerbosity::Log, ELogVerbosity::All>
{
  FORCEINLINE FLogCategoryLogClean() : FLogCategory(TEXT(""))
  {
  }
} LogClean;
#endif

#define OPENFORT_LOG_CLEAN(Format, ...) \
  UE_LOG(LogClean, Log, TEXT(Format), ##__VA_ARGS__)

// OPENFORT_API DECLARE_LOG_CATEGORY_EXTERN(LogFuncSig, Log, All);

#define OPENFORT_FUNCNAME __FUNCTION__
#if defined(_MSC_VER) && !defined(__clang__)
#define OPENFORT_FUNCSIG __FUNCSIG__
#else
#define OPENFORT_FUNCSIG __PRETTY_FUNCTION__
#endif

#define OPENFORT_LOG_FUNCSIG \
  UE_LOG(LogOpenfort, Log, TEXT("%s"), *FString(OPENFORT_FUNCSIG))

#define OPENFORT_LOG(Format, ...) \
  {UE_LOG(LogOpenfort, Log, TEXT(Format), ##__VA_ARGS__)}

#define OPENFORT_LOG_FUNC(Format, ...)                                           \
  {                                                                              \
    const FString _Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__);           \
    UE_LOG(LogOpenfort, Log, TEXT("%s: %s"), *FString(OPENFORT_FUNCNAME), *_Msg) \
  }

#define OPENFORT_DISPLAY(Format, ...) \
  {UE_LOG(LogOpenfort, Display, TEXT(Format), ##__VA_ARGS__)}

#define OPENFORT_VERBOSE(Format, ...) \
  {UE_LOG(LogOpenfort, Verbose, TEXT(Format), ##__VA_ARGS__)}

#define OPENFORT_VERBOSE_FUNC(Format, ...)                                   \
  {                                                                          \
    const FString _Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__);       \
    UE_LOG(LogOpenfort, Verbose, TEXT("%s: %s"), *FString(OPENFORT_FUNCSIG), \
           *_Msg)                                                            \
  }

#define OPENFORT_WARN(Format, ...) \
  {UE_LOG(LogOpenfort, Warning, TEXT(Format), ##__VA_ARGS__)}

#define OPENFORT_WARN_FUNC(Format, ...)                                       \
  {                                                                           \
    const FString _Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__);        \
    UE_LOG(LogOpenfort, Warning, TEXT("%s: %s"), *FString(OPENFORT_FUNCNAME), \
           *_Msg)                                                             \
  }

#define OPENFORT_ERR(Format, ...) \
  {UE_LOG(LogOpenfort, Error, TEXT(Format), ##__VA_ARGS__)}

#define OPENFORT_ERR_FUNC(Format, ...)                                      \
  {                                                                         \
    const FString _Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__);      \
    UE_LOG(LogOpenfort, Error, TEXT("%s: %s"), *FString(OPENFORT_FUNCNAME), \
           *_Msg)                                                           \
  }
