/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

// WAE
#include "oahWae.h"

#include "mfPreprocessorSettings.h"

#include "mfStringsHandling.h"

// OAH
#include "oahOah.h"

// MSR
// LPSR
// MXSR
#include "msrOah.h"
#include "msr2msrOah.h"


#include "msdlConverterOah.h"

#include "oahEarlyOptions.h"

#include "msdlConverterRegularHandler.h"

// WAE
#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdlConverterRegularHandler msdlConverterRegularHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader,
  const S_msdlConverterInsiderHandler&
                     insiderOahHandler,
  mfMultiGenerationOutputKind
                     multiGenerationOutputKind)
{
  // create the regular handler
  msdlConverterRegularHandler* obj = new
    msdlConverterRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler,
      multiGenerationOutputKind);
  assert (obj != nullptr);
  return obj;
}

msdlConverterRegularHandler::msdlConverterRegularHandler (
  const std::string& serviceName,
  const std::string& handlerHeader,
  const S_msdlConverterInsiderHandler&
                     insiderOahHandler,
  mfMultiGenerationOutputKind
                     multiGenerationOutputKind)
  : oahRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler)
{
  fMultiGenerationOutputKind = multiGenerationOutputKind;

  // this is done only only after the constructor has been executed,
  // because it uses pure virtual methods
  initializeOahRegularHandler ();

  // create the regular handler groups
  createRegularHandlerGroups ();

/* JMI
  // print the options handler initial state
  gLog <<
    "msdlConverterRegularHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    std::endl;

  ++gIndenter;

  gLog <<
    "===> printHelp():" <<
    std::endl;
  this->printHelp (gOutput); // JMI

  --gIndenter;
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    // JMI ???
  }
#endif // MF_TRACE_IS_ENABLED
}

msdlConverterRegularHandler::~msdlConverterRegularHandler ()
{}

void msdlConverterRegularHandler::createRegularHandlerGroups ()
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

  createInformationsRegularGroup ();

  createDisplayRegularGroup ();

  createMsdlRegularGroup ();

  createFilesRegularGroup ();

  createGenerateCodeRegularGroup ();

  // create the groups needed according to the generated output kind
  /*
    CAUTION:
      some option names are identical in OAH groups
      that are not meant to be used at the same time,
      such as gGlobalMsr2msrOahGroup and gGlobalMsr2lpsrOahGroup
  */

  switch (fMultiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      // create the braille OAH group
      createBrailleRegularGroup ();
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      // create the Guido OAH group
      createGuidoRegularGroup ();
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch

  createOahRegularGroup ();

  createWarningAndErrorsRegularGroup ();

/* JMI
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
  */

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

void msdlConverterRegularHandler::createInformationsRegularGroup ()
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

void msdlConverterRegularHandler::createDisplayRegularGroup ()
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

void msdlConverterRegularHandler::createMsdlRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "MSDL group",
        "help-msdl-group", "hmsdl-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MSDL",
        "help-msdl", "hmsdl",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup ("msdl-keywords-input-language", subGroup);
  registerAtomInRegularSubgroup ("display-keywords-in-language", subGroup);
  registerAtomInRegularSubgroup ("display-tokens-in-language", subGroup);
}

void msdlConverterRegularHandler::createGuidoRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Guido group",
        "guido-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Guido subgroup",
        "guido-subgroup", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("generate-guido-comments", subGroup);
  registerAtomInRegularSubgroup ("generate-guido-stem", subGroup);
  registerAtomInRegularSubgroup ("generate-guido-bars", subGroup);
}

void msdlConverterRegularHandler::createBrailleRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Braille group",
        "braille-group", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Braille output",
        "braille-output", "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup (K_BRAILLE_USE_ENCODING_IN_FILE_NAME_LONG_NAME, subGroup);
  registerAtomInRegularSubgroup (K_BRAILLE_BYTE_ORDERING_KIND_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup ("ascii", subGroup);
  registerAtomInRegularSubgroup ("utf8", subGroup);
  registerAtomInRegularSubgroup ("utf8d", subGroup);
  registerAtomInRegularSubgroup ("utf16", subGroup);
  registerAtomInRegularSubgroup ("auto-utf8d", subGroup);
}

//_______________________________________________________________________________
void msdlConverterRegularHandler::createFilesRegularGroup ()
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

