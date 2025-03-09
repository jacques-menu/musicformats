/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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

#include "mfPreprocessorSettings.h"

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "msrInitialization.h"
#include "lpsrInitialization.h"
#include "bsrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "oahDisplayOah.h"
#include "oahHarmoniesExtraOah.h"

#include "mfOutputFileOah.h"
#include "oahHarmoniesExtraOah.h"

// Guido
#include "guidoGenerationOah.h"

// MXSR
#include "mxsr2msrOah.h"
#include "msr2mxsrOah.h"

// MSR
#include "msrOah.h"
#include "msr2msrOah.h"

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

// early options
#include "oahEarlyOptions.h"

// history
#include "Mikrokosmos3WanderingComponent.h"

// WAE
#include "waeHandlers.h"

#include "Mikrokosmos3WanderingOah.h"
#include "Mikrokosmos3WanderingInsiderHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_Mikrokosmos3WanderingInsiderHandler Mikrokosmos3WanderingInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader,
  mfMultiGenerationOutputKind
                          multiGenerationOutputKind)
{
  // create the insider handler
  Mikrokosmos3WanderingInsiderHandler* obj = new
    Mikrokosmos3WanderingInsiderHandler (
      serviceName,
      handlerHeader,
      multiGenerationOutputKind);
  assert (obj != nullptr);
  return obj;
}

Mikrokosmos3WanderingInsiderHandler::Mikrokosmos3WanderingInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader,
  mfMultiGenerationOutputKind
                          multiGenerationOutputKind)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                Welcome to Mikrokosmos3Wandering,
     a generator of Guido, MusicXML, LilyPond or Braille
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
      usageInformation (
        multiGenerationOutputKind)),
      fMultiGenerationOutputKind (
        multiGenerationOutputKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the Mikrokosmos3Wandering prefixes
  createTheMikrokosmos3WanderingPrefixes ();

  // create the Mikrokosmos3Wandering option groups
  createTheMikrokosmos3WanderingOptionGroups (
    serviceName,
    multiGenerationOutputKind);
}

Mikrokosmos3WanderingInsiderHandler::~Mikrokosmos3WanderingInsiderHandler ()
{}

void Mikrokosmos3WanderingInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMikrokosmos3WanderingGeneratorComponent ();
}

std::string Mikrokosmos3WanderingInsiderHandler::usageInformation (
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  std::stringstream ss;

  ss <<
R"(Usage: Mikrokosmos3Wandering [option]*
)" <<
    std::endl;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      ss <<
        "The help below is available whichever output is produced";
      break;

    default:
      ss <<
        "The help below is available when generating " <<
        mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
        " output";
  } // switch

  return ss.str ();
}

std::string Mikrokosmos3WanderingInsiderHandler::handlerServiceAboutInformation () const
{
  return
    Mikrokosmos3WanderingAboutInformation (
      fMultiGenerationOutputKind);
}

std::string Mikrokosmos3WanderingInsiderHandler::Mikrokosmos3WanderingAboutInformation (
  mfMultiGenerationOutputKind multiGenerationOutputKind) const
{
  std::string result;

  size_t passesNumber = 0;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
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
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      // should only occur if the run is a pure help one
      headPart =
R"(What Mikrokosmos3Wandering does:

    This multi-pass generator creates a textual representation
    of Zoltán Kodály's Mikrokosmos III Wandering score.
    It performs various passes depending on the output generated,
    which should be specified a '-lilypond', '-braille', '-musicxml' or '-guido' option.)";
      break;

    default:
      {
        std::stringstream headPartStream;

        headPartStream <<
R"(What Mikrokosmos3Wandering does:

    This multi-pass generator creates a textual representation
    of Zoltán Kodály's Mikrokosmos III Wandering score.
    It basically performs )" <<
          passesNumber <<
          " passes when generating " <<
          mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
          " output:" <<
          std::endl <<
