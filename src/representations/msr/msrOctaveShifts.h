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

std::string msrOctaveShiftKindAsString (
  msrOctaveShiftKind octaveShiftKind);

std::ostream& operator << (std::ostream& os, const msrOctaveShiftKind& elt);

//______________________________________________________________________________
class EXP msrOctaveShift : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOctaveShift> create (
                            int                inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            msrOctaveShiftKind octaveShiftKind,
                            int                octaveShiftSize);

    static SMARTP<msrOctaveShift> create (
                            int                inputLineNumber,
                            msrOctaveShiftKind octaveShiftKind,
                            int                octaveShiftSize);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrOctaveShift (
                            int                inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            msrOctaveShiftKind octaveShiftKind,
                            int                octaveShiftSize);

    virtual               ~msrOctaveShift ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setOctaveShiftUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getOctaveShiftUpLinkToMeasure (); }

    void                  setOctaveShiftUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getOctaveShiftUpLinkToMeasure () const
                            { return fOctaveShiftUpLinkToMeasure; }

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrMeasure          fOctaveShiftUpLinkToMeasure;

    msrOctaveShiftKind    fOctaveShiftKind;

    int                   fOctaveShiftSize;
};
typedef SMARTP<msrOctaveShift> S_msrOctaveShift;
EXP std::ostream& operator << (std::ostream& os, const S_msrOctaveShift& elt);


}


#endif
