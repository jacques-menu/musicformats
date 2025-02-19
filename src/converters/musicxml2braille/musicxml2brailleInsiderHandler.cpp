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

#include "mxsr2msrOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2brailleComponent.h"

#include "musicxml2brailleInsiderHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_xml2brlInsiderHandler xml2brlInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  xml2brlInsiderHandler* obj = new
    xml2brlInsiderHandler (
      serviceName,
      handlerHeader);
  assert (obj!=0);

  return obj;
}

xml2brlInsiderHandler::xml2brlInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
        Welcome to the MusicXML to Braille converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
R"(
Usage: xml2brl [[option]* [MusicXMLFile|-] [[option]*
)")
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Initializing xml2brl insider options handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the xml2brl prefixes
  createTheXml2brlPrefixes ();

  // create the xml2brl option groups
  createTheXml2brlOptionGroups (serviceName);
}

xml2brlInsiderHandler::~xml2brlInsiderHandler ()
{}

void xml2brlInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMusicxml2brailleConverterComponent ();
}

//_______________________________________________________________________________
std::string xml2brlInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What xml2brl does:

    This multi-pass converter basically performs 6 passes:
        Pass 1: reads the contents of MusicXMLFile or stdin ('-')
                and converts it to a MusicXML tree;
        Pass 2: converts that MusicXML tree into
                a first Music Score Representation (MSR) skeleton;
        Pass 3: populates the MSR skeleton from the MusicXML tree
                to get a full MSR;
        Pass 4: converts the first MSR into a second MSR, to apply options
        Pass 5: converts the second MSR into
                a first Braille Score Representation (BSR)
                containing one Braille page per MusicXML page;
        Pass 6: converts the first BSR into a second BSR
                with as many Braille pages as needed
                to fit the line and page lengths;
        Pass 7: converts the BSR to Braille text
                and writes it to standard output.

    In this preliminary version, pass 3 merely clones the MSR it receives.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void xml2brlInsiderHandler::createTheXml2brlPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Creating the xml2brl prefixes";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void xml2brlInsiderHandler::createTheXml2brlOptionGroups (
  const std::string& serviceName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Creating the xml2brl insider option groups" <<
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

  // create the mxsr2msr OAH group
  appendGroupToHandler (
    createGlobalMxsr2msrOahGroup (
      this));

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2msr OAH group
  appendGroupToHandler (
    createGlobalMsr2msrOahGroup ());

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

  // create the global xml2brl OAH group only now,
  // after the groups whose options it may use
  // have been created
  appendGroupToHandler (
    createGlobalXml2brlOahGroup (
      serviceName,
      fHandlerHeader));

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    // print the options handler initial state
    std::stringstream ss;

    ss <<
      "xml2brlInsiderHandler has been initialized as:" <<
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
void xml2brlInsiderHandler::checkOptionsAndArguments () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Checking options and arguments from argc/argv in \"" <<
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
std::string xml2brlInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
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
      if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
        std::stringstream ss;

        ss <<
          "xml2brlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1: \"" <<
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
      if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
        std::stringstream ss;

        ss <<
          "xml2brlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2: \"" <<
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
      if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
        std::stringstream ss;

        ss <<
          "xml2brlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 3: \"" <<
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
void xml2brlInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2brlInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalXml2brlInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsr2msrOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsr2msrOahGroup->
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
void xml2brlInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void xml2brlInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2brlInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2brlInsiderOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_xml2brlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlInsiderOahGroup>*> (v)) {
        S_xml2brlInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2brlInsiderOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void xml2brlInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2brlInsiderOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_xml2brlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlInsiderOahGroup>*> (v)) {
        S_xml2brlInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2brlInsiderOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void xml2brlInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2brlInsiderOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2brlInsiderHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "xml2brlInsiderHandler:" <<
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

std::ostream& operator << (std::ostream& os, const S_xml2brlInsiderHandler& elt)
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
S_xml2brlInsiderOahGroup gGlobalXml2brlInsiderOahGroup;

S_xml2brlInsiderOahGroup xml2brlInsiderOahGroup::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  xml2brlInsiderOahGroup* obj = new xml2brlInsiderOahGroup (
    serviceName,
    handlerHeader);
  assert (obj!=0);

  return obj;
}

xml2brlInsiderOahGroup::xml2brlInsiderOahGroup (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahGroup (
      serviceName,
      "help-xml2brl", "hx2b",
R"(Options that are used by xml2brl are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2brlInsiderOahGroup ();
}

xml2brlInsiderOahGroup::~xml2brlInsiderOahGroup ()
{}

//_______________________________________________________________________________
void xml2brlInsiderOahGroup::initializeXml2brlInsiderOahGroup ()
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

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

//_______________________________________________________________________________
void xml2brlInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Creating insider quit subgroup in \"" <<
      fGroupHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "help-msr2brl-quit", "hm2bquit",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // quit after pass 2

  S_oahBooleanAtom
    quit2aOahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-2", "qap2",
R"(Quit after pass 2, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
        "fQuitAfterPass2",
        fQuitAfterPass2);

  subGroup->
    appendAtomToSubGroup (
      quit2aOahBooleanAtom);

  // quit after pass 3

  S_oahBooleanAtom
    quit2bOahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-3", "qap3",
R"(Quit after pass 3, i.e. after conversion
of the MusicXML tree to MSR.)",
        "fQuitAfterPass3",
        fQuitAfterPass3);

  subGroup->
    appendAtomToSubGroup (
      quit2bOahBooleanAtom);

  // quit after pass 4

  S_oahBooleanAtom
    quit3OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-4", "qap4",
R"(Quit after pass 4, i.e. after conversion
of the first MSR to a second MSR.)",
        "fQuitAfterPass4",
        fQuitAfterPass4);

  subGroup->
    appendAtomToSubGroup (
      quit3OahBooleanAtom);
}

//______________________________________________________________________________
void xml2brlInsiderOahGroup::printXml2brlInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The xml2brl options are:" <<
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
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass3" << ": " <<
    fQuitAfterPass3 <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_xml2brlInsiderOahGroup createGlobalXml2brlOahGroup (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global xml2brl OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalXml2brlInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalXml2brlInsiderOahGroup =
      xml2brlInsiderOahGroup::create (
        serviceName,
        handlerHeader);
    assert (gGlobalXml2brlInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2brlInsiderOahGroup;
}


}

