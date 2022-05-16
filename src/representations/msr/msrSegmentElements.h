/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSegmentElements___
#define ___msrSegmentElements___

#include "msrElements.h"
#include "msrMeasureElements.h"

#include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
  Various elements can found in segments,
  i.e. measures and some kind of repeated measures,
  hence class msrSegmentElement
*/

class EXP msrSegmentElement : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    static const string K_NO_MEASURE_NUMBER;

    static const rational K_NO_WHOLE_NOTES;

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSegmentElement (
                            int inputLineNumber);

    virtual               ~msrSegmentElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSegmentElementSoundingWholeNotes (
                            const rational& wholeNotes,
                            const string&   context);

    rational              getSegmentElementSoundingWholeNotes () const
                              { return fSegmentElementSoundingWholeNotes; }

    void                  setSegmentElementMeasureNumber (
                            const string& positionInMeasure)
                              {
                                fSegmentElementMeasureNumber = positionInMeasure;
                              }

    string                getSegmentElementMeasureNumber () const
                              { return fSegmentElementMeasureNumber; }

    void                  setSegmentElementPositionInMeasure (
                            const rational& positionInMeasure,
                            const string&   context);

    rational              getSegmentElementPositionInMeasure () const
                              { return fSegmentElementPositionInMeasure; }

    void                  setSegmentElementPositionInVoice (
                            const rational& positionInVoice,
                            const string&   context);

    rational              getSegmentElementPositionInVoice () const
                              { return fSegmentElementPositionInVoice; }

    void                  setSegmentElementMomentInMeasure (
                            const msrMoment& momentInMeasure,
                            const string&    context);

    const msrMoment&      getSegmentElementMomentInMeasure () const
                              { return fSegmentElementMomentInMeasure; }

    void                  setSegmentElementMomentInVoice (
                            const msrMoment& momentInVoice,
                            const string&    context);

    const msrMoment&      getSegmentElementMomentInVoice () const
                              { return fSegmentElementMomentInVoice; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          appendMeasureElementToSegmentElement (
                            S_msrMeasureElement elem) = 0;

    static bool           compareSegmentElementsByIncreasingPositionInMeasure (
                            const SMARTP<msrSegmentElement>& first,
                            const SMARTP<msrSegmentElement>& second);

    virtual void          assignSegmentElementPositionInVoice (
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
    void                  printShort (ostream& os) const override = 0;

    void                  printSummary (ostream& os) const override {}

  protected:

    // protected fields
    // ------------------------------------------------------

    /*
      The measure uplink is declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

    rational              fSegmentElementSoundingWholeNotes;

    string                fSegmentElementMeasureNumber;

    rational              fSegmentElementPositionInMeasure;
    rational              fSegmentElementPositionInVoice;

    msrMoment             fSegmentElementMomentInMeasure;
    msrMoment             fSegmentElementMomentInVoice;
};
typedef SMARTP<msrSegmentElement> S_msrSegmentElement;
EXP ostream& operator<< (ostream& os, const S_msrSegmentElement& elt);


} // namespace MusicFormats


#endif
