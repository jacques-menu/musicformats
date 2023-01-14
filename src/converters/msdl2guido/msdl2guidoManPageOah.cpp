/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "mfEnableTracingSetting.h"


#include "oahEarlyOptions.h"

#include "msdl2guidoManPageOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdl2guidoManPageGenerateAtom msdl2guidoManPageGenerateAtom::create (
  const std::string& shortName,
  const std::string& longName,
  const std::string& description,
  const S_oahVisitor&  theOahVisitor)
{
  msdl2guidoManPageGenerateAtom* o = new
    msdl2guidoManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msdl2guidoManPageGenerateAtom::msdl2guidoManPageGenerateAtom (
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

msdl2guidoManPageGenerateAtom::~msdl2guidoManPageGenerateAtom ()
{}

void msdl2guidoManPageGenerateAtom::applyElement (std::ostream& os)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a msdl2guidoManPageGenerateAtom" <<
      std::endl;
  }
#endif

  generateManPageData (os);
}

void msdl2guidoManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2guidoManPageGenerateAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdl2guidoManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2guidoManPageGenerateAtom>*> (v)) {
        S_msdl2guidoManPageGenerateAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2guidoManPageGenerateAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2guidoManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2guidoManPageGenerateAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdl2guidoManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2guidoManPageGenerateAtom>*> (v)) {
        S_msdl2guidoManPageGenerateAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2guidoManPageGenerateAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2guidoManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2guidoManPageGenerateAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void msdl2guidoManPageGenerateAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdl2guidoManPageGenerateAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msdl2guidoManPageGenerateAtom::generateManPageData (std::ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomUpLinkToHandler ()->
    setOahHandlerFoundAHelpOption (
      "msdl2guidoManPageGenerateAtom::generateManPageData");
}

void msdl2guidoManPageGenerateAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_msdl2guidoManPageGenerateAtom& elt)
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
S_msdl2guidoManPageOahGroup gGlobalMsdl2gmnManPageOahGroup;

S_msdl2guidoManPageOahGroup msdl2guidoManPageOahGroup::create (
  const S_oahVisitor& theOah2manPage)
{
  msdl2guidoManPageOahGroup* o = new msdl2guidoManPageOahGroup (
    theOah2manPage);
  assert (o!=0);
  return o;
}

msdl2guidoManPageOahGroup::msdl2guidoManPageOahGroup (
  const S_oahVisitor& theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2gmnManPageOahGroup (
    theOah2manPage);
}

msdl2guidoManPageOahGroup::~msdl2guidoManPageOahGroup ()
{}

void msdl2guidoManPageOahGroup::initializeManPageGenerateOptions (
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
      msdl2guidoManPageGenerateAtom::create (
        "generate-man-page", "gmanpage",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          std::regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        fOahVisitor));
}

#ifdef MF_TRACING_IS_ENABLED
void msdl2guidoManPageOahGroup::initializeManPagetracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2guidoManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msdl2guidoManPageOahGroup::initializeMsdl2gmnManPageOahGroup (
  const S_oahVisitor& theOah2manPage)
{
  // display
  // --------------------------------------
  initializeManPageGenerateOptions (
    theOah2manPage);

#ifdef MF_TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeManPagetracingOah ();
#endif
}

//______________________________________________________________________________
void msdl2guidoManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msdl2guidoManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2guidoManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2guidoManPageOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdl2guidoManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2guidoManPageOahGroup>*> (v)) {
        S_msdl2guidoManPageOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2guidoManPageOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2guidoManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2guidoManPageOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msdl2guidoManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2guidoManPageOahGroup>*> (v)) {
        S_msdl2guidoManPageOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2guidoManPageOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2guidoManPageOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2guidoManPageOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void msdl2guidoManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    std::endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msdl2guidoManPageOahGroup& elt)
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
S_msdl2guidoManPageOahGroup createGlobalMsdl2gmnManPageOahGroupHandler (
  const S_oahVisitor& theOah2manPage)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msdl2guidoManPage OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2gmnManPageOahGroup) {
    // create the global options group
    gGlobalMsdl2gmnManPageOahGroup =
      msdl2guidoManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalMsdl2gmnManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2gmnManPageOahGroup;
}


}
