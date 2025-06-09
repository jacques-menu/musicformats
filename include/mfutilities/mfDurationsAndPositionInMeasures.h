/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfDurations___
#define ___mfDurations___

#include <string>
#include <ostream>

#include "mfBasicTypes.h"

// #include "mfBool.h"
#include "mfIndentedTextOutput.h"
// #include "mfMoments.h"
// #include "mfPositionInMeasure.h"
#include "mfRational.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class mfDurationKind {
  kDuration_UNKNOWN_,

  // from longest to shortest for the algorithms
  kDurationMaxima, kDurationLonga, kDurationBreve,
  kDurationWhole, kDurationHalf,
  kDurationQuarter,
  kDurationEighth, kDuration16th, kDuration32nd, kDuration64th,
  kDuration128th, kDuration256th, kDuration512th, kDuration1024th
};

std::string mfDurationKindAsString (mfDurationKind notesDurationKind);

std::ostream& operator << (std::ostream& os, const mfDurationKind& elt);

EXP mfDurationKind mfDurationKindFromMusicXMLGraphicNoteType (
  const mfInputLineNumber& inputLineNumber,
  const std::string& durationString);

EXP mfDurationKind mfDurationKindFromInteger (
  const mfInputLineNumber& inputLineNumber,
  int durationInteger);

EXP mfDurationKind mfDurationKindFromString (
  const mfInputLineNumber& inputLineNumber,
  const std::string& durationString);

std::string mfDurationKindAsMusicXMLType (mfDurationKind notesDurationKind);


// //______________________________________________________________________________
// EXP void checkNoteDurationKindAndWholeNotesDurationConsistency (
//   const mfInputLineNumber& inputLineNumber,
//   mfDurationKind notesDurationKind,
//   mfPositionInMeasure        wholeNotesDuration);
//
// //______________________________________________________________________________
// std::string wholeNotesAndDotsNumberPitchAndOctaveAsString ( // JMI 0.9.67
//   const mfInputLineNumber& inputLineNumber,
//   const mfPositionInMeasure& wholeNotes,
//   int&                 dotsNumber);
//
// std::string wholeNotesPitchAndOctaveAsString (
//   const mfInputLineNumber& inputLineNumber,
//   const mfPositionInMeasure& wholeNotes);
//
// std::string multipleMeasureRestsWholeNotesPitchAndOctaveAsString (
//   const mfInputLineNumber& inputLineNumber, // JMI 0.9.67
//   const mfPositionInMeasure& wholeNotes);

//______________________________________________________________________________
class EXP mfWholeNotes
/*
  an mfWholeNotes is a fraction of a whole notes duration
  whose denominator is strictly positive
  and which is in rationalised form at all times,
  directly derived from libmusicxml2's rational class
*/
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

