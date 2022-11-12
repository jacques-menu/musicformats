/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlEnumTypes___
#define ___msdlEnumTypes___

#include <list>
#include <map>

#include "smartpointer.h"

#include "mfRational.h"


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

extern std::map<std::string, msdlUserLanguageKind>
  gGlobalMsdlUserLanguageKindsMap;

std::string existingMsdlUserLanguageKinds (size_t namesListMaxLength);

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

extern std::map<std::string, msdlCommentsTypeKind>
  gGlobalMsdlCommentsTypeKindsMap;

void initializeMsdlCommentsTypeKinds ();

std::string existingMsdlCommentsTypeKinds (size_t namesListMaxLength);

void initializeMsdlCommentsTypeKindsMap ();

// initialization
//______________________________________________________________________________
void initializeMSDLBasicTypes ();


}


#endif
