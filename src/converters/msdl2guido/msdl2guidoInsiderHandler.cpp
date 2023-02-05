/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfStaticSettings.h"

#include "mfInitialization.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "mfOutputFileOah.h"
#include "oahDisplayOah.h"

#include "msdl2msrOah.h"

#include "mxsrOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2mxsrOah.h"

#include "oahEarlyOptions.h"

#include "msdl2guidoComponent.h"

#include "msdl2guidoManPageOah.h"

#include "msdl2guidoInsiderHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdl2guidoInsiderHandler msdl2guidoInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  msdl2guidoInsiderHandler* o = new
    msdl2guidoInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o!=0);

  return o;
}

msdl2guidoInsiderHandler::msdl2guidoInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
             Welcome to the MSDL to Guido converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
R"(
Usage: msdl2guido [option]* [MSDLFile] [option]*
)")
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

  // create the msdl2guido prefixes
  createTheMsdl2gmnPrefixes ();

  // create the msdl2guido option groups
  createTheMsdl2gmnOptionGroups (serviceName);
}

msdl2guidoInsiderHandler::~msdl2guidoInsiderHandler ()
{}

void msdl2guidoInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMsdl2guidoConverterComponent ();
}

//_______________________________________________________________________________
std::string msdl2guidoInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What msdl2guido does:

    This multi-pass converter basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a first MusicXML tree;
        Pass 2a: converts that MusicXML tree into
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the MSR into a second MSR, to apply options;
        Pass 4:  converts the second MSR into a second MusicXML tree;
        Pass 5:  converts the second MusicXML tree to Guido code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void msdl2guidoInsiderHandler::createTheMsdl2gmnPrefixes ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating the msdl2guido prefixes in \"" <<
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
void msdl2guidoInsiderHandler::createTheMsdl2gmnOptionGroups (
  const std::string& serviceName)
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

  // create the output file OAH group
  appendGroupToHandler (
    createGlobalOutputFileOahGroup ());

  // create the display OAH group
  appendGroupToHandler (
    createGlobalDisplayOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  initializeMusicFormats ();

  initializeWAE ();

  initializeMSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the msdl2guido OAH group
  appendGroupToHandler (
    createGlobalMsdl2gmnInsiderOahGroup ());

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

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif
}

//______________________________________________________________________________
void msdl2guidoInsiderHandler::checkOptionsAndArguments () const
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

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
std::string msdl2guidoInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
  return ""; // JMI
}

//______________________________________________________________________________
void msdl2guidoInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msdl2guidoInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsdl2gmnInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2mxsrOahGroup->
    enforceGroupQuietness ();

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED
  gGlobalHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void msdl2guidoInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msdl2guidoInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "msdl2guidoInsiderOahGroup: a MusicXML output file name must be selected with '-o, -output-file-name";

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
void msdl2guidoInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2guidoInsiderOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msdl2guidoInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2guidoInsiderOahGroup>*> (v)) {
        S_msdl2guidoInsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2guidoInsiderOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2guidoInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2guidoInsiderOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msdl2guidoInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2guidoInsiderOahGroup>*> (v)) {
        S_msdl2guidoInsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2guidoInsiderOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2guidoInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2guidoInsiderOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msdl2guidoInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msdl2guidoInsiderHandler:" <<
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

std::ostream& operator << (std::ostream& os, const S_msdl2guidoInsiderHandler& elt)
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
S_msdl2guidoInsiderOahGroup gGlobalMsdl2gmnInsiderOahGroup;

S_msdl2guidoInsiderOahGroup msdl2guidoInsiderOahGroup::create ()
{
  msdl2guidoInsiderOahGroup* o = new msdl2guidoInsiderOahGroup ();
  assert (o!=0);

  return o;
}

msdl2guidoInsiderOahGroup::msdl2guidoInsiderOahGroup ()
  : oahGroup (
    "msdl2guido",
    "hmsdl2g", "help-msdl2guido",
R"(Options that are used by msdl2guido are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2gmnInsiderOahGroup ();
}

msdl2guidoInsiderOahGroup::~msdl2guidoInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msdl2guidoInsiderOahGroup::initializeMsdl2gmnInsiderOahGroup ()
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

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

void msdl2guidoInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
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
        "help-msdl2guido-quit", "hmsdl2guidoquit",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // quit after pass 1

  S_oahBooleanAtom
    quit2aOahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-1", "qap1",
R"(Quit after pass 1, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
        "fQuitAfterPass1",
        fQuitAfterPass1);

  subGroup->
    appendAtomToSubGroup (
      quit2aOahBooleanAtom);
}

//______________________________________________________________________________
void msdl2guidoInsiderOahGroup::printMsdl2gmnInsiderOahGroupValues (
  int fieldWidth)
{
  gLogStream <<
    "The msdl2guido options are:" <<
    std::endl;

  ++gIndenter;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fQuitAfterPass1" << ": " <<
    fQuitAfterPass1 <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_msdl2guidoInsiderOahGroup createGlobalMsdl2gmnInsiderOahGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating global msdl2guido insider OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2gmnInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsdl2gmnInsiderOahGroup =
      msdl2guidoInsiderOahGroup::create ();
    assert (gGlobalMsdl2gmnInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2gmnInsiderOahGroup;
}


}
