/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlKeywords___
#define ___msdlKeywords___

#include <string>
#include <map>


namespace MusicFormats
{

//______________________________________________________________________________
// pre-declaration
enum class msdlTokenKind;

// the MSDL keywords languages
//______________________________________________________________________________
enum class msdlKeywordsLanguageKind {
  kKeywordsLanguage_UNKNOWN_,

  kKeywordsLanguageEnglish, // MSDL default
  kKeywordsLanguageFrench,
  kKeywordsLanguageItalian,
  kKeywordsLanguageGerman,
  kKeywordsLanguageSpanish,
  kKeywordsLanguageNederlands
};

std::string msdlKeywordsLanguageKindAsString (
  msdlKeywordsLanguageKind languageKind);

std::ostream& operator << (std::ostream& os, const msdlKeywordsLanguageKind& elt);

msdlKeywordsLanguageKind msdlKeywordsLanguageKindFromString (
  const std::string& theString);

extern std::map<std::string, msdlKeywordsLanguageKind>
  gGlobalMsdlKeywordsLanguageKindsMap;

std::string availableMsdlKeywordsLanguageKinds (size_t namesListMaxLength);

void initializeMsdlKeywordsLanguageKindsMap ();

// the MSDL keywords
//______________________________________________________________________________
enum class msdlKeywordKind {
  kKeyword_UNKNOWN_,

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

std::string msdlKeywordKindAsString (
  msdlKeywordKind keywordKind);

std::ostream& operator << (std::ostream& os, const msdlKeywordKind& elt);

std::string msdlKeywordKindAsMsdlString (
  msdlKeywordsLanguageKind languageKind,
  msdlKeywordKind          keywordKind);

msdlKeywordKind msdlKeywordKindFromTokenKind (
  msdlTokenKind tokenKind);

msdlKeywordKind msdlKeywordKindFromString (
  msdlKeywordsLanguageKind languageKind,
  const std::string&       theString);

std::string availableKeywordsInLanguage (
  msdlKeywordsLanguageKind keywordsLanguageKind,
  size_t                   namesListMaxLength);

extern std::map<msdlKeywordKind, std::string> gGlobalEnglishKeywordsNamesMap;
extern std::map<msdlKeywordKind, std::string> gGlobalFrenchKeywordsNamesMap;
extern std::map<msdlKeywordKind, std::string> gGlobalItalianKeywordsNamesMap;
extern std::map<msdlKeywordKind, std::string> gGlobalGermanKeywordsNamesMap;
extern std::map<msdlKeywordKind, std::string> gGlobalSpanishKeywordsNamesMap;
extern std::map<msdlKeywordKind, std::string> gGlobalDutchKeywordsNamesMap;

void initializeEnglishKeywordsNamesMap ();
void initializeFrenchKeywordsNamesMap ();
void initializeItalianKeywordsNamesMap ();
void initializeGermanKeywordsNamesMap ();
void initializeSpanishKeywordsNamesMap ();
void initializeDutchKeywordsNamesMap ();

std::string availableKeywordsLanguageKinds (size_t namesListMaxLength);

// initialization
//______________________________________________________________________________
void initializeMSDLKeywords ();


}


#endif // ___msdlKeywords___
