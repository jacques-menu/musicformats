/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrNotesDurations___
#define ___msrNotesDurations___

#include <string>
#include <ostream>

#include "mfBool.h"
#include "mfIndentedTextOutput.h"
#include "mfRational.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrNotesDurationKind {
  kNotesDuration_UNKNOWN_,

  // from longest to shortest for the algorithms
  kNotesDurationMaxima, kNotesDurationLonga, kNotesDurationBreve,
  kNotesDurationWhole, kNotesDurationHalf,
  kNotesDurationQuarter,
  kNotesDurationEighth, kNotesDuration16th, kNotesDuration32nd, kNotesDuration64th,
  kNotesDuration128th, kNotesDuration256th, kNotesDuration512th, kNotesDuration1024th
};

std::string msrNotesDurationKindAsString (msrNotesDurationKind notesDurationKind);

std::ostream& operator << (std::ostream& os, const msrNotesDurationKind& elt);

EXP msrNotesDurationKind msrNotesDurationKindFromMusicXMLString (
  int                inputLineNumber,
  const std::string& durationString);

EXP msrNotesDurationKind msrNotesDurationKindFromInteger (
  int inputLineNumber,
  int durationInteger);

EXP msrNotesDurationKind msrNotesDurationKindFromString (
  int                inputLineNumber,
  const std::string& durationString);

std::string msrNotesDurationKindAsMusicXMLType (msrNotesDurationKind notesDurationKind);

//______________________________________________________________________________
class EXP msrWholeNotes
/*
  an msrWholeNotes is a fraction of a whole notes duration
  whose denominator is strictly positive
  and which is in rationalised form at all times,
  directly derived from libmusicxml2's rational class
*/
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

/*
  supplying an int when an msrWholeNotes value is expected
  can lead to nasty, hard to locate bugs (this occurrend once...),
  so there is no constructor with 1 'int' argument
*/
                          msrWholeNotes ();

                          msrWholeNotes (
                            int num,
                            int denom);

                          msrWholeNotes (const msrWholeNotes& wholeNotes);

                          msrWholeNotes (const std::string& theString);

    virtual               ~msrWholeNotes ();

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

    // arithmetic

    msrWholeNotes         inverse () const;
    msrWholeNotes         opposite () const;

    msrWholeNotes         operator + (const msrWholeNotes &wholeNotes) const;
    msrWholeNotes         operator - (const msrWholeNotes &wholeNotes) const;

    //! Useful for notes with dots.
    msrWholeNotes         operator * (const mfRational &rat) const;
    msrWholeNotes         operator / (const mfRational &rat) const;
    // (i.e. wholeNotes * 3/2 or wholeNotes * 7/4)

    msrWholeNotes         operator * (int num) const;
    msrWholeNotes         operator / (int num) const;

    mfRational            operator / (const msrWholeNotes &wholeNotes) const;

    msrWholeNotes&        operator += (const msrWholeNotes &wholeNotes);
    msrWholeNotes&        operator -= (const msrWholeNotes &wholeNotes);

    //! Useful for notes with dots.
    msrWholeNotes&        operator *= (const mfRational &rat);
    msrWholeNotes&        operator /= (const mfRational &rat);
    // (i.e. wholeNotes * 3/2 or wholeNotes * 7/4)

    msrWholeNotes&        operator *= (int num)
                              { fNumerator *= num; return *this; }
    msrWholeNotes&        operator /= (int num)
                              { fDenominator *= num; return *this; }

    // assignment

    msrWholeNotes&        operator = (const msrWholeNotes& wholeNotes);

    // comparisons
    Bool                  operator >  (const msrWholeNotes &wholeNotes) const;
    Bool                  operator >= (const msrWholeNotes &wholeNotes) const
                              {return !(*this < wholeNotes);}
    Bool                  operator <  (const msrWholeNotes &wholeNotes) const;
    Bool                  operator <= (const msrWholeNotes &wholeNotes) const
                              {return !(*this > wholeNotes);}

    Bool                  operator == (const msrWholeNotes &wholeNotes) const;
    Bool                  operator != (const msrWholeNotes &wholeNotes) const
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

    // 'rationalise' msrWholeNotes values
    void                  rationalise ();

  private:

    // private fields
    // ------------------------------------------------------

    int                   fNumerator;
    int                   fDenominator;
};

