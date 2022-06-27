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

#include "mfMultiGenerationOah.h"

#include "oahOah.h"

#include "msdlConverterOah.h"

#include "oahEarlyOptions.h"

#include "msdlConverterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________

S_msdlConverterOahGroup gGlobalMsdlConverterOahGroup;

S_msdlConverterOahGroup msdlConverterOahGroup::create ()
{
  msdlConverterOahGroup* o = new msdlConverterOahGroup ();
  assert (o != nullptr);
  return o;
}

msdlConverterOahGroup::msdlConverterOahGroup ()
  : oahGroup (
    "msdlConverter",
     "help-msdl-converter-group", "hmc-group",
R"(These options control the way MSDL converter works.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
/*
  fMultiGenerationOutputKind = mfMultiGenerationOutputKind::k_NoGeneration;

  fBrailleUTFKind = bsrUTFKind::kUTF8; // default value

  fBrailleOutputKind = bsrBrailleOutputKind::kBrailleOutputAscii; // default value

  fByteOrderingKind = bsrByteOrderingKind::kByteOrderingNone;
*/

  // initialize it
  initializeMsdlConverterOahGroup ();
}

msdlConverterOahGroup::~msdlConverterOahGroup ()
{}

void msdlConverterOahGroup::initializeMsdlConverterOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsdlConvertertracingOah ();
#endif
}

void msdlConverterOahGroup::enforceGroupQuietness ()
{}

void msdlConverterOahGroup::checkGroupOptionsConsistency ()
{}

void msdlConverterOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlConverterOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlConverterOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlConverterOahGroup>*> (v)) {
        S_msdlConverterOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlConverterOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlConverterOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlConverterOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdlConverterOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlConverterOahGroup>*> (v)) {
        S_msdlConverterOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlConverterOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlConverterOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlConverterOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void msdlConverterOahGroup::printMsdlConverterOahValues (
  int fieldWidth)
{
  gLogStream <<
    "The msdlConverter options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdlConverterOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msdlConverterOahGroup createGlobalMsdlConverterOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msdlConverter OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdlConverterOahGroup) {

    // initialize the generated output kinds map
    // ------------------------------------------------------

    initializeMultiGenerationOutputKindsMap ();

    // create the msdlConverter options
    // ------------------------------------------------------

    gGlobalMsdlConverterOahGroup =
      msdlConverterOahGroup::create ();
    assert (gGlobalMsdlConverterOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdlConverterOahGroup;
}


}
