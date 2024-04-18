/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTempos___
#define ___msrTempos___

#include <list>

#include "msrTemposEnumTypes.h"

#include "msrBeams.h"
#include "msrElements.h"
#include "msrMeasures.h"
#include "msrMeasureElements.h"
#include "msrNotesDurations.h"
#include "msrPlacements.h"
#include "msrTupletFactors.h"
#include "msrWords.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrTempoNote : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempoNote> create (
                            int             inputLineNumber,
                            const msrWholeNotes& tempoNoteWholeNotes,
                            Bool            tempoNoteBelongsToATuplet);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTempoNote (
                            int             inputLineNumber,
                            const msrWholeNotes& tempoNoteWholeNotes,
                            Bool            tempoNoteBelongsToATuplet);

    virtual               ~msrTempoNote ();

  public:

    // set and get
    // ------------------------------------------------------

    msrWholeNotes         getTempoNoteWholeNotes () const
                              { return fTempoNoteWholeNotes; }

    const std::list<S_msrBeam>&
                          getTempoNoteBeamsList () const
                              { return fTempoNoteBeamsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendBeamToTempoNote (const S_msrBeam& beam);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrWholeNotes         fTempoNoteWholeNotes;

    std::list<S_msrBeam>  fTempoNoteBeamsList;

    Bool                  fTempoNoteBelongsToATuplet;
};
typedef SMARTP<msrTempoNote> S_msrTempoNote;
EXP std::ostream& operator << (std::ostream& os, const S_msrTempoNote& elt);

//______________________________________________________________________________
class EXP msrTempoTuplet : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempoTuplet> create (
                            int                          inputLineNumber,
                            int                          tempoTupletNumber,
                            msrTempoTupletBracketKind    tempoTupletBracketKind,
                            msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
                            const msrTupletFactor&       tempoTupletFactor,
                            const msrWholeNotes&     memberNotesDisplayWholeNotes);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTempoTuplet (
                            int                          inputLineNumber,
                            int                          tempoTupletNumber,
                            msrTempoTupletBracketKind    tempoTupletBracketKind,
                            msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
                            const msrTupletFactor&       tempoTupletFactor,
                            const msrWholeNotes&     memberNotesDisplayWholeNotes);

    virtual               ~msrTempoTuplet ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getTempoTupletNumber () const
                              { return fTempoTupletNumber; }

    msrTempoTupletBracketKind
                          getTempoTupletBracketKind () const
                              { return fTempoTupletBracketKind; }

    msrTempoTupletShowNumberKind
                          getTempoTupletShowNumberKind () const
                              { return fTempoTupletShowNumberKind; }

    const msrTupletFactor&
                          getTempoTupletFactor () const
                              { return fTempoTupletFactor; }

    msrWholeNotes         getMemberNotesDisplayWholeNotes () const
                              { return fMemberNotesDisplayWholeNotes; }

    const std::list<S_msrElement>&
                          getTempoTupletElementsList () const
                              { return fTempoTupletElementsList; }

    msrWholeNotes         getTempoTupletDisplayWholeNotes () const
                              { return fTempoTupletDisplayWholeNotes; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addTempoNoteToTempoTuplet (S_msrTempoNote tempoNote);
    /*
    void                  addTempoTupletToTempoTuplet (S_msrTempoTuplet tempoTuplet);

    void                  addTempoTupletToTempoTupletClone (S_msrTempoTuplet tempoTuplet);

    void                  removeFirstNoteFromTempoTuplet (
                            int            inputLineNumber,
                            const S_msrTempoNote& tempoNote);
    */

 // JMI   void                  applyDisplayFactorToTempoTupletMembers ();

    /*
    void                  unapplySoundingFactorToTempoTupletMembers (
                            int containingTempoTupletActualNotes,
                            int containingTempoTupletNormalNotes);
*/

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fTempoTupletNumber;

    msrTempoTupletBracketKind
                          fTempoTupletBracketKind;

    msrTempoTupletShowNumberKind
                          fTempoTupletShowNumberKind;

    msrTupletFactor       fTempoTupletFactor;

    msrWholeNotes         fMemberNotesDisplayWholeNotes;

    msrWholeNotes         fTempoTupletDisplayWholeNotes;

    std::list<S_msrElement>
                          fTempoTupletElementsList;
};
typedef SMARTP<msrTempoTuplet> S_msrTempoTuplet;
EXP std::ostream& operator << (std::ostream& os, const S_msrTempoTuplet& elt);

