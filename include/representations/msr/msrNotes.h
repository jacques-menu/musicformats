/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrNotes___
#define ___msrNotes___

#include "msrTypesForwardDeclarations.h"

#include "msrNotesEnumTypes.h"

#include "msrAccidentals.h"
#include "msrArticulations.h"
#include "msrBeams.h"
#include "msrCodas.h"
#include "msrChords.h"
#include "msrColors.h"
#include "msrDynamics.h"
#include "msrEyeGlasses.h"
#include "msrFiguredBasses.h"
#include "msrGlissandos.h"
#include "msrHarmonies.h"
#include "msrInstruments.h"
#include "msrLyrics.h"
#include "msrGraceNotesGroups.h"
#include "msrLigatures.h"
#include "msrMeasures.h"
#include "msrOrnaments.h"
#include "msrParts.h"
#include "msrPrintObjects.h"
#include "msrSegnos.h"
#include "msrSingleTremolos.h"
#include "msrSlashes.h"
#include "msrSlides.h"
#include "msrSlurs.h"
#include "msrSpanners.h"
#include "msrStems.h"
#include "msrTechnicals.h"
#include "msrTies.h"
#include "msrTupletElementsAndTuplets.h"
#include "msrTupletElements.h"
#include "msrTupletFactors.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrNote : public msrTupletElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNote> create (
                            int                        inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,

