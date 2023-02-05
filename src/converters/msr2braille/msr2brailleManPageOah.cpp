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


#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "msr2brailleManPageOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msr2brailleManPageGenerateAtom msr2brailleManPageGenerateAtom::create (
  const std::string&  shortName,
  const std::string&  longName,
  const std::string&  description,
  const S_oahVisitor& theOahVisitor)
{
  msr2brailleManPageGenerateAtom* o = new
    msr2brailleManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (o!=0);
  return o;
}

msr2brailleManPageGenerateAtom::msr2brailleManPageGenerateAtom (
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

msr2brailleManPageGenerateAtom::~msr2brailleManPageGenerateAtom ()
{}

void msr2brailleManPageGenerateAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a msr2brailleManPageGenerateAtom" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  generateManPageData (os);
}

void msr2brailleManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msr2brailleManPageGenerateAtom::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msr2brailleManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2brailleManPageGenerateAtom>*> (v)) {
        S_msr2brailleManPageGenerateAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2brailleManPageGenerateAtom::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msr2brailleManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msr2brailleManPageGenerateAtom::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msr2brailleManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_msr2brailleManPageGenerateAtom>*> (v)) {
        S_msr2brailleManPageGenerateAtom elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2brailleManPageGenerateAtom::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2brailleManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msr2brailleManPageGenerateAtom::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif
}

void msr2brailleManPageGenerateAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msr2brailleManPageGenerateAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void msr2brailleManPageGenerateAtom::generateManPageData (std::ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomUpLinkToHandler ()->
    setOahHandlerFoundAHelpOption (
      "msr2brailleManPageGenerateAtom::generateManPageData");
}

void msr2brailleManPageGenerateAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_msr2brailleManPageGenerateAtom& elt)
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
S_msr2brailleManPageOahGroup gGlobalXml2brailleManPageOahGroup;

S_msr2brailleManPageOahGroup msr2brailleManPageOahGroup::create (
  const S_oahVisitor& theOah2manPage)
{
  msr2brailleManPageOahGroup* o =
    new msr2brailleManPageOahGroup (
      theOah2manPage);
  assert (o!=0);
  return o;
}

msr2brailleManPageOahGroup::msr2brailleManPageOahGroup (
  const S_oahVisitor& theOah2manPage)
  : oahGroup (
    "Man page",
    "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2brailleManPageOahGroup (
    theOah2manPage);
}

msr2brailleManPageOahGroup::~msr2brailleManPageOahGroup ()
{}

void msr2brailleManPageOahGroup::initializeManPageGenerateOptions (
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

  // generate man page
  fOah2manPage = theOah2manPage;

  subGroup->
    appendAtomToSubGroup (
      msr2brailleManPageGenerateAtom::create (
        "generate-man-page", "gmanpage",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          std::regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        fOah2manPage));
}

#ifdef MF_TRACING_IS_ENABLED
void msr2brailleManPageOahGroup::initializeManPagetracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msr2brailleManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif

void msr2brailleManPageOahGroup::initializeXml2brailleManPageOahGroup (
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
void msr2brailleManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void msr2brailleManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msr2brailleManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msr2brailleManPageOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msr2brailleManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2brailleManPageOahGroup>*> (v)) {
        S_msr2brailleManPageOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2brailleManPageOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msr2brailleManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msr2brailleManPageOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msr2brailleManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2brailleManPageOahGroup>*> (v)) {
        S_msr2brailleManPageOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2brailleManPageOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2brailleManPageOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msr2brailleManPageOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif
}

//______________________________________________________________________________
void msr2brailleManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLogStream <<
    "The OAH options are:" <<
    std::endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msr2brailleManPageOahGroup& elt)
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
S_msr2brailleManPageOahGroup createGlobalXml2brailleManPageOahHandler (
  const S_oahVisitor& theOah2manPage)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating global xml2blrManPage OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2brailleManPageOahGroup) {
    // create the global options group
    gGlobalXml2brailleManPageOahGroup =
      msr2brailleManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalXml2brailleManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2brailleManPageOahGroup;
}


}
