/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfStringsHandling.h"

#include "enableTracingIfDesired.h"

#ifdef TRACING_IS_ENABLED // encompasses this whole file

#include <iomanip>      // std::setw, std::setprecision, ...

#include "tracingOah.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_tracingOahGroup gGlobalTracingOahGroup;

S_tracingOahGroup tracingOahGroup::create (
  S_oahHandler handler)
{
  tracingOahGroup* o = new tracingOahGroup (
    handler);
  assert (o != nullptr);

  return o;
}

tracingOahGroup::tracingOahGroup (
  S_oahHandler handler)
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

  initializeTracingOahGroup ();
}

tracingOahGroup::~tracingOahGroup ()
{}

void tracingOahGroup::createTheTracePrefixes (S_oahHandler handler)
{
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
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
      "trace-", "trace",
      "'-trace=abc,yz' is equivalent to '-trace-abc, -trace-yz'");
  handler->
    registerPrefixInHandler (
      fLongTracePrefix);

  --gIndenter;
}

void tracingOahGroup::initializePrintLayoutstracingOah ()
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

  // print layouts

  S_oahTwoBooleansAtom
    tracePrintLayoutsAtom =
      oahTwoBooleansAtom::create (
        "trace-print-layouts", "tpl",
R"(Print layouts)",
        "fTracePrintLayouts",
        fTracePrintLayouts,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      tracePrintLayoutsAtom);
}

void tracingOahGroup::initializeOthertracingOah ()
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
        K_TRACE_COMPONENTS_LONG_OPTION_NAME, K_TRACE_COMPONENTS_SHORT_OPTION_NAME,
  R"(Write a trace of components handling to standard error.)",
        "fTraceComponents",
        fTraceComponents));

  // passes

  fTracePassesBooleanAtom =
    oahBooleanAtom::create (
      K_TRACE_PASSES_LONG_OPTION_NAME, K_TRACE_PASSES_SHORT_OPTION_NAME,
R"(Write a trace of the passes to standard error.)",
      "fTracePasses",
      fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      fTracePassesBooleanAtom);
  otherMultiplexBooleansAtom->
    addBooleanAtom (
      fTracePassesBooleanAtom);

  // geometry

  S_oahTwoBooleansAtom
    traceGeometryBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-geometry", "tgeom",
R"(Scaling)",
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

  // '-tft, -trace-for-tests' is hidden... JMI

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

void tracingOahGroup::initializeTranspositionstracingOah ()
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
        "trace-octaves-shifts", "toctshifts",
R"(Octave shifts (<octave-shift/> in MusicXML, \ottava in LilyPond))",
        "fTraceOctaveShifts",
        fTraceOctaveShifts,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceOctaveShiftsAtom);
}

void tracingOahGroup::initializeAboveStaffTracingOah ()
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

void tracingOahGroup::initializeBreaksAndBarLinestracingOah ()
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
        "trace-barlines", "tblines",
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
        "trace-barlines-details", "tblinesd",
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

void tracingOahGroup::initializeClefsToTempostracingOah ()
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
        "trace-times-signatures", "ttimesigs",
R"(Times)",
        "fTraceTimeSignatures",
        fTraceTimeSignatures,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceTimeSignaturesAtom);

  // temps
