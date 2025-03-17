/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfPositionInMeasure___
#define ___mfPositionInMeasure___

#include <string>
#include <ostream>

#include "mfBool.h"
#include "mfDurations.h"
#include "mfIndentedTextOutput.h"
#include "mfRational.h"


namespace MusicFormats
{

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


}


#endif // ___mfPositionInMeasure___
