/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsr2bsrFinalizerInterface___
#define ___bsr2bsrFinalizerInterface___

#include "exports.h"

#include "bsrOah.h"


namespace MusicFormats
{
//_______________________________________________________________________________
EXP S_bsrScore translateBsrToFinalizedBsr (
  const S_bsrScore     originalBsrScore,
  const S_bsrOahGroup& bsrOpts,
  mfPassIDKind         passIDKind,
  const std::string&   passDescription);

}


#endif
