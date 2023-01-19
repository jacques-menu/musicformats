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

// libmusicxml2
#include "visitor.h"

#include "oahWae.h"

#include "mfStaticSettings.h"

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "msrInitialization.h"
#include "lpsrInitialization.h"
#include "bsrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "oahDisplayOah.h"

#include "mfOutputFileOah.h"
#include "oahHarmoniesExtraOah.h"

// Guido
#include "guidoGenerationOah.h"

// MXSR
#include "mxsrOah.h"
#include "msr2mxsrOah.h"

// MSR
#include "msrOah.h"
#include "msr2msrOah.h"

// early options
#include "oahEarlyOptions.h"

// Lilypond
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

// Braille
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"

// generation
#include "msrGenerationOah.h"

// history
#include "LilyPondIssue34Component.h"

#include "LilyPondIssue34Oah.h"
#include "LilyPondIssue34InsiderHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_LilyPondIssue34InsiderHandler LilyPondIssue34InsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader,
  mfMultiGenerationOutputKind
                          multiGenerationOutputKind)
{
  // create the insider handler
  LilyPondIssue34InsiderHandler* o = new
    LilyPondIssue34InsiderHandler (
      serviceName,
      handlerHeader,
      multiGenerationOutputKind);
  assert (o != nullptr);
  return o;
}

LilyPondIssue34InsiderHandler::LilyPondIssue34InsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader,
  mfMultiGenerationOutputKind
                          multiGenerationOutputKind)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                   Welcome to LilyPondIssue34,
     a generator of Guido, MusicXML, LilyPond or Braille
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
      usageInformation (
        multiGenerationOutputKind)),
      fMultiGenerationOutputKind (
        multiGenerationOutputKind)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler" <<
      std::endl;
  }
#endif

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the LilyPondIssue34 prefixes
  createTheLilyPondIssue34Prefixes ();

  // create the LilyPondIssue34 option groups
  createTheLilyPondIssue34OptionGroups (
    serviceName,
    multiGenerationOutputKind);
}

LilyPondIssue34InsiderHandler::~LilyPondIssue34InsiderHandler ()
{}

void LilyPondIssue34InsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createLilyPondIssue34GeneratorComponent ();
}

std::string LilyPondIssue34InsiderHandler::usageInformation (
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  std::stringstream s;

  s <<
R"(Usage: LilyPondIssue34 [option]*
)" <<
    std::endl;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN:
      s <<
        "The help below is available whichever output is produced";
      break;

    default:
      s <<
        "The help below is available when generating " <<
        mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
        " output";
  } // switch

  return s.str ();
}

std::string LilyPondIssue34InsiderHandler::handlerServiceAboutInformation () const
{
  return
    LilyPondIssue34AboutInformation (
      fMultiGenerationOutputKind);
}

std::string LilyPondIssue34InsiderHandler::LilyPondIssue34AboutInformation (
  mfMultiGenerationOutputKind multiGenerationOutputKind) const
{
  std::string result;

  size_t passesNumber = 0;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN:
      // should not occur
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      passesNumber = 4;
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      passesNumber = 4;
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      passesNumber = 4;
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      passesNumber = 4;
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      passesNumber = 0;
      break;
  } // switch

  std::string headPart;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN:
      // should only occur if the run is a pure help one
      headPart =
R"(What LilyPondIssue34 does:

    This multi-pass generator creates a textual representation
    of a score illustrating the LilyPond issue 34,
    in which grace notes occur at the beginning of only one of the two staves.
    It performs various passes depending on the output generated,
    which should be specified a '-lilypond', '-braille', '-musicxml' or '-guido' option.)";
      break;

    default:
      {
        std::stringstream headPartStream;

        headPartStream <<
R"(What LilyPondIssue34 does:

    This multi-pass generator creates a textual representation
    of the LilyPondIssue34 score.
    It basically performs )" <<
          passesNumber <<
          " passes when generating " <<
          mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
          " output:" <<
          std::endl <<
R"(
        Pass 1:  generate a first MSR for the LilyPondIssue34 score)";

        headPart = headPartStream.str ();
      }
  } // switch


  std::string specificPart;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      specificPart =
