/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrClefs___
#define ___msrClefs___

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class   msrClef;
typedef SMARTP<msrClef> S_msrClef;

class EXP msrClef : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrClef> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure,
                            msrClefKind  clefKind,
                            int          clefStaffNumber);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrClef> createClefFromString (
                            int           inputLineNumber,
                            S_msrMeasure  upLinkToMeasure,
                            const string& clefString,
                            int           clefLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrClef (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure,
                            msrClefKind  clefKind,
                            int          clefStaffNumber);

    virtual               ~msrClef ();

  public:

    // set and get
    // ------------------------------------------------------

    msrClefKind           getClefKind () const
                              { return fClefKind; }

    int                   getClefStaffNumber () const
                              { return fClefStaffNumber; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (S_msrClef otherClef) const;

    Bool                  clefIsATablatureClef () const;

    Bool                  clefIsAPercussionClef () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;
    string                asShortStringForMeasuresSlices () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrClefKind           fClefKind;
    int                   fClefStaffNumber; // 0 by default in MSR,
                                            // 1 by default in MusicXML
};
typedef SMARTP<msrClef> S_msrClef;
EXP ostream& operator << (ostream& os, const S_msrClef& elt);


} // namespace MusicFormats


#endif
