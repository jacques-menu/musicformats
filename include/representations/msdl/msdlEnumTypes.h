/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlEnumTypes___
#define ___msdlEnumTypes___

#include <string>
#include <list>
#include <map>

#include <ostream>

#include "smartpointer.h"


namespace MusicFormats
{

// user languages
//______________________________________________________________________________
enum class msdlUserLanguageKind {
  kUserLanguageEnglish, // MSDL default
  kUserLanguageFrench,
  kUserLanguageItalian,
  kUserLanguageGerman,
  kUserLanguageSpanish,
  kUserLanguageDutch
};

std::string msdlUserLanguageKindAsString (
  msdlUserLanguageKind languageKind);

std::ostream& operator << (std::ostream& os, const msdlUserLanguageKind& elt);

msdlUserLanguageKind msdlUserLanguageKindFromString (
  const std::string& theString);

extern std::map <std::string, msdlUserLanguageKind>
  gGlobalMsdlUserLanguageKindsMap;

std::string availableMsdlUserLanguageKinds (size_t namesListMaxLength);

void initializeMsdlUserLanguageKindsMap ();

// comments types
//______________________________________________________________________________
enum class msdlCommentsTypeKind {
  kCommentsTypePercent, // MSDL default
  kCommentsTypeStar
};

std::string msdlCommentsTypeKindAsString (
  msdlCommentsTypeKind languageKind);

std::ostream& operator << (std::ostream& os, const msdlCommentsTypeKind& elt);

msdlCommentsTypeKind msdlCommentsTypeKindFromString (
  const std::string& theString);

extern std::map <std::string, msdlCommentsTypeKind>
  gGlobalMsdlCommentsTypeKindsMap;

void initializeMsdlCommentsTypeKinds ();

std::string availableMsdlCommentsTypeKinds (size_t namesListMaxLength);

void initializeMsdlCommentsTypeKindsMap ();

// initialization
//______________________________________________________________________________
void initializeMSDLBasicTypes ();


}


#endif // ___msdlEnumTypes___
