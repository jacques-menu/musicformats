/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "enableHarmoniesExtraOahIfDesired.h"

#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msr.h"

#include "oahOah.h"
#include "waeOah.h"
#include "outputFileOah.h"
#include "displayOah.h"

#include "mxsrOah.h"
#include "mxsrGenerationOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "mxsrGenerationOah.h"
#include "mxsr2msrOah.h"

#include "oahEarlyOptions.h"

#include "msr2guidoComponent.h"

#include "msr2guidoManPageOah.h"

#include "msr2guidoInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msr2guidoInsiderHandler msr2guidoInsiderHandler::create (
  const string& serviceName,
  const string& handlerHeader)
{
  // create the insider handler
  msr2guidoInsiderHandler* o = new
    msr2guidoInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o!=0);

  return o;
}

msr2guidoInsiderHandler::msr2guidoInsiderHandler (
  const string& serviceName,
  const string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
              Welcome to the MSR to Guido converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
R"(
Usage: msr2guido [[option]* [MusicXMLFile|-] [[option]*
)")
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler" <<
      endl;
  }
#endif

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the msr2guido prefixes
  createTheMsr2guidoPrefixes ();

  // create the msr2guido option groups
  createTheMsr2guidoOptionGroups (serviceName);
}

msr2guidoInsiderHandler::~msr2guidoInsiderHandler ()
{}

void msr2guidoInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMsr2guidoConverterComponent ();
}

string msr2guidoInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What msr2guido does:

    This multi-pass converter basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a first MusicXML tree;
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
void msr2guidoInsiderHandler::createTheMsr2guidoPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the msr2guido prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msr2guidoInsiderHandler::createTheMsr2guidoOptionGroups (
  const string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the \"" <<
      fHandlerHeader <<
      "\" insider option groups" <<
      endl;
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

  // create the msr2guido OAH group
  appendGroupToHandler (
    createGlobalMsr2guidoOahGroup ());

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
    createGlobalMxsrGenerationOahGroup ());

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif
}

//______________________________________________________________________________
void msr2guidoInsiderHandler::checkOptionsAndArguments () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
string msr2guidoInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Fetching the output file name from the options in OAH handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
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
        getVariableHasBeenSet ();

  Bool
    autoOutputFileNameHasBeenSet =
      autoOutputFileNameAtom->
        getVariableHasBeenSet ();

  string outputFileName;

  if (outputFileNameHasBeenSet) {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has been chosen
      // '-aofn, -auto-output-file-name' has been chosen
      stringstream s;

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
      string
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
        if (posInString != string::npos) {
          outputFileName.replace (
            posInString,
            outputFileName.size () - posInString,
            "");
        }
      }

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream <<
          "msr2guidoInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

      // append the file extension to the output file name
       outputFileName += ".gmn";

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream <<
          "msr2guidoInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
          outputFileName <<
          "\"" <<
          endl;
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
void msr2guidoInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2guidoInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsr2guidoInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsrGenerationOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsr2msrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsrGenerationOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void msr2guidoInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2guidoInsiderOahGroup::checkGroupOptionsConsistency ()
{
/* JMI

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) {
    stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (s.str ());
  }




  if (! fOutputFileName.size ()) {
    stringstream s;

    s <<
      "msr2guidoInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

    oahError (s.str ());
  }

  else if (fOutputFileName == gGlobalServiceRunData->getInputSourceName ()) {
    stringstream s;

    s <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (s.str ());
  }
  */
}

//______________________________________________________________________________
void msr2guidoInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2guidoInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2guidoInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2guidoInsiderOahGroup>*> (v)) {
        S_msr2guidoInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2guidoInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2guidoInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2guidoInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2guidoInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2guidoInsiderOahGroup>*> (v)) {
        S_msr2guidoInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2guidoInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2guidoInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2guidoInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msr2guidoInsiderHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msr2guidoInsiderHandler:" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);
  os << endl;

  os <<
    "Options groups (" <<
    mfSingularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

ostream& operator<< (ostream& os, const S_msr2guidoInsiderHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msr2guidoInsiderOahGroup gGlobalMsr2guidoInsiderOahGroup;

S_msr2guidoInsiderOahGroup msr2guidoInsiderOahGroup::create ()
{
  msr2guidoInsiderOahGroup* o = new msr2guidoInsiderOahGroup ();
  assert (o!=0);

  return o;
}

msr2guidoInsiderOahGroup::msr2guidoInsiderOahGroup ()
  : oahGroup (
    "msr2guido",
    "hm2g", "help-msr2guido",
R"(Options that are used by msr2guido are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2guidoInsiderOahGroup ();
}

msr2guidoInsiderOahGroup::~msr2guidoInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msr2guidoInsiderOahGroup::initializeMsr2guidoInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  gLogStream << left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    endl;
  }
#endif
}

//______________________________________________________________________________
void msr2guidoInsiderOahGroup::printMsr2guidoInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The msr2guido options are:" <<
    endl;

  ++gIndenter;

  // Guido
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Guido:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) <<
    "generateComments" << " : " << fGenerateComments <<
    endl <<
    setw (fieldWidth) <<
    "generateStem" << " : " << fGenerateStem <<
    endl <<
    setw (fieldWidth) <<
    "generateBars" << " : " << fGenerateBars <<
    endl;

  --gIndenter;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fQuitAfterPass2a" << " : " <<
    fQuitAfterPass2a <<
    endl <<
    setw (fieldWidth) << "fQuitAfterPass2b" << " : " <<
    fQuitAfterPass2b <<
    endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_msr2guidoInsiderOahGroup createGlobalMsr2guidoOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2guido OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2guidoInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsr2guidoInsiderOahGroup =
      msr2guidoInsiderOahGroup::create ();
    assert (gGlobalMsr2guidoInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2guidoInsiderOahGroup;
}


}
