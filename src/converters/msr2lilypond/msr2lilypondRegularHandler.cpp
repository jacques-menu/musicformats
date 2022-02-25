/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "enableHarmoniesExtraOahIfDesired.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "lpsr.h"


#include "oahOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "oahEarlyOptions.h"

#include "msr2lilypondRegularHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msr2lilypondRegularHandler msr2lilypondRegularHandler::create (
  const string&     serviceName,
  const string&     handlerHeader,
  S_msr2lilypondInsiderHandler
                    insiderOahHandler)
{
  // create the regular handler
  msr2lilypondRegularHandler* o = new
    msr2lilypondRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler);
  assert (o!=0);

  return o;
}

msr2lilypondRegularHandler::msr2lilypondRegularHandler (
  const string&     serviceName,
  const string&     handlerHeader,
  S_msr2lilypondInsiderHandler
                    insiderOahHandler)
  : oahRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler)
{
  // this is done only only after the constructor has been executed,
  // because it uses pure virtual methods
  initializeOahRegularHandler ();

  // create the regular handler groups
  createRegularHandlerGroups ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
  // print the options handler initial state
  gLogStream <<
    "msr2lilypondRegularHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    endl;
  }
#endif
  }

msr2lilypondRegularHandler::~msr2lilypondRegularHandler ()
{}

void msr2lilypondRegularHandler::createRegularHandlerGroups ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the regular handler groups for \"" << fHandlerHeader << "\"" <<
      endl;
  }
#endif

  createInformationsRegularGroup ();

  createFilesRegularGroup ();

  createOahRegularGroup ();

  createWarningAndErrorsRegularGroup ();

  createPartsRegularGroup ();
  createStavesRegularGroup ();
  createVoicesRegularGroup ();

  createTranspositionsRegularGroup ();

  createClefsRegularGroup ();
  createKeysRegularGroup ();
  createTimesRegularGroup ();

  createRepeatsRegularGroup ();

  createMeasuresRegularGroup ();

  createRestsRegularGroup ();
  createNotesRegularGroup ();

  creatBeamsRegularGroup ();

  createArticulationsRegularGroup ();

  createOrnamentsRegularGroup ();

  createGraceNotesRegularGroup ();

  createChordsRegularGroup ();

  createTiesRegularGroup ();
  createSlursRegularGroup ();
  createLigaturesRegularGroup ();

  createDynamicRegularGroup ();
  createWedgesRegularGroup ();

  createTupletsRegularGroup ();

  createLyricsRegularGroup ();

  createStringsRegularGroup ();

  createTablaturesRegularGroup ();

  createHarmoniesRegularGroup ();

  createFiguredBassRegularGroup ();

  createBookRegularGroup ();
  createHeaderRegularGroup ();
  createPaperRegularGroup ();
  createLayoutRegularGroup ();

  createOutputRegularGroup ();

  createMidiRegularGroup ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "All the regular handler groups for \"" <<
      fHandlerHeader <<
      "\" have been created" <<
      endl;
  }
#endif
}

void msr2lilypondRegularHandler::createOahRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Options and help group",
        "help-oah-group", "hoah-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options and help",
        "help-oah", "hoah",
        "",
        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup (K_INSIDER_OPTION_LONG_NAME, subGroup);
