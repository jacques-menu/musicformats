/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsr2lilypondTranslator___
#define ___lpsr2lilypondTranslator___

// libmusicxml2
#include "visitor.h"

#include "msrPitchesNames.h"

#include "msrTypesForwardDeclarations.h"
#include "lpsrTypesForwardDeclarations.h"

#include "msrDirections.h"
#include "msrInstruments.h"
#include "msrKeys.h"
#include "msrPlacements.h"
#include "msrPrintObjects.h"
#include "msrRepeats.h"
#include "msrStems.h"
#include "msrTupletFactors.h"

#include "msrOah.h"
#include "lpsrOah.h"

#include "msrHiddenMeasureAndBarLines.h"


namespace MusicFormats
{

//________________________________________________________________________
struct lpsrRepeatDescr : public smartable
{
/*
 * positions represent the order in which the parts appear in <part-std::list />
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrRepeatDescr> create (
                            S_msrRepeat repeat,
                            int         repeatEndingsNumber);

  protected:

    //ructors/destructor
    // ------------------------------------------------------

                          lpsrRepeatDescr (
                            S_msrRepeat repeat,
                            int         repeatEndingsNumber);

    virtual               ~lpsrRepeatDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrRepeat           getRepeat ()
                              { return fRepeat; }

    int                   getRepeatEndingsNumber ()
                              { return fRepeatEndingsNumber; }

    void                  incrementRepeatEndingsCounter ()
                              { ++fRepeatEndingsCounter; }

    int                   getRepeatEndingsCounter ()
                              { return fRepeatEndingsCounter; }

    void                  setEndOfRepeatHasBeenGenerated ()
                              { fEndOfRepeatHasBeenGenerated = true; }

    Bool                  getEndOfRepeatHasBeenGenerated ()
                              { return fEndOfRepeatHasBeenGenerated; }

  public:

    // public services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    std::string           asString ();

    virtual void          print (std::ostream& os);

  private:

    // private fields
    // ------------------------------------------------------

    S_msrRepeat           fRepeat;

    int                   fRepeatEndingsNumber;
    int                   fRepeatEndingsCounter;

    Bool                  fEndOfRepeatHasBeenGenerated;
};
typedef SMARTP<lpsrRepeatDescr> S_lpsrRepeatDescr;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrRepeatDescr& elt);

//________________________________________________________________________
enum class lilypondMarkupColumnKind { // JMI v0.9.66
  kMarkupColumnKindLeftAligned,
  kMarkupColumnKindLeftACentered
};

class EXP lpsr2lilypondTranslator :

  // LPSR

  public visitor<S_lpsrScore>,

  // rights

  public visitor<S_msrIdentification>,

  // Scheme variable-s

  public visitor<S_lpsrSchemeVariable>,

  // MSR scaling

  public visitor<S_msrScaling>,

  // header

  public visitor<S_lpsrHeader>,

  // paper

  public visitor<S_lpsrPaper>,

  // layout

  public visitor<S_lpsrLayout>,

  // score blocks

  public visitor<S_lpsrParallelMusicBLock>,

  public visitor<S_lpsrBookBlock>,
  public visitor<S_lpsrScoreBlock>,
  public visitor<S_lpsrBookPartBlock>,

  public visitor<S_lpsrPartGroupBlock>,
  public visitor<S_lpsrPartBlock>,
  public visitor<S_lpsrStaffBlock>,

  public visitor<S_lpsrUseVoiceCommand>,
  public visitor<S_lpsrNewLyricsBlock>,
  public visitor<S_lpsrVariableUseCommand>,

//  public visitor<S_lpsrContext>, JMI
  public visitor<S_lpsrChordNamesContext>,
  public visitor<S_lpsrFiguredBassContext>,

  public visitor<S_lpsrBarCommand>,

 // JMI public visitor<S_lpsrMelismaCommand>,

  public visitor<S_lpsrComment>,

  public visitor<S_lpsrSchemeFunction>,

  // MSR score

  public visitor<S_msrScore>,

  // MSR page layout

  public visitor<S_msrPageLayout>,

  // rights

  public visitor<S_msrCredit>,
  public visitor<S_msrCreditWords>,

  // parts & part groups

  public visitor<S_msrPartGroup>,

  public visitor<S_msrPart>,

  // staff details

  public visitor<S_msrStaffTuning>,
  public visitor<S_msrStaffDetails>,

  // staves and voices

  public visitor<S_msrStaff>,

  public visitor<S_msrVoice>,

  public visitor<S_msrVoiceStaffChange>,

  // harmonies

  public visitor<S_msrHarmony>,

  // frames

  public visitor<S_msrFrame>,

  // figured bass

  public visitor<S_msrFiguredBass>,
  public visitor<S_msrBassFigure>,

  public visitor<S_msrSegment>,
  public visitor<S_msrMeasure>,

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
  public visitor<S_msrTempoNotesRelationshipElements>,
  public visitor<S_msrTempoNote>,
  public visitor<S_msrTempoTuplet>,

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

  public visitor<S_msrCrescDecresc>,

  public visitor<S_msrWedge>,

  // grace notes

  public visitor<S_msrGraceNotesGroup>,

  public visitor<S_msrChordGraceNotesGroupLink>,

  public visitor<S_msrAfterGraceNotesGroup>,
  public visitor<S_msrAfterGraceNotesGroupContents>,

  // notes

  public visitor<S_msrNote>,
  public visitor<S_msrOctaveShift>,

  // accordion registration

  public visitor<S_msrAccordionRegistration>,

  // harp pedals tuning

  public visitor<S_msrHarpPedalsTuning>,

  // stems

  public visitor<S_msrStem>,

  // beams

  public visitor<S_msrBeam>,

  // chords

  public visitor<S_msrChord>,

  public visitor<S_msrTuplet>,

  public visitor<S_msrTie>,

  public visitor<S_msrSlur>,
  /*
  public visitor<S_msrChordSlurLink>,
*/

