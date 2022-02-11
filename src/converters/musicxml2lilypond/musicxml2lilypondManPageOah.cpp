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

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2lilypondManPageOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_xml2lyManPageGenerateAtom xml2lyManPageGenerateAtom::create (
  const string& shortName,
  const string& longName,
  const string& description,
  S_oahVisitor  theOahVisitor)
{
  xml2lyManPageGenerateAtom* o = new
    xml2lyManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

xml2lyManPageGenerateAtom::xml2lyManPageGenerateAtom (
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

xml2lyManPageGenerateAtom::~xml2lyManPageGenerateAtom ()
{}

void xml2lyManPageGenerateAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a xml2lyManPageGenerateAtom" <<
      endl;
  }
#endif

  generateManPageData (os);
}

void xml2lyManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2lyManPageGenerateAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyManPageGenerateAtom>*> (v)) {
        S_xml2lyManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2lyManPageGenerateAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2lyManPageGenerateAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyManPageGenerateAtom>*> (v)) {
        S_xml2lyManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2lyManPageGenerateAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2lyManPageGenerateAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2lyManPageGenerateAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "musicxml2lilypondInsiderManPageGenerateAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void xml2lyManPageGenerateAtom::generateManPageData (ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomHandlerUpLink ()->
    setOahHandlerFoundAHelpOption (
      "musicxml2lilypondInsiderManPageGenerateAtom::generateManPageData");
}

void xml2lyManPageGenerateAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2lyManPageGenerateAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2lyManPageOahGroup gGlobalXml2lyManPageOahGroup;

S_xml2lyManPageOahGroup xml2lyManPageOahGroup::create (
  S_oahVisitor theOah2manPage)
{
  xml2lyManPageOahGroup* o = new xml2lyManPageOahGroup (
    theOah2manPage);
  assert (o!=0);
  return o;
}

xml2lyManPageOahGroup::xml2lyManPageOahGroup (
  S_oahVisitor theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2lyManPageOahGroup (
    theOah2manPage);
}

xml2lyManPageOahGroup::~xml2lyManPageOahGroup ()
{}

void xml2lyManPageOahGroup::initializeManPageGenerateOptions (
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
      xml2lyManPageGenerateAtom::create (
        "generate-man-page", "gmp",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void xml2lyManPageOahGroup::initializeManPagetracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "musicxml2lilypondInsiderManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void xml2lyManPageOahGroup::initializeXml2lyManPageOahGroup (
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
void xml2lyManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2lyManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2lyManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2lyManPageOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2lyManPageOahGroup>*> (v)) {
        S_xml2lyManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2lyManPageOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2lyManPageOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2lyManPageOahGroup>*> (v)) {
        S_xml2lyManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2lyManPageOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2lyManPageOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void xml2lyManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_xml2lyManPageOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2lyManPageOahGroup createGlobalXml2lyManPageOahGroup (
  S_oahVisitor theOah2manPage)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global xml2lyManpage OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2lyManPageOahGroup) {
    // create the global options group
    gGlobalXml2lyManPageOahGroup =
      xml2lyManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalXml2lyManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2lyManPageOahGroup;
}


}
