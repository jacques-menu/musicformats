/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrChords___
#define ___msrChords___

#include "msrArticulations.h"
#include "msrBeams.h"
#include "msrCodas.h"
#include "msrDynamics.h"
#include "msrFiguredBasses.h"
#include "msrGlissandos.h"
#include "msrGraceNotesGroups.h"
#include "msrHarmonies.h"
#include "msrInstruments.h"
#include "msrLigatures.h"
#include "msrMeasures.h"
#include "msrNotes.h"
#include "msrOctaveShifts.h"
#include "msrOrnaments.h"
#include "msrSegnos.h"
#include "msrSingleTremolos.h"
#include "msrSlashes.h"
#include "msrSlides.h"
#include "msrSlurs.h"
#include "msrSpanners.h"
#include "msrStems.h"
#include "msrTechnicals.h"
#include "msrTies.h"
#include "msrTupletElements.h"
#include "msrWords.h"


namespace MusicFormats
{

// chords
//______________________________________________________________________________

enum class msrChordInKind {
  kChordIn_UNKNOWN_,

  kChordInMeasure,
  kChordInTuplet,
  kChordInGraceNotesGroup
};

EXP std::string msrChordInKindAsString (
  msrChordInKind chordInKind);

std::ostream& operator << (std::ostream& os, const msrChordInKind& elt);

//______________________________________________________________________________
class EXP msrChord : public msrTupletElement
{
  public:

    static SMARTP<msrChord> create (int inputLineNumber);

//     static SMARTP<msrChord> create (
//                             int                  inputLineNumber,
//                             const S_msrMeasure&  upLinkToMeasure);

    static SMARTP<msrChord> create (
                            int                  inputLineNumber,
                            const S_msrMeasure&  upLinkToMeasure,
                            const msrWholeNotes& chordDisplayWholeNotes,
                            msrNotesDurationKind chordGraphicNotesDurationKind);

//     static SMARTP<msrChord> create (
//                             int                  inputLineNumber,
//                             const msrWholeNotes& chordDisplayWholeNotes,
//                             msrNotesDurationKind chordGraphicNotesDurationKind);

    // creation from MusicXML
    // ------------------------------------------------------

