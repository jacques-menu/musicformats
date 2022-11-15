/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <regex>

// libmusicxml2
#include "visitor.h"

#include "oahWae.h"

#include "enableHarmoniesExtraOahIfDesired.h"
#ifdef EXTRA_OAH_IS_ENABLED
  #include "harmoniesExtraOah.h"
#endif

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrInitialization.h"
#include "bsrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "outputFileOah.h"
#include "displayOah.h"

#include "msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "oahEarlyOptions.h"

#include "msr2brailleComponent.h"

#include "msr2brailleInsiderHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msr2brailleInsiderHandler msr2brailleInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  msr2brailleInsiderHandler* o = new
    msr2brailleInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o!=0);

  return o;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing msr2braille insider options handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

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
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML tree into
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3a: converts the MSR into a
                 Braille Score Representation (BSR)
                 containing one Braille page per MusicXML page;
        Pass 3b: converts the BSRinto another BSR
                 with as many Braille pages as needed
                 to fit the line and page lengthes;
        Pass 4:  converts the BSR to Unicode text
                 and writes it to standard output.

    In this preliminary version, pass 3b merely clones the BSR it receives.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void msr2brailleInsiderHandler::createTheXml2braillePrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the msr2braille prefixes" <<
      std::endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msr2brailleInsiderHandler::createTheXml2brailleOptionGroups (
  const std::string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the msr2braille insider option groups" <<
      fHandlerHeader <<
      "\"" <<
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

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    // print the options handler initial state
    gLogStream <<
      "msr2brailleInsiderHandler has been initialized as:" <<
      std::endl;

    ++gIndenter;

    print (gLogStream);

    gLogStream <<
      std::endl;

    --gIndenter;
  }
#endif
}

//______________________________________________________________________________
void msr2brailleInsiderHandler::checkOptionsAndArguments () const
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
std::string msr2brailleInsiderHandler::fetchOutputFileNameFromTheOptions () const
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
          "msr2brailleInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          std::endl;
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

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream <<
          "msr2brailleInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif

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

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream <<
          "msr2brailleInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 3 = " <<
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
void msr2brailleInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2brailleInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsr2brailleInsiderOahGroup->
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
void msr2brailleInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2brailleInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2brailleInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2brailleInsiderOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2brailleInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2brailleInsiderOahGroup>*> (v)) {
        S_msr2brailleInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2brailleInsiderOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2brailleInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2brailleInsiderOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2brailleInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2brailleInsiderOahGroup>*> (v)) {
        S_msr2brailleInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2brailleInsiderOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2brailleInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2brailleInsiderOahGroup::browseData ()" <<
      std::endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msr2brailleInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

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

std::ostream& operator << (std::ostream& os, const S_msr2brailleInsiderHandler& elt)
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
S_msr2brailleInsiderOahGroup gGlobalMsr2brailleInsiderOahGroup;

S_msr2brailleInsiderOahGroup msr2brailleInsiderOahGroup::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  msr2brailleInsiderOahGroup* o = new msr2brailleInsiderOahGroup (
    serviceName,
    handlerHeader);
  assert (o!=0);

  return o;
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
void msr2brailleInsiderOahGroup::printXml2brailleInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The msr2braille options are:" <<
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
S_msr2brailleInsiderOahGroup createGlobalMrailleGenerationOahGroup (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2braille OAH group" <<
      std::endl;
  }
#endif

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

