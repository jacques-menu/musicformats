/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrOctaveShifts___
#define ___msrOctaveShifts___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrOctaveShiftKind {
  kOctaveShiftNone,
  kOctaveShiftUp, kOctaveShiftDown,
  kOctaveShiftStop, kOctaveShiftContinue
};

string msrOctaveShiftKindAsString (
  msrOctaveShiftKind octaveShiftKind);

ostream& operator << (ostream& os, const msrOctaveShiftKind& elt);

class EXP msrOctaveShift : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOctaveShift> create (
                            int                inputLineNumber,
                            S_msrMeasure       upLinkToMeasure,
                            msrOctaveShiftKind octaveShiftKind,
                            int                octaveShiftSize);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrOctaveShift (
                            int                inputLineNumber,
                            S_msrMeasure       upLinkToMeasure,
                            msrOctaveShiftKind octaveShiftKind,
                            int                octaveShiftSize);

    virtual               ~msrOctaveShift ();

  public:

    // set and get
    // ------------------------------------------------------

    msrOctaveShiftKind    getOctaveShiftKind () const
                              { return fOctaveShiftKind; }

    int                   getOctaveShiftSize () const
                              { return fOctaveShiftSize; }

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

    msrOctaveShiftKind    fOctaveShiftKind;

    int                   fOctaveShiftSize;
};
typedef SMARTP<msrOctaveShift> S_msrOctaveShift;
EXP ostream& operator << (ostream& os, const S_msrOctaveShift& elt);


} // namespace MusicFormats


#endif