    SMARTP<msrChord> createChordNewbornClone (
                            const S_msrPart& containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChord (
                            int                  inputLineNumber);

                          msrChord (
                            int                  inputLineNumber,
                            const S_msrMeasure&  upLinkToMeasure);

                          msrChord (
                            int                  inputLineNumber,
                            const S_msrMeasure&  upLinkToMeasure,
                            const msrWholeNotes& chordDisplayWholeNotes,
                            msrNotesDurationKind chordGraphicNotesDurationKind);

    virtual               ~msrChord ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to tuplet
    void                  setChordShortcutUpLinkToContainingTuplet (
                            const S_msrTuplet& tuplet);

    S_msrTuplet           getChordShortcutUpLinkToContainingTuplet () const;

    // uplink to grace notes group
    void                  setChordUpLinkToContainingGraceNotesGroup (
                            const S_msrGraceNotesGroup& graceNotesGroup)
                              {
                                fChordUpLinkToContainingGraceNotesGroup =
                                  graceNotesGroup;
                              }

    S_msrGraceNotesGroup  getChordUpLinkToContainingGraceNotesGroup () const
                              { return fChordUpLinkToContainingGraceNotesGroup; }

    // chord kind
    void                  setChordKind (
                            msrChordInKind chordKind)
                              { fChordKind = chordKind; }

    msrChordInKind        getChordKind () const
                              { return fChordKind; }

    void                  setChordDisplayWholeNotes (
                            const msrWholeNotes& wholeNotes);

    msrWholeNotes         getChordDisplayWholeNotes () const
                              { return fChordDisplayWholeNotes; }

    // graphic duration
    void                  setChordGraphicNotesDurationKind (
                            msrNotesDurationKind notesDurationKind)
                              { fChordGraphicNotesDurationKind = notesDurationKind; }

    msrNotesDurationKind  getChordGraphicNotesDurationKind () const
                              { return fChordGraphicNotesDurationKind; }

    // chord notes
    const std::vector <S_msrNote>&
                          getChordNotesVector () const
                              { return fChordNotesVector; }

    // stems
    const std::list <S_msrStem>&
                          getChordStemsList () const
                              { return fChordStemsList; }

    // beams
    /*
    const std::list <S_msrBeam>&
                          getChordBeams () const
                              { return fChordBeams; }
                              */
    const std::list <S_msrBeam>&
                          getChordBeamsList () const
                              { return fChordBeamsList; }

    // articulations
    const std::list <S_msrArticulation>&
                          getChordArticulationsList () const
                              { return fChordArticulationsList; }

    // spanners
    const std::list <S_msrSpanner>&
                          getChordSpannersList () const
                              { return fChordSpannersList; }

    // technicals
    const std::list <S_msrTechnical>&
                          getChordTechnicalsList () const
                              { return fChordTechnicalsList; }

    const std::list <S_msrTechnicalWithInteger>&
                          getChordTechnicalWithIntegersList () const
                              { return fChordTechnicalWithIntegersList; }

    const std::list <S_msrTechnicalWithFloat>&
                          getChordTechnicalWithFloatsList () const
                              { return fChordTechnicalWithFloatsList; }

    const std::list <S_msrTechnicalWithString>&
                          getChordTechnicalWithStringsList () const
                              { return fChordTechnicalWithStringsList; }

    // ornaments
    const std::list <S_msrOrnament>&
                          getChordOrnamentsList () const
                              { return fChordOrnamentsList; }

    // glissandos
    const std::list <S_msrGlissando>&
                          getChordGlissandosList () const
                              { return fChordGlissandosList; }

    // slides
    const std::list <S_msrSlide>&
                          getChordSlidesList () const
                              { return fChordSlidesList; }

    // singleTremolo
    void                  setChordSingleTremolo (
                            const S_msrSingleTremolo& trem);

    S_msrSingleTremolo    getChordSingleTremolo () const
                              { return fChordSingleTremolo; }

    // dynamics
    const std::list <S_msrDynamic>&
                          getChordDynamicsList () const
                              { return fChordDynamicsList; }
    const std::list <S_msrOtherDynamic>&
                          getChordOtherDynamicsList () const
                              { return fChordOtherDynamicsList; }

    const std::list <S_msrCrescDecresc>&
                          getChordCrescDecrescsList () const
                              { return fChordCrescDecrescsList; }

    const std::list <S_msrWedge>&
                          getChordWedgesList () const
                              { return fChordWedgesList; }

    // segnos
    const std::list <S_msrSegno>&
                          getChordSegnosList () const
                              { return fChordSegnosList; }

    // dal segnos
    const std::list <S_msrDalSegno>&
                          getChordDalSegnosList () const
                              { return fChordDalSegnosList; }

    // coda
    const std::list <S_msrCoda>&
                          getChordCodasList () const
                              { return fChordCodasList; }

    // words
    const std::list <S_msrWords>&
                          getChordWordsList () const
                              { return fChordWordsList; }

    // ties
    const std::list <S_msrTie>&
                          getChordTiesList () const
                              { return fChordTiesList; }

    // slurs
    const std::list <S_msrChordSlurLink>&
                          getChordSlurLinksList () const
                              { return fChordSlurLinksList; }

    // ligatures
    const std::list <S_msrLigature>&
                          getChordLigaturesList () const
                              { return fChordLigaturesList; }

    // pedals
    const std::list <S_msrPedal>&
                          getChordPedalsList () const
                              { return fChordPedalsList; }

    // double tremolo
    void                  setChordIsFirstChordInADoubleTremolo ()
                              { fChordIsFirstChordInADoubleTremolo = true; }

    Bool                  getChordIsFirstChordInADoubleTremolo () const
                              { return fChordIsFirstChordInADoubleTremolo; }

    void                  setChordIsSecondChordInADoubleTremolo ()
                              { fChordIsSecondChordInADoubleTremolo = true; }

    Bool                  getChordIsSecondChordInADoubleTremolo () const
                              { return fChordIsSecondChordInADoubleTremolo; }

    // grace notes
/*
    void                  setChordGraceNotesGroupBefore (
                            const S_msrGraceNotesGroup& graceNotesGroupBefore)
                              {
                                fChordGraceNotesGroupBefore =
                                  graceNotesGroupBefore;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupBefore () const
                              { return fChordGraceNotesGroupBefore; }

    void                  setChordGraceNotesGroupAfter (
                            const S_msrGraceNotesGroup& graceNotesGroupAfter)
                              {
                                fChordGraceNotesGroupAfter =
                                  graceNotesGroupAfter;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupAfter () const
                              { return fChordGraceNotesGroupAfter; }
*/

    void                  setChordGraceNotesGroupLinkBefore (
                            int     inputLineNumber,
                            const S_msrChordGraceNotesGroupLink&
                                    chordChordGraceNotesGroupLinkBefore);

    const S_msrChordGraceNotesGroupLink&
                          getChordGraceNotesGroupLinkBefore () const
                              { return fChordGraceNotesGroupLinkBefore; }

    void                  setChordGraceNotesGroupLinkAfter (
                            int     inputLineNumber,
                            const S_msrChordGraceNotesGroupLink&
                                    chordChordGraceNotesGroupLinkAfter);

    const S_msrChordGraceNotesGroupLink&
                          getChordGraceNotesGroupLinkAfter () const
                              { return fChordGraceNotesGroupLinkAfter; }

    // harmony
    void                  appendHarmonyToChord (
                            const S_msrHarmony& harmony)
                              { fChordHarmoniesList.push_back (harmony); }

    // figured bass
    void                  setChordFiguredBassesList (
                            const S_msrFiguredBass& figuredBass)
                              { fChordFiguredBassesList = figuredBass; }

    const S_msrFiguredBass&
                          getChordFiguredBassesList () const
                              { return fChordFiguredBassesList; }

    // octave shift
    void                  setChordOctaveShift (
                            const S_msrOctaveShift& octaveShift)
                              { fChordOctaveShift = octaveShift; }

    S_msrOctaveShift      getChordOctaveShift () const
                              { return fChordOctaveShift; }

//     // measure positions
//     // this override calls setChordMembersMeasurePosition()
//     void                  setMeasureElementMeasurePosition (
//                             const S_msrMeasure&  measure,
//                             const msrWholeNotes& measurePosition,
//                             const std::string&   context) override;

    void                  setChordMembersMeasurePosition (
                            const S_msrMeasure&  measure,
                            const msrWholeNotes& measurePosition);

  public:

    // public services
    // ------------------------------------------------------

//     // uplink to tupletg
//     const S_msrTuplet&    fetchChordUpLinkToTuplet () const;

//     // uplink to  grace notes group
//     S_msrGraceNotesGroup  fetchChordUpLinkToGraceNotesGroup () const;

    // score upLink
    S_msrScore            fetchChordUpLinkToScore () const;

    // notes
    void                  appendNoteToChord (
                            const S_msrNote&  note,
                            const S_msrVoice& voice);

    S_msrNote             fetchChordFirstNonGraceNote () const;

    // some values have to be copied from one of the chord's notes
    void                  copyNoteValuesToChord (
                            const S_msrNote note);

    // some attachments have to be copied from each chord member one to the chord
    void                  copyNoteElementsIfAnyToChord (
                            const S_msrNote& note);

    void                  copyNoteArticulationsListToChord (
                            const S_msrNote& note);

    void                  copyNoteDynamicsListToChord (
                            const S_msrNote& note);
    void                  copyNoteOtherDynamicsListToChord (
                            const S_msrNote& note);

    void                  copyNoteWordsListToChord (
                            const S_msrNote& note);

    void                  copyNoteTiesToChord (
                            const S_msrNote& note);

//    void                  copyNoteSlursListToChord (
//                            const S_msrNote& note, S_msrChord chord);

    void                  appendNoteSlursListLinksToChord (
                            const S_msrNote& note);

    void                  copyNoteLigaturesListToChord (
                            const S_msrNote& note);

    void                  copyNotePedalsListToChord (
                            const S_msrNote& note);

    void                  copyNoteSlashesListToChord (
                            const S_msrNote& note);

    void                  copyNoteWedgesListToChord (
                            const S_msrNote& note);

    void                  copyNoteSegnosListToChord (
                            const S_msrNote& note);
    void                  copyNoteDalSegnosListToChord (
                            const S_msrNote& note);
    void                  copyNoteCodasListToChord (
                            const S_msrNote& note);

    void                  copyNoteTechnicalsListToChord (
                            const S_msrNote& note);
    void                  copyNoteTechnicalWithIntegersListToChord (
                            const S_msrNote& note);
    void                  copyNoteTechnicalWithFloatsListToChord (
                            const S_msrNote& note);
    void                  copyNoteTechnicalWithStringsListToChord (
                            const S_msrNote& note);

    void                  copyNoteOrnamentsListToChord (
                            const S_msrNote& note);

    void                  copyNoteStemToChord (
                            const S_msrNote& note);

    void                  copyNoteBeamsListToChord (
                            const S_msrNote& note);

    void                  copyNoteHarmoniesToChord (
                            const S_msrNote& note);

    void                  copyNoteSpannersListToChord (
                            const S_msrNote& note);

    void                  copyNoteSingleTremoloToChord (
                            const S_msrNote& note);

    void                  copyNoteOctaveShiftToChord (
                            const S_msrNote& note);

    void                  addNoteGraceNotesGroupsLinksToChord (
                            const S_msrNote& note);

    void                  appendNoteBeamsListLinksToChord (
                            const S_msrNote& note);

    // articulations
    void                  appendArticulationToChord (
                            const S_msrArticulation& art);

    // spanners
    void                  appendSpannerToChord (
                            const S_msrSpanner& span);

    // technicals
    void                  appendTechnicalToChord (
                            const S_msrTechnical& tech);

    void                  appendTechnicalWithIntegerToChord (
                            const S_msrTechnicalWithInteger& tech);

    void                  appendTechnicalWithFloatToChord (
                            const S_msrTechnicalWithFloat& tech);

    void                  appendTechnicalWithStringToChord (
                            const S_msrTechnicalWithString& tech);

    // ornaments
    void                  appendOrnamentToChord (
                            const S_msrOrnament& orn);

    // glissandos
    void                  appendGlissandoToChord (
                            const S_msrGlissando& gliss);

    // slides
    void                  appendSlideToChord (
                            const S_msrSlide& slide);

    // dynamics
    void                  appendDynamicToChord (
                            const S_msrDynamic& dynamic)
                              { fChordDynamicsList.push_back (dynamic); }

    // other dynamics
    void                  appendOtherDynamicToChord (
                            const S_msrOtherDynamic& otherDynamic)
                              { fChordOtherDynamicsList.push_back (otherDynamic); }

    // slashes
    void                  appendSlashToChord (
                            const S_msrSlash& slash)
                              { fChordSlashesList.push_back (slash); }

    // cresc/decresc
    void                  appendCrescDecrescToChord (
                            const S_msrCrescDecresc& crescDecresc)
                              { fChordCrescDecrescsList.push_back (crescDecresc); }

    // wedges
    void                  appendWedgeToChord (
                            const S_msrWedge& wedge)
                              { fChordWedgesList.push_back (wedge); }

    // segnos
    void                  appendSegnoToChord (
                            const S_msrSegno& segno)
                              { fChordSegnosList.push_back (segno); }

    // dal segnos
    void                  appendDalSegnoToChord (
                            const S_msrDalSegno& dalSegno);

    // codas
    void                  appendCodaToChord (
                              const S_msrCoda& coda)
                              { fChordCodasList.push_back (coda); }

    // words
    void                  appendWordsToChord (
                            const S_msrWords& words)
                              { fChordWordsList.push_back (words); }

    // ties
    void                  appendTieToChord (
                            const S_msrTie& tie)
                              { fChordTiesList.push_back (tie); }

    // slurs
//    void                  appendSlurToChord (const S_msrSlur& slur);
    void                  appendChordSlurLinkToChord (
                            const S_msrChordSlurLink& chordSlurLink);

    // stems
    void                  appendStemToChord (
                            const S_msrStem& stem);

    // beams
    void                  appendBeamToChord (const S_msrBeam& beam);

    void                  appendChordBeamLinkToChord (
                            const S_msrChordBeamLink& chordBeamLink);

    // ligatures
    void                  appendLigatureToChord (
                            const S_msrLigature& ligature)
                              { fChordLigaturesList.push_back (ligature); }

    // pedals
    void                  appendPedalToChord (
                            const S_msrPedal& pedal)
                              { fChordPedalsList.push_back (pedal);  }

    // tuplet members
    /* JMI
    void                  applyTupletMemberDisplayedFactorToChordMembers (
                            int actualNotes, int normalNotes);
*/

//     // finalization
//     void                  finalizeChord (int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asStringwithRawDivisions () const;

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // uplink to containing tuplet
    S_msrTuplet           fChordShortcutUpLinkToContainingTuplet;

    // uplink to containing grace notes group
    S_msrGraceNotesGroup  fChordUpLinkToContainingGraceNotesGroup;

    msrChordInKind        fChordKind;

    // display whole notes
    msrWholeNotes         fChordDisplayWholeNotes;

    // graphic duration is needed for grace notes,
    // since they don't have any note (sounding) duration
    msrNotesDurationKind  fChordGraphicNotesDurationKind;

    std::vector <S_msrNote>
                          fChordNotesVector;

    // stems
    std::list <S_msrStem> fChordStemsList;

    // beams
    std::list <S_msrBeam> fChordBeamsList;

    std::list <S_msrChordBeamLink>
                          fChordBeamLinksList;

    // articulations
    std::list <S_msrArticulation>
                          fChordArticulationsList;

    // spanners
    std::list <S_msrSpanner>
                          fChordSpannersList;

    // single tremolo
    S_msrSingleTremolo    fChordSingleTremolo;

    // double tremolo
    Bool                  fChordIsFirstChordInADoubleTremolo;
    Bool                  fChordIsSecondChordInADoubleTremolo;

    // technicals
    std::list <S_msrTechnical>
                          fChordTechnicalsList;

    std::list <S_msrTechnicalWithInteger>
                          fChordTechnicalWithIntegersList;

    std::list <S_msrTechnicalWithFloat>
                          fChordTechnicalWithFloatsList;

    std::list <S_msrTechnicalWithString>
                          fChordTechnicalWithStringsList;

    // ornaments
    std::list <S_msrOrnament>
                          fChordOrnamentsList;

    // glissandos
    std::list <S_msrGlissando>
                          fChordGlissandosList;

    // slides
    std::list <S_msrSlide> fChordSlidesList;

    // dynamics
    std::list <S_msrDynamic>
                          fChordDynamicsList;
    std::list <S_msrOtherDynamic>
                          fChordOtherDynamicsList;

    // slashes
    std::list <S_msrSlash> fChordSlashesList;

    // cresc/decresc
    std::list <S_msrCrescDecresc>
                          fChordCrescDecrescsList;

    // wedges
    std::list <S_msrWedge> fChordWedgesList;

    // segnos
    std::list <S_msrSegno> fChordSegnosList;

    // dal segnos
    std::list <S_msrDalSegno>
                          fChordDalSegnosList;

    // coda
    std::list <S_msrCoda>  fChordCodasList;

    // octave shift
    S_msrOctaveShift      fChordOctaveShift;

    // words
    std::list <S_msrWords> fChordWordsList;

    // ties
    std::list <S_msrTie>   fChordTiesList;

    // slurs
    std::list <S_msrChordSlurLink>
                          fChordSlurLinksList;

    // ligatures
    std::list <S_msrLigature>

                          fChordLigaturesList;

    // pedals
    std::list <S_msrPedal> fChordPedalsList;

    // grace notes
//    S_msrGraceNotesGroup  fChordGraceNotesGroupBefore;
//    S_msrGraceNotesGroup  fChordGraceNotesGroupAfter;

    S_msrChordGraceNotesGroupLink
                          fChordGraceNotesGroupLinkBefore;
    S_msrChordGraceNotesGroupLink
                          fChordGraceNotesGroupLinkAfter;

    // harmony
    std::list <S_msrHarmony>
                          fChordHarmoniesList;

    // figured bass
    S_msrFiguredBass      fChordFiguredBassesList;
};
typedef SMARTP<msrChord> S_msrChord;
EXP std::ostream& operator << (std::ostream& os, const S_msrChord& elt);

//______________________________________________________________________________
class EXP msrChordBeamLink : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordBeamLink> create (
                            int               inputLineNumber,
                            const S_msrBeam&  originalBeam,
                            const S_msrChord& upLinkToChord);

