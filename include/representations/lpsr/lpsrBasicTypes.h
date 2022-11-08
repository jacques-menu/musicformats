/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrBasicTypes___
#define ___lpsrBasicTypes___

#include <list>
#include <map>

#include "smartpointer.h"

#include "mfRational.h"

#include "msrDurations.h"
#include "msrOctaves.h"
#include "msrPitchesNames.h"


namespace MusicFormats
{

// score output kinds
//______________________________________________________________________________
enum class lpsrScoreOutputKind {
  kScoreOutputScoreOnly, // default value
  kScoreOutputScoreAndParts,
  kScoreOutputPartsAndScore,
  kScoreOutputPartsOnly,
  kScoreOutputScoreAndPartsOneFile,
  kScoreOutputPartsAndScoreOneFile,
  kScoreOutputPartsOnlyOneFile
};

string lpsrScoreOutputKindAsString (
  lpsrScoreOutputKind scoreOutputKind);

ostream& operator << (ostream& os, const lpsrScoreOutputKind& elt);

extern map<string, lpsrScoreOutputKind>
  gGlobalLpsrScoreOutputKindsMap;

string existingLpsrScoreOutputKinds (size_t namesListMaxLength);

void initializeLpsrScoreOutputKindsMap ();

// accidental styles
//______________________________________________________________________________
enum class lpsrAccidentalStyleKind {
// JMI there are new ones in LilyPond 2.22
  kAccidentalStyleDefault,
  kVoice,
  kAccidentalStyleModern,
  kAccidentalStyleModernCautionary,
  kAccidentalStyleModernVoice,
  kAccidentalStyleModernVoiceCautionary,
  kAccidentalStylePiano,
  kAccidentalStylePianoCautionary,
  kAccidentalStyleNeoModern,
  kAccidentalStyleNeoModernCautionary,
  kAccidentalStyleNeoModernVoice,
  kAccidentalStyleNeoModernVoiceCautionary,
  kAccidentalStyleDodecaphonic,
  kAccidentalStyleDodecaphonicNoRepeat,
  kAccidentalStyleDodecaphonicFirst,
  kAccidentalStyleTeaching,
  kAccidentalStyleNoReset,
  kAccidentalStyleForget
};

string lpsrAccidentalStyleKindAsString (
  lpsrAccidentalStyleKind accidentalStyleKind);

ostream& operator << (ostream& os, const lpsrAccidentalStyleKind& elt);

string lpsrAccidentalStyleKindAsLilypondString (
  lpsrAccidentalStyleKind accidentalStyleKind);

extern map<string, lpsrAccidentalStyleKind>
  gGlobalLpsrAccidentalStyleKindsMap;

string existingLpsrAccidentalStyleKinds (size_t namesListMaxLength);

void initializeLpsrAccidentalStyleKindsMap ();

// chords languages
//______________________________________________________________________________
enum class lpsrChordsLanguageKind {
  kChordsIgnatzek, // LilyPond default
  kChordsGerman,
  kChordsSemiGerman,
  kChordsItalian,
  kChordsFrench
};

string lpsrChordsLanguageKindAsString (
  lpsrChordsLanguageKind languageKind);

ostream& operator << (ostream& os, const lpsrChordsLanguageKind& elt);

extern map<string, lpsrChordsLanguageKind>
  gGlobalLpsrChordsLanguageKindsMap;

string existingLpsrChordsLanguageKinds (size_t namesListMaxLength);

void initializeLpsrChordsLanguageKindsMap ();

// whole notes
//______________________________________________________________________________
string wholeNotesAsLilypondString (
  int             inputLineNumber,
  const Rational& wholeNotes,
  int&            dotsNumber);

string wholeNotesAsLilypondString (
  int             inputLineNumber,
  const Rational& wholeNotes);

// dotted durations
//______________________________________________________________________________

string dottedDurationAsLilypondString (
  int               inputLineNumber,
  msrDottedDuration dottedDuration);

string dottedDurationAsLilypondStringWithoutBackSlash (
  int               inputLineNumber,
  msrDottedDuration dottedDuration);

// rests measures
//______________________________________________________________________________
string multipleFullBarRestsWholeNoteAsLilypondString (
  int             inputLineNumber,
  const Rational& wholeNotes);

// texts lists
//______________________________________________________________________________
EXP void writeTextsListAsLilypondString (
  const list<string>& textsList,
  ostream&            os);

// pitches and octaves
//______________________________________________________________________________
string msrSemiTonesPitchKindAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);

string msrSemiTonesPitchAndOctaveAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  S_msrSemiTonesPitchAndOctave       quarterTonesPitchAndOctave);

// dynamics
//______________________________________________________________________________
enum class lpsrDynamicsTextSpannersStyleKind {
  kDynamicsTextSpannersStyleNone,
  kDynamicsTextSpannersStyleDashedLine, // default value
  kDynamicsTextSpannersStyleZigzag,
  kDynamicsTextSpannersStyleTrill
};

string lpsrDynamicsTextSpannersStyleKindAsString (
  lpsrDynamicsTextSpannersStyleKind dynamicsTextSpannersStyleKind);

ostream& operator << (ostream& os, const lpsrDynamicsTextSpannersStyleKind& elt);

lpsrDynamicsTextSpannersStyleKind lpsrDynamicsTextSpannersStyleKindFromString (
  int           inputLineNumber,
  const string& dynamicsTextSpannersStyleKindString);

extern map<string, lpsrDynamicsTextSpannersStyleKind>&
  getLpsrDynamicsTextSpannersStyleKindsMap ();

string existingLpsrDynamicsTextSpannersStyleKinds (
  size_t namesListMaxLength);

void initializeLpsrDynamicsTextSpannersStyleKindsMap ();

// lyrics durations
//______________________________________________________________________________
enum class lpsrLyricsDurationsKind {
  kLyricsDurationsImplicit, // default value
  kLyricsDurationsExplicit
};

string lpsrLyricsDurationsKindAsString (
  lpsrLyricsDurationsKind lyricsDurationsKind);

ostream& operator << (ostream& os, const lpsrLyricsDurationsKind& elt);

extern map<string, lpsrLyricsDurationsKind>
  gGlobalLpsrLyricsDurationsKindsMap;

string existingLpsrLyricsDurationsKinds (size_t namesListMaxLength);

void initializeLpsrLyricsDurationsKindsMap ();

// initialization
//______________________________________________________________________________
void initializeLpsrBasicTypes ();


} // namespace MusicFormats


#endif
