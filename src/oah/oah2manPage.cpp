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

#include "oah2manPage.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "oahOah.h"

using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
oah2manPage::oah2manPage (
  const S_oahHandler handler,
  ostream&           manPageOutputStream)
    : oahVisitor (
        handler),
      fManPageOutputStream (
        manPageOutputStream)
{}

oah2manPage::~oah2manPage ()
{}


} // namespace
