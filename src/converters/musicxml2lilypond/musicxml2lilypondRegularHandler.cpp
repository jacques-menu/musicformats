/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfPreprocessorSettings.h"

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2lilypondRegularHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_xml2lyRegularHandler xml2lyRegularHandler::create (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  const S_xml2lyInsiderHandler&
                    insiderOahHandler)
{
  // create the regular handler
  xml2lyRegularHandler* obj = new
    xml2lyRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler);
  assert (obj!=0);

  return obj;
}

xml2lyRegularHandler::xml2lyRegularHandler (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  const S_xml2lyInsiderHandler&
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

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
  // print the options handler initial state
  gLog <<
    "musicxml2lilypondInsiderRegularHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
  }

xml2lyRegularHandler::~xml2lyRegularHandler ()
{}

void xml2lyRegularHandler::createRegularHandlerGroups ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the regular handler groups for \"" << fHandlerHeader << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  // create the trace OAH group
  appendGroupToHandler (
    createGlobalTraceOahGroup (
      this));
#endif // MF_TRACE_IS_ENABLED

  createInformationsRegularGroup ();

  createDisplayRegularGroup ();

  createFilesRegularGroup ();

  createOahRegularGroup ();

  createWarningAndErrorsRegularGroup ();

  createInputRegularGroup ();

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

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "All the regular handler groups for \"" <<
      fHandlerHeader <<
      "\" have been created";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void xml2lyRegularHandler::createOahRegularGroup ()
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

  registerAtomInRegularSubgroup (K_LANGUAGE_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup (K_INSIDER_OPTION_LONG_NAME, subGroup);
//  registerAtomInRegularSubgroup (K_REGULAR_OPTION_LONG_NAME, subGroup);

#ifdef MF_TRACE_IS_ENABLED
  registerAtomInRegularSubgroup (K_TRACE_EARLY_OPTIONS_OPTION_LONG_NAME, subGroup);
  registerAtomInRegularSubgroup (K_OAH_VERBOSE_MODE_OPTION_LONG_NAME, subGroup);
#endif // MF_TRACE_IS_ENABLED

  registerAtomInRegularSubgroup ("help", subGroup);
  registerAtomInRegularSubgroup ("help-summary", subGroup);

  registerAtomInRegularSubgroup ("help-options-usage", subGroup);

  registerAtomInRegularSubgroup ("name-help", subGroup);
  registerAtomInRegularSubgroup ("query", subGroup);
  registerAtomInRegularSubgroup ("find", subGroup);
  registerAtomInRegularSubgroup ("filter", subGroup);
  registerAtomInRegularSubgroup (K_INCLUDE_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup ("reverse-names-display-order", subGroup);

  registerAtomInRegularSubgroup ("display-options-and-arguments", subGroup);
  registerAtomInRegularSubgroup ("display-early-options-values", subGroup);
  registerAtomInRegularSubgroup ("display-options-values", subGroup);
  registerAtomInRegularSubgroup ("display-options-values-all", subGroup);

  registerAtomInRegularSubgroup ("show-options-and-arguments", subGroup);

  registerAtomInRegularSubgroup ("display-part-groups", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-skeleton", subGroup);

  registerAtomInRegularSubgroup ("display-msr1", subGroup);
  registerAtomInRegularSubgroup ("display-msr1-full", subGroup);
  registerAtomInRegularSubgroup ("display-msr2", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-full", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-measures-slices", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-measures-slices", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-names", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-names", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-summary", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-summary", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-voices-flat-view", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-voices-flat-view", subGroup);

  registerAtomInRegularSubgroup ("display-lpsr", subGroup);
  registerAtomInRegularSubgroup ("display-lpsr-full", subGroup);
  registerAtomInRegularSubgroup ("display-lpsr-names", subGroup);
  registerAtomInRegularSubgroup ("display-lpsr-summary", subGroup);
  registerAtomInRegularSubgroup ("display-lpsr-voices-flat-view", subGroup);

#ifdef MF_TRACE_IS_ENABLED
  registerAtomInRegularSubgroup ("trace-mxsr", subGroup);
  registerAtomInRegularSubgroup ("trace-mxsr-visitors", subGroup);

  registerAtomInRegularSubgroup ("trace-msr", subGroup);
  registerAtomInRegularSubgroup ("trace-msr-visitors", subGroup);
  registerAtomInRegularSubgroup ("trace-msr-durations", subGroup);

  registerAtomInRegularSubgroup ("trace-encoding", subGroup);
  registerAtomInRegularSubgroup ("trace-divisions", subGroup);

  registerAtomInRegularSubgroup ("trace-backup", subGroup);
  registerAtomInRegularSubgroup ("trace-forward", subGroup);

  registerAtomInRegularSubgroup ("trace-lpsr", subGroup);
  registerAtomInRegularSubgroup ("trace-lpsr-visitors", subGroup);
#endif // MF_TRACE_IS_ENABLED

  registerAtomInRegularSubgroup ("quit-after-pass-2", subGroup);
  registerAtomInRegularSubgroup ("quit-after-pass-3", subGroup);
  registerAtomInRegularSubgroup ("quit-after-pass-4", subGroup);

  registerAtomInRegularSubgroup ("debug", subGroup);

  registerAtomInRegularSubgroup ("trace-lpsr-blocks", subGroup);

  registerAtomInRegularSubgroup ("all-paper-variables", subGroup);

//   registerAtomInRegularSubgroup ("length-unit", subGroup);
//
//   registerAtomInRegularSubgroup ("paper-height", subGroup);
//   registerAtomInRegularSubgroup ("paper-width", subGroup);
//
//   registerAtomInRegularSubgroup ("left-margin", subGroup);
//   registerAtomInRegularSubgroup ("right-margin", subGroup);
//   registerAtomInRegularSubgroup ("top-margin", subGroup);
//   registerAtomInRegularSubgroup ("bottom-margin", subGroup);
//
//   registerAtomInRegularSubgroup ("horizontal-shift", subGroup);
//   registerAtomInRegularSubgroup ("indent", subGroup);
//   registerAtomInRegularSubgroup ("short-indent", subGroup);
//
//   registerAtomInRegularSubgroup ("markup-system-spacing-padding", subGroup);
//   registerAtomInRegularSubgroup ("between-system-space", subGroup);
//   registerAtomInRegularSubgroup ("page-top-spacing", subGroup);
}

void xml2lyRegularHandler::createInformationsRegularGroup ()
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

  registerAtomInRegularSubgroup ("about", subGroup);

  registerAtomInRegularSubgroup ("version", subGroup);
  registerAtomInRegularSubgroup ("version-full", subGroup);

  registerAtomInRegularSubgroup ("history", subGroup);

  registerAtomInRegularSubgroup ("mf-version", subGroup);
  registerAtomInRegularSubgroup ("mf-history", subGroup);

  registerAtomInRegularSubgroup ("contact", subGroup);

  registerAtomInRegularSubgroup ("music-formats-test", subGroup);
}

void xml2lyRegularHandler::createDisplayRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Display group",
        "help-display-group", "hdisplay-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "help-display", "hdisplay",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup ("language", subGroup);

  registerAtomInRegularSubgroup ("display-prefixes", subGroup);
  registerAtomInRegularSubgroup ("display-single-character-options", subGroup);

  registerAtomInRegularSubgroup ("display-cpu-usage", subGroup);
}

void xml2lyRegularHandler::createWarningAndErrorsRegularGroup ()
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

  registerAtomInRegularSubgroup (K_QUIET_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup ("dont-show-errors", subGroup);
  registerAtomInRegularSubgroup ("dont-quit-on-errors", subGroup);
}

void xml2lyRegularHandler::createInputRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Input group",
        "help-input-group", "hinput-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Input",
        "help-input", "hinput",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-musicxml-words", subGroup);

  registerAtomInRegularSubgroup ("convert-musicxml-words-to-bold", subGroup);
  registerAtomInRegularSubgroup ("convert-musicxml-words-to-italic", subGroup);

  registerAtomInRegularSubgroup ("place-musicxml-words-above", subGroup);
  registerAtomInRegularSubgroup ("place-musicxml-words-below", subGroup);

  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-rehearsal-mark", subGroup);

  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-segno", subGroup);
  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-dal-segno", subGroup);
  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-dal_segno-al-fine", subGroup);
  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-dal_segno-al-coda", subGroup);
  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-coda-first", subGroup);
  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-coda-second", subGroup);

  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-cresc", subGroup);
  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-decresc", subGroup);

  registerAtomInRegularSubgroup ("convert-musicxml-words-to-msr-tempo", subGroup);

  registerAtomInRegularSubgroup ("cubase", subGroup);
}

