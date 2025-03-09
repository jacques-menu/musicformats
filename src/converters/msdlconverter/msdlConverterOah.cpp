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

// libmusicxml2
#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfMultiGenerationOah.h"

#include "oahOah.h"

#include "msdlConverterOah.h"

#include "oahEarlyOptions.h"

#include "msdlConverterInsiderHandler.h"

// WAE
#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________

S_msdlConverterOahGroup gGlobalMsdlConverterOahGroup;

S_msdlConverterOahGroup msdlConverterOahGroup::create ()
{
  msdlConverterOahGroup* obj = new msdlConverterOahGroup ();
  assert (obj != nullptr);
  return obj;
}

msdlConverterOahGroup::msdlConverterOahGroup ()
  : oahGroup (
      "msdlConverter",
      "help-msdl-converter-group", "hmc-group",
R"(These options control the way MSDL converter works.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
/*
  fMultiGenerationOutputKind = mfMultiGenerationOutputKind::kGeneration_UNKNOWN_;

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
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsdlConverterTraceOah ();
#endif // MF_TRACE_IS_ENABLED
}

void msdlConverterOahGroup::enforceGroupQuietness ()
{}

void msdlConverterOahGroup::checkGroupOptionsConsistency ()
{}

void msdlConverterOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlConverterOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlConverterOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlConverterOahGroup>*> (v)) {
        S_msdlConverterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlConverterOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdlConverterOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlConverterOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdlConverterOahGroup>*
    p =
      dynamic_cast<visitor<S_msdlConverterOahGroup>*> (v)) {
        S_msdlConverterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdlConverterOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdlConverterOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdlConverterOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msdlConverterOahGroup::printMsdlConverterOahValues (
  int fieldWidth)
{
  gLog <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msdlConverterOahGroup createGlobalMsdlConverterOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msdlConverter OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
