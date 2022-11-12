/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2mxsrTranslator___
#define ___msr2mxsrTranslator___

#include <map>
#include <vector>

#include "visitor.h"
#include "xml.h"

#include "msrKeys.h"
#include "msrPlacements.h"
#include "msrSpanners.h"
#include "msrTupletFactors.h"



namespace MusicFormats
{

//________________________________________________________________________
class EXP msr2mxsrTranslator :

  // MSR score

  public visitor<S_msrScore>,

  // rights

  public visitor<S_msrIdentification>,

  public visitor<S_msrCredit>,
  public visitor<S_msrCreditWords>,

  // scaling

  public visitor<S_msrScaling>,

  // layout

  public visitor<S_msrPageLayout>,
  public visitor<S_msrSystemLayout>,
  public visitor<S_msrStaffLayout>,
  public visitor<S_msrMeasureLayout>,

  // appearance

  public visitor<S_msrAppearance>,

  // print layouts

  public visitor<S_msrPrintLayout>,

  // part groups

  public visitor<S_msrPartGroup>,

  // parts

  public visitor<S_msrPart>,

  // staff details

  public visitor<S_msrStaffTuning>,
  public visitor<S_msrStaffDetails>,

  public visitor<S_msrStaff>,

  // voices

  public visitor<S_msrVoice>,

  public visitor<S_msrVoiceStaffChange>,

  // segments

  public visitor<S_msrSegment>,

  // measures

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

  // tempos

  public visitor<S_msrTempo>,

  // chords

  public visitor<S_msrChord>,

  // tuplets

  public visitor<S_msrTuplet>,

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

//  public visitor<S_msrSpanner>,

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

  // ties, slurs, brackets & beams

  public visitor<S_msrTie>,

  public visitor<S_msrBeam>,

  // ?

  public visitor<S_msrBarLine>,

  public visitor<S_msrSegno>,
  public visitor<S_msrDalSegno>,
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

  // midi

