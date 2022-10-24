/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrDivisions___
#define ___msrDivisions___

#include <list>

#include "msrElements.h"

#include "msrBasicTypes.h"


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

    void                  printDurationKindsDivisions (ostream& os);

    // MSR strings
    string                divisionsAsMsrString (
                            int  inputLineNumber,
                            int  divisions,
                            int& numberOfDotsNeeded);

    string                divisionsAsMsrString (
                            int  inputLineNumber,
                            int  divisions);
/* JMI
    string                wholeNotesAsMsrString (
                            int      inputLineNumber,
                            const Rational& wholeNotes,
                            int&     numberOfDotsNeeded);

    string                wholeNotesAsMsrString (
                            int      inputLineNumber,
                            const Rational& wholeNotes);
*/
    string                tupletDivisionsAsMsrString (
                            int inputLineNumber,
                            int divisions,
                            int actualNotes,
                            int normalNotes);

    string                tupletWholeNotesAsMsrString (
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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fDivisionsPerQuarterNote;

    list<pair<msrDurationKind, int> >
                          fDurationKindsToDivisions;
};
typedef SMARTP<msrDivisions> S_msrDivisions;
EXP ostream& operator << (ostream& os, const S_msrDivisions& elt);


} // namespace MusicFormats


#endif
