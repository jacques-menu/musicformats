/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsr2lilypondInterface___
#define ___lpsr2lilypondInterface___


#include "exports.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP void translateLpsrToLilypond (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  const std::string&     passNumber,
  const std::string&     passDescription,
  std::ostream&          lilypondCodeStream);

EXP void translateLpsrToLilypondWithHandler (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  const std::string&     passNumber,
  const std::string&     passDescription,
  S_oahHandler      handler,
  std::ostream&          out,
  std::ostream&          err);


}


#endif
