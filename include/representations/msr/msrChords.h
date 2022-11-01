#ifndef ___msrChords___
#define ___msrChords___

#include "msrCodas.h"
#include "msrDynamics.h"
#include "msrElements.h"
#include "msrGlissandos.h"
#include "msrGraceNotesGroups.h"
#include "msrHarmonies.h"
#include "msrInstruments.h"
#include "msrLigatures.h"
#include "msrNotes.h"
#include "msrOctaveShifts.h"
#include "msrOrnaments.h"
#include "msrScores.h"
#include "msrSegnos.h"
#include "msrDalSegnos.h"
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

//______________________________________________________________________________
class msrFiguredBass;
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;

class msrChordSlurLink;
typedef SMARTP<msrChordSlurLink> S_msrChordSlurLink;

class msrChordBeamLink;
typedef SMARTP<msrChordBeamLink> S_msrChordBeamLink;

class msrChordGraceNotesGroupLink;
typedef SMARTP<msrChordGraceNotesGroupLink> S_msrChordGraceNotesGroupLink;

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

class msrOrnament;
typedef SMARTP<msrOrnament> S_msrOrnament;

class msrGlissando;
typedef SMARTP<msrGlissando> S_msrGlissando;

class msrSlide;
typedef SMARTP<msrSlide> S_msrSlide;

class msrPedal;
typedef SMARTP<msrPedal> S_msrPedal;

class msrSegno;
typedef SMARTP<msrSegno> S_msrSegno;

class msrDalSegno;
typedef SMARTP<msrDalSegno> S_msrDalSegno;

//______________________________________________________________________________
class EXP msrChord : public msrTupletElement
{
  public:

    static SMARTP<msrChord> create (
                            int             inputLineNumber,
                            S_msrMeasure    upLinkToMeasure,
                            const Rational& chordSoundingWholeNotes,
                            const Rational& chordDisplayWholeNotes,
                            msrDurationKind chordGraphicDurationKind);

    // creation from MusicXML
    // ------------------------------------------------------

