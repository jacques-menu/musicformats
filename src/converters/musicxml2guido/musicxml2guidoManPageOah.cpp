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

#include "musicxml2guidoManPageOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_xml2gmnManPageGenerateAtom xml2gmnManPageGenerateAtom::create (
  const string& shortName,
  const string& longName,
  const string& description,
  S_oahVisitor  theOahVisitor)
{
  xml2gmnManPageGenerateAtom* o = new
    xml2gmnManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

xml2gmnManPageGenerateAtom::xml2gmnManPageGenerateAtom (
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

xml2gmnManPageGenerateAtom::~xml2gmnManPageGenerateAtom ()
{}

void xml2gmnManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a xml2gmnManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void xml2gmnManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2gmnManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2gmnManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2gmnManPageGenerateAtom>*> (v)) {
        S_xml2gmnManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2gmnManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2gmnManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2gmnManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2gmnManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2gmnManPageGenerateAtom>*> (v)) {
        S_xml2gmnManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2gmnManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2gmnManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2gmnManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2gmnManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2gmnManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void xml2gmnManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "xml2gmnManPageGenerateAtom::generateManPageData");
}

void xml2gmnManPageGenerateAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2gmnManPageGenerateAtom& elt)
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
S_xml2gmnManPageOahGroup gGlobalXml2gmnManPageOahGroup;

S_xml2gmnManPageOahGroup xml2gmnManPageOahGroup::create (
  S_oahVisitor theOah2manPage)
{
  xml2gmnManPageOahGroup* o = new xml2gmnManPageOahGroup (
    theOah2manPage);
  assert (o!=0);
  return o;
}

xml2gmnManPageOahGroup::xml2gmnManPageOahGroup (
  S_oahVisitor theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2gmnManPageOahGroup (
    theOah2manPage);
}

xml2gmnManPageOahGroup::~xml2gmnManPageOahGroup ()
{}

void xml2gmnManPageOahGroup::initializeManPageGenerateOptions (
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
      xml2gmnManPageGenerateAtom::create (
        "generate-man-page", "gmp",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void xml2gmnManPageOahGroup::initializeManPagetracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "xml2gmnManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void xml2gmnManPageOahGroup::initializeXml2gmnManPageOahGroup (
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
void xml2gmnManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2gmnManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2gmnManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2gmnManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2gmnManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2gmnManPageOahGroup>*> (v)) {
        S_xml2gmnManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2gmnManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2gmnManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2gmnManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2gmnManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2gmnManPageOahGroup>*> (v)) {
        S_xml2gmnManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2gmnManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2gmnManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2gmnManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void xml2gmnManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_xml2gmnManPageOahGroup& elt)
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
S_xml2gmnManPageOahGroup createGlobalXml2gmnManPageOahGroupHandler (
  S_oahVisitor theOah2manPage)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global xml2gmnManPage OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2gmnManPageOahGroup) {
    // create the global options group
    gGlobalXml2gmnManPageOahGroup =
      xml2gmnManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalXml2gmnManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2gmnManPageOahGroup;
}


}
