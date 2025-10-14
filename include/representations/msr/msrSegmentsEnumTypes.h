/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSegmentsEnumTypes___
#define ___msrSegmentsEnumTypes___

#include <string>
#include <ostream>

namespace MusicFormats
{

// segment
//______________________________________________________________________________
enum class msrSegmentKind {
  kSegmentKindAtVoiceLevel,
  kSegmentKindInRepeatCommonPart,
  kSegmentKindInRepeatEnding,
  kSegmentKindInMeasureRepeat
};

std::string msrSegmentKindAsString (
  msrSegmentKind segmentKind);

std::ostream& operator << (std::ostream& os, const msrSegmentKind& elt);

std::string msrSegmentKindAsStringForPrint (
  msrSegmentKind segmentKind);


}


#endif // ___msrSegmentsEnumTypes___
