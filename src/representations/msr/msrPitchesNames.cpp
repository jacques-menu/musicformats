/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

#include <map>

#include "mfConstants.h"
#include "mfIndentedTextOutput.h"

#include "msrPitchesNames.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
// notes pitches languages
/* the reference for note pitches names is LilyPond's scm/define-note-names.scm */

std::map<std::string, msrQuarterTonesPitchesLanguageKind>
  pQuarterTonesPitchesLanguageKindsMap;

std::map<std::string, msrQuarterTonesPitchesLanguageKind>&
  getQuarterTonesPitchesLanguageKindsMap ()
{
  return pQuarterTonesPitchesLanguageKindsMap;
}

//______________________________________________________________________________
void initializeQuarterTonesPitchesLanguageKinds ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
    pQuarterTonesPitchesLanguageKindsMap ["nederlands"] =
      msrQuarterTonesPitchesLanguageKind::kQTPNederlands;
    pQuarterTonesPitchesLanguageKindsMap ["catalan"]    =
      msrQuarterTonesPitchesLanguageKind::kQTPCatalan;
    pQuarterTonesPitchesLanguageKindsMap ["deutsch"]    =
      msrQuarterTonesPitchesLanguageKind::kQTPDeutsch;
    pQuarterTonesPitchesLanguageKindsMap ["english"]    =
      msrQuarterTonesPitchesLanguageKind::kQTPEnglish;
    pQuarterTonesPitchesLanguageKindsMap ["espanol"]    =
      msrQuarterTonesPitchesLanguageKind::kQTPEspanol;
    pQuarterTonesPitchesLanguageKindsMap ["italiano"]   =
      msrQuarterTonesPitchesLanguageKind::kQTPItaliano;
    pQuarterTonesPitchesLanguageKindsMap ["francais"]   =
      msrQuarterTonesPitchesLanguageKind::kQTPFrancais;
    pQuarterTonesPitchesLanguageKindsMap ["norsk"]      =
      msrQuarterTonesPitchesLanguageKind::kQTPNorsk;
    pQuarterTonesPitchesLanguageKindsMap ["portugues"]  =
      msrQuarterTonesPitchesLanguageKind::kQTPPortugues;
    pQuarterTonesPitchesLanguageKindsMap ["suomi"]      =
      msrQuarterTonesPitchesLanguageKind::kQTPSuomi;
    pQuarterTonesPitchesLanguageKindsMap ["svenska"]    =
      msrQuarterTonesPitchesLanguageKind::kQTPSvenska;
    pQuarterTonesPitchesLanguageKindsMap ["vlaams"]     =
      msrQuarterTonesPitchesLanguageKind::kQTPVlaams;
    pQuarterTonesPitchesLanguageKindsMap ["arabic"]     =
      msrQuarterTonesPitchesLanguageKind::kQTPArabic;

    pPrivateThisMethodHasBeenRun = true;
  }
}

//______________________________________________________________________________
std::map<msrQuarterTonesPitchKind, std::string> pNederlandsPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pCatalanPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pDeutschPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pEnglishPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pEspanolPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pFrancaisPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pItalianoPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pNorskPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pPortuguesPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pSuomiPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pSvenskaPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pVlaamsPitchesNamesMap;
std::map<msrQuarterTonesPitchKind, std::string> pArabicPitchesNamesMap;

//______________________________________________________________________________
void initializeNederlandsPitchesNamesMap ()
{
  // nederlands
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "aeses";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "aeseh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "aes";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "aeh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "a";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "aih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "ais";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "aisih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "aisis";

  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "beses";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "beseh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "bes";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "beh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "b";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "bih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "bis";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "bisih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "bisis";

  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "ceses";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "ceseh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "ces";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "ceh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "c";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "cih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "cis";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "cisih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "cisis";

  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "deses";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "deseh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "des";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "deh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "d";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "dih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "dis";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "disih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "disis";

  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "eeses";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "eeseh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "ees";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "eeh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "e";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "eih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "eis";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "eisih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "eisis";

  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "feses";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "feseh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fes";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "feh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "f";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fis";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "fisih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fisis";

  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "geses";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "geseh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "ges";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "geh";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "g";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "gih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "gis";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "gisih";
  pNederlandsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "gisis";
}

//______________________________________________________________________________
void initializeCatalanPitchesNamesMap ()
{
  // catalan
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "labb";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "labSesquiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "lab";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "aSemiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "la";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "aSemiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "lad";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "laSesquiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "ladd";

  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "sibb";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "sibSesquiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "sib";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "bSemiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "b";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "bSemiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "sid";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "siSesquiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "sidd";

  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "dobb";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "doSesquiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "dob";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "cSemiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "do";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "cSemiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "dod";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "doSesquiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "dodd";

  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "rebb";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "reSesquiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "reb";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "dSemiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "re";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "dSemiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "red";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "reSesquiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "redd";

  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "mibb";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "miSesquiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "mib";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "eSemiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "mi";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "eSemiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "mid";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "miSesquiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "midd";

  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "fabb";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "faSesquiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fab";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "fSemiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "fa";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fSemiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fad";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "faSesquiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fadd";

  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "solbb";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "solSesquiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "solb";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "gSemiFlat???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "sol";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "gSemiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "sold";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "solSesquiSharp???";
  pCatalanPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "soldd";
}

//______________________________________________________________________________
void initializeDeutschPitchesNamesMap ()
{
  // deutsch
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "asas";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "asah";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "as";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "ah";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "a";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "aih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "ais";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "aisih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "aisis";

  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "heses";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "heseh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "b";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "beh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "h";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "hih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "his";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "hisih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "hisis";

  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "ceses";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "ceseh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "ces";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "ceh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "c";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "cih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "cis";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "cisih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "cisis";

  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "deses";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "deseh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "des";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "deh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "d";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "dih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "dis";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "disih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "disis";

  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "eses";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "esseh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "es";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "eh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "e";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "eih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "eis";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "eisih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "eisis";

  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "feses";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "feseh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fes";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "feh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "f";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fis";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "fisih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fisis";

  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "geses";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "geseh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "ges";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "geh";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "g";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "gih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "gis";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "gisih";
  pDeutschPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "gisis";
}

//______________________________________________________________________________
void initializeEnglishPitchesNamesMap ()
{
  // english
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "aff";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "atqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "af";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "aqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "a";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "aqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "as";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "atqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "ax";

  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "bfqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "btqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "bf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "bqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "b";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "bqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "bs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "btqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "bx";

  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "cff";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "ctqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "cf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "cqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "c";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "cqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "cs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "ctqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "cx";

  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "dff";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "dtqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "df";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "dqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "d";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "dqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "ds";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "dtqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "dx";

  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "eff";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "etqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "ef";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "eqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "e";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "eqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "es";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "etqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "ex";

  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "fff";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "ftqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "ff";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "fqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "f";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "ftqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fx";

  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "gff";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "gtqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "gf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "gqf";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "g";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "gqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "gs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "gtqs";
  pEnglishPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "gx";
}

