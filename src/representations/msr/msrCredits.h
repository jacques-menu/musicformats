/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrCredits___
#define ___msrCredits___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrCreditWords : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCreditWords> create (
                            int                        inputLineNumber,
                            const string&              creditWordsContents,
                            float                      creditWordsDefaultX,
                            float                      creditWordsDefaultY,
                            const string&              creditWordsFontFamily,
                            float                      creditWordsFontSize,
                            msrFontWeightKind          creditWordsFontWeightKind,
                            msrFontStyleKind           creditWordsFontStyleKind,
                            msrJustifyKind             creditWordsJustifyKind,
                            msrHorizontalAlignmentKind creditWordsHorizontalAlignmentKind,
                            msrVerticalAlignmentKind   creditWordsVerticalAlignmentKind,
                            msrXMLLangKind             creditWordsXMLLang);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrCreditWords (
                            int                        inputLineNumber,
                            const string&              creditWordsContents,
                            float                      creditWordsDefaultX,
                            float                      creditWordsDefaultY,
                            const string&              creditWordsFontFamily,
                            float                      creditWordsFontSize,
                            msrFontWeightKind          creditWordsFontWeightKind,
                            msrFontStyleKind           creditWordsFontStyleKind,
                            msrJustifyKind             creditWordsJustifyKind,
                            msrHorizontalAlignmentKind creditWordsHorizontalAlignmentKind,
                            msrVerticalAlignmentKind   creditWordsVerticalAlignmentKind,
                            msrXMLLangKind             creditWordsXMLLang);

    virtual               ~msrCreditWords ();

  public:

    // set and get
    // ------------------------------------------------------
    string                getCreditWordsContents () const
                              { return fCreditWordsContents; }

    float                 getCreditWordsDefaultX () const
                              { return fCreditWordsDefaultX; }

    float                 getCreditWordsDefaultY () const
                              { return fCreditWordsDefaultY; }

    string                getCreditWordsFontFamily () const
                              { return fCreditWordsFontFamily; }

    float                 getCreditWordsFontSize () const
                              { return fCreditWordsFontSize; }

    msrFontWeightKind     getCreditWordsFontWeightKind () const
                              { return fCreditWordsFontWeightKind; }

    msrFontStyleKind      getCreditWordsFontStyleKind () const
                              { return fCreditWordsFontStyleKind; }

    msrJustifyKind        getCreditWordsJustifyKind () const
                              { return fCreditWordsJustifyKind; }

    msrHorizontalAlignmentKind
                          getCreditWordsHorizontalAlignmentKind () const
                              { return fCreditWordsHorizontalAlignmentKind; }

    msrVerticalAlignmentKind
                          getCreditWordsVerticalAlignmentKind () const
                              { return fCreditWordsVerticalAlignmentKind; }

    msrXMLLangKind        getCreditWordsXMLLang () const
                              { return fCreditWordsXMLLang; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

/*
    <credit-words default-x="607" default-y="1443" font-family="ＭＳ ゴシック" font-size="24" font-weight="bold" justify="center" valign="top" xml:lang="ja">越後獅子</credit-words>
*/

    string                fCreditWordsContents;

    float                 fCreditWordsDefaultX;
    float                 fCreditWordsDefaultY;

    string                fCreditWordsFontFamily;
    float                 fCreditWordsFontSize;
    msrFontWeightKind     fCreditWordsFontWeightKind;
    msrFontStyleKind      fCreditWordsFontStyleKind;
    msrJustifyKind        fCreditWordsJustifyKind;
    msrHorizontalAlignmentKind
                          fCreditWordsHorizontalAlignmentKind;
    msrVerticalAlignmentKind
                          fCreditWordsVerticalAlignmentKind;
    msrXMLLangKind        fCreditWordsXMLLang;
};
typedef SMARTP<msrCreditWords> S_msrCreditWords;
EXP ostream& operator<< (ostream& os, const S_msrCreditWords& elt);

//______________________________________________________________________________
class EXP msrCredit : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCredit> create (
                            int inputLineNumber,
                            int creditPageNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrCredit (
                            int inputLineNumber,
                            int creditPageNumber);

    virtual               ~msrCredit ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getCreditPageNumber () const
                              { return fCreditPageNumber; }

    const vector<S_msrCreditWords>&
                          getCreditWordsList () const
                              { return fCreditWordsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendCreditWordsToCredit (
                            S_msrCreditWords creditWords);

    string                asString () const override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fCreditPageNumber;

    vector<S_msrCreditWords>
                          fCreditWordsList;
};
typedef SMARTP<msrCredit> S_msrCredit;
EXP ostream& operator<< (ostream& os, const S_msrCredit& elt);


} // namespace MusicFormats


#endif
