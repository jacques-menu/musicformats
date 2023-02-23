/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "visitor.h"
#include "oahWae.h"

#include "mfStaticSettings.h"

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2mxsrOah.h"

#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats

{

//_______________________________________________________________________________

S_msr2mxsrOahGroup gGlobalMsr2mxsrOahGroup;

S_msr2mxsrOahGroup msr2mxsrOahGroup::create ()
{
  msr2mxsrOahGroup* o = new msr2mxsrOahGroup ();
  assert (o != nullptr);
  return o;
}

msr2mxsrOahGroup::msr2mxsrOahGroup ()
  : oahGroup (
    "Msr2mxsr",
    "help-msr2mxsr-to-msr", "hmsr2mxsr",
R"(These options control the way MSR data is translated to an MXSR.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2mxsrOahGroup ();
}

msr2mxsrOahGroup::~msr2mxsrOahGroup ()
{}

#ifdef MF_TRACE_IS_ENABLED
void msr2mxsrOahGroup::initializMsr2mxsrTraceOptions ()
{
  // JMI
}
#endif // MF_TRACE_IS_ENABLED

void msr2mxsrOahGroup::initializeCodeGenerationOptions ()
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
        "musicxml-comments", "xmlcom",
R"(Generate comments showing the structure of the score.)",
        "fMusicXMLComments",
        fMusicXMLComments));
}

void msr2mxsrOahGroup::initializeWorkAndMovementOptions ()
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

void msr2mxsrOahGroup::initializeMsr2mxsrOahGroup ()
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
void msr2mxsrOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2mxsrOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2mxsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2mxsrOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2mxsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2mxsrOahGroup>*> (v)) {
        S_msr2mxsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2mxsrOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msr2mxsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2mxsrOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2mxsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2mxsrOahGroup>*> (v)) {
        S_msr2mxsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2mxsrOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msr2mxsrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2mxsrOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msr2mxsrOahGroup::displayMsr2mxsrOahValues (int valueFieldWidth)
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

std::ostream& operator << (std::ostream& os, const S_msr2mxsrOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msr2mxsrOahGroup createGlobalMsr2mxsrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msr2mxsr OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMsr2mxsrOahGroup) {
    // create the global options group
    gGlobalMsr2mxsrOahGroup =
      msr2mxsrOahGroup::create ();
    assert (gGlobalMsr2mxsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2mxsrOahGroup;
}


}