//______________________________________________________________________________
void initializeEspanolPitchesNamesMap ()
{
  // espanol
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "labb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "latcb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "lab";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "lacb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "la";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "lacs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "las";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "latcs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "lax";

  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "sibb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "sitcb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "sib";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "sicb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "si";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "sics";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "sis";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "sitcs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "six";

  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "dobb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "dotcb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "dob";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "docb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "do";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "docs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "dos";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "dotcs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "dox";

  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "rebb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "retcb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "reb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "recb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "re";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "recs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "res";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "retcs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "rex";

  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "mibb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "mitcb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "mib";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "micb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "mi";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "mics";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "mis";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "mitcs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "mix";

  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "fabb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "fatcb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fab";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "facb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "fa";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "facs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fas";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "fatcs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fax";

  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "solbb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "soltcb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "solb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "solcb";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "sol";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "solcs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "sols";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "soltcs";
  pEspanolPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "solx";
}

//______________________________________________________________________________
void initializeFrancaisPitchesNamesMap ()
{
  // francais
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "labb";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "labtqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "lab";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "labqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "la";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "lasqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "lad";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "lastqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "lass";

  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "sibb";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "sibtqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "sib";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "sibqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "si";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "sisqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "sid";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "sistqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "siss";

  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "dobb";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "dobtqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "dob";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "dobqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "do";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "dosqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "dod";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "dostqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "doss";

  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "rebb";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "rebtqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "reb";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "rebqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "re";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "resqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "red";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "restqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "ress";

  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "mibb";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "mibtqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "mib";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "mibqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "mi";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "misqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "mid";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "mistqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "miss";

  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "fabb";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "fabtqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fab";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "fabqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "fa";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fasqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fad";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "fastqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fass";

  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "solbb";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "solbtqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "solb";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "solbqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "sol";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "solsqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "sold";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "solstqt";
  pFrancaisPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "solss";
}

//______________________________________________________________________________
void initializeItalianoPitchesNamesMap ()
{
  // italiano
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "labb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "labsb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "lab";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "lasb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "la";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "lasd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "lad";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "ladsd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "ladd";

  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "sibb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "sibsb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "sib";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "sisb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "si";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "sisd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "sid";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "sidsd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "sidd";

  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "dobb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "dobsb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "dob";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "dosb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "do";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "dosd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "dod";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "dodsd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "dodd";

  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "rebb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "rebsb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "reb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "resb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "re";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "resd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "red";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "redsd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "redd";

  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "mibb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "mibsb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "mib";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "misb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "mi";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "misd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "mid";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "midsd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "midd";

  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "fabb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "fabsb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fab";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "fasb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "fa";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fasd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fad";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "fadsd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fadd";

  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "solbb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "solbsb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "solb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "solsb";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "sol";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "solsd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "sold";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "soldsd";
  pItalianoPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "soldd";
}

