/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSegnos___
#define ___msrSegnos___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrDalSegnoKind {
  kDalSegnoNone,
  kDalSegno, kDalSegnoAlFine, kDalSegnoAlCoda
};

std::string msrDalSegnoKindAsString (
  msrDalSegnoKind dalSegnoKind);

std::ostream& operator << (std::ostream& os,const msrDalSegnoKind& elt);

//______________________________________________________________________________
class EXP msrSegno : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegno> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const mfStaffNumber& staffNumber);

    static SMARTP<msrSegno> create (
                            const mfInputLineNumber& inputLineNumber,
                            const mfStaffNumber& staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSegno (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const mfStaffNumber& staffNumber);

    virtual               ~msrSegno ();

  public:

    // set and get
    // ------------------------------------------------------

    mfStaffNumber         getStaffNumber () const
                              { return fStaffNumber; }

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
};

using S_msrSegno = SMARTP<msrSegno>;

EXP std::ostream& operator << (std::ostream& os, const S_msrSegno& elt);

//______________________________________________________________________________
class EXP msrDalSegno : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDalSegno> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            msrDalSegnoKind     dalSegnoKind,
                            const std::string&  dalSegnoString,
                            const mfStaffNumber& staffNumber);

    static SMARTP<msrDalSegno> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrDalSegnoKind     dalSegnoKind,
                            const std::string&  dalSegnoString,
                            const mfStaffNumber& staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDalSegno (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            msrDalSegnoKind     dalSegnoKind,
                            const std::string&  dalSegnoString,
                            const mfStaffNumber& staffNumber);

    virtual               ~msrDalSegno ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDalSegnoKind       getDalSegnoKind () const
                              { return fDalSegnoKind; }

    std::string           getDalSegnoString () const
                              { return fDalSegnoString; }

    mfStaffNumber         getStaffNumber () const
                              { return fStaffNumber; }

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

    msrDalSegnoKind       fDalSegnoKind;

    std::string           fDalSegnoString;

    mfStaffNumber         fStaffNumber;
};

using S_msrDalSegno = SMARTP<msrDalSegno>;

EXP std::ostream& operator << (std::ostream& os, const S_msrDalSegno& elt);


}


#endif // ___msrSegnos___
