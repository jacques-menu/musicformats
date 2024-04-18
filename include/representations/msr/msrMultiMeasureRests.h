/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMultiMeasureRests___
#define ___msrMultiMeasureRests___

#include <list>

#include "msrNotesDurations.h"
#include "msrSegmentElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMultiMeasureRest : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultiMeasureRest> create (
                            int                 inputLineNumber,
                            int                 measureRestsNumber,
                            const S_msrSegment& upLinkToSegment);

//     static SMARTP<msrMultiMeasureRest> create (
//                             int                 inputLineNumber,
//                             const S_msrMeasure& restMeasureClone,
//                             const S_msrSegment& upLinkToSegment);

    SMARTP<msrMultiMeasureRest> createMultiMeasureRestNewbornClone (
                              const S_msrSegment& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMultiMeasureRest (
                            int                 inputLineNumber,
                            int                 measureRestsNumber,
                            const S_msrSegment& upLinkToSegment);

//                           msrMultiMeasureRest (
//                             int                 inputLineNumber,
//                             const S_msrMeasure& restMeasureClone,
//                             const S_msrSegment& upLinkToSegment);

    virtual               ~msrMultiMeasureRest ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrSegment          getMultiMeasureRestUpLinkToSegment () const
                              { return fMultiMeasureRestUpLinkToSegment; }

    int                   getMeasureRestsNumber () const
                              { return fMeasureRestsNumber; }

    const std::list<S_msrMeasure>&
                          getMeasureRestsList () const
                              { return fMeasureRestsList; }

    void                  setNextMeasureNumber (
                            const std::string& nextMeasureNumber);

    std::string           getNextMeasureNumber () const
                              { return fNextMeasureNumber; }

    void                  setMultiMeasureRestLastMeasurePuristMeasureNumber (
                            int inputLineNumber);

    int                   getMultiMeasureRestLastMeasurePuristMeasureNumber () const
                              { return fLastMeasureRestPuristNumber; }

    msrWholeNotes         fetchMultiMeasureRestMeasureSoundingNotes () const;
                            // don't simply multiply duration by number,
                            // may help pin-point errors in representation
  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureElementToSegmentElement (
                            const S_msrMeasureElement& elem) override;

    void                  appendMeasureToMultiMeasureRest (
                            const S_msrMeasure& measure);

    void                  collectMultiMeasureRestIntoFlatList (
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

    void                  displayMultiMeasureRest (
                            int                inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrSegment          fMultiMeasureRestUpLinkToSegment;

    int                   fMeasureRestsNumber; // supplied by MusicXML
    std::list<S_msrMeasure>
                          fMeasureRestsList;

    int                   fLastMeasureRestPuristNumber;

    std::string           fNextMeasureNumber;
};
typedef SMARTP<msrMultiMeasureRest> S_msrMultiMeasureRest;
EXP std::ostream& operator << (std::ostream& os, const S_msrMultiMeasureRest& elt);


}


#endif // ___msrMultiMeasureRests___
