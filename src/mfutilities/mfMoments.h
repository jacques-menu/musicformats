/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfMoments___
#define ___mfMoments___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"

#include "mfDurations.h"


namespace MusicFormats
{

// moments
//______________________________________________________________________________
class EXP mfMoment
{
  public:

    // constants
    // ------------------------------------------------------

    static const mfMoment K_MOMENT_UNKNOWN_;

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfMoment ();

                          mfMoment (
                            const mfWholeNotes& writtenPositionInMeasure,
                            const mfWholeNotes& soundingRelativeOffset);

                          mfMoment (
                            const mfWholeNotes& writtenPositionInMeasure);

    virtual               ~mfMoment ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setWrittenPositionInMeseasure (mfWholeNotes position)
                              { fWrittenPositionInMeseasure = position; }

    mfWholeNotes         getWrittenPositionInMeseasure () const
                              { return fWrittenPositionInMeseasure; }

    void                  setSoundingRelativeOffset (mfWholeNotes offset)
                              { fSoundingRelativeOffset = offset; }

    mfWholeNotes         getSoundingRelativeOffset () const
                              { return fSoundingRelativeOffset; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  operator== (const mfMoment& other) const;

    Bool                  operator!= (const mfMoment& other) const
                              { return ! ((*this) == other); }

    Bool                  operator< (const mfMoment& other) const;

    Bool                  operator>= (const mfMoment& other) const
                              { return ! ((*this) < other); }

    Bool                  operator> (const mfMoment& other) const;

    Bool                  operator<= (const mfMoment& other) const
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

    mfWholeNotes         fWrittenPositionInMeseasure;
    mfWholeNotes         fSoundingRelativeOffset;
};
EXP std::ostream& operator << (std::ostream& os, const mfMoment& elt);

//______________________________________________________________________________
void testMsrMomentComparisons (std::ostream& os);


}

#endif // ___mfMoments___
