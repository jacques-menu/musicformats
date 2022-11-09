/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include "msrClefs.h"
#include "msrHarmonies.h"
#include "msrLengths.h"
#include "msrMargins.h"
#include "msrPitchesNames.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void initializeMSR ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (
      gGlobalOahEarlyOptions.getEarlyTracingOah ()
        &&
      ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
    ) {
      gLogStream <<
        "Initializing MSR " <<
        endl;
    }
#endif

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

/* JMI
#include <algorithm>    // for_each

    // Iterate over a map using std::for_each and Lambda function
    for_each (
      getQuarterTonesPitchesLanguageKindsMap ().begin(),
      getQuarterTonesPitchesLanguageKindsMap ().end(),
        [] (pair<string, getQuarterTonesPitchesLanguageKindsMap ()> element) {
          string theString = (element).first;

          ++count;

          cumulatedLength += theString.size ();
          if (cumulatedLength >= namesListMaxLength) {
            s << endl << gIndenter.getSpacer ();
            cumulatedLength = 0;
          }

          s << theString;

          if (count == nextToLast) {
            s << " and ";
          }
          else {
            s << ", ";
          }
        }
    ); // foreach
*/

