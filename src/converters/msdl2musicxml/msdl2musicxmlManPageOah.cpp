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

#include "msdl2musicxmlManPageOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msdl2musicxmlManPageGenerateAtom msdl2musicxmlManPageGenerateAtom::create (
  const string& shortName,
  const string& longName,
  const string& description,
  S_oahVisitor  theOahVisitor)
{
  msdl2musicxmlManPageGenerateAtom* o = new
    msdl2musicxmlManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msdl2musicxmlManPageGenerateAtom::msdl2musicxmlManPageGenerateAtom (
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

msdl2musicxmlManPageGenerateAtom::~msdl2musicxmlManPageGenerateAtom ()
{}

void msdl2musicxmlManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msdl2musicxmlManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void msdl2musicxmlManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2musicxmlManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2musicxmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2musicxmlManPageGenerateAtom>*> (v)) {
        S_msdl2musicxmlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2musicxmlManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2musicxmlManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2musicxmlManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2musicxmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2musicxmlManPageGenerateAtom>*> (v)) {
        S_msdl2musicxmlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2musicxmlManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2musicxmlManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2musicxmlManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void msdl2musicxmlManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdl2musicxmlManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msdl2musicxmlManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "msdl2musicxmlManPageGenerateAtom::generateManPageData");
}

void msdl2musicxmlManPageGenerateAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_msdl2musicxmlManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdl2musicxmlManPageOahGroup gGlobalMsdl2xmlManPageOahGroup;

S_msdl2musicxmlManPageOahGroup msdl2musicxmlManPageOahGroup::create (
  S_oahVisitor theOah2manPage)
{
  msdl2musicxmlManPageOahGroup* o = new msdl2musicxmlManPageOahGroup (
    theOah2manPage);
  assert (o!=0);
  return o;
}

msdl2musicxmlManPageOahGroup::msdl2musicxmlManPageOahGroup (
  S_oahVisitor theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2xmlManPageOahGroup (
    theOah2manPage);
}

msdl2musicxmlManPageOahGroup::~msdl2musicxmlManPageOahGroup ()
{}

void msdl2musicxmlManPageOahGroup::initializeManPageGenerateOptions (
  S_oahVisitor
       theOah2manPage)
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

/* JMI
  // OAH

  fDisplayOah = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "doah", "display-oah",
R"(Write the contents of the OAH data to standard error.)",
        "displayOah",
        fDisplayOah));
*/

  // generate man page
  fOahVisitor = theOah2manPage;

  subGroup->
    appendAtomToSubGroup (
      msdl2musicxmlManPageGenerateAtom::create (
        "generate-man-page", "gmp",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void msdl2musicxmlManPageOahGroup::initializeManPagetracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2musicxmlManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msdl2musicxmlManPageOahGroup::initializeMsdl2xmlManPageOahGroup (
  S_oahVisitor
       theOah2manPage)
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
void msdl2musicxmlManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msdl2musicxmlManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2musicxmlManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2musicxmlManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2musicxmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2musicxmlManPageOahGroup>*> (v)) {
        S_msdl2musicxmlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2musicxmlManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2musicxmlManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2musicxmlManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2musicxmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2musicxmlManPageOahGroup>*> (v)) {
        S_msdl2musicxmlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2musicxmlManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2musicxmlManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2musicxmlManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdl2musicxmlManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdl2musicxmlManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdl2musicxmlManPageOahGroup createGlobalMsdl2xmlManPageOahGroupHandler (
  S_oahVisitor theOah2manPage)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msdl2musicxmlManPage OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2xmlManPageOahGroup) {
    // create the global options group
    gGlobalMsdl2xmlManPageOahGroup =
      msdl2musicxmlManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalMsdl2xmlManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2xmlManPageOahGroup;
}


}