  public visitor<S_msrMidiTempo>

{
  public:

                          msr2mxsrTranslator (
                            S_msrScore visitedMsrScore);

    virtual               ~msr2mxsrTranslator ();

    Sxmlelement           translateMsrToMxsr ();

  protected:

    // scores
    virtual void          visitStart (S_msrScore& elt);
    virtual void          visitEnd   (S_msrScore& elt);

    // identification
    virtual void          visitStart (S_msrIdentification& elt);
    virtual void          visitEnd   (S_msrIdentification& elt);

    // credits
    virtual void          visitStart (S_msrCredit& elt);
    virtual void          visitEnd   (S_msrCredit& elt);
    virtual void          visitStart (S_msrCreditWords& elt);
    virtual void          visitEnd   (S_msrCreditWords& elt);

    // scaling

    virtual void          visitStart (S_msrScaling& elt);
    virtual void          visitEnd   (S_msrScaling& elt);

    // layout

    virtual void          visitStart (S_msrPageLayout& elt);
    virtual void          visitEnd   (S_msrPageLayout& elt);

    virtual void          visitStart (S_msrSystemLayout& elt);
    virtual void          visitEnd   (S_msrSystemLayout& elt);

    virtual void          visitStart (S_msrStaffLayout& elt);
    virtual void          visitEnd   (S_msrStaffLayout& elt);

    virtual void          visitStart (S_msrMeasureLayout& elt);
    virtual void          visitEnd   (S_msrMeasureLayout& elt);

    // appearance

    virtual void          visitStart (S_msrAppearance& elt);
    virtual void          visitEnd   (S_msrAppearance& elt);

    // part groups
    virtual void          visitStart (S_msrPartGroup& elt);
    virtual void          visitEnd   (S_msrPartGroup& elt);

    // part
    virtual void          visitStart (S_msrPart& elt);
    virtual void          visitEnd   (S_msrPart& elt);

/*
    virtual void          visitStart (S_msrStaffTuning& elt);
    virtual void          visitStart (S_msrStaffDetails& elt);
    virtual void          visitEnd   (S_msrStaffDetails& elt);
*/

    // staves
    virtual void          visitStart (S_msrStaff& elt);
    virtual void          visitEnd   (S_msrStaff& elt);

    // voices
    virtual void          visitStart (S_msrVoice& elt);
    virtual void          visitEnd   (S_msrVoice& elt);

/*
    virtual void          visitStart (S_msrVoiceStaffChange& elt);
*/

    // segments
    virtual void          visitStart (S_msrSegment& elt);
    virtual void          visitEnd   (S_msrSegment& elt);

    // measures
    virtual void          visitStart (S_msrMeasure& elt);
    virtual void          visitEnd   (S_msrMeasure& elt);

/*
    virtual void          visitStart (S_msrStanza& elt);
    virtual void          visitEnd   (S_msrStanza& elt);

    virtual void          visitStart (S_msrSyllable& elt);
    virtual void          visitEnd   (S_msrSyllable& elt);
*/

    // clefs
    virtual void          visitStart (S_msrClef& elt);
    virtual void          visitEnd   (S_msrClef& elt);

    // keys
    virtual void          visitStart (S_msrKey& elt);
    virtual void          visitEnd   (S_msrKey& elt);

    // time signatures
    virtual void          visitStart (S_msrTimeSignature& elt);
    virtual void          visitEnd   (S_msrTimeSignature& elt);
/*

    virtual void          visitStart (S_msrTransposition& elt);
    virtual void          visitEnd   (S_msrTransposition& elt);

    virtual void          visitStart (S_msrWords& elt);
    virtual void          visitEnd   (S_msrWords& elt);
*/
    // tempos
    virtual void          visitStart (S_msrTempo& elt);
    virtual void          visitEnd   (S_msrTempo& elt);
/*
    virtual void          visitStart (S_msrRehearsalMark& elt);
    virtual void          visitEnd   (S_msrRehearsalMark& elt);
*/
    // print layouts
    virtual void          visitStart (S_msrPrintLayout& elt);
    virtual void          visitEnd   (S_msrPrintLayout& elt);

    // chords
    virtual void          visitStart (S_msrChord& elt);
    virtual void          visitEnd   (S_msrChord& elt);

    // tuplets
    virtual void          visitStart (S_msrTuplet& elt);
    virtual void          visitEnd   (S_msrTuplet& elt);

/*
    virtual void          visitStart (S_msrHarmony& elt);
    virtual void          visitEnd   (S_msrHarmony& elt);
    virtual void          visitStart (S_msrHarmonyDegree& elt);

    virtual void          visitStart (S_msrFrame& elt);

    virtual void          visitStart (S_msrFiguredBass& elt);
    virtual void          visitEnd   (S_msrFiguredBass& elt);
    virtual void          visitStart (S_msrBassFigure& elt);
*/

/*
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
*/

    virtual void          visitStart (S_msrGraceNotesGroup& elt);
    virtual void          visitEnd   (S_msrGraceNotesGroup& elt);

    virtual void          visitStart (S_msrNote& elt);
    virtual void          visitEnd   (S_msrNote& elt);
/*
    virtual void          visitStart (S_msrOctaveShift& elt);
    virtual void          visitEnd   (S_msrOctaveShift& elt);

    virtual void          visitStart (S_msrAccordionRegistration& elt);

    virtual void          visitStart (S_msrHarpPedalsTuning& elt);

    virtual void          visitStart (S_msrStem& elt);
    virtual void          visitEnd   (S_msrStem& elt);

    virtual void          visitStart (S_msrBeam& elt);
    virtual void          visitEnd   (S_msrBeam& elt);

    virtual void          visitStart (S_msrTie& elt);
    virtual void          visitEnd   (S_msrTie& elt);

    virtual void          visitStart (S_msrSlur& elt);
    virtual void          visitEnd   (S_msrSlur& elt);

    virtual void          visitStart (S_msrLigature& elt);
    virtual void          visitEnd   (S_msrLigature& elt);
*/
    virtual void          visitStart (S_msrBarLine& elt);
    virtual void          visitEnd   (S_msrBarLine& elt);
/*
    virtual void          visitStart (S_msrSegno& elt);
    virtual void          visitStart (S_msrDalSegno& elt);
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

    virtual void          visitStart (S_msrPageBreak& elt);
    virtual void          visitEnd   (S_msrPageBreak& elt);

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

    virtual void          visitStart (S_msrMultipleFullBarRests& elt);
    virtual void          visitEnd   (S_msrMultipleFullBarRests& elt);
*/

/*
    virtual void          visitStart (S_msrMidiTempo& elt);
    virtual void          visitEnd   (S_msrMidiTempo& elt);
*/

  private:

    // history
    // ------------------------------------------------------
    std::string               fTranslatorName;


    // the MSR score we're visiting
    // ------------------------------------------------------
    S_msrScore                fVisitedMsrScore;


    // the MXSR we're building
    // ------------------------------------------------------
    Sxmlelement               fResultingMusicxmlelement;


    // scaling
    // ------------------------------------------------------
    float                     fMillimeters;
    float                     fTenths;

    Sxmlelement               fScoreDefaultsScalingElement;


    // work
    // ------------------------------------------------------

    Sxmlelement               fScoreWorkElement;

    void                      appendToScoreWork (
                                Sxmlelement elem);


    // movement number
    // ------------------------------------------------------

    Sxmlelement               fScoreMovementNumberElement;


    // movement title
    // ------------------------------------------------------

    Sxmlelement               fScoreMovementTitleElement;


    // identification
    // ------------------------------------------------------

    Sxmlelement               fScoreIdentificationElement;

/*
<!ELEMENT identification (creator*, rights*, encoding?,
  source?, relation*, miscellaneous?)>
*/

    // identification creator
    std::list<Sxmlelement>         fComposersElementsList;
    std::list<Sxmlelement>         fLyricistsElementsList;
    std::list<Sxmlelement>         fArrangersElementsList;

    std::list<Sxmlelement>         fPoetsElementsList;
    std::list<Sxmlelement>         fTranslatorsElementsList;
    std::list<Sxmlelement>         fArtistsElementsList;

    std::list<Sxmlelement>         fSoftwaresElementsList;

    // identification rights
    std::list<Sxmlelement>         fRightsElementsList;


    // identification encoding
    Sxmlelement               fScoreIdentificationEncodingElement;

/* JMI to be handled
<!ELEMENT encoding ((encoding-date | encoder |
  encoding-description | supports)*)>
<!ELEMENT encoding-date %yyyy-mm-dd;>
<!ELEMENT encoder (#PCDATA)>
<!ATTLIST encoder
    type CDATA #IMPLIED
>
<!ELEMENT encoding-description (#PCDATA)>
<!ELEMENT supports EMPTY>
<!ATTLIST supports
    type %yes-no; #REQUIRED
    element CDATA #REQUIRED
    attribute CDATA #IMPLIED
    value CDATA #IMPLIED
*/

    // identification source
    Sxmlelement               fScoreIdentificationSourceElement;

    // identification relation
    Sxmlelement               fScoreIdentificationRelationElement;

    // identification miscellaneous
    Sxmlelement               fIdentificationMiscellaneousElement;

    void                      appendToScoreIdentification (
                                Sxmlelement elem);

    void                      appendToScoreIdentificationEncoding (
                                Sxmlelement elem);

    // defaults
    // ------------------------------------------------------

    Sxmlelement               fScoreDefaultsElement;

    // layout
    // ------------------------------------------------------

    // page layout
    Sxmlelement               fScoreDefaultsPageLayoutElement;

    void                      appendToScoreDefaults (
                                Sxmlelement elem);

    void                      appendToScoreDefaultsPageLayout (
                                Sxmlelement elem);

    void                      appendPageMarginsToScoreDefaultsPageLayout (
                                S_msrMarginsGroup marginsGroup);

    void                      populatePageMargins (
                                Sxmlelement       elem,
                                S_msrMarginsGroup marginsGroup);

    // system layout
    Sxmlelement               fScoreDefaultsSystemLayoutElement;

    void                      appendSystemMarginsToScoreDefaultsSystemLayout (
                                S_msrSystemLayout systemLayout);

    void                      populateSystemMargins (
                                Sxmlelement       elem,
                                S_msrSystemLayout systemLayout);

    // staff layouts
    std::list<Sxmlelement>         fScoreDefaultsStaffLayoutElementsList;

    // measure print layout
    Sxmlelement               fCurrentMeasurePrintLayoutElement; // not used JMI

    // appearance
    // ------------------------------------------------------

    Sxmlelement               fScoreDefaultsAppearanceElement;

  void                        populateAppearanceLineWidths (
                                Sxmlelement                 appearanceElement,
                                const std::list<S_msrLineWidth>& lineWidthsList);

  void                        populateAppearanceNoteSizes (
                                Sxmlelement                appearanceElement,
                                const std::list<S_msrNoteSize>& noteSizesList);

  void                        populateAppearanceDistances (
                                Sxmlelement                appearanceElement,
                                const std::list<S_msrDistance>& distancesList);

  void                        populateAppearanceGlyphs (
                                Sxmlelement             appearanceElement,
                                const std::list<S_msrGlyph>& glyphsList);

  void                        populateAppearanceOtherAppearances (
                                Sxmlelement       appearanceElement,
                                const std::list<S_msrOtherAppearance>&
                                                  otherAppearancesList);

    // credits
    // ------------------------------------------------------

    Sxmlelement               fCurrentScoreCreditElement;

    std::list<Sxmlelement>         fPendingScoreCreditElementsList;

    // part std::list
    // ------------------------------------------------------

    Sxmlelement               fScorePartListElement;
    Sxmlelement               fScorePartElement;
    Sxmlelement               fScoreInstrumentElement;


    // the part direction element
    // ------------------------------------------------------

    void                      appendToMeasureDirection (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // divisions
    // ------------------------------------------------------
    Rational                  fPartShortestNoteDuration;
    msrTupletFactor           fPartShortestNoteTupletFactor;

    int                       fDivisionsPerQuarterNote;
    int                       fDivisionsMultiplyingFactor;

    int                       wholeNotesAsDivisions (
                                int             inputLineNumber,
                                const Rational& wholeNotes);

    Bool                      fPartDivisionsElementHasToBeAppended;

    // its header
    // ------------------------------------------------------


    // score
    // ------------------------------------------------------


    // header
    // ------------------------------------------------------


    // part groups
    // ------------------------------------------------------

    // partGroup's can be nested, hence this std::stack
    // the current partGroup is the top of the std::stack
    std::stack<Sxmlelement>        fPartGroupElementsStack;

    // parts
    // ------------------------------------------------------

    S_msrPart                 fCurrentMSRPart;

    Sxmlelement               fCurrentPartElement;

    std::list<Sxmlelement>         fPendingPartElementsList;

/*
    // staff details
    // ------------------------------------------------------

    // staves
    // ------------------------------------------------------

    // voices
    // ------------------------------------------------------

    // dal segnos
    // ------------------------------------------------------

    // JMI ???

    // harmonies
    // ------------------------------------------------------

    // figured bass
    // ------------------------------------------------------

    // segments
    // ------------------------------------------------------
*/

    // measures
    // ------------------------------------------------------
    Sxmlelement               fCurrentMeasureElement;

    std::map<std::string, Sxmlelement>  fPartMeasureNumbersToElementsMap;

    void                      appendOtherToMeasure (
                                Sxmlelement elem);

    // the measure attributes element
    // ------------------------------------------------------

    // <staves /> should be placed in <attributes />
    // between <time /> and <clef />, which may be absent:
    // we must thus delay the insertion of the elements
    // to append them in the right order
    Bool                      fAnAttributeElementIsNeeded;

    Sxmlelement               fDivisionsElement;

    Sxmlelement               fKeyElement;
    Sxmlelement               fTimeElement;

    Sxmlelement               fStavesElement;

    std::list<Sxmlelement>         fClefElementsList;

    void                      createMxmlAttributesElementAndAppendItToMeasure ();

/*
    // multiple full-bar rests compression
    S_msrMeasure              fCurrentRestMeasure;
    S_msrMultipleFullBarRests         fCurrentMultipleFullBarRests;

    // bar checks
    // ------------------------------------------------------
*/


    // clefs
    // ------------------------------------------------------

    S_msrClef                 fCurrentPartClef;

    // keys
    // ------------------------------------------------------

    S_msrKey                  fCurrentPartKey;

    std::string               msrModeKindAsMusicXMLString (
                                msrModeKind modeKind);

    // time signatures
    // ------------------------------------------------------

    S_msrTimeSignature        fCurrentPartTimeSignature;

    // print layouts
    // ------------------------------------------------------

    Sxmlelement               fCurrentPrintElement;

    Bool                      fOnGoingPrintLayout;

    // backup and forward
    // ------------------------------------------------------

    Rational                  fCurrentMeasurePosition;

    Rational                  fCurrentCumulatedSkipsDurations;
    int                       fCurrentCumulatedSkipsStaffNumber;
    int                       fCurrentCumulatedSkipsVoiceNumber;

    void                      appendABackupOrForwardToMeasureIfNeeded (
                                S_msrNote theMsrNote);
    void                      appendABackupToMeasure (
                                S_msrNote theMsrNote);
    void                      appendAForwardToMeasure (
                                S_msrNote theMsrNote);

    // directions
    // ------------------------------------------------------
    void                      populateNoteDirections (
                                S_msrNote theMsrNote);
    void                      appendNoteWedges (
                                S_msrNote theMsrNote);
    void                      appendNoteDynamics (
                                S_msrNote theMsrNote);

    // spanners
    // ------------------------------------------------------
    void                      appendNoteSpannersBeforeNote (
                                S_msrNote theMsrNote);
    void                      appendNoteSpannersAfterNote (
                                S_msrNote theMsrNote);

    void                      appendBasicsToNote (
                                S_msrNote theMsrNote);

    // staves
    // ------------------------------------------------------
    void                      appendStaffToNoteIfRelevant (
                                S_msrNote theMsrNote);

    // voice
    // ------------------------------------------------------
    void                      appendVoiceToNoteIfRelevant (
                                S_msrNote theMsrNote);

    // duration
    // ------------------------------------------------------
    void                      appendDurationToNoteIfRelevant (
                                S_msrNote theMsrNote);

    // tuplets
    // ------------------------------------------------------
    void                      appendTimeModificationToNoteIfRelevant (
                                S_msrNote theMsrNote);

    void                      appendNoteTupletIfRelevant (
                                S_msrNote theMsrNote);

    // notes
    // ------------------------------------------------------
    Sxmlelement               fCurrentNoteElement;
    Bool                      fANoteHasBeenMetInCurrentMeasure;

    S_msrNote                 fPreviousMSRNote;
    S_msrVoice                fPreviousMSRNoteVoice;
    S_msrStaff                fPreviousMSRNoteStaff;

    void                      appendMsrNoteToMesureIfRelevant (
                                S_msrNote theMsrNote);

    void                      appendNoteToMeasure (
                                Sxmlelement note,
                                S_msrNote   theMsrNote);

    Bool                      fCurrentNoteAwaitsGraceNotes;
    S_msrNote                 fPendingMsrNoteAwaitingGraceNotes;
    Sxmlelement               fPendingNoteElement;

    // stems
    // ------------------------------------------------------
    void                      appendStemToNote (S_msrNote theMsrNote);

    // beams
    // ------------------------------------------------------
    void                      appendBeamsToNote (S_msrNote theMsrNote);

    // note notations
    // ------------------------------------------------------
    Sxmlelement               fCurrentNoteNotationsElement;

    void                      appendNoteNotationsToNote (S_msrNote theMsrNote);

    void                      appendToNoteNotations (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // lyrics
    // ------------------------------------------------------
    void                      appendNoteLyricsToNote (S_msrNote theMsrNote);

    // note notations ornaments
    // ------------------------------------------------------
    Sxmlelement               fCurrentNoteNotationsOrnamentsElement;

    void                      appendNoteOrnaments (S_msrNote theMsrNote);

    void                      appendToNoteNotationsOrnaments (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // note notations articulations
    // ------------------------------------------------------
    Sxmlelement               fCurrentNoteNotationsArticulationsElement;

    void                      appendNoteArticulations (S_msrNote theMsrNote);

    void                      appendToNoteNotationsArticulations (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // note notations technicals
    // ------------------------------------------------------

    Sxmlelement               fCurrentNoteNotationsTechnicalsElement;

    void                      appendNoteTechnicals (S_msrNote theMsrNote);
    void                      appendNoteTechnicalWithIntegers (S_msrNote theMsrNote);
    void                      appendNoteTechnicalWithFloats (S_msrNote theMsrNote);
    void                      appendNoteTechnicalWithStrings (S_msrNote theMsrNote);

    void                      appendToNoteNotationsTechnicals (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // note ties
    // ------------------------------------------------------

    void                      appendNoteTieIfAny (S_msrNote theMsrNote);

    //note slurs
    // ------------------------------------------------------

    void                      appendNoteSlursIfAny (S_msrNote theMsrNote);

/*
    // glissandos
    // ------------------------------------------------------


    // slides
    // ------------------------------------------------------


    // double tremolos
    // ------------------------------------------------------


    // stems
    // ------------------------------------------------------


    // grace notes
    // ------------------------------------------------------

    // afterGraceNotes optimisation
    S_msrAfterGraceNotesGroup fPendingAfterGraceNotesGroup;
    S_msrElement              fCurrentAfterGraceNotesGroupElement;
*/

    // chords
    // ------------------------------------------------------
    Bool                      fOnGoingChord;
    Sxmlelement               fPendingChordStartCommentElement;

/*
    // stanzas
    // ------------------------------------------------------


    // syllables
    // ------------------------------------------------------


    // current ongoing values display
    // ------------------------------------------------------
    void                      displayCurrentOnGoingValues ();
*/

    std::string               msrLengthAsTenths (
                                msrLength length);
    std::string               S_msrLengthAsTenths (
                                S_msrLength length);

    std::string               msrPlacementKindAsMusicXMLString (
                                msrPlacementKind placementKind);

    std::string               msrSpannerTypeKindAsMusicXMLString (
                                msrSpannerTypeKind spannerTypeKind);
};


}


#endif
