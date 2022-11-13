/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasuresEnumTypes___
#define ___msrMeasuresEnumTypes___

#include <string>


namespace MusicFormats
{

// measures
//______________________________________________________________________________
enum class msrMeasureKind {
  kMeasureKindUnknown,
  kMeasureKindRegular,
  kMeasureKindAnacrusis,
  kMeasureKindIncompleteStandalone,
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

std::string msrMeasureKindasShortStringForMeasuresSlices (
  msrMeasureKind measureKind);

enum class msrMeasureImplicitKind {
  kMeasureImplicitKindYes,
  kMeasureImplicitKindNo
};

std::string msrMeasureImplicitKindAsString (
  msrMeasureImplicitKind measureImplicitKind);

std::ostream& operator << (std::ostream& os, const msrMeasureImplicitKind& elt);

//______________________________________________________________________________
  // data types

enum msrMeasureFirstInSegmentKind {
  kMeasureFirstInSegmentKindUnknown,
  kMeasureFirstInSegmentKindYes,
  kMeasureFirstInSegmentKindNo
};

std::string msrMeasureFirstInSegmentKindAsString (
  msrMeasureFirstInSegmentKind measureFirstInSegmentKind);

enum msrMeasureRepeatContextKind {
  kMeasureRepeatContext_NO_,
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

enum msrMeasureEndRegularKind {
  kMeasureEndRegularKindUnknown,
  kMeasureEndRegularKindYes,
  kMeasureEndRegularKindNo
};

std::string msrMeasureEndRegularKindAsString (
  msrMeasureEndRegularKind measureEndRegularKind);


}


#endif
