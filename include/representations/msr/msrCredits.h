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

#include <vector>

#include "msrElements.h"

#include "msrAlignments.h"
#include "msrFonts.h"
#include "msrMusicXMLSpecifics.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrCreditTypeKind {
  kCreditType_NO_,
  kCreditTypeNumber,
  kCreditTypeTitle, kCreditTypeSubtitle,
  kCreditTypeComposer, kCreditTypeArranger, kCreditTypeLyricist,
  kCreditTypeRights,
  kCreditTypePartName,
  kCreditTypeOther
};

std::string msrCreditTypeKindAsString (
  msrCreditTypeKind creditTypeKind);

std::ostream& operator << (std::ostream& os, const msrCreditTypeKind& elt);

//______________________________________________________________________________
class EXP msrCreditWords : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCreditWords> create (
                            int                        inputLineNumber,
                            const std::string&         creditWordsContents,
                            float                      creditWordsDefaultX,
                            float                      creditWordsDefaultY,
                            const std::string&         creditWordsFontFamily,
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
                            const std::string&         creditWordsContents,
                            float                      creditWordsDefaultX,
                            float                      creditWordsDefaultY,
                            const std::string&         creditWordsFontFamily,
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
    std::string           getCreditWordsContents () const
                              { return fCreditWordsContents; }

    float                 getCreditWordsDefaultX () const
                              { return fCreditWordsDefaultX; }

    float                 getCreditWordsDefaultY () const
                              { return fCreditWordsDefaultY; }

    std::string           getCreditWordsFontFamily () const
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

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

/*
    <credit-words default-x="607" default-y="1443" font-family="ＭＳ ゴシック" font-size="24" font-weight="bold" justify="center" valign="top" xml:lang="ja">越後獅子</credit-words>
*/

    std::string           fCreditWordsContents;

    float                 fCreditWordsDefaultX;
    float                 fCreditWordsDefaultY;

    std::string           fCreditWordsFontFamily;
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
EXP std::ostream& operator << (std::ostream& os, const S_msrCreditWords& elt);

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

    void                  setCreditTypeKind (
                            msrCreditTypeKind creditTypeKind)
                              { fCreditTypeKind = creditTypeKind; }

    msrCreditTypeKind     getCreditTypeKind () const
                              { return fCreditTypeKind; }

    const std::vector<S_msrCreditWords>&
                          getCreditWordsList () const
                              { return fCreditWordsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendCreditWordsToCredit (
                            const S_msrCreditWords& creditWords);

    std::string           asString () const override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fCreditPageNumber;

    msrCreditTypeKind     fCreditTypeKind;

    std::vector<S_msrCreditWords>
                          fCreditWordsList;
};
typedef SMARTP<msrCredit> S_msrCredit;
EXP std::ostream& operator << (std::ostream& os, const S_msrCredit& elt);


}


#endif
