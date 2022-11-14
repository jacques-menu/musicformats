/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrPitchesNames___
#define ___msrPitchesNames___

#include <map>

#include "msrPitches.h"


namespace MusicFormats
{

//______________________________________________________________________________
// quarter tones pitches languages

enum class msrQuarterTonesPitchesLanguageKind {
  kQTPNederlands, kQTPCatalan, kQTPDeutsch, kQTPEnglish,
  kQTPEspanol, kQTPFrancais, kQTPItaliano, kQTPNorsk,
  kQTPPortugues, kQTPSuomi, kQTPSvenska, kQTPVlaams, kQTPArabic
};

EXP std::string msrQuarterTonesPitchesLanguageKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind);

std::ostream& operator << (std::ostream& os, const msrQuarterTonesPitchesLanguageKind& elt);

msrQuarterTonesPitchesLanguageKind msrQuarterTonesPitchesLanguageKindFromString (
  const std::string& theString);

EXP std::string msrDiatonicPitchKindAsStringInLanguage (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrDiatonicPitchKind               diatonicPitchKind);

EXP std::string msrQuarterTonesPitchKindAsStringInLanguage (
  msrQuarterTonesPitchKind           quarterTonesPitchKind,
  msrQuarterTonesPitchesLanguageKind languageKind);

EXP msrQuarterTonesPitchKind quarterTonesPitchKindFromString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  const std::string&            quarterTonesPitchName);

/* JMI
 msrSemiTonesPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);
  */

/* JMI
std::string semiTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);
*/

/* JMI
std::string msrQuarterTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrQuarterTonesPitchKind           quarterTonesPitchKind);
*/

std::string existingMsrQuarterTonesPitchesLanguageKinds (
  size_t namesListMaxLength);

extern std::map<std::string, msrQuarterTonesPitchesLanguageKind>&
  getQuarterTonesPitchesLanguageKindsMap ();

void initializeQuarterTonesPitchesLanguageKinds ();

std::string existingQuarterTonesPitchesLanguageKinds (
  size_t namesListMaxLength);

//______________________________________________________________________________
extern std::map<msrQuarterTonesPitchKind, std::string>& getNederlandsPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getCatalanPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getDeutschPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getEnglishPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getEspanolPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getFrancaisPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getItalianoPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getNorskPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getPortuguesPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getSuomiPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getSvenskaPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getVlaamsPitchesNamesMap ();
extern std::map<msrQuarterTonesPitchKind, std::string>& getArabicPitchesNamesMap ();

// initialization
//______________________________________________________________________________
void initializeMsrPitchesNames ();


}


#endif
