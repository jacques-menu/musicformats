/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMultipleFullBarRests___
#define ___msrMultipleFullBarRests___

#include "msrSegmentElements.h"

// #include "msrMeasures.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

// enum class msrMultipleFullBarRestsKind { // unused ??? JMI v0.9.66
//   kMultipleFullBarRests_NO_,
//   kMultipleFullBarRestsStart, kMultipleFullBarRestsStop
// };
//
// string msrMultipleFullBarRestsKindAsString (
//   msrMultipleFullBarRestsKind multipleFullBarRestsKind);
//
// ostream& operator << (ostream& os, const msrMultipleFullBarRestsKind& elt);

class EXP msrMultipleFullBarRests : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultipleFullBarRests> create (
                            int             inputLineNumber,
                            int             multipleFullBarRestsNumber,
                            S_msrSegment    upLinkToSegment);

    static SMARTP<msrMultipleFullBarRests> create (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrSegment upLinkToSegment);

    SMARTP<msrMultipleFullBarRests> createMultipleFullBarRestsNewbornClone (
                              S_msrSegment containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMultipleFullBarRests (
                            int             inputLineNumber,
                            int             multipleFullBarRestsNumber,
                            S_msrSegment    upLinkToSegment);

                          msrMultipleFullBarRests (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrSegment upLinkToSegment);

    virtual               ~msrMultipleFullBarRests ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrSegment          getMultipleFullBarRestsUpLinkToSegment () const
                            { return fMultipleFullBarRestsUpLinkToSegment; }

    int                   getMultipleFullBarRestsNumber () const
                              { return fMultipleFullBarRestsNumber; }

    const list<S_msrMeasure>&
                          getFullBarRestsMeasuresList () const
                              { return fFullBarRestsMeasuresList; }

    void                  setMultipleFullBarRestsNextMeasureNumber (
                            const string& nextMeasureNumber);

    string                getMultipleFullBarRestsNextMeasureNumber () const
                              { return fMultipleFullBarRestsNextMeasureNumber; }

    void                  setMultipleFullBarRestsLastMeasurePuristMeasureNumber (
                            int inputLineNumber);

    int                   getMultipleFullBarRestsLastMeasurePuristMeasureNumber () const
                              { return fMultipleFullBarRestsLastMeasurePuristNumber; }

    Rational              fetchMultipleFullBarRestsMeasureSoundingNotes () const;
                            // don't simply multiply duration by number,
                            // may help pin-point errors in representation
  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureElementToSegmentElement (
                            S_msrMeasureElement elem) override;

    void                  appendMeasureToMultipleFullBarRests (
                            S_msrMeasure measure);

    int                   multipleFullBarRestsContentsMeasuresNumber () const
                            {
                              return 0; // JMI
//                                 fMultipleFullBarRestsContents->
//                                   multipleFullBarRestsContentsMeasuresNumber ();
                            }

    void                  collectMultipleFullBarRestsIntoFlatList (
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

    string                asString () const override;

    void                  displayMultipleFullBarRests (
                            int           inputLineNumber,
                            const string& context);

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrSegment          fMultipleFullBarRestsUpLinkToSegment;

    int                   fMultipleFullBarRestsNumber; // supplied by MusicXML
    list<S_msrMeasure>    fFullBarRestsMeasuresList;

    int                   fMultipleFullBarRestsLastMeasurePuristNumber;

    string                fMultipleFullBarRestsNextMeasureNumber;
};
typedef SMARTP<msrMultipleFullBarRests> S_msrMultipleFullBarRests;
EXP ostream& operator << (ostream& os, const S_msrMultipleFullBarRests& elt);


}


#endif
