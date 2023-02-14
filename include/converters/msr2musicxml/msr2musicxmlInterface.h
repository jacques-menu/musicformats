/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2musicxmlInterface___
#define ___msr2musicxmlInterface___

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "oahBasicTypes.h"


namespace MusicFormats
{
/*
  The API functions with an options and arguments and no handler
  are declared in libmusicxml.h
*/

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msrScore2musicxmlWithHandler (
  S_msrScore          theMsrScore,
  mfPassIDKind        passID_A,
  std::string         passDescription_A,
  mfPassIDKind        passID_B,
  std::string         passDescription_B,
  mfPassIDKind        passID_C,
  std::string         passDescription_C,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler);


}


#endif // ___msr2musicxmlInterface___
