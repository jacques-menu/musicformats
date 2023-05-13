/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2bsrTranslator___
#define ___msr2bsrTranslator___

#include <map>
#include <vector>

// libmusicxml2
#include "visitor.h"

#include "bsrTypesForwardDeclarations.h"

#include "bsrNotes.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP msr2bsrTranslator :

  // MSR score

  public visitor<S_msrScore>,

  // parts & part groups

  public visitor<S_msrPartGroup>,
  public visitor<S_msrPart>,

  // staves & voices

  public visitor<S_msrStaffTuning>,
  public visitor<S_msrStaff>,
  public visitor<S_msrVoice>,
  public visitor<S_msrVoiceStaffChange>,

  // bar lines

  public visitor<S_msrBarLine>,

  // measures

  public visitor<S_msrMeasure>,

  // clef, key, time signature

  public visitor<S_msrClef>,
  public visitor<S_msrKey>,
  public visitor<S_msrTimeSignature>,

  // tempos

  public visitor<S_msrTempo>,

  // notes

  public visitor<S_msrNote>,

  // dynamics

  public visitor<S_msrDynamic>,

  // pages & lines

  public visitor<S_msrLineBreak>,
  public visitor<S_msrPageBreak>

{
  public:

                          msr2bsrTranslator ();

    virtual               ~msr2bsrTranslator ();

    S_bsrScore            translateMsrToBsr (
                            const S_msrScore& theMsrScore);

  protected:

    // pages & lines

    virtual void          visitStart (S_msrPageBreak& elt);
    virtual void          visitEnd   (S_msrPageBreak& elt);

    virtual void          visitStart (S_msrLineBreak& elt);
    virtual void          visitEnd   (S_msrLineBreak& elt);

    // score

    virtual void          visitStart (S_msrScore& elt);
    virtual void          visitEnd   (S_msrScore& elt);

    // parts & part groups

    virtual void          visitStart (S_msrPartGroup& elt);
    virtual void          visitEnd   (S_msrPartGroup& elt);

    virtual void          visitStart (S_msrPart& elt);
    virtual void          visitEnd   (S_msrPart& elt);

    // staves & voices

    virtual void          visitStart (S_msrStaffTuning& elt);
    virtual void          visitStart (S_msrStaffDetails& elt);
    virtual void          visitEnd   (S_msrStaffDetails& elt);

    virtual void          visitStart (S_msrStaff& elt);
    virtual void          visitEnd   (S_msrStaff& elt);

    virtual void          visitStart (S_msrVoice& elt);
    virtual void          visitEnd   (S_msrVoice& elt);

    virtual void          visitStart (S_msrVoiceStaffChange& elt);

    // bar lines

    virtual void          visitStart (S_msrBarLine& elt);

    // measures

    virtual void          visitStart (S_msrMeasure& elt);
    virtual void          visitEnd   (S_msrMeasure& elt);

    // clef, key, time signature

    virtual void          visitStart (S_msrClef& elt);
    virtual void          visitEnd   (S_msrClef& elt);

    virtual void          visitStart (S_msrKey& elt);
    virtual void          visitEnd   (S_msrKey& elt);

    virtual void          visitStart (S_msrTimeSignature& elt);
    virtual void          visitEnd   (S_msrTimeSignature& elt);

    // tempos

    virtual void          visitStart (S_msrTempo& elt);
    virtual void          visitEnd   (S_msrTempo& elt);

    // notes

    virtual void          visitStart (S_msrNote& elt);
    virtual void          visitEnd   (S_msrNote& elt);

    // dynamics

    virtual void          visitStart (S_msrDynamic& elt);
    virtual void          visitEnd   (S_msrDynamic& elt);

  private:

    // messages
    // ------------------------------------------------------

    void                      notSupportedMessage (
                                int           inputLineNumber,
                                const std::string& message);

    // the MSR score we're visiting
    // ------------------------------------------------------
    S_msrScore                fVisitedMsrScore;


    // the BSR score we're building
    // ------------------------------------------------------
    S_bsrScore                fResultingBsr;


    // transcription notes
    // ------------------------------------------------------

    S_bsrTranscriptionNotes   fCurrentTranscriptionsNotes;


    // parts & part groups
    // ------------------------------------------------------
    S_msrPartGroup            fCurrentPartGroup;
    S_msrPart                 fCurrentPart;


    // staff details
    // ------------------------------------------------------

    S_msrStaffTuning          fCurrentStaffTuning;


    // staves
    // ------------------------------------------------------
    S_msrStaff                fCurrentStaff;


    // voices
    // ------------------------------------------------------
    S_msrVoice                fCurrentVoice;
 //   std::map<S_msrNote, S_msrNote> fVoiceNotesMap; // JMI


    // pages & lines
    // ------------------------------------------------------
    S_bsrPage                 fCurrentPage;
    int                       fCurrentPrintPageNumber;

    S_bsrLine                 fCurrentLine;
    int                       fCurrentPrintLineNumber;


    // headings
    // ------------------------------------------------------

    S_bsrPageHeading          fFirstPageHeading;
    S_bsrMusicHeading         fFirstMusicHeading;


    // foot notes
    // ------------------------------------------------------

// JMI    S_bsrFootNotes            fFirstFootNotes;


    // measures
    // ------------------------------------------------------

    std::string               fCurrentMeasureNumber;
    S_bsrMeasure              fCurrentMeasure;


    // clef, key, time signature
    // ------------------------------------------------------

    S_bsrKey                  fFirstKey;
    S_bsrTimeSignature        fFirstTimeSignature;

    // notes
    // ------------------------------------------------------

    bsrNoteOctaveKind         fCurrentNoteOctaveKind;

    bsrNoteValueSizeKind
                              fCurrentNoteValueSizeKind;

    void                      createBsrForNote (const S_msrNote& note);

    // relative octaves
    // ------------------------------------------------------
    S_msrNote                 fRelativeOctaveReference;
                                // contains absolute octave

    bsrNoteOctaveIsNeeded     brailleOctaveMarkIfNeeded (const S_msrNote& note);


/*
    // it's header
    // ------------------------------------------------------
    S_bsrHeader               fBsrScoreHeader;


    // score
    // ------------------------------------------------------
    S_msrScore                fCurrentMsrScoreClone;


    // identification
    // ------------------------------------------------------
    Bool                      fOnGoingIdentification;
    S_msrIdentification       fCurrentIdentification;


    // header
    // ------------------------------------------------------
    Bool                      fWorkNumberKnown;
    Bool                      fWorkCreditTypeTitleKnown;
    Bool                      fMovementNumberKnown;
    Bool                      fMovementTitleKnown;


    // paper
    // ------------------------------------------------------
    void                      setPaperIndentsIfNeeded (
                                const S_msrScaling& scaling);

    // credits
    // ------------------------------------------------------
    S_msrCredit               fCurrentCredit;


    // part groups
    // ------------------------------------------------------
  //  S_msrPartGroup            fCurrentPartGroupClone; JMI

    // the current partGroup is the top of the stack
    std::list<S_msrPartGroup>
                              fPartGroupsStack;


     // harmonies
    // ------------------------------------------------------
    Bool                      fOnGoingHarmoniesVoice;

    S_msrHarmony              fCurrentHarmonyClone;

    std::list<S_msrHarmony>
                              fPendingHarmoniesList;


    // frames
    // ------------------------------------------------------
 //   Bool                      fOnGoingFramesVoice; JMI

 //   std::list<S_msrFrame>          fPendingFramesList; // JMI


    // figured bass
    // ------------------------------------------------------
    Bool                      fOnGoingFiguredBassVoice;
    S_msrFiguredBass          fCurrentFiguredBass;


    // repeats
    // ------------------------------------------------------

// JMI    S_msrRepeatCommonPart     fCurrentRepeatCommonPartClone;
    S_msrRepeatEnding         fCurrentRepeatEndingClone;


    // measure repeats
    // ------------------------------------------------------

    S_msrMeasureRepeatPattern
                              fCurrentMeasureRepeatPatternClone;
    S_msrMeasureRepeatReplicas
                              fCurrentMeasureRepeatReplicasClone;

    // multiple full-bar rests
    // ------------------------------------------------------

    S_msrMultipleRest         fCurrentMultipleRestClone; // JMI
    S_msrMultipleRestContents fCurrentMultipleRestContentsClone;


    // segments
    // ------------------------------------------------------
    // segments can be imbedded in others,
    // the current segment clone is the one at the top of the stack
    std::list<S_msrSegment>       fCurrentSegmentClonesStack;


    // measures
    // ------------------------------------------------------
    // we need to count the measures for option fSeparatorLineEveryNMeasures,
    // since measure numbers are actually strings
    int                       fMeasuresCounter;

    S_msrMeasure              fCurrentMeasureClone;

    void                      finalizeCurrentMeasureClone (
                                int          inputLineNumber,
                                const S_msrMeasure& originalMeasure);

    // bar checks
    // ------------------------------------------------------
    S_msrBarCheck             fLastBarCheck;


    // notes
    // ------------------------------------------------------
    Bool                      fOnGoingNote;

    // fCurrentNonGraceNoteClone is not used for grace notes,
    // which are visited while the note they're attached to
    // is being visited too
    S_msrNote                 fCurrentNonGraceNoteClone;

    // to help workaround LilyPond issue #34
    S_msrNote                 fFirstNoteCloneInVoice;

    S_msrGraceNotesGroup      fCurrentSkipGraceNotesGroup;


    // glissandos
    // ------------------------------------------------------


    // slides
    // ------------------------------------------------------


    // double tremolos
    // ------------------------------------------------------
    S_msrDoubleTremolo        fCurrentDoubleTremoloClone;
    Bool                      fOnGoingDoubleTremolo;


    // stems
    // ------------------------------------------------------
    S_msrStem                 fCurrentStem;


    // grace notes
    // ------------------------------------------------------
    S_msrGraceNotesGroup      fCurrentGraceNotesGroupClone;
    S_msrNote                 fCurrentGraceNoteClone;
    Bool                      fOnGoingGraceNotesGroup;

    // afterGraceNotes optimisation
    S_msrAfterGraceNotesGroup fPendingAfterGraceNotesGroup;
    S_msrElement              fCurrentAfterGraceNotesGroupElement;


    // chords
    // ------------------------------------------------------
    Bool                      fOnGoingChord;
    S_msrChord                fCurrentChordClone;


    // tuplets
    // ------------------------------------------------------
//    S_msrTuplet             fCurrentTupletClone;
 //   Bool                      fOnGoingTuplet;
    std::list<S_msrTuplet>   fTupletClonesStack;


    // stanzas
    // ------------------------------------------------------
    S_msrStanza               fCurrentStanzaClone;
    Bool                      fOnGoingStanza;


    // syllables
    // ------------------------------------------------------
    S_msrSyllable             fCurrentSyllableClone;
    Bool                      fOnGoingSyllableExtend;


    // part groups block
    // the current partGroup block is the top of the stack
    std::list<S_bsrPartGroupBlock>
                              fPartGroupBlocksStack;
                              */

  private:

    // private work methods

    void                      createFirstPageHeadingIfNotYetCreated (
                                int inputLineNumber);
    void                      createFirstMusicHeadingIfNotYetCreated (
                                int inputLineNumber);

    // current ongoing values display
    // ------------------------------------------------------
    void                      displayCurrentOnGoingValues ();
};


}