//______________________________________________________________________________
void initializeNorskPitchesNamesMap ()
{
/*
    (norsk . (
              (cessess . ,(ly:make-pitch -1 0 DOUBLE-FLAT))
              (ceses . ,(ly:make-pitch -1 0 DOUBLE-FLAT))
              (cesseh . ,(ly:make-pitch -1 0 THREE-Q-FLAT))
              (ceseh . ,(ly:make-pitch -1 0 THREE-Q-FLAT))
              (cess . ,(ly:make-pitch -1 0 FLAT))
              (ces . ,(ly:make-pitch -1 0 FLAT))
              (ceh . ,(ly:make-pitch -1 0 SEMI-FLAT))
              (c . ,(ly:make-pitch -1 0 NATURAL))
              (cih . ,(ly:make-pitch -1 0 SEMI-SHARP))
              (ciss . ,(ly:make-pitch -1 0 SHARP))
              (cis . ,(ly:make-pitch -1 0 SHARP))
              (cissih . ,(ly:make-pitch -1 0 THREE-Q-SHARP))
              (cisih . ,(ly:make-pitch -1 0 THREE-Q-SHARP))
              (cississ . ,(ly:make-pitch -1 0 DOUBLE-SHARP))
              (cisis . ,(ly:make-pitch -1 0 DOUBLE-SHARP))

              (dessess . ,(ly:make-pitch -1 1 DOUBLE-FLAT))
              (deses . ,(ly:make-pitch -1 1 DOUBLE-FLAT))
              (desseh . ,(ly:make-pitch -1 1 THREE-Q-FLAT))
              (deseh . ,(ly:make-pitch -1 1 THREE-Q-FLAT))
              (dess . ,(ly:make-pitch -1 1 FLAT))
              (des . ,(ly:make-pitch -1 1 FLAT))
              (deh . ,(ly:make-pitch -1 1 SEMI-FLAT))
              (d . ,(ly:make-pitch -1 1 NATURAL))
              (dih . ,(ly:make-pitch -1 1 SEMI-SHARP))
              (diss . ,(ly:make-pitch -1 1 SHARP))
              (dis . ,(ly:make-pitch -1 1 SHARP))
              (dissih . ,(ly:make-pitch -1 1 THREE-Q-SHARP))
              (disih . ,(ly:make-pitch -1 1 THREE-Q-SHARP))
              (dississ . ,(ly:make-pitch -1 1 DOUBLE-SHARP))
              (disis . ,(ly:make-pitch -1 1 DOUBLE-SHARP))

              (eessess . ,(ly:make-pitch -1 2 DOUBLE-FLAT))
              (eeses . ,(ly:make-pitch -1 2 DOUBLE-FLAT))
              (essess . ,(ly:make-pitch -1 2 DOUBLE-FLAT))
              (eses . ,(ly:make-pitch -1 2 DOUBLE-FLAT))
              (eesseh . ,(ly:make-pitch -1 2 THREE-Q-FLAT))
              (eeseh . ,(ly:make-pitch -1 2 THREE-Q-FLAT))
              (eess . ,(ly:make-pitch -1 2 FLAT))
              (ees . ,(ly:make-pitch -1 2 FLAT))
              (ess . ,(ly:make-pitch -1 2 FLAT))
              (es . ,(ly:make-pitch -1 2 FLAT))
              (eeh . ,(ly:make-pitch -1 2 SEMI-FLAT))
              (e . ,(ly:make-pitch -1 2 NATURAL))
              (eih . ,(ly:make-pitch -1 2 SEMI-SHARP))
              (eiss . ,(ly:make-pitch -1 2 SHARP))
              (eis . ,(ly:make-pitch -1 2 SHARP))
              (eissih . ,(ly:make-pitch -1 2 THREE-Q-SHARP))
              (eisih . ,(ly:make-pitch -1 2 THREE-Q-SHARP))
              (eississ . ,(ly:make-pitch -1 2 DOUBLE-SHARP))
              (eisis . ,(ly:make-pitch -1 2 DOUBLE-SHARP))

              (fessess . ,(ly:make-pitch -1 3 DOUBLE-FLAT))
              (feses . ,(ly:make-pitch -1 3 DOUBLE-FLAT))
              (fesseh . ,(ly:make-pitch -1 3 THREE-Q-FLAT))
              (feseh . ,(ly:make-pitch -1 3 THREE-Q-FLAT))
              (fess . ,(ly:make-pitch -1 3 FLAT))
              (fes . ,(ly:make-pitch -1 3 FLAT))
              (feh . ,(ly:make-pitch -1 3 SEMI-FLAT))
              (f . ,(ly:make-pitch -1 3 NATURAL))
              (fih . ,(ly:make-pitch -1 3 SEMI-SHARP))
              (fiss . ,(ly:make-pitch -1 3 SHARP))
              (fis . ,(ly:make-pitch -1 3 SHARP))
              (fissih . ,(ly:make-pitch -1 3 THREE-Q-SHARP))
              (fisih . ,(ly:make-pitch -1 3 THREE-Q-SHARP))
              (fississ . ,(ly:make-pitch -1 3 DOUBLE-SHARP))
              (fisis . ,(ly:make-pitch -1 3 DOUBLE-SHARP))

              (gessess . ,(ly:make-pitch -1 4 DOUBLE-FLAT))
              (geses . ,(ly:make-pitch -1 4 DOUBLE-FLAT))
              (geseh . ,(ly:make-pitch -1 4 THREE-Q-FLAT))
              (gesseh . ,(ly:make-pitch -1 4 THREE-Q-FLAT))
              (gess . ,(ly:make-pitch -1 4 FLAT))
              (ges . ,(ly:make-pitch -1 4 FLAT))
              (geh . ,(ly:make-pitch -1 4 SEMI-FLAT))
              (g . ,(ly:make-pitch -1 4 NATURAL))
              (gih . ,(ly:make-pitch -1 4 SEMI-SHARP))
              (giss . ,(ly:make-pitch -1 4 SHARP))
              (gis . ,(ly:make-pitch -1 4 SHARP))
              (gissih . ,(ly:make-pitch -1 4 THREE-Q-SHARP))
              (gisih . ,(ly:make-pitch -1 4 THREE-Q-SHARP))
              (gississ . ,(ly:make-pitch -1 4 DOUBLE-SHARP))
              (gisis . ,(ly:make-pitch -1 4 DOUBLE-SHARP))

              (assess . ,(ly:make-pitch -1 5 DOUBLE-FLAT))
              (ases . ,(ly:make-pitch -1 5 DOUBLE-FLAT))
              (aessess . ,(ly:make-pitch -1 5 DOUBLE-FLAT))
              (aeses . ,(ly:make-pitch -1 5 DOUBLE-FLAT))
              (aesseh . ,(ly:make-pitch -1 5 THREE-Q-FLAT))
              (aeseh . ,(ly:make-pitch -1 5 THREE-Q-FLAT))
              (ass . ,(ly:make-pitch -1 5 FLAT))
              (as . ,(ly:make-pitch -1 5 FLAT))
              (aess . ,(ly:make-pitch -1 5 FLAT))
              (aes . ,(ly:make-pitch -1 5 FLAT))
              (aeh . ,(ly:make-pitch -1 5 SEMI-FLAT))
              (a . ,(ly:make-pitch -1 5 NATURAL))
              (aih . ,(ly:make-pitch -1 5 SEMI-SHARP))
              (aiss . ,(ly:make-pitch -1 5 SHARP))
              (ais . ,(ly:make-pitch -1 5 SHARP))
              (aissih . ,(ly:make-pitch -1 5 THREE-Q-SHARP))
              (aisih . ,(ly:make-pitch -1 5 THREE-Q-SHARP))
              (aississ . ,(ly:make-pitch -1 5 DOUBLE-SHARP))
              (aisis . ,(ly:make-pitch -1 5 DOUBLE-SHARP))

              (bess . ,(ly:make-pitch -1 6 DOUBLE-FLAT))
              (bes . ,(ly:make-pitch -1 6 DOUBLE-FLAT))
              (beh . ,(ly:make-pitch -1 6 THREE-Q-FLAT))
              (b . ,(ly:make-pitch -1 6 FLAT))
              (heh . ,(ly:make-pitch -1 6 SEMI-FLAT))
              (h . ,(ly:make-pitch -1 6 NATURAL))
              (hih . ,(ly:make-pitch -1 6 SEMI-SHARP))
              (hiss . ,(ly:make-pitch -1 6 SHARP))
              (his . ,(ly:make-pitch -1 6 SHARP))
              (hissih . ,(ly:make-pitch -1 6 THREE-Q-SHARP))
              (hisih . ,(ly:make-pitch -1 6 THREE-Q-SHARP))
              (hississ . ,(ly:make-pitch -1 6 DOUBLE-SHARP))
              (hisis . ,(ly:make-pitch -1 6 DOUBLE-SHARP))
              ))
*/

  // norsk
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "aeses";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "aSesquiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "aes";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "aSemiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "a";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "aSemiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "ais";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "aSesquiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "aisis";

  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "beses";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "bSesquiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "bes";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "bSemiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "b";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "bSemiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "bis";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "bSesquiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "bisis";

  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "ceses";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "cSesquiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "ces";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "cSemiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "c";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "cSemiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "cis";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "cSesquiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "cisis";

  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "deses";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "dSesquiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "des";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "dSemiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "d";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "dSemiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "dis";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "dSesquiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "disis";

  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "eeses";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "eSesquiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "ees";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "eSemiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "e";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "eSemiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "eis";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "eSesquiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "eisis";

  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "feses";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "fSesquiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fes";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "fSemiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "f";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fSemiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fis";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "fSesquiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fisis";

  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "geses";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "gSesquiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "ges";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "gSemiFlat???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "g";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "gSemiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "gis";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "gSesquiSharp???";
  pNorskPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "gisis";
}

//______________________________________________________________________________
void initializePortuguesPitchesNamesMap ()
{
  // portugues
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "labb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "labtqt";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "lab";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "lasb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "la";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "lasd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "lad";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "ladsd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "ladd";

  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "sibb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "sibtqt";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "sib";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "sisb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "si";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "sisd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "sid";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "sidsd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "sidd";

  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "dobb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "dobtqt";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "dob";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "dosb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "do";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "dosd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "dod";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "dodsd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "dodd";

  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "rebb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "rebtqt";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "reb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "resb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "re";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "resd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "red";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "redsd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "redd";

  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "mibb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "mibtqt";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "mib";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "misb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "mi";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "misd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "mid";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "midsd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "midd";

  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "fabb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "fabtqt";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fab";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "fasb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "fa";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fasd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fad";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "fadsd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fadd";

  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "solbb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "solbtqt";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "solb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "solsb";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "sol";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "solsd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "sold";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "soldsd";
  pPortuguesPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "soldd";
}

