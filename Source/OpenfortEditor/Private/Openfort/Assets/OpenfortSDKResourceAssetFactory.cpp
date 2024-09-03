
// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenfortSDKResourceAssetFactory.h"

#include "Openfort/Assets/OpenfortSDKResource.h"
#include "Openfort/Misc/OpenfortLogging.h"
#include "Misc/FileHelper.h"

UOpenfortSDKResourceAssetFactory::UOpenfortSDKResourceAssetFactory()
{
  Formats.Add(FString(TEXT("js; Openfort SDK Resource")));
  SupportedClass = UOpenfortSDKResource::StaticClass();
  bCreateNew = false;
  bEditorImport = true;
}

bool UOpenfortSDKResourceAssetFactory::FactoryCanImport(const FString &Filename)
{
  return FPaths::GetCleanFilename(Filename) == TEXT("index.js");
}

UObject *UOpenfortSDKResourceAssetFactory::FactoryCreateBinary(
    UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags,
    UObject *Context, const TCHAR *Type, const uint8 *&Buffer,
    const uint8 *BufferEnd, FFeedbackContext *Warn)
{
  UOpenfortSDKResource *Resource =
      NewObject<UOpenfortSDKResource>(InParent, InClass, InName, Flags);

  if (CurrentFilename.IsEmpty() || !FPaths::FileExists(CurrentFilename))
  {
    OPENFORT_LOG("Invalid input js file: %s", *CurrentFilename)
    return nullptr;
  }

  FFileHelper::LoadFileToString(Resource->Js, *CurrentFilename,
                                FFileHelper::EHashOptions::EnableVerify,
                                FILEREAD_NoFail);

  return Resource;
}
