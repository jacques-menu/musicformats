/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrDurations___
#define ___msrDurations___

#include <string>
#include <ostream>


namespace MusicFormats
{

// durations
//______________________________________________________________________________
enum class msrDurationKind {
  kDuration_NO_,

  // from longest to shortest for the algorithms
  kDurationMaxima, kDurationLonga, kDurationBreve,
  kDurationWhole, kDurationHalf,
  kDurationQuarter,
  kDurationEighth, kDuration16th, kDuration32nd, kDuration64th,
  kDuration128th, kDuration256th, kDuration512th, kDuration1024th
};

string msrDurationKindAsString (msrDurationKind durationKind);

ostream& operator << (ostream& os, const msrDurationKind& elt);

EXP msrDurationKind msrDurationKindFromMusicXMLString (
  int           inputLineNumber,
  const string& durationString);

EXP msrDurationKind msrDurationKindFromInteger (
  int inputLineNumber,
  int durationInteger);

EXP msrDurationKind msrDurationKindFromString (
  int           inputLineNumber,
  const string& durationString);

EXP Rational rationalFromDurationKindAndDotsNumber (
  msrDurationKind durationKind,
  int             dotsNumber);

EXP Rational msrDurationKindAsWholeNotes (
  msrDurationKind durationKind);

EXP msrDurationKind wholeNotesAsDurationKind (Rational wholeNotes);

string msrDurationKindAsMusicXMLType (msrDurationKind durationKind);

// durations
//______________________________________________________________________________
class EXP msrDuration
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDuration ();

                          msrDuration (
                            msrDurationKind durationKind,
                            int             dotsNumber);

    virtual               ~msrDuration ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setDurationKind (msrDurationKind durationKind)
                              { fDurationKind = durationKind; }

    msrDurationKind       getDurationKind () const
                              { return fDurationKind; }

    void                  setDotsNumber (int dotsNumber)
                              { fDotsNumber = dotsNumber; }

    int                   getDotsNumber () const
                              { return fDotsNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  incrDotsNumber ()
                              { ++fDotsNumber; }

    Rational              dottedDurationAsWholeNotes_FOR_TEMPO (
                            int inputLineNumber) const;

    Rational              dottedDurationAsWholeNotes (
                            int inputLineNumber) const;

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

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrDurationKind       fDurationKind;
    int                   fDotsNumber;
};
EXP ostream& operator << (ostream& os, const msrDuration& elt);

// dotted durations
//______________________________________________________________________________
class EXP msrDottedDuration
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDottedDuration ();

                          msrDottedDuration (
                            msrDurationKind durationKind,
                            int             dotsNumber);

    virtual               ~msrDottedDuration ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setDurationKind (msrDurationKind durationKind)
                              { fDurationKind = durationKind; }

    msrDurationKind       getDurationKind () const
                              { return fDurationKind; }

    void                  setDotsNumber (int dotsNumber)
                              { fDotsNumber = dotsNumber; }

    int                   getDotsNumber () const
                              { return fDotsNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  incrDotsNumber ()
                              { ++fDotsNumber; }

    Rational              dottedDurationAsWholeNotes_FOR_TEMPO (
                            int inputLineNumber) const;

    Rational              dottedDurationAsWholeNotes (
                            int inputLineNumber) const;

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

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrDurationKind       fDurationKind;
    int                   fDotsNumber;
};
EXP ostream& operator << (ostream& os, const msrDottedDuration& elt);

// whole notes
//______________________________________________________________________________
string wholeNotesAsMsrString (
  int             inputLineNumber,
  const Rational& wholeNotes,
  int&            dotsNumber);

string wholeNotesAsMsrString (
  int             inputLineNumber,
  const Rational& wholeNotes);

string multipleFullBarRestsWholeNotesAsMsrString (
  int             inputLineNumber, // JMI
  const Rational& wholeNotes);


}

#endif
