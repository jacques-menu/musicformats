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

#include "oahWae.h"

#include "mfInitialization.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "mfOutputFileOah.h"
#include "oahDisplayOah.h"

#include "mxsrOah.h"
#include "msr2mxsrOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "mxsr2msrOah.h"
#include "guidoGenerationOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2guidoComponent.h"

#include "musicxml2guidoManPageOah.h"

#include "musicxml2guidoInsiderHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_xml2gmnInsiderHandler xml2gmnInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  xml2gmnInsiderHandler* o = new
    xml2gmnInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o!=0);

  return o;
}

xml2gmnInsiderHandler::xml2gmnInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
           Welcome to the MusicXML to Guido converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
R"(
Usage: xml2gmn [[option]* [MusicXMLFile|-] [[option]*
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

  // create the xml2gmn prefixes
  createTheXml2gmnPrefixes ();

  // create the xml2gmn option groups
  createTheXml2gmnOptionGroups (serviceName);
}

xml2gmnInsiderHandler::~xml2gmnInsiderHandler ()
{}

void xml2gmnInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMusicxml2guidoConverterComponent ();
}

//_______________________________________________________________________________
std::string xml2gmnInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What xml2gmn does:

    This multi-pass converter basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a first MusicXML tree;
        Pass 2a: converts that MusicXML tree into
                 a first Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the first MSR a second MSR, to apply options;
        Pass 4:  converts the second MSR into a second MusicXML tree;
        Pass 5:  converts the second MusicXML tree to Guido code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void xml2gmnInsiderHandler::createTheXml2gmnPrefixes ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
		std::stringstream ss;

    ss <<
      "Creating the xml2gmn prefixes in \"" <<
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
void xml2gmnInsiderHandler::createTheXml2gmnOptionGroups (
  const std::string& serviceName)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
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

  // initialize common things
  // ------------------------------------------------------
  initializeMusicFormats ();

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

  initializeWAE ();

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

  // create the MXSR2guido OAH group
  appendGroupToHandler (
    createGlobalGuidoGenerationOahGroup ());

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif

  // create the global xml2brl OAH group only now,
  // after the groups whose options it may use
  // have been created
  appendGroupToHandler (
    createGlobalXml2gmnOahGroup ());
}

//______________________________________________________________________________
void xml2gmnInsiderHandler::checkOptionsAndArguments () const
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
		std::stringstream ss;

    ss <<
      "Checking options and arguments from argc/argv in \"" <<
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
std::string xml2gmnInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
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
          gGlobalCurrentServiceRunData->getInputSourceName ();

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
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        std::stringstream ss;

        ss <<
          "xml2gmnInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif

      // append the file extension to the output file name
       outputFileName += ".gmn";

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        std::stringstream ss;

        ss <<
          "xml2gmnInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
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
      // nothing to do
    }
  }

  return outputFileName;
}

//______________________________________________________________________________
void xml2gmnInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2gmnInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalXml2gmnInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsr2msrOahGroup->
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
void xml2gmnInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void xml2gmnInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "xml2gmnInsiderOahGroup: a MusicXML output file name must be selected with '-o, -output-file-name";

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
void xml2gmnInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> xml2gmnInsiderOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_xml2gmnInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2gmnInsiderOahGroup>*> (v)) {
        S_xml2gmnInsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2gmnInsiderOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void xml2gmnInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> xml2gmnInsiderOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_xml2gmnInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2gmnInsiderOahGroup>*> (v)) {
        S_xml2gmnInsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2gmnInsiderOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2gmnInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> xml2gmnInsiderOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2gmnInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2gmnInsiderHandler:" <<
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

std::ostream& operator << (std::ostream& os, const S_xml2gmnInsiderHandler& elt)
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
S_xml2gmnInsiderOahGroup gGlobalXml2gmnInsiderOahGroup;

S_xml2gmnInsiderOahGroup xml2gmnInsiderOahGroup::create ()
{
  xml2gmnInsiderOahGroup* o = new xml2gmnInsiderOahGroup ();
  assert (o!=0);

  return o;
}

xml2gmnInsiderOahGroup::xml2gmnInsiderOahGroup ()
  : oahGroup (
    "xml2gmn",
     "help-xml2gmn", "hx2g",
R"(Options that are used by xml2gmn are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2gmnInsiderOahGroup ();
}

xml2gmnInsiderOahGroup::~xml2gmnInsiderOahGroup ()
{}

//_______________________________________________________________________________
void xml2gmnInsiderOahGroup::initializeXml2gmnInsiderOahGroup ()
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

//_______________________________________________________________________________
void xml2gmnInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
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
        "help-msr2gmn-quit", "hm2xquit",
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

  // quit after pass 3

  S_oahBooleanAtom
    quit3OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-3", "qap3",
R"(Quit after pass 3, i.e. after conversion
of the first MSR to the second one.)",
        "fQuitAfterPass3",
        fQuitAfterPass3);

  subGroup->
    appendAtomToSubGroup (
      quit3OahBooleanAtom);
}

//______________________________________________________________________________
void xml2gmnInsiderOahGroup::printXml2gmnInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The xml2gmn options are:" <<
    std::endl;

  ++gIndenter;

/*
  // Guido
  // --------------------------------------

  gLogStream << std::left <<
    std::setw (fieldWidth) << "Guido:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "generateComments" << ": " << fGenerateComments <<
    std::endl <<
    std::setw (fieldWidth) <<
    "generateStem" << ": " << fGenerateStem <<
    std::endl <<
    std::setw (fieldWidth) <<
    "generateBars" << ": " << fGenerateBars <<
    std::endl;

  --gIndenter;
*/

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fQuitAfterPass2a" << ": " <<
    fQuitAfterPass2a <<
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass2b" << ": " <<
    fQuitAfterPass2b <<
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass3" << ": " <<
    fQuitAfterPass3 <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_xml2gmnInsiderOahGroup createGlobalXml2gmnOahGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating global xml2gmn OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2gmnInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalXml2gmnInsiderOahGroup =
      xml2gmnInsiderOahGroup::create ();
    assert (gGlobalXml2gmnInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2gmnInsiderOahGroup;
}


}
