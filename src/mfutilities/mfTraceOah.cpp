/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"


#ifdef MF_TRACE_IS_ENABLED // encompasses the remainder of this file

#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "visitor.h"

// #include "mfConstants.h"
#include "mfStringsHandling.h"

#include "oahAtomsCollection.h"
#include "oahEarlyOptions.h"

#include "mfTraceOah.h"

#include "mxsr2msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_traceOahGroup traceOahGroup::create (
  const S_oahHandler& handler)
{
  traceOahGroup* obj = new traceOahGroup (
    handler);
  assert (obj != nullptr);
  return obj;
}

traceOahGroup::traceOahGroup (
  const S_oahHandler& handler)
  : oahGroup (
      "OAH Trace",
      "help-trace", "ht",
R"(There are trace options transversal to the successive passes,
showing what's going on in the various conversion activities.
They're provided as a help to the maintainers, as well as for the curious.
The options in this group can be quite verbose, use them with small input data!
All of them imply '-trace-passes, -tpasses'.)",
      oahElementVisibilityKind::kElementVisibilityHeaderOnly)
{
  createTheTracePrefixes (handler);

  initializeTraceOahGroup ();
}

traceOahGroup::~traceOahGroup ()
{}

void traceOahGroup::createTheTracePrefixes (const S_oahHandler& handler)
{
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the OAH trace prefixes" <<
      std::endl;
  }

  ++gIndenter;

  // the 'trace' prefixes
  // --------------------------------------

 fShortTracePrefix =
    oahPrefix::create (
      "t", "t",
      "'-t=abc,wxyz' is equivalent to '-tabc, -twxyz'");
  handler->
    registerPrefixInHandler (
      fShortTracePrefix);

  fLongTracePrefix =
    oahPrefix::create (
      "trace", "trace-",
      "'-trace=abc,yz' is equivalent to '-trace-abc, -trace-yz'");
  handler->
    registerPrefixInHandler (
      fLongTracePrefix);

//   // the 'display' prefixes
//   // --------------------------------------
//
//  fShortDisplayPrefix =
//     oahPrefix::create (
//       "t", "t",
//       "'-t=abc,wxyz' is equivalent to '-tabc, -twxyz'");
//   handler->
//     registerPrefixInHandler (
//       fShortDisplayPrefix);
//
//   fLongDisplayPrefix =
//     oahPrefix::create (
//       "trace", "trace-",
//       "'-trace=abc,yz' is equivalent to '-trace-abc, -trace-yz'");
//   handler->
//     registerPrefixInHandler (
//       fLongDisplayPrefix);
//
  --gIndenter;
}

void traceOahGroup::initializeMusicXMLPrintLayoutsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Print layouts",
        "help-print-layouts", "hpl",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // MusicXML print layouts

  S_oahTwoBooleansAtom
    traceMusicXMLPrintLayoutsAtom =
      oahTwoBooleansAtom::create (
        "trace-print-layouts", "tpl",
R"(Print layouts)",
        "fTraceMusicXMLPrintLayouts",
        fTraceMusicXMLPrintLayouts,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMusicXMLPrintLayoutsAtom);
}

void traceOahGroup::initializeOtherTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Other",
        "help-trace-other", "hto",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'other' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    otherMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-other", "tio",
        "Trace SHORT_NAME/LONG_NAME in other.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      otherMultiplexBooleansAtom);

  // components

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        K_TRACE_COMPONENTS_OPTION_LONG_NAME, K_TRACE_COMPONENTS_OPTION_SHORT_NAME,
R"(Write a trace of components handling to standard error.)",
        "fEarlyTraceComponentsRef",
        gEarlyOptions.getEarlyTraceComponentsRef ()));

  // passes

  fTracePassesBooleanAtom =
    oahBooleanAtom::create (
      K_TRACE_PASSES_OPTION_LONG_NAME, K_TRACE_PASSES_OPTION_SHORT_NAME,
R"(Write a trace of the passes to standard error.)",
      "fEarlyTracePassesRef",
      gEarlyOptions.getEarlyTracePassesRef ());

  subGroup->
    appendAtomToSubGroup (
      fTracePassesBooleanAtom);

  otherMultiplexBooleansAtom->
    addBooleanAtom (
      fTracePassesBooleanAtom);

  mfPassIDKind
    mfPassIDKindDefaultValue =
      mfPassIDKind::kMfPassID_ALL; // default value for passes trace

  fTraceOnlyPassIDOahAtom =
    passIDOahAtom::create (
      K_TRACE_ONLY_PASS_OPTION_LONG_NAME, K_TRACE_ONLY_PASS_OPTION_SHORT_NAME,
      regex_replace (
        regex_replace (
          regex_replace (
R"(Write a trace of pass PASSID only to standard error.
The NUMBER PASSIDs available are:
PASSID_KINDS.
The default is 'DEFAULT_VALUE'.)",
          std::regex ("NUMBER"),
          std::to_string (gGlobalMusicFormatsPassIDKindsMap.size ())),
        std::regex ("PASSID_KINDS"),
        availableMusicFormatsPassIDKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
      std::regex ("DEFAULT_VALUE"),
      mfPassIDKindAsString (
        mfPassIDKindDefaultValue)),
    "PASSID",
    "fEarlyTraceOnlyPass",
    gEarlyOptions.getEarlyTraceOnlyPassRef ());

  subGroup->
    appendAtomToSubGroup (
      fTraceOnlyPassIDOahAtom);

  // geometry

  S_oahTwoBooleansAtom
    traceGeometryBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-geometry", "tgeom",
R"(Geometry)",
        "fTraceGeometry",
        fTraceGeometry,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceGeometryBooleanAtom);

  otherMultiplexBooleansAtom->
    addBooleanAtom (
      traceGeometryBooleanAtom);

  // identification

  S_oahTwoBooleansAtom
    traceIdentificationBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-identification", "tid",
R"(Identification)",
        "fTraceIdentification",
        fTraceIdentification,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceIdentificationBooleanAtom);

  otherMultiplexBooleansAtom->
    addBooleanAtom (
      traceIdentificationBooleanAtom);

  // '-trace-for-tests, -tft' is hidden... JMI

  S_oahBooleanAtom
    traceForTestsOahBooleanAtom =
      oahBooleanAtom::create (
        "trace-for-tests", "tft",
R"(Write a trace for tests to standard error.)",
        "fTraceForTests",
        fTraceForTests);

  traceForTestsOahBooleanAtom->
    setElementVisibilityKind (
      oahElementVisibilityKind::kElementVisibilityHidden);

  subGroup->
    appendAtomToSubGroup (
      traceForTestsOahBooleanAtom);
}

void traceOahGroup::initializeTranspositionsTraceOah ()
{
  S_oahSubGroup
   subGroup =
      oahSubGroup::create (
        "Transpositions",
         "help-trace-transpositions", "htt",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // transpositions

  S_oahTwoBooleansAtom
    traceTranspositionsAtom =
      oahTwoBooleansAtom::create (
        "trace-transpositions", "ttransps",
R"(Transpositions (<transpose/> in MusicXML, \transposition in LilyPond))",
        "fTraceTranspositions",
        fTraceTranspositions,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTranspositionsAtom);

  // octave shifts

  S_oahTwoBooleansAtom
    traceOctaveShiftsAtom =
      oahTwoBooleansAtom::create (
        "trace-octave-shifts", "toctshifts",
R"(Octave shifts (<octave-shift/> in MusicXML, \ottava in LilyPond))",
        "fTraceOctaveShifts",
        fTraceOctaveShifts,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceOctaveShiftsAtom);
}

void traceOahGroup::initializeAboveStaffTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Above staff",
        "help-trace-above-staff", "htas",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'above staff' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    aboveStaffMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-above-staff", "tias",
        "Trace SHORT_NAME/LONG_NAME in above staff.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      aboveStaffMultiplexBooleansAtom);

  // rehearsal marks

  S_oahTwoBooleansAtom
    traceRehearsalMarksAtom =
      oahTwoBooleansAtom::create (
        "trace-rehearsals-marks", "thm",
R"(Rehearsal marks)",
        "fTraceRehearsalMarks",
        fTraceRehearsalMarks,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceRehearsalMarksAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceRehearsalMarksAtom);

  // segnos

  S_oahTwoBooleansAtom
    traceSegnosAtom =
      oahTwoBooleansAtom::create (
        "trace-segnos", "tsegnos",
R"(Segnos)",
        "fTraceSegnos",
        fTraceSegnos,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSegnosAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceSegnosAtom);

  // dal segnos

  S_oahTwoBooleansAtom
    traceDalSegnosAtom =
      oahTwoBooleansAtom::create (
        "trace-dal-segnos", "tdalsegnos",
R"(Dal segnos)",
        "fTraceDalSegnos",
        fTraceDalSegnos,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceDalSegnosAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceDalSegnosAtom);

  // codas

  S_oahTwoBooleansAtom
    traceCodasAtom =
      oahTwoBooleansAtom::create (
        "trace-codas", "tcodas",
R"(Codas)",
        "fTraceCodas",
        fTraceCodas,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceCodasAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceCodasAtom);

  // eyeglasses

  S_oahTwoBooleansAtom
    traceEyeGlassesAtom =
      oahTwoBooleansAtom::create (
        "trace-eyeglasses", "teyes",
R"(Eyeglasses)",
        "fTraceEyeGlasses",
        fTraceEyeGlasses,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceEyeGlassesAtom);
  aboveStaffMultiplexBooleansAtom->
    addBooleanAtom (
      traceEyeGlassesAtom);
}

