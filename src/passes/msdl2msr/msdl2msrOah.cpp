/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "mfStaticSettings.h"

#include "mfConstants.h"
#include "mfStringsHandling.h"

#include "msrPitchesNames.h"

#include "oahOah.h"

#include "msdlInputOah.h"

#include "msdl2msrOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________

S_msdl2msrOahGroup gGlobalMsdl2msrOahGroup;

S_msdl2msrOahGroup msdl2msrOahGroup::create ()
{
  msdl2msrOahGroup* obj = new msdl2msrOahGroup ();
  assert (obj != nullptr);
  return obj;
}

msdl2msrOahGroup::msdl2msrOahGroup ()
  : oahGroup (
    "Msdl2msr",
    "help-msdl-to-msdr", "hmsdl2msr",
R"(These options control the way xmlelement are translated to MSDR.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2msrGroup ();
}

msdl2msrOahGroup::~msdl2msrOahGroup ()
{}

#ifdef MF_TRACE_IS_ENABLED
void msdl2msrOahGroup::initializeMsdl2msrTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2msdl Trace",
        "help-msdl-to-msdl-trace", "hmsdl2msdlt",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

 // MSDL tokens

  S_oahBooleanAtom
    traceTokensBooleanAtom =
      oahBooleanAtom::create (
        "trace-tokens", "ttoks",
R"(Write a trace of the MSDL tokens handling activity to standard error.)",
        "traceTokens",
        fTraceTokens);

  subGroup->
    appendAtomToSubGroup (
      traceTokensBooleanAtom);

  // MSDL tokens details

  subGroup->
    appendAtomToSubGroup (
      oahThreeBooleansAtom::create (
        "trace-tokens-details", "ttoksd",
R"(Write a trace of the MSDL tokens handling activity with more details to standard error.)",
        "traceTokensDetails",
        fTraceTokensDetails,
        traceTokensBooleanAtom,
        traceTokensBooleanAtom));

  // MSDL syntax

  S_oahBooleanAtom
    traceSyntaxBooleanAtom =
      oahBooleanAtom::create (
        "trace-syntax", "tsynt",
R"(Write a trace of the MSDL syntax analysis activity to standard error.)",
        "traceSyntax",
        fTraceSyntax);

  subGroup->
    appendAtomToSubGroup (
      traceSyntaxBooleanAtom);

  // MSDL syntax details

  subGroup->
    appendAtomToSubGroup (
      oahThreeBooleansAtom::create (
        "trace-syntax-details", "tsyntd",
R"(Write a trace of the MSDL syntax analysis activity with more details to standard error.)",
        "traceSyntaxDetails",
        fTraceSyntaxDetails,
        traceSyntaxBooleanAtom,
        traceSyntaxBooleanAtom));

  // MSDL syntax error recovery

  S_oahBooleanAtom
    traceSyntaxErrorRecoveryBooleanAtom =
      oahBooleanAtom::create (
        "trace-syntax-error-recovery", "tsynter",
R"(Write a trace of the MSDL syntax error recovery activity to standard error.)",
        "traceSyntaxErrorRecovery",
        fTraceSyntaxErrorRecovery);

  subGroup->
    appendAtomToSubGroup (
      traceSyntaxErrorRecoveryBooleanAtom);

  // MSDL syntax error recovery details

  subGroup->
    appendAtomToSubGroup (
      oahThreeBooleansAtom::create (
        "trace-syntax-error-recovery-details", "tsynterd",
R"(Write a trace of the MSDL syntax error recovery activity with more details to standard error.)",
        "traceSyntaxErrorRecoveryDetails",
        fTraceSyntaxErrorRecoveryDetails,
        traceSyntaxErrorRecoveryBooleanAtom,
        traceSyntaxErrorRecoveryBooleanAtom));
}
#endif // MF_TRACE_IS_ENABLED

