#ifndef ___msrNotes___
#define ___msrNotes___

#include <iomanip>

#include "msrArticulations.h"
#include "msrBeams.h"
#include "msrChords.h"
#include "msrEyeGlasses.h"
#include "msrFiguredBassElements.h"
#include "msrGlissandos.h"
#include "msrGraceNotes.h"
#include "msrHarmonies.h"
#include "msrLigatures.h"
#include "msrLyrics.h"
#include "msrMeasures.h"
#include "msrOrnaments.h"
#include "msrParts.h"
#include "msrSingleTremolos.h"
#include "msrSlashes.h"
#include "msrSlides.h"
#include "msrSlurs.h"
#include "msrSpanners.h"
#include "msrStaves.h"
#include "msrStems.h"
#include "msrTechnicals.h"
#include "msrTies.h"
#include "msrTupletElements.h"
#include "msrTuplets.h"
#include "msrVoices.h"
#include "msrWords.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrGlissando;
typedef SMARTP<msrGlissando> S_msrGlissando;

//______________________________________________________________________________
class EXP msrNote : public msrTupletElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrNoteHeadKind {
      kNoteHeadSlash,
      kNoteHeadTriangle, kNoteHeadDiamond, kNoteHeadSquare,
      kNoteHeadCross,kNoteHeadX,
      kNoteHeadCircleX, kNoteHeadInvertedTriangle,
      kNoteHeadArrowDown, kNoteHeadArrowUp,
      kNoteHeadSlashed, kNoteHeadBackSlashed,
      kNoteHeadNormal, kNoteHeadCluster,
      kNoteHeadCircleDot,
      kNoteHeadLeftTriangle,
      kNoteHeadRectangle,
      kNoteHeadNone,
      kNoteHeadDo, kNoteHeadRe, kNoteHeadMi, kNoteHeadFa, kNoteHeadFaUp,
      kNoteHeadSo, kNoteHeadLa, kNoteHeadTi
    };

    static string noteHeadKindAsString (
      msrNoteHeadKind noteHeadKind);

    enum msrNoteHeadFilledKind {
      kNoteHeadFilledYes, kNoteHeadFilledNo
    };

    static string noteHeadFilledKindAsString (
      msrNoteHeadFilledKind noteHeadFilledKind);

    enum msrNoteHeadParenthesesKind {
      kNoteHeadParenthesesYes, kNoteHeadParenthesesNo
    };

    static string noteHeadParenthesesKindAsString (
      msrNoteHeadParenthesesKind noteHeadParenthesesKind);

    enum msrNoteIsACueNoteKind {
      kNoteIsACueNoteYes, kNoteIsACueNoteNo
    };

    static string noteIsACueNoteKindAsString (
      msrNoteIsACueNoteKind noteIsACueNoteKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNote> create (
                            int                        inputLineNumber,
                            const string&              noteMeasureNumber,

                            msrNoteKind                noteKind,

                            msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
                            msrOctaveKind              noteOctaveKind,

                            const rational&            noteSoundingWholeNotes,
                            const rational&            noteDisplayWholeNotes,

                            int                        noteDotsNumber,

                            msrDurationKind            noteGraphicDurationKind,

                            msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
                            msrOctaveKind              noteDisplayOctaveKind,

                            msrNoteIsACueNoteKind      noteIsACueNoteKind,

                            msrPrintObjectKind         notePrintObjectKind,

                            msrNoteHeadKind            noteHeadKind,
                            msrNoteHeadFilledKind      noteHeadFilledKind,
                            msrNoteHeadParenthesesKind noteHeadParenthesesKind);

    SMARTP<msrNote> createNoteNewbornClone (
                            S_msrPart containingPart);

    SMARTP<msrNote> createNoteDeepClone (
                            S_msrVoice containingVoice);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrNote> createRestNote (
                            int             inputLineNumber,
                            const string&   noteMeasureNumber,
                            const rational& soundingWholeNotes,
                            const rational& displayWholeNotes,
                            int             dotsNumber);

    static SMARTP<msrNote> createRestNoteWithOctave (
                            int             inputLineNumber,
                            const string&   noteMeasureNumber,
                            msrOctaveKind   noteOctave,
                            const rational& soundingWholeNotes,
                            const rational& displayWholeNotes,
                            int             dotsNumber);

    static SMARTP<msrNote> createSkipNote (
                            int             inputLineNumber,
                            const string&   noteMeasureNumber,
                            const rational& soundingWholeNotes,
                            const rational& displayWholeNotes,
                            int             dotsNumberr);

    static SMARTP<msrNote> createSkipNoteWithOctave (
                            int             inputLineNumber,
                            const string&   noteMeasureNumber,
                            msrOctaveKind   noteOctave,
                            const rational& soundingWholeNotes,
                            const rational& displayWholeNotes,
                            int             dotsNumber);

    static SMARTP<msrNote> createGraceSkipNote (
                            // above with MusicXML??? JMI
                            int             inputLineNumber,
                            const string&   noteMeasureNumber,
                            const rational& soundingWholeNotes,
                            const rational& displayWholeNotes,
                            int             dotsNumber);

    static SMARTP<msrNote> createRegularNote (
                            int                      inputLineNumber,
                            const string&            noteMeasureNumber,
                            msrQuarterTonesPitchKind quarterTonesPitchKind,
                            msrOctaveKind            noteOctaveKind,
                            const rational&          soundingWholeNotes,
                            const rational&          displayWholeNotes,
                            int                      dotsNumber);

    static SMARTP<msrNote> createRestFromString (
                            int           inputLineNumber,
                            const string& restString,
                            const string& restMeasureNumber);

    static SMARTP<msrNote> createSkipFromString (
                            int           inputLineNumber,
                            const string& skipString,
                            const string& skipMeasureNumber);

    static SMARTP<msrNote> createNoteFromString (
                            int           inputLineNumber,
                            const string& noteString,
                            const string& noteMeasureNumber);

    // creation from the options
    // ------------------------------------------------------

    static SMARTP<msrNote> createNoteFromSemiTonesPitchAndOctave (
                            int                          inputLineNumber,
                            S_msrSemiTonesPitchAndOctave semiTonesPitchAndOctave);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrNote (
                            int                        inputLineNumber,
                            const string&              noteMeasureNumber,

                            msrNoteKind                noteKind,

                            msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
                            msrOctaveKind              noteOctaveKind,

                            const rational&            noteSoundingWholeNotes,
                            const rational&            noteDisplayWholeNotes,

                            int                        noteDotsNumber,

                            msrDurationKind            noteGraphicDurationKind,

                            msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
                            msrOctaveKind              noteDisplayOctaveKind,

                            msrNoteIsACueNoteKind      noteIsACueNoteKind,

                            msrPrintObjectKind         notePrintObjectKind,

                            msrNoteHeadKind            noteHeadKind,
                            msrNoteHeadFilledKind      noteHeadFilledKind,
                            msrNoteHeadParenthesesKind noteHeadParenthesesKind);

    virtual               ~msrNote ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeNote ();

  public:

    // set and get
    // ------------------------------------------------------

    // measure upLink
    void                  setNoteDirectMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fNoteDirectMeasureUpLink = measure; }

    S_msrMeasure          getNoteDirectMeasureUpLink () const
                              { return fNoteDirectMeasureUpLink; }

    // chord upLink
    void                  setNoteDirectChordUpLink (
                            const S_msrChord& chord)
                              { fNoteDirectChordUpLink = chord; }

    S_msrChord            getNoteDirectChordUpLink () const
                              { return fNoteDirectChordUpLink; }

    // grace notes group upLink
    void                  setNoteDirectGraceNotesGroupUpLink (
                            const S_msrGraceNotesGroup& graceNotesGroup)
                              { fNoteDirectGraceNotesGroupUpLink = graceNotesGroup; }

    S_msrGraceNotesGroup  getNoteDirectGraceNotesGroupUpLink () const
                              { return fNoteDirectGraceNotesGroupUpLink; }

    // tuplet upLink
    void                  setNoteDirectTupletUpLink (
                            const S_msrTuplet& tuplet)
                              { fNoteDirectTupletUpLink = tuplet; }

    S_msrTuplet           getNoteDirectTupletUpLink () const
                              { return fNoteDirectTupletUpLink; }

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

    // note print kind

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
                              noteHeadFilledKind)
                              { fNoteHeadFilledKind = noteHeadFilledKind; }

    msrNoteHeadFilledKind getNoteHeadFilledKind () const
                              { return fNoteHeadFilledKind; }

    void                  setNoteHeadParenthesesKind (
                            msrNoteHeadParenthesesKind
                              noteHeadParenthesesKind)
                              {
                                fNoteHeadParenthesesKind =
                                  noteHeadParenthesesKind;
                              }

    msrNoteHeadParenthesesKind
                          getNoteHeadParenthesesKind () const
                              { return fNoteHeadParenthesesKind; }

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

    // note whole notes

    void                  setNoteSoundingWholeNotes (
                            const rational& wholeNotes)
                              { fMeasureElementSoundingWholeNotes = wholeNotes; }

    rational              getNoteSoundingWholeNotes () const
                              { return fMeasureElementSoundingWholeNotes; }

    // note display

    msrQuarterTonesPitchKind
                          getNoteQuarterTonesDisplayPitchKind () const
                              { return fNoteQuarterTonesDisplayPitchKind; }

    void                  setNoteDisplayWholeNotes (
                            const rational& wholeNotes)
                              { fNoteDisplayWholeNotes = wholeNotes; }

    rational              getNoteDisplayWholeNotes () const
                              { return fNoteDisplayWholeNotes; }

    // dots

    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }

    msrDurationKind       getNoteGraphicDurationKind () const
                              { return fNoteGraphicDurationKind; }

    // tuplet factor

    void                  setNoteTupletFactor (
                            const msrTupletFactor& tupletFactor)
                              { fNoteTupletFactor = tupletFactor; }

    const msrTupletFactor&
                          getNoteTupletFactor () const
                              { return fNoteTupletFactor; }

    // display octave

    msrOctaveKind         getNoteDisplayOctaveKind () const
                              { return fNoteDisplayOctaveKind; }

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
                            msrNoteIsACueNoteKind noteIsACueNoteKind)
                              { fNoteIsACueNoteKind = noteIsACueNoteKind; }

    msrNoteIsACueNoteKind getNoteIsACueNoteKind () const
                              { return fNoteIsACueNoteKind; }

    // octave shifts
    void                  setNoteOctaveShift (
                             S_msrOctaveShift octaveShift)
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
    void                  appendHarmonyToNoteHarmoniesList (
                            S_msrHarmony harmony);

    const list<S_msrHarmony>&
                          getNoteHarmoniesList () const
                              { return fNoteHarmoniesList; }

    // figured bass
    void                  appendFiguredBassElementToNoteFiguredBassElementsList (
                            S_msrFiguredBassElement figuredBassElement);

    const list<S_msrFiguredBassElement>&
                          getNoteFiguredBassElementsList () const
                              { return fNoteFiguredBassElementsList; }

    // note lyrics
    // -------------------------------

    list<S_msrSyllable>   getNoteSyllables () const
                              { return fNoteSyllables; }

    // elements attached to the note
    // -------------------------------

    // stems
    void                  setNoteStem (S_msrStem stem);

    S_msrStem             getNoteStem () const
                              { return fNoteStem; }

    // beams
    const list<S_msrBeam>&
                          getNoteBeams () const
                              { return fNoteBeams; }

    // articulations
    const list<S_msrArticulation>&
                          getNoteArticulations () const
                              { return fNoteArticulations; }

    list<S_msrArticulation>&
                          getNoteArticulationsToModify ()
                              { return fNoteArticulations; }

    // spanners
    const list<S_msrSpanner>&
                          getNoteSpanners () const
                              { return fNoteSpanners; }

    list<S_msrSpanner>&
                          getNoteSpannersToModify ()
                              { return fNoteSpanners; }

    // technicals
    const list<S_msrTechnical>&
                          getNoteTechnicals () const
                              { return fNoteTechnicals; }

    const list<S_msrTechnicalWithInteger>&
                          getNoteTechnicalWithIntegers () const
                              { return fNoteTechnicalWithIntegers; }

    const list<S_msrTechnicalWithFloat>&
                          getNoteTechnicalWithFloats () const
                              { return fNoteTechnicalWithFloats; }

    const list<S_msrTechnicalWithString>&
                          getNoteTechnicalWithStrings () const
                              { return fNoteTechnicalWithStrings; }

    // ornaments
    const list<S_msrOrnament>&
                          getNoteOrnaments () const
                              { return fNoteOrnaments; }

    // glissandos
    const list<S_msrGlissando>&
                          getNoteGlissandos () const
                              { return fNoteGlissandos; }

    // slides
    const list<S_msrSlide>&
                          getNoteSlides () const
                              { return fNoteSlides; }

    // grace notes
    void                  setNoteGraceNotesGroupBefore (
                            S_msrGraceNotesGroup graceNotesGroupBefore);

    S_msrGraceNotesGroup  getNoteGraceNotesGroupBefore () const
                              { return fNoteGraceNotesGroupBefore; }

    void                  setNoteGraceNotesGroupAfter (
                            S_msrGraceNotesGroup graceNotesGroupAfter);

    S_msrGraceNotesGroup  getNoteGraceNotesGroupAfter () const
                              { return fNoteGraceNotesGroupAfter; }

    // after grace notes
    /* JMI
    void                  setNoteAfterGraceNotesGroup (
                            S_msrAfterGraceNotesGroup afterGraceNotesGroup);

    S_msrAfterGraceNotesGroup
                          getNoteAfterGraceNotesGroup () const
                              { return fNoteAfterGraceNotesGroup; }
                            */

    // singleTremolo
    void                  setNoteSingleTremolo (
                            S_msrSingleTremolo trem);

    S_msrSingleTremolo    getNoteSingleTremolo () const
                              { return fNoteSingleTremolo; }

    // ties
    void                  setNoteTie (S_msrTie tie)
                              { fNoteTie = tie; }
    S_msrTie              getNoteTie () const
                              { return fNoteTie; }

    // dynamics
    const list<S_msrDynamic>&
                          getNoteDynamics () const
                              { return fNoteDynamics; }
    const list<S_msrOtherDynamic>&
                          getNoteOtherDynamics () const
                              { return fNoteOtherDynamics; }

    // words
    const list<S_msrWords>&
                          getNoteWords () const
                              { return fNoteWords; }

    list<S_msrWords>&     getNoteWordsToModify ()
                              { return fNoteWords; }

    // slashes
    const list<S_msrSlash>&
                          getNoteSlashes () const
                              { return fNoteSlashes; }

    // wedges
    const list<S_msrCrescDecresc>&
                          getNoteCrescDecrescs () const
                              { return fNoteCrescDecrescs; }

    // wedges
    const list<S_msrWedge>&
                          getNoteWedges () const
                              { return fNoteWedges; }

    list<S_msrWedge>&
                          getNoteWedgesToModify ()
                              { return fNoteWedges; }

    // segnos
    const list<S_msrSegno>&
                          getNoteSegnos () const
                              { return fNoteSegnos; }

    // dal segnos
    const list<S_msrDalSegno>&
                          getNoteDalSegnos () const
                              { return fNoteDalSegnos; }

    // coda
    const list<S_msrCoda>&
                          getNoteCodas () const
                              { return fNoteCodas; }

    // eyeglasses
    const list<S_msrEyeGlasses>&
                          getNoteEyeGlasses () const
                              { return fNoteEyeGlasses; }

    // damps
    const list<S_msrDamp>&
                          getNoteDamps () const
                              { return fNoteDamps; }

    // damp alls
    const list<S_msrDampAll>&
                          getNoteDampAlls () const
                              { return fNoteDampAlls; }

    // scordaturas
    const list<S_msrScordatura>&
                          getNoteScordaturas () const
                              { return fNoteScordaturas; }

    // slurs
    const list<S_msrSlur>&
                          getNoteSlurs () const
                              { return fNoteSlurs; }

    // ligatures
    const list<S_msrLigature>&
                          getNoteLigatures () const
                              { return fNoteLigatures; }

    // pedals
    const list<S_msrPedal>&
                          getNotePedals () const
                              { return fNotePedals; }

    // note measure information
    // -------------------------------

    // position in measure
    void                  setNotePositionInMeasure (
                            const rational& positionInMeasure);

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
    void                  setNoteAlphaRGBColor (
                            msrAlphaRGBColor& noteAlphaRGBColor);

    const msrAlphaRGBColor&
                          getNoteAlphaRGBColor () const
                              { return fNoteAlphaRGBColor; }
    Bool                  getNoteAlphaRGBColorHasBenSet () const
                              { return fNoteAlphaRGBColorHasBenSet; }

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

    // measure upLink
    S_msrMeasure          fetchNoteMeasureUpLink () const;

    // tuplet upLink