  public visitor<S_msrLigature>,

  // ?

  public visitor<S_msrBarLine>,

  // segnos and codas
  public visitor<S_msrSegno>,
  public visitor<S_msrHiddenMeasureAndBarLine>,
  public visitor<S_msrCoda>,

  public visitor<S_msrEyeGlasses>,
  public visitor<S_msrPedal>,
  public visitor<S_msrDamp>,
  public visitor<S_msrDampAll>,
  public visitor<S_msrScordatura>,

  public visitor<S_msrBarCheck>,

  public visitor<S_msrBarNumberCheck>,

  public visitor<S_msrLineBreak>,

  public visitor<S_msrPageBreak>,

  // repeats

  public visitor<S_msrRepeat>,
  public visitor<S_msrRepeatCommonPart>,
  public visitor<S_msrRepeatEnding>,

  public visitor<S_msrMeasureRepeat>,
  public visitor<S_msrMeasureRepeatPattern>,
  public visitor<S_msrMeasureRepeatReplicas>,

  public visitor<S_msrMultipleFullBarRests>,

  // rehearsal mark

  public visitor<S_msrRehearsalMark>,

  // midi

  public visitor<S_msrMidiTempo>

{
  public:

                          lpsr2lilypondTranslator (
                            S_msrOahGroup&  msrOpts,
                            S_lpsrOahGroup& lpsrOpts,
                            std::ostream&        lilypondCodeStream);

    virtual               ~lpsr2lilypondTranslator ();

    void                  translateLpsrToLilypondCode (
                            S_lpsrScore theLpsrScore);

  protected:

    // LPSR
    // ------------------------------------------------------

    virtual void          visitStart (S_lpsrScore& elt);
    virtual void          visitEnd   (S_lpsrScore& elt);

    virtual void          visitStart (S_msrIdentification& elt);
    virtual void          visitEnd   (S_msrIdentification& elt);

    virtual void          visitStart (S_lpsrSchemeVariable& elt);
    virtual void          visitEnd   (S_lpsrSchemeVariable& elt);

    virtual void          visitStart (S_lpsrHeader& elt);
    virtual void          visitEnd   (S_lpsrHeader& elt);

    virtual void          visitStart (S_lpsrPaper& elt);
    virtual void          visitEnd   (S_lpsrPaper& elt);

    virtual void          visitStart (S_lpsrLayout& elt);
    virtual void          visitEnd   (S_lpsrLayout& elt);

    virtual void          visitStart (S_lpsrBookBlock& elt);
    virtual void          visitEnd   (S_lpsrBookBlock& elt);
    virtual void          visitStart (S_lpsrScoreBlock& elt);
    virtual void          visitEnd   (S_lpsrScoreBlock& elt);
    virtual void          visitStart (S_lpsrBookPartBlock& elt);
    virtual void          visitEnd   (S_lpsrBookPartBlock& elt);

    virtual void          visitStart (S_lpsrParallelMusicBLock& elt);
    virtual void          visitEnd   (S_lpsrParallelMusicBLock& elt);

    virtual void          visitStart (S_lpsrPartGroupBlock& elt);
    virtual void          visitEnd   (S_lpsrPartGroupBlock& elt);

    virtual void          visitStart (S_lpsrPartBlock& elt);
    virtual void          visitEnd   (S_lpsrPartBlock& elt);

    virtual void          visitStart (S_lpsrStaffBlock& elt);
    virtual void          visitEnd   (S_lpsrStaffBlock& elt);

    virtual void          visitStart (S_lpsrUseVoiceCommand& elt);
    virtual void          visitEnd   (S_lpsrUseVoiceCommand& elt);

    virtual void          visitStart (S_lpsrNewLyricsBlock& elt);
    virtual void          visitEnd   (S_lpsrNewLyricsBlock& elt);

    virtual void          visitStart (S_lpsrVariableUseCommand& elt);
    virtual void          visitEnd   (S_lpsrVariableUseCommand& elt);

 //   virtual void          visitStart (S_lpsrContext& elt); JMI
//    virtual void          visitEnd   (S_lpsrContext& elt);
    virtual void          visitStart (S_lpsrChordNamesContext& elt);
    virtual void          visitEnd   (S_lpsrChordNamesContext& elt);
    virtual void          visitStart (S_lpsrFiguredBassContext& elt);
    virtual void          visitEnd   (S_lpsrFiguredBassContext& elt);

    virtual void          visitStart (S_lpsrBarCommand& elt);
    virtual void          visitEnd   (S_lpsrBarCommand& elt);

// JMI    virtual void          visitStart (S_lpsrMelismaCommand& elt);
 //   virtual void          visitEnd   (S_lpsrMelismaCommand& elt);

    virtual void          visitStart (S_lpsrComment& elt);
    virtual void          visitEnd   (S_lpsrComment& elt);

    virtual void          visitStart (S_lpsrSchemeFunction& elt);
    virtual void          visitEnd   (S_lpsrSchemeFunction& elt);


    // MSR
    // ------------------------------------------------------

    // MSR score
    virtual void          visitStart (S_msrScore& elt);
    virtual void          visitEnd   (S_msrScore& elt);

    // MSR scaling
    virtual void          visitStart (S_msrScaling& elt);
    virtual void          visitEnd   (S_msrScaling& elt);

     // MSR page layout

    // layout
    virtual void          visitStart (S_msrPageLayout& elt);
    virtual void          visitEnd   (S_msrPageLayout& elt);

    virtual void          visitStart (S_msrSystemLayout& elt);
    virtual void          visitEnd   (S_msrSystemLayout& elt);

    virtual void          visitStart (S_msrStaffLayout& elt);
    virtual void          visitEnd   (S_msrStaffLayout& elt);

    // appearance
    virtual void          visitStart (S_msrAppearance& elt);
    virtual void          visitEnd   (S_msrAppearance& elt);

    // credits
    virtual void          visitStart (S_msrCredit& elt);
    virtual void          visitEnd   (S_msrCredit& elt);
    virtual void          visitStart (S_msrCreditWords& elt);
    virtual void          visitEnd   (S_msrCreditWords& elt);

    // part groups
    virtual void          visitStart (S_msrPartGroup& elt);
    virtual void          visitEnd   (S_msrPartGroup& elt);

    //parts
    virtual void          visitStart (S_msrPart& elt);
    virtual void          visitEnd   (S_msrPart& elt);

    // staves
    virtual void          visitStart (S_msrStaffTuning& elt);
    virtual void          visitStart (S_msrStaffDetails& elt);

    virtual void          visitStart (S_msrStaff& elt);
    virtual void          visitEnd   (S_msrStaff& elt);

    // voices
    virtual void          visitStart (S_msrVoice& elt);
    virtual void          visitEnd   (S_msrVoice& elt);

    virtual void          visitStart (S_msrVoiceStaffChange& elt);

    // harmonies
    virtual void          visitStart (S_msrHarmony& elt);

    // frames
    virtual void          visitStart (S_msrFrame& elt);

    // figured bass
    virtual void          visitStart (S_msrFiguredBass& elt);
    virtual void          visitEnd   (S_msrFiguredBass& elt);
    virtual void          visitStart (S_msrBassFigure& elt);

    virtual void          visitStart (S_msrSegment& elt);
    virtual void          visitEnd   (S_msrSegment& elt);

    virtual void          visitStart (S_msrMeasure& elt);
    virtual void          visitEnd   (S_msrMeasure& elt);

    // lyrics
    virtual void          visitStart (S_msrStanza& elt);
    virtual void          visitEnd   (S_msrStanza& elt);

    virtual void          visitStart (S_msrSyllable& elt);
    virtual void          visitEnd   (S_msrSyllable& elt);

    // clefs
    virtual void          visitStart (S_msrClef& elt);
    virtual void          visitEnd   (S_msrClef& elt);

    // keys
    virtual void          visitStart (S_msrKey& elt);
    virtual void          visitEnd   (S_msrKey& elt);

    // time signatures
    virtual void          visitStart (S_msrTimeSignature& elt);
    virtual void          visitEnd   (S_msrTimeSignature& elt);

    virtual void          visitStart (S_msrTransposition& elt);
    virtual void          visitEnd   (S_msrTransposition& elt);

    // words
    virtual void          visitStart (S_msrWords& elt);
    virtual void          visitEnd   (S_msrWords& elt);

    // tempo
    virtual void          visitStart (S_msrTempo& elt);
    virtual void          visitEnd   (S_msrTempo& elt);
    virtual void          visitStart (S_msrTempoNotesRelationshipElements& elt);
    virtual void          visitEnd   (S_msrTempoNotesRelationshipElements& elt);
    virtual void          visitStart (S_msrTempoNote& elt);
    virtual void          visitStart (S_msrTempoTuplet& elt);
    virtual void          visitEnd   (S_msrTempoTuplet& elt);

    // articulations
    virtual void          visitStart (S_msrArticulation& elt);
    virtual void          visitEnd   (S_msrArticulation& elt);

    virtual void          visitStart (S_msrFermata& elt);
    virtual void          visitEnd   (S_msrFermata& elt);

    virtual void          visitStart (S_msrArpeggiato& elt);
    virtual void          visitEnd   (S_msrArpeggiato& elt);
    virtual void          visitStart (S_msrNonArpeggiato& elt);
    virtual void          visitEnd   (S_msrNonArpeggiato& elt);

    // technicals
    virtual void          visitStart (S_msrTechnical& elt);
    virtual void          visitEnd   (S_msrTechnical& elt);

    virtual void          visitStart (S_msrTechnicalWithInteger& elt);
    virtual void          visitEnd   (S_msrTechnicalWithInteger& elt);

    virtual void          visitStart (S_msrTechnicalWithFloat& elt);
    virtual void          visitEnd   (S_msrTechnicalWithFloat& elt);

    virtual void          visitStart (S_msrTechnicalWithString& elt);
    virtual void          visitEnd   (S_msrTechnicalWithString& elt);

    // ornaments
    virtual void          visitStart (S_msrOrnament& elt);
    virtual void          visitEnd   (S_msrOrnament& elt);

    // glissandos
    virtual void          visitStart (S_msrGlissando& elt);
    virtual void          visitEnd   (S_msrGlissando& elt);

    // slides
    virtual void          visitStart (S_msrSlide& elt);
    virtual void          visitEnd   (S_msrSlide& elt);

    // tremolos
    virtual void          visitStart (S_msrSingleTremolo& elt);
    virtual void          visitEnd   (S_msrSingleTremolo& elt);

    virtual void          visitStart (S_msrDoubleTremolo& elt);
    virtual void          visitEnd   (S_msrDoubleTremolo& elt);

    // dynamics
    virtual void          visitStart (S_msrDynamic& elt);
    virtual void          visitEnd   (S_msrDynamic& elt);

    virtual void          visitStart (S_msrOtherDynamic& elt);
    virtual void          visitEnd   (S_msrOtherDynamic& elt);

    virtual void          visitStart (S_msrCrescDecresc& elt);
    virtual void          visitEnd   (S_msrCrescDecresc& elt);

    virtual void          visitStart (S_msrWedge& elt);
    virtual void          visitEnd   (S_msrWedge& elt);

    // grace notes
    virtual void          visitStart (S_msrGraceNotesGroup& elt);
    virtual void          visitEnd   (S_msrGraceNotesGroup& elt);

    virtual void          visitStart (S_msrChordGraceNotesGroupLink& elt);
    virtual void          visitEnd   (S_msrChordGraceNotesGroupLink& elt);

    virtual void          visitStart (S_msrAfterGraceNotesGroup& elt);
    virtual void          visitEnd   (S_msrAfterGraceNotesGroup& elt);
    virtual void          visitStart (S_msrAfterGraceNotesGroupContents& elt);
    virtual void          visitEnd   (S_msrAfterGraceNotesGroupContents& elt);

    // notes
    virtual void          visitStart (S_msrNote& elt);
    virtual void          visitEnd   (S_msrNote& elt);

    virtual void          visitStart (S_msrOctaveShift& elt);
    virtual void          visitEnd   (S_msrOctaveShift& elt);

    // accordion registration
    virtual void          visitStart (S_msrAccordionRegistration& elt);

    // harp pedals tuning
    virtual void          visitStart (S_msrHarpPedalsTuning& elt);

    // stems
    virtual void          visitStart (S_msrStem& elt);
    virtual void          visitEnd   (S_msrStem& elt);

    // beams
    virtual void          visitStart (S_msrBeam& elt);
    virtual void          visitEnd   (S_msrBeam& elt);

    // chords
    virtual void          visitStart (S_msrChord& elt);
    virtual void          visitEnd   (S_msrChord& elt);

    // tuplets
    virtual void          visitStart (S_msrTuplet& elt);
    virtual void          visitEnd   (S_msrTuplet& elt);

    // ties
    virtual void          visitStart (S_msrTie& elt);
    virtual void          visitEnd   (S_msrTie& elt);

    // slurs
    virtual void          visitStart (S_msrSlur& elt);
    virtual void          visitEnd   (S_msrSlur& elt);
    /*
    virtual void          visitStart (S_msrChordSlurLink& elt);
    virtual void          visitEnd   (S_msrChordSlurLink& elt);
    */

    // ligatures
    virtual void          visitStart (S_msrLigature& elt);
    virtual void          visitEnd   (S_msrLigature& elt);

    // bar lines
    virtual void          visitStart (S_msrBarLine& elt);
    virtual void          visitEnd   (S_msrBarLine& elt);

    // segnos and codas
    virtual void          visitStart (S_msrSegno& elt);
    virtual void          visitStart (S_msrHiddenMeasureAndBarLine& elt);
    virtual void          visitStart (S_msrCoda& elt);

    virtual void          visitStart (S_msrEyeGlasses& elt);
    virtual void          visitStart (S_msrPedal& elt);
    virtual void          visitStart (S_msrDamp& elt);
    virtual void          visitStart (S_msrDampAll& elt);
    virtual void          visitStart (S_msrScordatura& elt);

    // bar numbers
    virtual void          visitStart (S_msrBarCheck& elt);
    virtual void          visitEnd   (S_msrBarCheck& elt);

    virtual void          visitStart (S_msrBarNumberCheck& elt);
    virtual void          visitEnd   (S_msrBarNumberCheck& elt);

    // breaks
    virtual void          visitStart (S_msrLineBreak& elt);
    virtual void          visitEnd   (S_msrLineBreak& elt);

    virtual void          visitStart (S_msrPageBreak& elt);
    virtual void          visitEnd   (S_msrPageBreak& elt);

    // repeats
    virtual void          visitStart (S_msrRepeat& elt);
    virtual void          visitEnd   (S_msrRepeat& elt);
    virtual void          visitStart (S_msrRepeatCommonPart& elt);
    virtual void          visitEnd   (S_msrRepeatCommonPart& elt);
    virtual void          visitStart (S_msrRepeatEnding& elt);
    virtual void          visitEnd   (S_msrRepeatEnding& elt);

    // measures repests
    virtual void          visitStart (S_msrMeasureRepeat& elt);
    virtual void          visitEnd   (S_msrMeasureRepeat& elt);
    virtual void          visitStart (S_msrMeasureRepeatPattern& elt);
    virtual void          visitEnd   (S_msrMeasureRepeatPattern& elt);
    virtual void          visitStart (S_msrMeasureRepeatReplicas& elt);
    virtual void          visitEnd   (S_msrMeasureRepeatReplicas& elt);

    // multiple full-bar rests
    virtual void          visitStart (S_msrMultipleFullBarRests& elt);
    virtual void          visitEnd   (S_msrMultipleFullBarRests& elt);

    // rehearsal marks
    virtual void          visitStart (S_msrRehearsalMark& elt);
    virtual void          visitEnd   (S_msrRehearsalMark& elt);

    // MIDI
    virtual void          visitStart (S_msrMidiTempo& elt);
    virtual void          visitEnd   (S_msrMidiTempo& elt);

  private:

    // private fields and methods
    // ------------------------------------------------------

    // names

    std::string           nameAsLilypondString (
                            const std::string& name);

    // lengths

    std::string           lengthUnitAsLilypondString (
                            msrLengthUnitKind lengthUnitKind);

    // whole notes
    // JMI

    // comments
    void                  generateInputLineNumberAndOrMeasurePositionAsAComment (
                            S_msrMeasureElement measureElement);

    // markups

    std::string           generateAColumnForMarkup (
                            const std::string&    theString,
                            lilypondMarkupColumnKind columnKind);
    std::string           generateMultilineMarkup (
                            const std::string&    theString,
                            lilypondMarkupColumnKind columnKind);

    // octaves

    std::string           absoluteOctaveAsLilypondString (
                            int           inputLineNumber,
                            msrOctaveKind absoluteOctaveKind);

    // alterations

    std::string           alterationKindAsLilypondString (
                            msrAlterationKind alterationKind);

    // durations

    Rational              fLastMetWholeNotes;

    std::string           durationAsLilypondString (
                            int             inputLineNumber,
                            const Rational& wholeNotes);

    std::string           msrDurationKindAsLilypondString (
                            msrDurationKind durationKind);

    // notes

    msrPrintObjectKind    fCurrentNotePrinObjectKind;

    std::string           lilypondOctaveInRelativeEntryMode (
                            S_msrNote note);

    std::string           lilypondOctaveInFixedEntryMode (
                            S_msrNote note);

    std::string           notePitchAsLilypondString (
                            S_msrNote note);

    std::string           pitchedRestAsLilypondString (
                            S_msrNote note);

    void                  generateNoteBeams (
                            S_msrNote note);

    void                  generateNoteSlurDirection (
                            S_msrNote note);
    void                  generateNoteSlurs (
                            S_msrNote note);

    void                  generateNoteHeadColor (
                            S_msrNote note);
    void                  generateNoteLigatures (
                            S_msrNote note);
    void                  generateNoteHead (
                            S_msrNote note);

    void                  generateSegno (S_msrSegno segno);
    void                  generateCoda (S_msrCoda coda);

    void                  generateCodeRightBeforeNote (
                            S_msrNote note);

    void                  generateCodeForNote (
                            S_msrNote note);

    void                  generateCodeForNoteRegularInMeasure (
                            S_msrNote note);

    void                  generateCodeForNoteRestInMeasure (
                            S_msrNote note);
    void                  generateCodeForNoteSkipInMeasure (
                            S_msrNote note);
    void                  generateCodeForNoteUnpitchedInMeasure (
                            S_msrNote note);

    void                  generateCodeForNoteRegularInChord (
                            S_msrNote note);

    void                  generateCodeForNoteRegularInTuplet (
                            S_msrNote note);
    void                  generateCodeForNoteRestInTuplet (
                            S_msrNote note);
    void                  generateCodeForNoteUnpitchedInTuplet (
                            S_msrNote note);

    void                  generateCodeForNoteRegularInGraceNotesGroup (
                            S_msrNote note);
    void                  generateCodeForNoteSkipInGraceNotesGroup (
                            S_msrNote note);

    void                  generateCodeForNoteInChordInGraceNotesGroup (
                            S_msrNote note);

    void                  generateCodeForNoteInTupletInGraceNotesGroup (
                            S_msrNote note);

    void                  generateCodeForNoteInDoubleTremolo (
                            S_msrNote note);

    void                  generateNoteScordaturas (
                            S_msrNote note);

    void                  generateNoteSlashes (
                            S_msrNote note);

    void                  generateNoteWedges (
                            S_msrNote note);

    void                  generateNoteSlurLineTypes (
                            S_msrNote note);

    void                  generateNoteGlissandoStyles (
                            S_msrNote note);
    void                  generateNoteGlissandosWithText (
                            S_msrNote note);

    void                  generateNoteSlideLineStyles (
                            S_msrNote note);
    void                  generateNoteSlidesWithText (
                            S_msrNote note);

    void                  generateNoteTechnicalsWithStrings (
                            S_msrNote note);

    void                  generateCodeForNoteWords (
                            const std::list<S_msrWords>& noteWords);

    void                  generateCodeRightAfterNote (
                            S_msrNote note);

    // stems

    // the LilyPond \stem* commands have a persistent effect, hence:
    msrStemKind  fCurrentStemKind;

    std::string           stemAsLilypondString (
                            msrStemKind stemKind);

    void                  generateStemIfNeededAndUpdateCurrentStemKind (
                            S_msrStem stem);

    // articulations

    msrDirectionKind      fCurrentArpeggioDirectionKind;

    void                  generateNoteArticulation (
                            S_msrArticulation articulation);

    void                  generateChordArticulation (
                            S_msrArticulation articulation);

    // technicals

    std::string           technicalAsLilypondString (
                            S_msrTechnical technical);

    std::string           technicalWithIntegerAsLilypondString (
                            S_msrTechnicalWithInteger technicalWithInteger);

    std::string           technicalWithFloatAsLilypondString (
                            S_msrTechnicalWithFloat technicalWithFloat);

    std::string           technicalWithStringAsLilypondString (
                            S_msrTechnicalWithString technicalWithString);

    // ornaments

    void                  generateOrnament (
                            S_msrOrnament ornament);

    // trills

    Bool                  fOnGoingTrillSpanner;

    // spanners

    msrPlacementKind      fCurrentSpannerPlacementKind;

    void                  generateBeforeNoteSpannersIfAny (
                            S_msrNote note);

    void                  generateAfterNoteSpannersIfAny (
                            S_msrNote note);

    void                  generateCodeForSpannerBeforeNote (
                            S_msrSpanner spanner);
    void                  generateCodeForSpannerAfterNote (
                            S_msrSpanner spanner);

    // tuplets

    std::string           tupletFactorAsLilypondString (
                            const msrTupletFactor& tupletFactor);

    // grace notes

    Bool                  fOnGoingGraceNotesGroup;
    Bool                  fOnGoingChordGraceNotesGroupLink;

    void                  generateGraceNotesGroup (
                            S_msrGraceNotesGroup graceNotesGroup);

    // tremolos

    std::string           singleTremoloDurationAsLilypondString (
                            S_msrSingleTremolo singleTremolo);

    // dynamics

    std::string           dynamicAsLilypondString (
                            S_msrDynamic dynamic);

    // std::string tuning
    std::string           stringTuningAsLilypondString (
                            int               inputLineNumber,
                            S_msrStringTuning stringTuning);

    // harp pedals tuning

    std::string           harpPedalTuningAsLilypondString (
                            msrAlterationKind alterationKind);

    // transposition

    void                  transposeDiatonicError (
                            int inputLineNumber,
                            int transposeDiatonic,
                            int transposeChromatic);

    // harmonies

    Bool                  fPowerChordHaveAlreadyBeenGenerated;
                            // to generate it only once

    std::string           harmonyAsLilypondString (
                            S_msrHarmony harmony);

    std::string           harmonyDegreeAlterationKindAsLilypondString (
                            msrAlterationKind harmonyDegreeAlterationKind);

    // frames

    std::string           frameAsLilypondString (
                            S_msrFrame frame);

    // figured bass

    std::string           figureAsLilypondString (
                            S_msrBassFigure bassFigure);
    std::string           figuredBassAsLilypondString (
                            S_msrFiguredBass figuredBass);

    S_msrFiguredBass
                          fCurrentFiguredBass;
    size_t                fCurrentFiguredBassFiguresCounter;

  private:

    // options
    // ------------------------------------------------------

    S_msrOahGroup         fMsrOahGroup;
    S_lpsrOahGroup        fLpsrOahGroup;

    // the LPSR score we're visiting
    // ------------------------------------------------------
    S_lpsrScore           fVisitedLpsrScore;

    // the output stream
    // ------------------------------------------------------

    std::ostream&              fLilypondCodeStream;

    // LilyPond version
    // ------------------------------------------------------
    void                  generateLilypondVersion ();

    // identification
    // ------------------------------------------------------
    /* JMI
    std::string           fScoreTitle;
    std::string           fScoreSubTitle;
    std::string           fScoreSubSubTitle;

    void                  computeHeaderFields ();
*/

    // global staff size
    // ------------------------------------------------------
    void                  generateGlobalStaffSize ();

    // header handling
    // ------------------------------------------------------
    Bool                  fOnGoingHeader;

    void                  generateHeader (S_lpsrHeader header);

    void                  generateHeaderIdentificationPart (
                            S_msrIdentification          identification,
                            std::list<std::pair<std::string, std::string> >& nameValuePairsList);

    void                  createLilypondHeaderStringValuePairs (
                            S_lpsrHeader                 header,
                            std::list<std::pair<std::string, std::string> >& nameValuePairsList);

    std::string           generateStringAsLilypondMarkupOrDoubleQuotedString (
                            const std::string& value);

    // paper handling
    // ------------------------------------------------------
    void                  generatePaper (S_lpsrPaper paper);

    void                  fetchLengthValuesFromPaperPageSize (
                            S_lpsrPaper                     paper,
                            std::list<std::pair<std::string, msrLength> >& nameLengthValuePairsList);

    void                  fetchOnOffValuesFromLpsrOptionsGroup (
                            S_lpsrPaper                paper,
                            std::list<std::pair<std::string, Bool> >& nameBooleanValuePairsList);

    void                  generatePaperPageSize (
                            S_msrPageLayout   pageLayout,
                            msrLengthUnitKind defaultLengthUnit,
                            int               fieldWidth);

    void                  generatePaperMargins (
                            S_msrPageLayout   pageLayout,
                            msrLengthUnitKind defaultLengthUnit,
                            int               fieldWidth);

    void                  generatePaperIndents (
                            S_lpsrPaper       pagePaper,
                            msrLengthUnitKind defaultLengthUnit,
                            int               fieldWidth);

    void                  generatePaperSpaces (
                            S_lpsrPaper       pagePaper,
                            msrLengthUnitKind defaultLengthUnit,
                            int               fieldWidth);

    void                  generatePaperCounts (
                            S_lpsrPaper       pagePaper,
                            msrLengthUnitKind defaultLengthUnit,
                            int               fieldWidth);

    void                  generatePaperBooleans (
                            S_lpsrPaper       pagePaper,
                            int               fieldWidth);

    void                  generatePaperHeadersAndFooters (
                            S_lpsrPaper       pagePaper,
                            msrLengthUnitKind defaultLengthUnit,
                            int               fieldWidth);

    // score
    // ------------------------------------------------------
    S_msrScore            fCurrentMsrScoreClone;

    // part groups
    // ------------------------------------------------------
 // JMI   S_msrPartGroup        fCurrentMsrPartGroupClone;

    // parts
    // ------------------------------------------------------
    S_msrPart             fCurrentPart; // JMI jamais visitee

    // tempos
    // ------------------------------------------------------
    void                  generateCodeForTempoBeatUnitsWordsOnly (
                            S_msrTempo tempo);

    void                  generateCodeForTempoBeatUnitsPerMinute (
                            S_msrTempo tempo);

    void                  generateCodeForTempoBeatUnitsEquivalence (
                            S_msrTempo tempo);

    void                  generateCodeForTempoNotesRelationship (
                            S_msrTempo tempo);

    // clefs
    // ------------------------------------------------------
    S_msrClef             fCurrentVoiceClef;

    // keys
    // ------------------------------------------------------
    S_msrClef             fCurrentVoiceKey;

    std::string           msrModeKindAsLilypondString (
                            msrModeKind modeKind);
    // time
    // ------------------------------------------------------
    S_msrTimeSignature    fCurrentVoiceTimeSignature;

    Bool                  fVoiceIsCurrentlySenzaMisura; // JMI
    Bool                  fOnGoingVoiceCadenza;

    // staves
    // ------------------------------------------------------
    // prevent clef, key and time signature from being handled twice
    Bool                  fOnGoingStaff;

    // voices
    // ------------------------------------------------------
    Bool                  fOnGoingVoice;
    S_msrVoice            fCurrentVoice;

    Bool                  fOnGoingHarmoniesVoice;
    Bool                  fOnGoingFiguredBassVoice;

    // lyrics
    // ------------------------------------------------------
    Bool                  fOnGoingStanza;
    S_msrStanza           fCurrentStanza;

    // octaves entry
    // ------------------------------------------------------
    /* this reference is:
          mobile in relative mode
          unused in absolute mode
          fixed  in fixed mode
    */
    S_msrNote             fCurrentOctaveEntryReference;
    void                  setCurrentOctaveEntryReferenceFromTheLilypondOah ();

    // notes

    std::string           msrQuarterTonesPitchesLanguageKindAsLilypondString (
                            msrQuarterTonesPitchesLanguageKind languageKind);

    // repeats
    // ------------------------------------------------------
    std::list<S_lpsrRepeatDescr>
                          fRepeatDescrsStack;

    // multiple full-bar rests
    // ------------------------------------------------------
    int                   fRemainingMultipleFullBarRestsNumber;
    Bool                  fOnGoingMultipleFullBarRests;

    // segments
    // ------------------------------------------------------

    // measures
    // ------------------------------------------------------
    // we need to count the measures
    // for option fSeparatorLineEveryNMeasures and bar number checks,
    // since measure numbers can actually be any strings
    int                   fCurrentVoiceMeasuresCounter;

    // notes
    // ------------------------------------------------------
    // browsing grace notes groups leads to several notes
    // being ongoing simultaneously,
    // since such groups are attached to a note, hence:
    std::stack<S_msrNote>      fOnGoingNotesStack; // USELESS??? JMI CLAR


    // double tremolos
    // ------------------------------------------------------

    // octave shifts
    // ------------------------------------------------------
    void                  generateCodeForOctaveShiftBeforeNote (
                            S_msrOctaveShift octaveShift);
    void                  generateCodeForOctaveShiftAfterNote (
                            S_msrOctaveShift octaveShift);

    // chords
    // ------------------------------------------------------
    std::list<int>        fPendingChordMemberNotesStringNumbers;

    Bool                  fOnGoingChord;
    S_msrChord            fCurrentChord;

    std::list<int>        fCurrentChordPendingSlurs;

    void                  generateCodeAheadOfChordContents (
                            S_msrChord chord);

    void                  generateCodeRightBeforeChordContents (
                            S_msrChord chord);
    void                  generateCodeForChordInGraceNotesGroupContents (
                            S_msrChord chord);
    void                  generateCodeRightAfterChordContents (
                            S_msrChord chord);

    void                  generateChordInGraceNotesGroup (
                            S_msrChord chord);


    // tuplets
    // ------------------------------------------------------
// JMI     S_msrTuplet          fCurrentMsrTupletClone;
    std::stack<S_msrTuplet>    fOnGoingTupletsStack;

    // stanzas
    // ------------------------------------------------------
    S_msrStanza           fCurrentStanzaClone;
    Bool                  fGenerateCodeForOngoingNonEmptyStanza;


    // syllables
    // ------------------------------------------------------
    S_msrSyllable         fCurrentMsrSyllableClone;


    // book blocks
    // ------------------------------------------------------
    Bool                  fOnGoingBookBlock; // JMI

    // book part blocks
    // ------------------------------------------------------
    Bool                  fOnGoingBookPartBlock;


    // score blocks
    // ------------------------------------------------------
    Bool                  fOnGoingScoreBlock; // JMI


    // parallel music
    // ------------------------------------------------------
    S_lpsrParallelMusicBLock
                          fCurrentParallelMusicBLock;


    // part group blocks
    // ------------------------------------------------------
    int                   fNumberOfPartGroupBlocks;
    int                   fPartGroupBlocksCounter;


    // part blocks
    // ------------------------------------------------------
    int                   fNumberOfPartGroupBlockElements;
    int                   fPartGroupBlockElementsCounter;


    // staff blocks
    // ------------------------------------------------------
    int                   fNumberOfStaffBlocksElements;
    int                   fStaffBlocksCounter;


    // current ongoing values display
    // ------------------------------------------------------
    void                  displayCurrentOnGoingValues ();
};


}


#endif