void traceOahGroup::initializeBreaksAndBarLinesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Breaks and barLines",
        "help-trace-breaks-and-barlines", "htbab",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'breaks and barLines' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    breaksAndBarLinesMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-breaks-and-barlines", "tibab",
        "Trace SHORT_NAME/LONG_NAME in breaks and barLines.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      breaksAndBarLinesMultiplexBooleansAtom);

  // line breaks

  S_oahTwoBooleansAtom
    traceLineBreaksAtom =
      oahTwoBooleansAtom::create (
        "trace-line-breaks", "tlbreaks",
R"(Line breaks)",
        "fTraceLineBreaks",
        fTraceLineBreaks,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceLineBreaksAtom);
  breaksAndBarLinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceLineBreaksAtom);

  // page and line breaks

  S_oahTwoBooleansAtom
    tracePageBreaksAtom =
      oahTwoBooleansAtom::create (
        "trace-page-breaks", "tpbreaks",
R"(Page breaks)",
        "fTracePageBreaks",
        fTracePageBreaks,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      tracePageBreaksAtom);
  breaksAndBarLinesMultiplexBooleansAtom->
    addBooleanAtom (
      tracePageBreaksAtom);

  // bar lines

  S_oahTwoBooleansAtom
    traceBarLinesAtom =
      oahTwoBooleansAtom::create (
        "trace-barlines", "tbarlines",
R"(BarLines)",
        "fTraceBarLines",
        fTraceBarLines,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceBarLinesAtom);
  breaksAndBarLinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBarLinesAtom);

  // bar lines details

  S_oahThreeBooleansAtom
    traceBarLinesDetailsAtom =
      oahThreeBooleansAtom::create (
        "trace-barlines-details", "tbarlinesd",
R"(BarLines details)",
        "fTraceBarLinesDetails",
        fTraceBarLinesDetails,
        traceBarLinesAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceBarLinesDetailsAtom);
  breaksAndBarLinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBarLinesDetailsAtom);

  // bar checks

  S_oahThreeBooleansAtom
    traceBarChecksAtom =
      oahThreeBooleansAtom::create (
        "trace-bar-checks", "tbc",
R"(Bar checks)",
        "fTraceBarChecks",
        fTraceBarChecks,
        traceBarLinesAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceBarChecksAtom);
  breaksAndBarLinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBarChecksAtom);

  // bar number checks

  S_oahThreeBooleansAtom
    traceBarNumberChecksAtom =
      oahThreeBooleansAtom::create (
        "trace-bar-number-checks", "tbnc",
R"(Bar number checks)",
        "fTraceBarNumberChecks",
        fTraceBarNumberChecks,
        traceBarLinesAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceBarNumberChecksAtom);
  breaksAndBarLinesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBarNumberChecksAtom);
}

void traceOahGroup::initializeClefsToTemposTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs to tempos",
        "help-trace-clefs-to-tempos", "htctt",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'clefs to tempo' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    clefsToTemposMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-clefs-to-tempo", "tictt",
        "Trace SHORT_NAME/LONG_NAME in clefs to tempo.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      clefsToTemposMultiplexBooleansAtom);

  // clefs

  S_oahTwoBooleansAtom
    traceClefsAtom =
      oahTwoBooleansAtom::create (
        "trace-clefs", "tclefs",
R"(Clefs)",
        "fTraceClefs",
        fTraceClefs,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceClefsAtom);
  clefsToTemposMultiplexBooleansAtom->
    addBooleanAtom (
      traceClefsAtom);

  // keys

  S_oahTwoBooleansAtom
    traceKeysAtom =
      oahTwoBooleansAtom::create (
        "trace-keys", "tkeys",
R"(Keys)",
        "fTraceKeys",
        fTraceKeys,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceKeysAtom);

  S_oahThreeBooleansAtom
    traceKeysDetailsAtom =
      oahThreeBooleansAtom::create (
        "trace-keys-details", "tkeysd",
R"(Keys details)",
        "fTraceKeysDetails",
        fTraceKeysDetails,
        traceKeysAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceKeysDetailsAtom);

  // time signatures

  S_oahTwoBooleansAtom
    traceTimeSignaturesAtom =
      oahTwoBooleansAtom::create (
        "trace-time-signatures", "ttimesigs",
R"(Times)",
        "fTraceTimeSignatures",
        fTraceTimeSignatures,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTimeSignaturesAtom);

  // create the clef-key-timesignatures macro

  S_oahMacroAtom
    clefKeyTimeSignaturesMacroAtom =
      oahMacroAtom::create (
        "trace-clef-key-times-signatures", "tclefkeytimesigs",
        "To help debugging musicxml2lilypond.");

  subGroup->
    appendAtomToSubGroup (
      clefKeyTimeSignaturesMacroAtom);

  // populate it
#ifdef MF_TRACE_IS_ENABLED
  clefKeyTimeSignaturesMacroAtom->
    appendValueLessAtomToMacro (
      traceClefsAtom);
#endif // MF_TRACE_IS_ENABLED

  clefKeyTimeSignaturesMacroAtom->
    appendValueLessAtomToMacro (
      traceKeysAtom);

  clefKeyTimeSignaturesMacroAtom->
    appendValueLessAtomToMacro (
      traceTimeSignaturesAtom);

  // temps
/* JMI 0.9.68
  S_oahDeuxBooleansAtom
    traceTempsAtom =
      oahDeuxBooleansAtom::create (
        "trace-temps", "ttemps",
R"(Temps)",
        traceTimeSignaturesAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTempsAtom);
*/

  // tempos

  S_oahTwoBooleansAtom
    traceTemposAtom =
      oahTwoBooleansAtom::create (
        "trace-tempos", "ttempos",
R"(Tempos)",
        "fTraceTempos",
        fTraceTempos,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTemposAtom);
  clefsToTemposMultiplexBooleansAtom->
    addBooleanAtom (
      traceTemposAtom);
}

void traceOahGroup::initializeInterNotesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Inter-notes",
        "help-trace-inter-notes", "htin",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'inter notes' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    interNotesMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-inter-notes", "tiin",
        "Trace SHORT_NAME/LONG_NAME in inter notes.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      interNotesMultiplexBooleansAtom);

  // ties

  S_oahTwoBooleansAtom
    traceTiesAtom =
      oahTwoBooleansAtom::create (
        "trace-ties", "tties",
R"(Ties)",
        "fTraceTies",
        fTraceTies,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTiesAtom);
  interNotesMultiplexBooleansAtom->
    addBooleanAtom (
      traceTiesAtom);

  // glissandos

  S_oahTwoBooleansAtom
    traceGlissandosAtom =
      oahTwoBooleansAtom::create (
        "trace-glissandos", "tgliss",
R"(Glissandos)",
        "fTraceGlissandos",
        fTraceGlissandos,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceGlissandosAtom);
  interNotesMultiplexBooleansAtom->
    addBooleanAtom (
      traceGlissandosAtom);
}

void traceOahGroup::initializeSpannersTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Spanners",
        "help-trace-spanners", "hts",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'spanners' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    spannersMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-spanners", "tis",
        "Trace SHORT_NAME/LONG_NAME in spanners.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      spannersMultiplexBooleansAtom);

  // spanners

  S_oahTwoBooleansAtom
    traceSpannersAtom =
      oahTwoBooleansAtom::create (
        "trace-spanners", "tspans",
R"(Spanners)",
        "fTraceSpanners",
        fTraceSpanners,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSpannersAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceSpannersAtom);

  // cresc/decresc

  S_oahTwoBooleansAtom
    traceCrescDecrescsAtom =
      oahTwoBooleansAtom::create (
        "trace-cresc-decrescs", "tcrescDecrescs",
R"(Cresc/Decrescs)",
        "fTraceCrescDecrescs",
        fTraceCrescDecrescs,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceCrescDecrescsAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceCrescDecrescsAtom);

  // wedges

  S_oahTwoBooleansAtom
    traceWedgesAtom =
      oahTwoBooleansAtom::create (
        "trace-wedges", "twedges",
R"(Wedges)",
        "fTraceWedges",
        fTraceWedges,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceWedgesAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceWedgesAtom);

  // slurs

  S_oahTwoBooleansAtom
    traceSlursAtom =
      oahTwoBooleansAtom::create (
        "trace-slurs", "tslurs",
R"(Slurs)",
        "fTraceSlurs",
        fTraceSlurs,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSlursAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceSlursAtom);

  // slurs details

  S_oahThreeBooleansAtom
    traceSlursDetailsAtom =
      oahThreeBooleansAtom::create (
        "trace-slurs-details", "tslursd",
R"(Slurs details)",
        "fTraceSlursDetails",
        fTraceSlursDetails,
        traceSlursAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSlursDetailsAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceSlursDetailsAtom);

  // ligatures

  S_oahTwoBooleansAtom
    traceLigaturesAtom =
      oahTwoBooleansAtom::create (
        "trace-ligatures", "tligs",
R"(Ligatures)",
        "fTraceLigatures",
        fTraceLigatures,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceLigaturesAtom);
  spannersMultiplexBooleansAtom->
    addBooleanAtom (
      traceLigaturesAtom);
}

void traceOahGroup::initializeHarmoniesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies",
        "help-trace-harmonies", "htharms",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