void msdl2msrOahGroup::initializeMsdlLanguagesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "help-msdl-languages", "hmsdll",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  if (! setMsdlQuarterTonesPitchesLanguage ("english")) {
    std::stringstream ss;

    ss <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSDR pitches default language 'english' is unknown" <<
      std::endl <<
      "The " <<
      getQuarterTonesPitchesLanguageKindsMap ().size () <<
      " known MSDL pitches languages are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  // MSDL user language

  const msdlUserLanguageKind
    msdlUserLanguageKindDefaultValue =
      msdlUserLanguageKind::kUserLanguageEnglish; // MSDL default

  fMsdlUserLanguageKind =
    msdlUserLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlUserLanguageAtom::create (
        "msdl-user-language", "mul",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to interact with the user.
The NUMBER MSDL user languages available are:
USER_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              std::regex ("NUMBER"),
              std::to_string (gGlobalMsdlUserLanguageKindsMap.size ())),
            std::regex ("USER_LANGUAGES"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            availableMsdlUserLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          msdlUserLanguageKindAsString (
            msdlUserLanguageKindDefaultValue)),
        "LANGUAGE",
        "msdlUserLanguageKind",
        fMsdlUserLanguageKind));

  // MSDL pitches language

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      fMsdlQuarterTonesPitchesLanguageKind;

  fMsdlQuarterTonesPitchesLanguageKind =
    msrQuarterTonesPitchesLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlPitchesLanguageAtom::create (
        "msdl-pitches-language", "msdlpl",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to input note pitches.
The NUMBER MSDL pitches languages available are:
PITCHES_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              std::regex ("NUMBER"),
              std::to_string (getQuarterTonesPitchesLanguageKindsMap ().size ())),
            std::regex ("PITCHES_LANGUAGES"),
//           gIndenter.indentMultiLineString (
//             foundString,
//             os);
            availableQuarterTonesPitchesLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          msrQuarterTonesPitchesLanguageKindAsString (
          	msrQuarterTonesPitchesLanguageKindDefaultValue)),
        "LANGUAGE",
        "msdlPitchesLanguage",
        fMsdlQuarterTonesPitchesLanguageKind));

  // MSDL keywords input language

  const msdlKeywordsLanguageKind
    msdlKeywordsInputLanguageKindDefaultValue =
      msdlKeywordsLanguageKind::kKeywordsLanguageEnglish; // MSDL default

  fMsdlKeywordsInputLanguageKind =
    msdlKeywordsInputLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlKeywordsLanguageAtom::create (
        "msdl-keywords-input-language", "mkil",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to input keyword names.
The NUMBER MSDL keywords input languages available are:
KEYWORDS_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              std::regex ("NUMBER"),
              std::to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
            std::regex ("KEYWORDS_LANGUAGES"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            availableMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          msdlKeywordsLanguageKindAsString (
            msdlKeywordsInputLanguageKindDefaultValue)),
        "LANGUAGE",
        "fMsdlKeywordsInputLanguageKind",
        fMsdlKeywordsInputLanguageKind));
}

void msdl2msrOahGroup::initializeMsdlWholeInputScansOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MSDL whole input scans",
        "help-msdl-whole-input-scans", "hmwis",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // perform lexical analysis only

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lex-only", "lex",
R"(Perform lexical analysis only of the whole input at once,
without syntactic nor semantic analysis. No code is generated.)",
        "fLexicalAnalysisOnly",
        fLexicalAnalysisOnly));

  // ignore separator tokens

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ignore-separator-tokens", "istoks",
R"(Ignore separator tokens such as space, tab and end of line
when performing lexical analysis of the whole input at once.)",
        "fIgnoreSeparatorTokens",
        fIgnoreSeparatorTokens));

  // MSDL keywords translation language

  fMsdlKeywordsConversionLanguageKind = msdlKeywordsLanguageKind::kKeywordsLanguage_UNKNOWN_;

  subGroup->
    appendAtomToSubGroup (
      msdlKeywordsLanguageAtom::create (
        "msdl-keywords-translation-language", "mktl",
        regex_replace (
          regex_replace (
R"(Use LANGUAGE to output translated keyword names.
This option causes the translated MSDL text to be written to standard output,
followed by a quit.
The NUMBER MSDL keywords translation languages available are:
KEYWORDS_LANGUAGES.
The default is not to translate the keywords.)",
            std::regex ("NUMBER"),
            std::to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
          std::regex ("KEYWORDS_LANGUAGES"),
//           gIndenter.indentMultiLineString (
//             foundString,
//             os);
          availableMsdlKeywordsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
        "LANGUAGE",
        "fMsdlKeywordsConversionLanguageKind",
        fMsdlKeywordsConversionLanguageKind));

  // MSDL comments type

  const msdlCommentsTypeKind
    msdlCommentsTypeKindDefaultValue =
      msdlCommentsTypeKind::kCommentsTypePercent; // MSDL default

  fMsdlCommentsTypeKind =
    msdlCommentsTypeKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlCommentsTypeAtom::create (
        "msdl-comments-types", "mct",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use TYPE for the translated comments.
The NUMBER MSDL comments types available are:
COMMENTS_TYPES.
The default is 'DEFAULT_VALUE'.)",
              std::regex ("NUMBER"),
              std::to_string (gGlobalMsdlCommentsTypeKindsMap.size ())),
            std::regex ("COMMENTS_TYPES"),
