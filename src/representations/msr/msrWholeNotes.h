/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrWholeNotes___
#define ___msrWholeNotes___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"
#include "mfIndentedTextOutput.h"
#include "mfRational.h"


namespace MusicFormats
{

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

                          msrWholeNotes (
                            long int num   = 0,
                            long int denom = 1);

                          msrWholeNotes (const msrWholeNotes& wholeNotes);

                          msrWholeNotes (const std::string &theString);

    virtual               ~msrWholeNotes ();


  public:

    // set and get
    // ------------------------------------------------------

    void                  setNumerator (long int num)
                              { fNumerator = num; }
    long int              getNumerator ()  const
                              { return fNumerator; }

    void                  setDenominator (long int denom)
                              { fDenominator = denom; }
    long int              getDenominator () const
                              { return fDenominator; }

    void                  set (long int num, long int denom)
                              { fNumerator = num; fDenominator = denom; }

  public:

    // public services
    // ------------------------------------------------------

    // arithmetic

    msrWholeNotes          inverse () const;
    msrWholeNotes          opposite () const;

    msrWholeNotes          operator + (const msrWholeNotes &wholeNotes) const;
    msrWholeNotes          operator - (const msrWholeNotes &wholeNotes) const;

    //! Useful for notes with dots.
    msrWholeNotes          operator * (const mfRational &rat) const;
    msrWholeNotes          operator / (const mfRational &rat) const;
    // (i.e. wholeNotes * 3/2 or wholeNotes * 7/4)

    msrWholeNotes          operator * (int num) const;
    msrWholeNotes          operator / (int num) const;

    mfRational            operator / (const msrWholeNotes &wholeNotes) const;

    msrWholeNotes&         operator += (const msrWholeNotes &wholeNotes);
    msrWholeNotes&         operator -= (const msrWholeNotes &wholeNotes);

    //! Useful for notes with dots.
    msrWholeNotes&         operator *= (const mfRational &rat);
    msrWholeNotes&         operator /= (const mfRational &rat);
    // (i.e. wholeNotes * 3/2 or wholeNotes * 7/4)

    msrWholeNotes&         operator *= (long int num)
                              { fNumerator *= num; return *this; }
    msrWholeNotes&         operator /= (long int num)
                              { fDenominator *= num; return *this; }

    // assignment

    msrWholeNotes&         operator = (const msrWholeNotes& wholeNotes);

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

    std::string           asString () const;

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

    long int              fNumerator;
    long int              fDenominator;

    // Used by rationalise()
    long int gcd(long int a, long int b);
};
EXP std::ostream& operator << (std::ostream& os, const msrWholeNotes& wholeNotes);
EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const msrWholeNotes& wholeNotes);


//______________________________________________________________________________
EXP extern const msrWholeNotes K_WHOLE_NOTES_UNKNOWN;

//______________________________________________________________________________
std::string wholeNotesAsMsrString (
  int             inputLineNumber,
  const msrWholeNotes& wholeNotes,
  int&            dotsNumber);

std::string wholeNotesAsMsrString (
  int             inputLineNumber,
  const msrWholeNotes& wholeNotes);

std::string multipleFullBarRestsWholeNotesAsMsrString (
  int             inputLineNumber, // JMI v0.9.67
  const msrWholeNotes& wholeNotes);

//______________________________________________________________________________
EXP void testmWholeNotes ();


}


#endif // ___msrWholeNotes___
