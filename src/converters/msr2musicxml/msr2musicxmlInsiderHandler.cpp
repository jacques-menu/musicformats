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

#include "oahEarlyOptions.h"

#include "msr2musicxmlComponent.h"

#include "msr2musicxmlManPageOah.h"

#include "msr2musicxmlInsiderHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msr2musicxmlInsiderHandler msr2musicxmlInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  msr2musicxmlInsiderHandler* o = new
    msr2musicxmlInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o!=0);

  return o;
}

msr2musicxmlInsiderHandler::msr2musicxmlInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
            Welcome to the MSR to MusicXML converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
R"(
Usage: msr2musicxml [[option]* [MusicXMLFile|-] [[option]*
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

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the msr2musicxml prefixes
  createTheMsr2musicxmlPrefixes ();

  // create the msr2musicxml option groups
  createTheMsr2musicxmlOptionGroups (serviceName);
}

msr2musicxmlInsiderHandler::~msr2musicxmlInsiderHandler ()
{}

void msr2musicxmlInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMsr2musicxmlConverterComponent ();
}

std::string msr2musicxmlInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What msr2musicxml does:

    This multi-pass converter basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML tree into
                 a first Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the first MSR into a second MSR, to apply options;
        Pass 4:  converts the second MSR into a second MusicXML tree;
        Pass 5:  converts the second MusicXML tree to Guido code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void msr2musicxmlInsiderHandler::createTheMsr2musicxmlPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the msr2musicxml prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msr2musicxmlInsiderHandler::createTheMsr2musicxmlOptionGroups (
  const std::string& serviceName)
{
#ifdef TRACING_IS_ENABLED
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

  // create the msr2musicxml OAH group
  appendGroupToHandler (
    createGlobalMsr2musicxmlOahGroup ());

  // create the MXSR OAH group
  appendGroupToHandler (
    createGlobalMxsrOahGroup ());

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
}

//______________________________________________________________________________
void msr2musicxmlInsiderHandler::checkOptionsAndArguments () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
std::string msr2musicxmlInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef TRACING_IS_ENABLED
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
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream <<
          "msr2musicxmlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif

      // append the file extension to the output file name
       outputFileName += ".xml";

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream <<
          "msr2musicxmlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
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
void msr2musicxmlInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2musicxmlInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsr2musicxmlInsiderOahGroup->
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
void msr2musicxmlInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2musicxmlInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "msr2musicxmlInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

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
void msr2musicxmlInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlInsiderOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2musicxmlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2musicxmlInsiderOahGroup>*> (v)) {
        S_msr2musicxmlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2musicxmlInsiderOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2musicxmlInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlInsiderOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2musicxmlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2musicxmlInsiderOahGroup>*> (v)) {
        S_msr2musicxmlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2musicxmlInsiderOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2musicxmlInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlInsiderOahGroup::browseData ()" <<
      std::endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msr2musicxmlInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msr2musicxmlInsiderHandler:" <<
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

std::ostream& operator << (std::ostream& os, const S_msr2musicxmlInsiderHandler& elt)
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
S_msr2musicxmlInsiderOahGroup gGlobalMsr2musicxmlInsiderOahGroup;

S_msr2musicxmlInsiderOahGroup msr2musicxmlInsiderOahGroup::create ()
{
  msr2musicxmlInsiderOahGroup* o = new msr2musicxmlInsiderOahGroup ();
  assert (o!=0);

  return o;
}

msr2musicxmlInsiderOahGroup::msr2musicxmlInsiderOahGroup ()
  : oahGroup (
    "msr2musicxml",
    "hm2m", "help-msr2musicxml",
R"(Options that are used by msr2musicxml are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2musicxmlInsiderOahGroup ();
}

msr2musicxmlInsiderOahGroup::~msr2musicxmlInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msr2musicxmlInsiderOahGroup::initializeMsr2musicxmlInsiderOahGroup ()
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
}

//______________________________________________________________________________
void msr2musicxmlInsiderOahGroup::printMsr2musicxmlInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The msr2musicxml options are:" <<
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
S_msr2musicxmlInsiderOahGroup createGlobalMsr2musicxmlOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2musicxml OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2musicxmlInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsr2musicxmlInsiderOahGroup =
      msr2musicxmlInsiderOahGroup::create ();
    assert (gGlobalMsr2musicxmlInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2musicxmlInsiderOahGroup;
}


}
