/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

// libmusicxml2
#include "visitor.h"

// WAE
#include "oahWae.h"

#include "mfStaticSettings.h"

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "msrInitialization.h"
#include "lpsrInitialization.h"
#include "bsrInitialization.h"

// OAH
#include "oahOah.h"
#include "waeOah.h"
#include "oahDisplayOah.h"

#include "mfOutputFileOah.h"
#include "oahHarmoniesExtraOah.h"


// MSDL
#include "msdl.h"

#include "msdlInputOah.h"
#include "msdl2msrOah.h"


// MSR
#include "msrOah.h"
#include "msr2msrOah.h"


// Guido
#include "musicxml2guidoInsiderHandler.h"

#include "msdl2guidoInsiderHandler.h"


// MXSR
#include "mxsrOah.h"
#include "msr2mxsrOah.h"
#include "msr2mxsrOah.h"

#include "msdl2musicxmlInsiderHandler.h"


// Lilypond
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "msdl2lilypondInsiderHandler.h"


// Braille
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "msdl2brailleInsiderHandler.h"


// earlh options
#include "oahEarlyOptions.h"


// generation
#include "msrGenerationOah.h"
#include "guidoGenerationOah.h"

// components
#include "msdlComponent.h"
#include "msdlConverterComponent.h"


#include "msdlConverterInsiderHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdlConverterInsiderHandler msdlConverterInsiderHandler::create (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  // create the insider handler
  msdlConverterInsiderHandler* o = new
    msdlConverterInsiderHandler (
      serviceName,
      handlerHeader,
      multiGenerationOutputKind);
  assert (o != nullptr);
  return o;
}

msdlConverterInsiderHandler::msdlConverterInsiderHandler (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  mfMultiGenerationOutputKind multiGenerationOutputKind)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                   Welcome to the MSDL converter,
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
		std::stringstream ss;

    ss <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the msdlConverter prefixes
  createTheMsdlConverterPrefixes ();

  // create the msdlConverter option groups
  createTheMsdlConverterOptionGroups (
    serviceName,
    multiGenerationOutputKind);
}

msdlConverterInsiderHandler::~msdlConverterInsiderHandler ()
{}

void msdlConverterInsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createMsdlConverterConverterComponent ();
}

std::string msdlConverterInsiderHandler::usageInformation (
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  std::stringstream ss;

  ss <<
R"(Usage: msdl [option]*
)" <<
    std::endl;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN:
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

std::string msdlConverterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    msdlConverterAboutInformation (
      fMultiGenerationOutputKind);
}

std::string msdlConverterInsiderHandler::msdlConverterAboutInformation (
  mfMultiGenerationOutputKind multiGenerationOutputKind) const
{
  std::string result;

  size_t passesNumber = 0;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN:
      // should not occur
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      passesNumber = 3;
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
      {
        std::stringstream headPartStream;

        headPartStream <<
R"(What msdlConverter does:

    This multi-pass converter performs various passes depending on the output generated,
    which should be specified by an option)";

        headPart = headPartStream.str ();
      }
      break;

    default:
      {
        std::stringstream headPartStream;

        headPartStream <<
R"(What msdlConverter does:

    This multi-pass converter basically performs )" <<
          passesNumber <<
          " passes when generating " <<
          mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
          " output:" <<
          std::endl <<
