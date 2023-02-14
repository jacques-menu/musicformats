/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrFonts___
#define ___msrFonts___

#include <string>
#include <ostream>

#include "exports.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrFontSizeKind {
  kFontSizeNone,
  kFontSizeXXSmall, kFontSizeXSmall, kFontSizeSmall,
  kFontSizeMedium,
  kFontSizeLarge, kFontSizeXLarge, kFontSizeXXLarge,
  kFontSizeNumeric
};

std::string msrFontSizeKindAsString (
  msrFontSizeKind fontSizeKind);

enum class msrFontStyleKind {
  kFontStyleNone,
  kFontStyleNormal, KFontStyleItalic
};

std::string msrFontStyleKindAsString (
  msrFontStyleKind fontStyleKind);

std::ostream& operator << (std::ostream& os, const msrFontStyleKind& elt);

msrFontStyleKind msrFontStyleKindFromString (
  int                inputLineNumber,
  const std::string& fontStyleString);

enum class msrFontWeightKind {
  kFontWeightNone,
  kFontWeightNormal, kFontWeightBold
};

std::string msrFontWeightKindAsString (
  msrFontWeightKind fontWeightKind);

std::ostream& operator << (std::ostream& os, const msrFontWeightKind& elt);

msrFontWeightKind msrFontWeightKindFromString (
  int                inputLineNumber,
  const std::string& fontWeightString);

std::ostream& operator << (std::ostream& os, const msrFontSizeKind& elt);

//______________________________________________________________________________
class EXP msrFontSize : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFontSize> create (
                            msrFontSizeKind fontSizeKind);

    static SMARTP<msrFontSize> create (
                            float fontNumericSize);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFontSize (
                            msrFontSizeKind fontSizeKind);

                          msrFontSize (
                            float numericFontSize);

    virtual               ~msrFontSize ();

  public:

    // set and get
    // ------------------------------------------------------

    msrFontSizeKind       getFontSizeKind () const
                              { return fFontSizeKind; }

    float                 getFontNumericSize () const;

  public:

    // public services
    // ------------------------------------------------------

    std::string           fontSizeAsString () const;

    std::string           asString () const;

  public:

    // ------------------------------------------------------

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrFontSizeKind       fFontSizeKind;

    float                 fFontNumericSize;
                            // only relevant when
                            // fFontSizeKind == kNumericFontSize
};
typedef SMARTP<msrFontSize> S_msrFontSize;
EXP std::ostream& operator << (std::ostream& os, const S_msrFontSize& elt);


}

#endif // ___msrFonts___
