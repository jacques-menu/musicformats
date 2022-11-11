/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrTranslatorInterface___
#define ___mxsr2msrTranslatorInterface___


#include <iostream>

#include "exports.h"



#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP void populateMsrSkeletonFromMxsr (
  Sxmlelement   theMxsr,
  S_msrScore    scoreSkeletonToBePopulated,
  const string& passNumber,
  const string& passDescription);


}


#endif
