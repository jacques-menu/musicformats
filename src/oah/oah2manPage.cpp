/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "oah2manPage.h"

#include "mfPreprocessorSettings.h"


#include "oahOah.h"


namespace MusicFormats
{

//________________________________________________________________________
oah2manPage::oah2manPage (
  const S_oahHandler& handler,
  std::ostream&       manPageOutputStream)
    : oahVisitor (
        handler),
      fManPageOutputStream (
        manPageOutputStream)
{}

oah2manPage::~oah2manPage ()
{}


} // namespace