void msdlConverterRegularHandler::createOahRegularGroup ()
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
  registerAtomInRegularSubgroup (K_TRACE_OAH_OPTION_LONG_NAME, subGroup);
  registerAtomInRegularSubgroup (K_TRACE_OAH_DETAILS_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup (K_TRACE_PASSES_OPTION_LONG_NAME, subGroup);
  registerAtomInRegularSubgroup (K_TRACE_ONLY_PASS_OPTION_LONG_NAME, subGroup);
#endif // MF_TRACE_IS_ENABLED

  // atoms from the insider handler depending on the generated output kind
  switch (fMultiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      registerAtomInRegularSubgroup ("jianpu", subGroup);
      registerAtomInRegularSubgroup ("lyluatex", subGroup);

      registerAtomInRegularSubgroup ("lilypond-generation-infos", subGroup);
      registerAtomInRegularSubgroup ("lilypond-comments", subGroup);
      registerAtomInRegularSubgroup ("input-line-numbers", subGroup);

  registerAtomInRegularSubgroup ("notes-comments", subGroup);
      registerAtomInRegularSubgroup ("global-staff-size", subGroup);
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      registerAtomInRegularSubgroup ("display-bsr1", subGroup);
      registerAtomInRegularSubgroup ("display-bsr2", subGroup);

      registerAtomInRegularSubgroup ("display-bsr1", subGroup);
      registerAtomInRegularSubgroup ("display-bsr2", subGroup);

      registerAtomInRegularSubgroup ("display-bsr1-details", subGroup);
      registerAtomInRegularSubgroup ("display-bsr2-details", subGroup);
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
#ifdef MF_TRACE_IS_ENABLED
      registerAtomInRegularSubgroup ("trace-encoding", subGroup);
      registerAtomInRegularSubgroup ("trace-divisions", subGroup);

      registerAtomInRegularSubgroup ("trace-backup", subGroup);
      registerAtomInRegularSubgroup ("trace-forward", subGroup);
#endif // MF_TRACE_IS_ENABLED
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
#ifdef MF_TRACE_IS_ENABLED
      registerAtomInRegularSubgroup ("trace-encoding", subGroup);
      registerAtomInRegularSubgroup ("trace-divisions", subGroup);

      registerAtomInRegularSubgroup ("trace-backup", subGroup);
      registerAtomInRegularSubgroup ("trace-forward", subGroup);
#endif // MF_TRACE_IS_ENABLED
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch
}

void msdlConverterRegularHandler::createWarningAndErrorsRegularGroup ()
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
}

void msdlConverterRegularHandler::createGenerateCodeRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Generated output group",
        "help-generate-output-group", "hgc-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generated output",
        "help-generate-output", "hgo",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup (K_GENERATED_OUTPUT_KIND_GUIDO_NAME, subGroup);
  registerAtomInRegularSubgroup (K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME, subGroup);
  registerAtomInRegularSubgroup (K_GENERATED_OUTPUT_KIND_BRAILLE_NAME, subGroup);
  registerAtomInRegularSubgroup (K_GENERATED_OUTPUT_KIND_MUSICXML_NAME, subGroup);
// JMI  registerAtomInRegularSubgroup (K_GENERATED_OUTPUT_KIND_MIDI_NAME, subGroup);
}

void msdlConverterRegularHandler::createPresentationRegularGroup ()
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

void msdlConverterRegularHandler::createPartsRegularGroup ()
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

void msdlConverterRegularHandler::createStavesRegularGroup ()
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

  registerAtomInRegularSubgroup ("lpsr-staff-instrument-name", subGroup);

  registerAtomInRegularSubgroup ("create-voices-staff-relative-numbers", subGroup);
}

void msdlConverterRegularHandler::createVoicesRegularGroup ()
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

void msdlConverterRegularHandler::createClefsRegularGroup ()
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

void msdlConverterRegularHandler::createKeysRegularGroup ()
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

void msdlConverterRegularHandler::createTimesRegularGroup ()
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

void msdlConverterRegularHandler::createMeasuresRegularGroup ()
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

void msdlConverterRegularHandler::createRestsRegularGroup ()
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

void msdlConverterRegularHandler::createNotesRegularGroup ()
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

void msdlConverterRegularHandler::creatBeamsRegularGroup ()
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

void msdlConverterRegularHandler::createArticulationsRegularGroup ()
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

void msdlConverterRegularHandler::createOrnamentsRegularGroup ()
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

void msdlConverterRegularHandler::createGraceNotesRegularGroup ()
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

void msdlConverterRegularHandler::createChordsRegularGroup ()
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

#ifdef MF_TRACE_IS_ENABLED
  registerAtomInRegularSubgroup ("trace-chords", subGroup);
#endif // MF_TRACE_IS_ENABLED
}

void msdlConverterRegularHandler::createTiesRegularGroup ()
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

void msdlConverterRegularHandler::createSlursRegularGroup ()
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

void msdlConverterRegularHandler::createLigaturesRegularGroup ()
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

void msdlConverterRegularHandler::createDynamicRegularGroup ()
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

void msdlConverterRegularHandler::createWedgesRegularGroup ()
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

void msdlConverterRegularHandler::createTupletsRegularGroup ()
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

#ifdef MF_TRACE_IS_ENABLED
  registerAtomInRegularSubgroup ("trace-tuplets", subGroup);
#endif // MF_TRACE_IS_ENABLED

  registerAtomInRegularSubgroup ("display-tuplets", subGroup);
}

void msdlConverterRegularHandler::createLyricsRegularGroup ()
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

void msdlConverterRegularHandler::createHarmoniesRegularGroup ()
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

void msdlConverterRegularHandler::createFiguredBassRegularGroup ()
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

//______________________________________________________________________________
void msdlConverterRegularHandler::checkOptionsAndArguments () const
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

  if (MSDR_STANDARD_INPUT_NAME == std::string ("-")) {
    checkSingleInputSourceInArgumentsVector ();
  }
  else if (MSDR_STANDARD_INPUT_NAME == std::string ("")) {
    checkNoOrOneInputSourceInArgumentsVector ();
  }
  else {
    oahInternalError (
      "MSDR_STANDARD_INPUT_NAME should be empty or contain a single '-'");
  }
}

//______________________________________________________________________________
void msdlConverterRegularHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msdlConverterRegularHandler '" << fHandlerHeader << "':" <<
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

std::ostream& operator << (std::ostream& os, const S_msdlConverterRegularHandler& elt)
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
