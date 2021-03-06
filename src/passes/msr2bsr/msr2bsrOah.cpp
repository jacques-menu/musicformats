/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "oahOah.h"
#include "msr2bsrOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicFormats
{
//_______________________________________________________________________________

S_msr2bsrOahGroup gGlobalMsr2bsrOahGroup;

S_msr2bsrOahGroup msr2bsrOahGroup::create ()
{
  msr2bsrOahGroup* o = new msr2bsrOahGroup ();
  assert (o != nullptr);
  return o;
}

msr2bsrOahGroup::msr2bsrOahGroup ()
  : oahGroup (
    "Msr2bsr",
    "help-msr-to-bsr", "hmsr2bsr",
R"(These options control the way MSR data is translated to BSR.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2bsrOahGroup ();
}

msr2bsrOahGroup::~msr2bsrOahGroup ()
{}

void msr2bsrOahGroup::initializeMsr2bsrMiscellaneousOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Miscellaneous",
        "help-miscellaneous", "hlpm",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // Braille headings

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-music-headings", "nmh",
R"(Don't generate any music headings in the Braille code.)",
        "noBrailleHeadings",
        fNoBrailleHeadings));

  // braille tempos

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-tempos", "nt",
R"(Don't generate any tempos in the Braille code.)",
        "noTempos",
        fNoTempos));

  // braille page numbers

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-page-numbers", "npn",
R"(Don't generate any page numbers in the Braille code.)",
        "noBraillePageNumbers",
        fNoPageNumbers));

  // braille line numbers

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-line-numbers", "nln",
R"(Don't generate any line numbers in the Braille code.)",
        "noBrailleLineNumbers",
        fNoLineNumbers));

  // braille measure numbers

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-measure-numbers", "nmn",
R"(Don't generate any measure numbers in the Braille code.)",
        "noMeasureNumbers",
        fNoMeasureNumbers));

  // braille lyrics

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-lyrics", "nl",
R"(Don't generate any lyrics in the Braille code.)",
        "noBrailleLyrics",
        fNoBrailleLyrics));

  // braille conversion date

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "braille-conversion-date", "lpcd",
R"(Generate code to include the conversion date
when Braille creates the score.)",
        "brailleRunDate",
        fBrailleRunDate));

  // facsimile

  fFacSimileKind = kFacSimileNo;

  subGroup->
    appendAtomToSubGroup (
      bsrFacSimileKindAtom::create (
        "facsimile", "fs",
R"(Generate facsimile Braille nusic code.
By default, non-facsimile code is generated.)",
        "YES_OR_NO",
        "facSimileKind",
        fFacSimileKind));

  // clefs

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "clefs", "",
R"(Include clefs in BSR. By default, they are not.)",
        "includeClefs",
        fIncludeClefs));
}

void msr2bsrOahGroup::initializeMsr2bsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsr2bsrtracingOahGroup ();
#endif

  // miscellaneous
  // --------------------------------------
  initializeMsr2bsrMiscellaneousOptions ();
}

void msr2bsrOahGroup::enforceGroupQuietness ()
{}

void msr2bsrOahGroup::checkGroupOptionsConsistency ()
{}

void msr2bsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2bsrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2bsrOahGroup>*> (v)) {
        S_msr2bsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2bsrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2bsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2bsrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2bsrOahGroup>*> (v)) {
        S_msr2bsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2bsrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2bsrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2bsrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void msr2bsrOahGroup::printMsr2bsrOahValues (int fieldWidth)
{
  gLogStream <<
    "The Msr2bsr options are:" <<
    endl;

  ++gIndenter;

  // miscellaneous
  // --------------------------------------

  gLogStream <<
    "Miscellaneous:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fNoBrailleHeadings" << " : " <<
      fNoBrailleHeadings <<
      endl <<

    setw (fieldWidth) << "fNoTempos" << " : " <<
      fNoTempos <<
      endl <<

    setw (fieldWidth) << "fNoPageNumbers" << " : " <<
      fNoPageNumbers <<
      endl <<
    setw (fieldWidth) << "fNoLineNumbers" << " : " <<
      fNoLineNumbers <<
      endl <<
    setw (fieldWidth) << "fNoMeasureNumbers" << " : " <<
      fNoMeasureNumbers <<
      endl <<

    setw (fieldWidth) << "fNoBrailleLyrics" << " : " <<
      fNoBrailleLyrics <<
      endl <<

    setw (fieldWidth) << "fBrailleRunDate" << " : " <<
      fBrailleRunDate <<
      endl <<

    setw (fieldWidth) << "fFacSimileKind" << " : " <<
      bsrFacSimileKindAsString (fFacSimileKind) <<
      endl <<

    setw (fieldWidth) << "fIncludeClefs" << " : " <<
      fIncludeClefs <<
      endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msr2bsrOahGroup& elt)
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
S_msr2bsrOahGroup createGlobalMsr2bsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2bsr OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2bsrOahGroup) {
    // create the global options group
    gGlobalMsr2bsrOahGroup =
      msr2bsrOahGroup::create ();
    assert (gGlobalMsr2bsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2bsrOahGroup;
}


}