//  registerAtomInRegularSubgroup (K_REGULAR_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup (K_TRACE_EARLY_OPTIONS_LONG_OPTION_NAME, subGroup);
  registerAtomInRegularSubgroup (K_OAH_VERBOSE_MODE_LONG_OPTION_NAME, subGroup);

  registerAtomInRegularSubgroup ("help", subGroup);
  registerAtomInRegularSubgroup ("help-summary", subGroup);

  registerAtomInRegularSubgroup ("help-options-usage", subGroup);

  registerAtomInRegularSubgroup ("name-help", subGroup);
  registerAtomInRegularSubgroup ("query", subGroup);
  registerAtomInRegularSubgroup ("find", subGroup);
  registerAtomInRegularSubgroup (K_INCLUDE_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup ("reverse-names-display-order", subGroup);

  registerAtomInRegularSubgroup ("display-options-and-arguments", subGroup);
  registerAtomInRegularSubgroup ("display-early-options-values", subGroup);
  registerAtomInRegularSubgroup ("display-options-values", subGroup);
  registerAtomInRegularSubgroup ("display-options-values-all", subGroup);

  registerAtomInRegularSubgroup ("show-options-and-arguments", subGroup);

  registerAtomInRegularSubgroup ("display-part-groups", subGroup);

  registerAtomInRegularSubgroup ("display-msr-skeleton", subGroup);
  registerAtomInRegularSubgroup ("display-msr-1", subGroup);
  registerAtomInRegularSubgroup ("display-msr-2", subGroup);

  registerAtomInRegularSubgroup ("display-msr-1-short", subGroup);
  registerAtomInRegularSubgroup ("display-msr-2-short", subGroup);

  registerAtomInRegularSubgroup ("display-msr-1-slices", subGroup);
  registerAtomInRegularSubgroup ("display-msr-2-slices", subGroup);

  registerAtomInRegularSubgroup ("display-msr-1-details", subGroup);
  registerAtomInRegularSubgroup ("display-msr-2-details", subGroup);

  registerAtomInRegularSubgroup ("display-msr-1-names", subGroup);
  registerAtomInRegularSubgroup ("display-msr-2-names", subGroup);

  registerAtomInRegularSubgroup ("display-msr-1-summary", subGroup);
  registerAtomInRegularSubgroup ("display-msr-2-summary", subGroup);

  registerAtomInRegularSubgroup ("display-lpsr", subGroup);
  registerAtomInRegularSubgroup ("display-lpsr-full", subGroup);
  registerAtomInRegularSubgroup ("display-lpsr-short", subGroup);

#ifdef TRACING_IS_ENABLED
  registerAtomInRegularSubgroup (K_TRACE_OAH_LONG_OPTION_NAME, subGroup);
  registerAtomInRegularSubgroup (K_TRACE_OAH_DETAILS_LONG_OPTION_NAME, subGroup);

  registerAtomInRegularSubgroup (K_TRACE_PASSES_LONG_OPTION_NAME, subGroup);

  registerAtomInRegularSubgroup ("trace-msr", subGroup);
  registerAtomInRegularSubgroup ("trace-msr-visitors", subGroup);
  registerAtomInRegularSubgroup ("trace-msr-durations", subGroup);

  registerAtomInRegularSubgroup ("trace-encoding", subGroup);
  registerAtomInRegularSubgroup ("trace-divisions", subGroup);

  registerAtomInRegularSubgroup ("trace-backup", subGroup);
  registerAtomInRegularSubgroup ("trace-forward", subGroup);
#endif
}

void msr2lilypondRegularHandler::createInformationsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Informations group",
        "help-informations-group", "hinfos-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Informations",
        "help-informations", "hinfos",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup ("version", subGroup);
  registerAtomInRegularSubgroup ("version-full", subGroup);
  registerAtomInRegularSubgroup ("history", subGroup);
  registerAtomInRegularSubgroup ("mf-version", subGroup);
  registerAtomInRegularSubgroup ("mf-history", subGroup);
  registerAtomInRegularSubgroup ("about", subGroup);
  registerAtomInRegularSubgroup ("contact", subGroup);
  registerAtomInRegularSubgroup ("display-prefixes", subGroup);
  registerAtomInRegularSubgroup ("display-single-character-options", subGroup);

  registerAtomInRegularSubgroup ("display-cpu-usage", subGroup);
}

void msr2lilypondRegularHandler::createWarningAndErrorsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Warning and errors group",
        "help-warning-and-errors-group", "hwae-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Warning and errors",
        "help-warning-and-errors", "hwae",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("quiet", subGroup);

  registerAtomInRegularSubgroup ("dont-show-errors", subGroup);
  registerAtomInRegularSubgroup ("dont-quit-on-errors", subGroup);
}

void msr2lilypondRegularHandler::createFilesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Files group",
        "help-files-group", "hfiles-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Files",
        "help-files", "hfiles",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("output-file-name", subGroup);
  registerAtomInRegularSubgroup ("auto-output-file-name", subGroup);
}

