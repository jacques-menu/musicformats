/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrNotesEnumTypes___
#define ___msrNotesEnumTypes___

#include <iomanip>

#include <string>
#include <ostream>


namespace MusicFormats
{

// notes
//______________________________________________________________________________

/*
  Measures can contain either:
    individual notes;
    tuplets;
    chords;
    double tremolos.

  Tuplets can contain either:
    individual notes;
    other tuplets;
    chords.

  Chords contain:
    individual notes.

  Grace notes groups can contain either (no tuplets here):
    individual notes;
    chords.

  Double tremolos can contain either:
    individual notes;
    chords.

  Notes can have attached:
    grace notes groups (before and after the note).

  Attempting to use classes to describe this graph would be a nightmare:
  we thus use enum class types and uplinks to handles the variants.
*/

enum class msrNoteKind {
  kNote_UNKNOWN_,

  // in measures
  kNoteRegularInMeasure,
  kNoteRestInMeasure,
  kNoteSkipInMeasure, // an invisible rest
  kNoteUnpitchedInMeasure,

  // in chords
  kNoteRegularInChord,

  // in tuplets
  kNoteRegularInTuplet,
  kNoteRestInTuplet,
  kNoteUnpitchedInTuplet,

  // in grace notes groups
  kNoteRegularInGraceNotesGroup,
  kNoteSkipInGraceNotesGroup, // used to circumvent LilyPond_Issue_34

  // in chords in grace notes groups
  kNoteInChordInGraceNotesGroup,

  // in tuplets in grace notes groups
  kNoteInTupletInGraceNotesGroup,

  // in double-tremolos
  kNoteInDoubleTremolo
};

EXP std::string msrNoteKindAsString (
  msrNoteKind noteKind);

std::ostream& operator << (std::ostream& os, const msrNoteKind& elt);

// solo notes or rests
//______________________________________________________________________________

/*
  A solo note or rest is one that is played alone,
  with no other note or rest being played at the same time

  In a voice, a solo note is one not a member of a chord,
  and a rest is always a solo note

  In a staff, a solo note
*/

enum class msrSoloNoteOrRestInVoiceKind {
  kSoloNoteOrRestInVoiceYes,
  kSoloNoteOrRestInVoiceNo
};

std::string msrSoloNoteOrRestInVoiceKindAsString (
  msrSoloNoteOrRestInVoiceKind soloNoteOrRestInVoiceKind);

std::ostream& operator << (std::ostream& os, const msrSoloNoteOrRestInVoiceKind& elt);

enum class msrSoloNoteOrRestInStaffKind {
  kSoloNoteOrRestInStaffYes,
  kSoloNoteOrRestInStaffPartially,
  kSoloNoteOrRestInStaffNo
};

std::string msrSoloNoteOrRestInStaffKindAsString (
  msrSoloNoteOrRestInStaffKind soloNoteOrRestInStaffKind);

std::ostream& operator << (std::ostream& os, const msrSoloNoteOrRestInStaffKind& elt);

//______________________________________________________________________________
// data types

enum class msrNoteHeadKind {
  kNoteHeadSlash,
  kNoteHeadTriangle, kNoteHeadDiamond, kNoteHeadSquare,
  kNoteHeadCross,kNoteHeadX,
  kNoteHeadCircleX, kNoteHeadInvertedTriangle,
  kNoteHeadArrowDown, kNoteHeadArrowUp,
  kNoteHeadSlashed, kNoteHeadBackSlashed,
  kNoteHeadNormal, kNoteHeadCluster,
  kNoteHeadCircleDot,
  kNoteHeadLeftTriangle,
  kNoteHeadRectangle,
  kNoteHeadNone,
  kNoteHeadDo, kNoteHeadRe, kNoteHeadMi, kNoteHeadFa, kNoteHeadFaUp,
  kNoteHeadSo, kNoteHeadLa, kNoteHeadTi
};

std::string msrNoteHeadKindAsString (
  msrNoteHeadKind noteHeadKind);

std::ostream& operator << (std::ostream& os, const msrNoteHeadKind& elt);

enum class msrNoteHeadFilledKind {
  kNoteHeadFilledYes, kNoteHeadFilledNo
};

std::string msrNoteHeadFilledKindAsString (
  msrNoteHeadFilledKind msrNoteHeadFilledKind);

std::ostream& operator << (std::ostream& os, const msrNoteHeadFilledKind& elt);

enum class msrNoteHeadParenthesesKind {
  kNoteHeadParenthesesYes, kNoteHeadParenthesesNo
};

std::string msrNoteHeadParenthesesKindAsString (
  msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind);

std::ostream& operator << (std::ostream& os, const msrNoteHeadParenthesesKind& elt);

enum class msrNoteIsACueNoteKind {
  kNoteIsACueNoteYes, kNoteIsACueNoteNo
};

std::string msrNoteIsACueNoteKindAsString (
  msrNoteIsACueNoteKind msrNoteIsACueNoteKind);

std::ostream& operator << (std::ostream& os, const msrNoteIsACueNoteKind& elt);


}


#endif // ___msrNotesEnumTypes___