    SMARTP<msrChord> createChordNewbornClone (
                            S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChord (
                            int             inputLineNumber,
                            S_msrMeasure    upLinkToMeasure,
                            const Rational& chordSoundingWholeNotes,
                            const Rational& chordDisplayWholeNotes,
                            msrDurationKind chordGraphicDurationKind);

    virtual               ~msrChord ();

  public:

    // set and get
    // ------------------------------------------------------

    // tuplet upLink
    void                  setChordDirectUpLinkToTuplet (
                            const S_msrTuplet& tuplet)
                              { fChordDirectUpLinkToTuplet = tuplet; }

    S_msrTuplet           getChordDirectUpLinkToTuplet () const
                            { return fChordDirectUpLinkToTuplet; }

    // grace notes group upLink
    void                  setChordDirectUpLinkToGraceNotesGroup (
                            const S_msrGraceNotesGroup& graceNotesGroup)
                              {
                                fChordDirectUpLinkToGraceNotesGroup =
                                  graceNotesGroup;
                              }

    S_msrGraceNotesGroup  getChordDirectUpLinkToGraceNotesGroup () const
                            { return fChordDirectUpLinkToGraceNotesGroup; }

    // position in measure
    void                  setMeasureElementMeasurePosition (
                            const S_msrMeasure measure,
                            const Rational&    measurePosition,
                            const string&      context) override
                              {
                                setChordMeasurePosition (
                                  measure,
                                  measurePosition,
                                  context);
                              }

    void                  setChordMeasurePosition (
                            const S_msrMeasure measure,
                            const Rational&    measurePosition,
                            const string&      context);

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
    const vector<S_msrNote>&
                          getChordNotesVector () const
                              { return fChordNotesVector; }

    // stems
    const list<S_msrStem>&
                          getChordStems () const
                              { return fChordStems; }

    // beams
    /*
    const list<S_msrBeam>&
                          getChordBeams () const
                              { return fChordBeams; }
                              */
    const list<S_msrChordBeamLink>&
                          getChordBeamLinks () const
                              { return fChordBeamLinks; }

    // articulations
    const list<S_msrArticulation>&
                          getChordArticulations () const
                              { return fChordArticulations; }

    // spanners
    const list<S_msrSpanner>&
                          getChordSpanners () const
                              { return fChordSpanners; }

    // technicals
    const list<S_msrTechnical>&
                          getChordTechnicals () const
                              { return fChordTechnicals; }

    const list<S_msrTechnicalWithInteger>&
                          getChordTechnicalWithIntegers () const
                              { return fChordTechnicalWithIntegers; }

    const list<S_msrTechnicalWithFloat>&
                          getChordTechnicalWithFloats () const
                              { return fChordTechnicalWithFloats; }

    const list<S_msrTechnicalWithString>&
                          getChordTechnicalWithStrings () const
                              { return fChordTechnicalWithStrings; }

    // ornaments
    const list<S_msrOrnament>&
                          getChordOrnaments () const
                              { return fChordOrnaments; }

    // glissandos
    const list<S_msrGlissando>&
                          getChordGlissandos () const
                              { return fChordGlissandos; }

    // slides
    const list<S_msrSlide>&
                          getChordSlides () const
                              { return fChordSlides; }

    // singleTremolo
    void                  setChordSingleTremolo (
                            S_msrSingleTremolo trem);

    S_msrSingleTremolo    getChordSingleTremolo () const
                              { return fChordSingleTremolo; }

    // dynamics
    const list<S_msrDynamic>&
                          getChordDynamics () const
                              { return fChordDynamics; }
    const list<S_msrOtherDynamic>&
                          getChordOtherDynamics () const
                              { return fChordOtherDynamics; }

    const list<S_msrCrescDecresc>&
                          getChordCrescDecrescs () const
                              { return fChordCrescDecrescs; }

    const list<S_msrWedge>&
                          getChordWedges () const
                              { return fChordWedges; }

    // segnos
    const list<S_msrSegno>&
                          getChordSegnos () const
                              { return fChordSegnos; }

    // dal segnos
    const list<S_msrDalSegno>&
                          getChordDalSegnos () const
                              { return fChordDalSegnos; }

    // coda
    const list<S_msrCoda>&
                          getChordCodas () const
                              { return fChordCodas; }

    // words
    const list<S_msrWords>&
                          getChordWords () const
                              { return fChordWords; }

    // ties
    const list<S_msrTie>&
                          getChordTies () const
                              { return fChordTies; }

    // slurs
    const list<S_msrChordSlurLink>&
                          getChordSlurLinks () const
                              { return fChordSlurLinks; }

    // ligatures
    const list<S_msrLigature>&
                          getChordLigatures () const
                              { return fChordLigatures; }

    // pedals
    const list<S_msrPedal>&
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
                            S_msrGraceNotesGroup graceNotesGroupBefore)
                              {
                                fChordGraceNotesGroupBefore =
                                  graceNotesGroupBefore;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupBefore () const
                              { return fChordGraceNotesGroupBefore; }

    void                  setChordGraceNotesGroupAfter (
                            S_msrGraceNotesGroup graceNotesGroupAfter)
                              {
                                fChordGraceNotesGroupAfter =
                                  graceNotesGroupAfter;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupAfter () const
                              { return fChordGraceNotesGroupAfter; }
*/

    void                  setChordGraceNotesGroupLinkBefore (
                            int     inputLineNumber,
                            S_msrChordGraceNotesGroupLink
                                    chordChordGraceNotesGroupLinkBefore);

    S_msrChordGraceNotesGroupLink
                          getChordGraceNotesGroupLinkBefore () const
                              { return fChordGraceNotesGroupLinkBefore; }

    void                  setChordGraceNotesGroupLinkAfter (
                            int     inputLineNumber,
                            S_msrChordGraceNotesGroupLink
                                    chordChordGraceNotesGroupLinkAfter);

    S_msrChordGraceNotesGroupLink
                          getChordGraceNotesGroupLinkAfter () const
                              { return fChordGraceNotesGroupLinkAfter; }

    // harmony
    void                  appendHarmonyToChord (
                            S_msrHarmony harmony)
                              { fChordHarmoniesList.push_back (harmony); }

    // figured bass
    void                  setChordFiguredBass (
                            S_msrFiguredBass figuredBass)
                              { fChordFiguredBass = figuredBass; }

    const S_msrFiguredBass&
                          getChordFiguredBass () const
                              { return fChordFiguredBass; }

    // octave shift
    void                  setChordOctaveShift (
                            S_msrOctaveShift octaveShift)
                              { fChordOctaveShift = octaveShift; }

    S_msrOctaveShift      getChordOctaveShift () const
                            { return fChordOctaveShift; }

    // positions in measures
    void                  setChordMembersMeasurePosition (
                            S_msrMeasure    measure,
                            const Rational& measurePosition);

  public:

    // public services
    // ------------------------------------------------------

    // measure upLink
    S_msrMeasure          fetchChordUpLinkToMeasure () const;

    // tuplet upLink
    S_msrTuplet           fetchChordUpLinkToTuplet () const;

    // grace notes group upLink
    S_msrGraceNotesGroup  fetchChordUpLinkToGraceNotesGroup () const;

    // score upLink
    S_msrScore            fetchChordUpLinkToScore () const;

    // notes
    void                  addFirstNoteToChord (
                            S_msrNote  note,
                            S_msrVoice voice);

    void                  addAnotherNoteToChord (
                            S_msrNote note,
                            S_msrVoice voice);

    S_msrNote             fetchChordFirstNonGraceNote () const;

    // articulations
    void                  appendArticulationToChord (S_msrArticulation art);

    // spanners
    void                  appendSpannerToChord (S_msrSpanner span);

    // technicals
    void                  appendTechnicalToChord (S_msrTechnical tech);

    void                  appendTechnicalWithIntegerToChord (
                            S_msrTechnicalWithInteger tech);

    void                  appendTechnicalWithFloatToChord (
                            S_msrTechnicalWithFloat tech);

    void                  appendTechnicalWithStringToChord (
                            S_msrTechnicalWithString tech);

    // ornaments
    void                  appendOrnamentToChord (S_msrOrnament orn);

    // glissandos
    void                  appendGlissandoToChord (S_msrGlissando gliss);

    // slides
    void                  appendSlideToChord (S_msrSlide slide);

    // dynamics
    void                  appendDynamicToChord (S_msrDynamic dynamic)
                              { fChordDynamics.push_back (dynamic); }

    // other dynamics
    void                  appendOtherDynamicToChord (
                            S_msrOtherDynamic otherDynamic)
                              { fChordOtherDynamics.push_back (otherDynamic); }

    // slashes
    void                  appendSlashToChord (S_msrSlash slash)
                              { fChordSlashes.push_back (slash); }

    // cresc/decresc
    void                  appendCrescDecrescToChord (
                            S_msrCrescDecresc crescDecresc)
                              { fChordCrescDecrescs.push_back (crescDecresc); }

    // wedges
    void                  appendWedgeToChord (S_msrWedge wedge)
                              { fChordWedges.push_back (wedge); }

    // segnos
    void                  appendSegnoToChord (S_msrSegno segno)
                              { fChordSegnos.push_back (segno); }

    // dal segnos
    void                  appendDalSegnoToChord (S_msrDalSegno dalSegno);

    // codas
    void                  appendCodaToChord (S_msrCoda coda)
                              { fChordCodas.push_back (coda); }

    // words
    void                  appendWordsToChord (S_msrWords dynamic)
                              { fChordWords.push_back (dynamic); }

    // ties
    void                  appendTieToChord (S_msrTie tie)
                              { fChordTies.push_back (tie); }

    // slurs
//    void                  appendSlurToChord (S_msrSlur slur);
    void                  appendChordSlurLinkToChord (
                            S_msrChordSlurLink chordSlurLink);

    // stems
    void                  appendStemToChord (S_msrStem stem);

    // beams
//    void                  appendBeamToChord (S_msrBeam beam);
    void                  appendChordBeamLinkToChord (
                            S_msrChordBeamLink chordBeamLink);

    // ligatures
    void                  appendLigatureToChord (S_msrLigature ligature)
                              { fChordLigatures.push_back (ligature); }

    // pedals
    void                  appendPedalToChord (S_msrPedal pedal)
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

    string                asStringwithRawDivisions () const;

    string                asShortString () const override;
    string                asString () const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // tuplet uplink
    S_msrTuplet           fChordDirectUpLinkToTuplet;

    // grace notes group uplink
    S_msrGraceNotesGroup  fChordDirectUpLinkToGraceNotesGroup;

    msrChordInKind        fChordKind;

    // sounding whole notes
    // no need for 'Rational fChordSoundingWholeNotes;',
    // since fChordNotesVector [0] contains this information,
    // as do all the other elements in this vector

    // display whole notes
    Rational              fChordDisplayWholeNotes;

    // graphic duration is needed for grace notes,
    // since they don't have any note (sounding) duration
    msrDurationKind       fChordGraphicDurationKind;

    vector<S_msrNote>     fChordNotesVector;

    // stems
    list<S_msrStem>       fChordStems;

    // beams
//    list<S_msrBeam>       fChordBeams;
    list<S_msrChordBeamLink>
                          fChordBeamLinks;

    // articulations
    list<S_msrArticulation>
                          fChordArticulations;

    // spanners
    list<S_msrSpanner>    fChordSpanners;

    // single tremolo
    S_msrSingleTremolo    fChordSingleTremolo;

    // double tremolo
    Bool                  fChordIsFirstChordInADoubleTremolo;
    Bool                  fChordIsSecondChordInADoubleTremolo;

    // technicals
    list<S_msrTechnical>  fChordTechnicals;

    list<S_msrTechnicalWithInteger>
                          fChordTechnicalWithIntegers;

    list<S_msrTechnicalWithFloat>
                          fChordTechnicalWithFloats;

    list<S_msrTechnicalWithString>
                          fChordTechnicalWithStrings;

    // ornaments
    list<S_msrOrnament>   fChordOrnaments;

    // glissandos
    list<S_msrGlissando>  fChordGlissandos;

    // slides
    list<S_msrSlide>      fChordSlides;

    // dynamics
    list<S_msrDynamic>   fChordDynamics;
    list<S_msrOtherDynamic>
                          fChordOtherDynamics;

    // slashes
    list<S_msrSlash>      fChordSlashes;

    // cresc/decresc
    list<S_msrCrescDecresc>
                          fChordCrescDecrescs;

    // wedges
    list<S_msrWedge>      fChordWedges;

    // segnos
    list<S_msrSegno>      fChordSegnos;

    // dal segnos
    list<S_msrDalSegno>   fChordDalSegnos;

    // coda
    list<S_msrCoda>       fChordCodas;

    // octave shift
    S_msrOctaveShift      fChordOctaveShift;

    // words
    list<S_msrWords>      fChordWords;

    // ties
    list<S_msrTie>        fChordTies;

    // slurs
    list<S_msrChordSlurLink>
                          fChordSlurLinks;

    // ligatures
    list<S_msrLigature>   fChordLigatures;

    // pedals
    list<S_msrPedal>      fChordPedals;

    // grace notes
//    S_msrGraceNotesGroup  fChordGraceNotesGroupBefore;
//    S_msrGraceNotesGroup  fChordGraceNotesGroupAfter;

    S_msrChordGraceNotesGroupLink
                          fChordGraceNotesGroupLinkBefore;
    S_msrChordGraceNotesGroupLink
                          fChordGraceNotesGroupLinkAfter;

    // harmony
    list<S_msrHarmony>    fChordHarmoniesList;

    // figured bass
    S_msrFiguredBass      fChordFiguredBass;
};
typedef SMARTP<msrChord> S_msrChord;
EXP ostream& operator << (ostream& os, const S_msrChord& elt);

//______________________________________________________________________________
class EXP msrChordBeamLink : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordBeamLink> create (
                            int        inputLineNumber,
                            S_msrBeam  originalBeam,
                            S_msrChord upLinkToChord);

    SMARTP<msrChordBeamLink> createBeamNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChordBeamLink (
                            int        inputLineNumber,
                            S_msrBeam  originalBeam,
                            S_msrChord upLinkToChord);

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

    string                asShortString () const override;
    string                asString () const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrChord            fUpLinkToChord;

    S_msrBeam             fOriginalBeam;
};
typedef SMARTP<msrChordBeamLink> S_msrChordBeamLink;
EXP ostream& operator << (ostream& os, const S_msrChordBeamLink& elt);

//______________________________________________________________________________
class EXP msrChordSlurLink : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordSlurLink> create (
                            int        inputLineNumber,
                            S_msrSlur  originalSlur,
                            S_msrChord upLinkToChord);