/*
  supplying an int when an mfWholeNotes value is expected
  can lead to nasty, hard to locate bugs (this occurrend once...),
  so there is no constructor with 1 'int' argument
*/
                          mfWholeNotes ();

                          mfWholeNotes (
                            int numerator,
                            int denominator);

                          mfWholeNotes (const mfWholeNotes& wholeNotes);

                          mfWholeNotes (const std::string& theString);

    virtual               ~mfWholeNotes ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNumerator (int num)
                              { fNumerator = num; }
    int                   getNumerator ()  const
                              { return fNumerator; }

    void                  setDenominator (int denom)
                              { fDenominator = denom; }
    int                   getDenominator () const
                              { return fDenominator; }

    void                  set (int num, int denom)
                              { fNumerator = num; fDenominator = denom; }

  public:

    // public services
    // ------------------------------------------------------

    // 'rationalise' mfWholeNotes values

    void                  rationalise ();

    // arithmetic

    mfWholeNotes          inverse () const;
    mfWholeNotes          opposite () const;

    mfWholeNotes          operator + (const mfWholeNotes &wholeNotes) const;
    mfWholeNotes          operator - (const mfWholeNotes &wholeNotes) const;

    //! Useful for notes with dots.
    mfWholeNotes          operator * (const mfRational &rat) const;
    mfWholeNotes          operator / (const mfRational &rat) const;
    // (i.e. wholeNotes * 3/2 or wholeNotes * 7/4)

    mfWholeNotes          operator * (int num) const;
    mfWholeNotes          operator / (int num) const;

    mfRational            operator / (const mfWholeNotes &wholeNotes) const;

    mfWholeNotes&         operator += (const mfWholeNotes &wholeNotes);
    mfWholeNotes&         operator -= (const mfWholeNotes &wholeNotes);

    //! Useful for notes with dots.
    mfWholeNotes&         operator *= (const mfRational &rat);
    mfWholeNotes&         operator /= (const mfRational &rat);
    // (i.e. wholeNotes * 3/2 or wholeNotes * 7/4)

    mfWholeNotes&         operator *= (int num)
                              { fNumerator *= num; return *this; }
    mfWholeNotes&         operator /= (int num)
                              { fDenominator *= num; return *this; }

    // assignment

    mfWholeNotes&         operator = (const mfWholeNotes& wholeNotes);

    // comparisons
    Bool                  operator >  (const mfWholeNotes &wholeNotes) const;
    Bool                  operator >= (const mfWholeNotes &wholeNotes) const
                              {return !(*this < wholeNotes);}
    Bool                  operator <  (const mfWholeNotes &wholeNotes) const;
    Bool                  operator <= (const mfWholeNotes &wholeNotes) const
                              {return !(*this > wholeNotes);}

    Bool                  operator == (const mfWholeNotes &wholeNotes) const;
    Bool                  operator != (const mfWholeNotes &wholeNotes) const
                              {return !(*this == wholeNotes);}

    Bool                  operator >  (double num) const;
    Bool                  operator >= (double num) const;
    Bool                  operator <  (double num) const;
    Bool                  operator <= (double num) const;
    Bool                  operator == (double) const;

    // conversions

                          operator std::string () const;
                          operator double () const;
                          operator float () const;
                          operator int () const;

    std::string           toString () const;
    double                toDouble () const;
    float                 toFloat () const;
    int                   toInt () const;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const;
    std::string           asString () const
                              { return asShortString (); }

    std::string           asFractionString () const;

    void                  print (std::ostream& os) const;

  private:

    // private methods
    // ------------------------------------------------------

  private:

    // private fields
    // ------------------------------------------------------

    int                   fNumerator;
    int                   fDenominator;
};

EXP std::ostream& operator << (std::ostream& os, const mfWholeNotes& wholeNotes);
EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const mfWholeNotes& wholeNotes);

//______________________________________________________________________________
// whole notes

EXP extern const int K_WHOLE_NOTES_NUMERATOR_UNKNOWN_;

EXP extern const mfWholeNotes K_WHOLE_NOTES_UNKNOWN_;

EXP extern const mfWholeNotes K_WHOLE_NOTES_ZERO;

EXP extern const std::string
  kWholeNotePrefix,
  kWholeNoteDefaultValueString;

EXP void testWholeNotes ();

int  mfNumberOfDots (int n);

int  mfDurationBinaryLogarithm (int duration);

EXP mfWholeNotes wholeNotesFromNotesDurationKindAndDotsNumber (
  mfDurationKind notesDurationKind,
  int                  dotsNumber);

EXP mfWholeNotes mfDurationKindAsWholeNotes (
  mfDurationKind notesDurationKind);

EXP mfDurationKind wholeNotesAsNotesDurationKind (mfWholeNotes wholeNotes);

//______________________________________________________________________________
EXP void checkNoteDurationKindAndWholeNotesDurationConsistency (
  const mfInputLineNumber& inputLineNumber,
  mfDurationKind notesDurationKind,
  mfWholeNotes        wholeNotesDuration);

//______________________________________________________________________________
std::string wholeNotesAndDotsNumberPitchAndOctaveAsString ( // JMI 0.9.67
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& wholeNotes,
  int&                 dotsNumber);

std::string wholeNotesPitchAndOctaveAsString (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& wholeNotes);

std::string multipleMeasureRestsWholeNotesPitchAndOctaveAsString (
  const mfInputLineNumber& inputLineNumber, // JMI 0.9.67
  const mfWholeNotes& wholeNotes);

//______________________________________________________________________________
class EXP msrNotesDuration
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrNotesDuration ();

                          msrNotesDuration (
                            mfDurationKind notesDurationKind,
                            int                  dotsNumber);

    virtual               ~msrNotesDuration ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNotesDurationKind (
                            mfDurationKind notesDurationKind)
                              { fNotesDurationKind = notesDurationKind; }

    mfDurationKind   getNotesDurationKind () const
                              { return fNotesDurationKind; }

    void                  setDotsNumber (int dotsNumber)
                              { fDotsNumber = dotsNumber; }

    int                   getDotsNumber () const
                              { return fDotsNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  incrDotsNumber ()
                              { ++fDotsNumber; }

    mfWholeNotes          dottedNotesDurationAsWholeNotes_FOR_TEMPO (
                            const mfInputLineNumber& inputLineNumber) const;

    mfWholeNotes          dottedNotesDurationAsWholeNotes (
                            const mfInputLineNumber& inputLineNumber) const;

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    mfDurationKind        fNotesDurationKind;
    int                   fDotsNumber;
};
EXP std::ostream& operator << (std::ostream& os, const msrNotesDuration& elt);

