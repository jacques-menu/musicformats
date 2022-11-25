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


namespace MusicFormats
{

//______________________________________________________________________________
#ifdef WIN32
/*
  isatty() is needed for Flex interactive mode,
  but doesn't follow the Standard C rules
  for implementation-specific names

  we should thus use _isatty() instead on Windows systems


  #include <stdio.h>
  #include <io.h>

  int main( void )
  {
     if( _isatty( _fileno( stdout ) ) )
        printf( "stdout has not been redirected to a file\n" );
     else
        printf( "stdout has been redirected to a file\n");
  }
*/

  #include "io.h" // for _isatty()

  // see https://stackoverflow.com/questions/341817/is-there-a-replacement-for-unistd-h-for-windows-visual-c
  #define isatty _isatty

/*
2022-11-24T08:40:38.8477149Z mfslScanner.ll(564,11): warning C4996: 'strerror': This function or variable may be unsafe. Consider using strerror_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details. [D:\a\musicformats\musicformats\build\libdir\libmusicformats.vcxproj]
*/

//   #define strerror strerror_s // needs 2 arguments...

#endif

//______________________________________________________________________________
EXP int mfExecuteCommand (
  const std::string& command,
  Bool          doTrace);


}


#endif
