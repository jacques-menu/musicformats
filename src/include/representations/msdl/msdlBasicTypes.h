/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlBasicTypes___
#define ___msdlBasicTypes___

#include <list>
#include <map>

#include "smartpointer.h"

#include "rational.h"

#include "msrBasicTypes.h"


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

string msdlUserLanguageKindAsString (
  msdlUserLanguageKind languageKind);

ostream& operator<< (ostream& os, msdlUserLanguageKind& elt);

msdlUserLanguageKind msdlUserLanguageKindFromString (
  const string& theString);

extern map<string, msdlUserLanguageKind>
  gGlobalMsdlUserLanguageKindsMap;

string existingMsdlUserLanguageKinds (size_t namesListMaxLength);

void initializeMsdlUserLanguageKindsMap ();

// comments types
//______________________________________________________________________________
enum class msdlCommentsTypeKind {
  kCommentsTypePercent, // MSDL default
  kCommentsTypeStar
};

string msdlCommentsTypeKindAsString (
  msdlCommentsTypeKind languageKind);

ostream& operator<< (ostream& os, msdlCommentsTypeKind& elt);

msdlCommentsTypeKind msdlCommentsTypeKindFromString (
  const string& theString);

extern map<string, msdlCommentsTypeKind>
  gGlobalMsdlCommentsTypeKindsMap;

void initializeMsdlCommentsTypeKinds ();

string existingMsdlCommentsTypeKinds (size_t namesListMaxLength);

void initializeMsdlCommentsTypeKindsMap ();

// initialization
//______________________________________________________________________________
void initializeMSDLBasicTypes ();


} // namespace MusicFormats


#endif
