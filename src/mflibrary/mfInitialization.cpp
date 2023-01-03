/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfInitialization.h"
#include "mfLanguages.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
void initializeMusicFormats ()
{

  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef OAH_TRACING_IS_ENABLED
    if (
      gGlobalOahEarlyOptions.getEarlyTracingOah ()
        &&
      ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
    ) {
      gLogStream <<
        "Initializing MusicFormats" <<
        std::endl;
    }
#endif

    // languages handling
    // ------------------------------------------------------

    initializeMusicFormatsLanguages ();

    pPrivateThisMethodHasBeenRun = true;
  }
}


}

