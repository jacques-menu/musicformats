/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMoments___
#define ___msrMoments___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"
#include "mfRational.h"


namespace MusicFormats
{

// moments
//______________________________________________________________________________
class EXP msrMoment
{
  public:

    // constants
    // ------------------------------------------------------

    static const Rational  K_NO_POSITION;

    static const msrMoment K_NO_MOMENT;

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMoment ();

                          msrMoment (
                            const Rational& writtenMeasurePosition,
                            const Rational& soundingRelativeOffset);

                          msrMoment (
                            const Rational& writtenMeasurePosition);

    virtual               ~msrMoment ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setWrittenPositionInMeseasure (Rational position)
                              { fWrittenPositionInMeseasure = position; }

    Rational              getWrittenPositionInMeseasure () const
                              { return fWrittenPositionInMeseasure; }

    void                  setSoundingRelativeOffset (Rational offset)
                              { fSoundingRelativeOffset = offset; }

    Rational              getSoundingRelativeOffset () const
                              { return fSoundingRelativeOffset; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  operator== (const msrMoment& other) const;

    Bool                  operator!= (const msrMoment& other) const
                              { return ! ((*this) == other); }

    Bool                  operator< (const msrMoment& other) const;

    Bool                  operator>= (const msrMoment& other) const
                              { return ! ((*this) < other); }

    Bool                  operator> (const msrMoment& other) const;

    Bool                  operator<= (const msrMoment& other) const
                              { return ! ((*this) > other); }

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

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    Rational              fWrittenPositionInMeseasure;
    Rational              fSoundingRelativeOffset;
};
EXP std::ostream& operator << (std::ostream& os, const msrMoment& elt);

//______________________________________________________________________________
void testMsrMomentComparisons (std::ostream& os);


}

#endif