/* JMI
  // the 'harmonies and figured bass elements' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    harmoniesAndFiguredBassesMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "tihafb", "trace-in-harmonies-and-figured-bass",
        "Trace SHORT_NAME/LONG_NAME in harmonies and figured bass elements.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      harmoniesAndFiguredBassesMultiplexBooleansAtom);
*/

  // harmonies basics

  S_oahBooleanAtom
    traceHarmoniesBasicsBooleanAtom =
    oahTwoBooleansAtom::create (
      "trace-harmonies-basics", "tharmsb",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
      "fTraceHarmoniesBasics",
      fTraceHarmoniesBasics,
      fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceHarmoniesBasicsBooleanAtom);

  // harmonies

  fTraceHarmoniesBooleanAtom =
    oahTwoBooleansAtom::create (
      "trace-harmonies", "tharms",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
      "fTraceHarmonies",
      fTraceHarmonies,
      traceHarmoniesBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      fTraceHarmoniesBooleanAtom);

  // harmonies details

  S_oahThreeBooleansAtom
    traceHarmoniesDetailsAtom =
      oahThreeBooleansAtom::create (
        "trace-harmonies-details", "tharmsd",
R"(<harmony/> in MusicXML, \chordmode in LilyPond, with more details)",
        "fTraceHarmoniesDetails",
        fTraceHarmoniesDetails,
        fTraceHarmoniesBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceHarmoniesDetailsAtom);

  // extra harmonies

  S_oahTwoBooleansAtom
    traceExtraHarmoniesAtom =
      oahTwoBooleansAtom::create (
        "trace-extra-harmonies", "teharms",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
        "fTraceExtraHarmonies",
        fTraceExtraHarmonies,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceExtraHarmoniesAtom);
}

void traceOahGroup::initializeFiguredBassesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Figured bass elements",
        "help-trace-figured-bass", "hthfigbass",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // figured bass basics

  S_oahBooleanAtom
    traceFiguredBassesBasicsBooleanAtom =
    oahTwoBooleansAtom::create (
      "trace-figured-basses-basics", "tfigbassesb",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
      "fTraceFiguredBassesBasics",
      fTraceFiguredBassesBasics,
      fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceFiguredBassesBasicsBooleanAtom);

  // figured bass

  fTraceFiguredBassesBooleanAtom =
    oahTwoBooleansAtom::create (
      "trace-figured-basses", "tfigbasses",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
      "fTraceFiguredBasses",
      fTraceFiguredBasses,
      traceFiguredBassesBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      fTraceFiguredBassesBooleanAtom);

  // figured bass elements details

  S_oahThreeBooleansAtom
    traceFiguredBasseseAtomDetails =
      oahThreeBooleansAtom::create (
        "trace-figured-bass-details", "tfigbassd",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond, with more details)",
        "fTraceFiguredBassesDetails",
        fTraceFiguredBassesDetails,
        fTraceFiguredBassesBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceFiguredBasseseAtomDetails);
}

void traceOahGroup::initializeCreditsToWordsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Credits to words",
        "help-trace-credits-to-words", "htctw",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'credits to words' multiplex booleans atom // JMIJMIJMI

  S_oahCommonPrefixBooleansAtom
    creditsToWordsMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-credit-to-words", "tictw",
        "Trace SHORT_NAME/LONG_NAME in 'credits to words'.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      creditsToWordsMultiplexBooleansAtom);

  // credits

  S_oahTwoBooleansAtom
    traceCreditsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-credits", "tcredits",
R"(Credits)",
        "fTraceCredits",
        fTraceCredits,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceCreditsBooleanAtom);
  creditsToWordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceCreditsBooleanAtom);

  // lyrics basics

  S_oahTwoBooleansAtom
    traceLyricsBasicxBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-lyrics-basics", "tlyricsb",
R"(Lyrics)",
        "fTraceLyricsBasics",
        fTraceLyricsBasics,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceLyricsBasicxBooleanAtom);

  // lyrics

  S_oahTwoBooleansAtom
    traceLyricsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-lyrics", "tlyrics",
R"(Lyrics)",
        "fTraceLyrics",
        fTraceLyrics,
        traceLyricsBasicxBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceLyricsBooleanAtom);
  creditsToWordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceLyricsBooleanAtom);

  // lyrics details

  S_oahThreeBooleansAtom
    traceLyricsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-lyrics-details", "tlyricsd",
R"(Lyrics in MusicXML, stanzas in MSR)",
        "fTraceLyricsDetails",
        fTraceLyricsDetails,
        traceLyricsBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceLyricsDetailsBooleanAtom);
  creditsToWordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceLyricsDetailsBooleanAtom);

  // words

  S_oahTwoBooleansAtom
    traceWordsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-words", "twords",
R"(Words)",
        "fTraceWords",
        fTraceWords,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceWordsBooleanAtom);
  creditsToWordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceWordsBooleanAtom);
}

void traceOahGroup::initializeMxsrEventsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MXSR events",
        "help-trace-chords-and-tuplets", "htcat",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // trace MXSR events

  fTraceMxsrEventsAtom =
    oahBooleanAtom::create (
      "trace-mxsr-events", "tmxsre",
R"(Write a trace of the MXSR events to standard error.)",
      "fTraceMxsrEvents",
      fTraceMxsrEvents);

  subGroup->
    appendAtomToSubGroup (
      fTraceMxsrEventsAtom);

  // display MXSR events

  fDisplayMxsrEventsAtom =
    oahBooleanAtom::create (
      "display-mxsr-events", "dmxsre",
R"(Write the MXSR events found to standard error.)",
      "fDisplayMxsrEvents",
      fDisplayMxsrEvents);

  subGroup->
    appendAtomToSubGroup (
      fDisplayMxsrEventsAtom);
}

void traceOahGroup::initializeGraceNotesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Grace notes",
        "help-trace-grace-notes", "htgn",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

//   // the chords multiplex booleans atom
//
//   S_oahCommonPrefixBooleansAtom
//     chordsMultiplexBooleansAtom =
//       oahCommonPrefixBooleansAtom::create (
//         "trace-in-chords-and-tuplets", "ticat",
//         "Trace SHORT_NAME/LONG_NAME in chords and tuplets.",
//         "SHORT_NAME",
//         "LONG_NAME",
//         fShortTracePrefix,
//         fLongTracePrefix);
//
//   subGroup->
//     appendAtomToSubGroup (
//       chordsMultiplexBooleansAtom);

  // grace notes basics

  S_oahThreeBooleansAtom
    traceGraceNotesBasicsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-grace-notes-basics", "tgracesb",
R"(Grace notes basics).
This option implies -trace-mxsr-events, -tmxsre'
and '-trace-passes, -tpasses'.)",
        "fTraceGraceNotesBasics",
        fTraceGraceNotesBasics,
        fTraceMxsrEventsAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceGraceNotesBasicsBooleanAtom);

  // grace notes

  S_oahTwoBooleansAtom
    traceGraceNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-grace-notes", "tgraces",
R"(Grace notes basics.
This option implies '-trace-grace-notes-basics, -tgracesb'
-trace-mxsr-events, -tmxsre'
and '-trace-passes, -tpasses'.)",
        "fTraceGraceNotes",
        fTraceGraceNotes,
        traceGraceNotesBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceGraceNotesBooleanAtom);
}

void traceOahGroup::initializeCueNotesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Cue notes",
        "help-trace-cue-notes", "htcn",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

//   // the chords multiplex booleans atom
//
//   S_oahCommonPrefixBooleansAtom
//     chordsMultiplexBooleansAtom =
//       oahCommonPrefixBooleansAtom::create (
//         "trace-in-chords-and-tuplets", "ticat",
//         "Trace SHORT_NAME/LONG_NAME in chords and tuplets.",
//         "SHORT_NAME",
//         "LONG_NAME",
//         fShortTracePrefix,
//         fLongTracePrefix);
//
//   subGroup->
//     appendAtomToSubGroup (
//       chordsMultiplexBooleansAtom);

  // cue notes basics

  S_oahThreeBooleansAtom
    traceCueNotesBasicsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-cue-notes-basics", "tcuesb",
R"(Cue notes basics).
This option implies -trace-mxsr-events, -tmxsre'
and '-trace-passes, -tpasses'.)",
        "fTraceCueNotesBasics",
        fTraceCueNotesBasics,
        fTraceMxsrEventsAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceCueNotesBasicsBooleanAtom);

  // cue notes

  S_oahTwoBooleansAtom
    traceCueNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-cue-notes", "tcues",
R"(Cue notes basics.
This option implies '-trace-cue-notes-basics, -tcuesb'
-trace-mxsr-events, -tmxsre'
and '-trace-passes, -tpasses'.)",
        "fTraceCueNotes",
        fTraceCueNotes,
        traceCueNotesBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceCueNotesBooleanAtom);
}

void traceOahGroup::initializeChordsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords",
        "help-trace-chords", "htc",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the chords multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    chordsMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-chords-and-tuplets", "ticat",
        "Trace SHORT_NAME/LONG_NAME in chords and tuplets.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      chordsMultiplexBooleansAtom);

  // chords basics

  S_oahThreeBooleansAtom
    traceChordsBasicsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-chords-basics", "tchordsb",
R"(Chords basic trace information.
This option implies '-trace-mxsr-events, tmxsre'
and '-trace-passes, -tpasses'.)",
        "fTraceChordsBasics",
        fTraceChordsBasics,
        fTraceMxsrEventsAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceChordsBasicsBooleanAtom);

  // chords

  S_oahTwoBooleansAtom
    traceChordsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-chords", "tchords",
R"(Chords)",
        "fTraceChords",
        fTraceChords,
        traceChordsBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceChordsBooleanAtom);
  chordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceChordsBooleanAtom);

  // chords details

  S_oahTwoBooleansAtom
    traceChordsDetailsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-chords-details", "tchordsd",
R"(Chords details.
This option implies '-trace-chords-basics, -tchordsb',
'-trace-mxsr-events, tmxsre' and '-trace-passes, -tpasses'.)",
        "fTraceChordsDetails",
        fTraceChordsDetails,
        traceChordsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceChordsDetailsBooleanAtom);
  chordsMultiplexBooleansAtom->
    addBooleanAtom (
      traceChordsDetailsBooleanAtom);
}