//             gIndenter.indentMultiLineString (
//               foundString,
//               os);
            availableMsdlCommentsTypeKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
          std::regex ("DEFAULT_VALUE"),
          msdlCommentsTypeKindAsString (
            msdlCommentsTypeKindDefaultValue)),
        "TYPE",
        "fMsdlCommentsTypeKind",
        fMsdlCommentsTypeKind));
}

void msdl2msrOahGroup::initializeMsdl2msrGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMsdl2msrTraceOah ();
#endif // MF_TRACE_IS_ENABLED

  initializeMsdlLanguagesOptions ();

  initializeMsdlWholeInputScansOptions ();
}

//______________________________________________________________________________
Bool msdl2msrOahGroup::setMsdlQuarterTonesPitchesLanguage (std::string language)
{
  // is language in the pitches languages map?
  std::map<std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      getQuarterTonesPitchesLanguageKindsMap ().find (language);

  if (it == getQuarterTonesPitchesLanguageKindsMap ().end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsdlQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void msdl2msrOahGroup::enforceGroupQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
// JMI
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msdl2msrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2msrOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2msrOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2msrOahGroup>*> (v)) {
        S_msdl2msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2msrOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdl2msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2msrOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2msrOahGroup>*> (v)) {
        S_msdl2msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2msrOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdl2msrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2msrOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msdl2msrOahGroup::printMsdl2msrValues (int fieldWidth)
{
  gLog <<
    "The msdl2msr options are:" <<
    std::endl;

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------

  gLog <<
    "Trace:" <<
    std::endl;

  ++gIndenter;

  // MSDL tokens and syntax
  // --------------------------------------

  gLog <<
    "MSDL tokens and syntax:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fTraceTokens" << ": " <<
      fTraceTokens <<
      std::endl <<
    std::setw (fieldWidth) << "fTraceTokensDetails" << ": " <<
      fTraceTokensDetails <<
      std::endl <<

    std::setw (fieldWidth) << "fTraceSyntax" << ": " <<
      fTraceSyntax <<
      std::endl <<
    std::setw (fieldWidth) << "fTraceSyntaxDetails" << ": " <<
      fTraceSyntaxDetails <<
      std::endl <<

    std::setw (fieldWidth) << "fTraceSyntaxErrorRecovery" << ": " <<
      fTraceSyntaxErrorRecovery <<
      std::endl <<
    std::setw (fieldWidth) << "fTraceSyntaxErrorRecoveryDetails" << ": " <<
      fTraceSyntaxErrorRecoveryDetails <<
      std::endl;

  --gIndenter;
#endif // MF_TRACE_IS_ENABLED

  // languages
  gLog <<
    "Languages:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fMsdlUserLanguageKind" << ": " <<
      msdlUserLanguageKindAsString (
        fMsdlUserLanguageKind) <<
      std::endl <<
    std::setw (fieldWidth) << "fMsdlKeywordsInputLanguageKind" << ": " <<
      msdlKeywordsLanguageKindAsString (
        fMsdlKeywordsInputLanguageKind) <<
      std::endl <<

    std::setw (fieldWidth) << "fMsdlQuarterTonesPitchesLanguageKind" << ": " <<
			fMsdlQuarterTonesPitchesLanguageKind <<
      std::endl;

  --gIndenter;

  // whole input scan

  gLog <<
    "Whole input scan:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fLexicalAnalysisOnly" << ": " <<
      fLexicalAnalysisOnly <<
      std::endl <<
    std::setw (fieldWidth) << "fIgnoreSeparatorTokens" << ": " <<
      fIgnoreSeparatorTokens <<
      std::endl <<

    std::setw (fieldWidth) << "fMsdlKeywordsConversionLanguageKind" << ": " <<
      msdlKeywordsLanguageKindAsString (
        fMsdlKeywordsConversionLanguageKind) <<
      std::endl <<

    std::setw (fieldWidth) << "fMsdlCommentsTypeKind" << ": " <<
      msdlCommentsTypeKindAsString (
        fMsdlCommentsTypeKind) <<
      std::endl;

  --gIndenter;

    // quit

  gLog <<
    "Quit:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fQuitOnLexicalErrors" << ": " <<
      fQuitOnLexicalErrors <<
      std::endl;

  --gIndenter;


  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msdl2msrOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msdl2msrOahGroup createGlobalMsdl2msrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msdl2msr OAH group";

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMsdl2msrOahGroup) {
    // create the global OAH group
    gGlobalMsdl2msrOahGroup =
      msdl2msrOahGroup::create ();
    assert (gGlobalMsdl2msrOahGroup != 0);
  }

  // return global OAH group
  return gGlobalMsdl2msrOahGroup;
}


}