//______________________________________________________________________________
void initializeSuomiPitchesNamesMap ()
{
/*
    (suomi . (
              (ceses . ,(ly:make-pitch -1 0 DOUBLE-FLAT))
              (ceseh . ,(ly:make-pitch -1 0 THREE-Q-FLAT))
              (ces . ,(ly:make-pitch -1 0 FLAT))
              (ceh . ,(ly:make-pitch -1 0 SEMI-FLAT))
              (c . ,(ly:make-pitch -1 0 NATURAL))
              (cih . ,(ly:make-pitch -1 0 SEMI-SHARP))
              (cis . ,(ly:make-pitch -1 0 SHARP))
              (cisih . ,(ly:make-pitch -1 0 THREE-Q-SHARP))
              (cisis . ,(ly:make-pitch -1 0 DOUBLE-SHARP))

              (deses . ,(ly:make-pitch -1 1 DOUBLE-FLAT))
              (deseh . ,(ly:make-pitch -1 1 THREE-Q-FLAT))
              (des . ,(ly:make-pitch -1 1 FLAT))
              (deh . ,(ly:make-pitch -1 1 SEMI-FLAT))
              (d . ,(ly:make-pitch -1 1 NATURAL))
              (dih . ,(ly:make-pitch -1 1 SEMI-SHARP))
              (dis . ,(ly:make-pitch -1 1 SHARP))
              (disih . ,(ly:make-pitch -1 1 THREE-Q-SHARP))
              (disis . ,(ly:make-pitch -1 1 DOUBLE-SHARP))

              (eses . ,(ly:make-pitch -1 2 DOUBLE-FLAT))
              (eseh . ,(ly:make-pitch -1 2 THREE-Q-FLAT))
              (es . ,(ly:make-pitch -1 2 FLAT))
              (eeh . ,(ly:make-pitch -1 2 SEMI-FLAT))
              (e . ,(ly:make-pitch -1 2 NATURAL))
              (eih . ,(ly:make-pitch -1 2 SEMI-SHARP))
              (eis . ,(ly:make-pitch -1 2 SHARP))
              (eisih . ,(ly:make-pitch -1 2 THREE-Q-SHARP))
              (eisis . ,(ly:make-pitch -1 2 DOUBLE-SHARP))

              (feses . ,(ly:make-pitch -1 3 DOUBLE-FLAT))
              (feseh . ,(ly:make-pitch -1 3 THREE-Q-FLAT))
              (fes . ,(ly:make-pitch -1 3 FLAT))
              (feh . ,(ly:make-pitch -1 3 SEMI-FLAT))
              (f . ,(ly:make-pitch -1 3 NATURAL))
              (fih . ,(ly:make-pitch -1 3 SEMI-SHARP))
              (fis . ,(ly:make-pitch -1 3 SHARP))
              (fisih . ,(ly:make-pitch -1 3 THREE-Q-SHARP))
              (fisis . ,(ly:make-pitch -1 3 DOUBLE-SHARP))

              (geses . ,(ly:make-pitch -1 4 DOUBLE-FLAT))
              (geseh . ,(ly:make-pitch -1 4 THREE-Q-FLAT))
              (ges . ,(ly:make-pitch -1 4 FLAT))
              (geh . ,(ly:make-pitch -1 4 SEMI-FLAT))
              (g . ,(ly:make-pitch -1 4 NATURAL))
              (gih . ,(ly:make-pitch -1 4 SEMI-SHARP))
              (gis . ,(ly:make-pitch -1 4 SHARP))
              (gisih . ,(ly:make-pitch -1 4 THREE-Q-SHARP))
              (gisis . ,(ly:make-pitch -1 4 DOUBLE-SHARP))

              (asas . ,(ly:make-pitch -1 5 DOUBLE-FLAT))
              (ases . ,(ly:make-pitch -1 5 DOUBLE-FLAT))   ; non-standard name for asas
              (asah . ,(ly:make-pitch -1 5 THREE-Q-FLAT))
              (aseh . ,(ly:make-pitch -1 5 THREE-Q-FLAT))
              (as . ,(ly:make-pitch -1 5 FLAT))
              (aeh . ,(ly:make-pitch -1 5 SEMI-FLAT))
              (a . ,(ly:make-pitch -1 5 NATURAL))
              (aih . ,(ly:make-pitch -1 5 SEMI-SHARP))
              (ais . ,(ly:make-pitch -1 5 SHARP))
              (aisih . ,(ly:make-pitch -1 5 THREE-Q-SHARP))
              (aisis . ,(ly:make-pitch -1 5 DOUBLE-SHARP))

              (heses . ,(ly:make-pitch -1 6 DOUBLE-FLAT))  ; non-standard name for bb
              (bb . ,(ly:make-pitch -1 6 DOUBLE-FLAT)) ; should be bes; kept for backward compatibility
              (bes . ,(ly:make-pitch -1 6 DOUBLE-FLAT))
              (heseh . ,(ly:make-pitch -1 6 THREE-Q-FLAT))
              (b . ,(ly:make-pitch -1 6 FLAT))
              (heh . ,(ly:make-pitch -1 6 SEMI-FLAT))
              (h . ,(ly:make-pitch -1 6 NATURAL))
              (hih . ,(ly:make-pitch -1 6 SEMI-SHARP))
              (his . ,(ly:make-pitch -1 6 SHARP))
              (hisih . ,(ly:make-pitch -1 6 THREE-Q-SHARP))
              (hisis . ,(ly:make-pitch -1 6 DOUBLE-SHARP))
              ))
*/
  // suomi
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "asas";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "aSesquiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "as";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "aSemiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "a";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "aSemiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "ais";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "aSesquiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "aisis";

  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "bes";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "bSesquiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "b";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "bSemiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "h";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "bSemiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "his";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "bSesquiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "hisis";

  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "ceses";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "cSesquiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "ces";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "cSemiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "c";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "cSemiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "cis";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "cSesquiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "cisis";

  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "deses";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "dSesquiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "des";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "dSemiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "d";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "dSemiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "dis";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "dSesquiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "disis";

  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "eses";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "eSesquiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "es";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "eSemiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "e";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "eSemiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "eis";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "eSesquiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "eisis";

  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "feses";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "fSesquiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fes";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "fSemiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "f";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fSemiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fis";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "fSesquiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fisis";

  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "geses";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "gSesquiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "ges";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "gSemiFlat???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "g";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "gSemiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "gis";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "gSesquiSharp???";
  pSuomiPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "gisis";
}

