/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsr2bsrFinalizer___
#define ___bsr2bsrFinalizer___

#include "visitor.h"

#include "msrTypesForwardDeclarations.h"
#include "bsrTypesForwardDeclarations.h"

#include "bsrScores.h"

#include "bsrOah.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP bsr2bsrFinalizer :

  // BSR score

  public visitor<S_bsrScore>,

  public visitor<S_bsrSpaces>,

  public visitor<S_bsrBarLine>,

  public visitor<S_bsrTranscriptionNotes>,
  public visitor<S_bsrTranscriptionNotesElement>,

  public visitor<S_bsrPage>,

  public visitor<S_bsrPageHeading>,
  public visitor<S_bsrMusicHeading>,

  public visitor<S_bsrFootNotes>,

  public visitor<S_bsrLine>,
  public visitor<S_bsrLineContents>,

  public visitor<S_bsrMeasure>,

  public visitor<S_bsrNumber>,
  public visitor<S_bsrWords>,

  public visitor<S_bsrClef>,
  public visitor<S_bsrKey>,
  public visitor<S_bsrTimeSignature>,

  public visitor<S_bsrTempo>,

  public visitor<S_bsrNote>

{
  public:

                          bsr2bsrFinalizer (
                            const S_bsrOahGroup& bsrOpts,
                            const S_bsrScore&     originalBsrScore);

    virtual               ~bsr2bsrFinalizer ();

    void                  translateBsrToFinalizedBsr ();

    S_bsrScore            getBsr () const
                              { return fBsrScore; }

  protected:

    virtual void          visitStart (S_bsrScore& elt);
    virtual void          visitEnd   (S_bsrScore& elt);

    virtual void          visitStart (S_bsrSpaces& elt);
    virtual void          visitEnd   (S_bsrSpaces& elt);

    virtual void          visitStart (S_bsrBarLine& elt);

    virtual void          visitStart (S_bsrTranscriptionNotes& elt);
    virtual void          visitEnd   (S_bsrTranscriptionNotes& elt);

    virtual void          visitStart (S_bsrTranscriptionNotesElement& elt);
    virtual void          visitEnd   (S_bsrTranscriptionNotesElement& elt);

    virtual void          visitStart (S_bsrPage& elt);
    virtual void          visitEnd   (S_bsrPage& elt);

    virtual void          visitStart (S_bsrPageHeading& elt);
    virtual void          visitEnd   (S_bsrPageHeading& elt);
    virtual void          visitStart (S_bsrMusicHeading& elt);
    virtual void          visitEnd   (S_bsrMusicHeading& elt);

    virtual void          visitStart (S_bsrFootNotes& elt);
    virtual void          visitEnd   (S_bsrFootNotes& elt);

    virtual void          visitStart (S_bsrLine& elt);
    virtual void          visitEnd   (S_bsrLine& elt);
    virtual void          visitStart (S_bsrLineContents& elt);
    virtual void          visitEnd   (S_bsrLineContents& elt);

    virtual void          visitStart (S_bsrMeasure& elt);
    virtual void          visitEnd   (S_bsrMeasure& elt);

    virtual void          visitStart (S_bsrNumber& elt);
    virtual void          visitEnd   (S_bsrNumber& elt);

    virtual void          visitStart (S_bsrWords& elt);
    virtual void          visitEnd   (S_bsrWords& elt);

    virtual void          visitStart (S_bsrClef& elt);
    virtual void          visitEnd   (S_bsrClef& elt);

    virtual void          visitStart (S_bsrKey& elt);
    virtual void          visitEnd   (S_bsrKey& elt);

    virtual void          visitStart (S_bsrTimeSignature& elt);
    virtual void          visitEnd   (S_bsrTimeSignature& elt);

    virtual void          visitStart (S_bsrTempo& elt);
    virtual void          visitEnd   (S_bsrTempo& elt);

    virtual void          visitStart (S_bsrNote& elt);
    virtual void          visitEnd   (S_bsrNote& elt);

  private:

    // the MSR score we're relying on
    // ------------------------------------------------------
    S_msrScore            fBaseMsrScore;

    // the BSR score we're visiting
    // ------------------------------------------------------
    S_bsrScore            fVisitedBsrScore;

    // the BSR score we're building
    // ------------------------------------------------------
    S_bsrScore            fBsrScore;

    // pages & lines
    // ------------------------------------------------------

    S_bsrPage             fCurrentPage;
    S_bsrLine             fCurrentLine;

    // headings
    // ------------------------------------------------------

    S_bsrPageHeading      fCurrentPageHeading;
    S_bsrMusicHeading     fCurrentMusicHeading;

    // foot notes
    // ------------------------------------------------------

    S_bsrFootNotes        fCurrentFootNotes;

    // measures
    // ------------------------------------------------------

    S_bsrMeasure          fCurrentMeasure;

    // options
    // ------------------------------------------------------

    S_bsrOahGroup         fBsrOahGroup;
};


}


#endif // ___bsr2bsrFinalizer___
