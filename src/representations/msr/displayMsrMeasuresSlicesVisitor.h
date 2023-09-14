/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___displayMsrMeasuresSlicesVisitor___
#define ___displayMsrMeasuresSlicesVisitor___

#include "visitor.h"

#include "msrMeasuresSlices.h"

#include "msrOah.h"

#include "oahBasicTypes.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP displayMsrMeasuresSlicesVisitor :

  public visitor<S_msrScore>,

  // parts & part groups

  public visitor<S_msrPartGroup>,

  public visitor<S_msrPart>,

  // staves

  public visitor<S_msrStaff>,

  public visitor<S_msrVoice>,

  // clef, key, time signature

  public visitor<S_msrClef>,
  public visitor<S_msrKey>,
  public visitor<S_msrTimeSignature>,

  public visitor<S_msrSegment>,

  public visitor<S_msrMeasure>,

  public visitor<S_msrNote>,

  public visitor<S_msrChord>,

  public visitor<S_msrTuplet>,

  public visitor<S_msrBarLine>,
  public visitor<S_msrBarNumberCheck>,
  public visitor<S_msrLineBreak>,

  public visitor<S_msrRepeat>,

  public visitor<S_msrScaling>,
  public visitor<S_msrPageLayout>

{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          displayMsrMeasuresSlicesVisitor (
                            const S_msrOahGroup& msrOpts);

    virtual               ~displayMsrMeasuresSlicesVisitor ();

  protected:

    // protected services
    // ------------------------------------------------------

    virtual void          visitStart (S_msrScore& elt);
    virtual void          visitEnd   (S_msrScore& elt);

    virtual void          visitStart (S_msrPartGroup& elt);
    virtual void          visitEnd   (S_msrPartGroup& elt);

    virtual void          visitStart (S_msrPart& elt);
    virtual void          visitEnd   (S_msrPart& elt);

    virtual void          visitStart (S_msrStaff& elt);
    virtual void          visitEnd   (S_msrStaff& elt);

    virtual void          visitStart (S_msrVoice& elt);
    virtual void          visitEnd   (S_msrVoice& elt);

    virtual void          visitStart (S_msrClef& elt);
    virtual void          visitEnd   (S_msrClef& elt);

    virtual void          visitStart (S_msrKey& elt);
    virtual void          visitEnd   (S_msrKey& elt);

    virtual void          visitStart (S_msrTimeSignature& elt);
    virtual void          visitEnd   (S_msrTimeSignature& elt);

    virtual void          visitStart (S_msrSegment& elt);
    virtual void          visitEnd   (S_msrSegment& elt);

    virtual void          visitStart (S_msrMeasure& elt);
    virtual void          visitEnd   (S_msrMeasure& elt);

    virtual void          visitStart (S_msrNote& elt);
    virtual void          visitEnd   (S_msrNote& elt);

    virtual void          visitStart (S_msrChord& elt);
    virtual void          visitEnd   (S_msrChord& elt);

    virtual void          visitStart (S_msrTuplet& elt);
    virtual void          visitEnd   (S_msrTuplet& elt);

    virtual void          visitStart (S_msrBarLine& elt);
    virtual void          visitEnd   (S_msrBarLine& elt);

    virtual void          visitStart (S_msrBarNumberCheck& elt);
    virtual void          visitEnd   (S_msrBarNumberCheck& elt);

    virtual void          visitStart (S_msrLineBreak& elt);
    virtual void          visitEnd   (S_msrLineBreak& elt);

    virtual void          visitStart (S_msrRepeat& elt);
    virtual void          visitEnd   (S_msrRepeat& elt);

  public:

    // print
    // ------------------------------------------------------

    void                  printMsrScoreMeasuresSlices (
                            const S_msrScore& score);

  private:

    // private fields
    // ------------------------------------------------------

    S_msrOahGroup         fMsrOahGroup;

    S_msrMeasuresSlicesSequence
                          fMeasuresSlicesSequence;

    S_msrMeasuresSlice    fCurrentMeasuresSlice;

    // score
    // ------------------------------------------------------

    // part groups
    // ------------------------------------------------------

//     int                   fPartGroupsCounter;

    // parts
    // ------------------------------------------------------

//     int                   fPartsCounter;

    // staves
    // ------------------------------------------------------

//     int                   fStavesCounter;

    // prevent clef, key and time signature from being handled twice
//     Bool                  fOnGoingStaff;

    // voices
    // ------------------------------------------------------

//     int                   fVoicesCounter;

    // stanzas
    // ------------------------------------------------------

//     int                   fStanzasCounter;

    // notes
    // ------------------------------------------------------

//     int                   fScoreUnpitchedNotesCounter;
//     int                   fScoreRegularNotesCounter;
//     int                   fScoreRestNotesCounter;
//     int                   fScoreSkipNotesCounter;
//     int                   fScoreDoubleTremoloNotesCounter;
//     int                   fScoreGraceNotesGroupCounter;

    // chords
    // ------------------------------------------------------

//     int                   fScoreChordNotesCounter;
//     int                   fScoreChordsCounter;

    // tuplets
    // ------------------------------------------------------

//     int                   fScoreTupletNotesCounter;
//     int                   fScoreTupletsCounter;
};


}


#endif // ___displayMsrMeasuresSlicesVisitor___