void traceOahGroup::initializeTupletsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tuplets",
        "help-trace-tuplets", "htt",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the chords multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    tupletsMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-chords-and-tuplets", "ticat",
        "Trace SHORT_NAME/LONG_NAME in chords and tuplets.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      tupletsMultiplexBooleansAtom);

  // tuplets basics

  S_oahThreeBooleansAtom
    traceTupletsBasicsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-tuplets-basics", "ttupsb",
  R"(Tuplets basic trace information.)",
        "fTraceTupletsBasics",
        fTraceTupletsBasics,
        fTraceMxsrEventsAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTupletsBasicsBooleanAtom);

  // tuplets

  S_oahTwoBooleansAtom
    traceTupletsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-tuplets", "ttups",
R"(Tuplets trace information.
This option implies '-trace-tuplets-basics, -ttupsb'.)",
        "fTraceTuplets",
        fTraceTuplets,
        traceTupletsBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTupletsBooleanAtom);
  tupletsMultiplexBooleansAtom->
    addBooleanAtom (
      traceTupletsBooleanAtom);

  // tuplets details

  S_oahTwoBooleansAtom
    traceTupletsDetailsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-tuplets-details", "ttupsd",
R"(Tuplets detailed information.
This option implies '-trace-tuplets-basics, -ttupsb' and '-trace-tuplets, -ttups'.)",
        "fTraceTupletsDetails",
        fTraceTupletsDetails,
        traceTupletsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTupletsDetailsBooleanAtom);
  tupletsMultiplexBooleansAtom->
    addBooleanAtom (
      traceTupletsDetailsBooleanAtom);
}

void traceOahGroup::initializeInstrumentsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Instruments",
        "help-trace-instruments", "hti",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'instruments' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    instrumentsMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-instruments", "tiinstr", // JMI 0.9.67 NOT CLEAR
        "Trace SHORT_NAME/LONG_NAME in instruments.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      instrumentsMultiplexBooleansAtom);

  // instruments

  S_oahTwoBooleansAtom
    traceInstrumentsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-instruments", "tinstr",
R"(Instruments)",
        "fTraceInstruments",
        fTraceInstruments,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceInstrumentsBooleanAtom);

  // frames

  S_oahTwoBooleansAtom
    traceFramesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-frames", "tframes",
R"(<frame/> in MusicXML, \fret-diagram in LilyPond)",
        "fTraceFrames",
        fTraceFrames,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceFramesBooleanAtom);
      /* JMI
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceFramesBooleanAtom);
      */

  // scordaturas

  S_oahTwoBooleansAtom
    traceScordaturasBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-scordaturas", "tscords",
R"(Scordaturas)",
        "fTraceScordaturas",
        fTraceScordaturas,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceScordaturasBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceScordaturasBooleanAtom);

  // slides

  S_oahTwoBooleansAtom
    traceSlidesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-slides", "tslides",
R"(Slides)",
        "fTraceSlides",
        fTraceSlides,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSlidesBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceSlidesBooleanAtom);

  // pedals

  S_oahTwoBooleansAtom
    tracePedalsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-pedals", "tpedals",
R"(Pedals)",
        "fTracePedals",
        fTracePedals,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      tracePedalsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      tracePedalsBooleanAtom);

  // accordion registrations

  S_oahTwoBooleansAtom
    traceAccordionRegistrationsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-accordion-registrations", "taccreg",
R"(Accordion registrations)",
        "fTraceAccordionRegistrations",
        fTraceAccordionRegistrations,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceAccordionRegistrationsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceAccordionRegistrationsBooleanAtom);

  // harp pedals

  S_oahTwoBooleansAtom
    traceHarpPedalsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-harp-pedals", "thpedals",
R"(Harp pedals)",
        "fTraceHarpPedals",
        fTraceHarpPedals,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceHarpPedalsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceHarpPedalsBooleanAtom);

  // harp pedals tuning

  S_oahTwoBooleansAtom
    traceHarpPedalsTuningsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-harp-pedals-tunings", "thptuns",
R"(Harp pedals tuning)",
        "fTraceHarpPedalsTunings",
        fTraceHarpPedalsTunings,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceHarpPedalsTuningsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceHarpPedalsTuningsBooleanAtom);

  // damps

  S_oahTwoBooleansAtom
    traceDampsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-damps", "tdamps",
R"(Damps)",
        "fTraceDamps",
        fTraceDamps,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceDampsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceDampsBooleanAtom);

  // dampalls

  S_oahTwoBooleansAtom
    traceDampAllsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-dampalls", "tdampalls",
R"(Dampalls)",
        "fTraceDampAlls",
        fTraceDampAlls,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceDampAllsBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceDampAllsBooleanAtom);

  // MIDI

  S_oahTwoBooleansAtom
    traceMidiBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-midi", "tmidi",
R"(MIDI)",
        "fTraceMidi",
        fTraceMidi,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMidiBooleanAtom);
  instrumentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceMidiBooleanAtom);
}

void traceOahGroup::initializeNotesAttachmentsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes attachments",
        "help-trace-notes-attachments", "htna",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'notes attachments' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    notesAttachmentsMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-notes-attachments", "tina",
        "Trace SHORT_NAME/LONG_NAME in notes attachments.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      notesAttachmentsMultiplexBooleansAtom);

  // stems

  S_oahTwoBooleansAtom
    traceStemsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-stems", "tstems",
R"(Stems)",
        "fTraceStems",
        fTraceStems,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceStemsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceStemsBooleanAtom);

  // beams

  S_oahTwoBooleansAtom
    traceBeamsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-beams", "tbeams",
R"(Beams)",
        "fTraceBeams",
        fTraceBeams,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceBeamsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceBeamsBooleanAtom);

  // articulations

  S_oahTwoBooleansAtom
    traceArticulationsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-articulations", "tarts",
R"(Articulations)",
        "fTraceArticulations",
        fTraceArticulations,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceArticulationsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceArticulationsBooleanAtom);

  // technicals

  S_oahTwoBooleansAtom
    traceTechnicalsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-technicals", "ttechs",
R"(Technicals)",
        "fTraceTechnicals",
        fTraceTechnicals,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTechnicalsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceTechnicalsBooleanAtom);

  // ornaments

  S_oahTwoBooleansAtom
    traceOrnamentsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-ornaments", "torns",
R"(Ornaments)",
        "fTraceOrnaments",
        fTraceOrnaments,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceOrnamentsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceOrnamentsBooleanAtom);

  S_oahThreeBooleansAtom
    traceOrnamentsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-ornaments-details", "tornsd",
R"(Ornaments)",
        "fTraceOrnamentsDetails",
        fTraceOrnamentsDetails,
        traceOrnamentsBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceOrnamentsDetailsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceOrnamentsDetailsBooleanAtom);

  // dynamics

  S_oahTwoBooleansAtom
    traceDynamicsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-dynamics", "tdyns",
R"(Dynamics)",
        "fTraceDynamics",
        fTraceDynamics,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceDynamicsBooleanAtom);
  notesAttachmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceDynamicsBooleanAtom);
}

void traceOahGroup::initializeSegmentsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Segments",
        "help-trace-segments", "htsegs",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'segments' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    segmentsMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-segments", "tisam",
        "Trace SHORT_NAME/LONG_NAME in segments.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      segmentsMultiplexBooleansAtom);

  // segments basics

  S_oahTwoBooleansAtom
    traceSegmentsBasicsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-segments-basics", "tsegsb",
R"(Voices segments basics)",
        "fTraceSegmentsBasics",
        fTraceSegmentsBasics,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSegmentsBasicsBooleanAtom);

  // segments

  S_oahTwoBooleansAtom
    traceSegmentsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-segments", "tsegs",
R"(Voices segments)",
        "fTraceSegments",
        fTraceSegments,
        traceSegmentsBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSegmentsBooleanAtom);
  segmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceSegmentsBooleanAtom);

  // segments details

  S_oahThreeBooleansAtom
    traceSegmentsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-segments-details", "tsegsd",
R"(Voices segments details)",
        "fTraceSegmentsDetails",
        fTraceSegmentsDetails,
        traceSegmentsBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSegmentsDetailsBooleanAtom);
  segmentsMultiplexBooleansAtom->
    addBooleanAtom (
      traceSegmentsDetailsBooleanAtom);
}

void traceOahGroup::initializeMeasuresTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "help-trace-measures", "htmeas",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'measures' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    measuresMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-measures", "timeas",
        "Trace SHORT_NAME/LONG_NAME in segments.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      measuresMultiplexBooleansAtom);

  // measure numbers

  S_oahTwoBooleansAtom
    traceMeasuresNumbersBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measures-numbers", "tmneasnums",
R"(Measure numberss)",
        "fTraceMeasuresNumbers",
        fTraceMeasuresNumbers,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasuresNumbersBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasuresNumbersBooleanAtom);

  // measures basics

  S_oahTwoBooleansAtom
    traceMeasuresBasicsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measures-basics", "tmeasb",
R"(Measures basic trace information)",
        "fTraceMeasuresBasics",
        fTraceMeasuresBasics,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasuresBasicsBooleanAtom);

  // measures

  S_oahTwoBooleansAtom
    traceMeasuresBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measures", "tmeas",
R"(Measures.
This option implies '-trace-measures-basics, -tmeasb'.)",
        "fTraceMeasures",
        fTraceMeasures,
        traceMeasuresBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasuresBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasuresBooleanAtom);

  // measures details

  S_oahTwoBooleansAtom
    traceMeasuresDetailsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measures-details", "tmeasd",
