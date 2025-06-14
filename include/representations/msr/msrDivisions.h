/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrDivisions___
#define ___msrDivisions___

#include <list>

#include "msrElements.h"
#include "mfDurationsAndPositionInMeasures.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrDivisions : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDivisions> create (
                            const mfInputLineNumber& inputLineNumber,
                            int divisionsPerQuarterNote);

    SMARTP<msrDivisions> createDivisionsNewbornClone (); // JMI

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeDivisions ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDivisions (
                            const mfInputLineNumber& inputLineNumber,
                            int divisionsPerQuarterNote);

    virtual               ~msrDivisions ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setDivisionsPerQuarterNote ( // JMI
                            int divisionsPerQuarterNote)
                              {
                                fDivisionsPerQuarterNote =
                                  divisionsPerQuarterNote;
                              }

    int                   getDivisionsPerQuarterNote () const
                              { return fDivisionsPerQuarterNote; }

  public:

    // public services
    // ------------------------------------------------------

    // durations
    int                   notesDurationKindAsDivisions (
                            const mfInputLineNumber& inputLineNumber,
                            mfDurationKind notesDurationKind);

    void                  printNotesDurationKindsDivisions (std::ostream& os);

    // MSR strings
    std::string           divisionspitchAndOctaveAsString (
                            const mfInputLineNumber& inputLineNumber,
                            int  divisions,
                            int& numberOfDotsNeeded);

    std::string           divisionspitchAndOctaveAsString (
                            const mfInputLineNumber& inputLineNumber,
                            int  divisions);

    std::string           tupletDivisionspitchAndOctaveAsString (
                            const mfInputLineNumber& inputLineNumber,
                            int divisions,
                            int actualNotes,
                            int normalNotes);

    std::string           tupletWholeNotesPitchAndOctaveAsString (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& wholeNotes,
                            int             actualNotes,
                            int             normalNotes);

/* JMI
    void                  testDivisionsAndNotesDurations (); // JMI
    void                  testTupletSoundingWholeNotesAndNotesDurations ();
*/

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fDivisionsPerQuarterNote;

    std::list <std::pair <mfDurationKind, int>>
                          fNotesDurationKindsToDivisions;
};
typedef SMARTP<msrDivisions> S_msrDivisions;
EXP std::ostream& operator << (std::ostream& os, const S_msrDivisions& elt);


}


#endif // ___msrDivisions___