//                             const std::string&         noteMeasureNumber, JMI v0.9.66

                            msrNoteKind                noteKind,

                            msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
                            msrOctaveKind              noteOctaveKind,

                            const msrWholeNotes&       noteSoundingWholeNotes,
                            const msrWholeNotes&       noteDisplayWholeNotes,

                            int                        noteDotsNumber,

                            msrNotesDurationKind       noteGraphicNotesDurationKind,

                            msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
                            msrOctaveKind              noteDisplayOctaveKind,

                            msrNoteIsACueNoteKind      msrNoteIsACueNoteKind,

                            msrPrintObjectKind         notePrintObjectKind,

                            msrNoteHeadKind            noteHeadKind,
                            msrNoteHeadFilledKind      msrNoteHeadFilledKind,
                            msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind);

    SMARTP<msrNote> createNoteNewbornClone (
                            const S_msrPart& containingPart);

    SMARTP<msrNote> createNoteDeepClone (
                            const S_msrVoice& containingVoice);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrNote> createRestNote (
                            int                  inputLineNumber,
                            const std::string&   noteMeasureNumber,
                            const msrWholeNotes& soundingWholeNotes,
                            const msrWholeNotes& displayWholeNotes,
                            int                  dotsNumber);

    static SMARTP<msrNote> createRestNoteWithOctave (
                            int                  inputLineNumber,
                            const std::string&   noteMeasureNumber,
                            msrOctaveKind        noteOctave,
                            const msrWholeNotes& soundingWholeNotes,
                            const msrWholeNotes& displayWholeNotes,
                            int                  dotsNumber);

    static SMARTP<msrNote> createSkipNote (
                            int                  inputLineNumber,
                            const std::string&   noteMeasureNumber,
                            const msrWholeNotes& soundingWholeNotes,
                            const msrWholeNotes& displayWholeNotes,
                            int                  dotsNumberr);

    static SMARTP<msrNote> createSkipNoteWithOctave (
                            int                  inputLineNumber,
                            const std::string&   noteMeasureNumber,
                            msrOctaveKind        noteOctave,
                            const msrWholeNotes& soundingWholeNotes,
                            const msrWholeNotes& displayWholeNotes,
                            int                  dotsNumber);

    static SMARTP<msrNote> createGraceSkipNote (
                            int                  inputLineNumber,
                            const std::string&   noteMeasureNumber,
                            const msrWholeNotes& soundingWholeNotes,
                            const msrWholeNotes& displayWholeNotes,
                            int                  dotsNumber);

    static SMARTP<msrNote> createRegularNote (
                            int                      inputLineNumber,
                            const std::string&       noteMeasureNumber,
                            msrQuarterTonesPitchKind quarterTonesPitchKind,
                            msrOctaveKind            noteOctaveKind,
                            const msrWholeNotes&     soundingWholeNotes,
                            const msrWholeNotes&     displayWholeNotes,
                            int                      dotsNumber);

    static SMARTP<msrNote> createRestFromString (
                            int                inputLineNumber,
                            const std::string& restString,
                            const std::string& restMeasureNumber);

    static SMARTP<msrNote> createSkipFromString (
                            int                inputLineNumber,
                            const std::string& skipString,
                            const std::string& skipMeasureNumber);

    static SMARTP<msrNote> createNoteFromString (
                            int                inputLineNumber,
                            const std::string& noteString,
                            const std::string& noteMeasureNumber);

    // creation from the options
    // ------------------------------------------------------

    static SMARTP<msrNote> createNoteFromSemiTonesPitchAndOctave (
                            int                                 inputLineNumber,
                            const S_msrSemiTonesPitchAndOctave& semiTonesPitchAndOctave);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrNote (
                            int                        inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,

//                             const std::string&         noteMeasureNumber, JMI v0.9.66

                            msrNoteKind                noteKind,

                            msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
                            msrOctaveKind              noteOctaveKind,

                            const msrWholeNotes&       noteSoundingWholeNotes,
                            const msrWholeNotes&       noteDisplayWholeNotes,

                            int                        noteDotsNumber,

                            msrNotesDurationKind       noteGraphicNotesDurationKind,

                            msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
                            msrOctaveKind              noteDisplayOctaveKind,

                            msrNoteIsACueNoteKind      msrNoteIsACueNoteKind,

                            msrPrintObjectKind         notePrintObjectKind,

                            msrNoteHeadKind            noteHeadKind,
                            msrNoteHeadFilledKind      msrNoteHeadFilledKind,
                            msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind);

    virtual               ~msrNote ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeNote ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override;

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override;

    void                  setNoteUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getNoteUpLinkToMeasure () const;

    // uplink to chord
    void                  setNoteShortcutUpLinkToChord (
                            const S_msrChord& chord)
                              { fNoteShortcutUpLinkToChord = chord; }

    S_msrChord            getNoteShortcutUpLinkToChord () const
                              { return fNoteShortcutUpLinkToChord; }

    // uplink to grace notes group
    void                  setNoteShortcutUpLinkToGraceNotesGroup (
                            const S_msrGraceNotesGroup& graceNotesGroup)
                              { fNoteShortcutUpLinkToGraceNotesGroup = graceNotesGroup; }

    S_msrGraceNotesGroup  getNoteShortcutUpLinkToGraceNotesGroup () const
                              { return fNoteShortcutUpLinkToGraceNotesGroup; }

    // uplink to tuplet
    void                  setNoteShortcutUpLinkToTuplet (
                            const S_msrTuplet& tuplet)
                              { fNoteShortcutUpLinkToTuplet = tuplet; }

    S_msrTuplet           getNoteShortcutUpLinkToTuplet () const
                              { return fNoteShortcutUpLinkToTuplet; }

    // measure positions
    // this override calls setNoteAttachedElementsMeasurePosition()
    void                  setMeasurePosition (
                            const S_msrMeasure&  measure,
                            const msrWholeNotes& measurePosition,
                            const std::string&   context) override;

    void                  setNoteAttachedElementsMeasurePosition (
                            const S_msrMeasure&  measure,
                            const msrWholeNotes& measurePosition);

    // note kind

    void                  setNoteKind (msrNoteKind noteKind);

    msrNoteKind           getNoteKind () const
                              { return fNoteKind; }

    // note pitch kind

    msrQuarterTonesPitchKind
                          getNoteQuarterTonesPitchKind () const
                              { return fNoteQuarterTonesPitchKind; }

    // note octave kind
    msrOctaveKind         getNoteOctaveKind () const
                              { return fNoteOctaveKind; }

    // note print object kind

    void                  setNotePrintObjectKind (
                            msrPrintObjectKind notePrintObjectKind)
                              { fNotePrintObjectKind = notePrintObjectKind; }

    msrPrintObjectKind    getNotePrintObjectKind () const
                              { return fNotePrintObjectKind; }

    // note head

    void                  setNoteHeadKind (
                            msrNoteHeadKind noteHeadKind)
                              { fNoteHeadKind = noteHeadKind; }

    msrNoteHeadKind       getNoteHeadKind () const
                              { return fNoteHeadKind; }

    void                  setNoteHeadFilledKind (
                            msrNoteHeadFilledKind
                              msrNoteHeadFilledKind)
                              { fNoteHeadFilledKind = msrNoteHeadFilledKind; }

    msrNoteHeadFilledKind getNoteHeadFilledKind () const
                              { return fNoteHeadFilledKind; }

    void                  setNoteHeadParenthesesKind (
                            msrNoteHeadParenthesesKind
                              msrNoteHeadParenthesesKind)
                              {
                                fNoteHeadParenthesesKind =
                                  msrNoteHeadParenthesesKind;
                              }

    msrNoteHeadParenthesesKind
                          getNoteHeadParenthesesKind () const
                              { return fNoteHeadParenthesesKind; }

    // whole notes
    void                  setNoteSoundingWholeNotes (
                            const msrWholeNotes& wholeNotes,
                            const std::string&   context)
                              { setSoundingWholeNotes (wholeNotes, context); }

    void                  setNoteDisplayWholeNotes (
                            const msrWholeNotes& wholeNotes)
                              { fNoteDisplayWholeNotes = wholeNotes; }

    msrWholeNotes         getNoteDisplayWholeNotes () const
                              { return fNoteDisplayWholeNotes; }

    // accidentals

    void                  setNoteAccidentalKind (
                            msrAccidentalKind accidentalKind)
                              { fNoteAccidentalKind = accidentalKind; }

    msrAccidentalKind     getNoteAccidentalKind () const
                              { return fNoteAccidentalKind; }

    // editorial accidentals

    void                  setNoteEditorialAccidentalKind (
                            msrEditorialAccidentalKind editorialAccidentalKind)
                              {
                                fNoteEditorialAccidentalKind =
                                  editorialAccidentalKind;
                              }

    msrEditorialAccidentalKind
                          getNoteEditorialAccidentalKind () const
                              { return fNoteEditorialAccidentalKind; }

    // cautionary accidentals

    void                  setNoteCautionaryAccidentalKind (
                            msrCautionaryAccidentalKind cautionaryAccidentalKind)
                              {
                                fNoteCautionaryAccidentalKind =
                                  cautionaryAccidentalKind;
                              }

    msrCautionaryAccidentalKind
                          getNoteCautionaryAccidentalKind () const
                              { return fNoteCautionaryAccidentalKind; }

    // note display

    msrQuarterTonesPitchKind
                          getNoteQuarterTonesDisplayPitchKind () const
                              { return fNoteQuarterTonesDisplayPitchKind; }

    // dots

    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }

    msrNotesDurationKind  getNoteGraphicNotesDurationKind () const
                              { return fNoteGraphicNotesDurationKind; }

    // display octave

    msrOctaveKind         getNoteDisplayOctaveKind () const
                              { return fNoteDisplayOctaveKind; }

    // tuplet factor

    void                  setNoteTupletFactor (
                            const msrTupletFactor& tupletFactor)
                              { fNoteTupletFactor = tupletFactor; }

    const msrTupletFactor&
                          getNoteTupletFactor () const
                              { return fNoteTupletFactor; }

    // rest?
    Bool                  fetchNoteIsARest () const
                            // shortcut for efficiency
                              {
                                return
                                  fNoteKind == msrNoteKind::kNoteRestInMeasure
                                    ||
                                  fNoteKind == msrNoteKind::kNoteRestInTuplet;
                              }

    Bool                  fetchNoteIsAFullNoteRest () const
                              {
                                return
                                  fNoteKind == msrNoteKind::kNoteRestInMeasure
                                    &&
                                  fNoteOccupiesAFullMeasure;
                              }

    // unpitched?
    Bool                  fetchNoteIsUnpitched () const
                              {
                                return
                                  fNoteKind ==
                                    msrNoteKind::kNoteUnpitchedInMeasure
                                    ||
                                  fNoteKind ==
                                    msrNoteKind::kNoteUnpitchedInTuplet;
                              }

    // cue note?
    void                  setNoteIsACueNoteKind (
                            msrNoteIsACueNoteKind msrNoteIsACueNoteKind)
                              { fNoteIsACueNoteKind = msrNoteIsACueNoteKind; }

    msrNoteIsACueNoteKind getNoteIsACueNoteKind () const
                              { return fNoteIsACueNoteKind; }

    // octave shifts
    void                  setNoteOctaveShift (
                            const S_msrOctaveShift& octaveShift)
                              { fNoteOctaveShift = octaveShift; }

    S_msrOctaveShift      getNoteOctaveShift () const
                              { return fNoteOctaveShift; }

    // note context
    // -------------------------------

    // chord members
    void                  setNoteBelongsToAChord ();

    Bool                  getNoteBelongsToAChord () const
                              { return fNoteBelongsToAChord; }

    // grace notes
    Bool                  getNoteIsAGraceNote () const
                              {
                                return
                                  fNoteKind ==
                                    msrNoteKind::kNoteRegularInGraceNotesGroup
                                    ||
                                  fNoteKind ==
                                    msrNoteKind::kNoteSkipInGraceNotesGroup
                                    ||
                                  fNoteKind ==
                                    msrNoteKind::kNoteInChordInGraceNotesGroup
                                    ||
                                  fNoteKind ==
                                    msrNoteKind::kNoteInTupletInGraceNotesGroup;
                              }

    // harmonies
    void                  appendHarmonyToNote (
                            const S_msrHarmony& harmony);

    const std::list<S_msrHarmony>&
                          getNoteHarmoniesList () const
                              { return fNoteHarmoniesList; }

    // figured bass
