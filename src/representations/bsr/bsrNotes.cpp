/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "mfStringsHandling.h"

#include "bsrNotes.h"

#include "oahOah.h"

#include "bsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string bsrNoteOctaveKindAsString (
  bsrNoteOctaveKind noteOctaveKind)
{
  std::string result;

  switch (noteOctaveKind) {
    case bsrNoteOctaveKind::kNoteOctaveNone:   result = "octaveNone"; break;
    case bsrNoteOctaveKind::kNoteOctaveBelow1: result = "octaveBelow1"; break;
    case bsrNoteOctaveKind::kNoteOctave1:      result = "octave1"; break;
    case bsrNoteOctaveKind::kNoteOctave2:      result = "octave2"; break;
    case bsrNoteOctaveKind::kNoteOctave3:      result = "octave3"; break;
    case bsrNoteOctaveKind::kNoteOctave4:      result = "octave4"; break;
    case bsrNoteOctaveKind::kNoteOctave5:      result = "octave5"; break;
    case bsrNoteOctaveKind::kNoteOctave6:      result = "octave6"; break;
    case bsrNoteOctaveKind::kNoteOctave7:      result = "octave7"; break;
    case bsrNoteOctaveKind::kNoteOctaveAbove7: result = "octaveAbove7"; break;
  } // switch

  return result;
}

