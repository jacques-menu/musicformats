/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsr2lilypondTranslator___
#define ___lpsr2lilypondTranslator___

#include "visitor.h"

#include "msrTypesForwardDeclarations.h"
#include "lpsrTypesForwardDeclarations.h"

#include "msrDirections.h"
#include "msrHiddenMeasureAndBarLines.h"
#include "msrInstruments.h"
#include "msrClefsKeysTimeSignatures.h"
#include "msrPitchesNames.h"
#include "msrPlacements.h"
#include "msrPrintObjects.h"
#include "msrRepeats.h"
#include "msrStems.h"
#include "msrTuplets.h"

#include "lpsrContexts.h"

#include "msrOah.h"
#include "lpsrOah.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
struct lpsrRepeat : public smartable
{
/*
 * positions represent the order in which the parts appear in <part-list />
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrRepeat> create (
                            const S_msrRepeat& repeat,
                            int                repeatEndingsNumber);

  protected:

    //ructors/destructor
    // ------------------------------------------------------

                          lpsrRepeat (
                            const S_msrRepeat& repeat,
                            int                repeatEndingsNumber);

    virtual               ~lpsrRepeat ();

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
typedef SMARTP<lpsrRepeat> S_lpsrRepeat;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrRepeat& elt);

//________________________________________________________________________
enum class lilypondMarkupColumnKind { // JMI v0.9.66
  kMarkupColumnKindLeftAligned,
  kMarkupColumnKindLeftACentered
};

class EXP lpsr2lilypondTranslator :

  // LPSR

  public                      visitor<S_lpsrScore>,

  // rights

  public                      visitor<S_msrIdentification>,

  // Scheme variable-s

  public                      visitor<S_lpsrSchemeVariable>,

  // MSR scaling

  public                      visitor<S_msrScaling>,

  // header

  public                      visitor<S_lpsrHeader>,

  // paper

  public                      visitor<S_lpsrPaper>,

  // layout

  public                      visitor<S_lpsrLayout>,

  // score blocks

  public                      visitor<S_lpsrParallelMusicBLock>,

  public                      visitor<S_lpsrBookBlock>,
  public                      visitor<S_lpsrScoreBlock>,
  public                      visitor<S_lpsrBookPartBlock>,

  public                      visitor<S_lpsrPartGroupBlock>,
  public                      visitor<S_lpsrPartBlock>,
  public                      visitor<S_lpsrStaffBlock>,

  public                      visitor<S_lpsrUseVoiceCommand>,
  public                      visitor<S_lpsrNewLyricsBlock>,
  public                      visitor<S_lpsrVariableUseCommand>,

//  public                      visitor<S_lpsrContext>, JMI
  public                      visitor<S_lpsrChordNamesContext>,
  public                      visitor<S_lpsrFiguredBassContext>,

  public                      visitor<S_lpsrBarCommand>,

 // JMI public visitor<S_lpsrMelismaCommand>,

  public                      visitor<S_lpsrComment>,

  public                      visitor<S_lpsrSchemeFunction>,

  // MSR score

  public                      visitor<S_msrScore>,

  // MSR page layout

  public                      visitor<S_msrPageLayout>,

  // rights

  public                      visitor<S_msrCredit>,
  public                      visitor<S_msrCreditWords>,

  // parts & part groups

  public                      visitor<S_msrPartGroup>,

  public                      visitor<S_msrPart>,

  // staff details

  public                      visitor<S_msrStaffTuning>,
  public                      visitor<S_msrStaffDetails>,

  // staves and voices

  public                      visitor<S_msrStaff>,

  public                      visitor<S_msrVoice>,

  public                      visitor<S_msrVoiceStaffChange>,

  // harmonies

  public                      visitor<S_msrHarmony>,

  // frames

  public                      visitor<S_msrFrame>,

  // figured bass

  public                      visitor<S_msrFiguredBass>,
  public                      visitor<S_msrBassFigure>,

  public                      visitor<S_msrSegment>,
  public                      visitor<S_msrMeasure>,

  // lyrics

  public                      visitor<S_msrStanza>,
  public                      visitor<S_msrSyllable>,

  // clefs

  public                      visitor<S_msrClefKeyTimeSignatureGroup>,
  public                      visitor<S_msrClef>,

  // keys

  public                      visitor<S_msrKey>,

  // time signatures

  public                      visitor<S_msrTimeSignature>,

  // transposition

  public                      visitor<S_msrTransposition>,

  // words

  public                      visitor<S_msrWords>,

  // tempo

  public                      visitor<S_msrTempo>,
  public                      visitor<S_msrTempoNotesRelationshipElements>,
  public                      visitor<S_msrTempoNote>,
  public                      visitor<S_msrTempoTuplet>,

  // articulations

  public                      visitor<S_msrArticulation>,

  public                      visitor<S_msrFermata>,

  public                      visitor<S_msrArpeggiato>,
  public                      visitor<S_msrNonArpeggiato>,

  // technicals

  public                      visitor<S_msrTechnical>,
  public                      visitor<S_msrTechnicalWithInteger>,
  public                      visitor<S_msrTechnicalWithFloat>,
  public                      visitor<S_msrTechnicalWithString>,

  // ornaments

  public                      visitor<S_msrOrnament>,

  // glissandos

  public                      visitor<S_msrGlissando>,

  // slides

  public                      visitor<S_msrSlide>,

  // tremolos

  public                      visitor<S_msrSingleTremolo>,

  public                      visitor<S_msrDoubleTremolo>,

  // dynamics

  public                      visitor<S_msrDynamic>,

  public                      visitor<S_msrOtherDynamic>,

  public                      visitor<S_msrCrescDecresc>,

  public                      visitor<S_msrWedge>,

  // grace notes

  public                      visitor<S_msrGraceNotesGroup>,

  public                      visitor<S_msrChordGraceNotesGroupLink>,

  public                      visitor<S_msrAfterGraceNotesGroup>,
  public                      visitor<S_msrAfterGraceNotesGroupContents>,

  // notes

  public                      visitor<S_msrNote>,
  public                      visitor<S_msrOctaveShift>,

  // accordion registration

  public                      visitor<S_msrAccordionRegistration>,

  // harp pedals tuning

  public                      visitor<S_msrHarpPedalsTuning>,

  // stems

  public                      visitor<S_msrStem>,

  // beams

  public                      visitor<S_msrBeam>,

  // chords

  public                      visitor<S_msrChord>,

  public                      visitor<S_msrTuplet>,

  public                      visitor<S_msrTie>,

  public                      visitor<S_msrSlur>,
  /*
  public                      visitor<S_msrChordSlurLink>,
*/