void xml2lyRegularHandler::createFilesRegularGroup ()
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

void xml2lyRegularHandler::createPartsRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-part-id", subGroup);
  registerAtomInRegularSubgroup ("ignore-musicxml-part-name", subGroup);
  registerAtomInRegularSubgroup ("keep-musicxml-part-id", subGroup);
  registerAtomInRegularSubgroup ("keep-musicxml-part-name", subGroup);
  registerAtomInRegularSubgroup ("msr-rename-part", subGroup);

  registerAtomInRegularSubgroup ("ignore-musicxml-staff-number", subGroup);
  registerAtomInRegularSubgroup ("keep-musicxml-staff-number", subGroup);

  registerAtomInRegularSubgroup ("ignore-musicxml-voice-number", subGroup);
  registerAtomInRegularSubgroup ("keep-musicxml-voice-number", subGroup);

  registerAtomInRegularSubgroup ("lilypond-transpose-part-id", subGroup);
  registerAtomInRegularSubgroup ("lilypond-transpose-part-name", subGroup);
}

void xml2lyRegularHandler::createStavesRegularGroup ()
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

  registerAtomInRegularSubgroup ("lpsr-staff-instrument-name", subGroup);

  registerAtomInRegularSubgroup ("create-voices-staff-relative-numbers", subGroup);

  registerAtomInRegularSubgroup ("ledger-lines-color", subGroup);
}