//______________________________________________________________________________
class EXP mfDottedNotesDuration
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfDottedNotesDuration ();

                          mfDottedNotesDuration (
                            mfDurationKind notesDurationKind,
                            int                  dotsNumber);

    virtual               ~mfDottedNotesDuration ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNotesDurationKind (
                            mfDurationKind notesDurationKind)
                              { fNotesDurationKind = notesDurationKind; }

    mfDurationKind  getNotesDurationKind () const
                              { return fNotesDurationKind; }

    void                  setDotsNumber (int dotsNumber)
                              { fDotsNumber = dotsNumber; }

    int                   getDotsNumber () const
                              { return fDotsNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  incrDotsNumber ()
                              { ++fDotsNumber; }

    mfWholeNotes          dottedNotesDurationAsWholeNotes_FOR_TEMPO (
                            const mfInputLineNumber& inputLineNumber) const;

    mfWholeNotes          dottedNotesDurationAsWholeNotes (
                            const mfInputLineNumber& inputLineNumber) const;

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    mfDurationKind        fNotesDurationKind;
    int                   fDotsNumber;
};

EXP std::ostream& operator << (std::ostream& os, const mfDottedNotesDuration& elt);


//______________________________________________________________________________
// the empty string

EXP extern const std::string K_MF_EMPTY_STRING;

//______________________________________________________________________________
// input line numbers

EXP extern const int K_MF_INPUT_LINE_UNKNOWN_;

EXP extern const std::string
  kInputLineNumberPrefix,
  kInputLineNumberDefaultValueString;

//______________________________________________________________________________
// input locations

EXP extern const std::string
  kInputLocationRangePrefix,
  kInputLocationDefaultValueString;

//______________________________________________________________________________
// staff numbers

EXP extern const int K_STAFF_NUMBER_UNKNOWN_;

EXP extern const std::string
  kStaffNumberPrefix,
  kStaffNumberDefaultValueString;

EXP extern const int K_PART_HARMONIES_STAFF_NUMBER;

EXP extern const int K_PART_FIGURED_BASS_STAFF_NUMBER;

EXP extern std::string mfStaffNumberAsString (const mfStaffNumber& staffNumber);

//______________________________________________________________________________
// voice numbers

EXP extern const int K_VOICE_NUMBER_UNKNOWN_;

EXP extern const std::string
  kVoiceNumberPrefix,
  kVoiceNumberDefaultValueString;

EXP extern const int K_PART_HARMONIES_VOICE_NUMBER;
EXP extern const int K_VOICE_HARMONIES_VOICE_BASE_NUMBER;

EXP extern const int K_PART_FIGURED_BASS_VOICE_NUMBER;
EXP extern const int K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER;

EXP extern std::string mfVoiceNumberAsString (const mfVoiceNumber& voiceNumber);

//______________________________________________________________________________
// measure numbers

EXP extern const std::string K_MEASURE_NUMBER_UNKNOWN_;

EXP extern const std::string
  kMeasureNumberPrefix,
  kMeasureNumberDefaultValueString;

EXP extern std::string mfMeasureNumberAsString (
  const mfMeasureNumber& measureNumber);

//______________________________________________________________________________
// whole notes durations

EXP extern const int K_WHOLE_NOTES_NUMERATOR_UNKNOWN_;

EXP extern const mfWholeNotes K_WHOLE_NOTES_UNKNOWN_;

EXP extern const mfWholeNotes K_WHOLE_NOTES_ZERO;

// #ifndef MF_USE_WRAPPED_TYPES
//   using mfWholeNotes = mfWholeNotes;
// #else
//   using mfWholeNotes =
//     mfWrappedValueWithDefault <
//       mfWholeNotes,
//       kWholeNotePrefix,
//       K_MF_EMPTY_STRING,
//       K_WHOLE_NOTES_UNKNOWN_,
//       kWholeNoteDefaultValueString
//     >;
// #endif // MF_USE_WRAPPED_TYPES

