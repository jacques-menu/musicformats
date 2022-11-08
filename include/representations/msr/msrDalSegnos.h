/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrDalSegnos___
#define ___msrDalSegnos___

#include "msrMeasureElements.h"

#include "msrMeasures.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrDalSegnoKind {
  kDalSegnoNone,
  kDalSegno, kDalSegnoAlFine, kDalSegnoAlCoda
};

string msrDalSegnoKindAsString (
  msrDalSegnoKind dalSegnoKind);

ostream& operator << (ostream& os,const msrDalSegnoKind& elt);

//______________________________________________________________________________
class EXP msrDalSegno : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDalSegno> create (
                            int             inputLineNumber,
                            S_msrMeasure    upLinkToMeasure,
                            msrDalSegnoKind dalSegnoKind,
                            const string&   dalSegnoString,
                            int             staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDalSegno (
                            int             inputLineNumber,
                            S_msrMeasure    upLinkToMeasure,
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

    // position in measure
    void                  setMeasureElementMeasurePosition (
                            const S_msrMeasure measure,
                            const Rational&    measurePosition,
                            const string&      context) override
                              {
                                setDalSegnoMeasurePosition (
                                  measure,
                                  measurePosition,
                                  context);
                              }

    void                  setDalSegnoMeasurePosition (
                            const S_msrMeasure measure,
                            const Rational&    measurePosition,
                            const string&      context);

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
EXP ostream& operator << (ostream& os, const S_msrDalSegno& elt);


}


#endif
