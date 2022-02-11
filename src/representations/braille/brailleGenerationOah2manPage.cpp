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

#include "brailleGenerationOah2manPage.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "oahOah.h"


using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
brailleGenerationOah2manPage::brailleGenerationOah2manPage (
  const S_oahHandler handler,
  ostream&           manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

brailleGenerationOah2manPage::~brailleGenerationOah2manPage ()
{}

//________________________________________________________________________
void brailleGenerationOah2manPage::visitStart (S_brailleOutputKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting brailleOutputKindAtom" <<
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

void brailleGenerationOah2manPage::visitEnd (S_brailleOutputKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting brailleOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void brailleGenerationOah2manPage::visitStart (S_brailleUTFKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting brailleUTFKindAtom" <<
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

void brailleGenerationOah2manPage::visitEnd (S_brailleUTFKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting brailleUTFKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void brailleGenerationOah2manPage::visitStart (S_brailleByteOrderingKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting brailleByteOrderingKindAtom" <<
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

void brailleGenerationOah2manPage::visitEnd (S_brailleByteOrderingKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting brailleByteOrderingKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
