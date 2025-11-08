/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMultiMeasureRests___
#define ___msrMultiMeasureRests___

#include <list>

#include "mfDurationsAndPositionInMeasures.h"
#include "msrSegmentElements.h"
#include "msrUseSymbols.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMultiMeasureRest : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultiMeasureRest> create (
                            const mfInputLineNumber& inputLineNumber,
                            int               measuresNumber,
                            int               slashesNumber,
                            msrUseSymbolsKind fUseSymbolsKind);

    SMARTP<msrMultiMeasureRest> createMultiMeasureRestNewbornClone (
                              const S_msrSegment& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMultiMeasureRest (
                            const mfInputLineNumber& inputLineNumber,
                            int               measuresNumber,
                            int               slashesNumber,
                            msrUseSymbolsKind fUseSymbolsKind);

    virtual               ~msrMultiMeasureRest ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrSegment          getMultiMeasureRestUpLinkToSegment () const
                              { return fMultiMeasureRestUpLinkToSegment; }

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

    mfMeasureNumber       getNextMeasureNumber () const
                              { return fNextMeasureNumber; }

    void                  setLastMeasurePuristMeasureNumber (
                            const mfInputLineNumber& inputLineNumber,
                            int puristMeasureNumber);

    int                   getLastMeasurePuristMeasureNumber () const
                              { return fLastMeasurePuristNumber; }

    mfWholeNotes          fetchMultiMeasureRestMeasureSoundingNotes () const;
                            // don't simply multiply duration by number,
                            // may help pin-point errors in representation
  public:

    // public services
    // ------------------------------------------------------

//     void                  appendMeasureElementToSegmentElement (
//                             const S_msrMeasureElement& elem) override;

    void                  appendMeasureToMultiMeasureRest (
                            const S_msrMeasure& measure);

    void                  collectMultiMeasureRestIntoFlatList (
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

    void                  displayMultiMeasureRest (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrSegment          fMultiMeasureRestUpLinkToSegment;

    int                   fMeasuresNumber;
    int                   fSlashesNumber;
    msrUseSymbolsKind     fUseSymbolsKind;

    std::list <S_msrMeasure>
                          fMeasuresList;

    int                   fLastMeasurePuristNumber;

    mfMeasureNumber       fNextMeasureNumber;
};


using S_msrMultiMeasureRest = SMARTP<msrMultiMeasureRest>;

EXP std::ostream& operator << (std::ostream& os, const S_msrMultiMeasureRest& elt);

std::string fetchMultiMeasureRestAsShortString (
  const S_msrMultiMeasureRest& multiMeasureRest);
std::string fetchMultiMeasureRestAsString (
  const S_msrMultiMeasureRest& multiMeasureRest);


}


#endif // ___msrMultiMeasureRests___
