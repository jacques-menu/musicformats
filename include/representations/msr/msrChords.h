/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
#include "msrTuplets.h"
#include "msrTupletElements.h"
#include "msrWords.h"


namespace MusicFormats
{

// chords
//______________________________________________________________________________

enum class msrChordInKind {
  kChordIn_NO_,

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

    static SMARTP<msrChord> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const Rational&     chordSoundingWholeNotes,
                            const Rational&     chordDisplayWholeNotes,
                            msrDurationKind     chordGraphicDurationKind);

    static SMARTP<msrChord> create (
                            int                 inputLineNumber,
                            const Rational&     chordSoundingWholeNotes,
                            const Rational&     chordDisplayWholeNotes,
                            msrDurationKind     chordGraphicDurationKind);

    // creation from MusicXML
    // ------------------------------------------------------

    SMARTP<msrChord> createChordNewbornClone (
                            const S_msrPart& containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChord (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const Rational&     chordSoundingWholeNotes,
                            const Rational&     chordDisplayWholeNotes,
                            msrDurationKind     chordGraphicDurationKind);

    virtual               ~msrChord ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override;

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override;

    void                  setChordUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getChordUpLinkToMeasure () const;

    // uplink to tuplet
    void                  setChordDirectUpLinkToTuplet (
                            const S_msrTuplet& tuplet)
                              { fChordDirectUpLinkToTuplet = tuplet; }

    S_msrTuplet           getChordDirectUpLinkToTuplet () const
                            { return fChordDirectUpLinkToTuplet; }

    // uplink to grace notes group
    void                  setChordDirectUpLinkToGraceNotesGroup (
                            const S_msrGraceNotesGroup& graceNotesGroup)
                              {
                                fChordDirectUpLinkToGraceNotesGroup =
                                  graceNotesGroup;
                              }

    S_msrGraceNotesGroup  getChordDirectUpLinkToGraceNotesGroup () const
                            { return fChordDirectUpLinkToGraceNotesGroup; }

    // chord kind
    void                  setChordKind (
                            msrChordInKind chordKind)
                              { fChordKind = chordKind; }

    msrChordInKind        getChordKind () const
                              { return fChordKind; }

     // whole notes
    void                  setChordDisplayWholeNotes (
                            const Rational& wholeNotes);

    Rational              getChordDisplayWholeNotes () const
                             { return fChordDisplayWholeNotes; }

    // graphic duration
    msrDurationKind       getChordGraphicDurationKind () const
                              { return fChordGraphicDurationKind; }

    // chord notes
    const std::vector<S_msrNote>&
                          getChordNotesVector () const
                              { return fChordNotesVector; }

    // stems
    const std::list<S_msrStem>&
                          getChordStems () const
                              { return fChordStems; }

    // beams
    /*
    const std::list<S_msrBeam>&
                          getChordBeams () const
                              { return fChordBeams; }
                              */
    const std::list<S_msrChordBeamLink>&
                          getChordBeamLinks () const
                              { return fChordBeamLinks; }

    // articulations
    const std::list<S_msrArticulation>&
                          getChordArticulations () const
                              { return fChordArticulations; }

    // spanners
    const std::list<S_msrSpanner>&
                          getChordSpanners () const
                              { return fChordSpanners; }

    // technicals
    const std::list<S_msrTechnical>&
                          getChordTechnicals () const
                              { return fChordTechnicals; }

    const std::list<S_msrTechnicalWithInteger>&
                          getChordTechnicalWithIntegers () const
                              { return fChordTechnicalWithIntegers; }

    const std::list<S_msrTechnicalWithFloat>&
                          getChordTechnicalWithFloats () const
                              { return fChordTechnicalWithFloats; }

    const std::list<S_msrTechnicalWithString>&
                          getChordTechnicalWithStrings () const
                              { return fChordTechnicalWithStrings; }

    // ornaments
    const std::list<S_msrOrnament>&
                          getChordOrnaments () const
                              { return fChordOrnaments; }

    // glissandos
    const std::list<S_msrGlissando>&
                          getChordGlissandos () const
                              { return fChordGlissandos; }

    // slides
    const std::list<S_msrSlide>&
                          getChordSlides () const
                              { return fChordSlides; }

    // singleTremolo
    void                  setChordSingleTremolo (
                            const S_msrSingleTremolo& trem);

    S_msrSingleTremolo    getChordSingleTremolo () const
                              { return fChordSingleTremolo; }

    // dynamics
    const std::list<S_msrDynamic>&
                          getChordDynamics () const
                              { return fChordDynamics; }
    const std::list<S_msrOtherDynamic>&
                          getChordOtherDynamics () const
                              { return fChordOtherDynamics; }

    const std::list<S_msrCrescDecresc>&
                          getChordCrescDecrescs () const
                              { return fChordCrescDecrescs; }

    const std::list<S_msrWedge>&
                          getChordWedges () const
                              { return fChordWedges; }

    // segnos
    const std::list<S_msrSegno>&
                          getChordSegnos () const
                              { return fChordSegnos; }

    // dal segnos
    const std::list<S_msrDalSegno>&
                          getChordDalSegnos () const
                              { return fChordDalSegnos; }

    // coda
    const std::list<S_msrCoda>&
                          getChordCodas () const
                              { return fChordCodas; }

    // words
    const std::list<S_msrWords>&
                          getChordWords () const
                              { return fChordWords; }

    // ties
    const std::list<S_msrTie>&
                          getChordTies () const
                              { return fChordTies; }

    // slurs
    const std::list<S_msrChordSlurLink>&
                          getChordSlurLinks () const
                              { return fChordSlurLinks; }

    // ligatures
    const std::list<S_msrLigature>&
                          getChordLigatures () const
                              { return fChordLigatures; }

    // pedals
    const std::list<S_msrPedal>&
                          getChordPedals () const
                              { return fChordPedals; }

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
    void                  setChordFiguredBass (
                            const S_msrFiguredBass& figuredBass)
                              { fChordFiguredBass = figuredBass; }

    const S_msrFiguredBass&
                          getChordFiguredBass () const
                              { return fChordFiguredBass; }

    // octave shift
    void                  setChordOctaveShift (
                            const S_msrOctaveShift& octaveShift)
                              { fChordOctaveShift = octaveShift; }

    S_msrOctaveShift      getChordOctaveShift () const
                            { return fChordOctaveShift; }

    // positions in measures
    void                  setChordMembersMeasurePosition (
                            const S_msrMeasure&    measure,
                            const Rational& measurePosition);

  public:

    // public services
    // ------------------------------------------------------

    // uplink to tuplet
    S_msrTuplet           fetchChordUpLinkToTuplet () const;

    // uplink to grace notes group
    S_msrGraceNotesGroup  fetchChordUpLinkToGraceNotesGroup () const;

    // score upLink
    S_msrScore            fetchChordUpLinkToScore () const;

    // notes
    void                  addFirstNoteToChord (
                            const S_msrNote&  note,
                            const S_msrVoice& voice);

    void                  addAnotherNoteToChord (
                            const S_msrNote& note,
                            const S_msrVoice& voice);

    S_msrNote             fetchChordFirstNonGraceNote () const;

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
                              { fChordDynamics.push_back (dynamic); }

    // other dynamics
    void                  appendOtherDynamicToChord (
                            const S_msrOtherDynamic& otherDynamic)
                              { fChordOtherDynamics.push_back (otherDynamic); }

    // slashes
    void                  appendSlashToChord (
                            const S_msrSlash& slash)
                              { fChordSlashes.push_back (slash); }

    // cresc/decresc
    void                  appendCrescDecrescToChord (
                            const S_msrCrescDecresc& crescDecresc)
                              { fChordCrescDecrescs.push_back (crescDecresc); }

    // wedges
    void                  appendWedgeToChord (
                            const S_msrWedge& wedge)
                              { fChordWedges.push_back (wedge); }

    // segnos
    void                  appendSegnoToChord (
                            const S_msrSegno& segno)
                              { fChordSegnos.push_back (segno); }

    // dal segnos
    void                  appendDalSegnoToChord (
                            const S_msrDalSegno& dalSegno);

    // codas
    void                  appendCodaToChord (
                              const S_msrCoda& coda)
                              { fChordCodas.push_back (coda); }

    // words
    void                  appendWordsToChord (
                            const S_msrWords& words)
                              { fChordWords.push_back (words); }

    // ties
    void                  appendTieToChord (
                            const S_msrTie& tie)
                              { fChordTies.push_back (tie); }

    // slurs
//    void                  appendSlurToChord (const S_msrSlur& slur);
    void                  appendChordSlurLinkToChord (
                            const S_msrChordSlurLink& chordSlurLink);

    // stems
    void                  appendStemToChord (
                            const S_msrStem& stem);

    // beams
//    void                  appendBeamToChord (const S_msrBeam& beam);
    void                  appendChordBeamLinkToChord (
                            const S_msrChordBeamLink& chordBeamLink);

    // ligatures
    void                  appendLigatureToChord (
                            const S_msrLigature& ligature)
                              { fChordLigatures.push_back (ligature); }

    // pedals
    void                  appendPedalToChord (
                            const S_msrPedal& pedal)
                              { fChordPedals.push_back (pedal);  }

    // tuplet members
    /* JMI
    void                  applyTupletMemberDisplayedFactorToChordMembers (
                            int actualNotes, int normalNotes);
*/

    // finalization
    void                  finalizeChord (int inputLineNumber);

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

    void                  printShort (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // uplink to measure
    S_msrMeasure          fChordUpLinkToMeasure;

    // uplink to tuplet
    S_msrTuplet           fChordDirectUpLinkToTuplet;

    // uplink to grace notes group
    S_msrGraceNotesGroup  fChordDirectUpLinkToGraceNotesGroup;

    msrChordInKind        fChordKind;

    // sounding whole notes
    // no need for 'Rational fChordSoundingWholeNotes;',
    // since fChordNotesVector [0] contains this information,
    // as do all the other elements in this std::vector

    // display whole notes
    Rational              fChordDisplayWholeNotes;

    // graphic duration is needed for grace notes,
    // since they don't have any note (sounding) duration
    msrDurationKind       fChordGraphicDurationKind;

    std::vector<S_msrNote>
                          fChordNotesVector;

    // stems
    std::list<S_msrStem>  fChordStems;

    // beams
//    std::list<S_msrBeam>  fChordBeams;
    std::list<S_msrChordBeamLink>
                          fChordBeamLinks;

    // articulations
    std::list<S_msrArticulation>
                          fChordArticulations;

    // spanners
    std::list<S_msrSpanner>
                          fChordSpanners;

    // single tremolo
    S_msrSingleTremolo    fChordSingleTremolo;

    // double tremolo
    Bool                  fChordIsFirstChordInADoubleTremolo;
    Bool                  fChordIsSecondChordInADoubleTremolo;

    // technicals
    std::list<S_msrTechnical>
                          fChordTechnicals;

    std::list<S_msrTechnicalWithInteger>
                          fChordTechnicalWithIntegers;

    std::list<S_msrTechnicalWithFloat>
                          fChordTechnicalWithFloats;

    std::list<S_msrTechnicalWithString>
                          fChordTechnicalWithStrings;

    // ornaments
    std::list<S_msrOrnament>
                          fChordOrnaments;

    // glissandos
    std::list<S_msrGlissando>
                          fChordGlissandos;

    // slides
    std::list<S_msrSlide> fChordSlides;

    // dynamics
    std::list<S_msrDynamic>
                          fChordDynamics;
    std::list<S_msrOtherDynamic>
                          fChordOtherDynamics;

    // slashes
    std::list<S_msrSlash> fChordSlashes;

    // cresc/decresc
    std::list<S_msrCrescDecresc>
                          fChordCrescDecrescs;

    // wedges
    std::list<S_msrWedge> fChordWedges;

    // segnos
    std::list<S_msrSegno> fChordSegnos;

    // dal segnos
    std::list<S_msrDalSegno>
                          fChordDalSegnos;

    // coda
    std::list<S_msrCoda>  fChordCodas;

    // octave shift
    S_msrOctaveShift      fChordOctaveShift;

    // words
    std::list<S_msrWords> fChordWords;

    // ties
    std::list<S_msrTie>   fChordTies;

    // slurs
    std::list<S_msrChordSlurLink>
                          fChordSlurLinks;

    // ligatures
    std::list<S_msrLigature>

                          fChordLigatures;

    // pedals
    std::list<S_msrPedal> fChordPedals;

    // grace notes
//    S_msrGraceNotesGroup  fChordGraceNotesGroupBefore;
//    S_msrGraceNotesGroup  fChordGraceNotesGroupAfter;

    S_msrChordGraceNotesGroupLink
                          fChordGraceNotesGroupLinkBefore;
    S_msrChordGraceNotesGroupLink
                          fChordGraceNotesGroupLinkAfter;

    // harmony
    std::list<S_msrHarmony>
                          fChordHarmoniesList;

    // figured bass
    S_msrFiguredBass      fChordFiguredBass;
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
                            int        inputLineNumber,
                            const S_msrBeam&  originalBeam,
                            const S_msrChord& upLinkToChord);

    SMARTP<msrChordBeamLink> createBeamNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChordBeamLink (
                            int        inputLineNumber,
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

    void                  printShort (std::ostream& os) const override;

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
                            int        inputLineNumber,
                            const S_msrSlur&  originalSlur,
                            const S_msrChord& upLinkToChord);

    SMARTP<msrChordSlurLink> createSlurNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChordSlurLink (
                            int        inputLineNumber,
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

    void                  printShort (std::ostream& os) const override;

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
                            int                  inputLineNumber,
                            const S_msrGraceNotesGroup& originalGraceNotesGroup,
                            const S_msrChord&           upLinkToChord);

    SMARTP<msrChordGraceNotesGroupLink> createChordGraceNotesGroupLinkNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChordGraceNotesGroupLink (
                            int                  inputLineNumber,
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

    void                  printShort (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrChord            fUpLinkToChord;

    S_msrGraceNotesGroup  fOriginalGraceNotesGroup;
};
typedef SMARTP<msrChordGraceNotesGroupLink> S_msrChordGraceNotesGroupLink;
EXP std::ostream& operator << (std::ostream& os, const S_msrChordGraceNotesGroupLink& elt);


}


#endif
