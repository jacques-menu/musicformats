/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrVoiceConstants___
#define ___msrVoiceConstants___

#include <string>

#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
// hidden global null voice variable
extern S_msrVoice pGlobalNullVoice;

// a handy shortcut
#define gNullVoice pGlobalNullVoice


}


#endif // ___msrVoiceConstants___
