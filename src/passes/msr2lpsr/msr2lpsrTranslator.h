/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2lpsrTranslator___
#define ___msr2lpsrTranslator___

#include <map>
#include <vector>

#include "msrAfterGraceNotes.h"
#include "msrBarChecks.h"
#include "msrChords.h"
#include "msrCredits.h"
#include "msrDoubleTremolos.h"
#include "msrElements.h"
#include "msrFiguredBasses.h"
#include "msrGraceNotesGroups.h"
#include "msrHarmonies.h"
#include "msrIdentification.h"
#include "msrLyrics.h"
#include "msrMultipleMeasureRests.h"
#include "msrNotes.h"
#include "msrParts.h"
#include "msrPartGroups.h"
#include "msrSegments.h"
#include "msrScores.h"
#include "msrStaves.h"
#include "msrStavesDetails.h"
#include "msrStems.h"
#include "msrVoices.h"

#include "msr2msrTranslator.h"

#include "lpsrBookBlockElements.h"
#include "lpsrHeaders.h"
#include "lpsrPartBlocks.h"
#include "lpsrPartGroupBlocks.h"
#include "lpsrStaves.h"
#include "lpsrScores.h"
#include "lpsrStaves.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP msr2lpsrTranslator : public msr2msrTranslator

  // most MSR types are aleading inherited by msr2msrTranslator

//
//   // lyrics
//
//   public                      visitor<S_msrStanza>,
//   public                      visitor<S_msrSyllable>,
//
//   // clef, key and time signature handling
//
//   // clefs
//
//   public                      visitor<S_msrClef>,
//
//   // keys
//
//   public                      visitor<S_msrKey>,
//
//   // time signatures
//
//   public                      visitor<S_msrTimeSignature>,
//
//   // transposition
//
//   public                      visitor<S_msrTransposition>,
//
//   // words
//
//   public                      visitor<S_msrWords>,
//
//   // tempo
//
//   public                      visitor<S_msrTempo>,
//
//   // rehearsal mark
//
//   public                      visitor<S_msrRehearsalMark>,
//
//   // harmonies
////
//   // figured bass
//
//   public                      visitor<S_msrBassFigure>,
//
//   // measures
//
//   public                      visitor<S_msrMeasure>,
//
//   // articulations
//
//   public                      visitor<S_msrArticulation>,
//
//   public                      visitor<S_msrFermata>,
//
//   public                      visitor<S_msrArpeggiato>,
//   public                      visitor<S_msrNonArpeggiato>,
//
//   // technicals
//
//   public                      visitor<S_msrTechnical>,
//   public                      visitor<S_msrTechnicalWithInteger>,
//   public                      visitor<S_msrTechnicalWithFloat>,
//   public                      visitor<S_msrTechnicalWithString>,
//
//   // ornaments
//
//   public                      visitor<S_msrOrnament>,
//
//   // spanners
//
//   public                      visitor<S_msrSpanner>,
//
//   // glissandos
//
//   public                      visitor<S_msrGlissando>,
//
//   // slides
//
//   public                      visitor<S_msrSlide>,
//
//   // tremolos
//
//   public                      visitor<S_msrSingleTremolo>,
//
//   public                      visitor<S_msrDoubleTremolo>,
//
//   // dynamics
//
//   public                      visitor<S_msrDynamic>,
//
//   public                      visitor<S_msrOtherDynamic>,
//
//   // slashes
//
//   public                      visitor<S_msrSlash>,
//
//   // wedges
//
//   public                      visitor<S_msrCrescDecresc>,
//
//   public                      visitor<S_msrWedge>,
//
//   // slurs
//
//   public                      visitor<S_msrSlur>,
//   public                      visitor<S_msrChordSlurLink>,
//
//   // ligatures
//
//   public                      visitor<S_msrLigature>,
//
//   // grace note groups
//
//   public                      visitor<S_msrGraceNotesGroup>,
//   public                      visitor<S_msrChordGraceNotesGroupLink>,
//
//   // notes
//
//   public                      visitor<S_msrNote>,
//   public                      visitor<S_msrOctaveShift>,
//
//   // accordion registration
//
//   public                      visitor<S_msrAccordionRegistration>,
//
//   // harp pedals tuning
//
//   public                      visitor<S_msrHarpPedalsTuning>,
//
//   public                      visitor<S_msrStem>,
//
//   // chords
//
//   public                      visitor<S_msrChord>,
//
//   // tuplets
//
//   public                      visitor<S_msrTuplet>,
//
//   // ties, slurs, brackets & beams
//
//   public                      visitor<S_msrTie>,
//
//   public                      visitor<S_msrBeam>,
//   public                      visitor<S_msrChordBeamLink>,
//
//   // bar lines
//
//   public                      visitor<S_msrBarLine>,
//
//   // staff-level elements
//
//   public                      visitor<S_msrSegno>,
//   public                      visitor<S_msrDalSegno>,
//   public                      visitor<S_msrCoda>,
//
//   public                      visitor<S_msrEyeGlasses>,
//   public                      visitor<S_msrPedal>,
//   public                      visitor<S_msrDamp>,
//   public                      visitor<S_msrDampAll>,
//   public                      visitor<S_msrScordatura>,
//
//   // bars
//
//   public                      visitor<S_msrBarCheck>,
//   public                      visitor<S_msrBarNumberCheck>,
//
//   // breaks
//
//   public                      visitor<S_msrLineBreak>,
//
//   public                      visitor<S_msrPageBreak>,
//
//   // repeats
//
//   public                      visitor<S_msrRepeat>,
//   public                      visitor<S_msrRepeatCommonPart>,
//   public                      visitor<S_msrRepeatEnding>,
//
//   public                      visitor<S_msrMeasureRepeat>,
//   public                      visitor<S_msrMeasureRepeatPattern>,
//   public                      visitor<S_msrMeasureRepeatReplicas>,
//
//   public                      visitor<S_msrMultipleMeasureRest>,
//
//   // midi
//
//   public                      visitor<S_msrMidiTempo>

