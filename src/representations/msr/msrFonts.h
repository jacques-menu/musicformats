/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrFonts___
#define ___msrFonts___

#include <string>
#include <ostream>

#include "exports.h"


using namespace std;

namespace MusicFormats
{

// fonts
//______________________________________________________________________________
// data types
// ------------------------------------------------------

enum class msrFontSizeKind {
  kFontSizeNone,
  kFontSizeXXSmall, kFontSizeXSmall, kFontSizeSmall,
  kFontSizeMedium,
  kFontSizeLarge, kFontSizeXLarge, kFontSizeXXLarge,
  kFontSizeNumeric
};

string msrFontSizeKindAsString (
  msrFontSizeKind fontSizeKind);

ostream& operator << (ostream& os, const msrFontSizeKind& elt);

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

    string                fontSizeAsString () const;

  public:

    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrFontSizeKind       fFontSizeKind;

    float                 fFontNumericSize;
                            // only relevant when
                            // fFontSizeKind == kNumericFontSize
};
typedef SMARTP<msrFontSize> S_msrFontSize;
EXP ostream& operator << (ostream& os, const S_msrFontSize& elt);

enum class msrFontStyleKind {
  kFontStyleNone,
  kFontStyleNormal, KFontStyleItalic
};

string msrFontStyleKindAsString (
  msrFontStyleKind fontStyleKind);

ostream& operator << (ostream& os, const msrFontStyleKind& elt);

msrFontStyleKind msrFontStyleKindFromString (
  int           inputLineNumber,
  const string& fontStyleString);

enum class msrFontWeightKind {
  kFontWeightNone,
  kFontWeightNormal, kFontWeightBold
};

string msrFontWeightKindAsString (
  msrFontWeightKind fontWeightKind);

ostream& operator << (ostream& os, const msrFontWeightKind& elt);

msrFontWeightKind msrFontWeightKindFromString (
  int           inputLineNumber,
  const string& fontWeightString);


}

#endif