/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "msrWae.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrBrowsers.h"

#include "oahOah.h"
#include "msrOah.h"


namespace MusicFormats
{

// tuplets
//______________________________________________________________________________

EXP std::string msrTupletInKindAsString (
  msrTupletInKind tupletInKind)
{
  std::string result;

  switch (tupletInKind) {
    case msrTupletInKind::kTupletIn_UNKNOWN_:
      result = "kTupletIn_UNKNOWN_";
      break;
    case msrTupletInKind::kTupletInMeasure:
      result = "kTupletInMeasure";
      break;
    case msrTupletInKind::kTupletInTuplet:
      result = "kTupletInTuplet";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTupletInKind& elt)
{
  os << msrTupletInKindAsString (elt);
  return os;
}

std::string msrTupletTypeKindAsString (
  msrTupletTypeKind tupletTypeKind)
{
  std::string result;

  switch (tupletTypeKind) {
    case msrTupletTypeKind::kTupletTypeNone:
      result = "kTupletTypeNone";
      break;
    case msrTupletTypeKind::kTupletTypeStart:
      result = "kTupletTypeStart";
      break;
    case msrTupletTypeKind::kTupletTypeContinue:
      result = "kTupletTypeContinue";
      break;
    case msrTupletTypeKind::kTupletTypeStop:
      result = "kTupletTypeStop";
      break;
    case msrTupletTypeKind::kTupletTypeStartAndStopInARow:
      result = "kTupletTypeStartAndStopInARow";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTupletTypeKind& elt)
{
  os << msrTupletTypeKindAsString (elt);
  return os;
}

std::string msrTupletBracketKindAsString (
  msrTupletBracketKind tupletBracketKind)
{
  std::string result;

  switch (tupletBracketKind) {
    case msrTupletBracketKind::kTupletBracketYes:
      result = "kTupletBracketYes";
      break;
    case msrTupletBracketKind::kTupletBracketNo:
      result = "kTupletBracketNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTupletBracketKind& elt)
{
  os << msrTupletBracketKindAsString (elt);
  return os;
}

std::string msrTupletLineShapeKindAsString (
  msrTupletLineShapeKind tupletLineShapeKind)
{
  std::string result;

  switch (tupletLineShapeKind) {
    case msrTupletLineShapeKind::kTupletLineShapeStraight:
      result = "kTupletLineShapeStraight";
      break;
    case msrTupletLineShapeKind::kTupletLineShapeCurved:
      result = "kTupletLineShapeCurved";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTupletLineShapeKind& elt)
{
  os << msrTupletLineShapeKindAsString (elt);
  return os;
}

std::string msrTupletShowNumberKindAsString (
  msrTupletShowNumberKind tupletShowNumberKind)
{
  std::string result;

  switch (tupletShowNumberKind) {
    case msrTupletShowNumberKind::kTupletShowNumberActual:
      result = "kTupletShowNumberActual";
      break;
    case msrTupletShowNumberKind::kTupletShowNumberBoth:
      result = "kTupletShowNumberBoth";
      break;
    case msrTupletShowNumberKind::kTupletShowNumberNone:
      result = "kTupletShowNumberNone";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTupletShowNumberKind& elt)
{
  os << msrTupletShowNumberKindAsString (elt);
  return os;
}

std::string msrTupletShowTypeKindAsString (
  msrTupletShowTypeKind tupletShowTypeKind)
{
  std::string result;

  switch (tupletShowTypeKind) {
    case msrTupletShowTypeKind::kTupletShowTypeActual:
      result = "kTupletShowTypeActual";
      break;
    case msrTupletShowTypeKind::kTupletShowTypeBoth:
      result = "kTupletShowTypeBoth";
      break;
    case msrTupletShowTypeKind::kTupletShowTypeNone:
      result = "kTupletShowTypeNone";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTupletShowTypeKind& elt)
{
  os << msrTupletShowTypeKindAsString (elt);
  return os;
}

// // tuplets
// //______________________________________________________________________________
//
// EXP std::string msrTupletInKindAsString (
//   msrTupletInKind tupletInKind)
// {
//   std::string result;
//
//   switch (tupletInKind) {
//     case msrTupletInKind::kTupletIn_UNKNOWN_:
//       result = "kTupletIn_UNKNOWN_";
//       break;
//     case msrTupletInKind::kTupletInMeasure:
//       result = "kTupletInMeasure";
//       break;
//     case msrTupletInKind::kTupletInTuplet:
//       result = "kTupletInTuplet";
//       break;
//   } // switch
//
//   return result;
// }
//
// std::ostream& operator << (std::ostream& os, const msrTupletInKind& elt)
// {
//   os << msrTupletInKindAsString (elt);
//   return os;
// }
//
// std::string msrTupletTypeKindAsString (
//   msrTupletTypeKind tupletTypeKind)
// {
//   std::string result;
//
//   switch (tupletTypeKind) {
//     case msrTupletTypeKind::kTupletTypeNone:
//       result = "kTupletTypeNone";
//       break;
//     case msrTupletTypeKind::kTupletTypeStart:
//       result = "kTupletTypeStart";
//       break;
//     case msrTupletTypeKind::kTupletTypeContinue:
//       result = "kTupletTypeContinue";
//       break;
//     case msrTupletTypeKind::kTupletTypeStop:
//       result = "kTupletTypeStop";
//       break;
//     case msrTupletTypeKind::kTupletTypeStartAndStopInARow:
//       result = "kTupletTypeStartAndStopInARow";
//       break;
//   } // switch
//
//   return result;
// }
//
// std::ostream& operator << (std::ostream& os, const msrTupletTypeKind& elt)
// {
//   os << msrTupletTypeKindAsString (elt);
//   return os;
// }
//
// std::string msrTupletBracketKindAsString (
//   msrTupletBracketKind tupletBracketKind)
// {
//   std::string result;
//
//   switch (tupletBracketKind) {
//     case msrTupletBracketKind::kTupletBracketYes:
//       result = "kTupletBracketYes";
//       break;
//     case msrTupletBracketKind::kTupletBracketNo:
//       result = "kTupletBracketNo";
//       break;
//   } // switch
//
//   return result;
// }
//
// std::ostream& operator << (std::ostream& os, const msrTupletBracketKind& elt)
// {
//   os << msrTupletBracketKindAsString (elt);
//   return os;
// }
//
// std::string msrTupletLineShapeKindAsString (
//   msrTupletLineShapeKind tupletLineShapeKind)
// {
//   std::string result;
//
//   switch (tupletLineShapeKind) {
//     case msrTupletLineShapeKind::kTupletLineShapeStraight:
//       result = "kTupletLineShapeStraight";
//       break;
//     case msrTupletLineShapeKind::kTupletLineShapeCurved:
//       result = "kTupletLineShapeCurved";
//       break;
//   } // switch
//
//   return result;
// }
//
// std::ostream& operator << (std::ostream& os, const msrTupletLineShapeKind& elt)
// {
//   os << msrTupletLineShapeKindAsString (elt);
//   return os;
// }
//
// std::string msrTupletShowNumberKindAsString (
//   msrTupletShowNumberKind tupletShowNumberKind)
// {
//   std::string result;
//
//   switch (tupletShowNumberKind) {
//     case msrTupletShowNumberKind::kTupletShowNumberActual:
//       result = "kTupletShowNumberActual";
//       break;
//     case msrTupletShowNumberKind::kTupletShowNumberBoth:
//       result = "kTupletShowNumberBoth";
//       break;
//     case msrTupletShowNumberKind::kTupletShowNumberNone:
//       result = "kTupletShowNumberNone";
//       break;
//   } // switch
//
//   return result;
// }
//
// std::ostream& operator << (std::ostream& os, const msrTupletShowNumberKind& elt)
// {
//   os << msrTupletShowNumberKindAsString (elt);
//   return os;
// }
//
// std::string msrTupletShowTypeKindAsString (
//   msrTupletShowTypeKind tupletShowTypeKind)
// {
//   std::string result;
//
//   switch (tupletShowTypeKind) {
//     case msrTupletShowTypeKind::kTupletShowTypeActual:
//       result = "kTupletShowTypeActual";
//       break;
//     case msrTupletShowTypeKind::kTupletShowTypeBoth:
//       result = "kTupletShowTypeBoth";
//       break;
//     case msrTupletShowTypeKind::kTupletShowTypeNone:
//       result = "kTupletShowTypeNone";
//       break;
//   } // switch
//
//   return result;
// }
//
// std::ostream& operator << (std::ostream& os, const msrTupletShowTypeKind& elt)
// {
//   os << msrTupletShowTypeKindAsString (elt);
//   return os;
// }
//

}
