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

#include "mfStaticSettings.h"

#include "oahEarlyOptions.h"

#include "msdl2lilypondManPageOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdl2lilypondManPageGenerateAtom msdl2lilypondManPageGenerateAtom::create (
  const std::string&  shortName,
  const std::string&  longName,
  const std::string&  description,
  const S_oahVisitor& theOahVisitor)
{
  msdl2lilypondManPageGenerateAtom* obj = new
    msdl2lilypondManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (obj!=0);
  return obj;
}

msdl2lilypondManPageGenerateAtom::msdl2lilypondManPageGenerateAtom (
  const std::string&  shortName,
  const std::string&  longName,
  const std::string&  description,
  const S_oahVisitor& theOahVisitor)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{
  fOahVisitor = theOahVisitor;
}

msdl2lilypondManPageGenerateAtom::~msdl2lilypondManPageGenerateAtom ()
{}

void msdl2lilypondManPageGenerateAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a msdl2lilypondManPageGenerateAtom" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  generateManPageData (os);
}

void msdl2lilypondManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2lilypondManPageGenerateAtom::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2lilypondManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2lilypondManPageGenerateAtom>*> (v)) {
        S_msdl2lilypondManPageGenerateAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2lilypondManPageGenerateAtom::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdl2lilypondManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2lilypondManPageGenerateAtom::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2lilypondManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msdl2lilypondManPageGenerateAtom>*> (v)) {
        S_msdl2lilypondManPageGenerateAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2lilypondManPageGenerateAtom::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdl2lilypondManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2lilypondManPageGenerateAtom::browseData ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msdl2lilypondManPageGenerateAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msdl2lilypondManPageGenerateAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msdl2lilypondManPageGenerateAtom::generateManPageData (std::ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomUpLinkToHandler ()->
    setOahHandlerFoundAHelpOption (
      "msdl2lilypondManPageGenerateAtom::generateManPageData");
}

void msdl2lilypondManPageGenerateAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_msdl2lilypondManPageGenerateAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdl2lilypondManPageOahGroup gGlobalMsdl2lyManPageOahGroup;

S_msdl2lilypondManPageOahGroup msdl2lilypondManPageOahGroup::create (
  const S_oahVisitor& theOah2manPage)
{
  msdl2lilypondManPageOahGroup* obj = new msdl2lilypondManPageOahGroup (
    theOah2manPage);
  assert (obj!=0);
  return obj;
}

msdl2lilypondManPageOahGroup::msdl2lilypondManPageOahGroup (
  const S_oahVisitor& theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2lyManPageOahGroup (
    theOah2manPage);
}

msdl2lilypondManPageOahGroup::~msdl2lilypondManPageOahGroup ()
{}

void msdl2lilypondManPageOahGroup::initializeManPageGenerateOptions (
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
      msdl2lilypondManPageGenerateAtom::create (
        "generate-man-page", "gmanpage",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
        fOahVisitor));
}

#ifdef MF_TRACE_IS_ENABLED
void msdl2lilypondManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2lilypondManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif // MF_TRACE_IS_ENABLED

void msdl2lilypondManPageOahGroup::initializeMsdl2lyManPageOahGroup (
  const S_oahVisitor& theOah2manPage)
{
  // display
  // --------------------------------------
  initializeManPageGenerateOptions (
    theOah2manPage);

#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializeManPageTraceOah ();
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msdl2lilypondManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msdl2lilypondManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2lilypondManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2lilypondManPageOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2lilypondManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2lilypondManPageOahGroup>*> (v)) {
        S_msdl2lilypondManPageOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2lilypondManPageOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdl2lilypondManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2lilypondManPageOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2lilypondManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2lilypondManPageOahGroup>*> (v)) {
        S_msdl2lilypondManPageOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2lilypondManPageOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdl2lilypondManPageOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2lilypondManPageOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msdl2lilypondManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLog <<
    "The OAH options are:" <<
    std::endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msdl2lilypondManPageOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msdl2lilypondManPageOahGroup createGlobalMsdl2lyManPageOahGroup (
  const S_oahVisitor& theOah2manPage)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msdl2lilypondManpage OAH group" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMsdl2lyManPageOahGroup) {
    // create the global options group
    gGlobalMsdl2lyManPageOahGroup =
      msdl2lilypondManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalMsdl2lyManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2lyManPageOahGroup;
}


}
