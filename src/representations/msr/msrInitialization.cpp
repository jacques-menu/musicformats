/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrClefsKeysTimeSignatures.h"
#include "msrHarmonies.h"
#include "msrInitialization.h"
#include "msrLengths.h"
#include "msrMargins.h"
#include "msrPitchesNames.h"

#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
void initializeMSR ()
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
        "Initializing MSR" <<
        std::endl;

      gWaeHandler->waeTraceWithLocationDetails (
        __FILE__, __LINE__,
        ss.str ());
//         gServiceRunData->getCurrentMeasureNumber (),
//         gServiceRunData->getScoreMeasuresNumber ());
    }
#endif // MF_TRACE_IS_ENABLED

    // languages handling
    // ------------------------------------------------------

    initializeQuarterTonesPitchesLanguageKinds ();

    // clefs handling
    // ------------------------------------------------------

    initializeClefKinds ();

    // harmonies handling
    // ------------------------------------------------------

    initializeHarmonyKinds ();

    // harmony structures handling
    // ------------------------------------------------------

    initializeHarmonyStructuresMap ();

    // MSR lengths handling
    // ------------------------------------------------------

    initializeMsrLengthUnitKindsMap ();

    // MSR margins types handling
    // ------------------------------------------------------

    initializeMsrMarginTypeKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}


}