void xml2lyRegularHandler::createVoicesRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-msr-voice", subGroup);
  registerAtomInRegularSubgroup ("keep-msr-voice", subGroup);

  registerAtomInRegularSubgroup ("create-single-line-staves-as-rythmic", subGroup);
  registerAtomInRegularSubgroup ("connect-arpeggios", subGroup);

//  registerAtomInRegularSubgroup ("auto-voices", subGroup);
}

void xml2lyRegularHandler::createTranspositionsRegularGroup ()
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

void xml2lyRegularHandler::createClefsRegularGroup ()
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

void xml2lyRegularHandler::createKeysRegularGroup ()
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

void xml2lyRegularHandler::createTimesRegularGroup ()
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

void xml2lyRegularHandler::createRepeatsRegularGroup ()
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

// JMI  registerAtomInRegularSubgroup ("create-implicit-initial-repeat-barline", subGroup);
  registerAtomInRegularSubgroup ("keep-repeat-barlines", subGroup);
  registerAtomInRegularSubgroup ("repeat-brackets", subGroup);
  registerAtomInRegularSubgroup ("ignore-repeat-numbers", subGroup);
}

void xml2lyRegularHandler::createMeasuresRegularGroup ()
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
  registerAtomInRegularSubgroup ("ignore-musicxml-line-breaks", subGroup);

  registerAtomInRegularSubgroup ("add-empty-msr-measures", subGroup);
  registerAtomInRegularSubgroup ("replicate-msr-measure", subGroup);
  registerAtomInRegularSubgroup ("replicate-empty-lpsr-measure", subGroup);
}

