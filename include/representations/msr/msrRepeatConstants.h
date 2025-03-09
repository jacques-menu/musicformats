/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrRepeatConstants___
#define ___msrRepeatConstants___

#include "msrRepeats.h"


namespace MusicFormats
{

//______________________________________________________________________________
// hidden global null repeat variable
extern S_msrRepeat pGlobalNullRepeat;

// extern EXP S_msrRepeat getGlobalNullRepeat ();

// a handy shortcut
// #define gNullRepeat getGlobalNullRepeat ()
#define gNullRepeat pGlobalNullRepeat


}


#endif // ___msrRepeatConstants___
