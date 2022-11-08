/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrStavesEnumTypes___
#define ___msrStavesEnumTypes___

// #include "msrElements.h"
//
// #include "msrBarLines.h"
// #include "msrClefs.h"
// #include "msrInstruments.h"
// #include "msrKeys.h"
// #include "msrMeasuresEnumTypes.h"
// #include "msrMeasures.h"
// #include "msrMeasuresSlices.h"
// #include "msrParts.h"
// #include "msrPartGroups.h"
// #include "msrPrintObjects.h"
// #include "msrMultipleFullBarRests.h"
// #include "msrRehearsalMarks.h"
// #include "msrRepeats.h"
// #include "msrScores.h"
// #include "msrStavesDetails.h"
// #include "msrTimeSignatures.h"
// #include "msrTempos.h"
// #include "msrTranspositions.h"
// #include "msrTupletFactors.h"

namespace MusicFormats
{

// staves
// ------------------------------------------------------

enum class msrStaffKind {
  kStaffKindRegular,
  kStaffKindTablature,
  kStaffKindHarmonies,
  kStaffKindFiguredBass,
  kStaffKindDrum,
  kStaffKindRythmic
};

string msrStaffKindAsString (
  msrStaffKind staffKind);

ostream& operator << (ostream& os, const msrStaffKind& elt);

string msrStaffKindAsStringForPrint (
  msrStaffKind staffKind);


}


#endif
