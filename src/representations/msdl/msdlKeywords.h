/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlKeywords___
#define ___msdlKeywords___

#include <string>
#include <map>


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
enum class msdlTokenKind;

// the MSDL keywords languages
//______________________________________________________________________________
enum class msdlKeywordsLanguageKind {
  k_NoKeywordsLanguage,

  kKeywordsEnglish, // MSDL default
  kKeywordsFrench,
  kKeywordsItalian,
  kKeywordsGerman,
  kKeywordsSpanish,
  kKeywordsNederlands
};

string msdlKeywordsLanguageKindAsString (
  msdlKeywordsLanguageKind languageKind);

ostream& operator << (ostream& os, msdlKeywordsLanguageKind& elt);

msdlKeywordsLanguageKind msdlKeywordsLanguageKindFromString (
  const string& theString);

extern map<string, msdlKeywordsLanguageKind>
  gGlobalMsdlKeywordsLanguageKindsMap;

string existingMsdlKeywordsLanguageKinds (size_t namesListMaxLength);

void initializeMsdlKeywordsLanguageKindsMap ();

// the MSDL keywords
//______________________________________________________________________________
enum class msdlKeywordKind {
  k_NoKeywordKind,

  kKeywordTitle,
  kKeywordComposer,
  kKeywordOpus,

  kKeywordPitches,
  kKeywordOctaves,

  kKeywordAnacrusis,

  kKeywordBook,
  kKeywordScore,
  kKeywordPartGroup,
  kKeywordPart,
  kKeywordMusic,
  kKeywordFragment,

  kKeywordClef,
  kKeywordTreble,
  kKeywordSoprano,
  kKeywordAlto,
  kKeywordTenor,
  kKeywordBaryton,
  kKeywordBass,
  kKeywordTab,

  kKeywordKey,

  kKeywordTime
};

string msdlKeywordKindAsString (
  msdlKeywordKind keywordKind);

ostream& operator << (ostream& os, msdlKeywordKind& elt);

string msdlKeywordKindAsMsdlString (
  msdlKeywordsLanguageKind languageKind,
  msdlKeywordKind          keywordKind);

msdlKeywordKind msdlKeywordKindFromTokenKind (
  msdlTokenKind tokenKind);

msdlKeywordKind msdlKeywordKindFromString (
  msdlKeywordsLanguageKind languageKind,
  const string&            theString);

string existingKeywordsInLanguage (
  msdlKeywordsLanguageKind keywordsLanguageKind,
  size_t                   namesListMaxLength);

extern map<msdlKeywordKind, string> gGlobalEnglishKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalFrenchKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalItalianKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalGermanKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalSpanishKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalDutchKeywordsNamesMap;

void initializeEnglishKeywordsNamesMap ();
void initializeFrenchKeywordsNamesMap ();
void initializeItalianKeywordsNamesMap ();
void initializeGermanKeywordsNamesMap ();
void initializeSpanishKeywordsNamesMap ();
void initializeDutchKeywordsNamesMap ();

string existingKeywordsLanguageKinds (size_t namesListMaxLength);

// initialization
//______________________________________________________________________________
void initializeMSDLKeywords ();


}


#endif
