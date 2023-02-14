/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrEnumTypes___
#define ___lpsrEnumTypes___

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

std::string lpsrScoreOutputKindAsString (
  lpsrScoreOutputKind scoreOutputKind);

std::ostream& operator << (std::ostream& os, const lpsrScoreOutputKind& elt);

extern std::map<std::string, lpsrScoreOutputKind>
  gGlobalLpsrScoreOutputKindsMap;

std::string availableLpsrScoreOutputKinds (size_t namesListMaxLength);

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

std::string lpsrAccidentalStyleKindAsString (
  lpsrAccidentalStyleKind accidentalStyleKind);

std::ostream& operator << (std::ostream& os, const lpsrAccidentalStyleKind& elt);

std::string lpsrAccidentalStyleKindAsLilypondString (
  lpsrAccidentalStyleKind accidentalStyleKind);

extern std::map<std::string, lpsrAccidentalStyleKind>
  gGlobalLpsrAccidentalStyleKindsMap;

std::string availableLpsrAccidentalStyleKinds (size_t namesListMaxLength);

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

std::string lpsrChordsLanguageKindAsString (
  lpsrChordsLanguageKind languageKind);

std::ostream& operator << (std::ostream& os, const lpsrChordsLanguageKind& elt);

extern std::map<std::string, lpsrChordsLanguageKind>
  gGlobalLpsrChordsLanguageKindsMap;

std::string availableLpsrChordsLanguageKinds (size_t namesListMaxLength);

void initializeLpsrChordsLanguageKindsMap ();

// whole notes
//______________________________________________________________________________
std::string wholeNotesAsLilypondString (
  int             inputLineNumber,
  const Rational& wholeNotes,
  int&            dotsNumber);

std::string wholeNotesAsLilypondString (
  int             inputLineNumber,
  const Rational& wholeNotes);

// dotted durations
//______________________________________________________________________________

std::string dottedDurationAsLilypondString (
  int               inputLineNumber,
  msrDottedDuration dottedDuration);

std::string dottedDurationAsLilypondStringWithoutBackSlash (
  int               inputLineNumber,
  msrDottedDuration dottedDuration);

// rests measures
//______________________________________________________________________________
std::string multipleFullBarRestsWholeNoteAsLilypondString (
  int             inputLineNumber,
  const Rational& wholeNotes);

// texts lists
//______________________________________________________________________________
EXP void writeTextsListAsLilypondString (
  const std::list<std::string>& textsList,
  std::ostream&            os);

// pitches and octaves
//______________________________________________________________________________
std::string msrSemiTonesPitchKindAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind);

std::string msrSemiTonesPitchAndOctaveAsLilypondString (
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

std::string lpsrDynamicsTextSpannersStyleKindAsString (
  lpsrDynamicsTextSpannersStyleKind dynamicsTextSpannersStyleKind);

std::ostream& operator << (std::ostream& os, const lpsrDynamicsTextSpannersStyleKind& elt);

lpsrDynamicsTextSpannersStyleKind lpsrDynamicsTextSpannersStyleKindFromString (
  int           inputLineNumber,
  const std::string& dynamicsTextSpannersStyleKindString);

extern std::map<std::string, lpsrDynamicsTextSpannersStyleKind>&
  getLpsrDynamicsTextSpannersStyleKindsMap ();

std::string availableLpsrDynamicsTextSpannersStyleKinds (
  size_t namesListMaxLength);

void initializeLpsrDynamicsTextSpannersStyleKindsMap ();

// lyrics durations
//______________________________________________________________________________
enum class lpsrLyricsDurationsKind {
  kLyricsDurationsImplicit, // default value
  kLyricsDurationsExplicit
};

std::string lpsrLyricsDurationsKindAsString (
  lpsrLyricsDurationsKind lyricsDurationsKind);

std::ostream& operator << (std::ostream& os, const lpsrLyricsDurationsKind& elt);

extern std::map<std::string, lpsrLyricsDurationsKind>
  gGlobalLpsrLyricsDurationsKindsMap;

std::string availableLpsrLyricsDurationsKinds (size_t namesListMaxLength);

void initializeLpsrLyricsDurationsKindsMap ();


}


#endif // ___lpsrEnumTypes___
