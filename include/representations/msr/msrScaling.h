/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrScaling___
#define ___msrScaling___

#include "msrElements.h"

// #include "msrBasicTypes.h"

// #include "msrMusicXMLSpecifics.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrScaling : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrScaling> create (
                            int   inputLineNumber,
                            float millimeters,
                            float tenths);

    SMARTP<msrScaling> createMsrScalingNewbornClone ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrScaling (
                            int   inputLineNumber,
                            float millimeters,
                            float tenths);

    virtual               ~msrScaling ();

  public:

    // set and get
    // ------------------------------------------------------

    // relative to absolute lengths conversion
    float                 getMillimeters () const
                              { return fMillimeters; }

    float                 getTenths () const
                              { return float(fTenths); }

  public:

    // public services
    // ------------------------------------------------------

    float                 fetchGlobalStaffSize () const;

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

    // relative to absolute lengths conversion
    float                 fMillimeters;
    float                 fTenths;
};
typedef SMARTP<msrScaling> S_msrScaling;
EXP ostream& operator << (ostream& os, const S_msrScaling& elt);


} // namespace MusicFormats


#endif