R"(
        Pass 1:  converts the MSDL input into a first MSR)";

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
        Pass 2:  converts the first MSR into a second MSR;
        Pass 3:  converts the second MSR into a
                 LilyPond Score Representation (LPSR);
        Pass 4:  converts the LPSR to LilyPond code
                 and writes it to standard output.)";
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      specificPart =
R"(
        Pass 2:  converts the first MSR into a second MSR;
        Pass 3 : converts the second MSR into a
                 Braille Score Representation (BSR)
                 containing one Braille page per MusicXML page;
        Pass 4:  converts the BSRinto another BSR
                 with as many Braille pages as needed
                 to fit the line and page lengthes;
        Pass 5:  converts the BSR to Braille text
                 and writes it to standard output.)

    In this preliminary version, pass 2b merely clones the BSR it receives.)";
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      specificPart =
R"(
        Pass 2:  converts the first MSR into a second MSR;
        Pass 3:  converts the second MSR into an MusicXML tree;
        Pass 4:  converts the MusicXML tree to MusicXML code
                 and writes it to standard output.)";
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      specificPart =
R"(
        Pass 2:  converts the first MSR into a second MSR;
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
void msdlConverterInsiderHandler::createTheMsdlConverterPrefixes ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating the msdlConverter prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msdlConverterInsiderHandler::createTheMsdlConverterOptionGroups (
  const std::string&     serviceName,
  mfMultiGenerationOutputKind mfMultiGenerationOutputKind)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating the \"" <<
      fHandlerHeader <<
      "\" insider option groups" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
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

  initializeMSDL (); // used whatever the output kind

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the MSDR OAH group
  appendGroupToHandler (
    createGlobalMsdlInputOahGroup (
      serviceName));

  // create the msdlConverter OAH group
  appendGroupToHandler (
    createGlobalMsdlConverterOahGroup ());

  // create the msdlConverter insider OAH group
  appendGroupToHandler (
    createGlobalmsdlConverterInsiderOahGroup ());

  // create the msdl2msr OAH group
  appendGroupToHandler (
    createGlobalMsdl2msrOahGroup ());

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2msr OAH group
  appendGroupToHandler (
    createGlobalMsr2msrOahGroup ());

  // create the multi generator OAH group JMI
  appendGroupToHandler (
    createGlobalMultiGenerationOahGroup ());

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

      // create the msdl2lilypond OAH group
      appendGroupToHandler (
        createGlobalMsdl2lyInsiderOahGroup ());

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

      // create the msdl2braille OAH group
      appendGroupToHandler (
        createGlobalMsdl2brlOahGroup (
          serviceName,
          fHandlerHeader));
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      // create the msr2mxsr OAH group
      appendGroupToHandler (
        createGlobalMsr2mxsrOahGroup ());

      // create the MXSR OAH group
      appendGroupToHandler (
        createGlobalMxsrOahGroup ());

      // create the msdl2musicxml OAH group
      appendGroupToHandler (
        createGlobalMsdl2xmlOahGroup ());
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      // create the msr2mxsr OAH group
      appendGroupToHandler (
        createGlobalMsr2mxsrOahGroup ());

      // create the MXSR OAH group
      appendGroupToHandler (
        createGlobalMxsrOahGroup ());

      // create the xml2gmn OAH group
      appendGroupToHandler (
        createGlobalXml2gmnOahGroup ());

      // create the Guido generators OAH group
      appendGroupToHandler (
        createGlobalGuidoGenerationOahGroup ());

      // create the global msdl2gmn OAH group only now,
      // after the groups whose options it may use
      // have been created
      appendGroupToHandler (
        createGlobalMsdl2gmnInsiderOahGroup ());
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif
}

//______________________________________________________________________________
void msdlConverterInsiderHandler::checkOptionsAndArguments () const
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (MSDR_STANDARD_INPUT_NAME == std::string ("-")) {
    checkSingleInputSourceInArgumentsVector ();
  }
  else if (MSDR_STANDARD_INPUT_NAME == std::string ("")) { // JMI v0.9.66
    checkNoOrOneInputSourceInArgumentsVector ();
  }
  else {
    oahInternalError (
      "MSDR_STANDARD_INPUT_NAME should be empty or contain a single '-'");
  }
}