//______________________________________________________________________________
void initializeSvenskaPitchesNamesMap ()
{
/*
    (svenska . (
                (cessess . ,(ly:make-pitch -1 0 DOUBLE-FLAT))
                (cesseh . ,(ly:make-pitch -1 0 THREE-Q-FLAT))
                (cess . ,(ly:make-pitch -1 0 FLAT))
                (ceh . ,(ly:make-pitch -1 0 SEMI-FLAT))
                (c . ,(ly:make-pitch -1 0 NATURAL))
                (cih . ,(ly:make-pitch -1 0 SEMI-SHARP))
                (ciss . ,(ly:make-pitch -1 0 SHARP))
                (cissih . ,(ly:make-pitch -1 0 THREE-Q-SHARP))
                (cississ . ,(ly:make-pitch -1 0 DOUBLE-SHARP))

                (dessess . ,(ly:make-pitch -1 1 DOUBLE-FLAT))
                (desseh . ,(ly:make-pitch -1 1 THREE-Q-FLAT))
                (dess . ,(ly:make-pitch -1 1 FLAT))
                (deh . ,(ly:make-pitch -1 1 SEMI-FLAT))
                (d . ,(ly:make-pitch -1 1 NATURAL))
                (dih . ,(ly:make-pitch -1 1 SEMI-SHARP))
                (diss . ,(ly:make-pitch -1 1 SHARP))
                (dissih . ,(ly:make-pitch -1 1 THREE-Q-SHARP))
                (dississ . ,(ly:make-pitch -1 1 DOUBLE-SHARP))

                (essess . ,(ly:make-pitch -1 2 DOUBLE-FLAT))
                (esseh . ,(ly:make-pitch -1 2 THREE-Q-FLAT))
                (ess . ,(ly:make-pitch -1 2 FLAT))
                (eeh . ,(ly:make-pitch -1 2 SEMI-FLAT))
                (e . ,(ly:make-pitch -1 2 NATURAL))
                (eih . ,(ly:make-pitch -1 2 SEMI-SHARP))
                (eiss . ,(ly:make-pitch -1 2 SHARP))
                (eissih . ,(ly:make-pitch -1 2 THREE-Q-SHARP))
                (eississ . ,(ly:make-pitch -1 2 DOUBLE-SHARP))

                (fessess . ,(ly:make-pitch -1 3 DOUBLE-FLAT))
                (fesseh . ,(ly:make-pitch -1 3 THREE-Q-FLAT))
                (fess . ,(ly:make-pitch -1 3 FLAT))
                (feh . ,(ly:make-pitch -1 3 SEMI-FLAT))
                (f . ,(ly:make-pitch -1 3 NATURAL))
                (fih . ,(ly:make-pitch -1 3 SEMI-SHARP))
                (fiss . ,(ly:make-pitch -1 3 SHARP))
                (fissih . ,(ly:make-pitch -1 3 THREE-Q-SHARP))
                (fississ . ,(ly:make-pitch -1 3 DOUBLE-SHARP))

                (gessess . ,(ly:make-pitch -1 4 DOUBLE-FLAT))
                (gesseh . ,(ly:make-pitch -1 4 THREE-Q-FLAT))
                (gess . ,(ly:make-pitch -1 4 FLAT))
                (geh . ,(ly:make-pitch -1 4 SEMI-FLAT))
                (g . ,(ly:make-pitch -1 4 NATURAL))
                (gih . ,(ly:make-pitch -1 4 SEMI-SHARP))
                (giss . ,(ly:make-pitch -1 4 SHARP))
                (gissih . ,(ly:make-pitch -1 4 THREE-Q-SHARP))
                (gississ . ,(ly:make-pitch -1 4 DOUBLE-SHARP))

                (assess . ,(ly:make-pitch -1 5 DOUBLE-FLAT))
                (asseh . ,(ly:make-pitch -1 5 THREE-Q-FLAT))
                (ass . ,(ly:make-pitch -1 5 FLAT))
                (aeh . ,(ly:make-pitch -1 5 SEMI-FLAT))
                (a . ,(ly:make-pitch -1 5 NATURAL))
                (aih . ,(ly:make-pitch -1 5 SEMI-SHARP))
                (aiss . ,(ly:make-pitch -1 5 SHARP))
                (aissih . ,(ly:make-pitch -1 5 THREE-Q-SHARP))
                (aississ . ,(ly:make-pitch -1 5 DOUBLE-SHARP))

                (hessess . ,(ly:make-pitch -1 6 DOUBLE-FLAT))
                (hesseh . ,(ly:make-pitch -1 6 THREE-Q-FLAT))
                (b . ,(ly:make-pitch -1 6 FLAT))
                (heh . ,(ly:make-pitch -1 6 SEMI-FLAT))
                (h . ,(ly:make-pitch -1 6 NATURAL))
                (hih . ,(ly:make-pitch -1 6 SEMI-SHARP))
                (hiss . ,(ly:make-pitch -1 6 SHARP))
                (hissih . ,(ly:make-pitch -1 6 THREE-Q-SHARP))
                (hississ . ,(ly:make-pitch -1 6 DOUBLE-SHARP))
                ))
*/
  // svenska
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "assess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "aSesquiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "ass";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "aSemiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "a";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "aSemiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "aiss";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "aSesquiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "aississ";

  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "hessess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "bSesquiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "b";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "bSemiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "h";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "bSemiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "hiss";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "bSesquiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "hississ";

  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "cessess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "cSesquiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "cess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "cSemiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "c";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "cSemiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "ciss";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "cSesquiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "cississ";

  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "dessess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "dSesquiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "dess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "dSemiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "d";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "dSemiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "diss";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "dSesquiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "dississ";

  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "essess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "eSesquiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "ess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "eSemiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "e";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "eSemiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "eiss";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "eSesquiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "eississ";

  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "fessess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "fSesquiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "fSemiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "f";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "fSemiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fiss";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "fSesquiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fississ";

  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "gessess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "gSesquiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "gess";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "gSemiFlat???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "g";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "gSemiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "giss";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "gSesquiSharp???";
  pSvenskaPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "gississ";
}