EXP std::ostream& operator << (std::ostream& os, const msrWholeNotes& wholeNotes);
EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const msrWholeNotes& wholeNotes);

EXP msrWholeNotes wholeNotesFromNotesDurationKindAndDotsNumber (
  msrNotesDurationKind notesDurationKind,
  int                  dotsNumber);

EXP msrWholeNotes msrNotesDurationKindAsWholeNotes (
  msrNotesDurationKind notesDurationKind);

EXP msrNotesDurationKind wholeNotesAsNotesDurationKind (msrWholeNotes wholeNotes);

//______________________________________________________________________________
EXP void checkNoteDurationKindAndWholeNotesDurationConsistency (
  int                  inputLineNumber,
  msrNotesDurationKind notesDurationKind,
  msrWholeNotes        wholeNotesDuration);

//______________________________________________________________________________
std::string wholeNotesAndDotsNumberPitchAndOctaveAsString ( // JMI v0.9.67
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotes,
  int&                 dotsNumber);

std::string wholeNotesPitchAndOctaveAsString (
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotes);

std::string multipleMeasureRestsWholeNotesPitchAndOctaveAsString (
  int                  inputLineNumber, // JMI v0.9.67
  const msrWholeNotes& wholeNotes);

//______________________________________________________________________________
EXP void testWholeNotes ();

// //______________________________________________________________________________
// class EXP msrNotesDuration
// {
//   public:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           msrNotesDuration ();
//
//                           msrNotesDuration (
//                             msrNotesDurationKind notesDurationKind,
//                             int                  dotsNumber);
//
//     virtual               ~msrNotesDuration ();
//
//   public:
//
//     // set and get
//     // ------------------------------------------------------
//
//     void                  setNotesDurationKind (
//                             msrNotesDurationKind notesDurationKind)
//                               { fNotesDurationKind = notesDurationKind; }
//
//     msrNotesDurationKind  getNotesDurationKind () const
//                               { return fNotesDurationKind; }
//
//     void                  setDotsNumber (int dotsNumber)
//                               { fDotsNumber = dotsNumber; }
//
//     int                   getDotsNumber () const
//                               { return fDotsNumber; }
//
//   public:
//
//     // public services
//     // ------------------------------------------------------
//
//     void                  incrDotsNumber ()
//                               { ++fDotsNumber; }
//
//     msrWholeNotes         dottedNotesDurationAsWholeNotes_FOR_TEMPO (
//                             int inputLineNumber) const;
//
//     msrWholeNotes         dottedNotesDurationAsWholeNotes (
//                             int inputLineNumber) const;
//
//   public:
//
//     // visitors
//     // ------------------------------------------------------
//
// /* JMI
//     void                  acceptIn  (basevisitor* v) override;
//     void                  acceptOut (basevisitor* v) override;
//
//     void                  browseData (basevisitor* v) override;
// */
//
//   public:
//
//     // ------------------------------------------------------
//
//     std::string           asString () const;
//
//     void                  print (std::ostream& os) const;
//
//   private:
//
//     // private fields
//     // ------------------------------------------------------
//
//     msrNotesDurationKind  fNotesDurationKind;
//     int                   fDotsNumber;
// };
// EXP std::ostream& operator << (std::ostream& os, const msrNotesDuration& elt);

//______________________________________________________________________________
class EXP msrDottedNotesDuration
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDottedNotesDuration ();

                          msrDottedNotesDuration (
                            msrNotesDurationKind notesDurationKind,
                            int                  dotsNumber);

    virtual               ~msrDottedNotesDuration ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNotesDurationKind (
                            msrNotesDurationKind notesDurationKind)
                              { fNotesDurationKind = notesDurationKind; }

    msrNotesDurationKind  getNotesDurationKind () const
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

    msrWholeNotes         dottedNotesDurationAsWholeNotes_FOR_TEMPO (
                            int inputLineNumber) const;

    msrWholeNotes         dottedNotesDurationAsWholeNotes (
                            int inputLineNumber) const;

    int                   msrNumberOfDots (int n);

    int                   msrNotesDurationBinaryLogarithm (int duration);

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

    msrNotesDurationKind  fNotesDurationKind;
    int                   fDotsNumber;
};

EXP std::ostream& operator << (std::ostream& os, const msrDottedNotesDuration& elt);


}

#endif // ___msrNotesDurations___
