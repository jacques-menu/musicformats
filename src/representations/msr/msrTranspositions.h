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

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrTransposition;
typedef SMARTP<msrTransposition> S_msrTransposition;

class EXP msrTransposition : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

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
                            int  inputLineNumber,
                            int  transposeDiatonic,
                            int  transposeChromatic,
                            int  transposeOctaveChange,
                            Bool transposeDouble);

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

    Bool                  isEqualTo (S_msrTransposition otherTranspose) const;

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

    int                   fTranspositionDiatonic;
    int                   fTranspositionChromatic;
    int                   fTranspositionOctaveChange;
    Bool                  fTranspositionDouble;
};
typedef SMARTP<msrTransposition> S_msrTransposition;
EXP ostream& operator<< (ostream& os, const S_msrTransposition& elt);


} // namespace MusicFormats


#endif