//     void                  appendFiguredBassToNote (
//                             const S_msrFiguredBass& figuredBass);

    const std::list<S_msrFiguredBass>&
                          getNoteFiguredBassesList () const
                              { return fNoteFiguredBassesList; }

    // note lyrics
    // -------------------------------

    std::list<S_msrSyllable>
                          getNoteSyllables () const
                              { return fNoteSyllables; }

    // elements attached to the note
    // -------------------------------

    // stems
    void                  setNoteStem (const S_msrStem& stem);

    S_msrStem             getNoteStem () const
                              { return fNoteStem; }

    // beams
    const std::list<S_msrBeam>&
                          getNoteBeams () const
                              { return fNoteBeams; }

    // articulations
    const std::list<S_msrArticulation>&
                          getNoteArticulations () const
                              { return fNoteArticulations; }

    std::list<S_msrArticulation>&
                          getNoteArticulationsNonConst ()
                              { return fNoteArticulations; }

    // spanners
    const std::list<S_msrSpanner>&
                          getNoteSpanners () const
                              { return fNoteSpanners; }

    std::list<S_msrSpanner>&
                          getNoteSpannersNonConst ()
                              { return fNoteSpanners; }

    // technicals
    const std::list<S_msrTechnical>&
                          getNoteTechnicals () const
                              { return fNoteTechnicals; }

    const std::list<S_msrTechnicalWithInteger>&
                          getNoteTechnicalWithIntegers () const
                              { return fNoteTechnicalWithIntegers; }

    const std::list<S_msrTechnicalWithFloat>&
                          getNoteTechnicalWithFloats () const
                              { return fNoteTechnicalWithFloats; }

    const std::list<S_msrTechnicalWithString>&
                          getNoteTechnicalWithStrings () const
                              { return fNoteTechnicalWithStrings; }

    // ornaments
    const std::list<S_msrOrnament>&
                          getNoteOrnaments () const
                              { return fNoteOrnaments; }

    // glissandos
    const std::list<S_msrGlissando>&
                          getNoteGlissandos () const
                              { return fNoteGlissandos; }

    // slides
    const std::list<S_msrSlide>&
                          getNoteSlides () const
                              { return fNoteSlides; }

    // grace notes
    void                  setNoteGraceNotesGroupBefore (
                            const S_msrGraceNotesGroup& graceNotesGroupBefore);

    S_msrGraceNotesGroup  getNoteGraceNotesGroupBefore () const
                              { return fNoteGraceNotesGroupBefore; }

    void                  setNoteGraceNotesGroupAfter (
                            const S_msrGraceNotesGroup& graceNotesGroupAfter);

    S_msrGraceNotesGroup  getNoteGraceNotesGroupAfter () const
                              { return fNoteGraceNotesGroupAfter; }

    // after grace notes
    /* JMI
    void                  setNoteAfterGraceNotesGroup (
                            const S_msrAfterGraceNotes&Group afterGraceNotesGroup);

    S_msrAfterGraceNotesGroup
                          getNoteAfterGraceNotesGroup () const
                              { return fNoteAfterGraceNotesGroup; }
                            */

    // singleTremolo
    void                  setNoteSingleTremolo (
                            const S_msrSingleTremolo& trem);

    S_msrSingleTremolo    getNoteSingleTremolo () const
                              { return fNoteSingleTremolo; }

    // ties
    void                  setNoteTie (const S_msrTie& tie)
                              { fNoteTie = tie; }
    S_msrTie              getNoteTie () const
                              { return fNoteTie; }

    // dynamics
    const std::list<S_msrDynamic>&
                          getNoteDynamics () const
                              { return fNoteDynamics; }
    const std::list<S_msrOtherDynamic>&
                          getNoteOtherDynamics () const
                              { return fNoteOtherDynamics; }

    // words
    const std::list<S_msrWords>&
                          getNoteWords () const
                              { return fNoteWords; }

    std::list<S_msrWords>&     getNoteWordsNonConst ()
                              { return fNoteWords; }

    // slashes
    const std::list<S_msrSlash>&
                          getNoteSlashes () const
                              { return fNoteSlashes; }

    // wedges
    const std::list<S_msrCrescDecresc>&
                          getNoteCrescDecrescs () const
                              { return fNoteCrescDecrescs; }

    // wedges
    const std::list<S_msrWedge>&
                          getNoteWedges () const
                              { return fNoteWedges; }

    std::list<S_msrWedge>&
                          getNoteWedgesNonConst ()
                              { return fNoteWedges; }

    // segnos
    const std::list<S_msrSegno>&
                          getNoteSegnos () const
                              { return fNoteSegnos; }

    // dal segnos
    const std::list<S_msrDalSegno>&
                          getNoteDalSegnos () const
                              { return fNoteDalSegnos; }

    // coda
    const std::list<S_msrCoda>&
                          getNoteCodas () const
                              { return fNoteCodas; }

    // eyeglasses
    const std::list<S_msrEyeGlasses>&
                          getNoteEyeGlasses () const
                              { return fNoteEyeGlasses; }

    // damps
    const std::list<S_msrDamp>&
                          getNoteDamps () const
                              { return fNoteDamps; }

    // damp alls
    const std::list<S_msrDampAll>&
                          getNoteDampAlls () const
                              { return fNoteDampAlls; }

    // scordaturas
    const std::list<S_msrScordatura>&
                          getNoteScordaturas () const
                              { return fNoteScordaturas; }

    // slurs
    const std::list<S_msrSlur>&
                          getNoteSlurs () const
                              { return fNoteSlurs; }

    // ligatures
    const std::list<S_msrLigature>&
                          getNoteLigatures () const
                              { return fNoteLigatures; }

    // pedals
    const std::list<S_msrPedal>&
                          getNotePedals () const
                              { return fNotePedals; }

    // note measure information
    // -------------------------------

    void                  setNoteOccupiesAFullMeasure ();

    Bool                  getNoteOccupiesAFullMeasure () const
                              { return fNoteOccupiesAFullMeasure; }

    // note redundant information (for speed)
    Bool                  getNoteIsStemless () const
                              { return fNoteIsStemless; }

    void                  setNoteIsAChordsFirstMemberNote ()
                              { fNoteIsAChordsFirstMemberNote = true; }

    Bool                  getNoteIsAChordsFirstMemberNote () const
                              { return fNoteIsAChordsFirstMemberNote; }

    void                  setNoteIsFirstNoteInADoubleTremolo ()
                              { fNoteIsFirstNoteInADoubleTremolo = true; }

    Bool                  getNoteIsFirstNoteInADoubleTremolo () const
                              { return fNoteIsFirstNoteInADoubleTremolo; }

    void                  setNoteIsSecondNoteInADoubleTremolo ()
                              { fNoteIsSecondNoteInADoubleTremolo = true; }

    Bool                  getNoteIsSecondNoteInADoubleTremolo () const
                              { return fNoteIsSecondNoteInADoubleTremolo; }

    S_msrOrnament         getNoteTrillOrnament () const
                              { return fNoteTrillOrnament; }

    S_msrOrnament         getNoteDashesOrnament () const
                              { return fNoteDashesOrnament; }

    S_msrOrnament         getNoteDelayedTurnOrnament () const
                              { return fNoteDelayedTurnOrnament; }

    S_msrOrnament         getNoteDelayedInvertedTurnOrnament () const
                              { return fNoteDelayedInvertedTurnOrnament; }

    S_msrSpanner          getNoteWavyLineSpannerStart () const
                              { return fNoteWavyLineSpannerStart; }

    S_msrSpanner          getNoteWavyLineSpannerStop () const
                              { return fNoteWavyLineSpannerStop; }

    void                  setNoteIsFollowedByGraceNotesGroup ()
                              { fNoteIsFollowedByGraceNotesGroup = true; }

    Bool                  getNoteIsFollowedByGraceNotesGroup () const
                              { return fNoteIsFollowedByGraceNotesGroup; }

    // RGB color
    void                  setNoteColorAlphaRGB (
                            msrColorAlphaRGB& noteColorAlphaRGB);

    const msrColorAlphaRGB&
                          getNoteColorAlphaRGB () const
                              { return fNoteColorAlphaRGB; }
    Bool                  getNoteColorAlphaRGBHasBenSet () const
                              { return fNoteColorAlphaRGBHasBenSet; }

    // solo note or rest?
    void                  setSoloNoteOrRestInVoiceKind (
                            msrSoloNoteOrRestInVoiceKind value)
                              { fNoteSoloNoteOrRestInVoiceKind = value; }

    msrSoloNoteOrRestInVoiceKind
                          getSoloNoteOrRestInVoiceKind () const
                              { return fNoteSoloNoteOrRestInVoiceKind; }

    void                  setSoloNoteOrRestInStaffKind (
                            msrSoloNoteOrRestInStaffKind value)
                              { fNoteSoloNoteOrRestInStaffKind = value; }

    msrSoloNoteOrRestInStaffKind
                          getSoloNoteOrRestInStaffKind () const
                              { return fNoteSoloNoteOrRestInStaffKind; }

  public:

    // public services
    // ------------------------------------------------------

    // uplink to tuplet
