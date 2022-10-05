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

#include "msrBasicTypes.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
// quarter tones pitches languages

enum class msrQuarterTonesPitchesLanguageKind {
  kQTPNederlands, kQTPCatalan, kQTPDeutsch, kQTPEnglish,
  kQTPEspanol, kQTPFrancais, kQTPItaliano, kQTPNorsk,
  kQTPPortugues, kQTPSuomi, kQTPSvenska, kQTPVlaams, kQTPArabic
};

EXP string msrQuarterTonesPitchesLanguageKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind);

ostream& operator<< (ostream& os, msrQuarterTonesPitchesLanguageKind& elt);

msrQuarterTonesPitchesLanguageKind msrQuarterTonesPitchesLanguageKindFromString (
  const string& theString);

EXP string msrDiatonicPitchKindAsStringInLanguage (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrDiatonicPitchKind               diatonicPitchKind);

EXP string msrQuarterTonesPitchKindAsStringInLanguage (
  msrQuarterTonesPitchKind           quarterTonesPitchKind,
  msrQuarterTonesPitchesLanguageKind languageKind);

EXP msrQuarterTonesPitchKind quarterTonesPitchKindFromString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  const string&                      quarterTonesPitchName);

/* JMI
 msrSemiTonesPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);
  */

/* JMI
string semiTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);
*/

/* JMI
string msrQuarterTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrQuarterTonesPitchKind           quarterTonesPitchKind);
*/

string existingMsrQuarterTonesPitchesLanguageKinds (
  size_t namesListMaxLength);

extern map<string, msrQuarterTonesPitchesLanguageKind>&
  getQuarterTonesPitchesLanguageKindsMap ();

void initializeQuarterTonesPitchesLanguageKinds ();

string existingQuarterTonesPitchesLanguageKinds (
  size_t namesListMaxLength);

//______________________________________________________________________________
extern map<msrQuarterTonesPitchKind, string>& getNederlandsPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getCatalanPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getDeutschPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getEnglishPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getEspanolPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getFrancaisPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getItalianoPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getNorskPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getPortuguesPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getSuomiPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getSvenskaPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getVlaamsPitchesNamesMap ();
extern map<msrQuarterTonesPitchKind, string>& getArabicPitchesNamesMap ();

// initialization
//______________________________________________________________________________
void initializeMsrPitchesNames ();


} // namespace MusicFormats


#endif
