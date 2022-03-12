/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfMusicformatsError.h"


namespace MusicFormats
{

EXP std::string mfMusicFormatsErrorAsString (mfMusicformatsError err)
{
  std::string result;

  switch (err) {
    case mfMusicformatsError::k_NoError:
      result = "no error";
      break;
    case mfMusicformatsError::kErrorInvalidFile:
      result = "invalid file";
      break;
    case mfMusicformatsError::kErrorInvalidOption:
      result = "invalid option";
      break;
    case mfMusicformatsError::kErrorUnsupported:
      result = "unsupported";
      break;
  } // switch

  return result;
}

ostream& operator<< (ostream& os, mfMusicformatsError& elt)
{
  os << mfMusicFormatsErrorAsString (elt) << endl;
  return os;
}


}
