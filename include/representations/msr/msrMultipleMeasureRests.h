/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMultipleMeasureRests___
#define ___msrMultipleMeasureRests___

#include <list>

#include "mfDurationsAndPositionInMeasures.h"
#include "msrSegmentElements.h"
#include "msrUseSymbols.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMultipleMeasureRest : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultipleMeasureRest> create (
                            const mfInputLineNumber& inputLineNumber,
                            int               measuresNumber,
                            int               slashesNumber,
                            msrUseSymbolsKind fUseSymbolsKind);

    SMARTP<msrMultipleMeasureRest> createMultipleMeasureRestNewbornClone (
                              const S_msrSegment& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMultipleMeasureRest (
                            const mfInputLineNumber& inputLineNumber,
                            int               measuresNumber,
                            int               slashesNumber,
                            msrUseSymbolsKind fUseSymbolsKind);

    virtual               ~msrMultipleMeasureRest ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrSegment          getMultipleMeasureRestUpLinkToSegment () const
                              { return fMultipleMeasureRestUpLinkToSegment; }

    int                   getMeasuresNumber () const
                              { return fMeasuresNumber; }

    int                   getSlashesNumber () const
                              { return fSlashesNumber; }

    msrUseSymbolsKind     getUseSymbolsKind () const
                              { return fUseSymbolsKind; }

    const std::list <S_msrMeasure>&
                          getMeasuresList () const
                              { return fMeasuresList; }

    void                  setNextMeasureNumber (
                            const mfMeasureNumber& nextMeasureNumber);

    std::string           getNextMeasureNumber () const
                              { return fNextMeasureNumber; }

    void                  setLastMeasurePuristMeasureNumber (
                            const mfInputLineNumber& inputLineNumber,
                            int puristMeasureNumber);

    int                   getLastMeasurePuristMeasureNumber () const
                              { return fLastMeasurePuristNumber; }

    mfWholeNotes          fetchMultipleMeasureRestMeasureSoundingNotes () const;
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
                            const mfInputLineNumber& inputLineNumber);

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
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrSegment          fMultipleMeasureRestUpLinkToSegment;

    int                   fMeasuresNumber;
    int                   fSlashesNumber;
    msrUseSymbolsKind     fUseSymbolsKind;

    std::list <S_msrMeasure>
                          fMeasuresList;

    int                   fLastMeasurePuristNumber;

    std::string           fNextMeasureNumber;
};
typedef SMARTP<msrMultipleMeasureRest> S_msrMultipleMeasureRest;
EXP std::ostream& operator << (std::ostream& os, const S_msrMultipleMeasureRest& elt);


}


#endif // ___msrMultipleMeasureRests___
