/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "enableHarmoniesExtraOahIfDesired.h"

#ifdef EXTRA_OAH_IS_ENABLED

#include <iomanip>      // setw, setprecision, ...
#include <cmath>
#include <string>

#include "harmoniesExtraOah2manPage.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "oahOah.h"

using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
harmoniesExtraOah2manPage::harmoniesExtraOah2manPage (
  const S_oahHandler handler,
  ostream&           manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

harmoniesExtraOah2manPage::~harmoniesExtraOah2manPage ()
{}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraShowAllHarmoniesStructuresAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting extraShowAllHarmoniesStructuresAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
    */
}

void harmoniesExtraOah2manPage::visitEnd (S_extraShowAllHarmoniesStructuresAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting extraShowAllHarmoniesStructuresAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraShowAllHarmoniesContentsAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting extraShowAllHarmoniesContentsAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
*/
}

void harmoniesExtraOah2manPage::visitEnd (S_extraShowAllHarmoniesContentsAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting extraShowAllHarmoniesContentsAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraShowHarmonyDetailsAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting extraShowHarmonyDetailsAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
    */
}

void harmoniesExtraOah2manPage::visitEnd (S_extraShowHarmonyDetailsAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting extraShowHarmonyDetailsAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraShowHarmonyAnalysisAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting extraShowHarmonyAnalysisAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
    */
}

void harmoniesExtraOah2manPage::visitEnd (S_extraShowHarmonyAnalysisAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting extraShowHarmonyAnalysisAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace


#endif
