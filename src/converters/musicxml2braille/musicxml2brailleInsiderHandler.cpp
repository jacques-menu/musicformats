/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...
#include <regex>

#include "oahWae.h"

#include "enableHarmoniesExtraOahIfDesired.h"
#ifdef EXTRA_OAH_IS_ENABLED
  #include "harmoniesExtraOah.h"
#endif

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msr.h"
#include "bsr.h"

#include "oahOah.h"
#include "waeOah.h"
#include "outputFileOah.h"
#include "displayOah.h"

#include "mxsrOah.h"
#include "mxsr2msrOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2brailleComponent.h"

#include "musicxml2brailleInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_xml2brlInsiderHandler xml2brlInsiderHandler::create (
  const string& serviceName,
  const string& handlerHeader)
{
  // create the insider handler
  xml2brlInsiderHandler* o = new
    xml2brlInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o!=0);

  return o;
}

xml2brlInsiderHandler::xml2brlInsiderHandler (
  const string& serviceName,
  const string& handlerHeader)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Initializing xml2brl insider options handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

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
string xml2brlInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What xml2brl does:

    This multi-pass converter basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML tree into
                 a first Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the first MSR into a second MSR, to apply options
        Pass 4:  converts the second MSR into
                 a first Braille Score Representation (BSR)
                 containing one Braille page per MusicXML page;
        Pass 5:  converts the first BSR into a second BSR
                 with as many Braille pages as needed
                 to fit the line and page lengthes;
        Pass 6:  converts the BSR to Braille text
                 and writes it to standard output.

    In this preliminary version, pass 3 merely clones the MSR it receives.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void xml2brlInsiderHandler::createTheXml2brlPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Creating the xml2brl prefixes" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void xml2brlInsiderHandler::createTheXml2brlOptionGroups (
  const string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Creating the xml2brl insider option groups" <<
      fHandlerHeader <<
      "\"" <<
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
  initializeBSR ();

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

  // create the msr2bsr OAH group
  appendGroupToHandler (
    createGlobalMsr2bsrOahGroup ());

  // create the BSR OAH group
  appendGroupToHandler (
    createGlobalBsrOahGroup ());

  // create the braille generation OAH group
  appendGroupToHandler (
    createGlobalBrailleGenerationOahGroup ());

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif

  // create the global xml2brl OAH group only now,
  // after the groups whose options it may use
  // have been created
  appendGroupToHandler (
    createGlobalXml2brlOahGroup (
      serviceName,
      fHandlerHeader));

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    // print the options handler initial state
    gLogStream <<
      "xml2brlInsiderHandler has been initialized as:" <<
      endl;

    ++gIndenter;

    print (gLogStream);

    gLogStream <<
      endl;

    --gIndenter;
  }
#endif
}

//______________________________________________________________________________
void xml2brlInsiderHandler::checkOptionsAndArguments () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
string xml2brlInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
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
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "xml2brlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

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

          case bsrBrailleOutputKind::kBrailleOutputUtf8:
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

          case bsrBrailleOutputKind::kBrailleOutputUtf8Debug:
            outputFileName += "_Utf8Debug";
            break;

          case bsrBrailleOutputKind::kBrailleOutputUtf16:
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

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "xml2brlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

      // append the file extension to the output file name
      switch (brailleOutputKind) {
        case bsrBrailleOutputKind::kBrailleOutputAscii:
         outputFileName += ".brf";
          break;

        case bsrBrailleOutputKind::kBrailleOutputUtf8:
        case bsrBrailleOutputKind::kBrailleOutputUtf16:
          outputFileName += ".brf";
          break;

        case bsrBrailleOutputKind::kBrailleOutputUtf8Debug:
          outputFileName += ".brf"; // since braille cells and regular text are present
          break;
      } // switch

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "xml2brlInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 3 = " <<
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
void xml2brlInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2brlInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalXml2brlInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMxsr2msrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2bsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBrailleGenerationOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
#endif
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlInsiderOahGroup>*> (v)) {
        S_xml2brlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlInsiderOahGroup>*> (v)) {
        S_xml2brlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2brlInsiderHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2brlInsiderHandler:" <<
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

ostream& operator<< (ostream& os, const S_xml2brlInsiderHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2brlInsiderOahGroup gGlobalXml2brlInsiderOahGroup;

S_xml2brlInsiderOahGroup xml2brlInsiderOahGroup::create (
  const string& serviceName,
  const string& handlerHeader)
{
  xml2brlInsiderOahGroup* o = new xml2brlInsiderOahGroup (
    serviceName,
    handlerHeader);
  assert (o!=0);

  return o;
}

xml2brlInsiderOahGroup::xml2brlInsiderOahGroup (
  const string& serviceName,
  const string& handlerHeader)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream << left <<
      "Initializing \"" <<
      fGroupHeader <<
      "\" group" <<
      endl;
  }
#endif

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

//_______________________________________________________________________________
void xml2brlInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream << left <<
      "Creating insider quit subgroup in \"" <<
      fGroupHeader <<
      "\"" <<
      endl;
  }
#endif

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "help-msr2brl-quit", "hm2bquit",
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
of the first MSR to a second MSR.)",
        "fQuitAfterPass3",
        fQuitAfterPass3);

  subGroup->
    appendAtomToSubGroup (
      quit3OahBooleanAtom);
}

//______________________________________________________________________________
void xml2brlInsiderOahGroup::printXml2brlInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The xml2brl options are:" <<
    endl;

  ++gIndenter;

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
    endl <<
    setw (fieldWidth) << "fQuitAfterPass3" << " : " <<
    fQuitAfterPass3 <<
    endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_xml2brlInsiderOahGroup createGlobalXml2brlOahGroup (
  const string& serviceName,
  const string& handlerHeader)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global xml2brl OAH group" <<
      endl;
  }
#endif

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

