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
#include "msrTypesForwardDeclarations.h"

#include "msrMeasureElements.h"
#include "msrMeasures.h"
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
    void                  setMeasureElementMeasurePosition (
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
                              {
                                setMeasureElementSoundingWholeNotes (
                                  wholeNotes, context);
                              }

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

    // tuplet members
    void                  setNoteBelongsToATuplet ();

    Bool                  getNoteBelongsToATuplet () const
                              { return fNoteBelongsToATuplet; }

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
                          getNoteSyllablesList () const
                              { return fNoteSyllablesList; }

    // elements attached to the note
    // -------------------------------

    // stems
    void                  setNoteStem (const S_msrStem& stem);

    S_msrStem             getNoteStem () const
                              { return fNoteStem; }

    // beams
    const std::list<S_msrBeam>&
                          getNoteBeamsList () const
                              { return fNoteBeamsList; }

    // articulations
    const std::list<S_msrArticulation>&
                          getNoteArticulationsList () const
                              { return fNoteArticulationsList; }

    std::list<S_msrArticulation>&
                          getNoteArticulationsListNonConst ()
                              { return fNoteArticulationsList; }

    // spanners
    const std::list<S_msrSpanner>&
                          getNoteSpannersList () const
                              { return fNoteSpannersList; }

    std::list<S_msrSpanner>&
                          getNoteSpannersListNonConst ()
                              { return fNoteSpannersList; }

    // technicals
    const std::list<S_msrTechnical>&
                          getNoteTechnicalsList () const
                              { return fNoteTechnicalsList; }

    const std::list<S_msrTechnicalWithInteger>&
                          getNoteTechnicalWithIntegersList () const
                              { return fNoteTechnicalWithIntegersList; }

    const std::list<S_msrTechnicalWithFloat>&
                          getNoteTechnicalWithFloatsList () const
                              { return fNoteTechnicalWithFloatsList; }

    const std::list<S_msrTechnicalWithString>&
                          getNoteTechnicalWithStringsList () const
                              { return fNoteTechnicalWithStringsList; }

    // ornaments
    const std::list<S_msrOrnament>&
                          getNoteOrnamentsList () const
                              { return fNoteOrnamentsList; }

    // glissandos
    const std::list<S_msrGlissando>&
                          getNoteGlissandosList () const
                              { return fNoteGlissandosList; }

    // slides
    const std::list<S_msrSlide>&
                          getNoteSlidesList () const
                              { return fNoteSlidesList; }

    // grace notes
    void                  setGraceNotesGroupBeforeNote (
                            const S_msrGraceNotesGroup& graceNotesGroupBefore);

    S_msrGraceNotesGroup  getGraceNotesGroupBeforeNote () const
                              { return fGraceNotesGroupBeforeNote; }

    void                  setGraceNotesGroupAfterNote (
                            const S_msrGraceNotesGroup& graceNotesGroupAfter);

    S_msrGraceNotesGroup  getGraceNotesGroupAfterNote () const
                              { return fGraceNotesGroupAfterNote; }

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
    const std::list<S_msrTie>&
                          getNoteTiesList () const
                              { return fNoteTiesList; }

    // dynamics
    const std::list<S_msrDynamic>&
                          getNoteDynamicsList () const
                              { return fNoteDynamicsList; }
    const std::list<S_msrOtherDynamic>&
                          getNoteOtherDynamicsList () const
                              { return fNoteOtherDynamicsList; }

    // words
    const std::list<S_msrWords>&
                          getNoteWordsList () const
                              { return fNoteWordsList; }

    std::list<S_msrWords>&     getNoteWordsListNonConst ()
                              { return fNoteWordsList; }

    // slashes
    const std::list<S_msrSlash>&
                          getNoteSlashesList () const
                              { return fNoteSlashesList; }

    // wedges
    const std::list<S_msrCrescDecresc>&
                          getNoteCrescDecrescsList () const
                              { return fNoteCrescDecrescsList; }

    // wedges
    const std::list<S_msrWedge>&
                          getNoteWedgesList () const
                              { return fNoteWedgesList; }

    std::list<S_msrWedge>&
                          getNoteWedgesListNonConst ()
                              { return fNoteWedgesList; }

    // segnos
    const std::list<S_msrSegno>&
                          getNoteSegnosList () const
                              { return fNoteSegnosList; }

    // dal segnos
    const std::list<S_msrDalSegno>&
                          getNoteDalSegnosList () const
                              { return fNoteDalSegnosList; }

    // coda
    const std::list<S_msrCoda>&
                          getNoteCodasList () const
                              { return fNoteCodasList; }

    // eyeglasses
    const std::list<S_msrEyeGlasses>&
                          getNoteEyeGlassesList () const
                              { return fNoteEyeGlassesList; }

    // damps
    const std::list<S_msrDamp>&
                          getNoteDampsList () const
                              { return fNoteDampsList; }

    // damp alls
    const std::list<S_msrDampAll>&
                          getNoteDampAllsList () const
                              { return fNoteDampAllsList; }

    // scordaturas
    const std::list<S_msrScordatura>&
                          getNoteScordaturasList () const
                              { return fNoteScordaturasList; }

    // slurs
    const std::list<S_msrSlur>&
                          getNoteSlursList () const
                              { return fNoteSlursList; }

    // ligatures
    const std::list<S_msrLigature>&
                          getNoteLigaturesList () const
                              { return fNoteLigaturesList; }

    // pedals
    const std::list<S_msrPedal>&
                          getNotePedalsList () const
                              { return fNotePedalsList; }

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
    S_msrScore            fetchNoteUpLinkToScore () const;

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

    // ties
    void                  appendTieToNote (
                            const S_msrTie& tie);

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

    int                   fetchNoteSlurStartsNumber () const;
    int                   fetchNoteSlurStopsNumber () const;

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
    std::string           noteDiatonicPitchKindAsString (
                            int inputLineNumber) const;

    std::string           notePitchAsString () const;

    std::string           noteDisplayPitchKindAsString () const;

    // whole notes
    std::string           noteSoundingWholeNotesPitchAndOctaveAsString () const;
    std::string           noteDisplayWholeNotesPitchAndOctaveAsString () const;

    // octave kind
    std::string           noteDisplayOctaveKindAsString () const
                              {
                                return
                                  msrOctaveKindAsString (fNoteDisplayOctaveKind);
                              }

    std::string           bsrNoteOctaveKindAsString () const
                              {
                                return
                                  msrOctaveKindAsString (fNoteOctaveKind);
                              }

    // note head

    // graphic duration
    std::string           noteGraphicNotesDurationAsMusicXMLString () const;

//     std::string           tupletNoteGraphicNotesDurationpitchAndOctaveAsString ( // JMI v0.9.66
//                             int actualNotes, int normalNotes) const;

    // note as string
    std::string           noteCoreAsString () const;
    std::string           noteCoreAndInputLineNumbersAsString () const;

    std::string           noteComplementsAsString () const;

    std::string           noteCoreAndComplementAsString () const;

    std::string           asString () const override;
    std::string           asHeaderLessString () const;

    std::string           asShortString () const override;

    std::string           asShortStringWithRawWholeNotes () const;

    std::string           asStringForVoicesFlatView () const;

    std::string           asStringForMeasuresSlices () const override;

//     std::string           asMinimalString () const;

//     std::string           noteForPrintAsString () const;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    // ------------------------------------------------------

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
                          fNoteSyllablesList;

    // stem
    // ------------------------------------------------------

    S_msrStem             fNoteStem;

    // beams
    // ------------------------------------------------------

    std::list<S_msrBeam>  fNoteBeamsList;

    // articulations
    // ------------------------------------------------------

    std::list<S_msrArticulation>
                          fNoteArticulationsList;

    // spanners
    // ------------------------------------------------------

    std::list<S_msrSpanner>
                          fNoteSpannersList;

    // technicals
    // ------------------------------------------------------

    std::list<S_msrTechnical>
                          fNoteTechnicalsList;

    std::list<S_msrTechnicalWithInteger>
                          fNoteTechnicalWithIntegersList;

    std::list<S_msrTechnicalWithFloat>
                          fNoteTechnicalWithFloatsList;

    std::list<S_msrTechnicalWithString>
                          fNoteTechnicalWithStringsList;

    // ornaments
    // ------------------------------------------------------

    std::list<S_msrOrnament>
                          fNoteOrnamentsList;

    // glissandos
    // ------------------------------------------------------

    std::list<S_msrGlissando>
                          fNoteGlissandosList;

    // slides
    // ------------------------------------------------------

    std::list<S_msrSlide> fNoteSlidesList;

    // grace notes
    // ------------------------------------------------------

    S_msrGraceNotesGroup  fGraceNotesGroupBeforeNote;
  // JMI  S_msrAfterGraceNotesGroup ??? v0.9.64
    S_msrGraceNotesGroup
                          fGraceNotesGroupAfterNote;

    // single tremolo
    // ------------------------------------------------------

    S_msrSingleTremolo    fNoteSingleTremolo;

    // tie
    // ------------------------------------------------------

    std::list<S_msrTie>   fNoteTiesList;

    // dynamics
    // ------------------------------------------------------

    std::list<S_msrDynamic>
                          fNoteDynamicsList;
    std::list<S_msrOtherDynamic>
                          fNoteOtherDynamicsList;

    // slashes
    // ------------------------------------------------------

    std::list<S_msrSlash> fNoteSlashesList;

    // cresc/decresc
    // ------------------------------------------------------

    std::list<S_msrCrescDecresc>
                          fNoteCrescDecrescsList;

    // wedges
    // ------------------------------------------------------

    std::list<S_msrWedge> fNoteWedgesList;

    // segnos
    // ------------------------------------------------------

    std::list<S_msrSegno> fNoteSegnosList;

    // dal segnos
    // ------------------------------------------------------

    std::list<S_msrDalSegno>
                          fNoteDalSegnosList;

    // coda
    // ------------------------------------------------------

    std::list<S_msrCoda>  fNoteCodasList;

    // eyeglasses
    // ------------------------------------------------------

    std::list<S_msrEyeGlasses>
                          fNoteEyeGlassesList;

    // damps
    // ------------------------------------------------------

    std::list<S_msrDamp>  fNoteDampsList;

    // damp alls
    // ------------------------------------------------------

    std::list<S_msrDampAll>
                          fNoteDampAllsList;

    // scordaturas
    // ------------------------------------------------------

    std::list<S_msrScordatura>
                          fNoteScordaturasList;

    // words
    // ------------------------------------------------------

    std::list<S_msrWords> fNoteWordsList;

    // slurs
    // ------------------------------------------------------

    std::list<S_msrSlur>  fNoteSlursList;

    // ligatures
    // ------------------------------------------------------

    std::list<S_msrLigature>
                          fNoteLigaturesList;

    // pedals
    // ------------------------------------------------------

    std::list<S_msrPedal> fNotePedalsList;

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