// JMI ???    S_msrTuplet           fetchNoteUpLinkToTuplet () const;

    // uplink to grace notes group
    S_msrGraceNotesGroup  fetchNoteUpLinkToGraceNotesGroup () const;

    // voice upLink
    S_msrVoice            fetchNoteUpLinkToVoice () const;

    // staff upLink
    S_msrStaff            fetchUpLinkToNoteToStaff () const;

    // part upLink
    S_msrPart             fetchUpLinkToNoteToPart () const;

    // partgroup upLink
    S_msrPartGroup        fetchNoteUpLinkToPartGroup () const;

    // score upLink
    S_msrScore            fetchUpLinkToNoteToScore () const;

    // a pitched rest?
    Bool                  noteIsAPitchedRest () const;

    // diatonic pitch
    msrDiatonicPitchKind  noteDiatonicPitchKind (
                            int inputLineNumber) const;

    // tuplet members
    void                  determineTupletMemberSoundingFromDisplayWholeNotes (
                            int actualNotes,
                            int normalNotes);

    // lyrics
    void                  appendSyllableToNote (
                            const S_msrSyllable& syllable);

    // beams
    void                  appendBeamToNote (
                            const S_msrBeam& beam);

    // articulations
    void                  appendArticulationToNote (
                            const S_msrArticulation& art);

    // spanners
    void                  appendSpannerToNote (
                            const S_msrSpanner& span);

    // technicals
    void                  appendTechnicalToNote (
                            const S_msrTechnical& tech);

    void                  appendTechnicalWithIntegerToNote (
                            const S_msrTechnicalWithInteger& tech);

    void                  appendTechnicalWithFloatToNote (
                            const S_msrTechnicalWithFloat& tech);

    void                  appendTechnicalWithStringToNote (
                            const S_msrTechnicalWithString& tech);

    // ornaments
    void                  appendOrnamentToNote (
                            const S_msrOrnament& orn);

    // glissandos
    void                  appendGlissandoToNote (
                            const S_msrGlissando& glissando);

    // slides
    void                  appendSlideToNote (
                            const S_msrSlide& slide);

    // dynamics
    void                  appendDynamicToNote (
                            const S_msrDynamic& dynamic);
    void                  appendOtherDynamicToNote (
                            const S_msrOtherDynamic& otherDynamic);

    S_msrDynamic          removeFirstDynamics (); // ??? JMI v0.9.66

    // words
    void                  appendWordsToNote (
                            const S_msrWords& words);

    // slurs
    void                  appendSlurToNote (
                            const S_msrSlur& slur);

    // ligatures
    void                  appendLigatureToNote (
                            const S_msrLigature& ligature);

    // pedals
    void                  appendPedalToNote (
                            const S_msrPedal& pedal);

    //  slashes
    void                  appendSlashToNote (
                            const S_msrSlash& slash);

    // cresc/decresc
    void                  appendCrescDecrescToNote (
                            const S_msrCrescDecresc& crescDecresc);

    //  wedges
    void                  appendWedgeToNote (
                            const S_msrWedge& wedge);

    S_msrWedge            removeFirstWedge (); // JMI v0.9.66

    // segnos
    void                  appendSegnoToNote (
                            const S_msrSegno& segno);

    // dal segnos
    void                  appendDalSegnoToNote (
                            const S_msrDalSegno& dalSegno);

    // codas
    void                  appendCodaToNote (
                            const S_msrCoda& coda);

    // eyeglasses
    void                  appendEyeGlassesToNote (
                            const S_msrEyeGlasses& eyeGlasses);

    // damps
    void                  appendDampToNote (
                            const S_msrDamp& damp);

    // damp alls
    void                  appendDampAllToNote (
                            const S_msrDampAll& dampAll);

    // scordaturas
    void                  appendScordaturaToNote (
                            const S_msrScordatura& scordatura);

    static bool           compareNotesByIncreasingMeasurePosition (
                            const SMARTP<msrNote>& first,
                            const SMARTP<msrNote>& second);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    // pitch kind
    std::string           notePitchAsString () const;

    std::string           noteDisplayPitchKindAsString () const;

    // octave kind
    std::string           bsrNoteOctaveKindAsString () const
                              {
                                return
                                  msrOctaveKindAsString (fNoteOctaveKind);
                              }

    std::string           noteDisplayOctaveKindAsString () const
                              {
                                return
                                  msrOctaveKindAsString (fNoteDisplayOctaveKind);
                              }

    // note head
    std::string           noteDiatonicPitchKindAsString (
                            int inputLineNumber) const;

    // whole notes
    std::string           noteSoundingWholeNotesAsMsrString () const;
    std::string           noteDisplayWholeNotesAsMsrString () const;

    // graphic duration
    std::string           noteGraphicNotesDurationAsMusicXMLString () const;

