/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrDivisions___
#define ___msrDivisions___

#include <list>

#include "msrElements.h"

#include "mfRational.h"

#include "msrDurations.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrDivisions : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDivisions> create (
                            int inputLineNumber,
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
                            int inputLineNumber,
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
    int                   durationKindAsDivisions (
                            int             inputLineNumber,
                            msrDurationKind durationKind);

    void                  printDurationKindsDivisions (std::ostream& os);

    // MSR strings
    std::string           divisionsAsMsrString (
                            int  inputLineNumber,
                            int  divisions,
                            int& numberOfDotsNeeded);

    std::string           divisionsAsMsrString (
                            int  inputLineNumber,
                            int  divisions);
/* JMI
    std::string           wholeNotesAsMsrString (
                            int      inputLineNumber,
                            const Rational& wholeNotes,
                            int&     numberOfDotsNeeded);

    std::string           wholeNotesAsMsrString (
                            int      inputLineNumber,
                            const Rational& wholeNotes);
*/
    std::string           tupletDivisionsAsMsrString (
                            int inputLineNumber,
                            int divisions,
                            int actualNotes,
                            int normalNotes);

    std::string           tupletWholeNotesAsMsrString (
                            int             inputLineNumber,
                            const Rational& wholeNotes,
                            int             actualNotes,
                            int             normalNotes);

/* JMI
    void                  testDivisionsAndDurations (); // JMI
    void                  testTupletSoundingWholeNotesAndDurations ();
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

    std::list<std::pair<msrDurationKind, int> >
                          fDurationKindsToDivisions;
};
typedef SMARTP<msrDivisions> S_msrDivisions;
EXP std::ostream& operator << (std::ostream& os, const S_msrDivisions& elt);


}


#endif // ___msrDivisions___
