/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "mfPreprocessorSettings.h"

#include "oahConstants.h"
#include "oahEarlyOptions.h"

#include "musicxml2musicxmlManPageOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_xml2xmlManPageGenerateAtom xml2xmlManPageGenerateAtom::create (
  const std::string&  shortName,
  const std::string&  longName,
  const std::string&  description,
  const S_oahVisitor& theOahVisitor)
{
  xml2xmlManPageGenerateAtom* obj = new
    xml2xmlManPageGenerateAtom (
      longName,
      shortName,
      description,
      theOahVisitor);
  assert (obj!=0);
  return obj;
}

xml2xmlManPageGenerateAtom::xml2xmlManPageGenerateAtom (
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

xml2xmlManPageGenerateAtom::~xml2xmlManPageGenerateAtom ()
{}

void xml2xmlManPageGenerateAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a xml2xmlManPageGenerateAtom";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  generateManPageData (os);
}

void xml2xmlManPageGenerateAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2xmlManPageGenerateAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_xml2xmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlManPageGenerateAtom>*> (v)) {
        S_xml2xmlManPageGenerateAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2xmlManPageGenerateAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void xml2xmlManPageGenerateAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2xmlManPageGenerateAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_xml2xmlManPageGenerateAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlManPageGenerateAtom>*> (v)) {
        S_xml2xmlManPageGenerateAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2xmlManPageGenerateAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void xml2xmlManPageGenerateAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2xmlManPageGenerateAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void xml2xmlManPageGenerateAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2xmlManPageGenerateAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void xml2xmlManPageGenerateAtom::generateManPageData (std::ostream& os) const
{
  // generate the man page from the OAH handler
  fOahVisitor->visitTheHandler ();

  // a help option has been applied // JMI superflous?
  fetchAtomUpLinkToHandler ()->
    setOahHandlerFoundAHelpOption (
      "xml2xmlManPageGenerateAtom::generateManPageData");
}

void xml2xmlManPageGenerateAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_xml2xmlManPageGenerateAtom& elt)
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
S_xml2xmlManPageOahGroup gGlobalXml2xmlManPageOahGroup;

S_xml2xmlManPageOahGroup xml2xmlManPageOahGroup::create (
  const S_oahVisitor& theOah2manPage)
{
  xml2xmlManPageOahGroup* obj = new xml2xmlManPageOahGroup (
    theOah2manPage);
  assert (obj!=0);
  return obj;
}

xml2xmlManPageOahGroup::xml2xmlManPageOahGroup (
  const S_oahVisitor& theOah2manPage)
  : oahGroup (
      "Man page",
      "hmp", "help-man-page",
R"(These options control the way the man page is generated.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeXml2xmlManPageOahGroup (
    theOah2manPage);
}

xml2xmlManPageOahGroup::~xml2xmlManPageOahGroup ()
{}

void xml2xmlManPageOahGroup::initializeManPageGenerateOptions (
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
      xml2xmlManPageGenerateAtom::create (
        "generate-man-page", "gmanpage",
        regex_replace (
R"(Write man page data for EXECUTABLE to standard output.)",
          std::regex ("EXECUTABLE"),
          gOahOahGroup->getOahOahGroupServiceName ()),
        fOahVisitor));
}

#ifdef MF_TRACE_IS_ENABLED
void xml2xmlManPageOahGroup::initializeManPageTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "xml2xmlManPage Trace",
        "help-man-page-trace", "hmpt",
R"(  Note: the options in this group imply '-tmp, -trace-man-page'.)",
// JMI        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // JMI ???
}
#endif // MF_TRACE_IS_ENABLED

void xml2xmlManPageOahGroup::initializeXml2xmlManPageOahGroup (
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
void xml2xmlManPageOahGroup::enforceGroupQuietness ()
{
// JMI  fDisplayOah = false;
}

//______________________________________________________________________________
void xml2xmlManPageOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void xml2xmlManPageOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2xmlManPageOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_xml2xmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2xmlManPageOahGroup>*> (v)) {
        S_xml2xmlManPageOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2xmlManPageOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void xml2xmlManPageOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2xmlManPageOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_xml2xmlManPageOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2xmlManPageOahGroup>*> (v)) {
        S_xml2xmlManPageOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching xml2xmlManPageOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void xml2xmlManPageOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> xml2xmlManPageOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void xml2xmlManPageOahGroup::printManPageOahValues (int fieldWidth)
{
  gLog <<
    "The OAH options are:" <<
    std::endl;

  ++gIndenter;

  // JMI ???

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_xml2xmlManPageOahGroup& elt)
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
S_xml2xmlManPageOahGroup createGlobalXml2xmlManPageOahGroupHandler (
  const S_oahVisitor& theOah2manPage)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global xml2xmlManPage OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalXml2xmlManPageOahGroup) {
    // create the global options group
    gGlobalXml2xmlManPageOahGroup =
      xml2xmlManPageOahGroup::create (
        theOah2manPage);
    assert (gGlobalXml2xmlManPageOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalXml2xmlManPageOahGroup;
}


}