//     std::string           tupletNoteGraphicNotesDurationAsMsrString ( // JMI v0.9.66
//                             int actualNotes, int normalNotes) const;

    // note as string
    std::string           notePitchAndSoundingWholeNotesAsString () const
                              {
                                return
                                  notePitchAsString ()
                                    +
                                noteSoundingWholeNotesAsMsrString ();
                              }

    std::string           soundingNoteEssentialsAsString () const;
    std::string           soundingNoteEssentialsAsStringForMeasuresSlices () const;

    std::string           nonSoundingNoteEssentialsAsString () const;
    std::string           nonSoundingNoteEssentialsAsStringForMeasuresSlices () const;

    std::string           noteComplementsAsString () const;

    std::string           asMsrString () const;

    std::string           asString () const override;
    std::string           asShortStringForMeasuresSlices () const override;

    std::string           asShortString () const override;
    std::string           asShortStringWithRawWholeNotes () const;

    std::string           asShortStringForTimeView () const;

    std::string           asMinimalString () const;

    std::string           noteEssentialsAsSting () const;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    // ------------------------------------------------------

    S_msrMeasure          fNoteUpLinkToMeasure;

    S_msrChord            fNoteShortcutUpLinkToChord;

    S_msrGraceNotesGroup  fNoteShortcutUpLinkToGraceNotesGroup;

    S_msrTuplet           fNoteShortcutUpLinkToTuplet;

    // basic note description
    // ------------------------------------------------------

    // note kind
    msrNoteKind           fNoteKind;

    // note quarter tones pitch kind
    msrQuarterTonesPitchKind
                          fNoteQuarterTonesPitchKind;

    // note octave kind
    msrOctaveKind         fNoteOctaveKind;

    // whole notes
    msrWholeNotes         fNoteDisplayWholeNotes;

    int                   fNoteDotsNumber;

    msrNotesDurationKind  fNoteGraphicNotesDurationKind;

    msrTupletFactor       fNoteTupletFactor;

    msrQuarterTonesPitchKind
                          fNoteQuarterTonesDisplayPitchKind;
    msrOctaveKind         fNoteDisplayOctaveKind;
                                // for unpitched notes
                                // and pitched rests

    // cue note?
    msrNoteIsACueNoteKind fNoteIsACueNoteKind;

    // note octave shift
    S_msrOctaveShift      fNoteOctaveShift;

    // note print object kind
    msrPrintObjectKind    fNotePrintObjectKind;

    // note head
    msrNoteHeadKind       fNoteHeadKind;
    msrNoteHeadFilledKind fNoteHeadFilledKind;
    msrNoteHeadParenthesesKind
                          fNoteHeadParenthesesKind;

    // accidentals
    // ------------------------------------------------------

    msrAccidentalKind     fNoteAccidentalKind;

    msrEditorialAccidentalKind
                          fNoteEditorialAccidentalKind;
    msrCautionaryAccidentalKind
                          fNoteCautionaryAccidentalKind;

    // chord member?
    // ------------------------------------------------------

    Bool                  fNoteBelongsToAChord;

    // tuplet member?
    // ------------------------------------------------------

    Bool                  fNoteBelongsToATuplet;

    // note measure information
    // ------------------------------------------------------

    Bool                  fNoteOccupiesAFullMeasure;

    // multiple rest member?
    // ------------------------------------------------------

    Bool                  fNoteBelongsToAMultiMeasureRests; // JMI
    int                   fNoteMultiMeasureRestsSequenceNumber; // JMI

    // note lyrics
    // ------------------------------------------------------

    std::list<S_msrSyllable>
                          fNoteSyllables;

    // stem
    // ------------------------------------------------------

    S_msrStem             fNoteStem;

    // beams
    // ------------------------------------------------------

    std::list<S_msrBeam>  fNoteBeams;

    // articulations
    // ------------------------------------------------------

    std::list<S_msrArticulation>
                          fNoteArticulations;

    // spanners
    // ------------------------------------------------------

    std::list<S_msrSpanner>
                          fNoteSpanners;

    // technicals
    // ------------------------------------------------------

    std::list<S_msrTechnical>
                          fNoteTechnicals;

    std::list<S_msrTechnicalWithInteger>
                          fNoteTechnicalWithIntegers;

    std::list<S_msrTechnicalWithFloat>
                          fNoteTechnicalWithFloats;

    std::list<S_msrTechnicalWithString>
                          fNoteTechnicalWithStrings;

    // ornaments
    // ------------------------------------------------------

    std::list<S_msrOrnament>
                          fNoteOrnaments;

    // glissandos
    // ------------------------------------------------------

    std::list<S_msrGlissando>
                          fNoteGlissandos;

    // slides
    // ------------------------------------------------------

    std::list<S_msrSlide> fNoteSlides;

    // grace notes
    // ------------------------------------------------------

    S_msrGraceNotesGroup  fNoteGraceNotesGroupBefore;
  // JMI  S_msrAfterGraceNotesGroup ??? v0.9.64
    S_msrGraceNotesGroup
                          fNoteGraceNotesGroupAfter;

    // single tremolo
    // ------------------------------------------------------

    S_msrSingleTremolo    fNoteSingleTremolo;

    // tie
    // ------------------------------------------------------

    S_msrTie              fNoteTie;

    // dynamics
    // ------------------------------------------------------

    std::list<S_msrDynamic>
                          fNoteDynamics;
    std::list<S_msrOtherDynamic>
                          fNoteOtherDynamics;

    // slashes
    // ------------------------------------------------------

    std::list<S_msrSlash> fNoteSlashes;

    // cresc/decresc
    // ------------------------------------------------------

    std::list<S_msrCrescDecresc>
                          fNoteCrescDecrescs;

    // wedges
    // ------------------------------------------------------

    std::list<S_msrWedge> fNoteWedges;

    // segnos
    // ------------------------------------------------------

    std::list<S_msrSegno> fNoteSegnos;

    // dal segnos
    // ------------------------------------------------------

    std::list<S_msrDalSegno>
                          fNoteDalSegnos;

    // coda
    // ------------------------------------------------------

    std::list<S_msrCoda>  fNoteCodas;

    // eyeglasses
    // ------------------------------------------------------

    std::list<S_msrEyeGlasses>
                          fNoteEyeGlasses;

    // damps
    // ------------------------------------------------------

    std::list<S_msrDamp>  fNoteDamps;

    // damp alls
    // ------------------------------------------------------

    std::list<S_msrDampAll>
                          fNoteDampAlls;

    // scordaturas
    // ------------------------------------------------------

    std::list<S_msrScordatura>
                          fNoteScordaturas;

    // words
    // ------------------------------------------------------

    std::list<S_msrWords> fNoteWords;

    // slurs
    // ------------------------------------------------------

    std::list<S_msrSlur>  fNoteSlurs;

    // ligatures
    // ------------------------------------------------------

    std::list<S_msrLigature>
                          fNoteLigatures;

    // pedals
    // ------------------------------------------------------

    std::list<S_msrPedal> fNotePedals;

    // harmonies
    // ------------------------------------------------------

    std::list<S_msrHarmony>
                          fNoteHarmoniesList;

    // figured bass
    // ------------------------------------------------------

    std::list<S_msrFiguredBass>
                          fNoteFiguredBassesList;

    // note redundant information (for speed)
    // ------------------------------------------------------

    // this is needed to handle stemless notes,
    // because the <stem> is visited after 'visitorStart (S_msrNote )'
    Bool                  fNoteIsStemless;

    // this is to distinguish the first chord member note
    // from the other ones, since only it should get the slur if any
    Bool                  fNoteIsAChordsFirstMemberNote;

    // this is needed to handle double tremolos
    Bool                  fNoteIsFirstNoteInADoubleTremolo;
    Bool                  fNoteIsSecondNoteInADoubleTremolo;

    // this is useful
    // as well as to produce a nice \aftergrace in LilyPond
    S_msrOrnament         fNoteTrillOrnament;
    Bool                  fNoteIsFollowedByGraceNotesGroup;

    // this is useful to produce a text spanner in LilyPond
    S_msrOrnament         fNoteDashesOrnament;

    // this is needed to produce a delayed turn/inverted-turn in LilyPond
    S_msrOrnament         fNoteDelayedTurnOrnament;
    S_msrOrnament         fNoteDelayedInvertedTurnOrnament;

    // this is useful to  to combine a trill and a wavy line in LilyPond
    S_msrSpanner          fNoteWavyLineSpannerStart;
    S_msrSpanner          fNoteWavyLineSpannerStop;

    // RGB color
    msrColorAlphaRGB      fNoteColorAlphaRGB;
    Bool                  fNoteColorAlphaRGBHasBenSet;

    // solo note or rest?
    msrSoloNoteOrRestInVoiceKind
                          fNoteSoloNoteOrRestInVoiceKind;

    msrSoloNoteOrRestInStaffKind
                          fNoteSoloNoteOrRestInStaffKind;
};
typedef SMARTP<msrNote> S_msrNote;
EXP std::ostream& operator << (std::ostream& os, const S_msrNote& elt);


}


#endif // ___msrNotes___
