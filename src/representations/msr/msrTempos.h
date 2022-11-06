/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTempos___
#define ___msrTempos___

#include <list>

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"

#include "msrBeams.h"
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
                            const Rational& tempoNoteWholeNotes,
                            Bool            tempoNoteBelongsToATuplet);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTempoNote (
                            int             inputLineNumber,
                            const Rational& tempoNoteWholeNotes,
                            Bool            tempoNoteBelongsToATuplet);

    virtual               ~msrTempoNote ();

  public:

    // set and get
    // ------------------------------------------------------

    Rational              getTempoNoteWholeNotes () const
                              { return fTempoNoteWholeNotes; }

    const list<S_msrBeam>&
                          getTempoNoteBeams () const
                              { return fTempoNoteBeams; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendBeamToTempoNote (S_msrBeam beam);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    Rational              fTempoNoteWholeNotes;

    list<S_msrBeam>       fTempoNoteBeams;

    Bool                  fTempoNoteBelongsToATuplet;
};
typedef SMARTP<msrTempoNote> S_msrTempoNote;
EXP ostream& operator << (ostream& os, const S_msrTempoNote& elt);

//______________________________________________________________________________
// data types

enum class msrTempoTupletTypeKind {
  kTempoTupletTypeNone,
  kTempoTupletTypeStart, kTempoTupletTypeStop
};

string msrTempoTupletTypeKindAsString (
  msrTempoTupletTypeKind tempoTupletTypeKind);

ostream& operator << (ostream& os, const msrTempoTupletTypeKind& elt);

enum class msrTempoTupletBracketKind {
  kTempoTupletBracketYes, kTempoTupletBracketNo
};

string msrTempoTupletBracketKindAsString (
  msrTempoTupletBracketKind tempoTupletBracketKind);

ostream& operator << (ostream& os, const msrTempoTupletBracketKind& elt);

enum class msrTempoTupletShowNumberKind {
  kTempoTupletShowNumberActual,
  kTempoTupletShowNumberBoth,
  kTempoTupletShowNumberNone
};

string msrTempoTupletShowNumberKindAsString (
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind);

ostream& operator << (ostream& os, const msrTempoTupletShowNumberKind& elt);

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
      msrTupletFactor              tempoTupletFactor,
      const Rational&              memberNotesDisplayWholeNotes);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTempoTuplet (
      int                          inputLineNumber,
      int                          tempoTupletNumber,
      msrTempoTupletBracketKind    tempoTupletBracketKind,
      msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
      msrTupletFactor              tempoTupletFactor,
      const Rational&              memberNotesDisplayWholeNotes);

    virtual               ~msrTempoTuplet ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getTempoTupletNumber () const
                              { return fTempoTupletNumber; }

    msrTempoTupletBracketKind  getTempoTupletBracketKind () const
                              { return fTempoTupletBracketKind; }

    msrTempoTupletShowNumberKind
                          getTempoTupletShowNumberKind () const
                              { return fTempoTupletShowNumberKind; }

    const msrTupletFactor&
                          getTempoTupletFactor () const
                              { return fTempoTupletFactor; }

    Rational              getMemberNotesDisplayWholeNotes () const
                              { return fMemberNotesDisplayWholeNotes; }

    const list<S_msrElement>&
                          getTempoTupletElements () const
                              { return fTempoTupletElements; }

    Rational              getTempoTupletDisplayWholeNotes () const
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
                            S_msrTempoNote tempoNote);
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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fTempoTupletNumber;

    msrTempoTupletBracketKind
                          fTempoTupletBracketKind;

    msrTempoTupletShowNumberKind
                          fTempoTupletShowNumberKind;

    msrTupletFactor       fTempoTupletFactor;

    Rational              fMemberNotesDisplayWholeNotes;

    Rational              fTempoTupletDisplayWholeNotes;

    list<S_msrElement>    fTempoTupletElements;
};
typedef SMARTP<msrTempoTuplet> S_msrTempoTuplet;
EXP ostream& operator << (ostream& os, const S_msrTempoTuplet& elt);

//______________________________________________________________________________
// data types

enum class msrTempoNotesRelationshipElementsKind {
  kTempoNotesRelationshipElementsLeft,
  kTempoNotesRelationshipElementsRight
};

string msrTempoNotesRelationshipElementsKindAsString (
  msrTempoNotesRelationshipElementsKind tempoNotesRelationshipElementsKind);

ostream& operator << (ostream& os, const msrTempoNotesRelationshipElementsKind& elt);

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

    const list<S_msrElement>&
                          getTempoNotesRelationshipElementsList () const
                              { return fTempoNotesRelationshipElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    string                asString () const override;

    void                  addElementToTempoNotesRelationshipElements (
                            S_msrElement element);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    msrTempoNotesRelationshipElementsKind
                          fTempoNotesRelationshipElementsKind;

    list<S_msrElement>    fTempoNotesRelationshipElementsList;
};
typedef SMARTP<msrTempoNotesRelationshipElements> S_msrTempoNotesRelationshipElements;
EXP ostream& operator << (ostream& os, const S_msrTempoNotesRelationshipElements& elt);

//______________________________________________________________________________
// data types

enum class msrTempoKBeatUnitsKind {
  k_NoTempoBeatUnits,
  kTempoBeatUnitsWordsOnly,
  kTempoBeatUnitsPerMinute,
  kTempoBeatUnitsEquivalence,
  kTempoNotesRelationship
};

