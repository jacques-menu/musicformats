/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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

string msrTimeSignatureSymbolKindAsString (
  msrTimeSignatureSymbolKind timeSignatureSymbolKind);

ostream& operator << (ostream& os, const msrTimeSignatureSymbolKind& elt);

enum class msrTimeSignatureSeparatorKind {
  kTimeSignatureSeparatorNone,
  kTimeSignatureSeparatorHorizontal,
  kTimeSignatureSeparatorDiagonal,
  kTimeSignatureSeparatorVertical,
  kTimeSignatureSeparatorAdjacent
};

string msrTimeSignatureSeparatorKindAsString (
  msrTimeSignatureSeparatorKind timeSignatureSeparatorKind);

ostream& operator << (ostream& os, const msrTimeSignatureSeparatorKind& elt);

enum class msrTimeSignatureRelationKind {
  kTimeSignatureRelationNone,
  kTimeSignatureRelationParentheses,
  kTimeSignatureRelationBracket,
  kTimeSignatureRelationEquals,
  kTimeSignatureRelationSlash,
  kTimeSignatureRelationSpace,
  kTimeSignatureRelationHyphen
};

string msrTimeSignatureRelationKindAsString (
  msrTimeSignatureRelationKind timeSignatureRelationKind);

ostream& operator << (ostream& os, const msrTimeSignatureRelationKind& elt);

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

    const vector<int>&    getTimeSignatureBeatsNumbersVector () const
                              { return fTimeSignatureBeatsNumbersVector; }

    void                  setTimeSignatureBeatValue (int timeSignatureBeatValue);

    int                   getTimeSignatureBeatValue () const
                              { return fTimeSignatureBeatValue; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (
                            S_msrTimeSignatureItem otherTimeSignatureItem) const;

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

    string                asString () const override;
    string                asShortStringForMeasuresSlices () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    vector<int>           fTimeSignatureBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeSignatureBeatValue;
};
typedef SMARTP<msrTimeSignatureItem> S_msrTimeSignatureItem;
EXP ostream& operator << (ostream& os, const S_msrTimeSignatureItem& elt);

//______________________________________________________________________________
class EXP msrTimeSignature : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> create (
                            int           inputLineNumber,
                            S_msrMeasure  upLinkToMeasure,
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
                            int           inputLineNumber,
                            const string& timeString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTimeSignature (
                            int           inputLineNumber,
                            S_msrMeasure  upLinkToMeasure,
                            msrTimeSignatureSymbolKind
                                          timeSignatureSymbolKind);

    virtual               ~msrTimeSignature ();

  public:

    // set and get
    // ------------------------------------------------------

    // position in measure
    void                  setMeasureElementMeasurePosition (
                            const S_msrMeasure measure,
                            const Rational&    measurePosition,
                            const string&      context) override
                              {
                                setTimeSignatureMeasurePosition (
                                  measure,
                                  measurePosition,
                                  context);
                              }

    void                  setTimeSignatureMeasurePosition (
                            const S_msrMeasure measure,
                            const Rational&    measurePosition,
                            const string&      context);

    msrTimeSignatureSymbolKind
                          getTimeSignatureSymbolKind () const
                              { return fTimeSignatureSymbolKind; }

    Bool                  getTimeIsCompound () const
                              { return fTimeIsCompound; }

    const vector<S_msrTimeSignatureItem>&
                          getTimeSignatureItemsVector () const
                              { return fTimeSignatureItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (S_msrTimeSignature otherTimeSignature) const;

    void                  appendTimeSignatureItem (
                            S_msrTimeSignatureItem timeSignatureItem);

    Rational              wholeNotesDurationPerMeasure () const;

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

    string                asShortString () const override;
    string                asShortStringForMeasuresSlices () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrTimeSignatureSymbolKind
                          fTimeSignatureSymbolKind;

    vector<S_msrTimeSignatureItem>
                          fTimeSignatureItemsVector;

    // a time is compound if it contains several items
    // or if the only one has several beats numbers
    // i.e. 3/4 is not, (3+4)/8 is, and 2/4+3/4 is too
    Bool                  fTimeIsCompound;
};
typedef SMARTP<msrTimeSignature> S_msrTimeSignature;
EXP ostream& operator << (ostream& os, const S_msrTimeSignature& elt);


}


#endif