void msr2lilypondRegularHandler::createPartsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Parts group",
        "help-parts-group", "hparts-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Parts",
        "help-parts", "hparts",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("msr-ignore-musicxml-part-id", subGroup);
  registerAtomInRegularSubgroup ("msr-ignore-musicxml-part-name", subGroup);
  registerAtomInRegularSubgroup ("msr-keep-musicxml-part-id", subGroup);
  registerAtomInRegularSubgroup ("msr-keep-musicxml-part-name", subGroup);
  registerAtomInRegularSubgroup ("msr-rename-part", subGroup);
}

void msr2lilypondRegularHandler::createStavesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Staves group",
        "help-staves-group", "hstaves-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Staves",
        "help-staves", "hstaves",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("create-voices-staff-relative-numbers", subGroup);

  registerAtomInRegularSubgroup ("ledger-lines-color", subGroup);
}

void msr2lilypondRegularHandler::createVoicesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Voices group",
        "help-voices-group", "hvoices-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices",
        "help-voices", "hvoices",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("create-single-line-staves-as-rythmic", subGroup);
  registerAtomInRegularSubgroup ("connect-arpeggios", subGroup);

//  registerAtomInRegularSubgroup ("auto-voices", subGroup);
}

void msr2lilypondRegularHandler::createTranspositionsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Transpositions group",
        "help-transpositions-group", "htransp-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Transpositions",
        "help-transpositions", "htransp",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms
// JMI
}

void msr2lilypondRegularHandler::createClefsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Clefs group",
        "help-clefs-group", "hclefs-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs",
        "help-clefs", "hclefs",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("msr-replace-clef", subGroup);
  registerAtomInRegularSubgroup ("ignore-redundant-clefs", subGroup);
  registerAtomInRegularSubgroup ("comment-clef-changes", subGroup);
}

void msr2lilypondRegularHandler::createKeysRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Keys group",
        "help-keys-group", "hkeys-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Keys",
        "help-keys", "hkeys",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-redundant-keys", subGroup);
}

void msr2lilypondRegularHandler::createTimesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Times group",
        "help-times-group", "htimes-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Times",
        "help-times", "htimes",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-redundant-times", subGroup);
  registerAtomInRegularSubgroup ("numerical-time-signatures", subGroup);
}

void msr2lilypondRegularHandler::createRepeatsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Repeats group",
        "help-repeats-group", "hreps-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Repeats",
        "help-repeats", "hreps",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("create-implicit-initial-repeat-barline", subGroup);
  registerAtomInRegularSubgroup ("keep-repeat-barLines", subGroup);
  registerAtomInRegularSubgroup ("repeat-brackets", subGroup);
  registerAtomInRegularSubgroup ("ignore-repeat-numbers", subGroup);
}

void msr2lilypondRegularHandler::createMeasuresRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Measures group",
        "help-measures-group", "hmeas-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "help-measures", "hmeas",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-musicxml-page-breaks", subGroup);

  registerAtomInRegularSubgroup ("add-empty-msr-measures", subGroup);
  registerAtomInRegularSubgroup ("replicate-msr-measure", subGroup);
  registerAtomInRegularSubgroup ("replicate-empty-lpsr-measure", subGroup);
}

void msr2lilypondRegularHandler::createRestsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Rests group",
        "help-rests-group", "hrests-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Rests",
        "help-rests", "hrests",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms


  registerAtomInRegularSubgroup ("avoid-msr2msr", subGroup); // for TESTS

  registerAtomInRegularSubgroup ("delay-rests-slashes", subGroup);

  registerAtomInRegularSubgroup ("insert-page-break-after-measure", subGroup);
  registerAtomInRegularSubgroup ("insert-line-break-after-measure", subGroup);

  registerAtomInRegularSubgroup ("coalesce-empty-measures", subGroup);
  registerAtomInRegularSubgroup ("compress-empty-measures-in-lilypond", subGroup);

  registerAtomInRegularSubgroup ("merge-rests", subGroup);
}

void msr2lilypondRegularHandler::createNotesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Notes group",
        "help-notes-group", "hnotes-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "help-notes", "hnotes",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("msr-pitches-language", subGroup);

  registerAtomInRegularSubgroup ("absolute", subGroup);
  registerAtomInRegularSubgroup ("relative", subGroup);
  registerAtomInRegularSubgroup ("fixed", subGroup);

  registerAtomInRegularSubgroup ("white-note-heads", subGroup);

  registerAtomInRegularSubgroup ("generate-stems-directions", subGroup);

  registerAtomInRegularSubgroup ("generate-commented-out-variables", subGroup);
  registerAtomInRegularSubgroup ("generate-lpsr-visiting-information", subGroup);

  registerAtomInRegularSubgroup ("lilypond-accidental-style", subGroup);

  registerAtomInRegularSubgroup ("non-print-notes-head-color", subGroup);
}

