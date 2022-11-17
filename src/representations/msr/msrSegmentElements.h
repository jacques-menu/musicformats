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

#include "mfRational.h"

#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
  Various elements can found in segments,
  i.e. measures and some kind of repeated measures,
  hence class   msrSegmentElement
*/

class EXP msrSegmentElement : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    static const std::string K_NO_MEASURE_NUMBER;

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
                            const std::string&   context);

    Rational              getSegmentElementSoundingWholeNotes () const
                              { return fSegmentElementSoundingWholeNotes; }

//     void                  setSegmentElementMeasureNumber (
//                             const std::string& measurePosition)
//                               {
//                                 fSegmentElementMeasureNumber = measurePosition;
//                               }
//
//     std::string           getSegmentElementMeasureNumber () const
//                               { return fSegmentElementMeasureNumber; }
//
//     Rational              getSegmentElementMeasurePosition () const
//                               { return fSegmentElementMeasurePosition; }
//
//     void                  setSegmentElementVoicePosition (
//                             const Rational& voicePosition,
//                             const std::string&   context);
//
//     Rational              getSegmentElementVoicePosition () const
//                               { return fSegmentElementVoicePosition; }
//
//     void                  setSegmentElementMeasureMoment (
//                             const msrMoment& measureMoment,
//                             const std::string&    context);
//
//     const msrMoment&      getSegmentElementMeasureMoment () const
//                               { return fSegmentElementMeasureMoment; }
//
//     void                  setSegmentElementVoiceMoment (
//                             const msrMoment& voiceMoment,
//                             const std::string&    context);
//
//     const msrMoment&      getSegmentElementVoiceMoment () const
//                               { return fSegmentElementVoiceMoment; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          appendMeasureElementToSegmentElement (
                            const S_msrMeasureElement& elem) = 0;

//     static bool           compareSegmentElementsByIncreasingMeasurePosition (
//                             const SMARTP<msrSegmentElement>& first,
//                             const SMARTP<msrSegmentElement>& second);
//
//     virtual void          assignSegmentElementVoicePosition (
//                             Rational&     voicePosition,
//                             const std::string& context);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override = 0; // JMI ???

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printShort (std::ostream& os) const override = 0;

    void                  printSummary (std::ostream& os) const override {}

  protected:

    // protected fields
    // ------------------------------------------------------

    /*
      The segment uplink is declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

    Rational              fSegmentElementSoundingWholeNotes;

//     std::string           fSegmentElementMeasureNumber;
//
//     Rational              fSegmentElementMeasurePosition;
//     Rational              fSegmentElementVoicePosition;
//
//     msrMoment             fSegmentElementMeasureMoment;
//     msrMoment             fSegmentElementVoiceMoment;
};
typedef SMARTP<msrSegmentElement> S_msrSegmentElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrSegmentElement& elt);


}


#endif
