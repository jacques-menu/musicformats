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

#include "msr2lilypondManPageOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msr2lilypondManPageGenerateAtom msr2lilypondManPageGenerateAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  S_oahVisitor  theOahVisitor)
{
  msr2lilypondManPageGenerateAtom* o = new
    msr2lilypondManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msr2lilypondManPageGenerateAtom::msr2lilypondManPageGenerateAtom (
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

msr2lilypondManPageGenerateAtom::~msr2lilypondManPageGenerateAtom ()
{}

void msr2lilypondManPageGenerateAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msr2lilypondManPageGenerateAtom" <<
      std::endl;
  }
#endif

  generateManPageData (os);
}

void msr2lilypondManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lilypondManPageGenerateAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2lilypondManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2lilypondManPageGenerateAtom>*> (v)) {
        S_msr2lilypondManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lilypondManPageGenerateAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2lilypondManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lilypondManPageGenerateAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2lilypondManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2lilypondManPageGenerateAtom>*> (v)) {
        S_msr2lilypondManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lilypondManPageGenerateAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2lilypondManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lilypondManPageGenerateAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void msr2lilypondManPageGenerateAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msr2lilypondManPageGenerateAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msr2lilypondManPageGenerateAtom::generateManPageData (std::ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomUpLinkToHandler ()->
    setOahHandlerFoundAHelpOption (
      "msr2lilypondManPageGenerateAtom::generateManPageData");
}

void msr2lilypondManPageGenerateAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_msr2lilypondManPageGenerateAtom& elt)
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
S_msr2lilypondManPageOahGroup gGlobalMsr2lilypondManPageOahGroup;

S_msr2lilypondManPageOahGroup msr2lilypondManPageOahGroup::create (
  S_oahVisitor theOah2manPage)
{
  msr2lilypondManPageOahGroup* o = new msr2lilypondManPageOahGroup (
    theOah2manPage);
  assert (o!=0);
  return o;
}

msr2lilypondManPageOahGroup::msr2lilypondManPageOahGroup (
  S_oahVisitor theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2lilypondManPageOahGroup (
    theOah2manPage);
}

msr2lilypondManPageOahGroup::~msr2lilypondManPageOahGroup ()
{}

void msr2lilypondManPageOahGroup::initializeManPageGenerateOptions (
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
      msr2lilypondManPageGenerateAtom::create (
        "generate-man-page", "gmp",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          std::regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void msr2lilypondManPageOahGroup::initializeManPagetracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msr2lilypondManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msr2lilypondManPageOahGroup::initializeMsr2lilypondManPageOahGroup (
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
void msr2lilypondManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msr2lilypondManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msr2lilypondManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lilypondManPageOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2lilypondManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2lilypondManPageOahGroup>*> (v)) {
        S_msr2lilypondManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lilypondManPageOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2lilypondManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lilypondManPageOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2lilypondManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2lilypondManPageOahGroup>*> (v)) {
        S_msr2lilypondManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lilypondManPageOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2lilypondManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lilypondManPageOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void msr2lilypondManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    std::endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msr2lilypondManPageOahGroup& elt)
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
S_msr2lilypondManPageOahGroup createGlobalMsr2lilypondManPageOahGroup (
  S_oahVisitor theOah2manPage)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2lilypondManpage OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2lilypondManPageOahGroup) {
    // create the global options group
    gGlobalMsr2lilypondManPageOahGroup =
      msr2lilypondManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalMsr2lilypondManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2lilypondManPageOahGroup;
}


}
