/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2bsrInterface___
#define ___msr2bsrInterface___

#include "exports.h"

#include "msrOah.h"
#include "bsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
// type declarations for formal parameters

class   bsrScore;
typedef SMARTP<bsrScore> S_bsrScore;

//_______________________________________________________________________________
EXP S_bsrScore translateMsrToBsr (
  const S_msrScore&    originalMsrScore,
  const S_msrOahGroup& msrOpts,
  const S_bsrOahGroup& bsrOpts,
  mfPassIDKind         passIDKind,
  const std::string&   passDescription);


}


#endif // ___msr2bsrInterface___
