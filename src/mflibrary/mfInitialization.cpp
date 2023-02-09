/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfInitialization.h"
#include "languages.h"
#include "mfPasses.h"

#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
void initializeMusicFormats ()
{

  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef MF_TRACE_IS_ENABLED
    if (
      gEarlyOptions.getEarlyTraceOah ()
        &&
      ! gEarlyOptions.getEarlyQuietOption ()
    ) {
	  	std::stringstream ss;

      ss <<
        "Initializing MusicFormats" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

    // languages handling
    // ------------------------------------------------------

    initializeMusicFormatsLanguages ();

    initializeMusicFormatsPassIDs ();

    pPrivateThisMethodHasBeenRun = true;
  }
}


}