void xml2lyRegularHandler::createRestsRegularGroup ()
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

// JMI  registerAtomInRegularSubgroup ("merge-rests", subGroup);
}

void xml2lyRegularHandler::createNotesRegularGroup ()
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

  registerAtomInRegularSubgroup ("generate-commented-out-lilypond-variables", subGroup);
  registerAtomInRegularSubgroup ("generate-lpsr-visiting-information", subGroup);

  registerAtomInRegularSubgroup ("lilypond-accidental-style", subGroup);

  registerAtomInRegularSubgroup ("non-print-notes-head-color", subGroup);

  registerAtomInRegularSubgroup ("merge-rests-in-msr", subGroup);
  registerAtomInRegularSubgroup ("merge-staff-common-rests-in-lilypond", subGroup);
  registerAtomInRegularSubgroup ("multi-measure-rests-expand-limit", subGroup);
}

void xml2lyRegularHandler::creatBeamsRegularGroup ()
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

void xml2lyRegularHandler::createArticulationsRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-articulations", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-pedals", subGroup);
}

void xml2lyRegularHandler::createOrnamentsRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-ornaments", subGroup);

  registerAtomInRegularSubgroup ("delayed-ornaments-fraction", subGroup);
}

void xml2lyRegularHandler::createGraceNotesRegularGroup ()
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

void xml2lyRegularHandler::createChordsRegularGroup ()
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

  registerAtomInRegularSubgroup ("lpsr-pitches-language", subGroup);

  registerAtomInRegularSubgroup ("lpsr-chords-language", subGroup);

  registerAtomInRegularSubgroup ("chords-display", subGroup);
}

void xml2lyRegularHandler::createTiesRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-ties", subGroup);
}

void xml2lyRegularHandler::createSlursRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-slurs", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-slurs", subGroup);
}

void xml2lyRegularHandler::createLigaturesRegularGroup ()
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

void xml2lyRegularHandler::createDynamicRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-dynamics", subGroup);

  registerAtomInRegularSubgroup ("all-dynamics-below", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-dynamics", subGroup);

  registerAtomInRegularSubgroup ("lilypond-dynamics-text-spanners-style", subGroup);
}

void xml2lyRegularHandler::createWedgesRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-wedges", subGroup);

  registerAtomInRegularSubgroup ("all-wedges-below", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-wedges", subGroup);
}

void xml2lyRegularHandler::createTupletsRegularGroup ()
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

void xml2lyRegularHandler::createStringsRegularGroup ()
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

void xml2lyRegularHandler::createTablaturesRegularGroup ()
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

void xml2lyRegularHandler::createHarmoniesRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-harmonies", subGroup);

  registerAtomInRegularSubgroup ("show-harmony-voices", subGroup);

  registerAtomInRegularSubgroup ("display-harmonies-structures", subGroup);
  registerAtomInRegularSubgroup ("display-all-harmonies-contents", subGroup);
  registerAtomInRegularSubgroup ("display-harmony-details", subGroup);
  registerAtomInRegularSubgroup ("display-harmony-analysis", subGroup);
}

void xml2lyRegularHandler::createFiguredBassRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-figured-bass", subGroup);

  registerAtomInRegularSubgroup ("show-figured-bass-voices", subGroup);
}

void xml2lyRegularHandler::createLyricsRegularGroup ()
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

  registerAtomInRegularSubgroup ("ignore-musicxml-lyrics", subGroup);
  registerAtomInRegularSubgroup ("add-msr-words-from-the-msr-lyrics", subGroup);

  registerAtomInRegularSubgroup ("no-lilypond-lyrics", subGroup);
  registerAtomInRegularSubgroup ("lilypond-lyrics-durations-style", subGroup);
  registerAtomInRegularSubgroup ("add-lilypond-stanzas-numbers", subGroup);
}

void xml2lyRegularHandler::createBookRegularGroup ()
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

