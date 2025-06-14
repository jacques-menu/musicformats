/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTimeSignatures___
#define ___msrTimeSignatures___

#include <vector>


namespace MusicFormats
{

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
                            const mfInputLineNumber& inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTimeSignatureItem (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~msrTimeSignatureItem ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::vector <int>&
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

//     std::string           asStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::vector <int>      fTimeSignatureBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeSignatureBeatValue;
};
typedef SMARTP<msrTimeSignatureItem> S_msrTimeSignatureItem;
EXP std::ostream& operator << (std::ostream& os, const S_msrTimeSignatureItem& elt);

//______________________________________________________________________________
// PRE-declaration for class mutual dependencies
class   msrTimeSignature;
typedef SMARTP<msrTimeSignature> S_msrTimeSignature;

//______________________________________________________________________________
class EXP msrTimeSignature : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,
                            msrTimeSignatureSymbolKind timeSignatureSymbolKind);

    static SMARTP<msrTimeSignature> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrTimeSignatureSymbolKind timeSignatureSymbolKind);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> createTwoEightsTime (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrTimeSignature> createThreeEightsTime (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrTimeSignature> createSixEightsTime (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrTimeSignature> createTwoQuartersTime (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrTimeSignature> createThreeQuartersTime (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrTimeSignature> createFourQuartersTime (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrTimeSignature> createFiveQuartersTime (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrTimeSignature> createTwoHalvesTime (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrTimeSignature> createThreeHalvesTime (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrTimeSignature> createFourHalvesTime (
                            const mfInputLineNumber& inputLineNumber);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> createTimeFromString (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& timeString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTimeSignature (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,
                            msrTimeSignatureSymbolKind timeSignatureSymbolKind);

    virtual               ~msrTimeSignature ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTimeSignatureSymbolKind
                          getTimeSignatureSymbolKind () const
                              { return fTimeSignatureSymbolKind; }

    Bool                  getTimeIsCompound () const
                              { return fTimeIsCompound; }

    const std::vector <S_msrTimeSignatureItem>&
                          getTimeSignatureItemsVector () const
                              { return fTimeSignatureItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (
                            S_msrTimeSignature otherTimeSignature) const;

    void                  appendTimeSignatureItem (
                            const S_msrTimeSignatureItem& timeSignatureItem);

    mfWholeNotes         timeSignatureWholeNotesPerMeasure () const;

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

//     std::string           asStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrTimeSignatureSymbolKind
                          fTimeSignatureSymbolKind;

    std::vector <S_msrTimeSignatureItem>
                          fTimeSignatureItemsVector;

    // a time signature is compound if it contains several items
    // or if the only one has several beats numbers
    // i.e. 3/4 is not, (3+4)/8 is, and 2/4+3/4 is too
    Bool                  fTimeIsCompound;
};
typedef SMARTP<msrTimeSignature> S_msrTimeSignature;
EXP std::ostream& operator << (std::ostream& os, const S_msrTimeSignature& elt);
EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const S_msrTimeSignature& elt);


}


#endif // ___msrTimeSignatures___