// JMI ???    S_msrTuplet           fetchNoteTupletUpLink () const;

    // grace notes group upLink
    S_msrGraceNotesGroup  fetchNoteGraceNotesGroupUpLink () const;

    // voice upLink
    S_msrVoice            fetchNoteVoiceUpLink () const;

    // staff upLink
    S_msrStaff            fetchNoteStaffUpLink () const;

    // part upLink
    S_msrPart             fetchNotePartUpLink () const;

    // partgroup upLink
    S_msrPartGroup        fetchNotePartGroupUpLink () const;

    // score upLink
    S_msrScore            fetchNoteScoreUpLink () const;

    // a pitched rest?
    Bool                  noteIsAPitchedRest () const;

    // diatonic pitch
    msrDiatonicPitchKind  noteDiatonicPitchKind (
                            int inputLineNumber) const;

    // tuplet members
    void                  determineTupletMemberSoundingFromDisplayWholeNotes (
                            int actualNotes, int normalNotes);

    // lyrics
    void                  appendSyllableToNote (S_msrSyllable syllable);

    // beams
    void                  appendBeamToNote (S_msrBeam beam);

    // articulations
    void                  appendArticulationToNote (S_msrArticulation art);

    // spanners
    void                  appendSpannerToNote (S_msrSpanner span);

    // technicals
    void                  appendTechnicalToNote (S_msrTechnical tech);

    void                  appendTechnicalWithIntegerToNote (
                            S_msrTechnicalWithInteger tech);

    void                  appendTechnicalWithFloatToNote (
                            S_msrTechnicalWithFloat tech);

    void                  appendTechnicalWithStringToNote (
                            S_msrTechnicalWithString tech);

    // ornaments
    void                  appendOrnamentToNote (S_msrOrnament orn);

    // glissandos
    void                  appendGlissandoToNote (S_msrGlissando glissando);

    // slides
    void                  appendSlideToNote (S_msrSlide slide);

    // dynamics
    void                  appendDynamicToNote (
                            S_msrDynamic dynamic);
    void                  appendOtherDynamicToNote (
                            S_msrOtherDynamic otherDynamic);

    S_msrDynamic         removeFirstDynamics (); // ???

    // words
    void                  appendWordsToNote (S_msrWords words);

    // slurs
    void                  appendSlurToNote (S_msrSlur slur);

    // ligatures
    void                  appendLigatureToNote (S_msrLigature ligature);

    // pedals
    void                  appendPedalToNote (S_msrPedal pedal);

    //  slashes
    void                  appendSlashToNote (S_msrSlash slash);

    // cresc/decresc
    void                  appendCrescDecrescToNote (S_msrCrescDecresc crescDecresc);

    //  wedges
    void                  appendWedgeToNote (S_msrWedge wedge);

    S_msrWedge            removeFirstWedge (); // JMI

    // segnos
    void                  appendSegnoToNote (S_msrSegno segno);

    // dal segnos
    void                  appendDalSegnoToNote (S_msrDalSegno dalSegno);

    // codas
    void                  appendCodaToNote (S_msrCoda coda);

    // eyeglasses
    void                  appendEyeGlassesToNote (S_msrEyeGlasses eyeGlasses);

    // damps
    void                  appendDampToNote (S_msrDamp damp);

    // damp alls
    void                  appendDampAllToNote (S_msrDampAll dampAll);

    // scordaturas
    void                  appendScordaturaToNote (S_msrScordatura scordatura);

    virtual void          assignMeasureElementPositionInVoice (
                            rational&     positionInVoice,
                            const string& context) override;

    static bool           compareNotesByIncreasingPositionInMeasure (
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
    string                notePitchAsString () const;

    string                noteDisplayPitchKindAsString () const;

    // octave kind
    string                noteOctaveKindAsString () const
                              {
                                return
                                  msrOctaveKindAsString (fNoteOctaveKind);
                              }

    string                noteDisplayOctaveKindAsString () const
                              {
                                return
                                  msrOctaveKindAsString (fNoteDisplayOctaveKind);
                              }

    // note head
    string                noteDiatonicPitchKindAsString (
                            int inputLineNumber) const;

    // whole notes
    string                noteSoundingWholeNotesAsMsrString () const;
    string                noteDisplayWholeNotesAsMsrString () const;

    // graphic duration
    string                noteGraphicDurationAsMsrString () const;
    string                noteGraphicDurationAsMusicXMLString () const;

    string                tupletNoteGraphicDurationAsMsrString ( // JMI
                            int actualNotes, int normalNotes) const;

    // note as string
    string                notePitchAndSoundingWholeNotesAsString () const
                              {
                                return
                                  notePitchAsString ()
                                    +
                                noteSoundingWholeNotesAsMsrString ();
                              }

    string                soundingNoteEssentialsAsString () const;
    string                soundingNoteEssentialsAsStringForMeasuresSlices () const;

    string                nonSoundingNoteEssentialsAsString () const;
    string                nonSoundingNoteEssentialsAsStringForMeasuresSlices () const;

    string                noteComplementsAsString () const;

    string                asString () const override;
    string                asShortStringForMeasuresSlices () const override;

    string                asShortString () const override;
    string                asShortStringWithRawWholeNotes () const;

    string                asShortStringForTimeView () const;

    virtual void          printNoteEssentials (ostream& os) const;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    // ------------------------------------------------------

    S_msrChord            fNoteDirectChordUpLink;

    S_msrGraceNotesGroup  fNoteDirectGraceNotesGroupUpLink;

    S_msrTuplet           fNoteDirectTupletUpLink;

    S_msrMeasure          fNoteDirectMeasureUpLink;

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
    rational              fNoteDisplayWholeNotes;

    int                   fNoteDotsNumber;

    msrDurationKind       fNoteGraphicDurationKind;

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

    // note print kind
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

    Bool                  fNoteBelongsToAFullMeasureRests; // JMI
    int                   fNoteFullMeasureRestsSequenceNumber; // JMI

    // note lyrics
    // ------------------------------------------------------

    list<S_msrSyllable>   fNoteSyllables;

    // stem
    // ------------------------------------------------------

    S_msrStem             fNoteStem;

    // beams
    // ------------------------------------------------------

    list<S_msrBeam>       fNoteBeams;

    // articulations
    // ------------------------------------------------------

    list<S_msrArticulation>
                          fNoteArticulations;

    // spanners
    // ------------------------------------------------------

    list<S_msrSpanner>    fNoteSpanners;

    // technicals
    // ------------------------------------------------------

    list<S_msrTechnical>  fNoteTechnicals;

    list<S_msrTechnicalWithInteger>
                          fNoteTechnicalWithIntegers;

    list<S_msrTechnicalWithFloat>
                          fNoteTechnicalWithFloats;

    list<S_msrTechnicalWithString>
                          fNoteTechnicalWithStrings;

    // ornaments
    // ------------------------------------------------------

    list<S_msrOrnament>   fNoteOrnaments;

    // glissandos
    // ------------------------------------------------------

    list<S_msrGlissando>  fNoteGlissandos;

    // slides
    // ------------------------------------------------------

    list<S_msrSlide>      fNoteSlides;

    // grace notes
    // ------------------------------------------------------

    S_msrGraceNotesGroup  fNoteGraceNotesGroupBefore;
  // JMI  S_msrAfterGraceNotesGroup
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

    list<S_msrDynamic>   fNoteDynamics;
    list<S_msrOtherDynamic>
                          fNoteOtherDynamics;

    // slashes
    // ------------------------------------------------------

    list<S_msrSlash>      fNoteSlashes;

    // cresc/decresc
    // ------------------------------------------------------

    list<S_msrCrescDecresc>
                          fNoteCrescDecrescs;

    // wedges
    // ------------------------------------------------------

    list<S_msrWedge>      fNoteWedges;

    // segnos
    // ------------------------------------------------------

    list<S_msrSegno>      fNoteSegnos;

    // dal segnos
    // ------------------------------------------------------

    list<S_msrDalSegno>   fNoteDalSegnos;

    // coda
    // ------------------------------------------------------

    list<S_msrCoda>       fNoteCodas;

    // eyeglasses
    // ------------------------------------------------------

    list<S_msrEyeGlasses> fNoteEyeGlasses;

    // damps
    // ------------------------------------------------------

    list<S_msrDamp>       fNoteDamps;

    // damp alls
    // ------------------------------------------------------

    list<S_msrDampAll>    fNoteDampAlls;

    // scordaturas
    // ------------------------------------------------------

    list<S_msrScordatura> fNoteScordaturas;

    // words
    // ------------------------------------------------------

    list<S_msrWords>      fNoteWords;

    // slurs
    // ------------------------------------------------------

    list<S_msrSlur>       fNoteSlurs;

    // ligatures
    // ------------------------------------------------------

    list<S_msrLigature>   fNoteLigatures;

    // pedals
    // ------------------------------------------------------

    list<S_msrPedal>      fNotePedals;

    // harmonies
    // ------------------------------------------------------

    list<S_msrHarmony>    fNoteHarmoniesList;

    // figured bass
    // ------------------------------------------------------

    list<S_msrFiguredBassElement>
                          fNoteFiguredBassElementsList;

    // note redundant information (for speed)
    // ------------------------------------------------------

    // this is needed to handle stemless notes,
    // because the <stem> is visited after 'visitorStart ( S_msrNote )'
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
    msrAlphaRGBColor      fNoteAlphaRGBColor;
    Bool                  fNoteAlphaRGBColorHasBenSet;

    // solo note or rest?
    msrSoloNoteOrRestInVoiceKind
                          fNoteSoloNoteOrRestInVoiceKind;

    msrSoloNoteOrRestInStaffKind
                          fNoteSoloNoteOrRestInStaffKind;
};
typedef SMARTP<msrNote> S_msrNote;
EXP ostream& operator<< (ostream& os, const S_msrNote& elt);


}


#endif
