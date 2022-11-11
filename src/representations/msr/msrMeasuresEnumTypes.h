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


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations

// class msrMeasureElement;
// typedef SMARTP<msrMeasureElement> S_msrMeasureElement;

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

string msrMeasureKindAsString (
  msrMeasureKind measureKind);

ostream& operator << (ostream& os, const msrMeasureKind& elt);

string msrMeasureKindasShortStringForMeasuresSlices (
  msrMeasureKind measureKind);

enum class msrMeasureImplicitKind {
  kMeasureImplicitKindYes,
  kMeasureImplicitKindNo
};

string msrMeasureImplicitKindAsString (
  msrMeasureImplicitKind measureImplicitKind);

ostream& operator << (ostream& os, const msrMeasureImplicitKind& elt);

//______________________________________________________________________________
  // data types

enum msrMeasureFirstInSegmentKind {
  kMeasureFirstInSegmentKindUnknown,
  kMeasureFirstInSegmentKindYes,
  kMeasureFirstInSegmentKindNo
};

string msrMeasureFirstInSegmentKindAsString (
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

string msrMeasureRepeatContextKindAsString (
  msrMeasureRepeatContextKind measureRepeatContextKind);

enum msrMeasureEndRegularKind {
  kMeasureEndRegularKindUnknown,
  kMeasureEndRegularKindYes,
  kMeasureEndRegularKindNo
};

string msrMeasureEndRegularKindAsString (
  msrMeasureEndRegularKind measureEndRegularKind);


}


#endif
