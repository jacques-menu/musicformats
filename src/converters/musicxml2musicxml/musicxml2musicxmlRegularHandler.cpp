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

#include "oahEarlyOptions.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "musicxml2musicxmlRegularHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_xml2xmlRegularHandler xml2xmlRegularHandler::create (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  const S_xml2xmlInsiderHandler&
                    insiderOahHandler)
{
  // create the regular handler
  xml2xmlRegularHandler* obj = new
    xml2xmlRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler);
  assert (obj!=0);

  return obj;
}

xml2xmlRegularHandler::xml2xmlRegularHandler (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  const S_xml2xmlInsiderHandler&
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
    "xml2xmlRegularHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    std::endl;

  ++gIndenter;

  gLog <<
    "===> printHelp():" <<
    std::endl;
  this->printHelp (gOutput); // JMI

  --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

xml2xmlRegularHandler::~xml2xmlRegularHandler ()
{}

void xml2xmlRegularHandler::createRegularHandlerGroups ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the regular handler groups for \"" <<
      fHandlerHeader <<
      "\"";

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

// JMI  createMsrGenerationAPIRegularGroup ();

  createInputRegularGroup ();

  createPresentationRegularGroup ();

  createPartsRegularGroup ();
  createStavesRegularGroup ();
  createVoicesRegularGroup ();

  createClefsRegularGroup ();
  createKeysRegularGroup ();
  createTimesRegularGroup ();

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

  createHarmoniesRegularGroup ();

  createFiguredBassRegularGroup ();

  createOutputRegularGroup ();

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

void xml2xmlRegularHandler::createInformationsRegularGroup ()
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

void xml2xmlRegularHandler::createDisplayRegularGroup ()
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

void xml2xmlRegularHandler::createFilesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Files group",
        "files-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Files",
        "files", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("output-file-name", subGroup);
  registerAtomInRegularSubgroup ("auto-output-file-name", subGroup);
}

void xml2xmlRegularHandler::createOahRegularGroup ()
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
  registerAtomInRegularSubgroup ("display-options-values-full", subGroup);
  registerAtomInRegularSubgroup ("show-options-and-arguments", subGroup);


  registerAtomInRegularSubgroup ("display-part-groups", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-skeleton", subGroup);

  registerAtomInRegularSubgroup ("display-msr1", subGroup);
  registerAtomInRegularSubgroup ("display-msr1-full", subGroup);
  registerAtomInRegularSubgroup ("display-msr2", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-full", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-measures-slices", subGroup);
  registerAtomInRegularSubgroup ("display-msr1-measures-slices-details", subGroup);

  registerAtomInRegularSubgroup ("display-msr2-measures-slices", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-measures-slices-details", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-names", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-names", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-summary", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-summary", subGroup);

  registerAtomInRegularSubgroup ("display-msr1-voices-flat-view", subGroup);
  registerAtomInRegularSubgroup ("display-msr1-voices-flat-view-details", subGroup);
  
  registerAtomInRegularSubgroup ("display-msr2-voices-flat-view", subGroup);
  registerAtomInRegularSubgroup ("display-msr2-voices-flat-viewdetails", subGroup);

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
#endif // MF_TRACE_IS_ENABLED

  registerAtomInRegularSubgroup ("quit-after-pass-2", subGroup);
  registerAtomInRegularSubgroup ("quit-after-pass-3", subGroup);
}

void xml2xmlRegularHandler::createWarningAndErrorsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Warning and errors group",
        "warning-and-errors-group",
        "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Warning and errors",
        "warning-and-errors",
        "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup (K_QUIET_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup ("dont-show-errors", subGroup);
  registerAtomInRegularSubgroup ("dont-quit-on-errors", subGroup);

  registerAtomInRegularSubgroup ("maintainance", subGroup);
}

void xml2xmlRegularHandler::createInputRegularGroup ()
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

void xml2xmlRegularHandler::createPresentationRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Presentation group",
        "presentation-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Presentation",
        "presentation", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("use-filename-as-work-title", subGroup);
  registerAtomInRegularSubgroup ("use-lyricists-as-poets", subGroup);
}

void xml2xmlRegularHandler::createPartsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Parts group",
        "parts-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Parts",
        "parts", "",
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
}