void msr2lilypondRegularHandler::creatBeamsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Beams group",
        "help-beams-group", "hbeams-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Beams",
        "help-beams", "hbeams",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("no-auto-beaming", subGroup);
  registerAtomInRegularSubgroup ("no-beams", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-beams", subGroup); // JMI
}

void msr2lilypondRegularHandler::createArticulationsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Articulations group",
        "help-articulations-group", "harts-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Articulations",
        "help-articulations", "harts",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-articulations", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-pedals", subGroup);
}

void msr2lilypondRegularHandler::createOrnamentsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ornaments group",
        "help-ornaments-group", "horns-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ornaments",
         "help-ornaments", "horns",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-ornaments", subGroup);

  registerAtomInRegularSubgroup ("delayed-ornaments-fraction", subGroup);
}

void msr2lilypondRegularHandler::createGraceNotesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Grace notes group",
        "help-grace-notes-group", "hgraces-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Grace notes",
        "help-grace-notes", "hgraces",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("slash-all-grace-notes", subGroup);
  registerAtomInRegularSubgroup ("slur-all-grace-notes", subGroup);
  registerAtomInRegularSubgroup ("beam-all-grace-notes", subGroup);
}

void msr2lilypondRegularHandler::createChordsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Chords group",
        "help-chords-group", "hchords-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords",
        "help-chords", "hchords",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

#ifdef TRACING_IS_ENABLED
  registerAtomInRegularSubgroup ("trace-chords", subGroup);
#endif

  registerAtomInRegularSubgroup ("lpsr-pitches-language", subGroup);

  registerAtomInRegularSubgroup ("lpsr-chords-language", subGroup);

  registerAtomInRegularSubgroup ("chords-display", subGroup);
}

void msr2lilypondRegularHandler::createTiesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ties group",
        "help-ties-group", "hties-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ties",
        "help-ties", "hties",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-ties", subGroup);
}

void msr2lilypondRegularHandler::createSlursRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Slurs group",
        "help-slurs-group", "hslurs-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Slurs",
        "help-slurs","hslurs",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-slurs", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-slurs", subGroup);
}

void msr2lilypondRegularHandler::createLigaturesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ligatures group",
        "help-ligatures-group", "hligs-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ligatures",
        "help-ligatures", "hligs",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("delay-rests-ligatures", subGroup);
}

void msr2lilypondRegularHandler::createDynamicRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Dynamics group",
        "help-dynamics-group", "hdyns-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics",
        "help-dynamics", "hdyns",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-dynamics", subGroup);

  registerAtomInRegularSubgroup ("all-dynamics-below", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-dynamics", subGroup);

  registerAtomInRegularSubgroup ("lilypond-dynamics-text-spanners-style", subGroup);
}

void msr2lilypondRegularHandler::createWedgesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Wedges group",
        "help-wedges-group", "hweds-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Wedges",
        "help-wedges", "hweds",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-wedges", subGroup);

  registerAtomInRegularSubgroup ("all-wedges-below", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-wedges", subGroup);
}

void msr2lilypondRegularHandler::createTupletsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tuplets group",
        "help-tuplets-group", "htups-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tuplets",
        "help-tuplets", "htups",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("indent-tuplets", subGroup);
}

void msr2lilypondRegularHandler::createStringsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Strings group",
        "help-strings-group", "hstrings-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Strings",
        "hstrings", "", "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("roman-string-numbers", subGroup);
  registerAtomInRegularSubgroup ("avoid-open-strings", subGroup);
}

void msr2lilypondRegularHandler::createTablaturesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tablatures group",
        "help-tablatures-group", "htabs-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tablatures",
        "help-tablatures", "htabs",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("tab-full-notation", subGroup);
  registerAtomInRegularSubgroup ("modern-tab", subGroup);
}

