/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2xmlTranslatorInterface___
#define ___mxsr2xmlTranslatorInterface___


#include <iostream>

#include "exports.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP void translateMxsrToMusicXML (
  Sxmlelement   theMxsr,
  std::string        outputFileName,
  std::ostream&      err,
  const std::string& passNumber,
  const std::string& passDescription);


}


#endif
