/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSegmentElements___
#define ___msrSegmentElements___

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
  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSegmentElement (
                            int inputLineNumber);

    virtual               ~msrSegmentElement ();

  public:

    // set and get
    // ------------------------------------------------------

//     void                  setSegmentElementSoundingWholeNotes (
//                             const msrWholeNotes& wholeNotes,
//                             const std::string&   context);

//     mfWholeNotes            getSegmentElementSoundingWholeNotes () const
//                               { return fSegmentElementSoundingWholeNotes; }

//     void                  setSegmentElementMeasureMoment (
//                             const msrMoment& measureMoment,
//                             const std::string&         context);
//
//     const msrMoment&      getSegmentElementMeasureMoment () const
//                               { return fSegmentElementMeasureMoment; }
//
//     void                  setSegmentElementVoiceMoment (
//                             const msrMoment& voiceMoment,
//                             const std::string&         context);
//
//     const msrMoment&      getSegmentElementVoiceMoment () const
//                               { return fSegmentElementVoiceMoment; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          appendMeasureElementToSegmentElement (
                            const S_msrMeasureElement& elem) = 0;

//     static bool           compareSegmentElementsByIncreasingPositionInMeasure (
//                             const SMARTP<msrSegmentElement>& first,
//                             const SMARTP<msrSegmentElement>& second);

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
    void                  printFull (std::ostream& os) const override; // JMI ??? v0.9.70

    void                  printSummary (std::ostream& os) const override {}

  protected:

    // protected fields
    // ------------------------------------------------------

    /*
      The segment uplink is declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

//     mfWholeNotes              fSegmentElementSoundingWholeNotes;

//     msrMoment             fSegmentElementMeasureMoment;
//     msrMoment             fSegmentElementVoiceMoment;
};
typedef SMARTP<msrSegmentElement> S_msrSegmentElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrSegmentElement& elt);


}


#endif // ___msrSegmentElements___
