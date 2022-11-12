/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrColors___
#define ___msrColors___

#include <string>
#include <ostream>

#include "exports.h"


namespace MusicFormats
{

// RGB colors
//______________________________________________________________________________
class EXP msrColorRGB
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrColorRGB ();

                          msrColorRGB (
                            float theR,
                            float theG,
                            float theB);

                          msrColorRGB (
                            const std::string& theString);

    // set and get
    // ------------------------------------------------------

    float                 getR () const
                              { return fR; }

    float                 getG () const
                              { return fG; }

    float                 getB () const
                              { return fB; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString (int precision = 3) const;

    std::string           asSpaceSeparatedString (int precision = 3) const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    float                 fR;
    float                 fG;
    float                 fB;
};
EXP std::ostream& operator << (std::ostream& os, const msrColorRGB& elt);

// AlphaRGB colors
//______________________________________________________________________________
class EXP msrColorAlphaRGB
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrColorAlphaRGB (
                            const std::string& colorRGB,
                            const std::string& colorAlpha);

                          msrColorAlphaRGB (
                            const std::string& colorRGB);

    virtual               ~msrColorAlphaRGB ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getColorRGB () const
                              { return fColorRGB; }

    std::string           getColorAlpha () const
                              { return fColorAlpha; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fColorRGB;   // hexadecimal, 6 digits
    std::string           fColorAlpha; // hexadecimal, 2 digits
};
EXP std::ostream& operator << (std::ostream& os, const msrColorAlphaRGB& elt);


}

#endif