std::string bsrNoteOctaveKindAsDebugString (
  bsrNoteOctaveKind noteOctaveKind)
{
  std::string result;

  switch (noteOctaveKind) {
    case bsrNoteOctaveKind::kNoteOctaveNone: break; // should be "_", fix noteOctaveIsNeeded in msr2bsrTranslator.cpp for rests JMI
    case bsrNoteOctaveKind::kNoteOctaveBelow1: result = "oB1"; break;
    case bsrNoteOctaveKind::kNoteOctave1:      result = "o1"; break;
    case bsrNoteOctaveKind::kNoteOctave2:      result = "o2"; break;
    case bsrNoteOctaveKind::kNoteOctave3:      result = "o3"; break;
    case bsrNoteOctaveKind::kNoteOctave4:      result = "o4"; break;
    case bsrNoteOctaveKind::kNoteOctave5:      result = "o5"; break;
    case bsrNoteOctaveKind::kNoteOctave6:      result = "o6"; break;
    case bsrNoteOctaveKind::kNoteOctave7:      result = "o7"; break;
    case bsrNoteOctaveKind::kNoteOctaveAbove7: result = "oA7"; break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrNoteOctaveKind& elt)
{
  os << bsrNoteOctaveKindAsString (elt);
  return os;
}

S_bsrCellsList noteOctaveKindAsCellsList (
  int               inputLineNumber,
  bsrNoteOctaveKind noteOctaveKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (noteOctaveKind) {
    case bsrNoteOctaveKind::kNoteOctaveNone:
      break;
    case bsrNoteOctaveKind::kNoteOctaveBelow1:
      result->appendCellKindToCellsList (bsrCellKind::kDots4);
      result->appendCellKindToCellsList (bsrCellKind::kDots4);
      break;
    case bsrNoteOctaveKind::kNoteOctave1:
      result->appendCellKindToCellsList (bsrCellKind::kDots4);
      break;
    case bsrNoteOctaveKind::kNoteOctave2:
      result->appendCellKindToCellsList (bsrCellKind::kDots45);
      break;
    case bsrNoteOctaveKind::kNoteOctave3:
      result->appendCellKindToCellsList (bsrCellKind::kDots456);
      break;
    case bsrNoteOctaveKind::kNoteOctave4:
      result->appendCellKindToCellsList (bsrCellKind::kDots5);
      break;
    case bsrNoteOctaveKind::kNoteOctave5:
      result->appendCellKindToCellsList (bsrCellKind::kDots46);
      break;
    case bsrNoteOctaveKind::kNoteOctave6:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      break;
    case bsrNoteOctaveKind::kNoteOctave7:
      result->appendCellKindToCellsList (bsrCellKind::kDots6);
      break;
    case bsrNoteOctaveKind::kNoteOctaveAbove7:
      result->appendCellKindToCellsList (bsrCellKind::kDots6);
      result->appendCellKindToCellsList (bsrCellKind::kDots6);
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_bsrNote bsrNote::create (
  int                   inputLineNumber,
  bsrNoteValueKind      noteValueKind,
  int                   noteDotsNumber,
  bsrNoteOctaveKind     noteOctaveKind,
  bsrNoteOctaveIsNeeded noteOctaveIsNeeded,
  bsrAccidentalKind accidentalKind)
{
  bsrNote* o =
    new bsrNote (
      inputLineNumber,
      noteValueKind,
      noteDotsNumber,
      noteOctaveKind,
      noteOctaveIsNeeded,
      accidentalKind);
  assert (o != nullptr);
  return o;
}

bsrNote::bsrNote (
  int                   inputLineNumber,
  bsrNoteValueKind      noteValueKind,
  int                   noteDotsNumber,
  bsrNoteOctaveKind     noteOctaveKind,
  bsrNoteOctaveIsNeeded noteOctaveIsNeeded,
  bsrAccidentalKind accidentalKind)
    : bsrLineContentsElement (inputLineNumber)
{
  fNoteValueKind  = noteValueKind;
  fNoteDotsNumber = noteDotsNumber;

  fNoteValueSizeIsNeeded = false;

  fNoteOctaveKind = noteOctaveKind;
  fNoteOctaveIsNeeded = noteOctaveIsNeeded;

  fAccidentalKind = accidentalKind;
}

bsrNote::~bsrNote ()
{}

S_bsrCellsList noteValueKindAsCellsList (
  int              inputLineNumber,
  bsrNoteValueKind noteValueKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (noteValueKind) {
    case bsrNoteValueKind::kNoteValueNone:
      break;

    case bsrNoteValueKind::kNoteValueRestBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots134);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case bsrNoteValueKind::kNoteValueRestWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots134);
      break;
    case bsrNoteValueKind::kNoteValueRestHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots136);
      break;
    case bsrNoteValueKind::kNoteValueRestQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots1236);
      break;
    case bsrNoteValueKind::kNoteValueRest8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1346);
      break;
    case bsrNoteValueKind::kNoteValueRest16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots134);
      break;
    case bsrNoteValueKind::kNoteValueRest32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots136);
      break;
    case bsrNoteValueKind::kNoteValueRest64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1236);
      break;
    case bsrNoteValueKind::kNoteValueRest128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1346);
      break;
    case bsrNoteValueKind::kNoteValueRest256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots134);
      break;

    case bsrNoteValueKind::kNoteValueCBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots13456);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case bsrNoteValueKind::kNoteValueCWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots13456);
      break;
    case bsrNoteValueKind::kNoteValueCHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots1345);
      break;
    case bsrNoteValueKind::kNoteValueCQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots1456);
      break;
    case bsrNoteValueKind::kNoteValueC8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots145);
      break;
    case bsrNoteValueKind::kNoteValueC16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots13456);
      break;
    case bsrNoteValueKind::kNoteValueC32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots1345);
      break;
    case bsrNoteValueKind::kNoteValueC64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1456);
      break;
    case bsrNoteValueKind::kNoteValueC128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots145);
      break;
    case bsrNoteValueKind::kNoteValueC256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots13456);
      break;

    case bsrNoteValueKind::kNoteValueDBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots1356);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case bsrNoteValueKind::kNoteValueDWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots1356);
      break;
    case bsrNoteValueKind::kNoteValueDHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots135);
      break;
    case bsrNoteValueKind::kNoteValueDQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots156);
      break;
    case bsrNoteValueKind::kNoteValueD8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots15);
      break;
    case bsrNoteValueKind::kNoteValueD16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1356);
      break;
    case bsrNoteValueKind::kNoteValueD32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots135);
      break;
    case bsrNoteValueKind::kNoteValueD64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots156);
      break;
    case bsrNoteValueKind::kNoteValueD128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots15);
      break;
    case bsrNoteValueKind::kNoteValueD256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots1356);
      break;

    case bsrNoteValueKind::kNoteValueEBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots12346);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case bsrNoteValueKind::kNoteValueEWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots12346);
      break;
    case bsrNoteValueKind::kNoteValueEHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots1234);
      break;
    case bsrNoteValueKind::kNoteValueEQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots1246);
      break;
    case bsrNoteValueKind::kNoteValueE8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots124);
      break;
    case bsrNoteValueKind::kNoteValueE16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots12346);
      break;
    case bsrNoteValueKind::kNoteValueE32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots1234);
      break;
    case bsrNoteValueKind::kNoteValueE64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1246);
      break;
    case bsrNoteValueKind::kNoteValueE128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots124);
      break;
    case bsrNoteValueKind::kNoteValueE256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots12346);
      break;

    case bsrNoteValueKind::kNoteValueFBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots123456);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case bsrNoteValueKind::kNoteValueFWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots123456);
      break;
    case bsrNoteValueKind::kNoteValueFHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots12345);
      break;
    case bsrNoteValueKind::kNoteValueFQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots12456);
      break;
    case bsrNoteValueKind::kNoteValueF8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1245);
      break;
    case bsrNoteValueKind::kNoteValueF16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots123456);
      break;
    case bsrNoteValueKind::kNoteValueF32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots12345);
      break;
    case bsrNoteValueKind::kNoteValueF64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots12456);
      break;
    case bsrNoteValueKind::kNoteValueF128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1245);
      break;
    case bsrNoteValueKind::kNoteValueF256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots123456);
      break;

    case bsrNoteValueKind::kNoteValueGBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots12356);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case bsrNoteValueKind::kNoteValueGWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots12356);
      break;
    case bsrNoteValueKind::kNoteValueGHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots1235);
      break;
    case bsrNoteValueKind::kNoteValueGQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots1256);
      break;
    case bsrNoteValueKind::kNoteValueG8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots125);
      break;
    case bsrNoteValueKind::kNoteValueG16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots12356);
      break;
    case bsrNoteValueKind::kNoteValueG32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots1235);
      break;
    case bsrNoteValueKind::kNoteValueG64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1256);
      break;
    case bsrNoteValueKind::kNoteValueG128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots125);
      break;
    case bsrNoteValueKind::kNoteValueG256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots12356);
      break;

    case bsrNoteValueKind::kNoteValueABreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots2346);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case bsrNoteValueKind::kNoteValueAWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots2346);
      break;
    case bsrNoteValueKind::kNoteValueAHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots234);
      break;
    case bsrNoteValueKind::kNoteValueAQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots246);
      break;
    case bsrNoteValueKind::kNoteValueA8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots24);
      break;
    case bsrNoteValueKind::kNoteValueA16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots2346);
      break;
    case bsrNoteValueKind::kNoteValueA32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots234);
      break;
    case bsrNoteValueKind::kNoteValueA64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots246);
      break;
    case bsrNoteValueKind::kNoteValueA128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots24);
      break;
    case bsrNoteValueKind::kNoteValueA256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots2346);
      break;

    case bsrNoteValueKind::kNoteValueBBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots23456);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case bsrNoteValueKind::kNoteValueBWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots23456);
      break;
    case bsrNoteValueKind::kNoteValueBHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots2345);
      break;
    case bsrNoteValueKind::kNoteValueBQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots2456);
      break;
    case bsrNoteValueKind::kNoteValueB8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots245);
      break;
    case bsrNoteValueKind::kNoteValueB16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots23456);
      break;
    case bsrNoteValueKind::kNoteValueB32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots2345);
      break;
    case bsrNoteValueKind::kNoteValueB64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots2456);
      break;
    case bsrNoteValueKind::kNoteValueB128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots245);
      break;
    case bsrNoteValueKind::kNoteValueB256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots23456);
      break;
  } // switch

  return result;
}

