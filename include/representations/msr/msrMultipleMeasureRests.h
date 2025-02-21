/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMultipleMeasureRests___
#define ___msrMultipleMeasureRests___

#include <list>

#include "msrNotesDurations.h"
#include "msrSegmentElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMultipleMeasureRest : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultipleMeasureRest> create (
                            int                 inputLineNumber,
                            int                 measureRestsNumber,
                            const S_msrSegment& upLinkToSegment);

//     static SMARTP<msrMultipleMeasureRest> create (
//                             int                 inputLineNumber,
//                             const S_msrMeasure& restMeasureClone,
//                             const S_msrSegment& upLinkToSegment);

    SMARTP<msrMultipleMeasureRest> createMultipleMeasureRestNewbornClone (
                              const S_msrSegment& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMultipleMeasureRest (
                            int                 inputLineNumber,
                            int                 measureRestsNumber,
                            const S_msrSegment& upLinkToSegment);

//                           msrMultipleMeasureRest (
//                             int                 inputLineNumber,
//                             const S_msrMeasure& restMeasureClone,
//                             const S_msrSegment& upLinkToSegment);

    virtual               ~msrMultipleMeasureRest ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrSegment          getMultipleMeasureRestUpLinkToSegment () const
                              { return fMultipleMeasureRestUpLinkToSegment; }

    int                   getMeasureRestsNumber () const
                              { return fMeasureRestsNumber; }

    const std::list <S_msrMeasure>&
                          getMeasureRestsList () const
                              { return fMeasureRestsList; }

    void                  setNextMeasureNumber (
                            const std::string& nextMeasureNumber);

    std::string           getNextMeasureNumber () const
                              { return fNextMeasureNumber; }

    void                  setMultipleMeasureRestLastMeasurePuristMeasureNumber (
                            int inputLineNumber);

    int                   getMultipleMeasureRestLastMeasurePuristMeasureNumber () const
                              { return fLastMeasureRestPuristNumber; }

    msrWholeNotes         fetchMultipleMeasureRestMeasureSoundingNotes () const;
                            // don't simply multiply duration by number,
                            // may help pin-point errors in representation
  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureElementToSegmentElement (
                            const S_msrMeasureElement& elem) override;

    void                  appendMeasureToMultipleMeasureRest (
                            const S_msrMeasure& measure);

    void                  collectMultipleMeasureRestIntoFlatList (
                            int inputLineNumber);

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

    void                  displayMultipleMeasureRest (
                            int                inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrSegment          fMultipleMeasureRestUpLinkToSegment;

    int                   fMeasureRestsNumber; // supplied by MusicXML
    std::list <S_msrMeasure>
                          fMeasureRestsList;

    int                   fLastMeasureRestPuristNumber;

    std::string           fNextMeasureNumber;
};
typedef SMARTP<msrMultipleMeasureRest> S_msrMultipleMeasureRest;
EXP std::ostream& operator << (std::ostream& os, const S_msrMultipleMeasureRest& elt);


}


#endif // ___msrMultipleMeasureRests___
