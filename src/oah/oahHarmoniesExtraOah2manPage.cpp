/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"


#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "mfConstants.h"

#include "oahHarmoniesExtraOah2manPage.h"

#include "oahOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
harmoniesExtraOah2manPage::harmoniesExtraOah2manPage (
  const S_oahHandler& handler,
  std::ostream&       manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

harmoniesExtraOah2manPage::~harmoniesExtraOah2manPage ()
{}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraDisplayAllHarmoniesStructuresAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting extraDisplayAllHarmoniesStructuresAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
/*
  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
    */
}

void harmoniesExtraOah2manPage::visitEnd (S_extraDisplayAllHarmoniesStructuresAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting extraDisplayAllHarmoniesStructuresAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraDisplayAllHarmoniesContentsAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting extraDisplayAllHarmoniesContentsAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
/*
  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
*/
}

void harmoniesExtraOah2manPage::visitEnd (S_extraDisplayAllHarmoniesContentsAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting extraDisplayAllHarmoniesContentsAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraDisplayHarmonyDetailsAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting extraDisplayHarmonyDetailsAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
/*
  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
    */
}

void harmoniesExtraOah2manPage::visitEnd (S_extraDisplayHarmonyDetailsAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting extraDisplayHarmonyDetailsAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraDisplayHarmonyAnalysisAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting extraDisplayHarmonyAnalysisAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
/*
  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
    */
}

void harmoniesExtraOah2manPage::visitEnd (S_extraDisplayHarmonyAnalysisAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting extraDisplayHarmonyAnalysisAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace


