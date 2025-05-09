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

// WAE
#include "oahWae.h"

#include "mfPreprocessorSettings.h"

#include "mfInitialization.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrInitialization.h"

#include "lpsrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "mfOutputFileOah.h"
#include "oahDisplayOah.h"
#include "oahHarmoniesExtraOah.h"

#include "mxsr2msrOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2lilypondComponent.h"

#include "musicxml2lilypondInsiderHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_xml2lyInsiderHandler xml2lyInsiderHandler::create (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  oahHandlerUsedThruKind handlerUsedThruKind)
{
  // create the insider handler
  xml2lyInsiderHandler* obj = new
    xml2lyInsiderHandler (
      serviceName,
      handlerHeader,
      handlerUsedThruKind);
  assert (obj!=0);

  return obj;
}

xml2lyInsiderHandler::xml2lyInsiderHandler (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  oahHandlerUsedThruKind handlerUsedThruKind)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
          Welcome to the MusicXML to LilyPond converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
/* JMI
R"(
Usage: xml2ly [[option]* [MusicXMLFile|-] [[option]*
)"
*/
      "" // temporary, will be set below
      )
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing xml2ly insider options handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // initialize the history
  initializeHandlerMultiComponent ();

  // we can set the handler usage field thru a method now,
  // since the oahInsiderHandler constructor has been executed
  fHandlerUsage =
    usageFromUsedThruKind (handlerUsedThruKind);

  // create the xml2ly prefixes
  createTheXml2lyPrefixes ();

  // create the xml2xml option groups
  createTheXml2lyOptionGroups (
    serviceName);
}

xml2lyInsiderHandler::~xml2lyInsiderHandler ()
{}

void xml2lyInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMusicxml2lilypondConverterComponent ();
}

//_______________________________________________________________________________
std::string xml2lyInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What xml2ly does:

    This multi-pass converter basically performs 5 passes:
        Pass 1: reads the contents of MusicXMLFile or stdin ('-')
                and converts it to a MusicXML tree;
        Pass 2: converts that MusicXML tree into
                a first Music Score Representation (MSR) skeleton;
        Pass 3: populates the first MSR skeleton from the MusicXML tree
                to get a full MSR;
        Pass 4: converts the first MSR into a second MSR to apply options
        Pass 5: converts the second MSR into a
                LilyPond Score Representation (LPSR);
        Pass 6: converts the LPSR to LilyPond code
                and writes it to standard output.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
std::string xml2lyInsiderHandler::usageFromUsedThruKind (
   oahHandlerUsedThruKind handlerUsedThruKind) const
{
  std::string result;

  switch (handlerUsedThruKind) {
    case oahHandlerUsedThruKind::kHandlerUsedThruUnknown:
      {
        std::stringstream ss;

        ss <<
          "oahHandlerUsedThruKind::kHandlerUsedThruUnknown found in usageFromUsedThruKind() in handler \"" <<
          fHandlerHeader <<
          "\"";

        oahInternalError (ss.str ());
      }
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv:
      {
      result =
        fHandlerServiceName
          +
        " [[option]* [MusicXMLFile|-] [[option]*";
      }
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      result = "[options]";
      break;
  } // switch

  result = "Usage: " + result;

  return result;
}

