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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "mfslInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "mfslInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________

S_mfslInterpreterOahGroup gGlobalMfslInterpreterOahGroup;

S_mfslInterpreterOahGroup mfslInterpreterOahGroup::create ()
{
  mfslInterpreterOahGroup* o = new mfslInterpreterOahGroup ();
  assert (o != nullptr);
  return o;
}

mfslInterpreterOahGroup::mfslInterpreterOahGroup ()
  : oahGroup (
    "mfslInterpreter",
     "help-mfsl-interpreter-group", "hmi-group",
R"(These options control the way MFSL interpreter works.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  // initialize it
  initializemfslInterpreterOahGroup ();
}

mfslInterpreterOahGroup::~mfslInterpreterOahGroup ()
{}

void mfslInterpreterOahGroup::initializemfslInterpreterOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializemfslInterpretertracingOah ();
#endif
}

void mfslInterpreterOahGroup::enforceGroupQuietness ()
{}

void mfslInterpreterOahGroup::checkGroupOptionsConsistency ()
{}

void mfslInterpreterOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mfslInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterOahGroup>*> (v)) {
        S_mfslInterpreterOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfslInterpreterOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mfslInterpreterOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mfslInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterOahGroup>*> (v)) {
        S_mfslInterpreterOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfslInterpreterOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mfslInterpreterOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void mfslInterpreterOahGroup::printmfslInterpreterOahValues (
  int fieldWidth)
{
  gLogStream <<
    "The mfslInterpreter options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_mfslInterpreterOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mfslInterpreterOahGroup createGlobalMfslInterpreterOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global mfslInterpreter OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMfslInterpreterOahGroup) {
    // create the mfslInterpreter options
    // ------------------------------------------------------

    gGlobalMfslInterpreterOahGroup =
      mfslInterpreterOahGroup::create ();
    assert (gGlobalMfslInterpreterOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMfslInterpreterOahGroup;
}


}
