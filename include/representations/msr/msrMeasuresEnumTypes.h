/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasuresEnumTypes___
#define ___msrMeasuresEnumTypes___

#include <string>

#include "mfIndentedTextOutput.h"


namespace MusicFormats
{

// measures
//______________________________________________________________________________
enum class msrMeasureKind {
  kMeasureKindUnknown,
  kMeasureKindRegular,
  kMeasureKindAnacrusis,
  kMeasureKindIncompleteStandalone,
  kMeasureKindIncompleteLastMeasure,
  kMeasureKindIncompleteLastInRepeatCommonPart,
  kMeasureKindIncompleteLastInRepeatHookedEnding,
  kMeasureKindIncompleteLastInRepeatHooklessEnding,
  kMeasureKindIncompleteNextMeasureAfterCommonPart,
  kMeasureKindIncompleteNextMeasureAfterHookedEnding,
  kMeasureKindIncompleteNextMeasureAfterHooklessEnding,
  kMeasureKindOvercomplete,
  kMeasureKindCadenza,
  kMeasureKindMusicallyEmpty
};

std::string msrMeasureKindAsString (
  msrMeasureKind measureKind);

std::ostream& operator << (std::ostream& os, const msrMeasureKind& elt);
mfIndentedStringStream& operator << (mfIndentedStringStream& iss, const msrMeasureKind elt);


std::string msrMeasureKindasShortStringForMeasuresSlices (
  msrMeasureKind measureKind);

enum class msrMeasureImplicitKind {
  kMeasureImplicitKindYes,
  kMeasureImplicitKindNo
};

std::string msrMeasureImplicitKindAsString (
  msrMeasureImplicitKind measureImplicitKind);

std::ostream& operator << (std::ostream& os, const msrMeasureImplicitKind& elt);
mfIndentedStringStream& operator << (mfIndentedStringStream& iss, const msrMeasureImplicitKind elt);

//______________________________________________________________________________
// data types

enum msrMeasureFirstInSegmentKind {
  kMeasureFirstInSegmentKindUnknown,
  kMeasureFirstInSegmentKindYes,
  kMeasureFirstInSegmentKindNo
};

std::string msrMeasureFirstInSegmentKindAsString (
  msrMeasureFirstInSegmentKind measureFirstInSegmentKind);

std::ostream& operator << (std::ostream& os, const msrMeasureFirstInSegmentKind& elt);

enum msrMeasureRepeatContextKind {
  kMeasureRepeatContext_UNKNOWN_,
  kMeasureRepeatContextNone,
  kMeasureRepeatContextCommonPartLastMeasure,
  kMeasureRepeatContextHookedEndingLastMeasure,
  kMeasureRepeatContextHooklessEndingLastMeasure,
  kMeasureRepeatContextNextMeasureAfterCommonPart,
  kMeasureRepeatContextNextMeasureAfterHookedEnding,
  kMeasureRepeatContextNextMeasureAfterHooklessEnding
};

std::string msrMeasureRepeatContextKindAsString (
  msrMeasureRepeatContextKind measureRepeatContextKind);

std::ostream& operator << (std::ostream& os, const msrMeasureRepeatContextKind& elt);

enum msrMeasureEndRegularKind {
  kMeasureEndRegularKindUnknown,
  kMeasureEndRegularKindYes,
  kMeasureEndRegularKindNo
};

std::string msrMeasureEndRegularKindAsString (
  msrMeasureEndRegularKind measureEndRegularKind);

std::ostream& operator << (std::ostream& os, const msrMeasureEndRegularKind& elt);


}


#endif // ___msrMeasuresEnumTypes___
