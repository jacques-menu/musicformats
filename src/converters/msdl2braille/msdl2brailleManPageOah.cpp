/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "oahEarlyOptions.h"

#include "msdl2brailleManPageOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msdl2brailleManPageGenerateAtom msdl2brailleManPageGenerateAtom::create (
  const string& shortName,
  const string& longName,
  const string& description,
  S_oahVisitor  theOahVisitor)
{
  msdl2brailleManPageGenerateAtom* o = new
    msdl2brailleManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msdl2brailleManPageGenerateAtom::msdl2brailleManPageGenerateAtom (
  const string& shortName,
  const string& longName,
  const string& description,
  S_oahVisitor  theOahVisitor)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{
  fOahVisitor = theOahVisitor;
}

msdl2brailleManPageGenerateAtom::~msdl2brailleManPageGenerateAtom ()
{}

void msdl2brailleManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msdl2brailleManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void msdl2brailleManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brailleManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2brailleManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2brailleManPageGenerateAtom>*> (v)) {
        S_msdl2brailleManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2brailleManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2brailleManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brailleManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2brailleManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2brailleManPageGenerateAtom>*> (v)) {
        S_msdl2brailleManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2brailleManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2brailleManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brailleManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void msdl2brailleManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdl2brailleManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msdl2brailleManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomUpLinkToHandler ()->
    setOahHandlerFoundAHelpOption (
      "msdl2brailleManPageGenerateAtom::generateManPageData");
}

void msdl2brailleManPageGenerateAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_msdl2brailleManPageGenerateAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//_______________________________________________________________________________
S_msdl2brailleManPageOahGroup gGlobalMsdl2brlManPageOahGroup;

S_msdl2brailleManPageOahGroup msdl2brailleManPageOahGroup::create (
  S_oahVisitor theOah2manPage)
{
  msdl2brailleManPageOahGroup* o =
    new msdl2brailleManPageOahGroup (
      theOah2manPage);
  assert (o!=0);
  return o;
}

msdl2brailleManPageOahGroup::msdl2brailleManPageOahGroup (
  S_oahVisitor theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2brlManPageOahGroup (
    theOah2manPage);
}

msdl2brailleManPageOahGroup::~msdl2brailleManPageOahGroup ()
{}

void msdl2brailleManPageOahGroup::initializeManPageGenerateOptions (
  S_oahVisitor theOah2manPage)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generate",
        "help-generate-man-page", "hgmp",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // generate man page
  fOah2manPage = theOah2manPage;

  subGroup->
    appendAtomToSubGroup (
      msdl2brailleManPageGenerateAtom::create (
        "generate-man-page", "gmp",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        fOah2manPage));
}

#ifdef TRACING_IS_ENABLED
void msdl2brailleManPageOahGroup::initializeManPagetracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2brailleManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msdl2brailleManPageOahGroup::initializeMsdl2brlManPageOahGroup (
  S_oahVisitor theOah2manPage)
{
  // display
  // --------------------------------------
  initializeManPageGenerateOptions (
    theOah2manPage);

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeManPagetracingOah ();
#endif
}

//______________________________________________________________________________
void msdl2brailleManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msdl2brailleManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2brailleManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brailleManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2brailleManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2brailleManPageOahGroup>*> (v)) {
        S_msdl2brailleManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2brailleManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2brailleManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brailleManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2brailleManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2brailleManPageOahGroup>*> (v)) {
        S_msdl2brailleManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2brailleManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2brailleManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brailleManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdl2brailleManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdl2brailleManPageOahGroup& elt)
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
S_msdl2brailleManPageOahGroup createGlobalMsdl2brlManPageOahHandler (
  S_oahVisitor theOah2manPage)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msdl2blrManPage OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2brlManPageOahGroup) {
    // create the global options group
    gGlobalMsdl2brlManPageOahGroup =
      msdl2brailleManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalMsdl2brlManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2brlManPageOahGroup;
}


}
