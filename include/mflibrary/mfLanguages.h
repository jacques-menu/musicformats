/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfLanguages___
#define ___mfLanguages___

#include <string>
#include <map>


namespace MusicFormats
{

// the MusicFormats languages
//______________________________________________________________________________
enum class mfLanguageKind {
  kMusicFormatsLanguage_UNKNOWN,

  kMusicFormatsLanguageEnglish, // MusicFormats default
  kMusicFormatsLanguageFrench,
  kMusicFormatsLanguageItalian,
  kMusicFormatsLanguageGerman,
  kMusicFormatsLanguageSpanish,
  kMusicFormatsLanguageDutch
};

std::string mfLanguageKindAsString (
  mfLanguageKind languageKind);

std::ostream& operator << (std::ostream& os, const mfLanguageKind& elt);

mfLanguageKind mfLanguageKindFromString (
  const std::string& theString);

extern std::map<std::string, mfLanguageKind>
  gGlobalMusicFormatsLanguageKindsMap;

std::string availableMusicFormatsLanguageKinds (size_t namesListMaxLength);

void initializeMusicFormatsLanguageKindsMap ();

// initialization
//______________________________________________________________________________
void initializeMusicFormatsLanguages ();


}


#endif
