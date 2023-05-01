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

#include "mfStaticSettings.h"

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_waeOahGroup waeOahGroup::create ()
{
  waeOahGroup* obj = new waeOahGroup ();
  assert (obj != nullptr);
  return obj;
}

waeOahGroup::waeOahGroup ()
  : oahGroup (
    "Wae",
    "help-wae", "hg",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeWaeOah ();
}

waeOahGroup::~waeOahGroup ()
{}

void waeOahGroup::initializeWaeWarningAndErrorsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Warnings and errors",
        "help-warnings-and-errors", "hwae",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // quiet

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
         K_QUIET_OPTION_LONG_NAME, K_QUIET_OPTION_SHORT_NAME,
R"(Don't issue any warning or error messages.)",
        "fEarlyQuietOptionRef",
        gEarlyOptions.getEarlyQuietOptionRef ()));

  // don't show errors

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dont-show-errors", "dse",
R"(Don't show errors in the log.)",
        "fDontShowErrors",
        fDontShowErrors));

  // do not quit on errors

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dont-quit-on-errors", "deoe",
        regex_replace (
R"(Do not quit execution on errors and go ahead.
This may be useful when debugging EXECUTABLE.)",
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
        "fDontQuitOnErrors",
        fDontQuitOnErrors));
}

void waeOahGroup::initializeWaeOah ()
{
  // warning and error handling
  // --------------------------------------
  initializeWaeWarningAndErrorsOptions ();
}

//______________________________________________________________________________
void waeOahGroup::enforceGroupQuietness ()
{
  // fDisplayCPUusage = false; // JMI
}

//______________________________________________________________________________
void waeOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void waeOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> waeOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_waeOahGroup>*
    p =
      dynamic_cast<visitor<S_waeOahGroup>*> (v)) {
        S_waeOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching waeOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void waeOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> waeOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_waeOahGroup>*
    p =
      dynamic_cast<visitor<S_waeOahGroup>*> (v)) {
        S_waeOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching waeOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void waeOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> waeOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void waeOahGroup::displayWaeOahValues (int fieldWidth)
{
  gLog <<
    "The wae options are:" <<
    std::endl;

  ++gIndenter;

  // warning and error handling
  // --------------------------------------

  gLog << std::left <<
    std::setw (fieldWidth) << "Warning and error handling:" <<
    std::endl;

  ++gIndenter;

  gLog <<
    std::setw (fieldWidth) << "fDontShowErrors" << ": " <<
    fDontShowErrors <<
    std::endl <<
    std::setw (fieldWidth) << "fDontQuitOnErrors" << ": " <<
    fDontQuitOnErrors <<
    std::endl;

  --gIndenter;


--gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_waeOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//________________________________________________________________________
// hidden global OAH early options variable
EXP S_waeOahGroup pGlobalWaeOahGroup;

EXP S_waeOahGroup getGlobalWaeOahGroup ()
{
  return pGlobalWaeOahGroup;
}

//______________________________________________________________________________
S_waeOahGroup createGlobalWaeOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global wae OAH group";

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! pGlobalWaeOahGroup) {
    // create the global wae options group
    pGlobalWaeOahGroup =
      waeOahGroup::create ();
    assert (pGlobalWaeOahGroup != 0);
  }

  // return the global OAH group
  return pGlobalWaeOahGroup;
}


}