R"(
        Pass 2:  converts the first MSR a second MSR;
        Pass 3:  converts the second MSR into a
                 LilyPond Score Representation (LPSR);
        Pass 4:  converts the LPSR to LilyPond code
                 and writes it to standard output.)";
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      specificPart =
R"(
        Pass 2:  converts the first MSR a second MSR, to apply options;
        Pass 3:  converts the second MSR into a
                 Braille Score Representation (BSR)
                 containing one Braille page per MusicXML page;
        Pass 4:  converts the BSRinto another BSR
                 with as many Braille pages as needed
                 to fit the line and page lengthes;
        Pass 5:  converts the BSR to Braille text
                 and writes it to standard output.)

    In this preliminary version, pass 3 merely clones the BSR it receives.)";
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      specificPart =
R"(
        Pass 2:  converts the first MSR a second MSR, to apply options;
        Pass 3:  converts the second MSR into an MusicXML tree;
        Pass 4:  converts the MusicXML tree to MusicXML code
                 and writes it to standard output.)";
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      specificPart =
R"(
        Pass 2:  converts the first MSR into a second MSR, to apply options;
        Pass 3:  converts the second MSR into an MusicXML tree;
        Pass 4:  converts the MusicXML tree to Guido code
                 and writes it to standard output.)";
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch

   std::string commonTailPart =
R"(

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";

 return
   headPart + specificPart + commonTailPart;
}

//______________________________________________________________________________
void LilyPondIssue34InsiderHandler::createTheLilyPondIssue34Prefixes ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the LilyPondIssue34 prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void LilyPondIssue34InsiderHandler::createTheLilyPondIssue34OptionGroups (
  const std::string&       serviceName,
  mfMultiGenerationOutputKind mfMultiGenerationOutputKind)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the \"" <<
      fHandlerHeader <<
      "\" insider option groups" <<
      std::endl;
  }
#endif

  // initialize options handling, phase 1
  // ------------------------------------------------------

  // create the OAH OAH group first
  appendGroupToHandler (
    createGlobalOahOahGroup (
      serviceName));

  // create the WAE OAH group
  appendGroupToHandler (
    createGlobalWaeOahGroup ());

#ifdef MF_TRACING_IS_ENABLED
  // create the tracing OAH group
  appendGroupToHandler (
    createGlobalTracingOahGroup (
      this));
#endif

  // create the MSR generator OAH group
  appendGroupToHandler (
    createGlobalMsrGeneratorsOahGroup ());

  // create the output file OAH group
  appendGroupToHandler (
    createGlobalOutputFileOahGroup ());

  // create the display OAH group
  appendGroupToHandler (
    createGlobalDisplayOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  initializeMSR (); // used whatever the output kind

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the LilyPondIssue34 OAH group
  appendGroupToHandler (
    createGlobalLilyPondIssue34OahGroup ());

  // create the LilyPondIssue34 insider OAH group
  appendGroupToHandler (
    createGlobalLilyPondIssue34InsiderOahGroup ());

  // create the groups needed according to the generated output kind
  /*
    CAUTION:
      some option names are identical in OAH groups
      that are not meant to be used at the same time,
      such as gGlobalMsr2msrOahGroup and gGlobalMsr2lpsrOahGroup
  */

  switch (mfMultiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      // initialize the library
      initializeLPSR ();

      // create the msr2lpsr OAH group
      appendGroupToHandler (
        createGlobalMsr2lpsrOahGroup ());

      // create the LPSR OAH group
      appendGroupToHandler (
        createGlobalLpsrOahGroup ());

      // create the LilyPond generation OAH group
      appendGroupToHandler (
        createGlobalLpsr2lilypondOahGroup ());
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      // initialize the library
      initializeBSR ();

      // create the msr2bsr OAH group
      appendGroupToHandler (
        createGlobalMsr2bsrOahGroup ());

      // create the BSR OAH group
      appendGroupToHandler (
        createGlobalBsrOahGroup ());

      // create the braille generation OAH group
      appendGroupToHandler (
        createGlobalBrailleGenerationOahGroup ());
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      // create the msr2msr OAH group
      appendGroupToHandler (
        createGlobalMsr2msrOahGroup ());

      // create the msr2mxsr OAH group
      appendGroupToHandler (
        createGlobalMsr2mxsrOahGroup ());

      // create the MXSR OAH group
      appendGroupToHandler (
        createGlobalMxsrOahGroup ());
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      // create the msr2msr OAH group
      appendGroupToHandler (
        createGlobalMsr2msrOahGroup ());

      // create the msr2mxsr OAH group
      appendGroupToHandler (
        createGlobalMsr2mxsrOahGroup ());

      // create the MXSR OAH group
      appendGroupToHandler (
        createGlobalMxsrOahGroup ());

      // create the Guido generation OAH group
      appendGroupToHandler (
        createGlobalGuidoGenerationOahGroup ());
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch

  // create the multi generator OAH group
  appendGroupToHandler (
    createGlobalMultiGenerationOahGroup ());

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif
}

//______________________________________________________________________________
void LilyPondIssue34InsiderHandler::checkOptionsAndArguments () const
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  checkNoInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
std::string LilyPondIssue34InsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Fetching the output file name from the options in OAH handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  S_oahStringAtom
    outputFileNameStringAtom =
      gGlobalOutputFileOahGroup->
        getOutputFileNameStringAtom ();

  S_oahBooleanAtom
    autoOutputFileNameAtom =
      gGlobalOutputFileOahGroup->
        getAutoOutputFileNameAtom ();

  Bool
    outputFileNameHasBeenSet =
      outputFileNameStringAtom->
        getSetByAnOption ();

  Bool
    autoOutputFileNameHasBeenSet =
      autoOutputFileNameAtom->
        getSetByAnOption ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "LilyPondIssue34InsiderHandler::fetchOutputFileNameFromTheOptions()" <<
      " outputFileNameHasBeenSet: " <<
      outputFileNameHasBeenSet <<
      " autoOutputFileNameHasBeenSet: " <<
      autoOutputFileNameHasBeenSet <<
      std::endl;
  }