    SMARTP<msrChordBeamLink> createBeamNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChordBeamLink (
                            int               inputLineNumber,
                            const S_msrBeam&  originalBeam,
                            const S_msrChord& upLinkToChord);

    virtual               ~msrChordBeamLink ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrChord            getUpLinkToChord () const
                              { return fUpLinkToChord; }

    S_msrBeam             getOriginalBeam () const
                              { return fOriginalBeam; }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrChord            fUpLinkToChord;

    S_msrBeam             fOriginalBeam;
};
typedef SMARTP<msrChordBeamLink> S_msrChordBeamLink;
EXP std::ostream& operator << (std::ostream& os, const S_msrChordBeamLink& elt);

//______________________________________________________________________________
class EXP msrChordSlurLink : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordSlurLink> create (
                            int               inputLineNumber,
                            const S_msrSlur&  originalSlur,
                            const S_msrChord& upLinkToChord);

    SMARTP<msrChordSlurLink> createSlurNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChordSlurLink (
                            int               inputLineNumber,
                            const S_msrSlur&  originalSlur,
                            const S_msrChord& upLinkToChord);

    virtual               ~msrChordSlurLink ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrChord            getUpLinkToChord () const
                              { return fUpLinkToChord; }

    S_msrSlur             getOriginalSlur () const
                              { return fOriginalSlur; }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrChord            fUpLinkToChord;