bsrNoteValueSizeKind bsrNoteValueSizeKindFromNoteValueKind (
  bsrNoteValueKind noteValueKind)
{
  bsrNoteValueSizeKind
    result = bsrNoteValueSizeKind::kNoteValueSizeNone;

  switch (noteValueKind) {
    case bsrNoteValueKind::kNoteValueNone:
      break;

    case bsrNoteValueKind::kNoteValueRestBreve:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueRestWhole:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueRestHalf:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueRestQuarter:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueRest8th:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueRest16th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueRest32nd:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueRest64th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueRest128th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueRest256th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;

    case bsrNoteValueKind::kNoteValueCBreve:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueCWhole:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueCHalf:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueCQuarter:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueC8th:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueC16th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueC32nd:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueC64th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueC128th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueC256th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;

    case bsrNoteValueKind::kNoteValueDBreve:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueDWhole:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueDHalf:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueDQuarter:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueD8th:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueD16th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueD32nd:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueD64th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueD128th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueD256th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;

    case bsrNoteValueKind::kNoteValueEBreve:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueEWhole:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueEHalf:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueEQuarter:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueE8th:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueE16th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueE32nd:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueE64th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueE128th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueE256th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;

    case bsrNoteValueKind::kNoteValueFBreve:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueFWhole:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueFHalf:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueFQuarter:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueF8th:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueF16th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueF32nd:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueF64th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueF128th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueF256th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;

    case bsrNoteValueKind::kNoteValueGBreve:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueGWhole:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueGHalf:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueGQuarter:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueG8th:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueG16th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueG32nd:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueG64th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueG128th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueG256th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;

    case bsrNoteValueKind::kNoteValueABreve:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueAWhole:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueAHalf:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueAQuarter:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueA8th:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueA16th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueA32nd:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueA64th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueA128th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueA256th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;

    case bsrNoteValueKind::kNoteValueBBreve:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueBWhole:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueBHalf:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueBQuarter:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueB8th:
      result = bsrNoteValueSizeKind::kNoteValueSizeLarger;
      break;
    case bsrNoteValueKind::kNoteValueB16th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueB32nd:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueB64th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueB128th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
    case bsrNoteValueKind::kNoteValueB256th:
      result = bsrNoteValueSizeKind::kNoteValueSizeSmaller;
      break;
  } // switch

  return result;
}