    SMARTP<msrChordSlurLink> createSlurNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChordSlurLink (
                            int        inputLineNumber,
                            S_msrSlur  originalSlur,
                            S_msrChord upLinkToChord);

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

    string                asShortString () const override;
    string                asString () const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrChord            fUpLinkToChord;

    S_msrSlur             fOriginalSlur;
};
typedef SMARTP<msrChordSlurLink> S_msrChordSlurLink;
EXP ostream& operator << (ostream& os, const S_msrChordSlurLink& elt);

//______________________________________________________________________________
class EXP msrChordGraceNotesGroupLink : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordGraceNotesGroupLink> create (
                            int                  inputLineNumber,
                            S_msrGraceNotesGroup originalGraceNotesGroup,
                            S_msrChord            upLinkToChord);

    SMARTP<msrChordGraceNotesGroupLink> createChordGraceNotesGroupLinkNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrChordGraceNotesGroupLink (
                            int                  inputLineNumber,
                            S_msrGraceNotesGroup originalGraceNotesGroup,
                            S_msrChord            upLinkToChord);

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

    string                asShortString () const override;
    string                asString () const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrChord            fUpLinkToChord;

    S_msrGraceNotesGroup  fOriginalGraceNotesGroup;
};
typedef SMARTP<msrChordGraceNotesGroupLink> S_msrChordGraceNotesGroupLink;
EXP ostream& operator << (ostream& os, const S_msrChordGraceNotesGroupLink& elt);


}


#endif