/* JMI
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

void tracingOahGroup::initializeInterNotestracingOah ()
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

void tracingOahGroup::initializeSpannerstracingOah ()
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

void tracingOahGroup::initializeHarmoniestracingOah ()
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

  // harmonies

  fTraceHarmoniesBooleanAtom =
    oahTwoBooleansAtom::create (
      "trace-harmonies", "tharms",
R"(<harmony/> in MusicXML, \chordmode in LilyPond)",
      "fTraceHarmonies",
      fTraceHarmonies,
      fTracePassesBooleanAtom);
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

void tracingOahGroup::initializeFiguredBassestracingOah ()
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

  // figured bass

  fTraceFiguredBassBooleanAtom =
    oahTwoBooleansAtom::create (
      "trace-figured-bass", "tfigbass",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond)",
      "fTraceFiguredBass",
      fTraceFiguredBass,
      fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      fTraceFiguredBassBooleanAtom);

  // figured bass elements details

  S_oahThreeBooleansAtom
    traceFiguredBasseAtomDetails =
      oahThreeBooleansAtom::create (
        "trace-figured-bass-details", "tfigbassd",
R"(<figured-bass> in MusicXML, \figuremode in LilyPond, with more details)",
        "fTraceFiguredBassDetails",
        fTraceFiguredBassDetails,
        fTraceFiguredBassBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceFiguredBasseAtomDetails);
}

void tracingOahGroup::initializeCreditsToWordstracingOah ()
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

  // lyrics

  S_oahTwoBooleansAtom
    traceLyricsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-lyrics", "tlyrics",
R"(Lyrics)",
        "fTraceLyrics",
        fTraceLyrics,
        fTracePassesBooleanAtom);
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

void tracingOahGroup::initializeChordsAndTupletstracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords and tuplets",
        "help-trace-chords-and-tuplets", "htcat",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'chords and tuplets' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    chordsAndTupletsMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-in-chords-and-tuplets", "ticat",
        "Trace SHORT_NAME/LONG_NAME in chords and tuplets.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      chordsAndTupletsMultiplexBooleansAtom);

  // chords

  S_oahTwoBooleansAtom
    taceChordsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-chords", "tchords",
R"(Chords)",
        "fTraceChords",
        fTraceChords,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      taceChordsBooleanAtom);
  chordsAndTupletsMultiplexBooleansAtom->
    addBooleanAtom (
      taceChordsBooleanAtom);

  // chords details

  S_oahThreeBooleansAtom
    traceChordsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-chords-details", "tchordsd",
R"(Chords details)",
        "fTraceChordsDetails",
        fTraceChordsDetails,
        taceChordsBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceChordsDetailsBooleanAtom);
  chordsAndTupletsMultiplexBooleansAtom->
    addBooleanAtom (
      traceChordsDetailsBooleanAtom);

  // tuplets

  S_oahTwoBooleansAtom
    traceTupletsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-tuplets", "ttups",
R"(Tuplets)",
        "fTraceTuplets",
        fTraceTuplets,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceTupletsBooleanAtom);
  chordsAndTupletsMultiplexBooleansAtom->
    addBooleanAtom (
      traceTupletsBooleanAtom);

  // tuplets details

  S_oahThreeBooleansAtom
    traceTupletsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-tuplets-details", "ttupsd",
R"(Tuplets details)",
        "fTraceTupletsDetails",
        fTraceTupletsDetails,
        traceTupletsBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceTupletsDetailsBooleanAtom);
  chordsAndTupletsMultiplexBooleansAtom->
    addBooleanAtom (
      traceTupletsDetailsBooleanAtom);
}

void tracingOahGroup::initializeInstrumentstracingOah ()
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
        "trace-in-instruments", "tii",
        "Trace SHORT_NAME/LONG_NAME in instruments.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortTracePrefix,
        fLongTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      instrumentsMultiplexBooleansAtom);

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

void tracingOahGroup::initializeNotesAttachmentstracingOah ()
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

void tracingOahGroup::initializeSegmentstracingOah ()
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

  // segments

  S_oahTwoBooleansAtom
    traceSegmentsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-segments", "tsegs",
R"(Voices segments)",
        "fTraceSegments",
        fTraceSegments,
        fTracePassesBooleanAtom);
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

void tracingOahGroup::initializeMeasurestracingOah ()
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
        "trace-measure-numbers", "tmn",
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

  // measures

  S_oahTwoBooleansAtom
    traceMeasuresBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measures", "tmeas",
R"(Measures)",
        "fTraceMeasures",
        fTraceMeasures,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMeasuresBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasuresBooleanAtom);

  // measures details

  S_oahThreeBooleansAtom
    traceMeasuresDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-measures-details", "tmeasd",
R"(Measures details)",
        "fTraceMeasuresDetails",
        fTraceMeasuresDetails,
        traceMeasuresBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMeasuresDetailsBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasuresDetailsBooleanAtom);

  // positions in measures

  S_oahTwoBooleansAtom
    traceMeasurePositionsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-positions-in-measures", "tpim",
R"(Positions in measures)",
        "fTraceMeasurePositions",
        fTraceMeasurePositions,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMeasurePositionsBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasurePositionsBooleanAtom);

  // moments in measures

  S_oahTwoBooleansAtom
    traceMomentsInMeasuresBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-moments-in-measures", "tmim",
R"(Moments in measures)",
        "fTraceMomentsInMeasures",
        fTraceMomentsInMeasures,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMomentsInMeasuresBooleanAtom);
  measuresMultiplexBooleansAtom->
    addBooleanAtom (
      traceMomentsInMeasuresBooleanAtom);

  // fTraceDetailedMeasureNumbersSet is empty
}

void tracingOahGroup::initializeMeasuresSlicestracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures slices",
        "help-trace-measures-slices", "htslices",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

/* JMI
  // the 'measures slices' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    measuresSlicesMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "tislices", "trace-in-measures-slices",
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
        "trace-measures-slices", "tslices",
R"(Measures slices)",
        "fTraceMeasuresSlices",
        fTraceMeasuresSlices,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMeasuresSlicesBooleanAtom);
//   measuresSlicesMultiplexBooleansAtom->
//     addBooleanAtom (
//       traceMeasuresSlicesBooleanAtom);

  // measures slices details

  S_oahThreeBooleansAtom
    traceMeasuresSlicesDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-measures-slices-details", "tslicesd",
R"(Measures slices details)",
        "fTraceMeasuresSlicesDetails",
        fTraceMeasuresSlicesDetails,
        traceMeasuresSlicesBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMeasuresSlicesDetailsBooleanAtom);
//   measuresSlicesMultiplexBooleansAtom->
//     addBooleanAtom (
//       traceMeasuresSlicesDetailsBooleanAtom);
}

void tracingOahGroup::initializeBooksToVoicestracingOah ()
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
        "trace-part-groups", "tpgroups",
R"(Part groups)",
        "trafTracePartGroupscePartGroups",
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
        "trace-part-groups-details", "tpgroupsd",
R"(Part groups with more details.
This option implies '-tpgrps, -trace-part-groups'.)",
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

  // parts

  S_oahTwoBooleansAtom
    tracePartsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-parts", "tparts",
R"(Parts)",
        "fTraceParts",
        fTraceParts,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      tracePartsBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      tracePartsBooleanAtom);

  // staves

  S_oahTwoBooleansAtom
    traceStavesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-staves", "tstaves",
R"(Staves)",
        "fTraceStaves",
        fTraceStaves,
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
        "trace-staff-details", "tstd",
R"(Staff details)",
        "fTraceStaffDetails",
        fTraceStaffDetails,
        traceStavesBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceStaffDetailsBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceStaffDetailsBooleanAtom);

  // staff changes

  S_oahTwoBooleansAtom
    traceStaffChangesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-staff-changes", "tschanges",
R"(Staff changes)",
        "fTraceStaffChanges",
        fTraceStaffChanges,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceStaffChangesBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceStaffChangesBooleanAtom);

  // voices

  S_oahTwoBooleansAtom
    traceVoicesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-voices", "tvoices",
R"(Voices)",
        "fTraceVoices",
        fTraceVoices,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceVoicesBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceVoicesBooleanAtom);

  // voices details

  S_oahThreeBooleansAtom
    traceVoicesDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-voices-details", "tvoicesd",
R"(Voices with more details (quite verbose)..
This option implies '-tvoices, -trace-voices'.)",
        "fTraceVoicesDetails",
        fTraceVoicesDetails,
        traceVoicesBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceVoicesDetailsBooleanAtom);
  bookstoVoicesMultiplexBooleansAtom->
    addBooleanAtom (
      traceVoicesDetailsBooleanAtom);
}

void tracingOahGroup::initializeNotestracingOah ()
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

  // notes

  S_oahTwoBooleansAtom
    traceNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-notes", "tnotes",
R"(Notes)",
        "fTraceNotes",
        fTraceNotes,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceNotesBooleanAtom);

  // notes details

  S_oahThreeBooleansAtom
    traceNotesDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-notes-details", "tnotesd",
R"(Notes with more details, including divisions handling (quite verbose)...
This option implies '-tnotes, -trace-notes'.)",
        "fTraceNotesDetails",
        fTraceNotesDetails,
        traceNotesBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceNotesDetailsBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceNotesDetailsBooleanAtom);

  // whole notes

  S_oahTwoBooleansAtom
    traceWholeNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-whole-notes", "twn",
R"(Whole notes computations (quite verbose)...)",
        "fTraceWholeNotes",
        fTraceWholeNotes,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceWholeNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceWholeNotesBooleanAtom);

  // whole notes details

  S_oahThreeBooleansAtom
    traceWholeNotesDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-whole-notes-details", "twnd",
R"(Whole notes computations details (event more verbose)...)",
        "fTraceWholeNotesDetails",
        fTraceWholeNotesDetails,
        traceWholeNotesBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceWholeNotesDetailsBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceWholeNotesDetailsBooleanAtom);

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

  // grace notes

  S_oahTwoBooleansAtom
    traceGraceNotesBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-gracenotes", "tgraces",
R"(Grace notes)",
        "fTraceGraceNotes",
        fTraceGraceNotes,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceGraceNotesBooleanAtom);
  notesMultiplexBooleansAtom->
    addBooleanAtom (
      traceGraceNotesBooleanAtom);

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

void tracingOahGroup::initializeOptionsTraceAndDisplayOptions ()
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

  fTracingOahBooleanAtom =
    oahBooleanAtom::create (
      K_TRACE_OAH_LONG_OPTION_NAME, K_TRACE_OAH_SHORT_OPTION_NAME,
R"(Write a trace of options and help handling to standard error.)",
      "fTracingOah",
      fTracingOah);

  subGroup->
    appendAtomToSubGroup (
      fTracingOahBooleanAtom);

 // trace options details

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        K_TRACE_OAH_DETAILS_LONG_OPTION_NAME, K_TRACE_OAH_DETAILS_SHORT_OPTION_NAME,
R"(Write a trace of options and help handling with more details to standard error.)",
        "fTracingOahDetails",
        fTracingOahDetails,
        fTracingOahBooleanAtom));

  // fTraceDetailedMeasureNumbersSet is intially empty
}

void tracingOahGroup::initializeRepeatsToSlashestracingOah ()
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

  // repeats

  S_oahTwoBooleansAtom
    traceRepeatsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-repeats", "treps",
R"(Repeats)",
        "fTraceRepeats",
        fTraceRepeats,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceRepeatsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceRepeatsBooleanAtom);

  // repeats details

  S_oahThreeBooleansAtom
    traceRepeatsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-repeats-details", "trepsd",
R"(Repeats details)",
        "fTraceRepeatsDetails",
        fTraceRepeatsDetails,
        traceRepeatsBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceRepeatsDetailsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceRepeatsDetailsBooleanAtom);

  // measure repeats

  S_oahTwoBooleansAtom
    traceMeasureRepeatsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-measures-repeats", "tmreps",
R"(Measure repeats)",
        "fTraceMeasureRepeats",
        fTraceMeasureRepeats,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMeasureRepeatsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasureRepeatsBooleanAtom);

  S_oahThreeBooleansAtom
    traceMeasureRepeatsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-measures-repeats-details", "tmrepsd",
R"(Measure repeats)",
        "fTraceMeasureRepeatsDetails",
        fTraceMeasureRepeatsDetails,
        traceMeasureRepeatsBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMeasureRepeatsDetailsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceMeasureRepeatsDetailsBooleanAtom);

  // multiple full-bar rests

  S_oahTwoBooleansAtom
    traceMultipleFullBarRestsBooleanAtom =
      oahTwoBooleansAtom::create (
        "trace-multiple-full-bar-rests", "tmfbr",
R"(Full-bar rests)",
        "fTraceMultipleFullBarRests",
        fTraceMultipleFullBarRests,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMultipleFullBarRestsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceMultipleFullBarRestsBooleanAtom);

  S_oahThreeBooleansAtom
    traceMultipleFullBarRestsDetailsBooleanAtom =
      oahThreeBooleansAtom::create (
        "trace-multiple-full-bar-rests-details", "tmfbrd",
R"(Full-bar rests details)",
        "fTraceMultipleFullBarRestsDetails",
        fTraceMultipleFullBarRestsDetails,
        traceMultipleFullBarRestsBooleanAtom,
        fTracePassesBooleanAtom);
  subGroup->
    appendAtomToSubGroup (
      traceMultipleFullBarRestsDetailsBooleanAtom);
  repeatsToSlashesMultiplexBooleansAtom->
    addBooleanAtom (
      traceMultipleFullBarRestsDetailsBooleanAtom);

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

void tracingOahGroup::initializeTracingOahGroup ()
{
  // other trace
  // FIRST, to initialize passes tracing
  initializeOthertracingOah ();

  // options and help trace and display
  initializeOptionsTraceAndDisplayOptions ();

  // score to voices
  initializeBooksToVoicestracingOah ();

  // print layouts
  initializePrintLayoutstracingOah ();

  // segments
  initializeSegmentstracingOah ();

  // measures
  initializeMeasurestracingOah ();

  // measures slices
  initializeMeasuresSlicestracingOah ();

  // notes
  initializeNotestracingOah ();

  // notes attachments
  initializeNotesAttachmentstracingOah ();

  // repeats to slashes
  initializeRepeatsToSlashestracingOah ();

  // instruments
  initializeInstrumentstracingOah ();

  // chords and tuplets
  initializeChordsAndTupletstracingOah ();

  // texts
  initializeCreditsToWordstracingOah ();

  // harmonies
  initializeHarmoniestracingOah ();

  // figured bass elements
  initializeFiguredBassestracingOah ();

  // spanners
  initializeSpannerstracingOah ();

  // inter-notes
  initializeInterNotestracingOah ();

  // clefs to tempos
  initializeClefsToTempostracingOah ();

  // breaks
  initializeBreaksAndBarLinestracingOah ();

  // above staff
  initializeAboveStaffTracingOah ();

  //transpositions
  initializeTranspositionstracingOah ();
}

//______________________________________________________________________________
void tracingOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void tracingOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void tracingOahGroup::acceptIn (basevisitor* v)
{
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> tracingOahGroup::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_tracingOahGroup>*
    p =
      dynamic_cast<visitor<S_tracingOahGroup>*> (v)) {
        S_tracingOahGroup elem = this;

        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching tracingOahGroup::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void tracingOahGroup::acceptOut (basevisitor* v)
{
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> tracingOahGroup::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_tracingOahGroup>*
    p =
      dynamic_cast<visitor<S_tracingOahGroup>*> (v)) {
        S_tracingOahGroup elem = this;

        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching tracingOahGroup::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void tracingOahGroup::browseData (basevisitor* v)
{
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> tracingOahGroup::browseData ()" <<
      std::endl;
  }

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void tracingOahGroup::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int      valueFieldWidth) const
{}

void tracingOahGroup::printtracingOahValues (int fieldWidth)
{
  gLogStream <<
    "The trace options are:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "Trace:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    // options and help display
    std::setw (fieldWidth) << "fTracingOah" << " : " <<
    fTracingOah <<
    std::endl <<
    std::setw (fieldWidth) << "fTracingOahDetails" << " : " <<
    fTracingOahDetails <<
    std::endl <<

    // history
    std::setw (fieldWidth) << "fTtracecomponents" << " : " <<
    fTraceComponents <<
    std::endl <<

    // passes
    std::setw (fieldWidth) << "fTtracePasses" << " : " <<
    fTracePasses <<
    std::endl <<

    // for tests, hidden
    std::setw (fieldWidth) << "fTtraceForTests" << " : " <<
    fTraceForTests <<
    std::endl <<

    // score
    std::setw (fieldWidth) << "fTtraceScores" << " : " <<
    fTraceScores <<
    std::endl <<

    // identification
    std::setw (fieldWidth) << "fTtraceIdentification" << " : " <<
    fTraceIdentification <<
    std::endl <<

    // credits
    std::setw (fieldWidth) << "fTtraceCredits" << " : " <<
    fTraceCredits <<
    std::endl <<

    // scaling
    std::setw (fieldWidth) << "fTtraceGeometry" << " : " <<
    fTraceGeometry <<
    std::endl <<

    // part groups
    std::setw (fieldWidth) << "fTtracePartGroups" << " : " <<
    fTracePartGroups <<
    std::endl <<
    std::setw (fieldWidth) << "fTtracePartGroupsDetails" << " : " <<
    fTracePartGroupsDetails <<
    std::endl <<

    // parts
    std::setw (fieldWidth) << "fTtraceParts" << " : " <<
    fTraceParts <<
    std::endl <<

    // staves
    std::setw (fieldWidth) << "fTtraceStaves" << " : " <<
    fTraceStaves <<
    std::endl <<

    // voices
    std::setw (fieldWidth) << "fTtraceVoices" << " : " <<
    fTraceVoices <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceVoicesDetails" << " : " <<
    fTraceVoicesDetails <<
    std::endl <<

    // measures
    std::setw (fieldWidth) << "fTtraceMeasuresNumbers" << " : " <<
    fTraceMeasuresNumbers <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceMeasures" << " : " <<
    fTraceMeasures <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceMeasuresDetails" << " : " <<
    fTraceMeasuresDetails <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceMeasurePositions" << " : " <<
    fTraceMeasurePositions <<
    std::endl;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fTtraceDetailedMeasureNumbersSet" << " : " <<
    std::endl;

  if (fTraceDetailedMeasureNumbersSet.size ()) {
    std::set<int>::const_iterator
      iBegin = fTraceDetailedMeasureNumbersSet.begin (),
      iEnd   = fTraceDetailedMeasureNumbersSet.end (),
      i      = iBegin;

    ++gIndenter;

    for ( ; ; ) {
      gLogStream << "v " << (*i);
      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }
  else {
    gLogStream <<
      "[NONE]";
  }
  gLogStream << std::endl;

    // segments
  gLogStream << std::left <<
    std::setw (fieldWidth) << "fTtraceSegments" << " : " <<
    fTraceSegments <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceSegmentsDetails" << " : " <<
    fTraceSegmentsDetails <<
    std::endl <<

    // clefs
    std::setw (fieldWidth) << "fTtraceClefs" << " : " <<
    fTraceClefs <<
    std::endl <<

    // keys
    std::setw (fieldWidth) << "fTtraceKeys" << " : " <<
    fTraceKeys <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceKeysDetails" << " : " <<
    fTraceKeysDetails <<
    std::endl <<

    // time signatures
    std::setw (fieldWidth) << "fTtraceTimeSignatures" << " : " <<
    fTraceTimeSignatures <<
    std::endl <<

    // tempos
    std::setw (fieldWidth) << "fTtraceTempos" << " : " <<
    fTraceTempos <<
    std::endl <<

    // rehearsal marks
    std::setw (fieldWidth) << "fTtraceRehearsals" << " : " <<
    fTraceRehearsalMarks <<
    std::endl <<

    // line breaks
    std::setw (fieldWidth) << "fTtraceLineBreaks" << " : " <<
    fTraceLineBreaks <<
    std::endl <<

    // page and line breaks
    std::setw (fieldWidth) << "fTtracePageBreaks" << " : " <<
    fTracePageBreaks <<
    std::endl <<

    // staff changes
    std::setw (fieldWidth) << "fTtraceStaffChanges" << " : " <<
    fTraceStaffChanges <<
    std::endl <<

    // transpositions
    std::setw (fieldWidth) << "fTtraceTranspositions" << " : " <<
    fTraceTranspositions <<
    std::endl <<

    // octave shifts
    std::setw (fieldWidth) << "fTtraceOctaveShifts" << " : " <<
    fTraceOctaveShifts <<
    std::endl <<

    // bar lines
    std::setw (fieldWidth) << "fTtraceBarLines" << " : " <<
    fTraceBarLines <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceBarLinesDetails" << " : " <<
    fTraceBarLinesDetails <<
    std::endl <<

    // bar checks
    std::setw (fieldWidth) << "fTtraceBarChecks" << " : " <<
    fTraceBarChecks <<
    std::endl <<

    // bar number checks
    std::setw (fieldWidth) << "fTtraceBarNumberChecks" << " : " <<
    fTraceBarNumberChecks <<
    std::endl <<

    // repeats
    std::setw (fieldWidth) << "fTtraceRepeats" << " : " <<
    fTraceRepeats <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceRepeatsDetails" << " : " <<
    fTraceRepeatsDetails <<
    std::endl <<

    // measure repeats
    std::setw (fieldWidth) << "fTtraceMeasureRepeats" << " : " <<
    fTraceMeasureRepeats <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceMeasureRepeatsDetails" << " : " <<
    fTraceMeasureRepeatsDetails <<
    std::endl <<

    // multiple full-bar rests
    std::setw (fieldWidth) << "fTtraceMultipleFullBarRests" << " : " <<
    fTraceMultipleFullBarRests <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceMultipleFullBarRestsDetails" << " : " <<
    fTraceMultipleFullBarRestsDetails <<
    std::endl <<

    // slashes
    std::setw (fieldWidth) << "fTtraceSlashes" << " : " <<
    fTraceSlashes <<
    std::endl <<

    // notes
    std::setw (fieldWidth) << "fTtraceNotes" << " : " <<
    fTraceNotes <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceNotesDetails" << " : " <<
    fTraceNotesDetails <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceWholeNotes" << " : " <<
    fTraceWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceWholeNotesDetails" << " : " <<
    fTraceWholeNotesDetails <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceRestNotes" << " : " <<
    fTraceRestNotes <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceSkipNotes" << " : " <<
    fTraceSkipNotes <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceNotesOctaveEntry" << " : " <<
    fTraceNotesOctaveEntry <<
    std::endl <<

    // stems
    std::setw (fieldWidth) << "fTtraceStems" << " : " <<
    fTraceStems <<
    std::endl <<

    // beams
    std::setw (fieldWidth) << "fTtraceBeams" << " : " <<
    fTraceBeams <<
    std::endl <<

    // articulations
    std::setw (fieldWidth) << "fTtraceArticulations" << " : " <<
    fTraceArticulations <<
    std::endl <<

    // technicals
    std::setw (fieldWidth) << "fTtraceTechnicals" << " : " <<
    fTraceTechnicals <<
    std::endl <<

    // ornaments
    std::setw (fieldWidth) << "fTtraceOrnaments" << " : " <<
    fTraceOrnaments <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceOrnamentsDetails" << " : " <<
    fTraceOrnamentsDetails <<
    std::endl <<

    // dynamics
    std::setw (fieldWidth) << "fTtraceGraceDynamics" << " : " <<
    fTraceDynamics <<
    std::endl <<

    // spanners
    std::setw (fieldWidth) << "fTtraceSpanners" << " : " <<
    fTraceSpanners <<
    std::endl <<

    // words
    std::setw (fieldWidth) << "fTtraceWords" << " : " <<
    fTraceWords <<
    std::endl <<

    // tremolos
    std::setw (fieldWidth) << "fTtraceTremolos" << " : " <<
    fTraceTremolos <<
    std::endl <<

    // chords
    std::setw (fieldWidth) << "fTtraceChords" << " : " <<
    fTraceChords <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceChordsDetails" << " : " <<
    fTraceChordsDetails <<
    std::endl <<

    // tuplets
    std::setw (fieldWidth) << "fTtraceTuplets" << " : " <<
    fTraceTuplets <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceTupletsDetails" << " : " <<
    fTraceTupletsDetails <<
    std::endl <<

    // glissandos
    std::setw (fieldWidth) << "fTtraceGlissandos" << " : " <<
    fTraceGlissandos <<
    std::endl <<

    // eyeglases
    std::setw (fieldWidth) << "fTtraceEyeGlasses" << " : " <<
    fTraceEyeGlasses <<
    std::endl <<

    // damps
    std::setw (fieldWidth) << "fTtraceDamps" << " : " <<
    fTraceDamps <<
    std::endl <<

    // dampalls
    std::setw (fieldWidth) << "fTtraceDampAlls" << " : " <<
    fTraceDampAlls <<
    std::endl <<

    // slides
    std::setw (fieldWidth) << "fTtraceSlides" << " : " <<
    fTraceSlides <<
    std::endl <<

    // grace notes
    std::setw (fieldWidth) << "fTtraceGraceNotes" << " : " <<
    fTraceGraceNotes <<
    std::endl <<

    // lyrics
    std::setw (fieldWidth) << "fTtraceLyrics" << " : " <<
    fTraceLyrics <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceLyricsDetails" << " : " <<
    fTraceLyricsDetails <<
    std::endl <<

    // harmonies
    std::setw (fieldWidth) << "fTtraceHarmonies" << " : " <<
    fTraceHarmonies <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceHarmoniesDetails" << " : " <<
    fTraceHarmoniesDetails <<
    std::endl <<
    std::setw (fieldWidth) << "fTtraceExtraHarmonies" << " : " <<
    fTraceExtraHarmonies <<
    std::endl <<

    // figured bass
    std::setw (fieldWidth) << "fTraceFiguredBass" << " : " <<
    fTraceFiguredBass <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceFiguredBassDetails" << " : " <<
    fTraceFiguredBassDetails <<
    std::endl <<

    // frames
    std::setw (fieldWidth) << "fTraceFrames" << " : " <<
    fTraceFrames <<
    std::endl <<

    // ties
    std::setw (fieldWidth) << "fTtraceTies" << " : " <<
    fTraceTies <<
    std::endl <<

    // slurs
    std::setw (fieldWidth) << "fTtraceSlurs" << " : " <<
    fTraceSlurs <<
    std::endl <<

    // ligatures
    std::setw (fieldWidth) << "fTtraceLigatures" << " : " <<
    fTraceLigatures <<
    std::endl <<

    // pedals
    std::setw (fieldWidth) << "fTtracePedals" << " : " <<
    fTracePedals <<
    std::endl <<

    // cresc/decresc
    std::setw (fieldWidth) << "fTraceCrescDecrescs" << " : " <<
    fTraceCrescDecrescs <<
    std::endl <<

    // wedges
    std::setw (fieldWidth) << "fTtraceWedges" << " : " <<
    fTraceWedges <<
    std::endl <<

    // staff details
    std::setw (fieldWidth) << "fTtraceStaffDetails" << " : " <<
    fTraceStaffDetails <<
    std::endl <<

    // scordaturas
    std::setw (fieldWidth) << "fTtraceScordaturas" << " : " <<
    fTraceScordaturas <<
    std::endl <<

    // segnos
    std::setw (fieldWidth) << "fTtraceSegnos" << " : " <<
    fTraceSegnos <<
    std::endl <<

    // dal segnos
    std::setw (fieldWidth) << "ftraceDalSegnos" << " : " <<
    fTraceDalSegnos <<
    std::endl <<

    // codas
    std::setw (fieldWidth) << "fTtraceCodas" << " : " <<
    fTraceCodas <<
    std::endl <<

    // accordion registrations
    std::setw (fieldWidth) << "fTtraceAccordionRegistrations" << " : " <<
    fTraceAccordionRegistrations <<
    std::endl <<

    // harp pedals
    std::setw (fieldWidth) << "fTtraceHarpPedals" << " : " <<
    fTraceHarpPedals <<
    std::endl <<

    // harp pedals tuning
    std::setw (fieldWidth) << "fTtraceHarpPedalsTunings" << " : " <<
    fTraceHarpPedalsTunings <<
    std::endl <<

    // extra harmonies
    std::setw (fieldWidth) << "fTtraceExtraHarmonies" << " : " <<
    fTraceExtraHarmonies <<
    std::endl <<

/* JMI
    // msrStreams
    std::setw (fieldWidth) << "fTtraceMsrStreams" << " : " <<
    fTraceMsrStreams <<
    std::endl <<
*/

    // midi
    std::setw (fieldWidth) << "fTtraceMidi" << " : " <<
    fTraceMidi <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_tracingOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_tracingOahGroup createGlobalTracingOahGroup (
  S_oahHandler handler)
{
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global trace OAH group" <<
      std::endl;
  }

  // protect library against multiple initializations
  if (! gGlobalTracingOahGroup) {
    // create the global OAH group
    gGlobalTracingOahGroup =
      tracingOahGroup::create (
        handler);
    assert (gGlobalTracingOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalTracingOahGroup;
}


}


#endif


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
and 0 applies to the '<part-std::list>' and anacrusis if present.
The measure numbers should be those of the produced score,
since MusicXML measure numbers are arbitrary strings.
Generate a detailed trace of the activity and print additional
debugging information to standard error for the specified measures.)",
          "measureNumbersSet",
          "traceDetailedMeasureNumbersSet",
          fTraceDetailedMeasureNumbersSet));
  }
*/