void msr2lilypondRegularHandler::createHarmoniesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Harmonies group",
        "help-harmonies-group", "hharms-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies",
        "help-harmonies", "hharms",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-harmonies", subGroup);

  registerAtomInRegularSubgroup ("show-harmony-voices", subGroup);

#ifdef EXTRA_OAH_IS_ENABLED
  registerAtomInRegularSubgroup ("show-harmonies-structures", subGroup);
  registerAtomInRegularSubgroup ("show-all-harmonies-contents", subGroup);
  registerAtomInRegularSubgroup ("show-harmony-details", subGroup);
  registerAtomInRegularSubgroup ("show-harmony-analysis", subGroup);
#endif
}

void msr2lilypondRegularHandler::createFiguredBassRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Figured bass elements group",
        "help-figured-bass-group", "hfigbass-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Figured bass elements",
        "help-figured-bass", "hfigbass",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-figured-bass", subGroup);

  registerAtomInRegularSubgroup ("show-figured-bass-voices", subGroup);
}

void msr2lilypondRegularHandler::createLyricsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Lyrics group",
        "help-lyrics-group", "hlyrics-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Lyrics",
        "help-lyrics", "hlyrics",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("lilypond-dynamics-text-spanners-style", subGroup);

  registerAtomInRegularSubgroup ("ignore-musicxml-lyrics", subGroup);
  registerAtomInRegularSubgroup ("add-msr-words-from-the-msr-lyrics", subGroup);

  registerAtomInRegularSubgroup ("no-lilypond-lyrics", subGroup);
  registerAtomInRegularSubgroup ("lilypond-lyrics-durations-style", subGroup);
  registerAtomInRegularSubgroup ("add-lilypond-stanzas-numbers", subGroup);
}

void msr2lilypondRegularHandler::createBookRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Book group",
        "help-book-group", "hbook-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Book",
        "help-book", "hbook",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("no-book-block", subGroup);
  registerAtomInRegularSubgroup ("no-header-block", subGroup);
  registerAtomInRegularSubgroup ("no-paper-block", subGroup);
  registerAtomInRegularSubgroup ("no-bar-number-checks", subGroup);
}

void msr2lilypondRegularHandler::createHeaderRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Header group",
        "help-header-group", "hheader-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Header",
        "help-header", "hheader",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("rights", subGroup);

  registerAtomInRegularSubgroup ("composer", subGroup);
  registerAtomInRegularSubgroup ("arranger", subGroup);
  registerAtomInRegularSubgroup ("lyricist", subGroup);
  registerAtomInRegularSubgroup ("poet", subGroup);

  registerAtomInRegularSubgroup ("software", subGroup);

  registerAtomInRegularSubgroup ("dedication", subGroup);
  registerAtomInRegularSubgroup ("piece", subGroup);
  registerAtomInRegularSubgroup ("opus", subGroup);

  registerAtomInRegularSubgroup ("title", subGroup);
  registerAtomInRegularSubgroup ("subtitle", subGroup);
  registerAtomInRegularSubgroup ("subsubtitle", subGroup);

  registerAtomInRegularSubgroup ("instrument", subGroup);

  registerAtomInRegularSubgroup ("meter", subGroup);

  registerAtomInRegularSubgroup ("copyright", subGroup);

  registerAtomInRegularSubgroup ("tagline", subGroup);

  registerAtomInRegularSubgroup ("use-lyricists-as-poets", subGroup);

  registerAtomInRegularSubgroup ("use-filename-as-work-title", subGroup);
}

void msr2lilypondRegularHandler::createPaperRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Paper group",
        "help-paper-group", "hpaper-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Paper",
        "help-paper", "hpaper",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("paper-height", subGroup);
  registerAtomInRegularSubgroup ("paper-width", subGroup);

  registerAtomInRegularSubgroup ("left-margin", subGroup);
  registerAtomInRegularSubgroup ("right-margin", subGroup);
  registerAtomInRegularSubgroup ("top-margin", subGroup);
  registerAtomInRegularSubgroup ("bottom-margin", subGroup);

  registerAtomInRegularSubgroup ("ragged-bottom", subGroup);
  registerAtomInRegularSubgroup ("ragged-last", subGroup);
  registerAtomInRegularSubgroup ("ragged-last-bottom", subGroup);
  registerAtomInRegularSubgroup ("ragged-right", subGroup);

  registerAtomInRegularSubgroup ("horizontal-shift", subGroup);
  registerAtomInRegularSubgroup ("indent", subGroup);
  registerAtomInRegularSubgroup ("short-indent", subGroup);

  registerAtomInRegularSubgroup ("markup-system-spacing-padding", subGroup);

  registerAtomInRegularSubgroup ("page-count", subGroup);
  registerAtomInRegularSubgroup ("system-count", subGroup);
}

