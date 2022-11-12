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


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrTempoTupletTypeKind {
  kTempoTupletTypeNone,
  kTempoTupletTypeStart, kTempoTupletTypeStop
};

std::string msrTempoTupletTypeKindAsString (
  msrTempoTupletTypeKind tempoTupletTypeKind);

std::ostream& operator << (std::ostream& os, const msrTempoTupletTypeKind& elt);

enum class msrTempoTupletBracketKind {
  kTempoTupletBracketYes, kTempoTupletBracketNo
};

std::string msrTempoTupletBracketKindAsString (
  msrTempoTupletBracketKind tempoTupletBracketKind);

std::ostream& operator << (std::ostream& os, const msrTempoTupletBracketKind& elt);

enum class msrTempoTupletShowNumberKind {
  kTempoTupletShowNumberActual,
  kTempoTupletShowNumberBoth,
  kTempoTupletShowNumberNone
};

std::string msrTempoTupletShowNumberKindAsString (
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind);

std::ostream& operator << (std::ostream& os, const msrTempoTupletShowNumberKind& elt);

enum class msrTempoNotesRelationshipElementsKind {
  kTempoNotesRelationshipElementsLeft,
  kTempoNotesRelationshipElementsRight
};

std::string msrTempoNotesRelationshipElementsKindAsString (
  msrTempoNotesRelationshipElementsKind tempoNotesRelationshipElementsKind);

std::ostream& operator << (std::ostream& os, const msrTempoNotesRelationshipElementsKind& elt);

enum class msrTempoKBeatUnitsKind {
  kTempoBeatUnits_NO_,
  kTempoBeatUnitsWordsOnly,
  kTempoBeatUnitsPerMinute,
  kTempoBeatUnitsEquivalence,
  kTempoNotesRelationship
};

std::string msrTempoBeatUnitsKindAsString (
  msrTempoKBeatUnitsKind tempoKind);

std::ostream& operator << (std::ostream& os, const msrTempoKBeatUnitsKind& elt);

enum class msrTempoParenthesizedKind {
  kTempoParenthesizedYes, kTempoParenthesizedNo
};

std::string msrTempoParenthesizedKindAsString (
  msrTempoParenthesizedKind tempoParenthesizedKind);

std::ostream& operator << (std::ostream& os, const msrTempoParenthesizedKind& elt);

enum class msrTempoNotesRelationshipKind {
  kTempoNotesRelationshipNone, kTempoNotesRelationshipEquals
};

std::string msrTempoNotesRelationshipKindAsString (
  msrTempoNotesRelationshipKind tempoNotesRelationshipKind);

std::ostream& operator << (std::ostream& os, const msrTempoNotesRelationshipKind& elt);


}


#endif
