/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr___
#define ___msr___

#include "msrPitchesNames.h"
#include "msrBasicTypes.h"
#include "msrElements.h"

#include "msrIdentification.h"

#include "msrMusicXMLSpecifics.h"
#include "msrScaling.h"

#include "msrCredits.h"

#include "msrDivisions.h"

#include "msrStavesDetails.h"

#include "msrClefs.h"
#include "msrKeys.h"
#include "msrTimeSignatures.h"

#include "msrTempos.h"

#include "msrRehearsalMarks.h"

#include "msrWords.h"

#include "msrTranspositions.h"

#include "msrBreaks.h"

#include "msrStems.h"
#include "msrBeams.h"

#include "msrTechnicals.h"

#include "msrBarLines.h"

#include "msrBarChecks.h"
#include "msrBarNumberChecks.h"

#include "msrDynamics.h"
#include "msrArticulations.h"

#include "msrTablatures.h"
#include "msrInstruments.h"


#include "msrTies.h"
#include "msrSlurs.h"
#include "msrLigatures.h"

#include "msrSingleTremolos.h"
#include "msrDoubleTremolos.h"

#include "msrOrnaments.h"

#include "msrSpanners.h"

#include "msrOctaveShifts.h"

#include "msrGraceNotes.h"
#include "msrAfterGraceNotes.h"

#include "msrHarmonies.h"
#include "msrFiguredBassElements.h"

#include "msrGlissandos.h"

#include "msrSlides.h"

#include "msrArpeggiatos.h"

#include "msrNotes.h"

#include "msrChords.h"

#include "msrTupletElements.h"
#include "msrTuplets.h"

#include "msrLyrics.h"

#include "msrRepeats.h"
#include "msrRepeatCodas.h"

#include "msrMultipleFullBarRests.h"
#include "msrMeasureRepeats.h"
#include "msrBeatRepeats.h"
#include "msrSlashes.h"

#include "msrMeasureElements.h"
#include "msrMeasures.h"

#include "msrSegments.h"

#include "msrVoiceElements.h"
#include "msrVoices.h"

#include "msrStaves.h"

#include "msrParts.h"

#include "msrPartGroupElements.h"
#include "msrPartGroups.h"

#include "msrScores.h"

#include "msrBookElements.h"
#include "msrBooks.h"

#include "msrMidi.h"

#include "msr2namesVisitor.h"
#include "msr2summaryVisitor.h"

#include "msrPathToVoice.h"


namespace MusicFormats
{

//______________________________________________________________________________
void initializeMSR ();


}


#endif
