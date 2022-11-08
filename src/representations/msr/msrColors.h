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


namespace MusicFormats
{

// RGB colors
//______________________________________________________________________________
class EXP msrRGBColor
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRGBColor ();

                          msrRGBColor (
                            float theR,
                            float theG,
                            float theB);

                          msrRGBColor (
                            const string& theString);

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

    string                asString (int precision = 3) const;

    string                asSpaceSeparatedString (int precision = 3) const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    float                 fR;
    float                 fG;
    float                 fB;
};
EXP ostream& operator << (ostream& os, const msrRGBColor& elt);

// AlphaRGB colors
//______________________________________________________________________________
class EXP msrAlphaRGBColor
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrAlphaRGBColor (
                            const string& colorRGB,
                            const string& colorAlpha);

                          msrAlphaRGBColor (
                            const string& colorRGB);

    virtual               ~msrAlphaRGBColor ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getColorRGB () const
                              { return fColorRGB; }

    string                getColorAlpha () const
                              { return fColorAlpha; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    string                fColorRGB;   // hexadecimal, 6 digits
    string                fColorAlpha; // hexadecimal, 2 digits
};
EXP ostream& operator << (ostream& os, const msrAlphaRGBColor& elt);


}

#endif