#endif // ___msr2bsrTranslator___

    /*

    // rights

    public visitor<S_msrIdentification>,

    public visitor<S_msrCredit>,
    public visitor<S_msrCreditWords>,

    // scaling

    public visitor<S_msrScaling>,

    // layout

    public visitor<S_msrLayout>,

  // parts & part groups

  public visitor<S_msrPartGroup>,

  public visitor<S_msrPart>,

  // staff details

  public visitor<S_msrStaffTuning>,
  public visitor<S_msrStaffDetails>,

  public visitor<S_msrStaff>,

  public visitor<S_msrVoice>,

  public visitor<S_msrVoiceStaffChange>,

  // lyrics

  public visitor<S_msrStanza>,
  public visitor<S_msrSyllable>,

  // clefs

  public visitor<S_msrClef>,

  // keys

  public visitor<S_msrKey>,

  // time signatures

  public visitor<S_msrTimeSignature>,

  // transposition

  public visitor<S_msrTransposition>,

  // words

  public visitor<S_msrWords>,

  // tempo

  public visitor<S_msrTempo>,

  public visitor<S_msrSegment>,

  // rehearsal mark

  public visitor<S_msrRehearsalMark>,

  // harmonies

  public visitor<S_msrHarmony>,
  public visitor<S_msrHarmonyDegree>,

  // frames

  public visitor<S_msrFrame>,

  // figured bass

  public visitor<S_msrFiguredBass>,
  public visitor<S_msrBassFigure>,

  // measures

  public visitor<S_msrMeasure>,

  // articulations

  public visitor<S_msrArticulation>,

  public visitor<S_msrFermata>,

  public visitor<S_msrArpeggiato>,
  public visitor<S_msrNonArpeggiato>,

  // technicals

  public visitor<S_msrTechnical>,
  public visitor<S_msrTechnicalWithInteger>,
  public visitor<S_msrTechnicalWithFloat>,
  public visitor<S_msrTechnicalWithString>,

  // ornaments

  public visitor<S_msrOrnament>,

  // spanners

  public visitor<S_msrSpanner>,

  // glissandos

  public visitor<S_msrGlissando>,

  // slides

  public visitor<S_msrSlide>,

  // tremolos

  public visitor<S_msrSingleTremolo>,

  public visitor<S_msrDoubleTremolo>,

  // dynamics

  public visitor<S_msrDynamic>,

  public visitor<S_msrOtherDynamic>,

  public visitor<S_msrSlash>,

  public visitor<S_msrSlur>,
  public visitor<S_msrLigature>,

  public visitor<S_msrCrescDecresc>,

  public visitor<S_msrWedge>,

  // grace notes

  public visitor<S_msrGraceNotesGroup>,

  // notes

  public visitor<S_msrNote>,
  public visitor<S_msrOctaveShift>,

  // accordion registration

  public visitor<S_msrAccordionRegistration>,

  // harp pedals tuning

  public visitor<S_msrHarpPedalsTuning>,

  public visitor<S_msrStem>,

  // chords

  public visitor<S_msrChord>,

  // tuplets

  public visitor<S_msrTuplet>,

  // ties, slurs, brackets & beams

  public visitor<S_msrTie>,

  public visitor<S_msrBeam>,

  // ?

  public visitor<S_msrBarLine>,

  public visitor<S_msrSegno>,
  public visitor<S_msrCoda>,

  public visitor<S_msrEyeGlasses>,
  public visitor<S_msrPedal>,
  public visitor<S_msrDamp>,
  public visitor<S_msrDampAll>,
  public visitor<S_msrScordatura>,

  public visitor<S_msrBarCheck>,
  public visitor<S_msrBarNumberCheck>,


  // repeats

  public visitor<S_msrRepeat>,
  public visitor<S_msrRepeatCommonPart>,
  public visitor<S_msrRepeatEnding>,

  public visitor<S_msrMeasureRepeat>,
  public visitor<S_msrMeasureRepeatPattern>,
  public visitor<S_msrMeasureRepeatReplicas>,

  public visitor<S_msrMultipleRest>,
  public visitor<S_msrMultipleRestContents>,
    */