void xml2xmlRegularHandler::createStavesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Staves group",
        "staves-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Staves",
        "staves", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("create-voices-staff-relative-numbers", subGroup);
}

void xml2xmlRegularHandler::createVoicesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Voices group",
        "voices-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices",
        "voices", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-msr-voice", subGroup);
  registerAtomInRegularSubgroup ("keep-msr-voice", subGroup);

  registerAtomInRegularSubgroup ("create-single-line-staves-as-rythmic", subGroup);
}

void xml2xmlRegularHandler::createClefsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Clefs group",
        "clefs-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs",
        "clefs", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("msr-replace-clef", subGroup);
  registerAtomInRegularSubgroup ("ignore-redundant-clefs", subGroup);
}

void xml2xmlRegularHandler::createKeysRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Keys group",
        "keys-group",
        "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Keys",
        "keys", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-redundant-keys", subGroup);
}

void xml2xmlRegularHandler::createTimesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Times group",
        "times-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Times",
        "times", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-redundant-times", subGroup);
}

void xml2xmlRegularHandler::createMeasuresRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Measures group",
        "measures-group",
        "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "measures", "",
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
}

void xml2xmlRegularHandler::createRestsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Rests group",
        "rests-group",
        "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Rests",
        "rests", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("delay-rests-slashes", subGroup);
}

void xml2xmlRegularHandler::createNotesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Notes group",
        "notes-group",
        "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "notes", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("msr-pitches-language", subGroup);
}

void xml2xmlRegularHandler::creatBeamsRegularGroup ()
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

  registerAtomInRegularSubgroup ("delay-rests-beams", subGroup);
}

void xml2xmlRegularHandler::createArticulationsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Articulations group",
        "articulations-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Articulations",
        "articulations", "", "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-musicxml-articulations", subGroup);
}

void xml2xmlRegularHandler::createOrnamentsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ornaments group",
        "ornaments-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ornaments",
        "ornaments",
        "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-musicxml-ornaments", subGroup);
}

void xml2xmlRegularHandler::createGraceNotesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Grace notes group",
        "grace-notes-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Grace notes",
        "grace-notes", "",
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

void xml2xmlRegularHandler::createChordsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Chords group",
        "chords-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords",
        "chords", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  // JMI ???
}

void xml2xmlRegularHandler::createTiesRegularGroup ()
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

void xml2xmlRegularHandler::createSlursRegularGroup ()
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

void xml2xmlRegularHandler::createLigaturesRegularGroup ()
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

void xml2xmlRegularHandler::createDynamicRegularGroup ()
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
}

void xml2xmlRegularHandler::createWedgesRegularGroup ()
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

void xml2xmlRegularHandler::createTupletsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tuplets group",
        "tuplets-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tuplets",
        "tuplets", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  // JMI ???
}

void xml2xmlRegularHandler::createLyricsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Lyrics group",
        "lyrics-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Lyrics",
        "lyrics", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-musicxml-lyrics", subGroup);
}

void xml2xmlRegularHandler::createHarmoniesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Harmonies group",
        "harmonies-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies",
        "harmonies", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-musicxml-harmonies", subGroup);

  registerAtomInRegularSubgroup ("show-harmony-voices", subGroup);
}

void xml2xmlRegularHandler::createFiguredBassRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Figured bass elements group",
        "figured-bass-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Figured bass elements",
        "figured-bass", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-musicxml-figured-bass", subGroup);

  registerAtomInRegularSubgroup ("show-figured-bass-voices", subGroup);
}

void xml2xmlRegularHandler::createOutputRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Output group",
        "output-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Output",
        "output", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("musicxml-generation-infos", subGroup);
  registerAtomInRegularSubgroup ("musicxml-comments", subGroup);

  registerAtomInRegularSubgroup ("work-title", subGroup);
  registerAtomInRegularSubgroup ("work-number", subGroup);
  registerAtomInRegularSubgroup ("movement-title", subGroup);
  registerAtomInRegularSubgroup ("movement-number", subGroup);
}

//______________________________________________________________________________
void xml2xmlRegularHandler::checkOptionsAndArguments () const
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
void xml2xmlRegularHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2xmlRegularHandler '" << fHandlerHeader << "':" <<
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

std::ostream& operator << (std::ostream& os, const S_xml2xmlRegularHandler& elt)
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
