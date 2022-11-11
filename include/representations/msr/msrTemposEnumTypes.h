/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTemposEnumTypes___
#define ___msrTemposEnumTypes___

#include <string>
#include <ostream>


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrTempoTupletTypeKind {
  kTempoTupletTypeNone,
  kTempoTupletTypeStart, kTempoTupletTypeStop
};

string msrTempoTupletTypeKindAsString (
  msrTempoTupletTypeKind tempoTupletTypeKind);

ostream& operator << (ostream& os, const msrTempoTupletTypeKind& elt);

enum class msrTempoTupletBracketKind {
  kTempoTupletBracketYes, kTempoTupletBracketNo
};

string msrTempoTupletBracketKindAsString (
  msrTempoTupletBracketKind tempoTupletBracketKind);

ostream& operator << (ostream& os, const msrTempoTupletBracketKind& elt);

enum class msrTempoTupletShowNumberKind {
  kTempoTupletShowNumberActual,
  kTempoTupletShowNumberBoth,
  kTempoTupletShowNumberNone
};

string msrTempoTupletShowNumberKindAsString (
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind);

ostream& operator << (ostream& os, const msrTempoTupletShowNumberKind& elt);

enum class msrTempoNotesRelationshipElementsKind {
  kTempoNotesRelationshipElementsLeft,
  kTempoNotesRelationshipElementsRight
};

string msrTempoNotesRelationshipElementsKindAsString (
  msrTempoNotesRelationshipElementsKind tempoNotesRelationshipElementsKind);

ostream& operator << (ostream& os, const msrTempoNotesRelationshipElementsKind& elt);

enum class msrTempoKBeatUnitsKind {
  kTempoBeatUnits_NO_,
  kTempoBeatUnitsWordsOnly,
  kTempoBeatUnitsPerMinute,
  kTempoBeatUnitsEquivalence,
  kTempoNotesRelationship
};

string msrTempoBeatUnitsKindAsString (
  msrTempoKBeatUnitsKind tempoKind);

ostream& operator << (ostream& os, const msrTempoKBeatUnitsKind& elt);

enum class msrTempoParenthesizedKind {
  kTempoParenthesizedYes, kTempoParenthesizedNo
};

string msrTempoParenthesizedKindAsString (
  msrTempoParenthesizedKind tempoParenthesizedKind);

ostream& operator << (ostream& os, const msrTempoParenthesizedKind& elt);

enum class msrTempoNotesRelationshipKind {
  kTempoNotesRelationshipNone, kTempoNotesRelationshipEquals
};

string msrTempoNotesRelationshipKindAsString (
  msrTempoNotesRelationshipKind tempoNotesRelationshipKind);

ostream& operator << (ostream& os, const msrTempoNotesRelationshipKind& elt);


}


#endif