void xml2lyRegularHandler::createHeaderRegularGroup ()
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

//   registerAtomInRegularSubgroup ("instrument", subGroup); // JMI

  registerAtomInRegularSubgroup ("meter", subGroup);

  registerAtomInRegularSubgroup ("copyright", subGroup);

  registerAtomInRegularSubgroup ("tagline", subGroup);

  registerAtomInRegularSubgroup ("use-lyricists-as-poets", subGroup);

  registerAtomInRegularSubgroup ("use-filename-as-work-title", subGroup);
}

void xml2lyRegularHandler::createPaperRegularGroup ()
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

  registerAtomInRegularSubgroup ("ragged-last", subGroup);
  registerAtomInRegularSubgroup ("ragged-bottom", subGroup);
  registerAtomInRegularSubgroup ("ragged-last-bottom", subGroup);
  registerAtomInRegularSubgroup ("ragged-right", subGroup);

  registerAtomInRegularSubgroup ("horizontal-shift", subGroup);
  registerAtomInRegularSubgroup ("indent", subGroup);
  registerAtomInRegularSubgroup ("short-indent", subGroup);

  registerAtomInRegularSubgroup ("markup-system-spacing-padding", subGroup);

  registerAtomInRegularSubgroup ("page-count", subGroup);
  registerAtomInRegularSubgroup ("system-count", subGroup);
}

void xml2lyRegularHandler::createLayoutRegularGroup ()
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

void xml2lyRegularHandler::createOutputRegularGroup ()
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

  registerAtomInRegularSubgroup ("lilypond-generation-infos", subGroup);

  registerAtomInRegularSubgroup ("display-music", subGroup);

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

  registerAtomInRegularSubgroup ("generate-measure-moments", subGroup);
  registerAtomInRegularSubgroup ("generate-positions-in-measures", subGroup);

  registerAtomInRegularSubgroup ("all-bar-numbers", subGroup);
  registerAtomInRegularSubgroup ("all-lilypond-measure-numbers", subGroup);
  registerAtomInRegularSubgroup ("show-lilypond-measure-number-at", subGroup);
  registerAtomInRegularSubgroup ("bar-numbers-size", subGroup);
  registerAtomInRegularSubgroup ("generate-a-box-around-all-bar-numbers", subGroup);
  registerAtomInRegularSubgroup ("generate-a-box-around-bar-number", subGroup);
  registerAtomInRegularSubgroup ("reset-measure-number", subGroup);
  registerAtomInRegularSubgroup ("show-musicxml-measure-numbers", subGroup);

  registerAtomInRegularSubgroup ("separator-line-every-n-measures", subGroup);

  registerAtomInRegularSubgroup ("ignore-lpsr-line-breaks", subGroup);
  registerAtomInRegularSubgroup ("ignore-lpsr-page-breaks", subGroup);

  registerAtomInRegularSubgroup ("break-lines-at-incomplete-right-measures", subGroup);
  registerAtomInRegularSubgroup ("custos", subGroup);

  registerAtomInRegularSubgroup ("length-unit", subGroup);

  registerAtomInRegularSubgroup ("generate-all-lilypond-durations", subGroup);

  registerAtomInRegularSubgroup ("jazz-fonts", subGroup);

  registerAtomInRegularSubgroup ("jianpu", subGroup);
  registerAtomInRegularSubgroup ("lyluatex", subGroup);

  registerAtomInRegularSubgroup ("minimal", subGroup);
}

void xml2lyRegularHandler::createMidiRegularGroup ()
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
void xml2lyRegularHandler::checkOptionsAndArguments () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
void xml2lyRegularHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "musicxml2lilypondInsiderRegularHandler '" << fHandlerHeader << "':" <<
    std::endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);
  os << std::endl;

  os <<
    "Options groups (" <<
    mfSingularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    std::endl;

  if (fHandlerGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_xml2lyRegularHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
