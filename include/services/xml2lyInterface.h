/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef __xml2lyInterface___
#define __xml2lyInterface___

#include <ostream>

#include "exports.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
EXP int xml2ly (
  int   argc,
  char* argv[]);

//_______________________________________________________________________________
EXP int xml2ly (
  FILE*    fd,
  ostream& out,
  ostream& err);

//_______________________________________________________________________________
EXP int xml2ly (
  const char* buffer,
  ostream&    out,
  ostream&    err);


}

#endif