#endif

  std::string outputFileName;

  if (outputFileNameHasBeenSet) {
    // '-o, -output-file-name' has been chosen
    outputFileName =
      outputFileNameStringAtom->
        getStringVariable ();
  }

  else if (autoOutputFileNameHasBeenSet) {
     // '-aofn, -auto-output-file-name' has been chosen

    // start with the service name
    outputFileName = fHandlerServiceName;

    // add the output file name suffix
    switch (fMultiGenerationOutputKind) {
      case mfMultiGenerationOutputKind::kGeneration_UNKNOWN:
        // should not occur
        outputFileName = "___kGeneration_UNKNOWN____";
        break;

      case mfMultiGenerationOutputKind::kGenerationLilypond:
        outputFileName += ".ly";
        break;

      case mfMultiGenerationOutputKind::kGenerationBraille:
        {
          S_oahStringAtom
            outputFileNameStringAtom =
              gGlobalOutputFileOahGroup->
                getOutputFileNameStringAtom ();

          // should encoding be used by the output file name?
          bsrBrailleOutputKind
            brailleOutputKind =
              gGlobalBrailleGenerationOahGroup->
                getBrailleOutputKind ();

          if (gGlobalBrailleGenerationOahGroup->getUseEncodingInFileName ()) {
            switch (brailleOutputKind) {
              case bsrBrailleOutputKind::kBrailleOutputAscii:
                outputFileName += "_ASCII";
                break;

              case bsrBrailleOutputKind::kBrailleOutputUTF8:
                outputFileName += "_UTF8";
                  /* JMI
                switch (gGlobalBrailleGenerationOahGroup->getByteOrderingKind ()) {
                  case bsrByteOrderingKind::kByteOrderingNone:
                    break;
                  case bsrByteOrderingKind::kByteOrderingBigEndian:
                    outputFileName += "_BE";
                    break;
                  case bsrByteOrderingKind::kByteOrderingSmallEndian:
                    // should not occur JMI
                    break;
                } // switch
                */
                break;

              case bsrBrailleOutputKind::kBrailleOutputUTF8Debug:
                outputFileName += "_UTF8Debug";
                break;

              case bsrBrailleOutputKind::kBrailleOutputUTF16:
                outputFileName += "_UTF16";
                switch (gGlobalBrailleGenerationOahGroup->getByteOrderingKind ()) {
                  case bsrByteOrderingKind::kByteOrderingNone:
                    break;

                  case bsrByteOrderingKind::kByteOrderingBigEndian:
                    outputFileName += "_BE";
                    break;

                  case bsrByteOrderingKind::kByteOrderingSmallEndian:
                    outputFileName += "_SE";
                    break;
                } // switch
                break;
            } // switch
          }

          outputFileName += ".brf";
        }
        break;

      case mfMultiGenerationOutputKind::kGenerationMusicXML:
        outputFileName += ".xml";
        break;

      case mfMultiGenerationOutputKind::kGenerationGuido:
        outputFileName += ".gmn";
        break;

      case mfMultiGenerationOutputKind::kGenerationMidi:
        outputFileName += ".midi";
        break;
    } // switch
  }

  else {
    // neither outputFileNameHasBeenSet nor autoOutputFileNameHasBeenSet
    // has been set:
    // return empty outputFileName to indicate that output goes to stdout
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "outputFileName: " <<
      outputFileName <<
      std::endl;
  }
