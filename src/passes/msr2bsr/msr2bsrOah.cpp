/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfStaticSettings.h"

#include "mfStringsHandling.h"

#include "oahOah.h"
#include "msr2bsrOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"

#include "waeHandlers.h"


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

  fFacSimileKind = bsrFacSimileKind::kFacSimileNo;

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
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsr2bsrTraceOahGroup ();
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2bsrOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2bsrOahGroup>*> (v)) {
        S_msr2bsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2bsrOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msr2bsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2bsrOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msr2bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2bsrOahGroup>*> (v)) {
        S_msr2bsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2bsrOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msr2bsrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msr2bsrOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msr2bsrOahGroup::displayMsr2bsrOahValues (int fieldWidth)
{
  gLog <<
    "The Msr2bsr options are:" <<
    std::endl;

  ++gIndenter;

  // miscellaneous
  // --------------------------------------

  gLog <<
    "Miscellaneous:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fNoBrailleHeadings" << ": " <<
      fNoBrailleHeadings <<
      std::endl <<

    std::setw (fieldWidth) << "fNoTempos" << ": " <<
      fNoTempos <<
      std::endl <<

    std::setw (fieldWidth) << "fNoPageNumbers" << ": " <<
      fNoPageNumbers <<
      std::endl <<
    std::setw (fieldWidth) << "fNoLineNumbers" << ": " <<
      fNoLineNumbers <<
      std::endl <<
    std::setw (fieldWidth) << "fNoMeasureNumbers" << ": " <<
      fNoMeasureNumbers <<
      std::endl <<

    std::setw (fieldWidth) << "fNoBrailleLyrics" << ": " <<
      fNoBrailleLyrics <<
      std::endl <<

    std::setw (fieldWidth) << "fBrailleRunDate" << ": " <<
      fBrailleRunDate <<
      std::endl <<

    std::setw (fieldWidth) << "fFacSimileKind" << ": " <<
      bsrFacSimileKindAsString (fFacSimileKind) <<
      std::endl <<

    std::setw (fieldWidth) << "fIncludeClefs" << ": " <<
      fIncludeClefs <<
      std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msr2bsrOahGroup& elt)
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
S_msr2bsrOahGroup createGlobalMsr2bsrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msr2bsr OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