//______________________________________________________________________________
void initializeVlaamsPitchesNamesMap ()
{
/*
    (vlaams . (
               (dobb . ,(ly:make-pitch -1 0 DOUBLE-FLAT))
               (dobhb . ,(ly:make-pitch -1 0 THREE-Q-FLAT))
               (dob . ,(ly:make-pitch -1 0 FLAT))
               (dohb . ,(ly:make-pitch -1 0 SEMI-FLAT))
               (do . ,(ly:make-pitch -1 0 NATURAL))
               (dohk . ,(ly:make-pitch -1 0 SEMI-SHARP))
               (dok . ,(ly:make-pitch -1 0 SHARP))
               (dokhk . ,(ly:make-pitch -1 0 THREE-Q-SHARP))
               (dokk . ,(ly:make-pitch -1 0 DOUBLE-SHARP))

               (rebb . ,(ly:make-pitch -1 1 DOUBLE-FLAT))
               (rebhb . ,(ly:make-pitch -1 1 THREE-Q-FLAT))
               (reb . ,(ly:make-pitch -1 1 FLAT))
               (rehb . ,(ly:make-pitch -1 1 SEMI-FLAT))
               (re . ,(ly:make-pitch -1 1 NATURAL))
               (rehk . ,(ly:make-pitch -1 1 SEMI-SHARP))
               (rek . ,(ly:make-pitch -1 1 SHARP))
               (rekhk . ,(ly:make-pitch -1 1 THREE-Q-SHARP))
               (rekk . ,(ly:make-pitch -1 1 DOUBLE-SHARP))

               (mibb . ,(ly:make-pitch -1 2 DOUBLE-FLAT))
               (mibhb . ,(ly:make-pitch -1 2 THREE-Q-FLAT))
               (mib . ,(ly:make-pitch -1 2 FLAT))
               (mihb . ,(ly:make-pitch -1 2 SEMI-FLAT))
               (mi . ,(ly:make-pitch -1 2 NATURAL))
               (mihk . ,(ly:make-pitch -1 2 SEMI-SHARP))
               (mik . ,(ly:make-pitch -1 2 SHARP))
               (mikhk . ,(ly:make-pitch -1 2 THREE-Q-SHARP))
               (mikk . ,(ly:make-pitch -1 2 DOUBLE-SHARP))

               (fabb . ,(ly:make-pitch -1 3 DOUBLE-FLAT))
               (fabhb . ,(ly:make-pitch -1 3 THREE-Q-FLAT))
               (fab . ,(ly:make-pitch -1 3 FLAT))
               (fahb . ,(ly:make-pitch -1 3 SEMI-FLAT))
               (fa . ,(ly:make-pitch -1 3 NATURAL))
               (fahk . ,(ly:make-pitch -1 3 SEMI-SHARP))
               (fak . ,(ly:make-pitch -1 3 SHARP))
               (fakhk . ,(ly:make-pitch -1 3 THREE-Q-SHARP))
               (fakk . ,(ly:make-pitch -1 3 DOUBLE-SHARP))

               (solbb . ,(ly:make-pitch -1 4 DOUBLE-FLAT))
               (solbhb . ,(ly:make-pitch -1 4 THREE-Q-FLAT))
               (solb . ,(ly:make-pitch -1 4 FLAT))
               (solhb . ,(ly:make-pitch -1 4 SEMI-FLAT))
               (sol . ,(ly:make-pitch -1 4 NATURAL))
               (solhk . ,(ly:make-pitch -1 4 SEMI-SHARP))
               (solk . ,(ly:make-pitch -1 4 SHARP))
               (solkhk . ,(ly:make-pitch -1 4 THREE-Q-SHARP))
               (solkk . ,(ly:make-pitch -1 4 DOUBLE-SHARP))

               (labb . ,(ly:make-pitch -1 5 DOUBLE-FLAT))
               (labhb . ,(ly:make-pitch -1 5 THREE-Q-FLAT))
               (lab . ,(ly:make-pitch -1 5 FLAT))
               (lahb . ,(ly:make-pitch -1 5 SEMI-FLAT))
               (la . ,(ly:make-pitch -1 5 NATURAL))
               (lahk . ,(ly:make-pitch -1 5 SEMI-SHARP))
               (lak . ,(ly:make-pitch -1 5 SHARP))
               (lakhk . ,(ly:make-pitch -1 5 THREE-Q-SHARP))
               (lakk . ,(ly:make-pitch -1 5 DOUBLE-SHARP))

               (sibb . ,(ly:make-pitch -1 6 DOUBLE-FLAT))
               (sibhb . ,(ly:make-pitch -1 6 THREE-Q-FLAT))
               (sib . ,(ly:make-pitch -1 6 FLAT))
               (sihb . ,(ly:make-pitch -1 6 SEMI-FLAT))
               (si . ,(ly:make-pitch -1 6 NATURAL))
               (sihk . ,(ly:make-pitch -1 6 SEMI-SHARP))
               (sik . ,(ly:make-pitch -1 6 SHARP))
               (sikhk . ,(ly:make-pitch -1 6 THREE-Q-SHARP))
               (sikk . ,(ly:make-pitch -1 6 DOUBLE-SHARP))
               ))
*/
  // vlaams
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "labb";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "laSesquiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "lab";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "laSemiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "la";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "laSemiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "lak";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "laSesquiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "lakk";

  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "sibb";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "siSesquiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "sib";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "siSemiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "si";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "siSemiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "sik";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "siSesquiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "sikk";

  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "dobb";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "doSesquiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "dob";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "doSemiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "do";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "doSemiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "dok";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "doSesquiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "dokk";

  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "rebb";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "reSesquiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "reb";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "reSemiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "re";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "reSemiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "rek";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "reSesquiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "rekk";

  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "mibb";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "miSesquiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "mib";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "miSemiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "mi";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "miSemiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "mik";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "miSesquiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "mikk";

  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "fabb";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "faSesquiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fab";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "faSemiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "fa";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "faSemiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fak";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "faSesquiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fakk";

  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "solbb";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "solSesquiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "solb";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "solSemiFlat???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "sol";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "solSemiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "solk";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "solSesquiSharp???";
  pVlaamsPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "solkk";
}