#endif

  return outputFileName;
}

//______________________________________________________________________________
void LilyPondIssue34InsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void LilyPondIssue34InsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalLilyPondIssue34InsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2mxsrOahGroup->
    enforceGroupQuietness ();

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED
  gGlobalHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void LilyPondIssue34InsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void LilyPondIssue34InsiderOahGroup::checkGroupOptionsConsistency ()
{
/* JMI

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) {
    std::stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (s.str ());
  }




  if (! fOutputFileName.size ()) {
    std::stringstream s;

    s <<
      "LilyPondIssue34InsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

    oahError (s.str ());
  }

  else if (fOutputFileName == gGlobalServiceRunData->getInputSourceName ()) {
    std::stringstream s;

    s <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (s.str ());
  }
  */
}

//______________________________________________________________________________
void LilyPondIssue34InsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> LilyPondIssue34InsiderOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_LilyPondIssue34InsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_LilyPondIssue34InsiderOahGroup>*> (v)) {
        S_LilyPondIssue34InsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching LilyPondIssue34InsiderOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void LilyPondIssue34InsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> LilyPondIssue34InsiderOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_LilyPondIssue34InsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_LilyPondIssue34InsiderOahGroup>*> (v)) {
        S_LilyPondIssue34InsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching LilyPondIssue34InsiderOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void LilyPondIssue34InsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> LilyPondIssue34InsiderOahGroup::browseData ()" <<
      std::endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void LilyPondIssue34InsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "LilyPondIssue34InsiderHandler:" <<
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
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_LilyPondIssue34InsiderHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_LilyPondIssue34InsiderOahGroup gGlobalLilyPondIssue34InsiderOahGroup;

S_LilyPondIssue34InsiderOahGroup LilyPondIssue34InsiderOahGroup::create ()
{
  LilyPondIssue34InsiderOahGroup* o = new LilyPondIssue34InsiderOahGroup ();
  assert (o != nullptr);
  return o;
}

LilyPondIssue34InsiderOahGroup::LilyPondIssue34InsiderOahGroup ()
  : oahGroup (
    "LilyPondIssue34",
    "help-LilyPondIssue34", "hlpi34",
R"(Options that are used by LilyPondIssue34 are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
// JMI  fGenerationAPIKind = msrGenerationAPIKind::kMsrFunctionsAPIKind; // default value
  fGenerationAPIKind = msrGenerationAPIKind::kMsrStringsAPIKind; // default value

  fMultiGenerationOutputKind = mfMultiGenerationOutputKind::kGeneration_UNKNOWN;

  initializeLilyPondIssue34InsiderOahGroup ();
}

LilyPondIssue34InsiderOahGroup::~LilyPondIssue34InsiderOahGroup ()
{}

void LilyPondIssue34InsiderOahGroup::initializeLilyPondIssue34InsiderOahGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream << std::left <<
      "Initializing \"" <<
      fGroupHeader <<
      "\" group" <<
      std::endl;
  }
#endif

  // JMI ???
}

void LilyPondIssue34InsiderOahGroup::printLilyPondIssue34InsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The LilyPondIssue34 options are:" <<
    std::endl;

  ++gIndenter;

  // generation API kind
  // --------------------------------------

  gLogStream << std::left <<
    std::setw (fieldWidth) << "Generation API kind:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "msrGenerationAPIKind" << ": " <<
    msrGenerationAPIKindAsString (fGenerationAPIKind) <<
    std::endl;

  --gIndenter;

  // generate output kind
  // --------------------------------------

  gLogStream << std::left <<
    std::setw (fieldWidth) << "Generated output kind:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "mfMultiGenerationOutputKind" << ": " <<
    mfMultiGenerationOutputKindAsString (fMultiGenerationOutputKind) <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_LilyPondIssue34InsiderOahGroup createGlobalLilyPondIssue34InsiderOahGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global LilyPondIssue34 insider OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalLilyPondIssue34InsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalLilyPondIssue34InsiderOahGroup =
      LilyPondIssue34InsiderOahGroup::create ();
    assert (gGlobalLilyPondIssue34InsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalLilyPondIssue34InsiderOahGroup;
}


}
