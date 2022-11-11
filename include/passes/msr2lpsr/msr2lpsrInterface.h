/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2lpsrInterface___
#define ___msr2lpsrInterface___

#include "exports.h"

#include "msrTypesForwardDeclarations.h"
#include "lpsrTypesForwardDeclarations.h"

#include "msrOah.h"
#include "lpsrOah.h"


namespace MusicFormats
{
//_______________________________________________________________________________
EXP extern S_lpsrScore translateMsrToLpsr (
  S_msrScore          originalMsrScore,
  S_msrOahGroup       msrOpts,
  S_lpsrOahGroup      lpsrOpts,
  string              passNumber,
  string              passDescription,
  S_mfcMultiComponent multiComponent);


}


#endif