//______________________________________________________________________________
void initializeArabicPitchesNamesMap ()
{
/*
    (arabic . (
               (c . ,(ly:make-pitch -1 0 NATURAL))
               (d . ,(ly:make-pitch -1 1 NATURAL))
               (e . ,(ly:make-pitch -1 2 NATURAL))
               (f . ,(ly:make-pitch -1 3 NATURAL))
               (g . ,(ly:make-pitch -1 4 NATURAL))
               (a . ,(ly:make-pitch -1 5 NATURAL))
               (b . ,(ly:make-pitch -1 6 NATURAL))

               (cd . ,(ly:make-pitch -1 0 SHARP))
               (dd . ,(ly:make-pitch -1 1 SHARP))
               (ed . ,(ly:make-pitch -1 2 SHARP))
               (fd . ,(ly:make-pitch -1 3 SHARP))
               (gd . ,(ly:make-pitch -1 4 SHARP))
               (ad . ,(ly:make-pitch -1 5 SHARP))
               (bd . ,(ly:make-pitch -1 6 SHARP))

               (cb . ,(ly:make-pitch -1 0 FLAT))
               (db . ,(ly:make-pitch -1 1 FLAT))
               (eb . ,(ly:make-pitch -1 2 FLAT))
               (fb . ,(ly:make-pitch -1 3 FLAT))
               (gb . ,(ly:make-pitch -1 4 FLAT))
               (ab . ,(ly:make-pitch -1 5 FLAT))
               (bb . ,(ly:make-pitch -1 6 FLAT))

               (cdd . ,(ly:make-pitch -1 0 SEMI-SHARP))
               (ddd . ,(ly:make-pitch -1 1 SEMI-SHARP))
               (edd . ,(ly:make-pitch -1 2 SEMI-SHARP))
               (fdd . ,(ly:make-pitch -1 3 SEMI-SHARP))
               (gdd . ,(ly:make-pitch -1 4 SEMI-SHARP))
               (add . ,(ly:make-pitch -1 5 SEMI-SHARP))
               (bdd . ,(ly:make-pitch -1 6 SEMI-SHARP))

               (cdb . ,(ly:make-pitch -1 0 SEMI-FLAT))
               (ddb . ,(ly:make-pitch -1 1 SEMI-FLAT))
               (edb . ,(ly:make-pitch -1 2 SEMI-FLAT))
               (fdb . ,(ly:make-pitch -1 3 SEMI-FLAT))
               (gdb . ,(ly:make-pitch -1 4 SEMI-FLAT))
               (adb . ,(ly:make-pitch -1 5 SEMI-FLAT))
               (bdb . ,(ly:make-pitch -1 6 SEMI-FLAT))

               (ctqb . ,(ly:make-pitch -1 0 THREE-Q-FLAT))
               (dtqb . ,(ly:make-pitch -1 1 THREE-Q-FLAT))
               (etqb . ,(ly:make-pitch -1 2 THREE-Q-FLAT))
               (ftqb . ,(ly:make-pitch -1 3 THREE-Q-FLAT))
               (gtqb . ,(ly:make-pitch -1 4 THREE-Q-FLAT))
               (atqb . ,(ly:make-pitch -1 5 THREE-Q-FLAT))
               (btqb . ,(ly:make-pitch -1 6 THREE-Q-FLAT))

               (ctqd . ,(ly:make-pitch -1 0 THREE-Q-SHARP))
               (dtqd . ,(ly:make-pitch -1 1 THREE-Q-SHARP))
               (etqd . ,(ly:make-pitch -1 2 THREE-Q-SHARP))
               (ftqd . ,(ly:make-pitch -1 3 THREE-Q-SHARP))
               (gtqd . ,(ly:make-pitch -1 4 THREE-Q-SHARP))
               (atqd . ,(ly:make-pitch -1 5 THREE-Q-SHARP))
               (btqd . ,(ly:make-pitch -1 6 THREE-Q-SHARP))

               (cfhb . ,(ly:make-pitch -1 0 FIVE-HALF-FLAT))
               (dfhb . ,(ly:make-pitch -1 1 FIVE-HALF-FLAT))
               (efhb . ,(ly:make-pitch -1 2 FIVE-HALF-FLAT))
               (ffhb . ,(ly:make-pitch -1 3 FIVE-HALF-FLAT))
               (gfhb . ,(ly:make-pitch -1 4 FIVE-HALF-FLAT))
               (afhb . ,(ly:make-pitch -1 5 FIVE-HALF-FLAT))
               (bfhb . ,(ly:make-pitch -1 6 FIVE-HALF-FLAT))

               (cfhd . ,(ly:make-pitch -1 0 FIVE-HALF-SHARP))
               (dfhd . ,(ly:make-pitch -1 1 FIVE-HALF-SHARP))
               (efhd . ,(ly:make-pitch -1 2 FIVE-HALF-SHARP))
               (ffhd . ,(ly:make-pitch -1 3 FIVE-HALF-SHARP))
               (gfhd . ,(ly:make-pitch -1 4 FIVE-HALF-SHARP))
               (afhd . ,(ly:make-pitch -1 5 FIVE-HALF-SHARP))
               (bfhd . ,(ly:make-pitch -1 6 FIVE-HALF-SHARP))

               (cshb . ,(ly:make-pitch -1 0 SEVEN-HALF-FLAT))
               (dshb . ,(ly:make-pitch -1 1 SEVEN-HALF-FLAT))
               (eshb . ,(ly:make-pitch -1 2 SEVEN-HALF-FLAT))
               (fshb . ,(ly:make-pitch -1 3 SEVEN-HALF-FLAT))
               (gshb . ,(ly:make-pitch -1 4 SEVEN-HALF-FLAT))
               (ashb . ,(ly:make-pitch -1 5 SEVEN-HALF-FLAT))
               (bshb . ,(ly:make-pitch -1 6 SEVEN-HALF-FLAT))

               (cshd . ,(ly:make-pitch -1 0 SEVEN-HALF-SHARP))
               (dshd . ,(ly:make-pitch -1 1 SEVEN-HALF-SHARP))
               (eshd . ,(ly:make-pitch -1 2 SEVEN-HALF-SHARP))
               (fshd . ,(ly:make-pitch -1 3 SEVEN-HALF-SHARP))
               (gshd . ,(ly:make-pitch -1 4 SEVEN-HALF-SHARP))
               (ashd . ,(ly:make-pitch -1 5 SEVEN-HALF-SHARP))
               (bshd . ,(ly:make-pitch -1 6 SEVEN-HALF-SHARP))
               ))
    ))
*/
  // arabic
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_UNKNOWN]       = "kQTP_UNKNOWN"; // JMI
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Rest]          = "r";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_Skip]          = "s";

  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleFlat]  = "labb";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiFlat]  = "laSesquiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Flat]        = "lab";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiFlat]    = "laSemiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Natural]     = "la";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SemiSharp]   = "laSemiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_Sharp]       = "lak";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_SesquiSharp] = "laSesquiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_A_DoubleSharp] = "lakk";

  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleFlat]  = "sibb";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiFlat]  = "siSesquiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Flat]        = "sib";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiFlat]    = "siSemiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Natural]     = "si";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SemiSharp]   = "siSemiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_Sharp]       = "sik";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_SesquiSharp] = "siSesquiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_B_DoubleSharp] = "sikk";

  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleFlat]  = "dobb";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiFlat]  = "doSesquiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Flat]        = "dob";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiFlat]    = "doSemiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Natural]     = "do";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SemiSharp]   = "doSemiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_Sharp]       = "dok";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_SesquiSharp] = "doSesquiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_C_DoubleSharp] = "dokk";

  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleFlat]  = "rebb";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiFlat]  = "reSesquiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Flat]        = "reb";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiFlat]    = "reSemiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Natural]     = "re";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SemiSharp]   = "reSemiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_Sharp]       = "rek";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_SesquiSharp] = "reSesquiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_D_DoubleSharp] = "rekk";

  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleFlat]  = "mibb";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiFlat]  = "miSesquiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Flat]        = "mib";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiFlat]    = "miSemiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Natural]     = "mi";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SemiSharp]   = "miSemiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_Sharp]       = "mik";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_SesquiSharp] = "miSesquiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_E_DoubleSharp] = "mikk";

  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleFlat]  = "fabb";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiFlat]  = "faSesquiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Flat]        = "fab";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiFlat]    = "faSemiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Natural]     = "fa";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SemiSharp]   = "faSemiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_Sharp]       = "fak";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_SesquiSharp] = "faSesquiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_F_DoubleSharp] = "fakk";

  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleFlat]  = "solbb";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiFlat]  = "solSesquiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Flat]        = "solb";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiFlat]    = "solSemiFlat???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Natural]     = "sol";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SemiSharp]   = "solSemiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_Sharp]       = "solk";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_SesquiSharp] = "solSesquiSharp???";
  pArabicPitchesNamesMap [msrQuarterTonesPitchKind::kQTP_G_DoubleSharp] = "solkk";
}

