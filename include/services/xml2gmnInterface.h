/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef __xml2gmnInterface___
#define __xml2gmnInterface___

#include <ostream>

#include "exports.h"


namespace MusicFormats
{

//_______________________________________________________________________________
EXP int xml2gmn (
  int   argc,
  char* argv[]);

//_______________________________________________________________________________
EXP int xml2gmn (
  FILE*         fd,
  std::ostream& out,
  std::ostream& err);

//_______________________________________________________________________________
EXP int xml2gmn (
  const char*   buffer,
  std::ostream& out,
  std::ostream& err);


}

#endif // __xml2gmnInterface___