EXP extern const std::string
  kWholeNotePrefix,
  kWholeNoteDefaultValueString;

//______________________________________________________________________________
class EXP mfPositionInMeasure
/*
  an mfPositionInMeasure is a fraction
  whose denominator is strictly positive
  and which is in rationalised form at all times,
  directly derived from libmusicxml2's rational class

  It is sometimes necessary to create a position in measure
  from a whole notes value, or converserly,
  when handling measures contents
*/
{
  public:

    static mfPositionInMeasure
                          createFromWholeNotes (
                            const mfWholeNotes& wholeNotes);

  public:

    // constructors/destructor
    // ------------------------------------------------------

/*
  supplying an int when an mfPositionInMeasure value is expected
  can lead to nasty, hard to locate bugs (this occurrend once...),
  so there is no constructor with 1 'int' argument
*/
                          mfPositionInMeasure ();

                          mfPositionInMeasure (
                            int numerator,
                            int denominator);

                          mfPositionInMeasure (
                            const mfPositionInMeasure& positionInMeasure);

                          mfPositionInMeasure (const std::string& theString);

    virtual               ~mfPositionInMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNumerator (int num)
                              { fNumerator = num; }
    int                   getNumerator ()  const
                              { return fNumerator; }

    void                  setDenominator (int denom)
                              { fDenominator = denom; }
    int                   getDenominator () const
                              { return fDenominator; }

    void                  set (int num, int denom)
                              { fNumerator = num; fDenominator = denom; }

  public:

    // public services
    // ------------------------------------------------------

    // 'rationalise' mfPositionInMeasure values

    void                  rationalise ();

    // arithmetic

    mfPositionInMeasure   inverse () const;
    mfPositionInMeasure   opposite () const;

    mfPositionInMeasure   operator + (const mfWholeNotes &wholeNotes) const;
    mfPositionInMeasure   operator - (const mfWholeNotes &wholeNotes) const;

    mfPositionInMeasure&  operator += (const mfWholeNotes &wholeNotes);
    mfPositionInMeasure&  operator -= (const mfWholeNotes &wholeNotes);

    mfWholeNotes          operator - (
                            const mfPositionInMeasure &positionInMeasure) const;

    // assignment

    mfPositionInMeasure&  operator = (
                            const mfPositionInMeasure& positionInMeasure);

    // comparisons

    Bool                  operator >  (
                            const mfPositionInMeasure& positionInMeasure) const;
    Bool                  operator >= (
                            const mfPositionInMeasure& positionInMeasure) const
                              { return !(*this < positionInMeasure);}

    Bool                  operator <  (
                            const mfPositionInMeasure& positionInMeasure) const;
    Bool                  operator <= (
                            const mfPositionInMeasure& positionInMeasure) const
                              { return !(*this > positionInMeasure);}

    Bool                  operator == (
                            const mfPositionInMeasure& positionInMeasure) const;
    Bool                  operator != (
                            const mfPositionInMeasure& positionInMeasure) const
                              { return !(*this == positionInMeasure);}

    Bool                  operator >  (double num) const;
    Bool                  operator >= (double num) const;
    Bool                  operator <  (double num) const;
    Bool                  operator <= (double num) const;
    Bool                  operator == (double) const;

    // conversions

                          operator std::string () const;
                          operator double () const;
                          operator float () const;
                          operator int () const;

    std::string           toString () const;
    double                toDouble () const;
    float                 toFloat () const;
    int                   toInt () const;

    mfWholeNotes          asWholeNotes () const;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const;
    std::string           asString () const
                              { return asShortString (); }

    std::string           asFractionString () const;

    void                  print (std::ostream& os) const;

  private:

    // private methods
    // ------------------------------------------------------

  private:

    // private fields
    // ------------------------------------------------------

    int                   fNumerator;
    int                   fDenominator;
};

EXP std::ostream& operator << (
  std::ostream&              os,
  const mfPositionInMeasure& positionInMeasure);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream&    iss,
  const mfPositionInMeasure& positionInMeasure);

//______________________________________________________________________________
// position in measure

EXP extern const int K_POSITION_IN_MEASURE_NUMERATOR_UNKNOWN_;

EXP extern const mfPositionInMeasure K_POSITION_IN_MEASURE_UNKNOWN_;

EXP extern const mfPositionInMeasure K_POSITION_IN_MEASURE_ZERO;


}


#endif // ___mfDurations___
