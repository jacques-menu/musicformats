/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTranspositions___
#define ___msrTranspositions___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declaration

class   msrTransposition;
typedef SMARTP<msrTransposition> S_msrTransposition;

//______________________________________________________________________________
class EXP msrTransposition : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTransposition> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int           transposeDiatonic,
                            int           transposeChromatic,
                            int           transposeOctaveChange,
                            Bool          transposeDouble);

    static SMARTP<msrTransposition> create (
                            int  inputLineNumber,
                            int  transposeDiatonic,
                            int  transposeChromatic,
                            int  transposeOctaveChange,
                            Bool transposeDouble);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTransposition (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int           transposeDiatonic,
                            int           transposeChromatic,
                            int           transposeOctaveChange,
                            Bool          transposeDouble);

    virtual               ~msrTransposition ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setTranspositionUpLinkToMeasure (measure); }

    S_msrMeasure          getMeasureElementUpLinkToMeasure () const override
                            { return getTranspositionUpLinkToMeasure (); }

    void                  setTranspositionUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getTranspositionUpLinkToMeasure () const
                            { return fTranspositionUpLinkToMeasure; }

    int                   getTranspositionDiatonic () const
                              { return fTranspositionDiatonic; }

    int                   getTranspositionChromatic () const
                              { return fTranspositionChromatic; }

    int                   getTranspositionOctaveChange () const
                              { return fTranspositionOctaveChange; }

    Bool                  getTranspositionDouble () const
                              { return fTranspositionDouble; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (
                            const S_msrTransposition& otherTransposition) const;

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

    S_msrMeasure          fTranspositionUpLinkToMeasure;

    int                   fTranspositionDiatonic;
    int                   fTranspositionChromatic;
    int                   fTranspositionOctaveChange;
    Bool                  fTranspositionDouble;
};
typedef SMARTP<msrTransposition> S_msrTransposition;
EXP std::ostream& operator << (std::ostream& os, const S_msrTransposition& elt);


}


#endif