R"(Measures details.
This option implies '-trace-measures-basics, -tmeasb' and '-trace-measures, -tmeas'.)",
        "fTraceMeasuresDetails",
        fTraceMeasuresDetails,
        traceMeasuresBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasuresDetailsBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasuresDetailsBooleanAtom);

  // only measure

  fTraceOnlyMeasurerNumbeOahAtom =
    oahStringAtom::create (
      "trace-only-measure", "tom",
R"(Restrict the production of trace information to measure NUMBER)",
      "NUMBER",
      "fTraceOnlyMeasure",
      fTraceOnlyMeasure);

  subGroup->
    appendAtomToSubGroup (
      fTraceOnlyMeasurerNumbeOahAtom);

  // positions in measures

  S_oahTwoBooleansAtom
    tracePositionInMeasuresBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-positions-in-measures", "tpim",
R"(Positions in measures)",
        "fTracePositionsInMeasures",
        fTracePositionsInMeasures,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      tracePositionInMeasuresBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      tracePositionInMeasuresBooleanAtom);

  S_oahThreeBooleansAtom
    tracePositionInMeasuresDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-positions-in-measures-details", "tpimd",
R"(Positions in measures details)",
        "fTracePositionsInMeasuresDetails",
        fTracePositionsInMeasuresDetails,
        tracePositionInMeasuresBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      tracePositionInMeasuresDetailsBooleanAtom);

  // positions in voices

  S_oahTwoBooleansAtom
    tracePositionsInVoicesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-positions-in-voices", "tpiv",
R"(Positions in voices)",
        "fTracePositionsInVoices",
        fTracePositionsInVoices,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      tracePositionsInVoicesBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      tracePositionsInVoicesBooleanAtom);

  // measure moments

  S_oahTwoBooleansAtom
    traceMeasureMomentsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measure-moments", "tmeasmoms",
R"(Measure moments)",
        "fTraceMeasureMoments",
        fTraceMeasureMoments,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasureMomentsBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasureMomentsBooleanAtom);

  // voice moments

  S_oahTwoBooleansAtom
    traceVoiceMomentsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-voice-moments", "tvoicemoms",
R"(Voice moments)",
        "fTraceVoiceMoments",
        fTraceVoiceMoments,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceVoiceMomentsBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceVoiceMomentsBooleanAtom);

  // fTraceDetailedMeasureNumbersSet is empty
}

void traceOahGroup::initializeVoicesFlatViewTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices flat view",
        "help-trace-voices-flat-view", "htvoicesflatview",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // voices flat view

  S_oahTwoBooleansAtom
    traceVoicesFlatViewBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-voices-flat-view", "tvoicesflatview",
R"(Trace the voices flat view gathering activity.)",
        "fTraceVoicesFlatView",
        fTraceVoicesFlatView,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceVoicesFlatViewBooleanAtom);

  // voices flat view details

  S_oahThreeBooleansAtom
    traceVoicesFlatViewDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-voices-flat-view-details", "tvoicesflatviewd",
R"(Trace the voices flat view gathering activity with more details.)",
        "fTraceVoicesFlatViewDetails",
        fTraceVoicesFlatViewDetails,
        traceVoicesFlatViewBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceVoicesFlatViewDetailsBooleanAtom);
}

void traceOahGroup::initializeMeasuresSlicesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures slices",
        "help-trace-measures-slices", "htmeasslices",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

/* JMI
  // the 'measures slices' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    measuresSlicesMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-measures-slices", "tinmeasslices", // JMI 0.9.70
        "Trace SHORT_NAME/LONG_NAME in segments.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      measuresSlicesMultiplexBooleansAtom);
*/

  // measures slices

  S_oahTwoBooleansAtom
    traceMeasuresSlicesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measures-slices", "tmeasslices",
R"(Trace the measures slices gathering activity.)",
        "fTraceMeasuresSlices",
        fTraceMeasuresSlices,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasuresSlicesBooleanAtom);

  // measures slices details

  S_oahThreeBooleansAtom
    traceMeasuresSlicesDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-measures-slices-details", "tmeasslicesd",
R"(Trace the measures slices gathering activity details.)",
        "fTraceMeasuresSlicesDetails",
        fTraceMeasuresSlicesDetails,
        traceMeasuresSlicesBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasuresSlicesDetailsBooleanAtom);
}

void traceOahGroup::initializeBooksToVoicesTraceOah () // RENAME and SPLIT? JMI 0.9.71
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Score to voices",
        "help-trace-score-to-voices", "htstv",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'books to voices' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    bookstoVoicesMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-books-to-voices", "tibtv",
        "Trace SHORT_NAME/LONG_NAME in books to voices.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      bookstoVoicesMultiplexBooleansAtom);

  // books

  S_oahTwoBooleansAtom
    traceBooksBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-books", "tbooks",
R"(Books)",
        "fTraceBooks",
        fTraceBooks,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceBooksBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBooksBooleanAtom);

  // scores

  S_oahTwoBooleansAtom
    traceScoresBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-scores", "tscores",
R"(Score)",
        "fTraceScores",
        fTraceScores,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceScoresBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceScoresBooleanAtom);

  // part groups

  S_oahTwoBooleansAtom
    tracePartGroupsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-part-groups", "tpartgroups",
R"(Part groups)",
        "fTracePartGroups",
        fTracePartGroups,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      tracePartGroupsBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      tracePartGroupsBooleanAtom);

  // part groups details

  S_oahThreeBooleansAtom
    tracePartGroupsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-part-groups-details", "tpartgroupsd",
R"(Part groups with more details.
This option implies '-trace-part-groups, -tpgrps'.)",
        "fTracePartGroupsDetails",
        fTracePartGroupsDetails,
        tracePartGroupsBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      tracePartGroupsDetailsBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      tracePartGroupsDetailsBooleanAtom);

  // parts basic

  fTracePartsBasicsBooleanAtom =
    oahTwoBooleansAtom::create (
      "trace-parts-basics", "tpartsb",
R"(Parts basics)",
      "fTracePartsBasics",
      fTracePartsBasics,
      fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      fTracePartsBasicsBooleanAtom);

  // parts

  S_oahTwoBooleansAtom
    tracePartsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-parts", "tparts",
R"(Parts)",
        "fTraceParts",
        fTraceParts,
        fTracePartsBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      tracePartsBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      tracePartsBooleanAtom);

  // staves basics

  S_oahTwoBooleansAtom
    traceStavesBasicsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-staves-basics", "tstavesb",
R"(Staves basics)",
        "fTraceStavesBasics",
        fTraceStavesBasics,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceStavesBasicsBooleanAtom);

  // staves
  S_oahThreeBooleansAtom
    traceStavesBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-staves", "tstaves",
R"(Staves)",
        "fTraceStaves",
        fTraceStaves,
        traceStavesBasicsBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceStavesBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceStavesBooleanAtom);

  // staff details

  S_oahThreeBooleansAtom
    traceStaffDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-staves-details", "tstavesd",
R"(Staff details)",
        "fTraceStavesDetails",
        fTraceStavesDetails,
        traceStavesBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceStaffDetailsBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceStaffDetailsBooleanAtom);

  // staff changes basics

  S_oahThreeBooleansAtom
    traceStaffChangesBasicsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-staff-changes-basics", "tstchangesb",
R"(Staff changes basics.
This options implies '-trace-mxsr-events, -tmxsre' and '-trace-passes, -tpasses'.)",
        "fTraceStaffChangesBasics",
        fTraceStaffChangesBasics,
        fTraceMxsrEventsAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceStaffChangesBasicsBooleanAtom);

  // staff changes

  S_oahTwoBooleansAtom
    traceStaffChangesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-staff-changes", "tstchanges",
R"(Staff changes)",
        "fTraceStaffChanges",
        fTraceStaffChanges,
        traceStaffChangesBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceStaffChangesBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceStaffChangesBooleanAtom);

  // voices basics

  S_oahTwoBooleansAtom
    traceVoicesBasicsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-voices-basics", "tvoicesb",
R"(Voices basics)",
        "fTraceVoicesBasics",
        fTraceVoicesBasics,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceVoicesBasicsBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceVoicesBasicsBooleanAtom);

  // voices

  S_oahTwoBooleansAtom
    traceVoicesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-voices", "tvoices",
R"(Voices)
This option implies '-trace-voices-basics, -tvoicesb'.)",
        "fTraceVoices",
        fTraceVoices,
        traceVoicesBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceVoicesBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceVoicesBooleanAtom);

  // voices details

  S_oahTwoBooleansAtom
    traceVoicesDetailsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-voices-details", "tvoicesd",
R"(Voices with more details (quite verbose)...
This option implies '-trace-voices-basics, -tvoicesb' and '-trace-voices, -tvoices'.)",
        "fTraceVoicesDetails",
        fTraceVoicesDetails,
        traceVoicesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceVoicesDetailsBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceVoicesDetailsBooleanAtom);
}

void traceOahGroup::initializeDurationsTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Durations",
        "help-trace-durations", "htracedurs",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // durations

  S_oahTwoBooleansAtom
    traceDurationsBooleanAtom =
      oahTwoBooleansAtom::create ( // JMI trace too??? 0.9.67
        "trace-durations", "tdurs",
R"(Notes durations)",
        "fTraceDurations",
        fTraceDurations,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceDurationsBooleanAtom);
}

void traceOahGroup::initializeNotesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "help-trace-notes", "htn",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'notes' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    notesMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-notes", "tin",
        "Trace SHORT_NAME/LONG_NAME in notes.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      notesMultiplexBooleansAtom);

  // notes basics

  S_oahThreeBooleansAtom
    traceNotesBasicsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-notes-basics", "tnotesb",
R"(Notes basic trace information)",
        "fTraceNotesBasics",
        fTraceNotesBasics,
        fTraceMxsrEventsAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceNotesBasicsBooleanAtom);

  // notes

  S_oahTwoBooleansAtom
    traceNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-notes", "tnotes",