//______________________________________________________________________________
std::string msdlConverterInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Fetching the output file name from the options in OAH handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
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
        getSelected ();

  Bool
    autoOutputFileNameHasBeenSet =
      autoOutputFileNameAtom->
        getSelected ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "msdlConverterInsiderHandler::fetchOutputFileNameFromTheOptions()" <<
      " outputFileNameHasBeenSet: " <<
      outputFileNameHasBeenSet <<
      " autoOutputFileNameHasBeenSet: " <<
      autoOutputFileNameHasBeenSet <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  std::string
    inputSourceName =
      gGlobalCurrentServiceRunData->getInputSourceName ();

  std::string outputFileName;

  if (outputFileNameHasBeenSet) {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has been selected
      // '-aofn, -auto-output-file-name' has been selected
      std::stringstream ss;

      ss <<
        "options' " <<
        outputFileNameStringAtom->fetchNames () <<
        "' and '" <<
        autoOutputFileNameAtom->fetchNames () <<
        "' cannot be selected simultaneously" <<
        "\")";

      oahError (ss.str ());
    }
    else {
      // '-o, -output-file-name' has been selected
      // '-aofn, -auto-output-file-name' has NOT been selected
      outputFileName =
        outputFileNameStringAtom->
          getStringVariable ();
    }
  }

  else {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has NOT been selected
      // '-aofn, -auto-output-file-name' has been selected
      // determine output file base name
      if (inputSourceName == "-") {
        outputFileName = "stdin";
      }

      else {
        // determine output file name,
        outputFileName =
          mfBaseName (inputSourceName);

        size_t
          posInString =
            outputFileName.rfind ('.');

        // remove file extension
        if (posInString != std::string::npos) {
          outputFileName.replace (
            posInString,
            outputFileName.size () - posInString,
            "");
        }
      }

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        std::stringstream ss;

        ss <<
          "msdlConverterInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        std::stringstream ss;

        ss <<
          "msdlConverterInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
          outputFileName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif
    }

    else {
      // '-o, -output-file-name' has NOT been selected
      // '-aofn, -auto-output-file-name' has NOT been selected
      outputFileName = inputSourceName; // will augmented below
    }
  }

  // complement the output file name if relevant,
  // and add the output file name suffix
  if (! outputFileNameHasBeenSet) {
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "outputFileName: " <<
      outputFileName <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  return outputFileName;
}

//______________________________________________________________________________
void msdlConverterInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msdlConverterInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalmsdlConverterInsiderOahGroup->
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
void msdlConverterInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msdlConverterInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "msdlConverterInsiderOahGroup: a MusicXML output file name must be selected with '-o, -output-file-name";

    oahError (ss.str ());
  }

  else if (fOutputFileName == gGlobalCurrentServiceRunData->getInputSourceName ()) {
    std::stringstream ss;

    ss <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (ss.str ());
  }
  */
}

//______________________________________________________________________________
void msdlConverterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msdlConverterInsiderOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msdlConverterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlConverterInsiderOahGroup>*> (v)) {
        S_msdlConverterInsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlConverterInsiderOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msdlConverterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msdlConverterInsiderOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msdlConverterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlConverterInsiderOahGroup>*> (v)) {
        S_msdlConverterInsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlConverterInsiderOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlConverterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msdlConverterInsiderOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msdlConverterInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msdlConverterInsiderHandler:" <<
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

std::ostream& operator << (std::ostream& os, const S_msdlConverterInsiderHandler& elt)
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
S_msdlConverterInsiderOahGroup gGlobalmsdlConverterInsiderOahGroup;

S_msdlConverterInsiderOahGroup msdlConverterInsiderOahGroup::create ()
{
  msdlConverterInsiderOahGroup* o = new msdlConverterInsiderOahGroup ();
  assert (o != nullptr);
  return o;
}

msdlConverterInsiderOahGroup::msdlConverterInsiderOahGroup ()
  : oahGroup (
    "msdl-converter",
    "help-msdl-converter", "hmc",
R"(Options that are used by msdlConverter are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  fMultiGenerationOutputKind = mfMultiGenerationOutputKind::kGeneration_UNKNOWN;

  initializemsdlConverterInsiderOahGroup ();
}

msdlConverterInsiderOahGroup::~msdlConverterInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msdlConverterInsiderOahGroup::initializemsdlConverterInsiderOahGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    std::stringstream ss;

    ss <<
      "Initializing \"" <<
      fGroupHeader <<
      "\" group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // JMI ???
}

//______________________________________________________________________________
void msdlConverterInsiderOahGroup::printmsdlConverterInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The msdlConverter options are:" <<
    std::endl;

  ++gIndenter;

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
S_msdlConverterInsiderOahGroup createGlobalmsdlConverterInsiderOahGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating global msdlConverter insider OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalmsdlConverterInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalmsdlConverterInsiderOahGroup =
      msdlConverterInsiderOahGroup::create ();
    assert (gGlobalmsdlConverterInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalmsdlConverterInsiderOahGroup;
}


}
