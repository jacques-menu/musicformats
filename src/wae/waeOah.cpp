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

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
S_waeOahGroup gGlobalWaeOahGroup;

S_waeOahGroup waeOahGroup::create ()
{
  waeOahGroup* o = new waeOahGroup ();
  assert (o != nullptr);

  return o;
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
         "quiet", "q",
R"(Don't issue any warning or error messages.)",
        "fQuiet",
        fQuiet));

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
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> waeOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_waeOahGroup>*
    p =
      dynamic_cast<visitor<S_waeOahGroup>*> (v)) {
        S_waeOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching waeOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void waeOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> waeOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_waeOahGroup>*
    p =
      dynamic_cast<visitor<S_waeOahGroup>*> (v)) {
        S_waeOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching waeOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void waeOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> waeOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void waeOahGroup::printWaeOahValues (int fieldWidth)
{
  gLogStream <<
    "The wae options are:" <<
    endl;

  ++gIndenter;

  // warning and error handling
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Warning and error handling:" <<
    endl;

  ++gIndenter;

  gLogStream <<
    setw (fieldWidth) << "fQuiet" << " : " <<
    fQuiet <<
    endl <<
    setw (fieldWidth) << "fDontShowErrors" << " : " <<
    fDontShowErrors <<
    endl <<
    setw (fieldWidth) << "fDontQuitOnErrors" << " : " <<
    fDontQuitOnErrors <<
    endl;

  --gIndenter;


  --gIndenter;
}

ostream& operator<< (ostream& os, const S_waeOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_waeOahGroup createGlobalWaeOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global wae OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalWaeOahGroup) {
    // create the global wae options group
    gGlobalWaeOahGroup =
      waeOahGroup::create ();
    assert (gGlobalWaeOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalWaeOahGroup;
}


}
