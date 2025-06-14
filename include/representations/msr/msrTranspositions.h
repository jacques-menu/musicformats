/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

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
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int           transposeDiatonic,
                            int           transposeChromatic,
                            int           transposeOctaveChange,
                            Bool          transposeDouble);

    static SMARTP<msrTransposition> create (
                            const mfInputLineNumber& inputLineNumber,
                            int  transposeDiatonic,
                            int  transposeChromatic,
                            int  transposeOctaveChange,
                            Bool transposeDouble);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTransposition (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int           transposeDiatonic,
                            int           transposeChromatic,
                            int           transposeOctaveChange,
                            Bool          transposeDouble);

    virtual               ~msrTransposition ();

  public:

    // set and get
    // ------------------------------------------------------

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

    int                   fTranspositionDiatonic;
    int                   fTranspositionChromatic;
    int                   fTranspositionOctaveChange;
    Bool                  fTranspositionDouble;
};
typedef SMARTP<msrTransposition> S_msrTransposition;
EXP std::ostream& operator << (std::ostream& os, const S_msrTransposition& elt);


}


#endif // ___msrTranspositions___
