/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef __mnx2msrInterface___
#define __mnx2msrInterface___

#include <ostream>

#include "exports.h"


namespace MusicFormats
{

//_______________________________________________________________________________
EXP int mnx2msr (
  int   argc,
  char* argv[]);

//_______________________________________________________________________________
EXP int mnx2msr (
  FILE*         fd,
  std::ostream& out,
  std::ostream& err);

//_______________________________________________________________________________
EXP int mnx2msr (
  const char*   buffer,
  std::ostream& out,
  std::ostream& err);


}

#endif // __mnx2msrInterface___
