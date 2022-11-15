/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "oahWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "enableHarmoniesExtraOahIfDesired.h"
#ifdef EXTRA_OAH_IS_ENABLED
  #include "harmoniesExtraOah.h"
#endif

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "outputFileOah.h"
#include "displayOah.h"

#include "mxsrOah.h"
#include "msr2mxsrOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "mxsr2msrOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2musicxmlComponent.h"

#include "musicxml2musicxmlManPageOah.h"

#include "musicxml2musicxmlInsiderHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_xml2xmlInsiderHandler xml2xmlInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  xml2xmlInsiderHandler* o = new
    xml2xmlInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o!=0);

  return o;
}

xml2xmlInsiderHandler::xml2xmlInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
          Welcome to the MusicXML to MusicXML converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
R"(
Usage: xml2xml [[option]* [MusicXMLFile|-] [[option]*
)")
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler" <<
      std::endl;
  }
#endif

  // initialize the multi-component
  initializeHandlerMultiComponent ();

  // create the xml2xml prefixes
  createTheXml2xmlPrefixes ();

  // create the xml2xml option groups
  createTheXml2xmlOptionGroups (serviceName);
}

xml2xmlInsiderHandler::~xml2xmlInsiderHandler ()
{}

void xml2xmlInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMusicxml2musicxmlConverterComponent ();
}

std::string xml2xmlInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What xml2xml does:

    This multi-pass converter basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML tree into
                 a first Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the first MSR into a second MSR, to apply options;
        Pass 4:  converts the second MSR into a second MusicXML tree;
        Pass 5:  converts the second MusicXML tree to MusicXML code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::createTheXml2xmlPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Creating the xml2xml prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::createTheXml2xmlOptionGroups (
  const std::string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
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

#ifdef TRACING_IS_ENABLED
  // create the tracing OAH group
  appendGroupToHandler (
    createGlobalTracingOahGroup (
      this));
#endif

  // create the output file OAH group
  appendGroupToHandler (
    createGlobalOutputFileOahGroup ());

  // create the display OAH group
  appendGroupToHandler (
    createGlobalDisplayOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the MXSR OAH group
  appendGroupToHandler (
    createGlobalMxsrOahGroup ());

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

  // create the msr2mxsr OAH group
  appendGroupToHandler (
    createGlobalMsr2mxsrOahGroup ());

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif

  // create the global xml2xml OAH group only now,
  // after the groups whose options it may use
  // have been created
  appendGroupToHandler (
    createGlobalXml2xmlOahGroup ());
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::checkOptionsAndArguments () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
std::string xml2xmlInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
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

  std::string outputFileName;

  if (outputFileNameHasBeenSet) {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has been chosen
      // '-aofn, -auto-output-file-name' has been chosen
      std::stringstream s;

      s <<
        "options' " <<
        outputFileNameStringAtom->fetchNames () <<
        "' and '" <<
        autoOutputFileNameAtom->fetchNames () <<
        "' cannot be chosen simultaneously" <<
        "\")";

      oahError (s.str ());
    }
    else {
      // '-o, -output-file-name' has been chosen
      // '-aofn, -auto-output-file-name' has NOT been chosen
      outputFileName =
        outputFileNameStringAtom->
          getStringVariable ();
    }
  }

  else {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has NOT been chosen
      // '-aofn, -auto-output-file-name' has been chosen
      std::string
        inputSourceName =
          gGlobalServiceRunData->getInputSourceName ();

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

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "xml2xmlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif

      // append the file extension to the output file name
       outputFileName += ".xml";

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "xml2xmlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif
    }

    else {
      // '-o, -output-file-name' has NOT been chosen
      // '-aofn, -auto-output-file-name' has NOT been chosen
      // nothing to do
    }
  }

  return outputFileName;
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2xmlInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalXml2xmlInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsr2msrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2mxsrOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void xml2xmlInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void xml2xmlInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "xml2xmlInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

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
void xml2xmlInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlInsiderOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_xml2xmlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2xmlInsiderOahGroup>*> (v)) {
        S_xml2xmlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2xmlInsiderOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2xmlInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlInsiderOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_xml2xmlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2xmlInsiderOahGroup>*> (v)) {
        S_xml2xmlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2xmlInsiderOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2xmlInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2xmlInsiderOahGroup::browseData ()" <<
      std::endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2xmlInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2xmlInsiderHandler:" <<
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

std::ostream& operator << (std::ostream& os, const S_xml2xmlInsiderHandler& elt)
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
S_xml2xmlInsiderOahGroup gGlobalXml2xmlInsiderOahGroup;

S_xml2xmlInsiderOahGroup xml2xmlInsiderOahGroup::create ()
{
  xml2xmlInsiderOahGroup* o = new xml2xmlInsiderOahGroup ();
  assert (o!=0);

  return o;
}

xml2xmlInsiderOahGroup::xml2xmlInsiderOahGroup ()
  : oahGroup (
    "xml2xml",
   "help-xml2xml", "hm2x",
R"(Options that are used by xml2xml are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2xmlInsiderOahGroup ();
}

xml2xmlInsiderOahGroup::~xml2xmlInsiderOahGroup ()
{}

//_______________________________________________________________________________
void xml2xmlInsiderOahGroup::initializeXml2xmlInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  gLogStream << std::left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    std::endl;
  }
#endif

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

//_______________________________________________________________________________
void xml2xmlInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
  gLogStream << std::left <<
    "Creating insider quit subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    std::endl;
  }
#endif

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "help-msr2xml-quit", "hm2xquit",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // quit after pass 2a

  S_oahBooleanAtom
    quit2aOahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-2a", "qap2a",
R"(Quit after pass 2a, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
        "fQuitAfterPass2a",
        fQuitAfterPass2a);

  subGroup->
    appendAtomToSubGroup (
      quit2aOahBooleanAtom);

  // quit after pass 2b

  S_oahBooleanAtom
    quit2bOahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-2b", "qap2b",
R"(Quit after pass 2b, i.e. after conversion
of the MusicXML tree to MSR.)",
        "fQuitAfterPass2b",
        fQuitAfterPass2b);

  subGroup->
    appendAtomToSubGroup (
      quit2bOahBooleanAtom);
}

//______________________________________________________________________________
void xml2xmlInsiderOahGroup::printXml2xmlInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The xml2xml options are:" <<
    std::endl;

  ++gIndenter;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fQuitAfterPass2a" << " : " <<
    fQuitAfterPass2a <<
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass2b" << " : " <<
    fQuitAfterPass2b <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_xml2xmlInsiderOahGroup createGlobalXml2xmlOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global xml2xml OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2xmlInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalXml2xmlInsiderOahGroup =
      xml2xmlInsiderOahGroup::create ();
    assert (gGlobalXml2xmlInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2xmlInsiderOahGroup;
}


}
