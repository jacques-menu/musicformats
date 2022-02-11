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


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string noteOctaveKindAsString (
  bsrNoteOctaveKind noteOctaveKind)
{
  string result;

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

string noteOctaveKindAsDebugString (
  bsrNoteOctaveKind noteOctaveKind)
{
  string result;

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

S_bsrCellsList bsrNote::noteValueKindAsCellsList (
  int              inputLineNumber,
  bsrNoteValueKind noteValueKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (noteValueKind) {
    case kNoteValueNone:
      break;

    case kNoteValueRestBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots134);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case kNoteValueRestWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots134);
      break;
    case kNoteValueRestHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots136);
      break;
    case kNoteValueRestQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots1236);
      break;
    case kNoteValueRest8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1346);
      break;
    case kNoteValueRest16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots134);
      break;
    case kNoteValueRest32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots136);
      break;
    case kNoteValueRest64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1236);
      break;
    case kNoteValueRest128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1346);
      break;
    case kNoteValueRest256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots134);
      break;

    case kNoteValueCBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots13456);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case kNoteValueCWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots13456);
      break;
    case kNoteValueCHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots1345);
      break;
    case kNoteValueCQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots1456);
      break;
    case kNoteValueC8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots145);
      break;
    case kNoteValueC16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots13456);
      break;
    case kNoteValueC32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots1345);
      break;
    case kNoteValueC64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1456);
      break;
    case kNoteValueC128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots145);
      break;
    case kNoteValueC256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots13456);
      break;

    case kNoteValueDBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots1356);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case kNoteValueDWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots1356);
      break;
    case kNoteValueDHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots135);
      break;
    case kNoteValueDQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots156);
      break;
    case kNoteValueD8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots15);
      break;
    case kNoteValueD16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1356);
      break;
    case kNoteValueD32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots135);
      break;
    case kNoteValueD64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots156);
      break;
    case kNoteValueD128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots15);
      break;
    case kNoteValueD256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots1356);
      break;

    case kNoteValueEBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots12346);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case kNoteValueEWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots12346);
      break;
    case kNoteValueEHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots1234);
      break;
    case kNoteValueEQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots1246);
      break;
    case kNoteValueE8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots124);
      break;
    case kNoteValueE16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots12346);
      break;
    case kNoteValueE32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots1234);
      break;
    case kNoteValueE64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1246);
      break;
    case kNoteValueE128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots124);
      break;
    case kNoteValueE256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots12346);
      break;

    case kNoteValueFBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots123456);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case kNoteValueFWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots123456);
      break;
    case kNoteValueFHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots12345);
      break;
    case kNoteValueFQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots12456);
      break;
    case kNoteValueF8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1245);
      break;
    case kNoteValueF16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots123456);
      break;
    case kNoteValueF32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots12345);
      break;
    case kNoteValueF64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots12456);
      break;
    case kNoteValueF128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1245);
      break;
    case kNoteValueF256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots123456);
      break;

    case kNoteValueGBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots12356);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case kNoteValueGWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots12356);
      break;
    case kNoteValueGHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots1235);
      break;
    case kNoteValueGQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots1256);
      break;
    case kNoteValueG8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots125);
      break;
    case kNoteValueG16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots12356);
      break;
    case kNoteValueG32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots1235);
      break;
    case kNoteValueG64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots1256);
      break;
    case kNoteValueG128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots125);
      break;
    case kNoteValueG256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots12356);
      break;

    case kNoteValueABreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots2346);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case kNoteValueAWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots2346);
      break;
    case kNoteValueAHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots234);
      break;
    case kNoteValueAQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots246);
      break;
    case kNoteValueA8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots24);
      break;
    case kNoteValueA16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots2346);
      break;
    case kNoteValueA32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots234);
      break;
    case kNoteValueA64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots246);
      break;
    case kNoteValueA128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots24);
      break;
    case kNoteValueA256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots2346);
      break;

    case kNoteValueBBreve:
      result->appendCellKindToCellsList (bsrCellKind::kDots23456);
      result->appendCellKindToCellsList (bsrCellKind::kDots13);
      break;
    case kNoteValueBWhole:
      result->appendCellKindToCellsList (bsrCellKind::kDots23456);
      break;
    case kNoteValueBHalf:
      result->appendCellKindToCellsList (bsrCellKind::kDots2345);
      break;
    case kNoteValueBQuarter:
      result->appendCellKindToCellsList (bsrCellKind::kDots2456);
      break;
    case kNoteValueB8th:
      result->appendCellKindToCellsList (bsrCellKind::kDots245);
      break;
    case kNoteValueB16th:
      result->appendCellKindToCellsList (bsrCellKind::kDots23456);
      break;
    case kNoteValueB32nd:
      result->appendCellKindToCellsList (bsrCellKind::kDots2345);
      break;
    case kNoteValueB64th:
      result->appendCellKindToCellsList (bsrCellKind::kDots2456);
      break;
    case kNoteValueB128th:
      result->appendCellKindToCellsList (bsrCellKind::kDots245);
      break;
    case kNoteValueB256th:
      result->appendCellKindToCellsList (bsrCellKind::kDots56);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      result->appendCellKindToCellsList (bsrCellKind::kDots23456);
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::noteValueKindAsCellsList () const
{
  return
    noteValueKindAsCellsList (
      fInputLineNumber,
      fNoteValueKind);
}

