/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTimeSignatures___
#define ___msrTimeSignatures___

#include <vector>

#include "msrElements.h"
#include "msrMeasureElements.h"


namespace MusicFormats
{

// time signatures
//______________________________________________________________________________
enum class msrTimeSignatureSymbolKind {
  kTimeSignatureSymbolNone,
  kTimeSignatureSymbolCommon,
  kTimeSignatureSymbolCut,
  kTimeSignatureSymbolNote,
  kTimeSignatureSymbolDottedNote,
  kTimeSignatureSymbolSingleNumber,
  kTimeSignatureSymbolSenzaMisura
};

std::string msrTimeSignatureSymbolKindAsString (
  msrTimeSignatureSymbolKind timeSignatureSymbolKind);

std::ostream& operator << (std::ostream& os, const msrTimeSignatureSymbolKind& elt);

enum class msrTimeSignatureSeparatorKind {
  kTimeSignatureSeparatorNone,
  kTimeSignatureSeparatorHorizontal,
  kTimeSignatureSeparatorDiagonal,
  kTimeSignatureSeparatorVertical,
  kTimeSignatureSeparatorAdjacent
};

std::string msrTimeSignatureSeparatorKindAsString (
  msrTimeSignatureSeparatorKind timeSignatureSeparatorKind);

std::ostream& operator << (std::ostream& os, const msrTimeSignatureSeparatorKind& elt);

enum class msrTimeSignatureRelationKind {
  kTimeSignatureRelationNone,
  kTimeSignatureRelationParentheses,
  kTimeSignatureRelationBracket,
  kTimeSignatureRelationEquals,
  kTimeSignatureRelationSlash,
  kTimeSignatureRelationSpace,
  kTimeSignatureRelationHyphen
};

std::string msrTimeSignatureRelationKindAsString (
  msrTimeSignatureRelationKind timeSignatureRelationKind);

std::ostream& operator << (std::ostream& os, const msrTimeSignatureRelationKind& elt);

//______________________________________________________________________________
class EXP msrTimeSignatureItem : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTimeSignatureItem> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTimeSignatureItem (
                            int inputLineNumber);

    virtual               ~msrTimeSignatureItem ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::vector<int>&
                          getTimeSignatureBeatsNumbersVector () const
                              { return fTimeSignatureBeatsNumbersVector; }

    void                  setTimeSignatureBeatValue (int timeSignatureBeatValue);

    int                   getTimeSignatureBeatValue () const
                              { return fTimeSignatureBeatValue; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (
                            const S_msrTimeSignatureItem& otherTimeSignatureItem) const;

    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeSignatureBeatsNumber () const;

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
    std::string           asShortStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::vector<int>      fTimeSignatureBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeSignatureBeatValue;
};
typedef SMARTP<msrTimeSignatureItem> S_msrTimeSignatureItem;
EXP std::ostream& operator << (std::ostream& os, const S_msrTimeSignatureItem& elt);

//______________________________________________________________________________
class EXP msrTimeSignature : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> create (
                            int            inputLineNumber,
                            const S_msrMeasure&   upLinkToMeasure,
                            msrTimeSignatureSymbolKind
                                           timeSignatureSymbolKind);

    static SMARTP<msrTimeSignature> create (
                            int            inputLineNumber,
                            msrTimeSignatureSymbolKind
                                           timeSignatureSymbolKind);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> createTwoEightsTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createThreeEightsTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createSixEightsTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createTwoQuartersTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createThreeQuartersTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createFourQuartersTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createFiveQuartersTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createTwoHalvesTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createThreeHalvesTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createFourHalvesTime (
                            int inputLineNumber);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> createTimeFromString (
                            int                inputLineNumber,
                            const std::string& timeString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTimeSignature (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            msrTimeSignatureSymbolKind
                                          timeSignatureSymbolKind);

    virtual               ~msrTimeSignature ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setTimeSignatureUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getTimeSignatureUpLinkToMeasure (); }

    void                  setTimeSignatureUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getTimeSignatureUpLinkToMeasure () const
                            { return fTimeSignatureUpLinkToMeasure; }

    msrTimeSignatureSymbolKind
                          getTimeSignatureSymbolKind () const
                              { return fTimeSignatureSymbolKind; }

    Bool                  getTimeIsCompound () const
                              { return fTimeIsCompound; }

    const std::vector<S_msrTimeSignatureItem>&
                          getTimeSignatureItemsVector () const
                              { return fTimeSignatureItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (
                            S_msrTimeSignature otherTimeSignature) const;

    void                  appendTimeSignatureItem (
                            const S_msrTimeSignatureItem& timeSignatureItem);

    msrWholeNotes         timeSignatureWholeNotesPerMeasure () const;

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

    std::string           asShortString () const override;
    std::string           asShortStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrMeasure          fTimeSignatureUpLinkToMeasure;

    msrTimeSignatureSymbolKind
                          fTimeSignatureSymbolKind;

    std::vector<S_msrTimeSignatureItem>
                          fTimeSignatureItemsVector;

    // a time is compound if it contains several items
    // or if the only one has several beats numbers
    // i.e. 3/4 is not, (3+4)/8 is, and 2/4+3/4 is too
    Bool                  fTimeIsCompound;
};
typedef SMARTP<msrTimeSignature> S_msrTimeSignature;
EXP std::ostream& operator << (std::ostream& os, const S_msrTimeSignature& elt);


}


#endif // ___msrTimeSignatures___
