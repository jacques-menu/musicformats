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

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "oahEarlyOptions.h"

#include "msr2musicxmlManPageOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msr2musicxmlManPageGenerateAtom msr2musicxmlManPageGenerateAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const S_oahVisitor&  theOahVisitor)
{
  msr2musicxmlManPageGenerateAtom* o = new
    msr2musicxmlManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msr2musicxmlManPageGenerateAtom::msr2musicxmlManPageGenerateAtom (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const S_oahVisitor&  theOahVisitor)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{
  fOahVisitor = theOahVisitor;
}

msr2musicxmlManPageGenerateAtom::~msr2musicxmlManPageGenerateAtom ()
{}

void msr2musicxmlManPageGenerateAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msr2musicxmlManPageGenerateAtom" <<
      std::endl;
  }
#endif

  generateManPageData (os);
}

void msr2musicxmlManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlManPageGenerateAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2musicxmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2musicxmlManPageGenerateAtom>*> (v)) {
        S_msr2musicxmlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2musicxmlManPageGenerateAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2musicxmlManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlManPageGenerateAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2musicxmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2musicxmlManPageGenerateAtom>*> (v)) {
        S_msr2musicxmlManPageGenerateAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2musicxmlManPageGenerateAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2musicxmlManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlManPageGenerateAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void msr2musicxmlManPageGenerateAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msr2musicxmlManPageGenerateAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msr2musicxmlManPageGenerateAtom::generateManPageData (std::ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomUpLinkToHandler ()->
    setOahHandlerFoundAHelpOption (
      "msr2musicxmlManPageGenerateAtom::generateManPageData");
}

void msr2musicxmlManPageGenerateAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_msr2musicxmlManPageGenerateAtom& elt)
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
S_msr2musicxmlManPageOahGroup gGlobalMsr2musicxmlManPageOahGroup;

S_msr2musicxmlManPageOahGroup msr2musicxmlManPageOahGroup::create (
  const S_oahVisitor& theOah2manPage)
{
  msr2musicxmlManPageOahGroup* o = new msr2musicxmlManPageOahGroup (
    theOah2manPage);
  assert (o!=0);
  return o;
}

msr2musicxmlManPageOahGroup::msr2musicxmlManPageOahGroup (
  const S_oahVisitor& theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2musicxmlManPageOahGroup (
    theOah2manPage);
}

msr2musicxmlManPageOahGroup::~msr2musicxmlManPageOahGroup ()
{}

void msr2musicxmlManPageOahGroup::initializeManPageGenerateOptions (
  const S_oahVisitor& theOah2manPage)
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
      msr2musicxmlManPageGenerateAtom::create (
        "generate-man-page", "gmp",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          std::regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        fOahVisitor));
}

#ifdef TRACING_IS_ENABLED
void msr2musicxmlManPageOahGroup::initializeManPagetracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msr2musicxmlManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msr2musicxmlManPageOahGroup::initializeMsr2musicxmlManPageOahGroup (
  const S_oahVisitor& theOah2manPage)
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
void msr2musicxmlManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msr2musicxmlManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msr2musicxmlManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlManPageOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2musicxmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2musicxmlManPageOahGroup>*> (v)) {
        S_msr2musicxmlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2musicxmlManPageOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2musicxmlManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlManPageOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2musicxmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2musicxmlManPageOahGroup>*> (v)) {
        S_msr2musicxmlManPageOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2musicxmlManPageOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2musicxmlManPageOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2musicxmlManPageOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void msr2musicxmlManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    std::endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msr2musicxmlManPageOahGroup& elt)
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
S_msr2musicxmlManPageOahGroup createGlobalMsr2musicxmlManPageOahGroupHandler (
  const S_oahVisitor& theOah2manPage)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2musicxmlManPage OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2musicxmlManPageOahGroup) {
    // create the global options group
    gGlobalMsr2musicxmlManPageOahGroup =
      msr2musicxmlManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalMsr2musicxmlManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2musicxmlManPageOahGroup;
}


}