//______________________________________________________________________________
std::map<msrQuarterTonesPitchKind, std::string>& getNederlandsPitchesNamesMap ()
{
  if (! pNederlandsPitchesNamesMap.size ()) {
    initializeNederlandsPitchesNamesMap ();
 }

 return pNederlandsPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getCatalanPitchesNamesMap ()
{
  if (! pCatalanPitchesNamesMap.size ()) {
    initializeCatalanPitchesNamesMap ();
 }

 return pCatalanPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getDeutschPitchesNamesMap ()
{
  if (! pDeutschPitchesNamesMap.size ()) {
    initializeDeutschPitchesNamesMap ();
 }

 return pDeutschPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getEnglishPitchesNamesMap ()
{
  if (! pEnglishPitchesNamesMap.size ()) {
    initializeEnglishPitchesNamesMap ();
 }

 return pEnglishPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getEspanolPitchesNamesMap ()
{
  if (! pEspanolPitchesNamesMap.size ()) {
    initializeEspanolPitchesNamesMap ();
 }

 return pEspanolPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getFrancaisPitchesNamesMap ()
{
  if (! pFrancaisPitchesNamesMap.size ()) {
    initializeFrancaisPitchesNamesMap ();
 }

 return pFrancaisPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getItalianoPitchesNamesMap ()
{
  if (! pItalianoPitchesNamesMap.size ()) {
    initializeItalianoPitchesNamesMap ();
 }

 return pItalianoPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getNorskPitchesNamesMap ()
{
  if (! pNorskPitchesNamesMap.size ()) {
    initializeNorskPitchesNamesMap ();
 }

 return pNorskPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getPortuguesPitchesNamesMap ()
{
  if (! pPortuguesPitchesNamesMap.size ()) {
    initializePortuguesPitchesNamesMap ();
 }

 return pPortuguesPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getSuomiPitchesNamesMap ()
{
  if (! pSuomiPitchesNamesMap.size ()) {
    initializeSuomiPitchesNamesMap ();
 }

 return pSuomiPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getSvenskaPitchesNamesMap ()
{
  if (! pSvenskaPitchesNamesMap.size ()) {
    initializeSvenskaPitchesNamesMap ();
 }

 return pSvenskaPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getVlaamsPitchesNamesMap ()
{
  if (! pVlaamsPitchesNamesMap.size ()) {
    initializeVlaamsPitchesNamesMap ();
 }

 return pVlaamsPitchesNamesMap;
}

std::map<msrQuarterTonesPitchKind, std::string>& getArabicPitchesNamesMap ()
{
  if (! pArabicPitchesNamesMap.size ()) {
    initializeArabicPitchesNamesMap ();
 }

 return pArabicPitchesNamesMap;
}

//______________________________________________________________________________
msrQuarterTonesPitchesLanguageKind msrQuarterTonesPitchesLanguageKindFromString (
  const std::string& theString)
{
  msrQuarterTonesPitchesLanguageKind
    result =
      msrQuarterTonesPitchesLanguageKind::kQTPEnglish;  // MSR default JMI ???

  std::map<std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      pQuarterTonesPitchesLanguageKindsMap.find (
        theString);

  if (it == pQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, Pitches language kind is unknown in the map
    std::stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      pQuarterTonesPitchesLanguageKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      std::endl;

    ++gIndenter;

    s <<
      availableMsrQuarterTonesPitchesLanguageKinds (
        K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (s.str ());
  }

  result = (*it).second;

  return result;
}

//______________________________________________________________________________
std::string msrQuarterTonesPitchKindAsStringInLanguage (
  msrQuarterTonesPitchKind           quarterTonesPitchKind,
  msrQuarterTonesPitchesLanguageKind languageKind)
{
  std::string result;

  switch (languageKind) {
    case msrQuarterTonesPitchesLanguageKind::kQTPNederlands:
      result = pNederlandsPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPCatalan:
      result = pCatalanPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPDeutsch:
      result = pDeutschPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEnglish:
      result = pEnglishPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEspanol:
      result = pEspanolPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPFrancais:
      result = pFrancaisPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPItaliano:
      result = pItalianoPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPNorsk:
      result = pNorskPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPPortugues:
      result = pPortuguesPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSuomi:
      result = pSuomiPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSvenska:
      result = pSvenskaPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPVlaams:
      result = pVlaamsPitchesNamesMap [quarterTonesPitchKind];
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPArabic:
      result = pArabicPitchesNamesMap [quarterTonesPitchKind];
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
std::string msrDiatonicPitchKindAsStringInLanguage (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrDiatonicPitchKind               diatonicPitchKind)
{
  std::string result;

  switch (diatonicPitchKind) {
    case msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN:
      result = "kDiatonicPitch_UNKNOWN";
      break;

    case msrDiatonicPitchKind::kDiatonicPitchA:
      result = "a";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchB:
      result = "b";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchC:
      result = "c";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchD:
      result = "d";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchE:
      result = "e";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchF:
      result = "f";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchG:
      result = "g";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
std::string msrQuarterTonesPitchesLanguageKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind)
{
  std::string result;

  switch (languageKind) {
    case msrQuarterTonesPitchesLanguageKind::kQTPNederlands:
      result = "kQTPNederlands";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPCatalan:
      result = "kQTPCatalan";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPDeutsch:
      result = "kQTPDeutsch";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEnglish:
      result = "kQTPEnglish";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEspanol:
      result = "kQTPEspanol";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPFrancais:
      result = "kQTPFrancais";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPItaliano:
      result = "kQTPItaliano";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPNorsk:
      result = "kQTPNorsk";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPPortugues:
      result = "kQTPPortugues";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSuomi:
      result = "kQTPSuomi";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSvenska:
      result = "kQTPSvenska";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPVlaams:
      result = "kQTPVlaams";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPArabic:
      result = "kQTPArabic";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
msrQuarterTonesPitchKind quarterTonesPitchKindFromString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  const std::string&            quarterTonesPitchName)
{
  msrQuarterTonesPitchKind
    result =
      msrQuarterTonesPitchKind::kQTP_UNKNOWN;

  std::map<msrQuarterTonesPitchKind, std::string>* pitchesNamesMapPTR;

  // select the relevant pitch names map
  switch (languageKind) {
    case msrQuarterTonesPitchesLanguageKind::kQTPNederlands:
      pitchesNamesMapPTR = &pNederlandsPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPCatalan:
      pitchesNamesMapPTR = &pCatalanPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPDeutsch:
      pitchesNamesMapPTR = &pDeutschPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEnglish:
      pitchesNamesMapPTR = &pEnglishPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEspanol:
      pitchesNamesMapPTR = &pEspanolPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPFrancais:
      pitchesNamesMapPTR = &pFrancaisPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPItaliano:
      pitchesNamesMapPTR = &pItalianoPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPNorsk:
      pitchesNamesMapPTR = &pNorskPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPPortugues:
      pitchesNamesMapPTR = &pPortuguesPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSuomi:
      pitchesNamesMapPTR = &pSuomiPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSvenska:
      pitchesNamesMapPTR = &pSvenskaPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPVlaams:
      pitchesNamesMapPTR = &pVlaamsPitchesNamesMap;
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPArabic:
      pitchesNamesMapPTR = &pArabicPitchesNamesMap;
      break;
  } // switch

  // is quarterTonesPitchName present in the map?
  if (pitchesNamesMapPTR->size ()) {
    std::map<msrQuarterTonesPitchKind, std::string>::const_iterator
      iBegin = pitchesNamesMapPTR->begin (),
      iEnd   = pitchesNamesMapPTR->end (),
      i      = iBegin;

    for ( ; ; ) {
      if ((*i).second == quarterTonesPitchName) {
        result = (*i).first;
        break;
      }
      if (++i == iEnd) break;
    } // for
  }

  return result;
}

//______________________________________________________________________________
void initializeMsrPitchesNames ()
{
  initializeQuarterTonesPitchesLanguageKinds ();
}


}
