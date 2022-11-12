/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "oahEarlyOptions.h"

#include "musicxml2brailleManPageOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_xml2brlManPageGenerateAtom xml2brlManPageGenerateAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  S_oahVisitor  theOahVisitor)
{
  xml2brlManPageGenerateAtom* o = new
    xml2brlManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

xml2brlManPageGenerateAtom::xml2brlManPageGenerateAtom (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  S_oahVisitor  theOahVisitor)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{
  fOahVisitor = theOahVisitor;
}

xml2brlManPageGenerateAtom::~xml2brlManPageGenerateAtom ()
{}

void xml2brlManPageGenerateAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a xml2brlManPageGenerateAtom" <<
      std::endl;
  }
#endif

  generateManPageData (os);
}

void xml2brlManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageGenerateAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_xml2brlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageGenerateAtom>*> (v)) {
        S_xml2brlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlManPageGenerateAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageGenerateAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_xml2brlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageGenerateAtom>*> (v)) {
        S_xml2brlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlManPageGenerateAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageGenerateAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void xml2brlManPageGenerateAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2brlManPageGenerateAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void xml2brlManPageGenerateAtom::generateManPageData (std::ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomUpLinkToHandler ()->
    setOahHandlerFoundAHelpOption (
      "xml2brlManPageGenerateAtom::generateManPageData");
}

void xml2brlManPageGenerateAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_xml2brlManPageGenerateAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }
  
  return os;
}

//_______________________________________________________________________________
S_xml2brlManPageOahGroup gGlobalXml2brlManPageOahGroup;

S_xml2brlManPageOahGroup xml2brlManPageOahGroup::create (
  S_oahVisitor theOah2manPage)
{
  xml2brlManPageOahGroup* o =
    new xml2brlManPageOahGroup (
      theOah2manPage);
  assert (o!=0);
  return o;
}

xml2brlManPageOahGroup::xml2brlManPageOahGroup (
  S_oahVisitor theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2brlManPageOahGroup (
    theOah2manPage);
}

xml2brlManPageOahGroup::~xml2brlManPageOahGroup ()
{}

void xml2brlManPageOahGroup::initializeManPageGenerateOptions (
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
      xml2brlManPageGenerateAtom::create (
        "generate-man-page", "gmp",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          std::regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        fOah2manPage));
}

#ifdef TRACING_IS_ENABLED
void xml2brlManPageOahGroup::initializeManPagetracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "xml2brlManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void xml2brlManPageOahGroup::initializeXml2brlManPageOahGroup (
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
void xml2brlManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2brlManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2brlManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_xml2brlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageOahGroup>*> (v)) {
        S_xml2brlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlManPageOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_xml2brlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlManPageOahGroup>*> (v)) {
        S_xml2brlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlManPageOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlManPageOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void xml2brlManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    std::endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_xml2brlManPageOahGroup& elt)
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
S_xml2brlManPageOahGroup createGlobalXml2brlManPageOahHandler (
  S_oahVisitor theOah2manPage)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global xml2blrManPage OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2brlManPageOahGroup) {
    // create the global options group
    gGlobalXml2brlManPageOahGroup =
      xml2brlManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalXml2brlManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2brlManPageOahGroup;
}


}