/*
    virtual void          visitStart (S_msrIdentification& elt);
    virtual void          visitEnd   (S_msrIdentification& elt);

    virtual void          visitStart (S_msrCredit& elt);
    virtual void          visitEnd   (S_msrCredit& elt);
    virtual void          visitStart (S_msrCreditWords& elt);
    virtual void          visitEnd   (S_msrCreditWords& elt);

    virtual void          visitStart (S_msrStanza& elt);
    virtual void          visitEnd   (S_msrStanza& elt);

    virtual void          visitStart (S_msrSyllable& elt);
    virtual void          visitEnd   (S_msrSyllable& elt);
*/


/*
    virtual void          visitStart (S_msrTransposition& elt);
    virtual void          visitEnd   (S_msrTransposition& elt);

    virtual void          visitStart (S_msrWords& elt);
    virtual void          visitEnd   (S_msrWords& elt);

    virtual void          visitStart (S_msrTempo& elt);
    virtual void          visitEnd   (S_msrTempo& elt);

    virtual void          visitStart (S_msrRehearsalMark& elt);
    virtual void          visitEnd   (S_msrRehearsalMark& elt);

    virtual void          visitStart (S_msrSegment& elt);
    virtual void          visitEnd   (S_msrSegment& elt);

    virtual void          visitStart (S_msrHarmony& elt);
    virtual void          visitEnd   (S_msrHarmony& elt);
    virtual void          visitStart (S_msrHarmonyDegree& elt);

    virtual void          visitStart (S_msrFrame& elt);

    virtual void          visitStart (S_msrFiguredBass& elt);
    virtual void          visitEnd   (S_msrFiguredBass& elt);
    virtual void          visitStart (S_msrBassFigure& elt);

    virtual void          visitStart (S_msrArticulation& elt);
    virtual void          visitEnd   (S_msrArticulation& elt);

    virtual void          visitStart (S_msrFermata& elt);

    virtual void          visitStart (S_msrArpeggiato& elt);
    virtual void          visitStart (S_msrNonArpeggiato& elt);

    virtual void          visitStart (S_msrTechnical& elt);
    virtual void          visitEnd   (S_msrTechnical& elt);

    virtual void          visitStart (S_msrTechnicalWithInteger& elt);
    virtual void          visitEnd   (S_msrTechnicalWithInteger& elt);

    virtual void          visitStart (S_msrTechnicalWithFloat& elt);
    virtual void          visitEnd   (S_msrTechnicalWithFloat& elt);

    virtual void          visitStart (S_msrTechnicalWithString& elt);
    virtual void          visitEnd   (S_msrTechnicalWithString& elt);

    virtual void          visitStart (S_msrOrnament& elt);
    virtual void          visitEnd   (S_msrOrnament& elt);

    virtual void          visitStart (S_msrSpanner& elt);
    virtual void          visitEnd   (S_msrSpanner& elt);

    virtual void          visitStart (S_msrGlissando& elt);
    virtual void          visitEnd   (S_msrGlissando& elt);

    virtual void          visitStart (S_msrSlide& elt);
    virtual void          visitEnd   (S_msrSlide& elt);

    virtual void          visitStart (S_msrSingleTremolo& elt);
    virtual void          visitEnd   (S_msrSingleTremolo& elt);

    virtual void          visitStart (S_msrDoubleTremolo& elt);
    virtual void          visitEnd   (S_msrDoubleTremolo& elt);

    virtual void          visitStart (S_msrDynamic& elt);
    virtual void          visitEnd   (S_msrDynamic& elt);

    virtual void          visitStart (S_msrOtherDynamic& elt);
    virtual void          visitEnd   (S_msrOtherDynamic& elt);

    virtual void          visitStart (S_msrSlash& elt);

    virtual void          visitStart (S_msrCrescDecresc& elt);
    virtual void          visitEnd   (S_msrCrescDecresc& elt);

    virtual void          visitStart (S_msrWedge& elt);
    virtual void          visitEnd   (S_msrWedge& elt);

    virtual void          visitStart (S_msrGraceNotesGroup& elt);
    virtual void          visitEnd   (S_msrGraceNotesGroup& elt);

    virtual void          visitStart (S_msrNote& elt);
    virtual void          visitEnd   (S_msrNote& elt);

    virtual void          visitStart (S_msrOctaveShift& elt);
    virtual void          visitEnd   (S_msrOctaveShift& elt);

    virtual void          visitStart (S_msrAccordionRegistration& elt);

    virtual void          visitStart (S_msrHarpPedalsTuning& elt);

    virtual void          visitStart (S_msrStem& elt);
    virtual void          visitEnd   (S_msrStem& elt);

    virtual void          visitStart (S_msrBeam& elt);
    virtual void          visitEnd   (S_msrBeam& elt);

    virtual void          visitStart (S_msrChord& elt);
    virtual void          visitEnd   (S_msrChord& elt);

    virtual void          visitStart (S_msrTuplet& elt);
    virtual void          visitEnd   (S_msrTuplet& elt);

    virtual void          visitStart (S_msrTie& elt);
    virtual void          visitEnd   (S_msrTie& elt);

    virtual void          visitStart (S_msrSlur& elt);
    virtual void          visitEnd   (S_msrSlur& elt);

    virtual void          visitStart (S_msrLigature& elt);
    virtual void          visitEnd   (S_msrLigature& elt);

    virtual void          visitStart (S_msrBarLine& elt);
    virtual void          visitEnd   (S_msrBarLine& elt);

    virtual void          visitStart (S_msrSegno& elt);
    virtual void          visitStart (S_msrCoda& elt);

    virtual void          visitStart (S_msrEyeGlasses& elt);
    virtual void          visitStart (S_msrPedal& elt);
    virtual void          visitStart (S_msrDamp& elt);
    virtual void          visitStart (S_msrDampAll& elt);
    virtual void          visitStart (S_msrScordatura& elt);

    virtual void          visitStart (S_msrBarCheck& elt);
    virtual void          visitEnd   (S_msrBarCheck& elt);
    virtual void          visitStart (S_msrBarNumberCheck& elt);
    virtual void          visitEnd   (S_msrBarNumberCheck& elt);

    virtual void          visitStart (S_msrLineBreak& elt);
    virtual void          visitEnd   (S_msrLineBreak& elt);

    virtual void          visitStart (S_msrRepeat& elt);
    virtual void          visitEnd   (S_msrRepeat& elt);
    virtual void          visitStart (S_msrRepeatCommonPart& elt);
    virtual void          visitEnd   (S_msrRepeatCommonPart& elt);
    virtual void          visitStart (S_msrRepeatEnding& elt);
    virtual void          visitEnd   (S_msrRepeatEnding& elt);

    virtual void          visitStart (S_msrMeasureRepeat& elt);
    virtual void          visitEnd   (S_msrMeasureRepeat& elt);
    virtual void          visitStart (S_msrMeasureRepeatPattern& elt);
    virtual void          visitEnd   (S_msrMeasureRepeatPattern& elt);
    virtual void          visitStart (S_msrMeasureRepeatReplicas& elt);
    virtual void          visitEnd   (S_msrMeasureRepeatReplicas& elt);

    virtual void          visitStart (S_msrMultipleRest& elt);
    virtual void          visitEnd   (S_msrMultipleRest& elt);
    virtual void          visitStart (S_msrMultipleRestContents& elt);
    virtual void          visitEnd   (S_msrMultipleRestContents& elt);

    virtual void          visitStart (S_msrScaling& elt);
    virtual void          visitEnd   (S_msrScaling& elt);

    virtual void          visitStart (S_msrLayout& elt);
    virtual void          visitEnd   (S_msrLayout& elt);

    virtual void          visitStart (S_msrMidi& elt);
    virtual void          visitEnd   (S_msrMidi& elt);
    */