void msr2lilypondRegularHandler::createLayoutRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Layout group",
        "help-layout-group", "hlayout-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Layout",
        "help-layout", "hlayout",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("global-staff-size", subGroup);
  registerAtomInRegularSubgroup ("keep-staff-size", subGroup);
}

void msr2lilypondRegularHandler::createOutputRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Output group",
        "help-output-group", "houtput-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Output",
        "help-output", "houtput",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("msr2lilypond-infos", subGroup);

  registerAtomInRegularSubgroup ("no-lilypond-code", subGroup);

  registerAtomInRegularSubgroup ("draft-mode", subGroup);
  registerAtomInRegularSubgroup ("point-and-clic-off", subGroup);

  registerAtomInRegularSubgroup ("lilypond-version", subGroup);
  registerAtomInRegularSubgroup ("lilypond-conversion-date", subGroup);
  registerAtomInRegularSubgroup ("lilypond-comments", subGroup);

  registerAtomInRegularSubgroup ("no-set-global-staff-size", subGroup);

  registerAtomInRegularSubgroup ("msr2lpsr-score-output-kind", subGroup);

  registerAtomInRegularSubgroup ("global", subGroup);

  registerAtomInRegularSubgroup ("ambitus", subGroup);

  registerAtomInRegularSubgroup ("input-line-numbers", subGroup);

  registerAtomInRegularSubgroup ("notes-comments", subGroup);

  registerAtomInRegularSubgroup ("generate-moments-in-measures", subGroup);
  registerAtomInRegularSubgroup ("generate-positions-in-measures", subGroup);

  registerAtomInRegularSubgroup ("all-bar-numbers", subGroup);
  registerAtomInRegularSubgroup ("all-measure-numbers", subGroup);
  registerAtomInRegularSubgroup ("show-measure-number-at", subGroup);
  registerAtomInRegularSubgroup ("bar-numbers-size", subGroup);
  registerAtomInRegularSubgroup ("generate-a-box-around-all-bar-numbers", subGroup);
  registerAtomInRegularSubgroup ("generate-a-box-around-bar-number", subGroup);
  registerAtomInRegularSubgroup ("reset-measure-number", subGroup);
  registerAtomInRegularSubgroup ("original-measure-numbers", subGroup);

  registerAtomInRegularSubgroup ("separator-line-every-n-measures", subGroup);

  registerAtomInRegularSubgroup ("ignore-lpsr-line-breaks", subGroup);
  registerAtomInRegularSubgroup ("ignore-lpsr-page-breaks", subGroup);

  registerAtomInRegularSubgroup ("break-lines-at-incomplete-right-measures", subGroup);
  registerAtomInRegularSubgroup ("custos", subGroup);

  registerAtomInRegularSubgroup ("length-unit", subGroup);

  registerAtomInRegularSubgroup ("all-durations", subGroup);

  registerAtomInRegularSubgroup ("jazz-fonts", subGroup);

  registerAtomInRegularSubgroup ("jianpu", subGroup);
  registerAtomInRegularSubgroup ("lyluatex", subGroup);

  registerAtomInRegularSubgroup ("minimal", subGroup);
}

void msr2lilypondRegularHandler::createMidiRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "MIDI group",
        "help-midi-group", "hmidi-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MIDI",
        "help-midi", "hmidi",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("no-midi", subGroup);
  registerAtomInRegularSubgroup ("midi-tempo", subGroup);
}

//______________________________________________________________________________
void msr2lilypondRegularHandler::checkOptionsAndArguments () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
void msr2lilypondRegularHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msr2lilypondRegularHandler '" << fHandlerHeader << "':" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);
  os << endl;

  os <<
    "Options groups (" <<
    mfSingularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

ostream& operator<< (ostream& os, const S_msr2lilypondRegularHandler& elt)
{
  elt->print (os);
  return os;
}


}