R"(Notes.
This option implies '-trace-notes-basics, -tnotesb'.)",
        "fTraceNotes",
        fTraceNotes,
        traceNotesBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceNotesBooleanAtom);

  // notes details

  S_oahTwoBooleansAtom
    traceNotesDetailsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-notes-details", "tnotesd",
R"(Notes with more details, including divisions handling (quite verbose)...
This option implies '-trace-notes-basics, -tnotesb' and '-trace-notes, -tnotes'.)",
        "fTraceNotesDetails",
        fTraceNotesDetails,
        traceNotesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceNotesDetailsBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceNotesDetailsBooleanAtom);

  // whole notes

  S_oahTwoBooleansAtom
    traceDurationsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-whole-notes-durations", "twhndurs", // JMI 0.9.67 SUPERFLOUS with trace-notes-durations ???
R"(Whole notes computations (quite verbose)...)",
        "fTraceDurations",
        fTraceDurations,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceDurationsBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceDurationsBooleanAtom);

  // whole notes details

  S_oahThreeBooleansAtom
    traceDurationsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-whole-notes-details", "twhndursd",
R"(Whole notes computations details (event more verbose)...)",
        "fTraceDurationsDetails",
        fTraceDurationsDetails,
        traceDurationsBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceDurationsDetailsBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceDurationsDetailsBooleanAtom);

  // measures whole notes vectors

  S_oahTwoBooleansAtom
    traceMeasuresWholeNotesVectorsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measures-whole-notes-vectors", "tmeaswhnvects",
R"(Meaures whole notes vectors)",
        "fTraceMeasuresWholeNotesVectors",
        fTraceMeasuresWholeNotesVectors,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasuresWholeNotesVectorsBooleanAtom);

  // rest notes

  S_oahTwoBooleansAtom
    traceRestNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-rest-notes", "trn",
R"(Rest notes)",
        "fTraceRestNotes",
        fTraceRestNotes,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceRestNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceRestNotesBooleanAtom);

  // skip notes

  S_oahTwoBooleansAtom
    traceSkipNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-skip-notes", "tskips",
R"(Skip notes)",
        "fTraceSkipNotes",
        fTraceSkipNotes,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSkipNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceSkipNotesBooleanAtom);

  // notes octave entry

  S_oahTwoBooleansAtom
    traceNotesOctaveEntryBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-notes-octave-entry", "tnoe",
R"(Notes octave entry)",
        "fTraceNotesOctaveEntry",
        fTraceNotesOctaveEntry,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceNotesOctaveEntryBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceNotesOctaveEntryBooleanAtom);

  // tremolos

  S_oahTwoBooleansAtom
    traceTremolosBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-tremolos", "ttrems",
R"(Tremolos)",
        "fTraceTremolos",
        fTraceTremolos,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceTremolosBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceTremolosBooleanAtom);
}

void traceOahGroup::initializeOptionsTraceAndDisplayOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options handling trace",
        "help-trace-options-handling", "htoh",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // trace options

  fTraceOahBooleanAtom =
    oahBooleanAtom::create (
      K_TRACE_OAH_OPTION_LONG_NAME, K_TRACE_OAH_OPTION_SHORT_NAME,
R"(Write a trace of options and help handling to standard error.)",
      "fTraceOah",
      fTraceOah);

  subGroup->
    appendAtomToSubGroup (
      fTraceOahBooleanAtom);

 // trace options details

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        K_TRACE_OAH_DETAILS_OPTION_LONG_NAME, K_TRACE_OAH_DETAILS_OPTION_SHORT_NAME,
R"(Write a trace of options and help handling with more details to standard error.)",
        "fTraceOahDetails",
        fTraceOahDetails,
        fTraceOahBooleanAtom));

  // fTraceDetailedMeasureNumbersSet is intially empty
}

void traceOahGroup::initializeRepeatsToSlashesTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Repeats to slashes",
        "help-trace-repeats-to-slashes", "htrts",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'repeats to slashes' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    repeatsToSlashesMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-repeats-to-slashes", "tirts",
        "Trace SHORT_NAME/LONG_NAME in repeats to slashes.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      repeatsToSlashesMultiplexBooleansAtom);

  // repeats basics

  S_oahTwoBooleansAtom
    traceRepeatsBasicsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-repeats-basics", "trepsb",
R"(Repeats basic trace information)",
        "fTraceRepeatsBasics",
        fTraceRepeatsBasics,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceRepeatsBasicsBooleanAtom);

  // repeats

  S_oahTwoBooleansAtom
    traceRepeatsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-repeats", "treps",
R"(Repeats.
This option implies '-trace-tuplets-basics, -ttupsb'.)",
        "fTraceRepeats",
        fTraceRepeats,
        traceRepeatsBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceRepeatsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceRepeatsBooleanAtom);

  // repeats details

  S_oahTwoBooleansAtom
    traceRepeatsDetailsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-repeats-details", "trepsd",
R"(Repeats details.
This option implies '-trace-tuplets-basics, -ttupsb' and '-trace-tuplets, -ttups'.)",
        "fTraceRepeatsDetails",
        fTraceRepeatsDetails,
        traceRepeatsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceRepeatsDetailsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceRepeatsDetailsBooleanAtom);

  // measure repeats basics

  S_oahTwoBooleansAtom
    traceMeasureRepeatsBasicsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measure-repeats-basics", "tmrepsb",
R"(Measure repeats basic trace information)",
        "fTraceMeasureRepeatsBasics",
        fTraceMeasureRepeatsBasics,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasureRepeatsBasicsBooleanAtom);

  // measure repeats

  S_oahTwoBooleansAtom
    traceMeasureRepeatsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measure-repeats", "tmeareps",
R"(Measure repeats.
This option implies '-trace-measure-repeats-basics, -tmrepsb'.)",
        "fTraceMeasureRepeats",
        fTraceMeasureRepeats,
        traceMeasureRepeatsBasicsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasureRepeatsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasureRepeatsBooleanAtom);

  // measure repeats details

  S_oahTwoBooleansAtom
    traceMeasureRepeatsDetailsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measure-repeats-details", "tmearepsd",
R"(Measure repeats.
This option implies '-trace-tuplets-basics, -ttupsb' and '-trace-tuplets, -ttups'.)",
        "fTraceMeasureRepeatsDetails",
        fTraceMeasureRepeatsDetails,
        traceMeasureRepeatsBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMeasureRepeatsDetailsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasureRepeatsDetailsBooleanAtom);

  // multiple measure rests basics

  S_oahTwoBooleansAtom
    traceMultipleMeasureRestsBasicsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-multiple-measure-rests-basics", "tmmrestsb",
R"(Multiple measure rests basics)",
        "fTraceMultipleMeasureRestsBasics",
        fTraceMultipleMeasureRestsBasics,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMultipleMeasureRestsBasicsBooleanAtom);

  // multiple measure rests

  S_oahTwoBooleansAtom
    traceMultipleMeasureRestsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-multiple-measure-rests", "tmmrests",
R"(Multiple measure rests)",
        "fTraceMultipleMeasureRests",
        fTraceMultipleMeasureRests,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMultipleMeasureRestsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceMultipleMeasureRestsBooleanAtom);

  // multiple measure rests

  S_oahThreeBooleansAtom
    traceMultipleMeasureRestsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-multiple-measure-rests-details", "tmmrestsd",
R"(Multiple measure rests details)",
        "fTraceMultipleMeasureRestsDetails",
        fTraceMultipleMeasureRestsDetails,
        traceMultipleMeasureRestsBooleanAtom,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceMultipleMeasureRestsDetailsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceMultipleMeasureRestsDetailsBooleanAtom);

  // beats repeats

  S_oahTwoBooleansAtom
    traceBeatRepeatsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-beats-repeats", "tbreps",
R"(Beats repeatss)",
        "fTraceBeatRepeats",
        fTraceBeatRepeats,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceBeatRepeatsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceBeatRepeatsBooleanAtom);

  // slashes

  S_oahTwoBooleansAtom
    traceSlashesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-slashes", "tslashes",
R"(Slashes)",
        "fTraceSlashes",
        fTraceSlashes,
        fTracePassesBooleanAtom);

  subGroup->
    appendAtomToSubGroup (
      traceSlashesBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceSlashesBooleanAtom);
}

void traceOahGroup::initializeTraceOahGroup ()
{
  // other trace
  // FIRST, to initialize passes trace
  initializeOtherTraceOah ();

  // options and help trace and display
  initializeOptionsTraceAndDisplayOptions ();

  // MXSR events (fTraceMxsrEventsAtom is used by other initializations)
  initializeMxsrEventsTraceOah ();

  // score to voices
  initializeBooksToVoicesTraceOah ();

  // print layouts
  initializeMusicXMLPrintLayoutsTraceOah ();

  // segments
  initializeSegmentsTraceOah ();

  // measures
  initializeMeasuresTraceOah ();

  // measures slices
  initializeMeasuresSlicesTraceOah ();

  // voices flat view
  initializeVoicesFlatViewTraceOah ();

  // durations
  initializeDurationsTraceOah ();

  // notes
  initializeNotesTraceOah ();

  // notes attachments
  initializeNotesAttachmentsTraceOah ();

  // repeats to slashes
  initializeRepeatsToSlashesTraceOah ();

  // instruments
  initializeInstrumentsTraceOah ();

  // grace notes
  initializeGraceNotesTraceOah ();

  // cue notes
  initializeCueNotesTraceOah ();

  // chords
  initializeChordsTraceOah ();

  // tuplets
  initializeTupletsTraceOah ();

  // texts
  initializeCreditsToWordsTraceOah ();

  // harmonies
  initializeHarmoniesTraceOah ();

  // figured bass elements
  initializeFiguredBassesTraceOah ();

  // spanners
  initializeSpannersTraceOah ();

  // inter-notes
  initializeInterNotesTraceOah ();

  // clefs to tempos
  initializeClefsToTemposTraceOah ();

  // breaks
  initializeBreaksAndBarLinesTraceOah ();

  // above staff
  initializeAboveStaffTraceOah ();

  //transpositions
  initializeTranspositionsTraceOah ();
}

