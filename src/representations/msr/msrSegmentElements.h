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

    static const Rational K_NO_WHOLE_NOTES;

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
                            const Rational& wholeNotes,
                            const string&   context);

    Rational              getSegmentElementSoundingWholeNotes () const
                              { return fSegmentElementSoundingWholeNotes; }

//     void                  setSegmentElementMeasureNumber (
//                             const string& measurePosition)
//                               {
//                                 fSegmentElementMeasureNumber = measurePosition;
//                               }
//
//     string                getSegmentElementMeasureNumber () const
//                               { return fSegmentElementMeasureNumber; }
//
//     void                  setSegmentElementMeasurePosition (
//                             const Rational& measurePosition,
//                             const string&   context); // v0.9.66 PIM
//
//     Rational              getSegmentElementMeasurePosition () const
//                               { return fSegmentElementMeasurePosition; }
//
//     void                  setSegmentElementVoicePosition (
//                             const Rational& voicePosition,
//                             const string&   context);
//
//     Rational              getSegmentElementVoicePosition () const
//                               { return fSegmentElementVoicePosition; }
//
//     void                  setSegmentElementMeasureMoment (
//                             const msrMoment& measureMoment,
//                             const string&    context);
//
//     const msrMoment&      getSegmentElementMeasureMoment () const
//                               { return fSegmentElementMeasureMoment; }
//
//     void                  setSegmentElementVoiceMoment (
//                             const msrMoment& voiceMoment,
//                             const string&    context);
//
//     const msrMoment&      getSegmentElementVoiceMoment () const
//                               { return fSegmentElementVoiceMoment; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          appendMeasureElementToSegmentElement (
                            S_msrMeasureElement elem) = 0;

//     static bool           compareSegmentElementsByIncreasingMeasurePosition (
//                             const SMARTP<msrSegmentElement>& first,
//                             const SMARTP<msrSegmentElement>& second);
//
//     virtual void          assignSegmentElementVoicePosition (
//                             Rational&     voicePosition,
//                             const string& context);

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
      The segment uplink is declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

    Rational              fSegmentElementSoundingWholeNotes;

//     string                fSegmentElementMeasureNumber;
//
//     Rational              fSegmentElementMeasurePosition;
//     Rational              fSegmentElementVoicePosition;
//
//     msrMoment             fSegmentElementMeasureMoment;
//     msrMoment             fSegmentElementVoiceMoment;
};
typedef SMARTP<msrSegmentElement> S_msrSegmentElement;
EXP ostream& operator << (ostream& os, const S_msrSegmentElement& elt);


} // namespace MusicFormats


#endif
