/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTransposes___
#define ___msrTransposes___

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrTranspose;
typedef SMARTP<msrTranspose> S_msrTranspose;

class EXP msrTranspose : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTranspose> create (
                            int  inputLineNumber,
                            int  transposeDiatonic,
                            int  transposeChromatic,
                            int  transposeOctaveChange,
                            Bool transposeDouble);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTranspose (
                            int  inputLineNumber,
                            int  transposeDiatonic,
                            int  transposeChromatic,
                            int  transposeOctaveChange,
                            Bool transposeDouble);

    virtual               ~msrTranspose ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getTransposeDiatonic () const
                              { return fTransposeDiatonic; }

    int                   getTransposeChromatic () const
                              { return fTransposeChromatic; }

    int                   getTransposeOctaveChange () const
                              { return fTransposeOctaveChange; }

    Bool                  getTransposeDouble () const
                              { return fTransposeDouble; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (S_msrTranspose otherTranspose) const;

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

    int                   fTransposeDiatonic;
    int                   fTransposeChromatic;
    int                   fTransposeOctaveChange;
    Bool                  fTransposeDouble;
};
typedef SMARTP<msrTranspose> S_msrTranspose;
EXP ostream& operator<< (ostream& os, const S_msrTranspose& elt);


} // namespace MusicFormats


#endif
