/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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

#include "msrLyrics.h"
#include "msrNotesDurations.h"
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

extern std::map <std::string, lpsrScoreOutputKind>
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

extern std::map <std::string, lpsrAccidentalStyleKind>
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

extern std::map <std::string, lpsrChordsLanguageKind>
  gGlobalLpsrChordsLanguageKindsMap;

std::string availableLpsrChordsLanguageKinds (size_t namesListMaxLength);

void initializeLpsrChordsLanguageKindsMap ();

// whole notes as LilyPond 'make-duration'
//______________________________________________________________________________
// std::string wholeNotesAsLilypondMakeDurationArguments (
//   int                  inputLineNumber,
//   const msrWholeNotes& wholeNotes);

// whole notes as LilyPond strings
//______________________________________________________________________________
std::string wholeNotesAsLilypondString (
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotes,
  int&                 dotsNumber);

std::string wholeNotesAsLilypondString (
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotes);

// dotted durations
//______________________________________________________________________________

std::string dottedNotesDurationAsLilypondString (
  int                    inputLineNumber,
  msrDottedNotesDuration dottedNotesDuration);

std::string dottedNotesDurationAsLilypondStringWithoutBackSlash (
  int                    inputLineNumber,
  msrDottedNotesDuration dottedNotesDuration);

// rests measures
//______________________________________________________________________________
std::string multipleMeasureRestsWholeNoteAsLilypondString (
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotes);

// syllable elements lists
//______________________________________________________________________________
EXP std::string syllableElementsListAsLilypondString (
  const std::list <msrSyllableElement>& syllableElementsList);

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

extern std::map <std::string, lpsrDynamicsTextSpannersStyleKind>&
  getLpsrDynamicsTextSpannersStyleKindsMap ();

std::string availableLpsrDynamicsTextSpannersStyleKinds (
  size_t namesListMaxLength);

void initializeLpsrDynamicsTextSpannersStyleKindsMap ();

// notes durations generation
//______________________________________________________________________________
enum class lilypondDurationsGenerationKind {
  kLilypondDurationsGenerationImplicit, // default value
  kLilypondDurationsGenerationExplicit
};

std::string lilypondDurationsGenerationKindAsString (
  lilypondDurationsGenerationKind lilypondDurationsGenerationKind);

std::ostream& operator << (std::ostream& os, const lilypondDurationsGenerationKind& elt);

extern std::map <std::string, lilypondDurationsGenerationKind>
  gGlobalLilypondDurationsGenerationKindsMap;

std::string availableLilypondDurationsGenerationKinds (size_t namesListMaxLength);

void initializeLilypondDurationsGenerationKindsMap ();

// lyrics durations
//______________________________________________________________________________
enum class lpsrLyricsNotesDurationsKind {
  kLyricsNotesDurationsAutomatic, // default value
  kLyricsNotesDurationsImplicit,
  kLyricsNotesDurationsExplicit
};

std::string lpsrLyricsNotesDurationsKindAsString (
  lpsrLyricsNotesDurationsKind lyricsNotesDurationsKind);

std::ostream& operator << (std::ostream& os, const lpsrLyricsNotesDurationsKind& elt);

extern std::map <std::string, lpsrLyricsNotesDurationsKind>
  gGlobalLpsrLyricsNotesDurationsKindsMap;

std::string availableLpsrLyricsNotesDurationsKinds (size_t namesListMaxLength);

void initializeLpsrLyricsNotesDurationsKindsMap ();


}


#endif // ___lpsrEnumTypes___
