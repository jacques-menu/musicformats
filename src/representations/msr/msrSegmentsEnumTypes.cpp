/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include "msrSegmentsEnumTypes.h"


namespace MusicFormats
{

// segments
//______________________________________________________________________________
std::string msrSegmentKindAsString (
  msrSegmentKind segmentKind)
{
  std::string result;

  switch (segmentKind) {
    case msrSegmentKind::kSegmentKindAtVoiceLevel:
      result = "kSegmentKindAtVoiceLevel";
      break;
    case msrSegmentKind::kSegmentKindInRepeatCommonPart:
      result = "kSegmentKindInRepeatCommonPart";
      break;
    case msrSegmentKind::kSegmentKindInRepeatEnding:
      result = "kSegmentKindInRepeatEnding";
      break;
    case msrSegmentKind::kSegmentKindInMeasureRepeat:
      result = "kSegmentKindInMeasureRepeat";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSegmentKind& elt)
{
  os << msrSegmentKindAsString (elt);
  return os;
}

std::string msrSegmentKindAsStringForPrint (
  msrSegmentKind segmentKind)
{
  std::string result;

  switch (segmentKind) {
    case msrSegmentKind::kSegmentKindAtVoiceLevel:
      result = "AtVoiceLevel";
      break;
    case msrSegmentKind::kSegmentKindInRepeatCommonPart:
      result = "InRepeatCommonPart";
      break;
    case msrSegmentKind::kSegmentKindInRepeatEnding:
      result = "InRepeatEnding";
      break;
    case msrSegmentKind::kSegmentKindInMeasureRepeat:
      result = "InMeasureRepeat";
      break;
  } // switch

  return result;
}


}