  public                      visitor<S_msrLigature>,

  // ?

  public                      visitor<S_msrBarLine>,

  // segnos and codas
  public                      visitor<S_msrSegno>,
  public                      visitor<S_msrHiddenMeasureAndBarLine>,
  public                      visitor<S_msrCoda>,

  public                      visitor<S_msrEyeGlasses>,
  public                      visitor<S_msrPedal>,
  public                      visitor<S_msrDamp>,
  public                      visitor<S_msrDampAll>,
  public                      visitor<S_msrScordatura>,

  public                      visitor<S_msrBarCheck>,

  public                      visitor<S_msrBarNumberCheck>,

  public                      visitor<S_msrLineBreak>,

  public                      visitor<S_msrPageBreak>,

  // repeats

  public                      visitor<S_msrRepeat>,
  public                      visitor<S_msrRepeatCommonPart>,
  public                      visitor<S_msrRepeatEnding>,

  public                      visitor<S_msrMeasureRepeat>,
  public                      visitor<S_msrMeasureRepeatPattern>,
  public                      visitor<S_msrMeasureRepeatReplicas>,

  public                      visitor<S_msrMultipleMeasureRest>,

  // rehearsal mark

  public                      visitor<S_msrRehearsalMark>,

  // midi

  public                      visitor<S_msrMidiTempo>

{
  public:

                              lpsr2lilypondTranslator (
                                const S_msrOahGroup&  msrOpts,
                                const S_lpsrOahGroup& lpsrOpts,
                                std::ostream&         lilypondCodeStream);

        virtual               ~lpsr2lilypondTranslator ();

        void                  translateLpsrToLilypondCode (
                                const S_lpsrScore& theLpsrScore);

  protected:

    // LPSR
    // ------------------------------------------------------

    virtual void              visitStart (S_lpsrScore& elt);
    virtual void              visitEnd   (S_lpsrScore& elt);

    virtual void              visitStart (S_msrIdentification& elt);
    virtual void              visitEnd   (S_msrIdentification& elt);

    virtual void              visitStart (S_lpsrSchemeVariable& elt);
    virtual void              visitEnd   (S_lpsrSchemeVariable& elt);

    virtual void              visitStart (S_lpsrHeader& elt);
    virtual void              visitEnd   (S_lpsrHeader& elt);

    virtual void              visitStart (S_lpsrPaper& elt);
    virtual void              visitEnd   (S_lpsrPaper& elt);

    virtual void              visitStart (S_lpsrLayout& elt);
    virtual void              visitEnd   (S_lpsrLayout& elt);

    virtual void              visitStart (S_lpsrBookBlock& elt);
    virtual void              visitEnd   (S_lpsrBookBlock& elt);
    virtual void              visitStart (S_lpsrScoreBlock& elt);
    virtual void              visitEnd   (S_lpsrScoreBlock& elt);
    virtual void              visitStart (S_lpsrBookPartBlock& elt);
    virtual void              visitEnd   (S_lpsrBookPartBlock& elt);

    virtual void              visitStart (S_lpsrParallelMusicBLock& elt);
    virtual void              visitEnd   (S_lpsrParallelMusicBLock& elt);

    virtual void              visitStart (S_lpsrPartGroupBlock& elt);
    virtual void              visitEnd   (S_lpsrPartGroupBlock& elt);

    virtual void              visitStart (S_lpsrPartBlock& elt);
    virtual void              visitEnd   (S_lpsrPartBlock& elt);

    virtual void              visitStart (S_lpsrStaffBlock& elt);
    virtual void              visitEnd   (S_lpsrStaffBlock& elt);

    virtual void              visitStart (S_lpsrUseVoiceCommand& elt);
    virtual void              visitEnd   (S_lpsrUseVoiceCommand& elt);

    virtual void              visitStart (S_lpsrNewLyricsBlock& elt);
    virtual void              visitEnd   (S_lpsrNewLyricsBlock& elt);

    virtual void              visitStart (S_lpsrVariableUseCommand& elt);
    virtual void              visitEnd   (S_lpsrVariableUseCommand& elt);

 //   virtual void          visitStart (S_lpsrContext& elt); JMI
//    virtual void              visitEnd   (S_lpsrContext& elt);
    virtual void              visitStart (S_lpsrChordNamesContext& elt);
    virtual void              visitEnd   (S_lpsrChordNamesContext& elt);
    virtual void              visitStart (S_lpsrFiguredBassContext& elt);
    virtual void              visitEnd   (S_lpsrFiguredBassContext& elt);

    virtual void              visitStart (S_lpsrBarCommand& elt);
    virtual void              visitEnd   (S_lpsrBarCommand& elt);

// JMI    virtual void              visitStart (S_lpsrMelismaCommand& elt);
 //   virtual void          visitEnd   (S_lpsrMelismaCommand& elt);

    virtual void              visitStart (S_lpsrComment& elt);
    virtual void              visitEnd   (S_lpsrComment& elt);

    virtual void              visitStart (S_lpsrSchemeFunction& elt);
    virtual void              visitEnd   (S_lpsrSchemeFunction& elt);


    // MSR
    // ------------------------------------------------------

    // MSR score
    virtual void              visitStart (S_msrScore& elt);
    virtual void              visitEnd   (S_msrScore& elt);

    // MSR scaling
    virtual void              visitStart (S_msrScaling& elt);
    virtual void              visitEnd   (S_msrScaling& elt);

     // MSR page layout

    // layout
    virtual void              visitStart (S_msrPageLayout& elt);
    virtual void              visitEnd   (S_msrPageLayout& elt);

    virtual void              visitStart (S_msrSystemLayout& elt);
    virtual void              visitEnd   (S_msrSystemLayout& elt);

    virtual void              visitStart (S_msrStaffLayout& elt);
    virtual void              visitEnd   (S_msrStaffLayout& elt);

    // appearance
    virtual void              visitStart (S_msrAppearance& elt);
    virtual void              visitEnd   (S_msrAppearance& elt);

    // credits
    virtual void              visitStart (S_msrCredit& elt);
    virtual void              visitEnd   (S_msrCredit& elt);
    virtual void              visitStart (S_msrCreditWords& elt);
    virtual void              visitEnd   (S_msrCreditWords& elt);

    // part groups
    virtual void              visitStart (S_msrPartGroup& elt);
    virtual void              visitEnd   (S_msrPartGroup& elt);

    //parts
    virtual void              visitStart (S_msrPart& elt);
    virtual void              visitEnd   (S_msrPart& elt);

    // staves
    virtual void              visitStart (S_msrStaffTuning& elt);
    virtual void              visitStart (S_msrStaffDetails& elt);

    virtual void              visitStart (S_msrStaff& elt);
    virtual void              visitEnd   (S_msrStaff& elt);

    // voices
    virtual void              visitStart (S_msrVoice& elt);
    virtual void              visitEnd   (S_msrVoice& elt);

    virtual void              visitStart (S_msrVoiceStaffChange& elt);

    // harmonies
    virtual void              visitStart (S_msrHarmony& elt);

    // frames
    virtual void              visitStart (S_msrFrame& elt);

    // figured bass
    virtual void              visitStart (S_msrFiguredBass& elt);
    virtual void              visitEnd   (S_msrFiguredBass& elt);
    virtual void              visitStart (S_msrBassFigure& elt);

    virtual void              visitStart (S_msrSegment& elt);
    virtual void              visitEnd   (S_msrSegment& elt);

    virtual void              visitStart (S_msrMeasure& elt);
    virtual void              visitEnd   (S_msrMeasure& elt);

    // lyrics
    virtual void              visitStart (S_msrStanza& elt);
    virtual void              visitEnd   (S_msrStanza& elt);

    virtual void              visitStart (S_msrSyllable& elt);
    virtual void              visitEnd   (S_msrSyllable& elt);

    // clefs
    virtual void              visitStart (S_msrClefKeyTimeSignatureGroup& elt);
    virtual void              visitEnd   (S_msrClefKeyTimeSignatureGroup& elt);

    // clefs
    virtual void              visitStart (S_msrClef& elt);
    virtual void              visitEnd   (S_msrClef& elt);

    // keys
    virtual void              visitStart (S_msrKey& elt);
    virtual void              visitEnd   (S_msrKey& elt);

    // time signatures
    virtual void              visitStart (S_msrTimeSignature& elt);
    virtual void              visitEnd   (S_msrTimeSignature& elt);

    virtual void              visitStart (S_msrTransposition& elt);
    virtual void              visitEnd   (S_msrTransposition& elt);

    // words
    virtual void              visitStart (S_msrWords& elt);
    virtual void              visitEnd   (S_msrWords& elt);

    // tempo
    virtual void              visitStart (S_msrTempo& elt);
    virtual void              visitEnd   (S_msrTempo& elt);
    virtual void              visitStart (S_msrTempoNotesRelationshipElements& elt);
    virtual void              visitEnd   (S_msrTempoNotesRelationshipElements& elt);
    virtual void              visitStart (S_msrTempoNote& elt);
    virtual void              visitStart (S_msrTempoTuplet& elt);
    virtual void              visitEnd   (S_msrTempoTuplet& elt);

    // articulations
    virtual void              visitStart (S_msrArticulation& elt);
    virtual void              visitEnd   (S_msrArticulation& elt);

    virtual void              visitStart (S_msrFermata& elt);
    virtual void              visitEnd   (S_msrFermata& elt);

    virtual void              visitStart (S_msrArpeggiato& elt);
    virtual void              visitEnd   (S_msrArpeggiato& elt);
    virtual void              visitStart (S_msrNonArpeggiato& elt);
    virtual void              visitEnd   (S_msrNonArpeggiato& elt);

    // technicals
    virtual void              visitStart (S_msrTechnical& elt);
    virtual void              visitEnd   (S_msrTechnical& elt);

    virtual void              visitStart (S_msrTechnicalWithInteger& elt);
    virtual void              visitEnd   (S_msrTechnicalWithInteger& elt);

    virtual void              visitStart (S_msrTechnicalWithFloat& elt);
    virtual void              visitEnd   (S_msrTechnicalWithFloat& elt);

    virtual void              visitStart (S_msrTechnicalWithString& elt);
    virtual void              visitEnd   (S_msrTechnicalWithString& elt);

    // ornaments
    virtual void              visitStart (S_msrOrnament& elt);
    virtual void              visitEnd   (S_msrOrnament& elt);

    // glissandos
    virtual void              visitStart (S_msrGlissando& elt);
    virtual void              visitEnd   (S_msrGlissando& elt);

    // slides
    virtual void              visitStart (S_msrSlide& elt);
    virtual void              visitEnd   (S_msrSlide& elt);

    // tremolos
    virtual void              visitStart (S_msrSingleTremolo& elt);
    virtual void              visitEnd   (S_msrSingleTremolo& elt);

    virtual void              visitStart (S_msrDoubleTremolo& elt);
    virtual void              visitEnd   (S_msrDoubleTremolo& elt);

    // dynamics
    virtual void              visitStart (S_msrDynamic& elt);
    virtual void              visitEnd   (S_msrDynamic& elt);

    virtual void              visitStart (S_msrOtherDynamic& elt);
    virtual void              visitEnd   (S_msrOtherDynamic& elt);

    virtual void              visitStart (S_msrCrescDecresc& elt);
    virtual void              visitEnd   (S_msrCrescDecresc& elt);

    virtual void              visitStart (S_msrWedge& elt);
    virtual void              visitEnd   (S_msrWedge& elt);

    // grace notes
    virtual void              visitStart (S_msrGraceNotesGroup& elt);
    virtual void              visitEnd   (S_msrGraceNotesGroup& elt);

    virtual void              visitStart (S_msrChordGraceNotesGroupLink& elt);
    virtual void              visitEnd   (S_msrChordGraceNotesGroupLink& elt);

    virtual void              visitStart (S_msrAfterGraceNotesGroup& elt);
    virtual void              visitEnd   (S_msrAfterGraceNotesGroup& elt);
    virtual void              visitStart (S_msrAfterGraceNotesGroupContents& elt);
    virtual void              visitEnd   (S_msrAfterGraceNotesGroupContents& elt);

    // notes
    virtual void              visitStart (S_msrNote& elt);
    virtual void              visitEnd   (S_msrNote& elt);

    virtual void              visitStart (S_msrOctaveShift& elt);
    virtual void              visitEnd   (S_msrOctaveShift& elt);

    // accordion registration
    virtual void              visitStart (S_msrAccordionRegistration& elt);

    // harp pedals tuning
    virtual void              visitStart (S_msrHarpPedalsTuning& elt);

    // stems
    virtual void              visitStart (S_msrStem& elt);
    virtual void              visitEnd   (S_msrStem& elt);

    // beams
    virtual void              visitStart (S_msrBeam& elt);
    virtual void              visitEnd   (S_msrBeam& elt);

    // chords
    virtual void              visitStart (S_msrChord& elt);
    virtual void              visitEnd   (S_msrChord& elt);

    // tuplets
    virtual void              visitStart (S_msrTuplet& elt);
    virtual void              visitEnd   (S_msrTuplet& elt);

    // ties
    virtual void              visitStart (S_msrTie& elt);
    virtual void              visitEnd   (S_msrTie& elt);

    // slurs
    virtual void              visitStart (S_msrSlur& elt);
    virtual void              visitEnd   (S_msrSlur& elt);
    /*
    virtual void              visitStart (S_msrChordSlurLink& elt);
    virtual void              visitEnd   (S_msrChordSlurLink& elt);
    */

    // ligatures
    virtual void              visitStart (S_msrLigature& elt);
    virtual void              visitEnd   (S_msrLigature& elt);

    // bar lines
    virtual void              visitStart (S_msrBarLine& elt);
    virtual void              visitEnd   (S_msrBarLine& elt);

    // segnos and codas
    virtual void              visitStart (S_msrSegno& elt);
    virtual void              visitStart (S_msrHiddenMeasureAndBarLine& elt);
    virtual void              visitStart (S_msrCoda& elt);

    virtual void              visitStart (S_msrEyeGlasses& elt);
    virtual void              visitStart (S_msrPedal& elt);
    virtual void              visitStart (S_msrDamp& elt);
    virtual void              visitStart (S_msrDampAll& elt);
    virtual void              visitStart (S_msrScordatura& elt);

    // bar numbers
    virtual void              visitStart (S_msrBarCheck& elt);
    virtual void              visitEnd   (S_msrBarCheck& elt);

    virtual void              visitStart (S_msrBarNumberCheck& elt);
    virtual void              visitEnd   (S_msrBarNumberCheck& elt);

    // breaks
    virtual void              visitStart (S_msrLineBreak& elt);
    virtual void              visitEnd   (S_msrLineBreak& elt);

    virtual void              visitStart (S_msrPageBreak& elt);
    virtual void              visitEnd   (S_msrPageBreak& elt);

    // repeats
    virtual void              visitStart (S_msrRepeat& elt);
    virtual void              visitEnd   (S_msrRepeat& elt);
    virtual void              visitStart (S_msrRepeatCommonPart& elt);
    virtual void              visitEnd   (S_msrRepeatCommonPart& elt);
    virtual void              visitStart (S_msrRepeatEnding& elt);
    virtual void              visitEnd   (S_msrRepeatEnding& elt);

    // measures repests
    virtual void              visitStart (S_msrMeasureRepeat& elt);
    virtual void              visitEnd   (S_msrMeasureRepeat& elt);
    virtual void              visitStart (S_msrMeasureRepeatPattern& elt);
    virtual void              visitEnd   (S_msrMeasureRepeatPattern& elt);
    virtual void              visitStart (S_msrMeasureRepeatReplicas& elt);
    virtual void              visitEnd   (S_msrMeasureRepeatReplicas& elt);

    // multiple measure rests
    virtual void              visitStart (S_msrMultipleMeasureRest& elt);
    virtual void              visitEnd   (S_msrMultipleMeasureRest& elt);

    // rehearsal marks
    virtual void              visitStart (S_msrRehearsalMark& elt);
    virtual void              visitEnd   (S_msrRehearsalMark& elt);

    // MIDI
    virtual void              visitStart (S_msrMidiTempo& elt);
    virtual void              visitEnd   (S_msrMidiTempo& elt);

  private:

    // private fields and methods
    // ------------------------------------------------------

    // spaces
    // ------------------------------------------------------

    static char constexpr cLilyPondSpace = ' ';

    std::string           cLilyPondCommaAndSpace;

    // comments
    // ------------------------------------------------------

    void                  generateComment (
                            const std::string& comment);

    void                  generateInputLineNumberAndOrPositionInMeasureAsAComment (
                            const S_msrMeasureElement& measureElement);

    // LilyPond fragments
    // ------------------------------------------------------

    void                  initializeLilypondUsefulFragments ();

    // contexts
    // ------------------------------------------------------

    std::string           lpsrContextTypeKindAsLilypondString (
                            lpsrContextTypeKind contextTypeKind);

    // names
    // ------------------------------------------------------

    std::string           nameAsLilypondString (
                            const std::string& name);

    // lengths
    // ------------------------------------------------------

    std::string           lengthUnitAsLilypondString (
                            msrLengthUnitKind lengthUnitKind);

    // whole notes
    // ------------------------------------------------------
    // JMI

    // markups
    // ------------------------------------------------------

    std::string           generateAColumnForMarkup (
                            const std::string&       theString,
                            lilypondMarkupColumnKind columnKind);
    std::string           generateMultilineMarkup (
                            const std::string&       theString,
                            lilypondMarkupColumnKind columnKind);

    // octaves
    // ------------------------------------------------------

    std::string           absoluteOctaveAsLilypondString (
                            int           inputLineNumber,
                            msrOctaveKind absoluteOctaveKind);

    // alterations
    // ------------------------------------------------------

    std::string           alterationKindAsLilypondString (
                            msrAlterationKind alterationKind);

    // durations
    // ------------------------------------------------------

    mfWholeNotes          fLastMetWholeNotes;

    Bool                  wholeNotesDurationShouldBeGenerated (
                            const mfWholeNotes& wholeNotes);

    void                  generateWholeNotesDuration (
                            int                  inputLineNumber,
                            const mfWholeNotes& wholeNotes);

    void                  generateWholeNotesDurationOnStream (
                            int                  inputLineNumber,
                            const mfWholeNotes& wholeNotes,
                            std::ostream&        os);

    std::string           durationAsLilypondStringIfItShouldBeGenerated (
                            int                  inputLineNumber,
                            const mfWholeNotes& wholeNotes);

    std::string           notesDurationKindAsLilypondString (
                            mfDurationKind notesDurationKind);

    // musically empty measures
    // ------------------------------------------------------

    void                  generateMusicallyEmptyMeasure (
                            S_msrMeasure& measure);

    // with
    // ------------------------------------------------------

    std::string           cLilypondHide;

    // global staff size
    // ------------------------------------------------------

    void                  generateGlobalStaffSize ();

    // header handling
    // ------------------------------------------------------

    Bool                  fOnGoingHeader;

    void                  generateHeader (S_lpsrHeader header);

    void                  generateHeaderIdentificationPart (
                            const S_msrIdentification& identification,
                            std::list <std::pair <std::string, std::string>>&
                                                       nameStringValuePairsList);

    void                  createLilypondHeaderStringValuePairs (
                            const S_lpsrHeader& header,
                            std::list <std::pair <std::string, std::string>>&
                                                nameStringValuePairsList);

    std::string           generateStringAsLilypondMarkupOrDoubleQuotedString (
                            const std::string& value);

    // paper handling
    // ------------------------------------------------------
    void                  generatePaper (S_lpsrPaper paper);

    void                  fetchLengthValuesFromPaperPageSize (
                            const S_lpsrPaper& paper,
                            std::list <std::pair <std::string, msrLength>>&
                                               nameLengthValuePairsList);

    void                  fetchOnOffValuesFromLpsrOptionsGroup (
                            const S_lpsrPaper& paper,
                            std::list <std::pair <std::string, Bool>>&
                                               nameBooleanValuePairsList);

    void                  generatePaperPageSize (
                            const S_msrPageLayout& pageLayout,
                            msrLengthUnitKind      defaultLengthUnit,
                            int                    fieldWidth,
                            Bool&                  outputHasBeenProduced);

    void                  generatePaperMargins (
                            const S_msrPageLayout& pageLayout,
                            msrLengthUnitKind      defaultLengthUnit,
                            int                    fieldWidth,
                            Bool&                  outputHasBeenProduced);

    void                  generatePaperIndents (
                            const S_lpsrPaper& pagePaper,
                            msrLengthUnitKind  defaultLengthUnit,
                            int                fieldWidth,
                            Bool&              outputHasBeenProduced);

    void                  generatePaperSpaces (
                            const S_lpsrPaper& pagePaper,
                            msrLengthUnitKind  defaultLengthUnit,
                            int                fieldWidth,
                            Bool&              outputHasBeenProduced);

    void                  generatePaperCounts (
                            const S_lpsrPaper& pagePaper,
                            msrLengthUnitKind  defaultLengthUnit,
                            int                fieldWidth,
                            Bool&              outputHasBeenProduced);

    void                  generatePaperBooleans (
                            const S_lpsrPaper& pagePaper,
                            int                fieldWidth,
                            Bool&              outputHasBeenProduced);

    void                  generatePaperHeadersAndFooters (
                            const S_lpsrPaper& pagePaper,
                            msrLengthUnitKind  defaultLengthUnit,
                            int                fieldWidth,
                            Bool&              outputHasBeenProduced);

    // score
    // ------------------------------------------------------

    S_msrScore            fCurrentVisitedMsrScore;

    // part groups
    // ------------------------------------------------------

 // JMI   S_msrPartGroup        fCurrentMsrPartGroupClone; // JMI v0.9.69

    // parts
    // ------------------------------------------------------

    std::string           cLilypondPartOpener,
                          cLilypondPartCloser;

    S_msrPart             fCurrentPart; // JMI jamais visitee

    // tempos
    // ------------------------------------------------------

    void                  generateTempoBeatUnitsWordsOnly (
                            const S_msrTempo& tempo);

    void                  generateTempoBeatUnitsPerMinute (
                            const S_msrTempo& tempo);

    void                  generateTempoBeatUnitsEquivalence (
                            const S_msrTempo& tempo);

    void                  generateTempoNotesRelationship (
                            const S_msrTempo& tempo);

    // clefs
    // ------------------------------------------------------

    S_msrClef             fCurrentVoiceClef;

    // keys
    // ------------------------------------------------------

    S_msrClef             fCurrentVoiceKey;

    std::string           msrModeKindAsLilypondString (
                            msrModeKind modeKind);
    // time signatures
    // ------------------------------------------------------

    S_msrTimeSignature    fCurrentVoiceTimeSignature;

    Bool                  fVoiceIsCurrentlySenzaMisura; // JMI
    Bool                  fOnGoingVoiceCadenza;

    // staves
    // ------------------------------------------------------

    std::string           cLilypondStaffBlockOpener,
                          cLilypondStaffClockCloser;

    // staves
    // ------------------------------------------------------

    std::string           cLilypondNewStaff,
                          cLilypondNewGrandStaff,
                          cLilypondNewPianoStaff,
                          cLilypondNewChoirStaff;

    std::string           cLilypondNewTabStaff,
                          cLilypondNewHarmoniesStaff,
                          cLilypondNewFiguredBassStaff,
                          cLilypondNewDrumStaff,
                          cLilypondNewRythmicStaff,
                          cLilypondNewJianpuStaff;

    // prevent clef, key and time signature from being handled twice
    Bool                  fOnGoingStaff;

    // voices
    // ------------------------------------------------------

    std::string           cLilypondUseVoiceOpener,
                          cLilypondUseVoiceCloser;

    Bool                  fOnGoingVoice;
    S_msrVoice            fCurrentVoice;

    Bool                  fOnGoingHarmoniesVoice;
    Bool                  fOnGoingFiguredBassVoice;

    // notes
    // ------------------------------------------------------

/*
  regular notes occur are those not in a grace notes group,
  and the 'regular' adjective is not used ??? JMI
*/

    msrPrintObjectKind    fCurrentNotePrinObjectKind;

    std::string           lilypondOctaveInRelativeEntryMode (
                            const S_msrNote& note);

    std::string           lilypondOctaveInFixedEntryMode (
                            const S_msrNote& note);

    std::string           notePitchAsLilypondString (
                            const S_msrNote& note);

    std::string           pitchedRestAsLilypondString (
                            const S_msrNote& note);

    // regular notes in measures
//     void                  handleRegularNoteInMeasure (
//                             const S_msrNote& regularNote);

    void                  generateRegularNoteInMeasure (
                            const S_msrNote& regularNote);

    // rests in measures
//     void                  handleRestInMeasure (
//                             const S_msrNote& rest);

    void                  generateRestInMeasure (
                            const S_msrNote& rest);

    // pitched rests in measures
    void                  generatePitchedRestInMeasure (
                            const S_msrNote& pitchedRest);

    // unpitched rests in measures
    void                  generateUnpitchedRestInMeasure (
                            const S_msrNote& unpitchedRest);

    // skips in measures
//     void                  handleSkipInMeasure (
//                             const S_msrNote& skip);

    // unpitched notes in measures
//     void                  handleUnpitchedNoteInMeasure (
//                             const S_msrNote& unpitchedNote);

    void                  generateUnpitchedNoteInMeasure (
                            const S_msrNote& note);

    // notes in grace notes groups
//     void                  handleNoteInGraceNotesGroup (
//                             const S_msrNote& note);

    // skips in grace notes groups
//     void                  handleSkipInGraceNotesGroup (
//                             const S_msrNote& note);

//     // notes in chords
//     void                  handleRegularNoteInChord (
//                             const S_msrNote& regularNote);
//
    void                  generateCodeRightBeforeNote (
                            const S_msrNote& note);

    void                  generateRegularNoteInChord (
                            const S_msrNote& note);

    void                  generateCodeRightAfterNote (
                            const S_msrNote& note);

    // regular notes in tuplets
//     void                  handleRegularNoteInTuplet (
//                             const S_msrNote& regularNote);
//
    void                  generateRegularNoteInTuplet (
                            const S_msrNote& note);

    // rests in tuplets
//     void                  handleRestInTuplet (
//                             const S_msrNote& rest);
//
//     // unpitched notes in tuplets
//     void                  handleUnpitchedNoteInTuplet (
//                             const S_msrNote& unpitchedNote);
//
//     // notes in chords in grace notes groups
//     void                  handleNoteInChordInGraceNotesGroup (
//                             const S_msrNote& regularNote);
//
//     // notes in tuplets in grace notes groups
//     void                  handleNoteInTupletInGraceNotesGroup (
//                             const S_msrNote& regularNote);
//
//     // double tremolos
//     void                  handleNoteInDoubleTremolo (
//                             const S_msrNote& regularNote);

    void                  generateStuffBeforeTheNoteIself (
                            const S_msrNote& note);

    void                  generateTheNoteItself (
                            const S_msrNote& note);

    // rests
    // ------------------------------------------------------

    // pitched notes
    // ------------------------------------------------------

    // unpitched notes
    // ------------------------------------------------------

    // unpitched rests
    // ------------------------------------------------------

    // skips
    // ------------------------------------------------------

    void                  generateSkipInMeasure (
                            const S_msrNote& note);

    // cue notes
    // ------------------------------------------------------

    void                  generateCuedNoteInMeasure (
                            const S_msrNote& note);

    // grace notes groups
    // ------------------------------------------------------

    void                  generateGraceNotesGroup (
                            const S_msrGraceNotesGroup& graceNotesGroup);

    void                  generateRegularNoteInGraceNotesGroup (
                            const S_msrNote& note);

    void                  generateSkipInGraceNotesGroup (
                            const S_msrNote& note);

    void                  generateNoteInChordInGraceNotesGroup (
                            const S_msrNote& note);

    void                  generateNoteInTupletInGraceNotesGroup (
                            const S_msrNote& note);

    Bool                  fOnGoingGraceNotesGroup;
    S_msrGraceNotesGroup  fCurrentGraceNotesGroup;

    Bool                  fOnGoingChordGraceNotesGroupLink;


    // chords
    // ------------------------------------------------------

    std::string           cLilypondChordOpener,
                          cLilypondChordCloser;

    std::list <int>       fPendingChordMemberNotesStringNumbers;

    Bool                  fOnGoingChord;
    S_msrChord            fCurrentChord;

    std::list <int>       fCurrentChordPendingSlurs;

    void                  generateChordGlissandos (
                            const std::list <S_msrGlissando>& chordGlissandosList);

    void                  generateChordSlides (
                            const std::list <S_msrSlide>& chordSlidesList);

    void                  generateChordLigatures (
                            const std::list <S_msrLigature>& chordLigaturesList);

    void                  generateChordStems (
                            const S_msrChord&            chord,
                            const std::list <S_msrStem>& chordStemsList);


    void                  generateCodeBeforeChordBegin (
                            const S_msrChord& chord);

    void                  generateCodeAfterChordEnd (
                            const S_msrChord& chord);

    void                  generateChordInGraceNotesGroupContents (
                            const S_msrChord& chord);

    void                  generateChordInGraceNotesGroup (
                            const S_msrChord& chord);


    // tuplets
    // ------------------------------------------------------

    std::string           cLilypondTupletOpener1,
                          cLilypondTupletOpener2,
                          cLilypondTupletCloser;

    std::string           tupletFactorAsLilypondString (
                            const msrTupletFactor& tupletFactor);

    void                  generateRestInTuplet (
                            const S_msrNote& note);

    void                  generateNoteUnpitchedInTuplet (
                            const S_msrNote& note);

//     std::list <S_msrTuplet>
//                           fCurrentTupletsStack;

    // tremolos
    // ------------------------------------------------------

    std::string           singleTremoloNotesDurationAsLilypondString (
                            const S_msrSingleTremolo& singleTremolo);

    void                  generateNoteInDoubleTremolo (
                            const S_msrNote& note);

    // scordaturas
    // ------------------------------------------------------

    std::string           cLilypondScordaturaOpener,
                          cLilypondScordaturaCloser;

    void                  generateNoteScordaturasList (
                            const S_msrNote& note);

    // slashed
    // ------------------------------------------------------

    void                  generateNoteSlashesList (
                            const S_msrNote& note);

    // slurs
    // ------------------------------------------------------

    void                  generateNoteSlurLineTypes (
                            const S_msrNote& note);

    // note heads
    // ------------------------------------------------------

    void                  generateNoteHeadAndStem (
                            S_msrNote note);

    void                  generateNoteHeadColor (
                            const S_msrNote& note);

    void                  generateNoteHeadSettingsBeforeNote (
                            const S_msrNote& note);
    void                  generateNoteHeadSettingsAfterNote (
                            const S_msrNote& note);

    // beams
    // ------------------------------------------------------

    void                  generateNoteBeamsAfterNote (
                            const S_msrNote& note);

    // slurs
    // ------------------------------------------------------

    void                  generateNoteSlurDirection (
                            const S_msrNote& note);
    void                  generateNoteSlursList (
                            const S_msrNote& note);

    void                  generateNoteLigaturesList (
                            const S_msrNote& note);

    // segnos
    // ------------------------------------------------------

    void                  generateSegno (
                            const S_msrSegno& segno);

    // codas
    // ------------------------------------------------------

    void                  generateCoda (
                            const S_msrCoda& coda);

    // glissandos
    // ------------------------------------------------------

    void                  generateNoteGlissandoStyles (
                            const S_msrNote& note);

    void                  generateNoteGlissandosListWithText (
                            const S_msrNote& note);

    void                  generateNoteGlissandos (
                            const std::list <S_msrGlissando>&
                              glissandosList);

    // slides
    // ------------------------------------------------------

    void                  generateNoteSlideLineStyles (
                            const S_msrNote& note);
    void                  generateNoteSlidesListWithText (
                            const S_msrNote& note);

    void                  generateNoteSlides (
                            const std::list <S_msrSlide>&
                              slidesList);

    // words
    // ------------------------------------------------------

    void                  generateNoteWordsList (
                            const std::list <S_msrWords>& noteWordsList);

    // stems
    // ------------------------------------------------------

    // the LilyPond \stem* commands have a persistent effect, hence:
    msrStemKind           fCurrentStemKind;

    std::string           stemAsLilypondString (
                            msrStemKind stemKind);

    void                  generateStemIfNeededAndUpdateCurrentStemKind (
                            const S_msrStem& stem);

    // beams
    // ------------------------------------------------------

    // JMI ???

    // slurs
    // ------------------------------------------------------

    // the LilyPond \slur* commands have a persistent effect, hence:
    msrPlacementKind      fCurrentSlurPlacementKind;

    // articulations

    msrDirectionKind      fCurrentArpeggioDirectionKind;

    void                  generateNoteArticulation (
                            const S_msrArticulation& articulation);

    void                  generateChordArticulation (
                            const S_msrArticulation& articulation);

    void                  generateArticulations (
                            const S_msrNote note);

    // technicals
    // ------------------------------------------------------

    std::string           technicalAsLilypondString (
                            const S_msrTechnical& technical);

    std::string           technicalWithIntegerAsLilypondString (
                            const S_msrTechnicalWithInteger& technicalWithInteger);

    std::string           technicalWithFloatAsLilypondString (
                            const S_msrTechnicalWithFloat& technicalWithFloat);

    std::string           technicalWithStringAsLilypondString (
                            const S_msrTechnicalWithString& technicalWithString);


    void                  generateTechnicals (
                            const std::list <S_msrTechnical>&
                              technicalsList);

    void                  generateTechnicalsWithInteger (
                            const std::list <S_msrTechnicalWithInteger>&
                              technicalWithIntegersList);

    void                  generateTechnicalsWithFloat (
                            const std::list <S_msrTechnicalWithFloat>&
                              technicalWithFloatsList);

    void                  generateNoteTechnicalsListWithStrings (
                            const S_msrNote& note);

    void                  generateTechnicalsWithStringsList (
                            const std::list <S_msrTechnicalWithString>&
                              technicalWithStringsList);

    // ornaments
    // ------------------------------------------------------

    // in LilyPond, \stopTrillSpan should apply to the note following
    // the one that holds it in MusicXML and in the MSR
    S_msrSpanner          fPendingTrillSpannerForStop;

    void                  generateOrnament (
                            const S_msrOrnament& ornament);

    // trills
    // ------------------------------------------------------

    Bool                  fOnGoingTrillSpanner;

    // spanners
    // ------------------------------------------------------

    msrPlacementKind      fCurrentSpannerPlacementKind;

    void                  generateBeforeNoteSpannersListIfAny (
                            const S_msrNote& note);

    void                  generateAfterNoteSpannersListIfAny (
                            const S_msrNote& note);

    void                  generateSpannerBeforeNote (
                            const S_msrSpanner& spanner,
                            const S_msrNote&    note);

    void                  generateSpannerAfterNote (
                            const S_msrSpanner& spanner,
                            const S_msrNote&    note);

    // dynamics
    // ------------------------------------------------------

    std::string           dynamicAsLilypondString (
                            const S_msrDynamic& dynamic);

    void                  generateDynamics (
                            const std::list <S_msrDynamic>&
                              dynamicsList);

    void                  generateOtherDynamics (
                            const std::list <S_msrOtherDynamic>&
                              otherDynamicsList);

    // string tuning
    // ------------------------------------------------------

    std::string           stringTuningAsLilypondString (
                            int                      inputLineNumber,
                            const S_msrStringTuning& stringTuning);

    // harp pedals tuning
    // ------------------------------------------------------

    std::string           harpPedalTuningAsLilypondString (
                            msrAlterationKind alterationKind);

    // transposition
    // ------------------------------------------------------

    void                  transposeDiatonicError (
                            int inputLineNumber,
                            int transposeDiatonic,
                            int transposeChromatic);

    // harmonies
    // ------------------------------------------------------

    Bool                  fPowerChordHaveAlreadyBeenGenerated;
                            // to generate it only once

    std::string           harmonyAsLilypondString (
                            const S_msrHarmony& harmony);

    std::string           harmonyDegreeAlterationKindAsLilypondString (
                            msrAlterationKind harmonyDegreeAlterationKind);

    // frames
    // ------------------------------------------------------

    std::string           frameAsLilypondString (
                            const S_msrFrame& frame);

    // figured bass
    // ------------------------------------------------------

    std::string           figureAsLilypondString (
                            const S_msrBassFigure& bassFigure);
    std::string           figuredBassAsLilypondString (
                            const S_msrFiguredBass& figuredBass);

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

    std::ostream&         fLilypondCodeStream;

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

    // book blocks
    // ------------------------------------------------------

    Bool                  fOnGoingBookBlock; // JMI

    // book part blocks
    // ------------------------------------------------------

    Bool                  fOnGoingBookPartBlock;


    // score blocks
    // ------------------------------------------------------

    Bool                  fOnGoingScoreBlock; // JMI

    // layout contexts
    // ------------------------------------------------------

    std::string           cLilypondLayoutContextScoreOpener,
                          cLilypondLayoutContextScoreCloser;

    std::string           cLilypondLayoutContextStaffOpener,
                          cLilypondLayoutContextStaffCloser;

    std::string           cLilypondLayoutContextVoiceOpener,
                          cLilypondLayoutContextVoiceCloser;

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

    // partgroups
    // ------------------------------------------------------

    std::string           cLilypondPartGroupOpener,
                          cLilypondPartGroupCloser;

    // staff groups
    // ------------------------------------------------------

    std::string           cLilypondNewStaffGroup,
                          cLilypondStaffGroupOpener,
                          cLilypondStaffGroupCloser;

    // barlines
    // ------------------------------------------------------


    // with
    // ------------------------------------------------------

    std::string           cLilypondWithOpener,
                          cLilypondWithCloser;

    // parallel music
    // ------------------------------------------------------

    std::string           cLilypondParallelMusicOpener,
                          cLilypondParallelMusicCloser;

    // ligatures
    // ------------------------------------------------------

    std::string           cLilypondLigatureOpener,
                          cLilypondLigatureCloser;

    void                  generateLigatures (
                            const std::list <S_msrLigature>&
                              ligaturesList);

    // wedges
    // ------------------------------------------------------

    void                  generateNoteWedgesList (
                            const S_msrNote& note);

    void                  generateWedges (
                            const std::list <S_msrWedge>&
                              wedgesList);

    // hairpins
    // ------------------------------------------------------

    std::string           cLilypondHairPinsCrescendo,
                          cLilypondHairPinsDecrescendo,
                          cLilypondHairPinsStop;

    // lyrics
    // ------------------------------------------------------

    std::string           cLilypondNewLyrics;

    Bool                  fOnGoingStanza;
    S_msrStanza           fCurrentStanza;

    Bool                  fOnGoingExtend;

    void                  generateSyllableDescripionAsComment (
                            const S_msrSyllable& syllable);

    void                  generateLilypondSyllable (
                            const S_msrSyllable& syllable);

    void                  generateCodeBeforeSyllableIfRelevant (
                            const S_msrSyllable& syllable);

    void                  generateCodeAfterSyllableIfRelevant (
                            const S_msrSyllable& syllable);

    void                  generateLyricExtenderAndOrSkipWithAutomaticDurations (
                            const S_msrSyllable& syllable);

    void                  generateLyricExtenderAndOrSkipWithImplicitDurations (
                            const S_msrSyllable& syllable);

    void                  generateLyricExtenderAndOrSkipWithExplicitDurations (
                            const S_msrSyllable& syllable);

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
    // ------------------------------------------------------

    std::string           msrQuarterTonesPitchesLanguageKindAsLilypondString (
                            msrQuarterTonesPitchesLanguageKind languageKind);

    // repeats
    // ------------------------------------------------------

    std::list <S_lpsrRepeat>
                          fRepeatDescrsStack;

    // multiple measure rests
    // ------------------------------------------------------

    int                   fRemainingMeasureRestsNumber;
    Bool                  fOnGoingMultipleMeasureRests;

    // segments
    // ------------------------------------------------------

    // measures
    // ------------------------------------------------------

    // we need to count the measures
    // for option fSeparatorLineEveryNMeasures and bar number checks,
    // since measure numbers can actually be any strings
    std::string           fCurrentMeasureNumber;
    int                   fCurrentVoiceMeasuresCounter;

    // notes
    // ------------------------------------------------------

    // browsing grace notes groups leads to several notes
    // being ongoing simultaneously,
    // since such groups are attached to a note, hence:
//     std::list <S_msrNote> fCurrentNotesTupletsStack; // USELESS??? JMI CLAR


    // double tremolos
    // ------------------------------------------------------

    // octave shifts
    // ------------------------------------------------------

    void                  generateOctaveShiftBeforeNote (
                            const S_msrOctaveShift& octaveShift);
    void                  generateOctaveShiftAfterNote (
                            const S_msrOctaveShift& octaveShift);

    // stanzas
    // ------------------------------------------------------

//     S_msrStanza           fCurrentStanzaClone;
    Bool                  fGenerateOngoingNonEmptyStanza;


    // syllables
    // ------------------------------------------------------

    S_msrSyllable         fCurrentMsrSyllableClone;


    // current ongoing values display
    // ------------------------------------------------------

    void                  displayCurrentOnGoingValues ();
};


}


#endif // ___lpsr2lilypondTranslator___
