/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___ischemeInterpreterInterface___
#define ___ischemeInterpreterInterface___

#include "exports.h"

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "mfBool.h"

#include "oahBasicTypes.h"


using namespace MusicFormats;

/*
  CAUTION:
    DONT't put these interface functions in the MusicFormats namespace:
    there would be no way to 'see' them from outside ischemeScanner.cpp
*/

EXP extern mfMusicformatsErrorKind launchIschemeInterpreter ();


#endif // ___ischemeInterpreterInterface___
