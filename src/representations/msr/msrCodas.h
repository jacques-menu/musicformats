/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrCodas___
#define ___msrCodas___

#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrCodaKind {
  kCodaFirst,
  kCodaSecond
};

std::string msrCodaKindAsString (
  msrCodaKind codaKind);

std::ostream& operator << (std::ostream& os,const msrCodaKind& elt);

//______________________________________________________________________________
class EXP msrCoda : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCoda> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const mfStaffNumber& staffNumber,
                            msrCodaKind         codaKind);

    static SMARTP<msrCoda> create (
                            const mfInputLineNumber& inputLineNumber,
                            const mfStaffNumber& staffNumber,
                            msrCodaKind         codaKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrCoda (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const mfStaffNumber& staffNumber,
                            msrCodaKind         codaKind);

    virtual               ~msrCoda ();

  public:

    // set and get
    // ------------------------------------------------------

    mfStaffNumber         getStaffNumber () const
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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    mfStaffNumber         fStaffNumber;

    msrCodaKind           fCodaKind;
};
typedef SMARTP<msrCoda> S_msrCoda;
EXP std::ostream& operator << (std::ostream& os, const S_msrCoda& elt);


}


#endif // _ms

