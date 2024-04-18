/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMoments___
#define ___msrMoments___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"

#include "msrNotesDurations.h"


namespace MusicFormats
{

// moments
//______________________________________________________________________________
class EXP msrMoment
{
  public:

    // constants
    // ------------------------------------------------------

    static const msrMoment K_MOMENT_UNKNOWN_;

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMoment ();

                          msrMoment (
                            const msrWholeNotes& writtenMeasurePosition,
                            const msrWholeNotes& soundingRelativeOffset);

                          msrMoment (
                            const msrWholeNotes& writtenMeasurePosition);

    virtual               ~msrMoment ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setWrittenPositionInMeseasure (msrWholeNotes position)
                              { fWrittenPositionInMeseasure = position; }

    msrWholeNotes         getWrittenPositionInMeseasure () const
                              { return fWrittenPositionInMeseasure; }

    void                  setSoundingRelativeOffset (msrWholeNotes offset)
                              { fSoundingRelativeOffset = offset; }

    msrWholeNotes         getSoundingRelativeOffset () const
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

    msrWholeNotes         fWrittenPositionInMeseasure;
    msrWholeNotes         fSoundingRelativeOffset;
};
EXP std::ostream& operator << (std::ostream& os, const msrMoment& elt);

//______________________________________________________________________________
void testMsrMomentComparisons (std::ostream& os);


}

#endif // ___msrMoments___