{
  public:

                              msr2lpsrTranslator ();

    virtual                   ~msr2lpsrTranslator ();

    S_lpsrScore               translateMsrToLpsr (
                                const S_msrScore&          theMsrScore,
                                const S_mfcMultiComponent& multiComponent);

  protected:

  // most MSR types are aleading inherited by msr2msrTranslator


   virtual void              visitStart (S_msrScore& elt) override;
   virtual void              visitEnd   (S_msrScore& elt) override;

   virtual void              visitStart (S_msrIdentification& elt) override;
   virtual void              visitEnd   (S_msrIdentification& elt) override;

   virtual void              visitStart (S_msrCredit& elt) override;
   virtual void              visitEnd   (S_msrCredit& elt) override;
   virtual void              visitStart (S_msrCreditWords& elt) override;
   virtual void              visitEnd   (S_msrCreditWords& elt) override;
//
//     // parts & part groups

   virtual void              visitStart (S_msrPartGroup& elt) override;
   virtual void              visitEnd   (S_msrPartGroup& elt) override;

   virtual void              visitStart (S_msrPart& elt) override;
   virtual void              visitEnd   (S_msrPart& elt) override;

   virtual void              visitStart (S_msrStaff& elt) override;
   virtual void              visitEnd   (S_msrStaff& elt) override;

   virtual void              visitStart (S_msrVoice& elt) override;

   virtual void              visitStart (S_msrStanza& elt) override;

   virtual void              visitStart (S_msrSyllable& elt) override;

//    virtual void              visitStart (S_msrClefKeyTimeSignatureGroup& elt) override;
//    virtual void              visitEnd   (S_msrClefKeyTimeSignatureGroup& elt) override;

//    virtual void              visitStart (S_msrClef& elt) override;
//    virtual void              visitEnd   (S_msrClef& elt) override;

//    virtual void              visitStart (S_msrKey& elt) override;
//    virtual void              visitEnd   (S_msrKey& elt) override;

//    virtual void              visitStart (S_msrTimeSignature& elt) override;
//    virtual void              visitEnd   (S_msrTimeSignature& elt) override;

//    virtual void              visitStart (S_msrTransposition& elt) override;
//    virtual void              visitEnd   (S_msrTransposition& elt) override;

   virtual void              visitStart (S_msrWords& elt) override;

   virtual void              visitStart (S_msrTempo& elt) override;

//    virtual void              visitStart (S_msrRehearsalMark& elt) override;
//    virtual void              visitEnd   (S_msrRehearsalMark& elt) override;

//    virtual void              visitStart (S_msrBassFigure& elt) override;

   virtual void              visitStart (S_msrMeasure& elt) override;
   virtual void              visitEnd   (S_msrMeasure& elt) override;

   virtual void              visitStart (S_msrArticulation& elt) override;

   virtual void              visitStart (S_msrTechnical& elt) override;

   virtual void              visitStart (S_msrTechnicalWithString& elt) override;

   virtual void              visitStart (S_msrOrnament& elt) override;

   virtual void              visitStart (S_msrSpanner& elt) override;

   virtual void              visitStart (S_msrGlissando& elt) override;

    // dynamics
   virtual void              visitStart (S_msrDynamic& elt) override;

   virtual void              visitStart (S_msrOtherDynamic& elt) override;

    // grace notes groups
   virtual void              visitStart (S_msrGraceNotesGroup& elt) override;
//    virtual void              visitStart (S_msrChordGraceNotesGroupLink& elt) override;
//    virtual void              visitEnd   (S_msrChordGraceNotesGroupLink& elt) override;

    // notes
//    virtual void              visitStart (S_msrNote& elt) override;
//    virtual void              visitEnd   (S_msrNote& elt) override;

    // octave shifts
//    virtual void              visitStart (S_msrOctaveShift& elt) override;
//    virtual void              visitEnd   (S_msrOctaveShift& elt) override;

    // instruments
//    virtual void              visitStart (S_msrAccordionRegistration& elt) override;

//    virtual void              visitStart (S_msrHarpPedalsTuning& elt) override;

    // stems
//    virtual void              visitStart (S_msrStem& elt) override;
//    virtual void              visitEnd   (S_msrStem& elt) override;

    // beams
//    virtual void              visitStart (S_msrBeam& elt) override;
//    virtual void              visitEnd   (S_msrBeam& elt) override;
//    virtual void              visitStart (S_msrChordBeamLink& elt) override;
//    virtual void              visitEnd   (S_msrChordBeamLink& elt) override;

    // chords
//    virtual void              visitStart (S_msrChord& elt) override;
//    virtual void              visitEnd   (S_msrChord& elt) override;

    // tuplets
//    virtual void              visitStart (S_msrTuplet& elt) override;
//    virtual void              visitEnd   (S_msrTuplet& elt) override;

    // ties
//    virtual void              visitStart (S_msrTie& elt) override;
//    virtual void              visitEnd   (S_msrTie& elt) override;

    // bar lines
//    virtual void              visitStart (S_msrBarLine& elt) override;
//    virtual void              visitEnd   (S_msrBarLine& elt) override;

//    virtual void              visitStart (S_msrSegno& elt) override;
//    virtual void              visitStart (S_msrDalSegno& elt) override;
//    virtual void              visitStart (S_msrCoda& elt) override;

//    virtual void              visitStart (S_msrEyeGlasses& elt) override;
//    virtual void              visitStart (S_msrPedal& elt) override;
//    virtual void              visitStart (S_msrDamp& elt) override;
//    virtual void              visitStart (S_msrDampAll& elt) override;
//    virtual void              visitStart (S_msrScordatura& elt) override;

    // bar checks
//    virtual void              visitStart (S_msrBarCheck& elt) override;
//    virtual void              visitEnd   (S_msrBarCheck& elt) override;

    // bar number checks
//    virtual void              visitStart (S_msrBarNumberCheck& elt) override;
//    virtual void              visitEnd   (S_msrBarNumberCheck& elt) override;

    // breaks
//    virtual void              visitStart (S_msrLineBreak& elt) override;
//    virtual void              visitEnd   (S_msrLineBreak& elt) override;

//    virtual void              visitStart (S_msrPageBreak& elt) override;
//    virtual void              visitEnd   (S_msrPageBreak& elt) override;

    // repeats
//    virtual void              visitStart (S_msrRepeat& elt) override;
//    virtual void              visitEnd   (S_msrRepeat& elt) override;
//    virtual void              visitStart (S_msrRepeatCommonPart& elt) override;
//    virtual void              visitEnd   (S_msrRepeatCommonPart& elt) override;
//    virtual void              visitStart (S_msrRepeatEnding& elt) override;
//    virtual void              visitEnd   (S_msrRepeatEnding& elt) override;

    // measure repeats
//    virtual void              visitStart (S_msrMeasureRepeat& elt) override;
//    virtual void              visitEnd   (S_msrMeasureRepeat& elt) override;
//    virtual void              visitStart (S_msrMeasureRepeatPattern& elt) override;
//    virtual void              visitEnd   (S_msrMeasureRepeatPattern& elt) override;
//    virtual void              visitStart (S_msrMeasureRepeatReplicas& elt) override;
//    virtual void              visitEnd   (S_msrMeasureRepeatReplicas& elt) override;

    // multiple measure rests
//    virtual void              visitStart (S_msrMultipleMeasureRest& elt) override;
//    virtual void              visitEnd   (S_msrMultipleMeasureRest& elt) override;

    // scaling
   virtual void              visitStart (S_msrScaling& elt) override;
   virtual void              visitEnd   (S_msrScaling& elt) override;

    // layout

//    virtual void              visitEnd   (S_msrStaffLayout& elt) override;

    // appearance

    // midi
//    virtual void              visitStart (S_msrMidiTempo& elt) override;
//    virtual void              visitEnd   (S_msrMidiTempo& elt) override;

  private:

//     // the MSR score we're visiting
//     // ------------------------------------------------------
//     S_msrScore                fVisitedMsrScore;


    // the LPSR score we're building (it is always built)
    // ------------------------------------------------------
    S_lpsrScore               fResultingLpsr;

    S_lpsrScoreBlock          fCurrentScoreBlock;


    // its header
    // ------------------------------------------------------
    S_lpsrHeader              fCurrentLpsrScoreHeader;


    // score
    // ------------------------------------------------------
    S_msrScore                fCurrentMsrScoreClone;


    // identification
    // ------------------------------------------------------
//     Bool                      fOnGoingIdentification;
//     S_msrIdentification       fCurrentIdentification;

    void                      computeLilypondScoreHeaderFields ();

    void                      computeLilypondScoreHeaderTitleAndSubTitle ();

    // header
    // ------------------------------------------------------
    Bool                      fWorkNumberKnown;
    Bool                      fWorkCreditTypeTitleKnown;
    Bool                      fOpusKnown;
    Bool                      fMovementNumberKnown;
    Bool                      fMovementTitleKnown;

    void                      populateHeaderFromIdentification (
                                const S_lpsrHeader&        header,
                                const S_msrIdentification& identification);

    // paper
    // ------------------------------------------------------
    void                      setPaperIndentsIfNeeded (
                                const S_msrScaling& scaling);

    // credits
    // ------------------------------------------------------
//     S_msrCredit               fCurrentCredit;


    // books
    // ------------------------------------------------------
    S_lpsrBookBlock           fCurrentLpsrBookBlock;


    // part groups
    // ------------------------------------------------------
  //  S_msrPartGroup          fCurrentPartGroupClone; JMI

    // partGroup's can be nested, hence this stack
    // the current partGroup is the top of the stack
    std::list <S_msrPartGroup>
                              fPartGroupsStack;

    void                      displayPartGroupsStack (
                                const std::string& context);

    // part groups block are nested as the partGroup's are
    // the current partGroup block is the top of the stack
    std::list <S_lpsrPartGroupBlock>
                              fPartGroupBlocksStack;

    S_lpsrBookPartBlock       fCurrentBookPartBlock;


    // parts
    // ------------------------------------------------------
//     S_msrPart                 fCurrentPartClone;
    S_lpsrPartBlock           fCurrentPartBlock;


    // staff details
    // ------------------------------------------------------

    // staves
    // ------------------------------------------------------
//     S_msrStaff                fCurrentStaffClone;
    S_lpsrStaffBlock          fCurrentStaffBlock;
    // prevent clef, key and time signature from being handled twice
//     Bool                      fOnGoingStaff;


    // voices
    // ------------------------------------------------------
//     S_msrVoice                fCurrentVoiceClone;
//     S_msrVoice                fCurrentVoiceOriginal;

//     std::map <S_msrNote, S_msrNote>
//                               fVoiceNotesMap; // JMI

    // clef, key, time signature
    // ------------------------------------------------------

//     S_msrClefKeyTimeSignatureGroup
//                               fCurrentClefKeyTimeSignatureGroup;

    // dal segnos
    // ------------------------------------------------------

//     std::list <S_msrHiddenMeasureAndBarLineDescr>
//                               fPartHiddenMeasureAndBarLineDescrList;
////
//     void                      handlePartHiddenMeasureAndBarLineDescrList ();


    // harmonies
    // ------------------------------------------------------
//     Bool                      fOnGoingHarmoniesVoice;
//
//     Bool                      fOnGoingHarmony;
//     S_msrHarmony              fCurrentHarmonyClone;
//
//     std::list <S_msrHarmony>
//                               fPendingHarmoniesList;


    // figured bass
    // ------------------------------------------------------
//     Bool                      fOnGoingFiguredBassVoice;
//
//     Bool                      fOnGoingFiguredBass;


    // segments
    // ------------------------------------------------------
    // the current segment clone is the one at the top of the stack
//     S_msrSegment              fCurrentSegmentClone;


    // measures
    // ------------------------------------------------------
    // multiple measure rests compression
//     S_msrMeasure              fCurrentRestMeasure;

//     S_msrMultipleMeasureRest fCurrentMultipleMeasureRestsClone;
//     Bool                      fOnGoingMultipleMeasureRests;

    // bar checks
    // ------------------------------------------------------
//     S_msrBarCheck             fLastBarCheck;


    // line breaks
    // ------------------------------------------------------
//     S_msrLineBreak            fLastLineBreak;


    // notes
    // ------------------------------------------------------

//     S_msrNote                 fCurrentNoteClone; // JMI 0.9.72

    // browsing grace notes groups leads to several notes
    // being ongoing simultaneously,
    // since such groups are attached to a note, hence:
//     std::list <S_msrNote>     fOnGoingNotesStack;

//     Bool                      fOnGoingNonGraceNote;

    // fCurrentNonGraceNoteClone is not used for grace notes,
    // which are visited while the note they're attached to
    // is being visited too
//     S_msrNote                 fCurrentNonGraceNoteClone;

    // to help workaround LilyPond_Issue_34
//     S_msrNote                 fFirstNoteCloneInVoice;

//     S_msrGraceNotesGroup      fCurrentSkipGraceNotesGroup;


    // beams
    // ------------------------------------------------------
//     Bool                      fOnGoingChordBeamLink;

    // glissandos
    // ------------------------------------------------------


    // slides
    // ------------------------------------------------------


    // double tremolos
    // ------------------------------------------------------
//     S_msrDoubleTremolo        fCurrentDoubleTremoloClone;
//     Bool                      fOnGoingDoubleTremolo;


    // stems
    // ------------------------------------------------------
//     S_msrStem                 fCurrentStem;


    // grace notes
    // ------------------------------------------------------
//     S_msrGraceNotesGroup      fCurrentGraceNotesGroupClone;
//
//     S_msrNote                 fCurrentGraceNoteClone;
//     Bool                      fOnGoingGraceNotesGroup;
//
//     Bool                      fOnGoingChordGraceNotesGroupLink;
//
//     // afterGraceNotes optimisation
//     S_msrAfterGraceNotesGroup fPendingAfterGraceNotesGroup;
//     S_msrElement              fCurrentAfterGraceNotesGroupElement;

    // circumvent LilyPond #34 issue if relevant
    void                      circumventLilyPond34IssueIfRelevant (
                                S_msrGraceNotesGroup& graceNotesGroup);

    /* JMI
    void                      prependSkipGraceNotesGroupToPartOtherVoices (
                                const S_msrPart&            partClone,
                                const S_msrVoice&           voiceClone,
                                const S_msrGraceNotesGroup& skipGraceNotesGroup);
                                */

    // chords
    // ------------------------------------------------------
//     Bool                      fOnGoingChord;
//     S_msrChord                fCurrentChordClone;

//     Bool                      fCurrentChordHasBeenPopulatedFromItsFirstNote;

    // tuplets
    // ------------------------------------------------------
//     std::list <S_msrTuplet>   fTupletClonesStack;


    // slurs
    // ------------------------------------------------------
//     Bool                      fOnGoingChordSlurLink;

    // stanzas
    // ------------------------------------------------------
//     S_msrStanza               fCurrentStanzaClone;
//     Bool                      fOnGoingStanza;


    // syllables
    // ------------------------------------------------------
//     S_msrSyllable             fCurrentSyllableClone;
//     Bool                      fOnGoingSyllableExtend;

    // current ongoing values display
    // ------------------------------------------------------
    virtual void              displayCurrentOnGoingValues () override;

};


}


#endif // ___msr2lpsrTranslator___
