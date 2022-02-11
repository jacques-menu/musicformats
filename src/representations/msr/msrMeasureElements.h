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

    static const rational K_NO_WHOLE_NOTES;

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
                            const rational& wholeNotes,
                            const string&   context);

    rational              getMeasureElementSoundingWholeNotes () const
                              { return fMeasureElementSoundingWholeNotes; }

    void                  setMeasureElementMeasureNumber (
                            const string& positionInMeasure)
                              {
                                fMeasureElementMeasureNumber = positionInMeasure;
                              }

    string                getMeasureElementMeasureNumber () const
                              { return fMeasureElementMeasureNumber; }

    void                  setMeasureElementPositionInMeasure (
                            const rational& positionInMeasure,
                            const string&   context);

    rational              getMeasureElementPositionInMeasure () const
                              { return fMeasureElementPositionInMeasure; }

    void                  setMeasureElementPositionInVoice (
                            const rational& positionInVoice,
                            const string&   context);

    rational              getMeasureElementPositionInVoice () const
                              { return fMeasureElementPositionInVoice; }

    void                  setMeasureElementMomentInMeasure (
                            const msrMoment& momentInMeasure,
                            const string&    context);

    const msrMoment&      getMeasureElementMomentInMeasure () const
                              { return fMeasureElementMomentInMeasure; }

    void                  setMeasureElementMomentInVoice (
                            const msrMoment& momentInVoice,
                            const string&    context);

    const msrMoment&      getMeasureElementMomentInVoice () const
                              { return fMeasureElementMomentInVoice; }

  public:

    // public services
    // ------------------------------------------------------

    static bool           compareMeasureElementsByIncreasingPositionInMeasure (
                            const SMARTP<msrMeasureElement>& first,
                            const SMARTP<msrMeasureElement>& second);

    virtual void          assignMeasureElementPositionInVoice (
                            rational&     positionInVoice,
                            const string& context);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override = 0; // JMI ???

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const override;
    string                asString () const override;

    void                  print (ostream& os) const override;

    void                  printSummary (ostream& os) const override {}

  protected:

    // protected fields
    // ------------------------------------------------------

    /*
      The measure uplink is declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

    rational              fMeasureElementSoundingWholeNotes;

    string                fMeasureElementMeasureNumber;

    rational              fMeasureElementPositionInMeasure;
    rational              fMeasureElementPositionInVoice;

    msrMoment             fMeasureElementMomentInMeasure;
    msrMoment             fMeasureElementMomentInVoice;
};
typedef SMARTP<msrMeasureElement> S_msrMeasureElement;
EXP ostream& operator<< (ostream& os, const S_msrMeasureElement& elt);


} // namespace MusicFormats


#endif
