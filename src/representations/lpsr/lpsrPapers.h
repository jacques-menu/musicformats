/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrPapers___
#define ___lpsrPapers___

#include <string>
#include <iostream>

#include "msrScaling.h"

#include "lpsrElements.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrPaper : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPaper> create (
                            int             inputLineNumber,
                            S_msrScaling    scaling,
                            S_msrPageLayout pageLayout);

    SMARTP<lpsrPaper> createPaperNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrPaper (
                            int             inputLineNumber,
                            S_msrScaling    scaling,
                            S_msrPageLayout pageLayout);

    virtual               ~lpsrPaper ();

  public:

    // set and get
    // ------------------------------------------------------

    // LPSR scaling
    void                  setScaling (S_msrScaling value)
                              { fScaling = value; }
    S_msrScaling          getScaling () const
                            { return fScaling; }

    // MSR page layout
    void                  setPageLayout (S_msrPageLayout value)
                              { fPageLayout = value; }
    S_msrPageLayout       getPageLayout () const
                            { return fPageLayout; }

    // indents
    void                  setHorizontalShift (S_msrLength value)
                              { fHorizontalShift = value; }
    S_msrLength           getHorizontalShift () const
                              { return fHorizontalShift; }

    void                  setIndent (S_msrLength value)
                              { fIndent = value; }
    S_msrLength           getIndent () const
                              { return fIndent; }

    void                  setShortIndent (S_msrLength value)
                              { fShortIndent = value; }
    S_msrLength           getShortIndent () const
                              { return fShortIndent; }

    // spaces
    void                  setMarkupSystemSpacingPadding (S_msrLength val)
                              { fMarkupSystemSpacingPadding = val; }
    S_msrLength           getMarkupSystemSpacingPadding () const
                              { return fMarkupSystemSpacingPadding; }

    void                  setBetweenSystemSpace (S_msrLength val)
                              { fBetweenSystemSpace = val; }
    S_msrLength           getBetweenSystemSpace () const
                              { return fBetweenSystemSpace; }

    void                  setPageTopSpacing (S_msrLength val)
                              { fPageTopSpacing = val; }
    S_msrLength           getPageTopSpacing () const
                              { return fPageTopSpacing; }

    // counts
    void                  setPageCount (int value)
                              { fPageCount = value; }
    int                   getPageCount () const
                              { return fPageCount; }

    void                  setSystemCount (int value)
                              { fSystemCount = value; }
    int                   getSystemCount () const
                              { return fSystemCount; }

    // ragged output
    void                  setRaggedLast (mfOnOffKind value)
                              { fRaggedLast = value; }

    mfOnOffKind           getRaggedLast () const
                              { return fRaggedLast; }
    S_oahOnOffAtom        getRaggedLastAtom () const
                              { return fRaggedLastAtom; }

    void                  setRaggedBottom (mfOnOffKind value)
                              { fRaggedBottom = value; }

    mfOnOffKind           getRaggedBottom () const
                              { return fRaggedBottom; }

    S_oahOnOffAtom        getRaggedBottomAtom () const
                              { return fRaggedBottomAtom; }

    void                  setRaggedLastBottom (mfOnOffKind value)
                              { fRaggedLastBottom = value; }

    mfOnOffKind           getRaggedLastBottom () const
                              { return fRaggedLastBottom; }
    S_oahOnOffAtom        getRaggedLastBottomAtom () const
                              { return fRaggedLastBottomAtom; }

    void                  setRaggedRight (mfOnOffKind value)
                              { fRaggedRight = value; }

    mfOnOffKind           getRaggedRight () const
                              { return fRaggedRight; }
    S_oahOnOffAtom        getRaggedRightAtom () const
                              { return fRaggedRightAtom; }


    // headers and footers
    void                  setOddHeaderMarkup (string value)
                              { fOddHeaderMarkup = value; }
    string                getOddHeaderMarkup () const
                              { return fOddHeaderMarkup; }

    void                  setEvenHeaderMarkup (string value)
                              { fEvenHeaderMarkup = value; }
    string                getEvenHeaderMarkup () const
                              { return fEvenHeaderMarkup; }

    void                  setOddFooterMarkup (string value)
                              { fOddFooterMarkup = value; }
    string                getOddFooterMarkup () const
                              { return fOddFooterMarkup; }

    void                  setEvenFooterMarkup (string value)
                              { fEvenFooterMarkup = value; }
    string                getEvenFooterMarkup () const
                              { return fEvenFooterMarkup; }

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

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;


  private:

    // private fields
    // ------------------------------------------------------

    // MSR scaling
    S_msrScaling          fScaling;

    // MSR page layout
    S_msrPageLayout       fPageLayout;

    // indents
    S_msrLength           fHorizontalShift;
    S_msrLength           fIndent;
    S_msrLength           fShortIndent;

    // spaces
    S_msrLength           fMarkupSystemSpacingPadding;
    S_msrLength           fBetweenSystemSpace;
    S_msrLength           fPageTopSpacing;

    // counts
    int                   fPageCount;
    int                   fSystemCount;

    // on/off values
    mfOnOffKind           fRaggedBottom;
    S_oahOnOffAtom        fRaggedBottomAtom;

    mfOnOffKind           fRaggedLast;
    S_oahOnOffAtom        fRaggedLastAtom;

    mfOnOffKind           fRaggedLastBottom;
    S_oahOnOffAtom        fRaggedLastBottomAtom;

    mfOnOffKind           fRaggedRight;
    S_oahOnOffAtom        fRaggedRightAtom;

    // headers and footers
    string                fOddHeaderMarkup;
    string                fEvenHeaderMarkup;
    string                fOddFooterMarkup;
    string                fEvenFooterMarkup;
};
typedef SMARTP<lpsrPaper> S_lpsrPaper;
EXP ostream& operator << (ostream& os, const S_lpsrPaper& elt);


}


#endif