//______________________________________________________________________________
Bool traceOahGroup::fetchTraceClefKeyTimeSignatureGroups () const
{
  return
    getTraceClefs ()
      ||
    getTraceKeys ()
      ||
    getTraceTimeSignatures ();
}

//______________________________________________________________________________
void traceOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void traceOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void traceOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> traceOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_traceOahGroup>*
    p =
      dynamic_cast<visitor<S_traceOahGroup>*> (v)) {
        S_traceOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching traceOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void traceOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> traceOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_traceOahGroup>*
    p =
      dynamic_cast<visitor<S_traceOahGroup>*> (v)) {
        S_traceOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching traceOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void traceOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> traceOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void traceOahGroup::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{}

void traceOahGroup::displayTraceOahValues (int fieldWidth)
{
  gLog <<
    "The trace options are:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "Trace:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    // options and help display
    std::setw (fieldWidth) << "fTraceOah" << ": " <<
    fTraceOah <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceOahDetails" << ": " <<
    fTraceOahDetails <<
    std::endl <<

    // geometry
    std::setw (fieldWidth) << "fTraceGeometry" << ": " <<
    fTraceGeometry <<
    std::endl <<

    // identification
    std::setw (fieldWidth) << "fTraceIdentification" << ": " <<
    fTraceIdentification <<
    std::endl <<

    // for tests, hidden
    std::setw (fieldWidth) << "fTraceForTests" << ": " <<
    fTraceForTests <<
    std::endl <<

    // credits
    std::setw (fieldWidth) << "fTraceCredits" << ": " <<
    fTraceCredits <<
    std::endl <<

    // books
    std::setw (fieldWidth) << "fTraceBooks" << ": " <<
    fTraceBooks <<
    std::endl <<

    // scores
    std::setw (fieldWidth) << "fTraceScores" << ": " <<
    fTraceScores <<
    std::endl <<

    // part groups
    std::setw (fieldWidth) << "fTracePartGroups" << ": " <<
    fTracePartGroups <<
    std::endl <<
    std::setw (fieldWidth) << "fTracePartGroupsDetails" << ": " <<
    fTracePartGroupsDetails <<
    std::endl <<

    // parts
    std::setw (fieldWidth) << "fTraceParts" << ": " <<
    fTraceParts <<
    std::endl <<

    // staves
    std::setw (fieldWidth) << "fTraceStavesBasics" << ": " <<
    fTraceStavesBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceStaves" << ": " <<
    fTraceStaves <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceStavesDetails" << ": " <<
    fTraceStavesDetails <<
    std::endl <<

    // staff changes
    std::setw (fieldWidth) << "fTraceStaffChangesBasics" << ": " <<
    fTraceStaffChangesBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceStaffChanges" << ": " <<
    fTraceStaffChanges <<
    std::endl <<

    // voices
    std::setw (fieldWidth) << "fTraceVoicesBasics" << ": " <<
    fTraceVoicesBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceVoices" << ": " <<
    fTraceVoices <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceVoicesDetails" << ": " <<
    fTraceVoicesDetails <<
    std::endl <<

    // MusicXML print layouts
    std::setw (fieldWidth) << "fTraceMusicXMLPrintLayouts" << ": " <<
    fTraceMusicXMLPrintLayouts <<
    std::endl <<

    // segments
    std::setw (fieldWidth) << "fTraceSegments" << ": " <<
    fTraceSegments <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceSegmentsDetails" << ": " <<
    fTraceSegmentsDetails <<
    std::endl <<

    // measures
    std::setw (fieldWidth) << "fTraceMeasuresNumbers" << ": " <<
    fTraceMeasuresNumbers <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceMeasuresBasics" << ": " <<
    fTraceMeasuresBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceMeasures" << ": " <<
    fTraceMeasures <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceMeasuresDetails" << ": " <<
    fTraceMeasuresDetails <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceOnlyMeasure" << ": " <<
    fTraceOnlyMeasure <<
    std::endl;

  gLog << std::left <<
    std::setw (fieldWidth) << "fTraceDetailedMeasureNumbersSet" << ": " <<
    std::endl;

  if (fTraceDetailedMeasureNumbersSet.size ()) {
//     std::set <int>::const_iterator
//       iBegin = fTraceDetailedMeasureNumbersSet.begin (),
//       iEnd   = fTraceDetailedMeasureNumbersSet.end (),
//       i      = iBegin;

    ++gIndenter;

    for (int measureNumber : fTraceDetailedMeasureNumbersSet) {
      gLog << "v " << measureNumber;
    } // for

    --gIndenter;
  }
  else {
    gLog <<
      "[EMPTY]";
  }
  gLog << std::endl;

  gLog << std::left <<
    std::setw (fieldWidth) << "fTracePositionsInMeasures" << ": " <<
    fTracePositionsInMeasures <<
    std::endl <<
    std::setw (fieldWidth) << "fTracePositionsInMeasuresDetails" << ": " <<
    fTracePositionsInMeasuresDetails <<
    std::endl <<

    std::setw (fieldWidth) << "fTracePositionsInVoices" << ": " <<
    fTracePositionsInVoices <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceMeasureMoments" << ": " <<
    fTraceMeasureMoments <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceVoiceMoments" << ": " <<
    fTraceVoiceMoments <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceVoicesFlatView" << ": " <<
    fTraceVoicesFlatView <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceVoicesFlatViewDetails" << ": " <<
    fTraceVoicesFlatViewDetails <<
    std::endl <<

    // measures slices

    std::setw (fieldWidth) << "fTraceMeasuresSlices" << ": " <<
    fTraceMeasuresSlices <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceMeasuresSlicesDetails" << ": " <<
    fTraceMeasuresSlicesDetails <<
    std::endl <<

    // clefs
    std::setw (fieldWidth) << "fTraceClefs" << ": " <<
    fTraceClefs <<
    std::endl <<

    // keys
    std::setw (fieldWidth) << "fTraceKeys" << ": " <<
    fTraceKeys <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceKeysDetails" << ": " <<
    fTraceKeysDetails <<
    std::endl <<

    // time signatures
    std::setw (fieldWidth) << "fTraceTimeSignatures" << ": " <<
    fTraceTimeSignatures <<
    std::endl <<

    // tempos
    std::setw (fieldWidth) << "fTraceTempos" << ": " <<
    fTraceTempos <<
    std::endl <<

    // breaks
    std::setw (fieldWidth) << "fTraceLineBreaks" << ": " <<
    fTraceLineBreaks <<
    std::endl <<
    std::setw (fieldWidth) << "fTracePageBreaks" << ": " <<
    fTracePageBreaks <<
    std::endl <<

    // bar lines
    std::setw (fieldWidth) << "fTraceBarLines" << ": " <<
    fTraceBarLines <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceBarLinesDetails" << ": " <<
    fTraceBarLinesDetails <<
    std::endl <<

    // bar checks
    std::setw (fieldWidth) << "fTraceBarChecks" << ": " <<
    fTraceBarChecks <<
    std::endl <<

    // bar number checks
    std::setw (fieldWidth) << "fTraceBarNumberChecks" << ": " <<
    fTraceBarNumberChecks <<
    std::endl <<

    // transpositions
    std::setw (fieldWidth) << "fTraceTranspositions" << ": " <<
    fTraceTranspositions <<
    std::endl <<

    // octave shifts
    std::setw (fieldWidth) << "fTraceOctaveShifts" << ": " <<
    fTraceOctaveShifts <<
    std::endl <<

    // repeats
    std::setw (fieldWidth) << "fTraceRepeatsBasics" << ": " <<
    fTraceRepeatsBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceRepeats" << ": " <<
    fTraceRepeats <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceRepeatsDetails" << ": " <<
    fTraceRepeatsDetails <<
    std::endl <<

    // measure repeats
    std::setw (fieldWidth) << "fTraceMeasureRepeatsBasics" << ": " <<
    fTraceMeasureRepeatsBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceMeasureRepeats" << ": " <<
    fTraceMeasureRepeats <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceMeasureRepeatsDetails" << ": " <<
    fTraceMeasureRepeatsDetails <<
    std::endl <<

    // multiple measure rests
    std::setw (fieldWidth) << "fTraceMultipleMeasureRests" << ": " <<
    fTraceMultipleMeasureRests <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceMultipleMeasureRestsDetails" << ": " <<
    fTraceMultipleMeasureRestsDetails <<
    std::endl <<

    // beats repeats
    std::setw (fieldWidth) << "fTraceBeatRepeats" << ": " <<
    fTraceBeatRepeats <<
    std::endl <<

    // slashes
    std::setw (fieldWidth) << "fTraceSlashes" << ": " <<
    fTraceSlashes <<
    std::endl <<

    // notes durations
    std::setw (fieldWidth) << "fTraceDurations" << ": " <<
    fTraceDurations <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceDurationsDetails" << ": " <<
    fTraceDurationsDetails <<
    std::endl <<

    // notes
    std::setw (fieldWidth) << "fTraceNotesBasics" << ": " <<
    fTraceNotesBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceNotes" << ": " <<
    fTraceNotes <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceNotesDetails" << ": " <<
    fTraceNotesDetails <<
    std::endl <<

    // measures whole notes vectors
    std::setw (fieldWidth) << "fTraceMeasuresWholeNotesVectors" << ": " <<
    fTraceMeasuresWholeNotesVectors <<
    std::endl <<

    // rest and skip notes
    std::setw (fieldWidth) << "fTraceRestNotes" << ": " <<
    fTraceRestNotes <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceSkipNotes" << ": " <<
    fTraceSkipNotes <<
    std::endl <<

    // notes octave entry
    std::setw (fieldWidth) << "fTraceNotesOctaveEntry" << ": " <<
    fTraceNotesOctaveEntry <<
    std::endl <<

    // grace notes
    std::setw (fieldWidth) << "fTraceGraceNotesBasics" << ": " <<
    fTraceGraceNotesBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceGraceNotes" << ": " <<
    fTraceGraceNotes <<
    std::endl <<

    // cue notes
    std::setw (fieldWidth) << "fTraceCueNotesBasics" << ": " <<
    fTraceCueNotesBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceCueNotes" << ": " <<
    fTraceCueNotes <<
    std::endl <<

    // tremolos
    std::setw (fieldWidth) << "fTraceTremolos" << ": " <<
    fTraceTremolos <<
    std::endl <<

    // stems
    std::setw (fieldWidth) << "fTraceStems" << ": " <<
    fTraceStems <<
    std::endl <<

    // beams
    std::setw (fieldWidth) << "fTraceBeams" << ": " <<
    fTraceBeams <<
    std::endl <<

    // articulations
    std::setw (fieldWidth) << "fTraceArticulations" << ": " <<
    fTraceArticulations <<
    std::endl <<

    // technicals
    std::setw (fieldWidth) << "fTraceTechnicals" << ": " <<
    fTraceTechnicals <<
    std::endl <<

    // ornaments
    std::setw (fieldWidth) << "fTraceOrnaments" << ": " <<
    fTraceOrnaments <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceOrnamentsDetails" << ": " <<
    fTraceOrnamentsDetails <<
    std::endl <<

    // dynamics
    std::setw (fieldWidth) << "fTraceDynamics" << ": " <<
    fTraceDynamics <<
    std::endl <<

    // spanners
    std::setw (fieldWidth) << "fTraceSpanners" << ": " <<
    fTraceSpanners <<
    std::endl <<

    // cresc/decresc
    std::setw (fieldWidth) << "fTraceCrescDecrescs" << ": " <<
    fTraceCrescDecrescs <<
    std::endl <<

    // wedges
    std::setw (fieldWidth) << "fTraceWedges" << ": " <<
    fTraceWedges <<
    std::endl <<

    // slurs
    std::setw (fieldWidth) << "fTraceSlurs" << ": " <<
    fTraceSlurs <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceSlursDetails" << ": " <<
    fTraceSlursDetails <<
    std::endl <<

    // ligatures
    std::setw (fieldWidth) << "fTraceLigatures" << ": " <<
    fTraceLigatures <<
    std::endl <<

    // lyrics
    std::setw (fieldWidth) << "fTraceLyricsBasics" << ": " <<
    fTraceLyricsBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceLyrics" << ": " <<
    fTraceLyrics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceLyricsDetails" << ": " <<
    fTraceLyricsDetails <<
    std::endl <<

    // MXSR events
    std::setw (fieldWidth) << "fTraceMxsrEvents" << ": " <<
    fTraceMxsrEvents <<
    std::endl <<
    std::setw (fieldWidth) << "fDisplayMxsrEvents" << ": " <<
    fDisplayMxsrEvents <<
    std::endl <<

    // words
    std::setw (fieldWidth) << "fTraceWords" << ": " <<
    fTraceWords <<
    std::endl <<

    // grace notes
    std::setw (fieldWidth) << "fTraceGraceNotesBasics" << ": " <<
    fTraceGraceNotesBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceGraceNotes" << ": " <<
    fTraceGraceNotes <<
    std::endl <<

    // cue notes
    std::setw (fieldWidth) << "fTraceCueNotesBasics" << ": " <<
    fTraceCueNotesBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceCueNotes" << ": " <<
    fTraceCueNotes <<
    std::endl <<

    // chords
    std::setw (fieldWidth) << "fTraceChordsBasics" << ": " <<
    fTraceChordsBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceChords" << ": " <<
    fTraceChords <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceChordsDetails" << ": " <<
    fTraceChordsDetails <<
    std::endl <<

    // tuplets
    std::setw (fieldWidth) << "fTraceTupletsBasics" << ": " <<
    fTraceTupletsBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceTuplets" << ": " <<
    fTraceTuplets <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceTupletsDetails" << ": " <<
    fTraceTupletsDetails <<
    std::endl <<

    // ties
    std::setw (fieldWidth) << "fTraceTies" << ": " <<
    fTraceTies <<
    std::endl <<

    // glissandos
    std::setw (fieldWidth) << "fTraceGlissandos" << ": " <<
    fTraceGlissandos <<
    std::endl <<

    // rehearsal marks
    std::setw (fieldWidth) << "fTraceRehearsalMarks" << ": " <<
    fTraceRehearsalMarks <<
    std::endl <<

    // segnos
    std::setw (fieldWidth) << "fTraceSegnos" << ": " <<
    fTraceSegnos <<
    std::endl <<

    // dal segnos
    std::setw (fieldWidth) << "fTraceDalSegnos" << ": " <<
    fTraceDalSegnos <<
    std::endl <<

    // codas
    std::setw (fieldWidth) << "fTraceCodas" << ": " <<
    fTraceCodas <<
    std::endl <<

    // eyeglases
    std::setw (fieldWidth) << "fTraceEyeGlasses" << ": " <<
    fTraceEyeGlasses <<
    std::endl <<

     // harmonies
    std::setw (fieldWidth) << "fTraceHarmoniesBasics" << ": " <<
    fTraceHarmoniesBasics <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceHarmonies" << ": " <<
    fTraceHarmonies <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceHarmoniesDetails" << ": " <<
    fTraceHarmoniesDetails <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceExtraHarmonies" << ": " <<
    fTraceExtraHarmonies <<
    std::endl <<

    // figured bass
    std::setw (fieldWidth) << "fTraceFiguredBasses" << ": " <<
    fTraceFiguredBasses <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceFiguredBassesDetails" << ": " <<
    fTraceFiguredBassesDetails <<
    std::endl <<

    // instruments
    std::setw (fieldWidth) << "fTraceInstruments" << ": " <<
    fTraceInstruments <<
    std::endl <<

    // frames
    std::setw (fieldWidth) << "fTraceFrames" << ": " <<
    fTraceFrames <<
    std::endl <<

    // scordaturas
    std::setw (fieldWidth) << "fTraceScordaturas" << ": " <<
    fTraceScordaturas <<
    std::endl <<

    // slides
    std::setw (fieldWidth) << "fTraceSlides" << ": " <<
    fTraceSlides <<
    std::endl <<

    // pedals
    std::setw (fieldWidth) << "fTracePedals" << ": " <<
    fTracePedals <<
    std::endl <<

    // accordion registrations
    std::setw (fieldWidth) << "fTraceAccordionRegistrations" << ": " <<
    fTraceAccordionRegistrations <<
    std::endl <<

    // harp pedals
    std::setw (fieldWidth) << "fTraceHarpPedals" << ": " <<
    fTraceHarpPedals <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceHarpPedalsTunings" << ": " <<
    fTraceHarpPedalsTunings <<
    std::endl <<

    // damps
    std::setw (fieldWidth) << "fTraceDamps" << ": " <<
    fTraceDamps <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceDampAlls" << ": " <<
    fTraceDampAlls <<
    std::endl <<


/* JMI
    // msrStreams
    std::setw (fieldWidth) << "fTraceMsrStreams" << ": " <<
    fTraceMsrStreams <<
    std::endl <<
*/

    // midi
    std::setw (fieldWidth) << "fTraceMidi" << ": " <<
    fTraceMidi <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_traceOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//________________________________________________________________________
// hidden global trace OAH variable
EXP S_traceOahGroup pGlobalTraceOahGroup;

EXP S_traceOahGroup getGlobalTraceOahGroup ()
{
  return pGlobalTraceOahGroup;
}

//______________________________________________________________________________
S_traceOahGroup createGlobalTraceOahGroup (
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

//     ss <<
    gLog << // JMI ??? 0.9.71 OAH_OAH
      "Creating global trace OAH group 1" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation (
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! pGlobalTraceOahGroup) {
    // create the global OAH group
    pGlobalTraceOahGroup =
      traceOahGroup::create (
        handler);
    assert (pGlobalTraceOahGroup != 0);
  }

  // return the global OAH group
  return pGlobalTraceOahGroup;
}


}


#endif // MF_TRACE_IS_ENABLED



/* JMI
  // traceDetailedMeasureNumbersSet
  // --------------------------------------

  {
    S_oahSubGroup
      traceAndDisplaySubGroup =
        oahSubGroup::create (
          "Trace and display",
          "htd", "help-trace-and-display",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

    appendSubGroupToGroup (traceAndDisplaySubGroup);

  // detailed measure numbers set

  // fTraceDetailedMeasureNumbersSet is empty

    traceAndDisplaySubGroup->
      appendAtomToSubGroup (
        oahNaturalNumbersSetElementAtom::create (
          "trace-detailed", "tdetail",
R"('measureNumbersSet' has a form such as '0,2-14,^8-10',
where '^' excludes the corresponding numbers interval
and 0 applies to the '<part-list>' and anacrusis if present.
The measure numbers should be those of the produced score,
since MusicXML measure numbers are arbitrary strings.
Generate a detailed trace of the activity and print additional
debugging information to standard error for the specified measures.)",
          "measureNumbersSet",
          "traceDetailedMeasureNumbersSet",
          fTraceDetailedMeasureNumbersSet));
  }
*/