//______________________________________________________________________________
void xml2lyInsiderHandler::createTheXml2lyPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Creating the xml2ly prefixes";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void xml2lyInsiderHandler::createTheXml2lyOptionGroups (
  const std::string& serviceName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Creating the xml2ly insider option groups" <<
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
  initializeLPSR ();

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

  // create the msr2lpsr OAH group
  appendGroupToHandler (
    createGlobalMsr2lpsrOahGroup ());

  // create the LPSR OAH group
  appendGroupToHandler (
    createGlobalLpsrOahGroup ());

  // create the LilyPond generation OAH group
  appendGroupToHandler (
    createGlobalLpsr2lilypondOahGroup ());

  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());

  // create the global xml2ly OAH group only now,
  // after the groups whose options it may use
  // have been created
  appendGroupToHandler (
    createGlobalXml2lyInsiderOahGroup ());

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    // print the options handler initial state
    gLog <<
      "musicxml2lilypondInsiderInsiderHandler has been initialized as:" <<
      std::endl;

    ++gIndenter;

    print (gLog);

    gLog <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    gLog <<
      "musicxml2lilypondInsiderInsiderHandler help:" <<
      std::endl;

    this->printHelp (gOutput);
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void xml2lyInsiderHandler::checkOptionsAndArguments () const
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
std::string xml2lyInsiderHandler::fetchOutputFileNameFromTheOptions () const
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
          "musicxml2lilypondInsiderInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1: \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append the file extension to the output file name
       outputFileName += ".ly";

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
        std::stringstream ss;

        ss <<
          "musicxml2lilypondInsiderInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2: " <<
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
void xml2lyInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2lyInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalXml2lyInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsr2msrOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsr2msrOahGroup->
    enforceGroupQuietness ();

  gMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2lpsrOahGroup->
    enforceGroupQuietness ();

  gLpsrOahGroup->
    enforceGroupQuietness ();

  gGlobalLpsr2lilypondOahGroup->
    enforceGroupQuietness ();

  gHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void xml2lyInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void xml2lyInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2lyInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2lyInsiderOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_xml2lyInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2lyInsiderOahGroup>*> (v)) {
        S_xml2lyInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2lyInsiderOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void xml2lyInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2lyInsiderOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_xml2lyInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2lyInsiderOahGroup>*> (v)) {
        S_xml2lyInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2lyInsiderOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void xml2lyInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2lyInsiderOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

void xml2lyInsiderHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "musicxml2lilypondInsiderInsiderHandler '" << fHandlerHeader << "':" <<
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

std::ostream& operator << (std::ostream& os, const S_xml2lyInsiderHandler& elt)
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
S_xml2lyInsiderOahGroup gGlobalXml2lyInsiderOahGroup;

S_xml2lyInsiderOahGroup xml2lyInsiderOahGroup::create ()
{
  xml2lyInsiderOahGroup* obj = new xml2lyInsiderOahGroup ();
  assert (obj!=0);

  return obj;
}

xml2lyInsiderOahGroup::xml2lyInsiderOahGroup ()
  : oahGroup (
      "xml2ly",
      "help-xml2ly", "hx2l",
R"(Options that are used by xml2ly are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2lyInsiderOahGroup ();
}

xml2lyInsiderOahGroup::~xml2lyInsiderOahGroup ()
{}

//_______________________________________________________________________________
void xml2lyInsiderOahGroup::initializeXml2lyInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
  gLog << std::left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();

  // macros
  // --------------------------------------

  createInsiderMacrosSubGroup ();
}

//_______________________________________________________________________________
void xml2lyInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
  gLog << std::left <<
    "Creating insider quit subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "help-xml2ly-quit", "hxml2lyquit",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // quit after pass 2

  S_oahBooleanAtom
    quit2OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-2", "qap2",
R"(Quit after pass 2, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
        "fQuitAfterPass2",
        fQuitAfterPass2);

  subGroup->
    appendAtomToSubGroup (
      quit2OahBooleanAtom);

  // quit after pass 3

  S_oahBooleanAtom
    quit3OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-3", "qap3",
R"(Quit after pass 3, i.e. after conversion
of the MusicXML tree to MSR.)",
        "fQuitAfterPass3",
        fQuitAfterPass3);

  subGroup->
    appendAtomToSubGroup (
      quit3OahBooleanAtom);

  // quit after pass 4

  S_oahBooleanAtom
    quit4OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-4", "qap4",
R"(Quit after pass 4, i.e. after conversion
of the first MSR to a second MSR.)",
        "fQuitAfterPass4",
        fQuitAfterPass4);

  subGroup->
    appendAtomToSubGroup (
      quit4OahBooleanAtom);

  // quit after pass 5

  S_oahBooleanAtom
    quit5OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-5", "qap5",
R"(Quit after pass 5, i.e. after conversion
of the second MSR to LPSR.)",
        "fQuitAfterPass5",
        fQuitAfterPass5);

  subGroup->
    appendAtomToSubGroup (
      quit5OahBooleanAtom);
}

//_______________________________________________________________________________
void xml2lyInsiderOahGroup::createInsiderMacrosSubGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
  gLog << std::left <<
    "Creating insider macro subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "xml2ly macros",
        "help-xml2ly-macros", "hxml2lymacros",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // create the debug macro

  S_oahMacroAtom
    debugMacroAtom =
      oahMacroAtom::create (
        "debug", "",
        "To help debugging musicxml2lilypond.");

  subGroup->
    appendAtomToSubGroup (
      debugMacroAtom);

  // populate it
#ifdef MF_TRACE_IS_ENABLED
  debugMacroAtom->
    appendValueLessAtomToMacro (
      gTraceOahGroup->
        getTracePassesBooleanAtom ());
#endif // MF_TRACE_IS_ENABLED

  debugMacroAtom->
    appendValueLessAtomToMacro (
      gGlobalOutputFileOahGroup->
        getAutoOutputFileNameAtom ());

  debugMacroAtom->
    appendValueLessAtomToMacro (
      gGlobalDisplayOahGroup->
        getDisplayCPUusageAtom ());
}

//______________________________________________________________________________
void xml2lyInsiderOahGroup::printXml2lyInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The xml2ly options are:" <<
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
    std::setw (fieldWidth) << "fQuitAfterPass4" << ": " <<
    fQuitAfterPass4 <<
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass5" << ": " <<
    fQuitAfterPass5 <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_xml2lyInsiderOahGroup createGlobalXml2lyInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global xml2ly OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalXml2lyInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalXml2lyInsiderOahGroup =
      xml2lyInsiderOahGroup::create ();
    assert (gGlobalXml2lyInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2lyInsiderOahGroup;
}


}