S_bsrCellsList bsrNote::noteValueSizeKindAsCellsList () const
{
  return
    noteValueSizeKindAsCellsList (
      fInputLineNumber,
      noteValueSizeKindFromNoteValueKind (
        fNoteValueKind));
}

bsrNote::bsrNoteValueSizeKind bsrNote::noteValueSizeKindFromNoteValueKind (
  bsrNoteValueKind noteValueKind)
{
  bsrNoteValueSizeKind
    result = bsrNote::kNoteValueSizeNone;

  switch (noteValueKind) {
    case kNoteValueNone:
      break;

    case kNoteValueRestBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRestWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRestHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRestQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRest8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRest16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueRest32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueRest64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueRest128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueRest256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueCBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueCWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueCHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueCQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueC8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueC16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueC32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueC64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueC128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueC256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueDBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueDWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueDHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueDQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueD8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueD16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueD32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueD64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueD128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueD256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueEBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueEWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueEHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueEQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueE8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueE16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueE32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueE64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueE128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueE256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueFBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueFWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueFHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueFQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueF8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueF16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueF32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueF64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueF128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueF256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueGBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueGWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueGHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueGQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueG8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueG16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueG32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueG64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueG128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueG256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueABreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueAWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueAHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueAQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueA8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueA16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueA32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueA64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueA128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueA256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueBBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueBWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueBHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueBQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueB8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueB16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueB32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueB64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueB128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueB256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::noteValueSizeKindAsCellsList (
  int                  inputLineNumber,
  bsrNoteValueSizeKind noteValueSizeKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (noteValueSizeKind) {
    case bsrNote::kNoteValueSizeNone:
      break;
    case bsrNote::kNoteValueSizeLarger:
      result->appendCellKindToCellsList (bsrCellKind::kDots45);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      break;
    case bsrNote::kNoteValueSizeSmaller:
      result->appendCellKindToCellsList (bsrCellKind::kDots6);
      result->appendCellKindToCellsList (bsrCellKind::kDots126);
      result->appendCellKindToCellsList (bsrCellKind::kDots2);
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::accidentalKindAsCellsList (
  int                   inputLineNumber,
  bsrAccidentalKind accidentalKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (accidentalKind) {
    case bsrNote::bsrAccidentalKind::kAccidentalNone:
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalSharp:
      result->appendCellKindToCellsList (kCellSharp);
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalNatural:
      result->appendCellKindToCellsList (kCellNatural);
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalFlat:
      result->appendCellKindToCellsList (kCellFlat);
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalDoubleFlat:
      result->appendCellKindToCellsList (kCellFlat);
      result->appendCellKindToCellsList (kCellFlat);
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalDoubleSharp:
      result->appendCellKindToCellsList (kCellSharp);
      result->appendCellKindToCellsList (kCellSharp);
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalQuarterSharp:
      result->appendCellKindToCellsList (bsrCellKind::kDots4);
      result->appendCellKindToCellsList (kCellSharp);
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalQuarterFlat:
      result->appendCellKindToCellsList (bsrCellKind::kDots4);
      result->appendCellKindToCellsList (kCellFlat);
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalThreeQuarterSharp:
      result->appendCellKindToCellsList (bsrCellKind::kDots456);
      result->appendCellKindToCellsList (kCellSharp);
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalThreeQuarterFlat:
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
      noteValueSizeKindAsCellsList ());
  }

  // append note accidental if any
  result->appendCellsListToCellsList (
    accidentalKindAsCellsList ());

  // append note octave if needed
  switch (fNoteOctaveIsNeeded) {
    case bsrNote::kNoteOctaveIsNeededYes:
      result->appendCellsListToCellsList (
        noteOctaveKindAsCellsList (
          fInputLineNumber,
          fNoteOctaveKind));
      break;
    case bsrNote::kNoteOctaveIsNeededNo:
      break;
  } // switch

  // append note value
  result->appendCellsListToCellsList (
    noteValueKindAsCellsList ());

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
      endl;
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
            endl;
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
      endl;
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
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrNote::browseData (basevisitor* v)
{}

string bsrNote::noteValueKindAsString (
  bsrNoteValueKind noteValueKind)
{
  string result;

  switch (noteValueKind) {
    case kNoteValueNone: result = "None"; break;

    case kNoteValueRestBreve: result = "RestBreve"; break;
    case kNoteValueRestWhole: result = "RestWhole"; break;
    case kNoteValueRestHalf: result = "RestHalf"; break;
    case kNoteValueRestQuarter: result = "RestQuarter"; break;
    case kNoteValueRest8th: result = "Rest8th"; break;
    case kNoteValueRest16th: result = "Rest16th"; break;
    case kNoteValueRest32nd: result = "Rest32nd"; break;
    case kNoteValueRest64th: result = "Rest64th"; break;
    case kNoteValueRest128th: result = "Rest128th"; break;
    case kNoteValueRest256th: result = "Rest256th"; break;

    case kNoteValueCBreve: result = "CBreve"; break;
    case kNoteValueCWhole: result = "CWhole"; break;
    case kNoteValueCHalf: result = "CHalf"; break;
    case kNoteValueCQuarter: result = "CQuarter"; break;
    case kNoteValueC8th: result = "C8th"; break;
    case kNoteValueC16th: result = "C16th"; break;
    case kNoteValueC32nd: result = "C32nd"; break;
    case kNoteValueC64th: result = "C64th"; break;
    case kNoteValueC128th: result = "C128th"; break;
    case kNoteValueC256th: result = "C256th"; break;

    case kNoteValueDBreve: result = "DBreve"; break;
    case kNoteValueDWhole: result = "DWhole"; break;
    case kNoteValueDHalf: result = "DHalf"; break;
    case kNoteValueDQuarter: result = "DQuarter"; break;
    case kNoteValueD8th: result = "D8th"; break;
    case kNoteValueD16th: result = "D16th"; break;
    case kNoteValueD32nd: result = "D32nd"; break;
    case kNoteValueD64th: result = "D64th"; break;
    case kNoteValueD128th: result = "D128th"; break;
    case kNoteValueD256th: result = "D256th"; break;

    case kNoteValueEBreve: result = "EBreve"; break;
    case kNoteValueEWhole: result = "EWhole"; break;
    case kNoteValueEHalf: result = "EHalf"; break;
    case kNoteValueEQuarter: result = "EQuarter"; break;
    case kNoteValueE8th: result = "E8th"; break;
    case kNoteValueE16th: result = "E16th"; break;
    case kNoteValueE32nd: result = "E32nd"; break;
    case kNoteValueE64th: result = "E64th"; break;
    case kNoteValueE128th: result = "E128th"; break;
    case kNoteValueE256th: result = "E256th"; break;

    case kNoteValueFBreve: result = "FBreve"; break;
    case kNoteValueFWhole: result = "FWhole"; break;
    case kNoteValueFHalf: result = "FHalf"; break;
    case kNoteValueFQuarter: result = "FQuarter"; break;
    case kNoteValueF8th: result = "F8th"; break;
    case kNoteValueF16th: result = "F16th"; break;
    case kNoteValueF32nd: result = "F32nd"; break;
    case kNoteValueF64th: result = "F64th"; break;
    case kNoteValueF128th: result = "F128th"; break;
    case kNoteValueF256th: result = "F256th"; break;

    case kNoteValueGBreve: result = "GBreve"; break;
    case kNoteValueGWhole: result = "GWhole"; break;
    case kNoteValueGHalf: result = "GHalf"; break;
    case kNoteValueGQuarter: result = "GQuarter"; break;
    case kNoteValueG8th: result = "G8th"; break;
    case kNoteValueG16th: result = "G16th"; break;
    case kNoteValueG32nd: result = "G32nd"; break;
    case kNoteValueG64th: result = "G64th"; break;
    case kNoteValueG128th: result = "G128th"; break;
    case kNoteValueG256th: result = "G256th"; break;

    case kNoteValueABreve: result = "ABreve"; break;
    case kNoteValueAWhole: result = "AWhole"; break;
    case kNoteValueAHalf: result = "AHalf"; break;
    case kNoteValueAQuarter: result = "AQuarter"; break;
    case kNoteValueA8th: result = "A8th"; break;
    case kNoteValueA16th: result = "A16th"; break;
    case kNoteValueA32nd: result = "A32nd"; break;
    case kNoteValueA64th: result = "A64th"; break;
    case kNoteValueA128th: result = "A128th"; break;
    case kNoteValueA256th: result = "A256th"; break;

    case kNoteValueBBreve: result = "BBreve"; break;
    case kNoteValueBWhole: result = "BWhole"; break;
    case kNoteValueBHalf: result = "BHalf"; break;
    case kNoteValueBQuarter: result = "BQuarter"; break;
    case kNoteValueB8th: result = "B8th"; break;
    case kNoteValueB16th: result = "B16th"; break;
    case kNoteValueB32nd: result = "B32nd"; break;
    case kNoteValueB64th: result = "B64th"; break;
    case kNoteValueB128th: result = "B128th"; break;
    case kNoteValueB256th: result = "B256th"; break;
  } // switch

  return result;
}

string bsrNote::noteValueKindAsDebugString (
  bsrNoteValueKind noteValueKind)
{
  string result;

  switch (noteValueKind) {
    case kNoteValueNone: result = "_"; break;

    case kNoteValueRestBreve: result = "rBrv"; break;
    case kNoteValueRestWhole: result = "r1"; break;
    case kNoteValueRestHalf: result = "r2"; break;
    case kNoteValueRestQuarter: result = "r4"; break;
    case kNoteValueRest8th: result = "r8"; break;
    case kNoteValueRest16th: result = "r16"; break;
    case kNoteValueRest32nd: result = "r32"; break;
    case kNoteValueRest64th: result = "r64"; break;
    case kNoteValueRest128th: result = "r128"; break;
    case kNoteValueRest256th: result = "r256"; break;

    case kNoteValueCBreve: result = "cBrv"; break;
    case kNoteValueCWhole: result = "c1"; break;
    case kNoteValueCHalf: result = "c2"; break;
    case kNoteValueCQuarter: result = "c4"; break;
    case kNoteValueC8th: result = "c8"; break;
    case kNoteValueC16th: result = "c16"; break;
    case kNoteValueC32nd: result = "c32"; break;
    case kNoteValueC64th: result = "c64"; break;
    case kNoteValueC128th: result = "c128"; break;
    case kNoteValueC256th: result = "c256"; break;

    case kNoteValueDBreve: result = "dBrv"; break;
    case kNoteValueDWhole: result = "d1"; break;
    case kNoteValueDHalf: result = "d2"; break;
    case kNoteValueDQuarter: result = "d4"; break;
    case kNoteValueD8th: result = "d8"; break;
    case kNoteValueD16th: result = "d16"; break;
    case kNoteValueD32nd: result = "d32"; break;
    case kNoteValueD64th: result = "d64"; break;
    case kNoteValueD128th: result = "d128"; break;
    case kNoteValueD256th: result = "d256"; break;

    case kNoteValueEBreve: result = "eBrv"; break;
    case kNoteValueEWhole: result = "e1"; break;
    case kNoteValueEHalf: result = "e2"; break;
    case kNoteValueEQuarter: result = "e4"; break;
    case kNoteValueE8th: result = "e8"; break;
    case kNoteValueE16th: result = "e16"; break;
    case kNoteValueE32nd: result = "e32"; break;
    case kNoteValueE64th: result = "e64"; break;
    case kNoteValueE128th: result = "e128"; break;
    case kNoteValueE256th: result = "e256"; break;

    case kNoteValueFBreve: result = "fBrv"; break;
    case kNoteValueFWhole: result = "f1"; break;
    case kNoteValueFHalf: result = "f2"; break;
    case kNoteValueFQuarter: result = "f4"; break;
    case kNoteValueF8th: result = "f8"; break;
    case kNoteValueF16th: result = "f16"; break;
    case kNoteValueF32nd: result = "f32"; break;
    case kNoteValueF64th: result = "f64"; break;
    case kNoteValueF128th: result = "f128"; break;
    case kNoteValueF256th: result = "f256"; break;

    case kNoteValueGBreve: result = "gBrv"; break;
    case kNoteValueGWhole: result = "g1"; break;
    case kNoteValueGHalf: result = "g2"; break;
    case kNoteValueGQuarter: result = "g4"; break;
    case kNoteValueG8th: result = "g8"; break;
    case kNoteValueG16th: result = "g16"; break;
    case kNoteValueG32nd: result = "g32"; break;
    case kNoteValueG64th: result = "g64"; break;
    case kNoteValueG128th: result = "g128"; break;
    case kNoteValueG256th: result = "g256"; break;

    case kNoteValueABreve: result = "aBrv"; break;
    case kNoteValueAWhole: result = "a1"; break;
    case kNoteValueAHalf: result = "a2"; break;
    case kNoteValueAQuarter: result = "a4"; break;
    case kNoteValueA8th: result = "a8"; break;
    case kNoteValueA16th: result = "a16"; break;
    case kNoteValueA32nd: result = "a32"; break;
    case kNoteValueA64th: result = "a64"; break;
    case kNoteValueA128th: result = "a128"; break;
    case kNoteValueA256th: result = "a256"; break;

    case kNoteValueBBreve: result = "bBrv"; break;
    case kNoteValueBWhole: result = "b1"; break;
    case kNoteValueBHalf: result = "b2"; break;
    case kNoteValueBQuarter: result = "b4"; break;
    case kNoteValueB8th: result = "b8"; break;
    case kNoteValueB16th: result = "b16"; break;
    case kNoteValueB32nd: result = "b32"; break;
    case kNoteValueB64th: result = "b64"; break;
    case kNoteValueB128th: result = "b128"; break;
    case kNoteValueB256th: result = "b256"; break;
  } // switch

  return result;
}

string bsrNote::noteValueSizeKindAsString (
  bsrNoteValueSizeKind noteValueSizeKind)
{
  string result;

  switch (noteValueSizeKind) {
    case bsrNote::kNoteValueSizeNone:
      result = "None";
      break;
    case bsrNote::kNoteValueSizeLarger:
      result = "noteValueSizeLarger";
      break;
    case bsrNote::kNoteValueSizeSmaller:
      result = "noteValueSizeSmaller";
      break;
  } // switch

  return result;
}

string bsrNote::noteOctaveIsNeededAsString (
  bsrNoteOctaveIsNeeded noteOctaveIsNeeded)
{
  string result;

  switch (noteOctaveIsNeeded) {
    case bsrNote::kNoteOctaveIsNeededYes:
      result = "noteOctaveIsNeededYes";
      break;
    case bsrNote::kNoteOctaveIsNeededNo:
      result = "noteOctaveIsNeededNo";
      break;
  } // switch

  return result;
}

string bsrNote::msrAccidentalKindAsString (
  bsrAccidentalKind accidentalKind)
{
  string result;

  switch (accidentalKind) {
    case bsrNote::bsrAccidentalKind::kAccidentalNone:
      result = "accidentalNone";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalSharp:
      result = "accidentalSharp";
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalNatural:
      result = "accidentalNatural";
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalFlat:
      result = "accidentalFlat";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalDoubleFlat:
      result = "accidentalDoubleFlat";
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalDoubleSharp:
      result = "accidentalDoubleSharp";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalQuarterSharp:
      result = "accidentalQuarterSharp";
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalQuarterFlat:
      result = "accidentalQuarterFlat";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalThreeQuarterSharp:
      result = "accidentalThreeQuarterSharp";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalThreeQuarterFlat:
      result = "accidentalThreeQuarterFlat";
      break;
  } // switch

  return result;
}

string bsrNote::accidentalKindAsDebugString (
  bsrAccidentalKind accidentalKind)
{
  string result;

  switch (accidentalKind) {
    case bsrNote::bsrAccidentalKind::kAccidentalNone:
      result = "";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalSharp:
      result = "#";
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalNatural:
      result = "_";
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalFlat:
      result = "b";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalDoubleFlat:
      result = "bb";
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalDoubleSharp:
      result = "##";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalQuarterSharp:
      result = "q#";
      break;
    case bsrNote::bsrAccidentalKind::kAccidentalQuarterFlat:
      result = "qb";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalThreeQuarterSharp:
      result = "3q#";
      break;

    case bsrNote::bsrAccidentalKind::kAccidentalThreeQuarterFlat:
      result = "3qb";
      break;
  } // switch

  return result;
}

string bsrNote::asString () const
{
  stringstream s;

  s <<
    "Note" <<
    ", noteValue: " <<
    noteValueKindAsString (fNoteValueKind) <<
    ", noteDotsNumber: " << fNoteDotsNumber <<
    ", noteOctaveKind: " <<
    noteOctaveKindAsString (fNoteOctaveKind) <<
    ", noteValueSizeIsNeeded: " <<
    fNoteValueSizeIsNeeded <<
    ", noteOctaveIsNeeded: " <<
    noteOctaveIsNeededAsString (fNoteOctaveIsNeeded) <<
    ", msrAccidentalKindAsString: " <<
    msrAccidentalKindAsString (fAccidentalKind) <<
    ", cellsList (): " <<
    buildCellsList ()->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrNote::asShortString () const // finalize this JMI
{
  stringstream s;

  s <<
    "Note" <<
    ", noteValue: " <<
    noteValueKindAsString (fNoteValueKind) <<
    ", noteDotsNumber: " << fNoteDotsNumber <<
    ", noteOctaveKind: " <<
    noteOctaveKindAsString (fNoteOctaveKind) <<
    ", noteValueSizeIsNeeded: " <<
    fNoteValueSizeIsNeeded <<
    ", noteOctaveIsNeeded: " <<
    noteOctaveIsNeededAsString (fNoteOctaveIsNeeded) <<
    ", msrAccidentalKindAsString: " <<
    msrAccidentalKindAsString (fAccidentalKind) <<
    ", cellsList (): " <<
    buildCellsList ()->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrNote::asDebugString () const
{
  stringstream s;

  switch (fNoteOctaveIsNeeded) {
    case bsrNote::kNoteOctaveIsNeededYes:
      s <<
        noteOctaveKindAsDebugString (fNoteOctaveKind);
      break;
    case bsrNote::kNoteOctaveIsNeededNo:
      break;
  } // switch

  s <<
    noteValueKindAsDebugString (fNoteValueKind) <<
    accidentalKindAsDebugString (fAccidentalKind);

  for (int i = 1; i <= fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  return s.str ();
}

void bsrNote::print (ostream& os) const
{
  os <<
    "Note" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 23;

  os <<
    setw (fieldWidth) <<
    "noteValue" << " : " <<
    noteValueKindAsString (fNoteValueKind) <<
    endl <<
    setw (fieldWidth) <<
    "noteDotsNumber" << " : " << fNoteDotsNumber <<
    endl <<
    setw (fieldWidth) <<
    "noteOctaveKind" << " : " <<
    noteOctaveKindAsString (fNoteOctaveKind) <<
    endl <<
    setw (fieldWidth) <<
    "noteValueSizeIsNeeded" << " : " <<
    fNoteValueSizeIsNeeded <<
    endl <<
    setw (fieldWidth) <<
    "noteOctaveIsNeeded" << " : " <<
    noteOctaveIsNeededAsString (fNoteOctaveIsNeeded) <<
    endl <<
    setw (fieldWidth) <<
    "accidentalKind" << " : " <<
    msrAccidentalKindAsString (fAccidentalKind) <<
    endl <<
    setw (fieldWidth) <<
    "cellsList" << " : " <<
    buildCellsList ()->asShortString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrNote& elt)
{
  elt->print (os);
  return os;
}


}
