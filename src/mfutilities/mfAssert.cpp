/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"

#include "mfExceptions.h"

#include "mfAssert.h"
#include "mfIndentedTextOutput.h"
#include "mfServices.h"


namespace MusicFormats
{

//______________________________________________________________________________
// assert

void mfAssert (
  const std::string& sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  Bool               condition,
  const std::string& messageIfFalse)
{
  if (! condition) {
    gIndenter.resetToZero ();

    gLog <<
      "#### mfAssert" <<
      ", " << sourceCodeFileName << ":" << sourceCodeLineNumber <<
      " -- ";

    if (gServiceRunData) {
      gLog <<
        "measure " <<
        gServiceRunData->getCurrentMeasureNumber () <<
        '/' <<
        gServiceRunData->getScoreMeasuresNumber () <<
        ", part " <<
        gServiceRunData->getCurrentPartIDAndName () <<
        ", staff " <<
        gServiceRunData->getCurrentStaveNumber () <<
        ", voice " <<
        gServiceRunData->getCurrentVoiceNumber () <<
        " - ";
    }

    gLog <<
      messageIfFalse <<
      " - quitting." <<
      std::endl << std::endl;

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

    throw mfAssertException (messageIfFalse);
  }
}

void mfAssertFalse (
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       messageIfFalse)
{
  mfAssert (
    sourceCodeFileName,
    sourceCodeLineNumber,
    false,
    messageIfFalse);
}

//______________________________________________________________________________
// assert with input location
void mfAssertWithInputLocation (
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  Bool                     condition,
  const std::string&       inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       messageIfFalse)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  if (! condition) {
    gIndenter.resetToZero ();

    gLog <<
      "#### mfAssert" <<
      ", " << inputSourceName << ":" << inputLineNumber <<
      ", " << sourceCodeFileName << ":" << sourceCodeLineNumber <<
      " -- ";

    if (gServiceRunData) {
      gLog <<
        "measure " <<
        gServiceRunData->getCurrentMeasureNumber () <<
        '/' <<
        gServiceRunData->getScoreMeasuresNumber () <<
        ", part " <<
        gServiceRunData->getCurrentPartIDAndName () <<
        ", staff " <<
        gServiceRunData->getCurrentStaveNumber () <<
        ", voice " <<
        gServiceRunData->getCurrentVoiceNumber () <<
        " - ";
    }

    gLog <<
      messageIfFalse <<
      " - quitting." <<
      std::endl << std::endl;

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

    throw mfAssertException (messageIfFalse);
  }
}


}
