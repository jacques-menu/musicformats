/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasureElements___
#define ___msrMeasureElements___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicFormats
{
//______________________________________________________________________________
// PRE-declarations for classes mutual dependencies
class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

//______________________________________________________________________________
/*
  Various elements can found in measures,
  hence class msrMeasureElement
*/

class EXP msrMeasureElement : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    static const string K_NO_MEASURE_NUMBER;

    static const Rational K_NO_WHOLE_NOTES;

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureElement (
                            int inputLineNumber);

    virtual               ~msrMeasureElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureElementSoundingWholeNotes (
                            const Rational& wholeNotes,
                            const string&   context);

    Rational              getMeasureElementSoundingWholeNotes () const
                              { return fMeasureElementSoundingWholeNotes; }

    void                  setMeasureElementMeasureNumber (
                            const string& positionInMeasure)
                              {
                                fMeasureElementMeasureNumber = positionInMeasure;
                              }

    string                getMeasureNumber () const
                              { return fMeasureElementMeasureNumber; }

    // this method is overridden in sub-classes such as those for
    // time signatures, harmonies and figured bass elements,
    // as well as those elements that have an actual whole notes duration

    // the overrides call a class-specific method that can be called directly,
    // such occurrence are more explicit when debugging
    // the computations of positions in measures
    virtual void          setMeasureElementPositionInMeasure (
                            const S_msrMeasure measure,
                            const Rational&    positionInMeasure,
                            const string&      context);

    Rational              getMeasureElementPositionInMeasure () const
                              { return fMeasureElementPositionInMeasure; }

    void                  setMeasureElementPositionFromBeginningOfVoice (
                            const Rational& positionFromBeginningOfVoice,
                            const string&   context);

    Rational              getMeasureElementPositionFromBeginningOfVoice () const
                              { return fMeasureElementPositionFromBeginningOfVoice; }

    void                  setMeasureElementMomentInMeasure (
                            const msrMoment& momentInMeasure,
                            const string&    context);

    const msrMoment&      getMeasureElementMomentInMeasure () const
                              { return fMeasureElementMomentInMeasure; }

    void                  setMeasureElementMomentFromBeginningOfVoice (
                            const msrMoment& momentFromBeginningOfVoice,
                            const string&    context);

    const msrMoment&      getMeasureElementMomentFromBeginningOfVoice () const
                              { return fMeasureElementMomentFromBeginningOfVoice; }

  public:

    // public services
    // ------------------------------------------------------

    static bool           compareMeasureElementsByIncreasingPositionInMeasure (
                            const SMARTP<msrMeasureElement>& first,
                            const SMARTP<msrMeasureElement>& second);

//     virtual void          assignMeasureElementPositionFromBeginningOfVoice ( // JMI v0.9.66 REMOVE
//                             Rational&     positionFromBeginningOfVoice,
//                             const string& context);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override = 0; // JMI ??? v0.9.66

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const override;
    string                asString () const override;

    void                  print (ostream& os) const override;

    void                  printSummary (ostream& os) const override {}

  protected:

    // protected methods
    // ------------------------------------------------------

    void                  doSetMeasureElementSoundingWholeNotes (
                            const Rational& wholeNotes,
                            const string&   context);

  protected:

    // protected fields
    // ------------------------------------------------------

    /*
      The measure uplink is declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

    Rational              fMeasureElementSoundingWholeNotes;

    string                fMeasureElementMeasureNumber;

    Rational              fMeasureElementPositionInMeasure;
    Rational              fMeasureElementPositionFromBeginningOfVoice;

    msrMoment             fMeasureElementMomentInMeasure;
    msrMoment             fMeasureElementMomentFromBeginningOfVoice;
};
typedef SMARTP<msrMeasureElement> S_msrMeasureElement;
EXP ostream& operator << (ostream& os, const S_msrMeasureElement& elt);


} // namespace MusicFormats


#endif