string msrTempoKBeatUnitsKindAsString (
  msrTempoKBeatUnitsKind tempoKind);

ostream& operator << (ostream& os, const msrTempoKBeatUnitsKind& elt);

enum class msrTempoParenthesizedKind {
  kTempoParenthesizedYes, kTempoParenthesizedNo
};

string msrTempoParenthesizedKindAsString (
  msrTempoParenthesizedKind tempoParenthesizedKind);

ostream& operator << (ostream& os, const msrTempoParenthesizedKind& elt);

enum class msrTempoNotesRelationshipKind {
  kTempoNotesRelationshipNone, kTempoNotesRelationshipEquals
};

string msrTempoNotesRelationshipKindAsString (
  msrTempoNotesRelationshipKind tempoNotesRelationshipKind);

ostream& operator << (ostream& os, const msrTempoNotesRelationshipKind& elt);

class EXP msrTempo : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempo> createTempoWordsOnly (
                            int               inputLineNumber,
                            S_msrMeasure      upLinkToMeasure,
                            S_msrWords        tempoWords,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoPerMinute (
                            int               inputLineNumber,
                            S_msrMeasure      upLinkToMeasure,
                            msrDottedDuration tempoBeatUnit,
                            string            tempoPerMinute,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoBeatUnitEquivalent (
                            int               inputLineNumber,
                            S_msrMeasure      upLinkToMeasure,
                            msrDottedDuration tempoBeatUnit,
                            msrDottedDuration tempoEquivalentBeatUnit,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> createTempoNotesRelationship (
                            int               inputLineNumber,
                            S_msrMeasure      upLinkToMeasure,
                            S_msrTempoNotesRelationshipElements
                                              tempoNotesRelationshipLeftElements,
                            msrTempoNotesRelationshipKind
                                              tempoNotesRelationshipKind,
                            S_msrTempoNotesRelationshipElements
                                              tempoNotesRelationshipRightElements,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTempo (
                            int               inputLineNumber,
                            S_msrMeasure      upLinkToMeasure,
                            S_msrWords        tempoWords,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

                          msrTempo (
                            int               inputLineNumber,
                            S_msrMeasure      upLinkToMeasure,
                            msrDottedDuration tempoBeatUnit,
                            string            tempoPerMinute,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

                          msrTempo (
                            int               inputLineNumber,
                            S_msrMeasure      upLinkToMeasure,
                            msrDottedDuration tempoBeatUnit,
                            msrDottedDuration tempoEquivalentBeatUnit,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

                          msrTempo (
                            int               inputLineNumber,
                            S_msrMeasure      upLinkToMeasure,
                            S_msrTempoNotesRelationshipElements
                                              tempoNotesRelationshipLeftElements,
                            msrTempoNotesRelationshipKind
                                              tempoNotesRelationshipKind,
                            S_msrTempoNotesRelationshipElements
                                              tempoNotesRelationshipRightElements,
                            msrTempoParenthesizedKind
                                              tempoParenthesizedKind,
                            msrPlacementKind  tempoPlacementKind);

    virtual               ~msrTempo ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTempoKBeatUnitsKind          getTempoKind () const
                              { return fTempoKind; }

    const list<S_msrWords>&
                          getTempoWordsList () const
                              { return fTempoWordsList; }

    const msrDottedDuration&
                          getTempoBeatUnit () const
                              { return fTempoBeatUnit; }

    string                getTempoPerMinute () const
                              { return fTempoPerMinute; }

    const msrDottedDuration&
                          getTempoEquivalentBeatUnit () const
                              { return fTempoEquivalentBeatUnit; }

    S_msrTempoNotesRelationshipElements
                          getTempoNotesRelationshipLeftElements () const
                              { return fTempoNotesRelationshipLeftElements; }

    void                  setTempoNotesRelationshipKind (
                            msrTempoNotesRelationshipKind tempoNotesRelationshipKind)
                              { fTempoNotesRelationshipKind = tempoNotesRelationshipKind; }

    msrTempoNotesRelationshipKind  getTempoNotesRelationshipKind () const
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

    string                tempoWordsListAsString (const string& separator) const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;
    string                asShortStringForMeasuresSlices () const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // commmon elements

    msrTempoKBeatUnitsKind          fTempoKind;

    list<S_msrWords>      fTempoWordsList;

    msrDottedDuration     fTempoBeatUnit;

    msrTempoParenthesizedKind
                          fTempoParenthesizedKind;

    msrPlacementKind      fTempoPlacementKind;

    // kTempoBeatUnitsWordsOnly: nothing more

    // kTempoBeatUnitsPerMinute

    string                fTempoPerMinute; // '90' or '132-156' for example

    // kTempoBeatUnitsEquivalence

    msrDottedDuration     fTempoEquivalentBeatUnit; // unused ??? JMI

    // kTempoNotesRelationship

    S_msrTempoNotesRelationshipElements
                          fTempoNotesRelationshipLeftElements;
    msrTempoNotesRelationshipKind  fTempoNotesRelationshipKind;
    S_msrTempoNotesRelationshipElements
                          fTempoNotesRelationshipRightElements;
};
typedef SMARTP<msrTempo> S_msrTempo;
EXP ostream& operator << (ostream& os, const S_msrTempo& elt);


} // namespace MusicFormats


#endif
