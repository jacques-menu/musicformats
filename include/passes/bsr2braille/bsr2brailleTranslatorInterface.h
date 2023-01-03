/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsr2brailleTranslatorInterface___
#define ___bsr2brailleTranslatorInterface___

#include "exports.h"

#include "bsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP void translateBsrToBraille (
  const S_bsrScore originalBsrScore,
  const S_bsrOahGroup&    bsrOpts,
  const std::string&    passNumber,
  const std::string&    passDescription,
  std::ostream&         brailleOutputStream);


}


#endif
