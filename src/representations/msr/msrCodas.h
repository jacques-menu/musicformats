/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrCodas___
#define ___msrCodas___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrCodaKind {
  kCodaFirst,
  kCodaSecond
};

string codaKindAsString (
  msrCodaKind codaKind);

ostream& operator<< (ostream& os,const msrCodaKind& elt);

//______________________________________________________________________________
class EXP msrCoda : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCoda> create (
                            int         inputLineNumber,
                            int         staffNumber,
                            msrCodaKind codaKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrCoda (
                            int         inputLineNumber,
                            int         staffNumber,
                            msrCodaKind codaKind);

    virtual               ~msrCoda ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getStaffNumber () const
                              { return fStaffNumber; }

    msrCodaKind           getCodaKind () const
                              { return fCodaKind; }

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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fStaffNumber;

    msrCodaKind           fCodaKind;
};
typedef SMARTP<msrCoda> S_msrCoda;
EXP ostream& operator<< (ostream& os, const S_msrCoda& elt);


} // namespace MusicFormats


#endif

