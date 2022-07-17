/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfSystemInterface___
#define ___mfSystemInterface___

#include "exports.h"

#include "mfBool.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
#ifdef WIN32
EXP "C" int isatty (int fd);
#endif

//______________________________________________________________________________
EXP int mfExecuteCommand (
  const string& command,
  Bool          doTrace);


}


#endif
