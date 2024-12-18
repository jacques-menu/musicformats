/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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

#include "mfInitialization.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrInitialization.h"

#include "bsrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "mfOutputFileOah.h"
#include "oahDisplayOah.h"
#include "oahHarmoniesExtraOah.h"

#include "msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "oahEarlyOptions.h"

#include "msr2brailleComponent.h"

#include "msr2brailleInsiderHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msr2brailleInsiderHandler msr2brailleInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  msr2brailleInsiderHandler* obj = new
    msr2brailleInsiderHandler (
      serviceName,
      handlerHeader);
  assert (obj!=0);

  return obj;
}

msr2brailleInsiderHandler::msr2brailleInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
          Welcome to the MSR to Braille converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
R"(
Usage: msr2braille [option]* [MusicXMLFile|-] [[option]*
)")
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing msr2braille insider options handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the msr2braille prefixes
  createTheXml2braillePrefixes ();

  // create the msr2braille option groups
  createTheXml2brailleOptionGroups (serviceName);
}

msr2brailleInsiderHandler::~msr2brailleInsiderHandler ()
{}

void msr2brailleInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMsr2brailleConverterComponent ();
}

std::string msr2brailleInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What msr2braille does:

    This multi-pass converter basically performs 6 passes:
        Pass 1: reads the contents of MusicXMLFile or stdin ('-')
                and converts it to a MusicXML tree;
        Pass 2: converts that MusicXML tree into
                a Music Score Representation (MSR) skeleton;
        Pass 3: populates the MSR skeleton from the MusicXML tree
                to get a full MSR;
        Pass 4: converts the MSR into a
                Braille Score Representation (BSR)
                containing one Braille page per MusicXML page;
        Pass 5: converts the BSRinto another BSR
                with as many Braille pages as needed
                to fit the line and page lengthes;
        Pass 6: converts the BSR to Unicode text
                and writes it to standard output.

    In this preliminary version, pass 4 merely clones the BSR it receives.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void msr2brailleInsiderHandler::createTheXml2braillePrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the msr2braille prefixes";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msr2brailleInsiderHandler::createTheXml2brailleOptionGroups (
  const std::string& serviceName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the msr2braille insider option groups" <<
      fHandlerHeader <<
      "\"";

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

  // create the output file OAH group
  appendGroupToHandler (
    createGlobalOutputFileOahGroup ());

  // create the display OAH group
  appendGroupToHandler (
    createGlobalDisplayOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();
  initializeBSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the msr2braille OAH group
  appendGroupToHandler (
    createGlobalMrailleGenerationOahGroup (
      serviceName,
      fHandlerHeader));

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2bsr OAH group
  appendGroupToHandler (
    createGlobalMsr2bsrOahGroup ());

  // create the BSR OAH group
  appendGroupToHandler (
    createGlobalBsrOahGroup ());

  // create the braille generation OAH group
  appendGroupToHandler (
    createGlobalBrailleGenerationOahGroup ());

  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    // print the options handler initial state
    std::stringstream ss;

    ss <<
      "msr2brailleInsiderHandler has been initialized as:" <<
      std::endl;

    ++gIndenter;

    print (gLog);

    gLog <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msr2brailleInsiderHandler::checkOptionsAndArguments () const
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
std::string msr2brailleInsiderHandler::fetchOutputFileNameFromTheOptions () const
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
      std::string
        inputSourceName =
          gServiceRunData->getInputSourceName ();

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

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        std::stringstream ss;

        ss <<
          "msr2brailleInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1: \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        std::stringstream ss;

        ss <<
          "msr2brailleInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2: \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append the file extension to the output file name
      switch (brailleOutputKind) {
        case bsrBrailleOutputKind::kBrailleOutputAscii:
         outputFileName += ".brf";
          break;

        case bsrBrailleOutputKind::kBrailleOutputUTF8:
        case bsrBrailleOutputKind::kBrailleOutputUTF16:
          outputFileName += ".brf";
          break;

        case bsrBrailleOutputKind::kBrailleOutputUTF8Debug:
          outputFileName += ".brf"; // since braille cells and regular text are present
          break;
      } // switch

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        std::stringstream ss;

        ss <<
          "msr2brailleInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 3: \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }

    else {
      // '-o, -output-file-name' has NOT been selected
      // '-aofn, -auto-output-file-name' has NOT been selected
      // nothing to do
    }
  }

  return outputFileName;
}

//______________________________________________________________________________
void msr2brailleInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2brailleInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalMsr2brailleInsiderOahGroup->
    enforceGroupQuietness ();

  gMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2bsrOahGroup->
    enforceGroupQuietness ();

  gBsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBrailleGenerationOahGroup->
    enforceGroupQuietness ();

  gHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void msr2brailleInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2brailleInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2brailleInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2brailleInsiderOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2brailleInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2brailleInsiderOahGroup>*> (v)) {
        S_msr2brailleInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2brailleInsiderOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msr2brailleInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2brailleInsiderOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2brailleInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2brailleInsiderOahGroup>*> (v)) {
        S_msr2brailleInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2brailleInsiderOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msr2brailleInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2brailleInsiderOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msr2brailleInsiderHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "msr2brailleInsiderHandler:" <<
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

std::ostream& operator << (std::ostream& os, const S_msr2brailleInsiderHandler& elt)
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
S_msr2brailleInsiderOahGroup gGlobalMsr2brailleInsiderOahGroup;

S_msr2brailleInsiderOahGroup msr2brailleInsiderOahGroup::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  msr2brailleInsiderOahGroup* obj = new msr2brailleInsiderOahGroup (
    serviceName,
    handlerHeader);
  assert (obj!=0);

  return obj;
}

msr2brailleInsiderOahGroup::msr2brailleInsiderOahGroup (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahGroup (
      serviceName,
      "hx2b", "help-msr2braille",
R"(Options that are used by msr2braille are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2brailleInsiderOahGroup ();
}

msr2brailleInsiderOahGroup::~msr2brailleInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msr2brailleInsiderOahGroup::initializeXml2brailleInsiderOahGroup ()
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
}

//______________________________________________________________________________
void msr2brailleInsiderOahGroup::printXml2brailleInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The msr2braille options are:" <<
    std::endl;

  ++gIndenter;

  // quit after some passes
  // --------------------------------------

  gLog <<
    "Quit after some passes:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fQuitAfterPass2" << ": " <<
    fQuitAfterPass2 <<
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass3" << ": " <<
    fQuitAfterPass3 <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_msr2brailleInsiderOahGroup createGlobalMrailleGenerationOahGroup (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msr2braille OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMsr2brailleInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsr2brailleInsiderOahGroup =
      msr2brailleInsiderOahGroup::create (
        serviceName,
        handlerHeader);
    assert (gGlobalMsr2brailleInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2brailleInsiderOahGroup;
}


}

