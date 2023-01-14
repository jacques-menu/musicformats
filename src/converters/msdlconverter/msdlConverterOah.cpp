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

// libmusicxml2
#include "visitor.h"

#include "mfEnableTracingSetting.h"

#include "mfMultiGenerationOah.h"

#include "oahOah.h"

#include "msdlConverterOah.h"

#include "oahEarlyOptions.h"

#include "msdlConverterInsiderHandler.h"


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
  fMultiGenerationOutputKind = mfMultiGenerationOutputKind::kGeneration_UNKNOWN;

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
#ifdef MF_TRACING_IS_ENABLED
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlConverterOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlConverterOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlConverterOahGroup>*> (v)) {
        S_msdlConverterOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlConverterOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdlConverterOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlConverterOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdlConverterOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlConverterOahGroup>*> (v)) {
        S_msdlConverterOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdlConverterOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdlConverterOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdlConverterOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

void msdlConverterOahGroup::printMsdlConverterOahValues (
  int fieldWidth)
{
  gLogStream <<
    "The msdlConverter options are:" <<
    std::endl;

  ++gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msdlConverterOahGroup& elt)
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
S_msdlConverterOahGroup createGlobalMsdlConverterOahGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msdlConverter OAH group" <<
      std::endl;
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
