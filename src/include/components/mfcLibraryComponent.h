/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfcLibraryComponent__
#define ___mfcLibraryComponent__

#include "exports.h"

#include "mfcBasicTypes.h"


using namespace std;

namespace MusicFormats
{
//______________________________________________________________________________

EXP extern string getGlobalMusicFormatsVersionNumber ();
EXP extern string getGlobalMusicFormatsVersionDate ();

EXP extern string getGlobalMusicFormatsVersionNumberAndDate ();

//______________________________________________________________________________
S_mfcLibraryComponent EXP createLibraryComponent ();


}


#endif

