/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// #include <iostream>
// #include <sstream>
// #include <cassert>
//
// #include <climits>      // INT_MIN, INT_MAX
// #include <iomanip>      // setw, ...
//
// #include <regex>
//
// #include "mfRational.h"
//
// #include "mfServiceRunData.h"
//
// #include "mfStringsHandling.h"
// #include "mfEnumAll.h"
//
// #include "oahWae.h"
// #include "msrWae.h"
//
// #include "enableTracingIfDesired.h"
// #ifdef TRACING_IS_ENABLED
//   #include "tracingOah.h"
// #endif
//
// #include "msrPitchesNames.h"
//
// #include "oahOah.h"
// #include "waeOah.h"
//
// #include "msrOah.h"
// #include "lpsrOah.h"
//
// #include "oahEarlyOptions.h"

#include "lpsrInitialization.h"


namespace MusicFormats
{

//______________________________________________________________________________
void initializeLPSR ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah () && ! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      gLogStream <<
        "Initializing LPSR basic types handling" <<
        endl;
    }
#endif

    // LPSR score output handling
    // ------------------------------------------------------

    initializeLpsrScoreOutputKindsMap ();

    // LPSR octave entry handling
    // ------------------------------------------------------

    initializeMsrOctaveEntryKindsMap ();

    // LPSR accidental styles handling
    // ------------------------------------------------------

    initializeLpsrAccidentalStyleKindsMap ();

    // LPSR chords languages handling
    // ------------------------------------------------------

    initializeLpsrChordsLanguageKindsMap ();

    // LPSR dynamic text spanners style kinds handling
    // ------------------------------------------------------

    initializeLpsrDynamicsTextSpannersStyleKindsMap ();

    // LPSR lyrics durations kinds handling
    // ------------------------------------------------------

    initializeLpsrLyricsDurationsKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}


}

