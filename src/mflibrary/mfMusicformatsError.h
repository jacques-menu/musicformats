/*
  MusicFormats Library
  Copyright (C) Grame 2006-2020

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicformatsError___
#define ___musicformatsError___

#include <string>

#include "exports.h"


namespace MusicFormats
{

enum class mfMusicformatsError {
  k_NoError, kErrorInvalidFile, kErrorInvalidOption, kErrorUnsupported
};

EXP std::string mfMusicFormatsErrorAsString (mfMusicformatsError err);


}


#endif