S_bsrCellsList noteValueSizeKindAsCellsList (
  int                  inputLineNumber,
  bsrNoteValueSizeKind noteValueSizeKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (noteValueSizeKind) {
    case bsrNoteValueSizeKind::kNoteValueSizeNone:
      break;
    case bsrNoteValueSizeKind::kNoteValueSizeLarger:
      result->appendCellKindToCellsList (bsrCellKind::kDots45);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      break;
    case bsrNoteValueSizeKind::kNoteValueSizeSmaller:
      result->appendCellKindToCellsList (bsrCellKind::kDots6);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::accidentalKindAsCellsList (
  int               inputLineNumber,
  bsrAccidentalKind accidentalKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (accidentalKind) {
    case bsrAccidentalKind::kAccidentalNone:
      break;

    case bsrAccidentalKind::kAccidentalSharp:
      result->appendCellKindToCellsList (kCellSharp);
      break;
    case bsrAccidentalKind::kAccidentalNatural:
      result->appendCellKindToCellsList (kCellNatural);
      break;
    case bsrAccidentalKind::kAccidentalFlat:
      result->appendCellKindToCellsList (kCellFlat);
      break;

    case bsrAccidentalKind::kAccidentalDoubleFlat:
      result->appendCellKindToCellsList (kCellFlat);
      result->appendCellKindToCellsList (kCellFlat);
      break;
    case bsrAccidentalKind::kAccidentalDoubleSharp:
      result->appendCellKindToCellsList (kCellSharp);
      result->appendCellKindToCellsList (kCellSharp);
      break;

    case bsrAccidentalKind::kAccidentalQuarterSharp:
      result->appendCellKindToCellsList (bsrCellKind::kDots4);
      result->appendCellKindToCellsList (kCellSharp);
      break;
    case bsrAccidentalKind::kAccidentalQuarterFlat:
      result->appendCellKindToCellsList (bsrCellKind::kDots4);
      result->appendCellKindToCellsList (kCellFlat);
      break;

    case bsrAccidentalKind::kAccidentalThreeQuarterSharp:
      result->appendCellKindToCellsList (bsrCellKind::kDots456);
      result->appendCellKindToCellsList (kCellSharp);
      break;

    case bsrAccidentalKind::kAccidentalThreeQuarterFlat:
      result->appendCellKindToCellsList (bsrCellKind::kDots456);
      result->appendCellKindToCellsList (kCellFlat);
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::accidentalKindAsCellsList () const
{
  return
    accidentalKindAsCellsList (
      fInputLineNumber,
      fAccidentalKind);
}

S_bsrCellsList bsrNote::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append the note value sign if needed
  if (fNoteValueSizeIsNeeded) {
    result->appendCellsListToCellsList (
      noteValueSizeKindAsCellsList (
        fInputLineNumber,
        bsrNoteValueSizeKindFromNoteValueKind (
          fNoteValueKind)));
  }

  // append note accidental if any
  result->appendCellsListToCellsList (
    accidentalKindAsCellsList ());

  // append note octave if needed
  switch (fNoteOctaveIsNeeded) {
    case bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededYes:
      result->appendCellsListToCellsList (
        noteOctaveKindAsCellsList (
          fInputLineNumber,
          fNoteOctaveKind));
      break;
    case bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededNo:
      break;
  } // switch

  // append note value
  result->appendCellsListToCellsList (
    noteValueKindAsCellsList (
      fInputLineNumber,
      fNoteValueKind));

  // append dots if any
  for (int i = 0; i < fNoteDotsNumber; ++i) {
    result->appendCellKindToCellsList (
      kCellAugmentationDot);
  } // for

  return result;
}

int bsrNote::fetchCellsNumber() const
{
  return buildCellsList ()->fetchCellsNumber();
}

void bsrNote::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrNote::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrNote>*
    p =
      dynamic_cast<visitor<S_bsrNote>*> (v)) {
        S_bsrNote elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrNote::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrNote::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrNote::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrNote>*
    p =
      dynamic_cast<visitor<S_bsrNote>*> (v)) {
        S_bsrNote elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrNote::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrNote::browseData (basevisitor* v)
{}

std::string bsrNoteValueKindAsString (
  bsrNoteValueKind noteValueKind)
{
  std::string result;

  switch (noteValueKind) {
    case bsrNoteValueKind::kNoteValueNone: result = "None"; break;

    case bsrNoteValueKind::kNoteValueRestBreve: result = "RestBreve"; break;
    case bsrNoteValueKind::kNoteValueRestWhole: result = "RestWhole"; break;
    case bsrNoteValueKind::kNoteValueRestHalf: result = "RestHalf"; break;
    case bsrNoteValueKind::kNoteValueRestQuarter: result = "RestQuarter"; break;
    case bsrNoteValueKind::kNoteValueRest8th: result = "Rest8th"; break;
    case bsrNoteValueKind::kNoteValueRest16th: result = "Rest16th"; break;
    case bsrNoteValueKind::kNoteValueRest32nd: result = "Rest32nd"; break;
    case bsrNoteValueKind::kNoteValueRest64th: result = "Rest64th"; break;
    case bsrNoteValueKind::kNoteValueRest128th: result = "Rest128th"; break;
    case bsrNoteValueKind::kNoteValueRest256th: result = "Rest256th"; break;

    case bsrNoteValueKind::kNoteValueCBreve: result = "CBreve"; break;
    case bsrNoteValueKind::kNoteValueCWhole: result = "CWhole"; break;
    case bsrNoteValueKind::kNoteValueCHalf: result = "CHalf"; break;
    case bsrNoteValueKind::kNoteValueCQuarter: result = "CQuarter"; break;
    case bsrNoteValueKind::kNoteValueC8th: result = "C8th"; break;
    case bsrNoteValueKind::kNoteValueC16th: result = "C16th"; break;
    case bsrNoteValueKind::kNoteValueC32nd: result = "C32nd"; break;
    case bsrNoteValueKind::kNoteValueC64th: result = "C64th"; break;
    case bsrNoteValueKind::kNoteValueC128th: result = "C128th"; break;
    case bsrNoteValueKind::kNoteValueC256th: result = "C256th"; break;

    case bsrNoteValueKind::kNoteValueDBreve: result = "DBreve"; break;
    case bsrNoteValueKind::kNoteValueDWhole: result = "DWhole"; break;
    case bsrNoteValueKind::kNoteValueDHalf: result = "DHalf"; break;
    case bsrNoteValueKind::kNoteValueDQuarter: result = "DQuarter"; break;
    case bsrNoteValueKind::kNoteValueD8th: result = "D8th"; break;
    case bsrNoteValueKind::kNoteValueD16th: result = "D16th"; break;
    case bsrNoteValueKind::kNoteValueD32nd: result = "D32nd"; break;
    case bsrNoteValueKind::kNoteValueD64th: result = "D64th"; break;
    case bsrNoteValueKind::kNoteValueD128th: result = "D128th"; break;
    case bsrNoteValueKind::kNoteValueD256th: result = "D256th"; break;

    case bsrNoteValueKind::kNoteValueEBreve: result = "EBreve"; break;
    case bsrNoteValueKind::kNoteValueEWhole: result = "EWhole"; break;
    case bsrNoteValueKind::kNoteValueEHalf: result = "EHalf"; break;
    case bsrNoteValueKind::kNoteValueEQuarter: result = "EQuarter"; break;
    case bsrNoteValueKind::kNoteValueE8th: result = "E8th"; break;
    case bsrNoteValueKind::kNoteValueE16th: result = "E16th"; break;
    case bsrNoteValueKind::kNoteValueE32nd: result = "E32nd"; break;
    case bsrNoteValueKind::kNoteValueE64th: result = "E64th"; break;
    case bsrNoteValueKind::kNoteValueE128th: result = "E128th"; break;
    case bsrNoteValueKind::kNoteValueE256th: result = "E256th"; break;

    case bsrNoteValueKind::kNoteValueFBreve: result = "FBreve"; break;
    case bsrNoteValueKind::kNoteValueFWhole: result = "FWhole"; break;
    case bsrNoteValueKind::kNoteValueFHalf: result = "FHalf"; break;
    case bsrNoteValueKind::kNoteValueFQuarter: result = "FQuarter"; break;
    case bsrNoteValueKind::kNoteValueF8th: result = "F8th"; break;
    case bsrNoteValueKind::kNoteValueF16th: result = "F16th"; break;
    case bsrNoteValueKind::kNoteValueF32nd: result = "F32nd"; break;
    case bsrNoteValueKind::kNoteValueF64th: result = "F64th"; break;
    case bsrNoteValueKind::kNoteValueF128th: result = "F128th"; break;
    case bsrNoteValueKind::kNoteValueF256th: result = "F256th"; break;

    case bsrNoteValueKind::kNoteValueGBreve: result = "GBreve"; break;
    case bsrNoteValueKind::kNoteValueGWhole: result = "GWhole"; break;
    case bsrNoteValueKind::kNoteValueGHalf: result = "GHalf"; break;
    case bsrNoteValueKind::kNoteValueGQuarter: result = "GQuarter"; break;
    case bsrNoteValueKind::kNoteValueG8th: result = "G8th"; break;
    case bsrNoteValueKind::kNoteValueG16th: result = "G16th"; break;
    case bsrNoteValueKind::kNoteValueG32nd: result = "G32nd"; break;
    case bsrNoteValueKind::kNoteValueG64th: result = "G64th"; break;
    case bsrNoteValueKind::kNoteValueG128th: result = "G128th"; break;
    case bsrNoteValueKind::kNoteValueG256th: result = "G256th"; break;

    case bsrNoteValueKind::kNoteValueABreve: result = "ABreve"; break;
    case bsrNoteValueKind::kNoteValueAWhole: result = "AWhole"; break;
    case bsrNoteValueKind::kNoteValueAHalf: result = "AHalf"; break;
    case bsrNoteValueKind::kNoteValueAQuarter: result = "AQuarter"; break;
    case bsrNoteValueKind::kNoteValueA8th: result = "A8th"; break;
    case bsrNoteValueKind::kNoteValueA16th: result = "A16th"; break;
    case bsrNoteValueKind::kNoteValueA32nd: result = "A32nd"; break;
    case bsrNoteValueKind::kNoteValueA64th: result = "A64th"; break;
    case bsrNoteValueKind::kNoteValueA128th: result = "A128th"; break;
    case bsrNoteValueKind::kNoteValueA256th: result = "A256th"; break;

    case bsrNoteValueKind::kNoteValueBBreve: result = "BBreve"; break;
    case bsrNoteValueKind::kNoteValueBWhole: result = "BWhole"; break;
    case bsrNoteValueKind::kNoteValueBHalf: result = "BHalf"; break;
    case bsrNoteValueKind::kNoteValueBQuarter: result = "BQuarter"; break;
    case bsrNoteValueKind::kNoteValueB8th: result = "B8th"; break;
    case bsrNoteValueKind::kNoteValueB16th: result = "B16th"; break;
    case bsrNoteValueKind::kNoteValueB32nd: result = "B32nd"; break;
    case bsrNoteValueKind::kNoteValueB64th: result = "B64th"; break;
    case bsrNoteValueKind::kNoteValueB128th: result = "B128th"; break;
    case bsrNoteValueKind::kNoteValueB256th: result = "B256th"; break;
  } // switch

  return result;
}

std::string bsrNoteValueKindAsDebugString (
  bsrNoteValueKind noteValueKind)
{
  std::string result;

  switch (noteValueKind) {
    case bsrNoteValueKind::kNoteValueNone: result = "_"; break;

    case bsrNoteValueKind::kNoteValueRestBreve: result = "rBrv"; break;
    case bsrNoteValueKind::kNoteValueRestWhole: result = "r1"; break;
    case bsrNoteValueKind::kNoteValueRestHalf: result = "r2"; break;
    case bsrNoteValueKind::kNoteValueRestQuarter: result = "r4"; break;
    case bsrNoteValueKind::kNoteValueRest8th: result = "r8"; break;
    case bsrNoteValueKind::kNoteValueRest16th: result = "r16"; break;
    case bsrNoteValueKind::kNoteValueRest32nd: result = "r32"; break;
    case bsrNoteValueKind::kNoteValueRest64th: result = "r64"; break;
    case bsrNoteValueKind::kNoteValueRest128th: result = "r128"; break;
    case bsrNoteValueKind::kNoteValueRest256th: result = "r256"; break;

    case bsrNoteValueKind::kNoteValueCBreve: result = "cBrv"; break;
    case bsrNoteValueKind::kNoteValueCWhole: result = "c1"; break;
    case bsrNoteValueKind::kNoteValueCHalf: result = "c2"; break;
    case bsrNoteValueKind::kNoteValueCQuarter: result = "c4"; break;
    case bsrNoteValueKind::kNoteValueC8th: result = "c8"; break;
    case bsrNoteValueKind::kNoteValueC16th: result = "c16"; break;
    case bsrNoteValueKind::kNoteValueC32nd: result = "c32"; break;
    case bsrNoteValueKind::kNoteValueC64th: result = "c64"; break;
    case bsrNoteValueKind::kNoteValueC128th: result = "c128"; break;
    case bsrNoteValueKind::kNoteValueC256th: result = "c256"; break;

    case bsrNoteValueKind::kNoteValueDBreve: result = "dBrv"; break;
    case bsrNoteValueKind::kNoteValueDWhole: result = "d1"; break;
    case bsrNoteValueKind::kNoteValueDHalf: result = "d2"; break;
    case bsrNoteValueKind::kNoteValueDQuarter: result = "d4"; break;
    case bsrNoteValueKind::kNoteValueD8th: result = "d8"; break;
    case bsrNoteValueKind::kNoteValueD16th: result = "d16"; break;
    case bsrNoteValueKind::kNoteValueD32nd: result = "d32"; break;
    case bsrNoteValueKind::kNoteValueD64th: result = "d64"; break;
    case bsrNoteValueKind::kNoteValueD128th: result = "d128"; break;
    case bsrNoteValueKind::kNoteValueD256th: result = "d256"; break;

    case bsrNoteValueKind::kNoteValueEBreve: result = "eBrv"; break;
    case bsrNoteValueKind::kNoteValueEWhole: result = "e1"; break;
    case bsrNoteValueKind::kNoteValueEHalf: result = "e2"; break;
    case bsrNoteValueKind::kNoteValueEQuarter: result = "e4"; break;
    case bsrNoteValueKind::kNoteValueE8th: result = "e8"; break;
    case bsrNoteValueKind::kNoteValueE16th: result = "e16"; break;
    case bsrNoteValueKind::kNoteValueE32nd: result = "e32"; break;
    case bsrNoteValueKind::kNoteValueE64th: result = "e64"; break;
    case bsrNoteValueKind::kNoteValueE128th: result = "e128"; break;
    case bsrNoteValueKind::kNoteValueE256th: result = "e256"; break;

    case bsrNoteValueKind::kNoteValueFBreve: result = "fBrv"; break;
    case bsrNoteValueKind::kNoteValueFWhole: result = "f1"; break;
    case bsrNoteValueKind::kNoteValueFHalf: result = "f2"; break;
    case bsrNoteValueKind::kNoteValueFQuarter: result = "f4"; break;
    case bsrNoteValueKind::kNoteValueF8th: result = "f8"; break;
    case bsrNoteValueKind::kNoteValueF16th: result = "f16"; break;
    case bsrNoteValueKind::kNoteValueF32nd: result = "f32"; break;
    case bsrNoteValueKind::kNoteValueF64th: result = "f64"; break;
    case bsrNoteValueKind::kNoteValueF128th: result = "f128"; break;
    case bsrNoteValueKind::kNoteValueF256th: result = "f256"; break;

    case bsrNoteValueKind::kNoteValueGBreve: result = "gBrv"; break;
    case bsrNoteValueKind::kNoteValueGWhole: result = "g1"; break;
    case bsrNoteValueKind::kNoteValueGHalf: result = "g2"; break;
    case bsrNoteValueKind::kNoteValueGQuarter: result = "g4"; break;
    case bsrNoteValueKind::kNoteValueG8th: result = "g8"; break;
    case bsrNoteValueKind::kNoteValueG16th: result = "g16"; break;
    case bsrNoteValueKind::kNoteValueG32nd: result = "g32"; break;
    case bsrNoteValueKind::kNoteValueG64th: result = "g64"; break;
    case bsrNoteValueKind::kNoteValueG128th: result = "g128"; break;
    case bsrNoteValueKind::kNoteValueG256th: result = "g256"; break;

    case bsrNoteValueKind::kNoteValueABreve: result = "aBrv"; break;
    case bsrNoteValueKind::kNoteValueAWhole: result = "a1"; break;
    case bsrNoteValueKind::kNoteValueAHalf: result = "a2"; break;
    case bsrNoteValueKind::kNoteValueAQuarter: result = "a4"; break;
    case bsrNoteValueKind::kNoteValueA8th: result = "a8"; break;
    case bsrNoteValueKind::kNoteValueA16th: result = "a16"; break;
    case bsrNoteValueKind::kNoteValueA32nd: result = "a32"; break;
    case bsrNoteValueKind::kNoteValueA64th: result = "a64"; break;
    case bsrNoteValueKind::kNoteValueA128th: result = "a128"; break;
    case bsrNoteValueKind::kNoteValueA256th: result = "a256"; break;

    case bsrNoteValueKind::kNoteValueBBreve: result = "bBrv"; break;
    case bsrNoteValueKind::kNoteValueBWhole: result = "b1"; break;
    case bsrNoteValueKind::kNoteValueBHalf: result = "b2"; break;
    case bsrNoteValueKind::kNoteValueBQuarter: result = "b4"; break;
    case bsrNoteValueKind::kNoteValueB8th: result = "b8"; break;
    case bsrNoteValueKind::kNoteValueB16th: result = "b16"; break;
    case bsrNoteValueKind::kNoteValueB32nd: result = "b32"; break;
    case bsrNoteValueKind::kNoteValueB64th: result = "b64"; break;
    case bsrNoteValueKind::kNoteValueB128th: result = "b128"; break;
    case bsrNoteValueKind::kNoteValueB256th: result = "b256"; break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrNoteValueKind& elt)
{
  os << bsrNoteValueKindAsString (elt);
  return os;
}

std::string bsrNoteValueSizeKindAsString (
  bsrNoteValueSizeKind noteValueSizeKind)
{
  std::string result;

  switch (noteValueSizeKind) {
    case bsrNoteValueSizeKind::kNoteValueSizeNone:
      result = "None";
      break;
    case bsrNoteValueSizeKind::kNoteValueSizeLarger:
      result = "noteValueSizeLarger";
      break;
    case bsrNoteValueSizeKind::kNoteValueSizeSmaller:
      result = "noteValueSizeSmaller";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrNoteValueSizeKind& elt)
{
  os << bsrNoteValueSizeKindAsString (elt);
  return os;
}

std::string bsrNoteOctaveIsNeededAsString (
  bsrNoteOctaveIsNeeded noteOctaveIsNeeded)
{
  std::string result;

  switch (noteOctaveIsNeeded) {
    case bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededYes:
      result = "noteOctaveIsNeededYes";
      break;
    case bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededNo:
      result = "noteOctaveIsNeededNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrNoteOctaveIsNeeded& elt)
{
  os << bsrNoteOctaveIsNeededAsString (elt);
  return os;
}

std::string bsrAccidentalKindAsString (
  bsrAccidentalKind accidentalKind)
{
  std::string result;

  switch (accidentalKind) {
    case bsrAccidentalKind::kAccidentalNone:
      result = "accidentalNone";
      break;

    case bsrAccidentalKind::kAccidentalSharp:
      result = "accidentalSharp";
      break;
    case bsrAccidentalKind::kAccidentalNatural:
      result = "accidentalNatural";
      break;
    case bsrAccidentalKind::kAccidentalFlat:
      result = "accidentalFlat";
      break;

    case bsrAccidentalKind::kAccidentalDoubleFlat:
      result = "accidentalDoubleFlat";
      break;
    case bsrAccidentalKind::kAccidentalDoubleSharp:
      result = "accidentalDoubleSharp";
      break;

    case bsrAccidentalKind::kAccidentalQuarterSharp:
      result = "accidentalQuarterSharp";
      break;
    case bsrAccidentalKind::kAccidentalQuarterFlat:
      result = "accidentalQuarterFlat";
      break;

    case bsrAccidentalKind::kAccidentalThreeQuarterSharp:
      result = "accidentalThreeQuarterSharp";
      break;

    case bsrAccidentalKind::kAccidentalThreeQuarterFlat:
      result = "accidentalThreeQuarterFlat";
      break;
  } // switch

  return result;
}

std::string bsrAccidentalKindAsDebugString (
  bsrAccidentalKind accidentalKind)
{
  std::string result;

  switch (accidentalKind) {
    case bsrAccidentalKind::kAccidentalNone:
      result = "";
      break;

    case bsrAccidentalKind::kAccidentalSharp:
      result = "#";
      break;
    case bsrAccidentalKind::kAccidentalNatural:
      result = "_";
      break;
    case bsrAccidentalKind::kAccidentalFlat:
      result = "b";
      break;

    case bsrAccidentalKind::kAccidentalDoubleFlat:
      result = "bb";
      break;
    case bsrAccidentalKind::kAccidentalDoubleSharp:
      result = "##";
      break;

    case bsrAccidentalKind::kAccidentalQuarterSharp:
      result = "q#";
      break;
    case bsrAccidentalKind::kAccidentalQuarterFlat:
      result = "qb";
      break;

    case bsrAccidentalKind::kAccidentalThreeQuarterSharp:
      result = "3q#";
      break;

    case bsrAccidentalKind::kAccidentalThreeQuarterFlat:
      result = "3qb";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrAccidentalKind& elt)
{
  os << bsrAccidentalKindAsString (elt);
  return os;
}

std::string bsrNote::asString () const
{
  std::stringstream s;

  s <<
    "Note" <<
    ", fNoteValueKind: " << fNoteValueKind <<
    ", fNoteDotsNumber: " << fNoteDotsNumber <<
    ", fNoteOctaveKind: " << fNoteOctaveKind <<
    ", fNoteValueSizeIsNeeded: " << fNoteValueSizeIsNeeded <<
    ", fNoteOctaveIsNeeded: " << fNoteOctaveIsNeeded <<
    ", fAccidentalKind: " << fAccidentalKind <<
    ", cellsList (): " <<
    buildCellsList ()->asShortString () <<
    ", fSpacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

std::string bsrNote::asShortString () const // finalize this JMI
{
  std::stringstream s;

  s <<
    "Note" <<
    ", fNoteValueKind: " << fNoteValueKind <<
    ", fNoteDotsNumber: " << fNoteDotsNumber <<
    ", fNoteOctaveKind: " << fNoteOctaveKind <<
    ", fNoteValueSizeIsNeeded: " <<fNoteValueSizeIsNeeded <<
    ", fNoteOctaveIsNeeded: " <<fNoteOctaveIsNeeded <<
    ", fAccidentalKind: " <<fAccidentalKind <<
    ", cellsList (): " <<
    buildCellsList ()->asShortString () <<
    ", fSpacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

std::string bsrNote::asDebugString () const
{
  std::stringstream s;

  switch (fNoteOctaveIsNeeded) {
    case bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededYes:
      s <<
        bsrNoteOctaveKindAsDebugString (fNoteOctaveKind);
      break;
    case bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededNo:
      break;
  } // switch

  s <<
    bsrNoteValueKindAsDebugString (fNoteValueKind) <<
    bsrAccidentalKindAsDebugString (fAccidentalKind);

  for (int i = 1; i <= fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  return s.str ();
}

void bsrNote::print (std::ostream& os) const
{
  os <<
    "Note" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 23;

  os <<
    std::setw (fieldWidth) <<
    "fNoteValueKind" << ": " << fNoteValueKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteDotsNumber" << ": " << fNoteDotsNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteOctaveKind" << ": " <<  fNoteOctaveKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteValueSizeIsNeeded" << ": " << fNoteValueSizeIsNeeded <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteOctaveIsNeeded" << ": " << fNoteOctaveIsNeeded <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fAccidentalKind" << ": " << fAccidentalKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "cellsList" << ": " <<
    buildCellsList ()->asShortString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSpacesBefore" << ": " << fSpacesBefore <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrNote& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
