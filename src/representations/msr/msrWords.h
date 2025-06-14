/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrWords___
#define ___msrWords___

#include "mfIndentedTextOutput.h"

#include "msrAlignments.h"
#include "msrElements.h"
#include "msrFonts.h"
#include "msrMusicXMLBasicTypes.h"
#include "msrPlacements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrWordsFontStyleKind {
  kNormalStyle, KItalicStyle
};

std::string msrWordsFontStyleKindAsString (
  msrWordsFontStyleKind wordsFontStyleKind);

std::ostream& operator << (std::ostream& os, const msrWordsFontStyleKind& elt);

enum class msrWordsFontWeightKind {
  kNormalWeight, kBoldWeight
};

std::string msrWordsFontWeightKindAsString (
  msrWordsFontWeightKind wordsFontWeightKind);

std::ostream& operator << (std::ostream& os, const msrWordsFontWeightKind& elt);

//______________________________________________________________________________
class EXP msrWords : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrWords> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrPlacementKind           wordsPlacementKind,
                            const std::string&         wordsContents,
                            msrJustifyKind             wordsJustifyKind,
                            msrHorizontalAlignmentKind wordsHorizontalAlignmentKind,
                            msrVerticalAlignmentKind   wordsVerticalAlignmentKind,
                            msrFontStyleKind           wordsFontStyleKind,
                            const S_msrFontSize&       wordsFontSize,
                            msrFontWeightKind          wordsFontWeightKind,
                            msrXMLLangKind             wordsXMLLangKind,
                            const mfStaffNumber&       wordsStaffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrWords (
                            const mfInputLineNumber& inputLineNumber,
                            msrPlacementKind           wordsPlacementKind,
                            const std::string&         wordsContents,
                            msrJustifyKind             wordsJustifyKind,
                            msrHorizontalAlignmentKind wordsHorizontalAlignmentKind,
                            msrVerticalAlignmentKind   wordsVerticalAlignmentKind,
                            msrFontStyleKind           wordsFontStyleKind,
                            const S_msrFontSize&       wordsFontSize,
                            msrFontWeightKind          wordsFontWeightKind,
                            msrXMLLangKind             wordsXMLLangKind,
                            const mfStaffNumber&       wordsStaffNumber);

    virtual               ~msrWords ();

  public:

    // set and get
    // ------------------------------------------------------

    msrPlacementKind      getWordsPlacementKind () const
                              { return fWordsPlacementKind; }

    std::string           getWordsContents () const
                              { return fWordsContents; }

    msrJustifyKind        getWordsJustifyKind () const
                              { return fWordsJustifyKind; }

    msrHorizontalAlignmentKind
                          getWordsHorizontalAlignmentKind () const
                              { return fWordsHorizontalAlignmentKind; }

    msrVerticalAlignmentKind
                          getWordsVerticalAlignmentKind () const
                              { return fWordsVerticalAlignmentKind; }

    msrFontStyleKind      getWordsFontStyleKind () const
                              { return fWordsFontStyleKind; }

    S_msrFontSize         getWordsFontSize () const
                              { return fWordsFontSize; }

    msrFontWeightKind     getWordsFontWeightKind () const
                              { return fWordsFontWeightKind; }

    msrXMLLangKind        getWordsXMLLangKind () const
                              { return fWordsXMLLangKind; }

    void                  setWordsStaffNumber (const mfStaffNumber& wordsStaffNumber)
                              { fWordsStaffNumber = wordsStaffNumber; }

    mfStaffNumber         getWordsStaffNumber () const
                              { return fWordsStaffNumber; }

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

    std::string           wordsPlacementKindAsString () const;

    std::string           wordsJustifyKindAsString () const;

    std::string           wordsHorizontalAlignmentKindAsString () const;
    std::string           wordsVerticalAlignmentKindAsString () const;

    std::string           msrWordsFontStyleKindAsString () const;

    std::string           wordsFontSizeAsString () const;

    std::string           wordsFontWeightKindAsString () const;

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrPlacementKind      fWordsPlacementKind;

    std::string           fWordsContents;

    msrJustifyKind        fWordsJustifyKind;

    msrHorizontalAlignmentKind
                          fWordsHorizontalAlignmentKind;
    msrVerticalAlignmentKind
                          fWordsVerticalAlignmentKind;

    msrFontStyleKind      fWordsFontStyleKind;
    S_msrFontSize         fWordsFontSize;
    msrFontWeightKind     fWordsFontWeightKind;

    msrXMLLangKind        fWordsXMLLangKind;

    mfStaffNumber         fWordsStaffNumber;
};

using S_msrWords = SMARTP<msrWords>;

EXP std::ostream& operator << (std::ostream& os, const S_msrWords& elt);
EXP mfIndentedStringStream& operator << (mfIndentedStringStream& iss, const S_msrWords& elt);


}


#endif // ___msrWords___
