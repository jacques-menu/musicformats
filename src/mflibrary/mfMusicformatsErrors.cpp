/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfMusicformatsErrors.h"


namespace MusicFormats
{

EXP std::string mfMusicFormatsErrorAsString (
  mfMusicformatsErrorKind err)
{
  std::string result;

  switch (err) {
    case mfMusicformatsErrorKind::kMusicformatsError_NO_:
      result = "no error";
      break;
    case mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile:
      result = "invalid file";
      break;
    case mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption:
      result = "invalid option";
      break;
    case mfMusicformatsErrorKind::kMusicformatsErrorUnsupported:
      result = "unsupported";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const mfMusicformatsErrorKind& elt)
{
  os << mfMusicFormatsErrorAsString (elt) << endl;
  return os;
}


}