/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfLibraryComponent__
#define ___mfLibraryComponent__

#include "exports.h"

#include "mfcBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________

EXP extern std::string getGlobalMusicFormatsVersionNumber ();
EXP extern std::string getGlobalMusicFormatsVersionDate ();

EXP extern std::string getGlobalMusicFormatsVersionNumberAndDate ();

//______________________________________________________________________________
S_mfcLibraryComponent EXP createLibraryComponent ();


}


#endif // ___mfLibraryComponent__

