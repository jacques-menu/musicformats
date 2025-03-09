/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "bsrEnumTypes.h"

#include "oahEarlyOptions.h"

#include "bsrInitialization.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
void initializeBSR ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef MF_TRACE_IS_ENABLED
    if (
      gEarlyOptions.getTraceEarlyOptions ()
        &&
      ! gEarlyOptions.getEarlyQuietOption ()
    ) {
      std::stringstream ss;

      ss <<
        "Initializing BSR";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // BSR braille output kinds handling
    // ------------------------------------------------------

    initializeBsrBrailleOutputKindsMap ();

    // BSR texts languages handling
    // ------------------------------------------------------

    initializeBsrTextsLanguageKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}


}