//______________________________________________________________________________
class EXP msrTempoNotesRelationshipElements : public msrElement
{
  public:

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempoNotesRelationshipElements> create (
                            int      inputLineNumber,
                            msrTempoNotesRelationshipElementsKind
                                     tempoNotesRelationshipElementsKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTempoNotesRelationshipElements (
                            int      inputLineNumber,
                            msrTempoNotesRelationshipElementsKind
                                     tempoNotesRelationshipElementsKind);

    virtual               ~msrTempoNotesRelationshipElements ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTempoNotesRelationshipElementsKind
                          getTempoNotesRelationshipElementsKind () const
                              { return fTempoNotesRelationshipElementsKind; }

    const std::list<S_msrElement>&
                          getTempoNotesRelationshipElementsList () const
                              { return fTempoNotesRelationshipElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  addElementToTempoNotesRelationshipElements (
                            const S_msrElement& element);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    msrTempoNotesRelationshipElementsKind
                          fTempoNotesRelationshipElementsKind;

    std::list<S_msrElement>
                          fTempoNotesRelationshipElementsList;
};
typedef SMARTP<msrTempoNotesRelationshipElements> S_msrTempoNotesRelationshipElements;
EXP std::ostream& operator << (std::ostream& os, const S_msrTempoNotesRelationshipElements& elt);

//______________________________________________________________________________
class EXP msrTempo : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempo> createTempoWordsOnly (
                            int               inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            const S_msrWords&        tempoWords,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoWordsOnly (
                            int               inputLineNumber,
                            const S_msrWords&        tempoWords,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoPerMinute (
                            int               inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            msrDottedNotesDuration tempoBeatUnit,
                            std::string       tempoPerMinute,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoPerMinute (
                            int               inputLineNumber,
                            msrDottedNotesDuration tempoBeatUnit,
                            std::string       tempoPerMinute,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoBeatUnitEquivalent (
                            int               inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            msrDottedNotesDuration tempoBeatUnit,
                            msrDottedNotesDuration tempoEquivalentBeatUnit,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoBeatUnitEquivalent (
                            int               inputLineNumber,
                            msrDottedNotesDuration tempoBeatUnit,
                            msrDottedNotesDuration tempoEquivalentBeatUnit,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoNotesRelationship (
                            int               inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            const S_msrTempoNotesRelationshipElements&
                                              tempoNotesRelationshipLeftElements,
                            msrTempoNotesRelationshipKind
                                              tempoNotesRelationshipKind,
                            const S_msrTempoNotesRelationshipElements&
                                              tempoNotesRelationshipRightElements,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoNotesRelationship (
                            int               inputLineNumber,
                            const S_msrTempoNotesRelationshipElements&
                                              tempoNotesRelationshipLeftElements,
                            msrTempoNotesRelationshipKind
                                              tempoNotesRelationshipKind,
                            const S_msrTempoNotesRelationshipElements&
                                              tempoNotesRelationshipRightElements,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTempo (
                            int               inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            const S_msrWords&        tempoWords,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

                          msrTempo (
                            int               inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            msrDottedNotesDuration tempoBeatUnit,
                            std::string       tempoPerMinute,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

                          msrTempo (
                            int               inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            msrDottedNotesDuration tempoBeatUnit,
                            msrDottedNotesDuration tempoEquivalentBeatUnit,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

                          msrTempo (
                            int               inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            const S_msrTempoNotesRelationshipElements&
                                              tempoNotesRelationshipLeftElements,
                            msrTempoNotesRelationshipKind
                                              tempoNotesRelationshipKind,
                            const S_msrTempoNotesRelationshipElements&
                                              tempoNotesRelationshipRightElements,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    virtual               ~msrTempo ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTempoKBeatUnitsKind
                          getTempoKind () const
                              { return fTempoKind; }

    const std::list<S_msrWords>&
                          getTempoWordsList () const
                              { return fTempoWordsList; }

    const msrDottedNotesDuration&
                          getTempoBeatUnit () const
                              { return fTempoBeatUnit; }

    std::string           getTempoPerMinute () const
                              { return fTempoPerMinute; }

    const msrDottedNotesDuration&
                          getTempoEquivalentBeatUnit () const
                              { return fTempoEquivalentBeatUnit; }

    S_msrTempoNotesRelationshipElements
                          getTempoNotesRelationshipLeftElements () const
                              { return fTempoNotesRelationshipLeftElements; }

    void                  setTempoNotesRelationshipKind (
                            msrTempoNotesRelationshipKind tempoNotesRelationshipKind)
                              { fTempoNotesRelationshipKind = tempoNotesRelationshipKind; }

    msrTempoNotesRelationshipKind

                          getTempoNotesRelationshipKind () const
                              { return fTempoNotesRelationshipKind; }

    S_msrTempoNotesRelationshipElements
                          getTempoNotesRelationshipRightElements () const
                              { return fTempoNotesRelationshipRightElements; }

    msrTempoParenthesizedKind
                          getTempoParenthesizedKind () const
                              { return fTempoParenthesizedKind; }

    msrPlacementKind      getTempoPlacementKind () const
                              { return fTempoPlacementKind; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendWordsToTempo (S_msrWords tempoWords)
                              {
                                fTempoWordsList.push_back (tempoWords);
                              }

    std::string           tempoWordsListAsString (const std::string& separator) const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

//     std::string           asStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // commmon elements

    msrTempoKBeatUnitsKind
                          fTempoKind;

    std::list<S_msrWords> fTempoWordsList;

    msrDottedNotesDuration
                          fTempoBeatUnit;

    msrTempoParenthesizedKind
                          fTempoParenthesizedKind;

    msrPlacementKind      fTempoPlacementKind;

    // kTempoBeatUnitsWordsOnly: nothing more

    // kTempoBeatUnitsPerMinute

    std::string           fTempoPerMinute; // '90' or '132-156' for example

    // kTempoBeatUnitsEquivalence

    msrDottedNotesDuration
                          fTempoEquivalentBeatUnit; // unused ??? JMI

    // kTempoNotesRelationship

    S_msrTempoNotesRelationshipElements
                          fTempoNotesRelationshipLeftElements;
    msrTempoNotesRelationshipKind
                          fTempoNotesRelationshipKind;
    S_msrTempoNotesRelationshipElements
                          fTempoNotesRelationshipRightElements;
};
typedef SMARTP<msrTempo> S_msrTempo;
EXP std::ostream& operator << (std::ostream& os, const S_msrTempo& elt);


}


#endif // ___msrTempos___