R"(
        Pass 1:  generate a first MSR for the Mikrokosmos III Wandering score)";

        headPart = headPartStream.str ();
      }
  } // switch


  std::string specificPart;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
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
                 to fit the line and page lengths;
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
void Mikrokosmos3WanderingInsiderHandler::createTheMikrokosmos3WanderingPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the Mikrokosmos3Wandering prefixes in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderHandler::createTheMikrokosmos3WanderingOptionGroups (
  const std::string&        serviceName,
  mfMultiGenerationOutputKind mfMultiGenerationOutputKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the \"" <<
      fHandlerHeader <<
      "\" insider option groups";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // initialize options handling, phase 1
  // ------------------------------------------------------

  // create the OAH OAH group first
  appendGroupToHandler (
    createGlobalOahOahGroup (
      serviceName));

  // create the WAE OAH group
  appendGroupToHandler (
    createGlobalWaeOahGroup ());

#ifdef MF_TRACE_IS_ENABLED
  // create the trace OAH group
  appendGroupToHandler (
    createGlobalTraceOahGroup (
      this));
#endif // MF_TRACE_IS_ENABLED

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

  // create the Mikrokosmos3Wandering OAH group
  appendGroupToHandler (
    createGlobalMikrokosmos3WanderingOahGroup ());

  // create the Mikrokosmos3Wandering insider OAH group
  appendGroupToHandler (
    createGlobalMikrokosmos3WanderingInsiderOahGroup ());

  // create the groups needed according to the generated output kind
  /*
    CAUTION:
      some option names are identical in OAH groups
      that are not meant to be used at the same time,
      such as gGlobalMsr2msrOahGroup and gGlobalMsr2lpsrOahGroup
  */

  switch (mfMultiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
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
        createGlobalMsr2mxsr2msrOahGroup ());
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      // create the msr2msr OAH group
      appendGroupToHandler (
        createGlobalMsr2msrOahGroup ());

      // create the msr2mxsr OAH group
      appendGroupToHandler (
        createGlobalMsr2mxsr2msrOahGroup ());

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

  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderHandler::checkOptionsAndArguments () const
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

  checkNoInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
std::string Mikrokosmos3WanderingInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Fetching the output file name from the options in OAH handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        getSelected ();

  Bool
    autoOutputFileNameHasBeenSet =
      autoOutputFileNameAtom->
        getSelected ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Mikrokosmos3WanderingInsiderHandler::fetchOutputFileNameFromTheOptions()" <<
      " outputFileNameHasBeenSet: " <<
      outputFileNameHasBeenSet <<
      " autoOutputFileNameHasBeenSet: " <<
      autoOutputFileNameHasBeenSet;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string outputFileName;

  if (outputFileNameHasBeenSet) {
    // '-o, -output-file-name' has been selected
    outputFileName =
      outputFileNameStringAtom->
        getStringVariable ();
  }

  else if (autoOutputFileNameHasBeenSet) {
     // '-aofn, -auto-output-file-name' has been selected

    // start with the service name
    outputFileName = fHandlerServiceName;

    // add the output file name suffix
    switch (fMultiGenerationOutputKind) {
      case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
        // should not occur
        outputFileName = "___kGeneration_UNKNOWN_____";
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

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "outputFileName: " <<
      outputFileName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return outputFileName;
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalMikrokosmos3WanderingInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsr2msrOahGroup->
    enforceGroupQuietness ();

  gMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2mxsr2msrOahGroup->
    enforceGroupQuietness ();

  gHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::checkGroupOptionsConsistency ()
{
/* JMI

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) {
    std::stringstream ss;

    ss <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (ss.str ());
  }




  if (! fOutputFileName.size ()) {
    std::stringstream ss;

    ss <<
      "Mikrokosmos3WanderingInsiderOahGroup: a MusicXML output file name must be selected with '-o, -output-file-name";

    oahError (ss.str ());
  }

  else if (fOutputFileName == gServiceRunData->getInputSourceName ()) {
    std::stringstream ss;

    ss <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (ss.str ());
  }
  */
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> Mikrokosmos3WanderingInsiderOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_Mikrokosmos3WanderingInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingInsiderOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching Mikrokosmos3WanderingInsiderOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void Mikrokosmos3WanderingInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> Mikrokosmos3WanderingInsiderOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_Mikrokosmos3WanderingInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingInsiderOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching Mikrokosmos3WanderingInsiderOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void Mikrokosmos3WanderingInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> Mikrokosmos3WanderingInsiderOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "Mikrokosmos3WanderingInsiderHandler:" <<
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

    std::list <S_oahGroup>::const_iterator
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

std::ostream& operator << (std::ostream& os, const S_Mikrokosmos3WanderingInsiderHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_Mikrokosmos3WanderingInsiderOahGroup gGlobalMikrokosmos3WanderingInsiderOahGroup;

S_Mikrokosmos3WanderingInsiderOahGroup Mikrokosmos3WanderingInsiderOahGroup::create ()
{
  Mikrokosmos3WanderingInsiderOahGroup* obj = new Mikrokosmos3WanderingInsiderOahGroup ();
  assert (obj != nullptr);
  return obj;
}

Mikrokosmos3WanderingInsiderOahGroup::Mikrokosmos3WanderingInsiderOahGroup ()
  : oahGroup (
      "Mikrokosmos3Wandering",
      "help-Mikrokosmos3Wandering", "hmkk3w",
R"(Options that are used by Mikrokosmos3Wandering are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
// JMI  fGenerationAPIKind = msrGenerationAPIKind::kMsrFunctionsAPIKind; // default value
  fGenerationAPIKind = msrGenerationAPIKind::kMsrStringsAPIKind; // default value

  fMultiGenerationOutputKind = mfMultiGenerationOutputKind::kGeneration_UNKNOWN_;

  initializeMikrokosmos3WanderingInsiderOahGroup ();
}

Mikrokosmos3WanderingInsiderOahGroup::~Mikrokosmos3WanderingInsiderOahGroup ()
{}

void Mikrokosmos3WanderingInsiderOahGroup::initializeMikrokosmos3WanderingInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing \"" <<
      fGroupHeader <<
      "\" group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI ???
}

void Mikrokosmos3WanderingInsiderOahGroup::printMikrokosmos3WanderingInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The Mikrokosmos3Wandering options are:" <<
    std::endl;

  ++gIndenter;

  // generation API kind
  // --------------------------------------

  gLog << std::left <<
    std::setw (fieldWidth) << "Generation API kind:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "msrGenerationAPIKind" << ": " <<
    msrGenerationAPIKindAsString (fGenerationAPIKind) <<
    std::endl;

  --gIndenter;

  // generate output kind
  // --------------------------------------

  gLog << std::left <<
    std::setw (fieldWidth) << "Generated output kind:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "mfMultiGenerationOutputKind" << ": " <<
    mfMultiGenerationOutputKindAsString (fMultiGenerationOutputKind) <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_Mikrokosmos3WanderingInsiderOahGroup createGlobalMikrokosmos3WanderingInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global Mikrokosmos3Wandering insider OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMikrokosmos3WanderingInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMikrokosmos3WanderingInsiderOahGroup =
      Mikrokosmos3WanderingInsiderOahGroup::create ();
    assert (gGlobalMikrokosmos3WanderingInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMikrokosmos3WanderingInsiderOahGroup;
}


}
