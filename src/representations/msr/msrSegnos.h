/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSegnos___
#define ___msrSegnos___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrSegno : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegno> create (
                            int  inputLineNumber,
                            int  staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSegno (
                            int inputLineNumber,
                            int staffNumber);

    virtual               ~msrSegno ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getStaffNumber () const
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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fStaffNumber;
};
typedef SMARTP<msrSegno> S_msrSegno;
EXP ostream& operator<< (ostream& os, const S_msrSegno& elt);

//______________________________________________________________________________
class EXP msrDalSegno : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrDalSegnoKind {
      kDalSegnoNone,
      kDalSegno, kDalSegnoAlFine, kDalSegnoAlCoda
    };

    static string dalSegnoKindAsString (
      msrDalSegnoKind dalSegnoKind);

    static msrDalSegnoKind msrDalSegnoKindFromString (
      const string& theString);

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDalSegno> create (
                            int             inputLineNumber,
                            msrDalSegnoKind dalSegnoKind,
                            const string&   dalSegnoString,
                            int             staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDalSegno (
                            int             inputLineNumber,
                            msrDalSegnoKind dalSegnoKind,
                            const string&   dalSegnoString,
                            int             staffNumber);

    virtual               ~msrDalSegno ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDalSegnoKind       getDalSegnoKind () const
                              { return fDalSegnoKind; }

    string                getDalSegnoString () const
                              { return fDalSegnoString; }

    int                   getStaffNumber () const
                              { return fStaffNumber; }

    void                  setDalSegnoPositionInMeasure (
                            const rational& positionInMeasure);

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


      msrDalSegnoKind     fDalSegnoKind;

      string              fDalSegnoString;

      int                 fStaffNumber;
};
typedef SMARTP<msrDalSegno> S_msrDalSegno;
EXP ostream& operator<< (ostream& os, const S_msrDalSegno& elt);


}


#endif
