/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfRational___
#define ___mfRational___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfRational
/*
  a mfRational is a rational number whose denominator is strictly positive
   and which is in rationalised form at all times,
   directly derived from libmusicxml2's mfRational class
*/
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfRational (
                            long int num   = 0,
                            long int denom = 1);

                          mfRational (const mfRational& rat);

                          mfRational (const std::string& theString);

    virtual               ~mfRational ();


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

    mfRational              inverse () const;
    mfRational              opposite () const;

    mfRational              operator + (const mfRational &rat) const;
    mfRational              operator - (const mfRational &rat) const;
    //! Useful for notes with dots.
    mfRational              operator * (const mfRational &rat) const;
    mfRational              operator / (const mfRational &rat) const;
    // (i.e. rat * 3/2 or rat * 7/4)

    mfRational              operator * (int num) const;
    mfRational              operator / (int num) const;

    mfRational&             operator += (const mfRational &rat);
    mfRational&             operator -= (const mfRational &rat);
    //! Useful for notes with dots.
    mfRational&             operator *= (const mfRational &rat);
    mfRational&             operator /= (const mfRational &rat);
    // (i.e. rat * 3/2 or rat * 7/4)

    mfRational&             operator *= (long int num)
                              { fNumerator *= num; return *this; }
    mfRational&             operator /= (long int num)
                              { fDenominator *= num; return *this; }

    // assignment

    mfRational&             operator = (const mfRational& rat);

    // comparisons

    Bool                  operator >  (const mfRational &rat) const;
    Bool                  operator >= (const mfRational &rat) const
                              {return !(*this < rat);}
    Bool                  operator <  (const mfRational &rat) const;
    Bool                  operator <= (const mfRational &rat) const
                              {return !(*this > rat);}

    Bool                  operator == (const mfRational &rat) const;
    Bool                  operator != (const mfRational &rat) const
                              {return !(*this == rat);}

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

    // 'rationalise' mfRational values
    void                  rationalise ();

  private:

    // private fields
    // ------------------------------------------------------

    long int              fNumerator;
    long int              fDenominator;
};

EXP std::ostream& operator << (std::ostream& os, const mfRational& rat);


EXP void testRational ();


}


#endif // ___mfRational___