    S_msrSlur             fOriginalSlur;
};
typedef SMARTP<msrChordSlurLink> S_msrChordSlurLink;
EXP std::ostream& operator << (std::ostream& os, const S_msrChordSlurLink& elt);

//______________________________________________________________________________
class EXP msrChordGraceNotesGroupLink : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordGraceNotesGroupLink> create (
                            int                         inputLineNumber,
                            const S_msrGraceNotesGroup& originalGraceNotesGroup,
                            const S_msrChord&           upLinkToChord);

    SMARTP<msrChordGraceNotesGroupLink> createChordGraceNotesGroupLinkNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChordGraceNotesGroupLink (
                            int                         inputLineNumber,
                            const S_msrGraceNotesGroup& originalGraceNotesGroup,
                            const S_msrChord&           upLinkToChord);

    virtual               ~msrChordGraceNotesGroupLink ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrChord            getUpLinkToChord () const
                              { return fUpLinkToChord; }

    S_msrGraceNotesGroup  getOriginalGraceNotesGroup () const
                              { return fOriginalGraceNotesGroup; }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrChord            fUpLinkToChord;

    S_msrGraceNotesGroup  fOriginalGraceNotesGroup;
};
typedef SMARTP<msrChordGraceNotesGroupLink> S_msrChordGraceNotesGroupLink;
EXP std::ostream& operator << (std::ostream& os, const S_msrChordGraceNotesGroupLink& elt);


}


#endif // ___msrChords___
