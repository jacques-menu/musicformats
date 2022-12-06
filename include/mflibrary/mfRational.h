/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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

// Rational
//______________________________________________________________________________
class EXP Rational
/*
  a Rational is a Rational whose denominator is strictly positive
   and always in rationalised form,
   directly derived from libmusicxml2's Rational class
*/
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          Rational (
                            long int num   = 0,
                            long int denom = 1);

                          Rational (const Rational& pos);

                          Rational (const std::string &theString);

    virtual               ~Rational ();


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

    Rational              inverse () const;
    Rational              opposite () const;

    Rational              operator + (const Rational &pos) const;
    Rational              operator - (const Rational &pos) const;
    //! Useful for notes with dots.
    Rational              operator * (const Rational &pos) const;
    Rational              operator / (const Rational &pos) const;
    // (i.e. pos * 3/2 or pos * 7/4)

    Rational              operator * (int num) const;
    Rational              operator / (int num) const;

    Rational&             operator += (const Rational &pos);
    Rational&             operator -= (const Rational &pos);
    //! Useful for notes with dots.
    Rational&             operator *= (const Rational &pos);
    Rational&             operator /= (const Rational &pos);
    // (i.e. pos * 3/2 or pos * 7/4)

    Rational&             operator *= (long int num)
                              { fNumerator *= num; return *this; }
    Rational&             operator /= (long int num)
                              { fDenominator *= num; return *this; }

    // comparisons

    Rational&             operator = (const Rational& pos);

    Bool                  operator >  (const Rational &pos) const;
    Bool                  operator >= (const Rational &pos) const
                              {return !(*this < pos);}
    Bool                  operator <  (const Rational &pos) const;
    Bool                  operator <= (const Rational &pos) const
                              {return !(*this > pos);}

    Bool                  operator == (const Rational &pos) const;
    Bool                  operator != (const Rational &pos) const
                              {return !(*this == pos);}

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
    float                  toFloat () const;
    int                    toInt () const;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    std::string           asFractionString () const;

    void                  print (std::ostream& os) const;

  private:

    // private methods
    // ------------------------------------------------------

    // Used to "rationalise" Rational.
    void                  rationalise ();

  private:

    // private fields
    // ------------------------------------------------------

    long int              fNumerator;
    long int              fDenominator;

    // Used by rationalise()
    long int gcd(long int a, long int b);
};
EXP std::ostream& operator << (std::ostream& os, const Rational& elt);


EXP void testRational ();


}


#endif
