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

#include "visitor.h"
#include "oahWae.h"

#include "mfPreprocessorSettings.h"

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2mxsrOah.h"

#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats

{

//_______________________________________________________________________________

S_msr2mxsr2msrOahGroup gGlobalMsr2mxsr2msrOahGroup;

S_msr2mxsr2msrOahGroup msr2mxsr2msrOahGroup::create ()
{
  msr2mxsr2msrOahGroup* obj = new msr2mxsr2msrOahGroup ();
  assert (obj != nullptr);
  return obj;
}

msr2mxsr2msrOahGroup::msr2mxsr2msrOahGroup ()
  : oahGroup (
      "msr2mxsr",
      "help-msr2mxsr-to-msr", "hmsr2mxsr",
R"(These options control the way MSR data is translated to an MXSR.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2mxsr2msrOahGroup ();
}

msr2mxsr2msrOahGroup::~msr2mxsr2msrOahGroup ()
{}

#ifdef MF_TRACE_IS_ENABLED
void msr2mxsr2msrOahGroup::initializMsr2mxsrTraceOptions ()
{
  // JMI
}
#endif // MF_TRACE_IS_ENABLED

void msr2mxsr2msrOahGroup::initializeCodeGenerationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "help-musicxml-code-generation", "hmxmlcg",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // infos
  // --------------------------------------

  fXml2xmlInfos = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "musicxml-generation-infos", "mxmlgi",
R"(Generate initial comments showing the MusicXML generation informations.)",
        "fXml2xmlInfos",
        fXml2xmlInfos));

  // comments
  // --------------------------------------

  fMusicXMLComments = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "musicxml-comments", "xmlcoms",
R"(Generate comments showing the structure of the score.)",
        "fMusicXMLComments",
        fMusicXMLComments));
}

void msr2mxsr2msrOahGroup::initializeWorkAndMovementOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Work and movement",
      "help-work-and-movement", "hwam",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // work title

  subGroup->
    appendAtomToSubGroup (
      oahStringAtom::create (
        "work-title", "wt",
R"(Set the work title to STRING in the MusicXML output.)",
        "STRING",
        "fWorkCreditTypeTitle",
        fWorkCreditTypeTitle));

  // work number

  subGroup->
    appendAtomToSubGroup (
      oahStringAtom::create (
        "work-number", "wn",
R"(Set the work number to STRING in the MusicXML output.)",
        "STRING",
        "fWorkNumber",
        fWorkNumber));

  // movement title

  subGroup->
    appendAtomToSubGroup (
      oahStringAtom::create (
        "movement-title", "mt",
R"(Set the movement title to STRING in the MusicXML output.)",
        "STRING",
        "fMovementTitle",
        fMovementTitle));

  // movement number

  subGroup->
    appendAtomToSubGroup (
      oahStringAtom::create (
        "movement-number", "mn",
R"(Set the movement number to STRING in the MusicXML output.)",
        "STRING",
        "fMovementNumber",
        fMovementNumber));
}

void msr2mxsr2msrOahGroup::initializeMsr2mxsr2msrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializMsr2mxsrTraceOptions ();
#endif // MF_TRACE_IS_ENABLED

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions ();

  // work and movement
  // --------------------------------------
  initializeWorkAndMovementOptions ();
}

//______________________________________________________________________________
void msr2mxsr2msrOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2mxsr2msrOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2mxsr2msrOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2mxsr2msrOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2mxsr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2mxsr2msrOahGroup>*> (v)) {
        S_msr2mxsr2msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2mxsr2msrOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msr2mxsr2msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2mxsr2msrOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2mxsr2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2mxsr2msrOahGroup>*> (v)) {
        S_msr2mxsr2msrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2mxsr2msrOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msr2mxsr2msrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2mxsr2msrOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msr2mxsr2msrOahGroup::displayMsr2mxsrOahValues (int valueFieldWidth)
{
  gLog <<
    "The MusicXML options are:" <<
    std::endl;

  ++gIndenter;

  // work and movement
  // --------------------------------------

  gLog <<
    "Book:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) <<
    "fWorkCreditTypeTitle" << ": " <<
    fWorkCreditTypeTitle <<
    std::endl <<
    std::setw (valueFieldWidth) <<
    "fWorkNumber" << ": " <<
    fWorkNumber <<
    std::endl <<

    std::setw (valueFieldWidth) <<
    "fMovementTitle" << ": " <<
    fMovementTitle <<
    std::endl <<
    std::setw (valueFieldWidth) <<
    "fMovementNumber" << ": " <<
    fMovementNumber <<
    std::endl;

  --gIndenter;


  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msr2mxsr2msrOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msr2mxsr2msrOahGroup createGlobalMsr2mxsr2msrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msr2mxsr OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMsr2mxsr2msrOahGroup) {
    // create the global options group
    gGlobalMsr2mxsr2msrOahGroup =
      msr2mxsr2msrOahGroup::create ();
    assert (gGlobalMsr2mxsr2msrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2mxsr2msrOahGroup;
}


}
