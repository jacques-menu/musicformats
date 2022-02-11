/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...
#include <cmath>
#include <string>

#include <regex>

#include "bsrOah2manPage.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "oahOah.h"

using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
bsrOah2manPage::bsrOah2manPage (
  const S_oahHandler handler,
  ostream&           manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

bsrOah2manPage::~bsrOah2manPage ()
{}

//________________________________________________________________________
void bsrOah2manPage::visitStart (S_bsrFacSimileKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting bsrFacSimileKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void bsrOah2manPage::visitEnd (S_bsrFacSimileKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting bsrFacSimileKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsrOah2manPage::visitStart (S_bsrTextsLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting bsrTextsLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void bsrOah2manPage::visitEnd (S_bsrTextsLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting bsrTextsLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
