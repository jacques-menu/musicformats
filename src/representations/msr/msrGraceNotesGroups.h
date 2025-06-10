/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrGraceNotesGroups___
#define ___msrGraceNotesGroups___

#include "msrElements.h"
#include "mfDurationsAndPositionInMeasures.h"
#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrGraceNotesGroupKind {
  kGraceNotesGroupBefore,
  kGraceNotesGroupAfter
};

std::string msrGraceNotesGroupKindAsString (
  msrGraceNotesGroupKind graceNotesGroupKind);

std::ostream& operator << (std::ostream& os, const msrGraceNotesGroupKind& elt);

//______________________________________________________________________________
class EXP msrGraceNotesGroup : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGraceNotesGroup> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrGraceNotesGroupKind   graceNotesGroupKind,
                            Bool                     graceNotesGroupIsSlashed,
                            Bool                     graceNotesGroupIsBeamed,
                            Bool                     graceNotesGroupIsTied,
                            Bool                     graceNotesGroupIsSlurred,
                            const mfMeasureNumber&   graceNotesGroupMeasureNumber);

    SMARTP<msrGraceNotesGroup> createGraceNotesGroupNewbornClone ();

    SMARTP<msrGraceNotesGroup> createSkipGraceNotesGroupClone ();

    SMARTP<msrGraceNotesGroup> createGraceNotesGroupDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrGraceNotesGroup (
                            const mfInputLineNumber& inputLineNumber,
                            msrGraceNotesGroupKind   graceNotesGroupKind,
                            Bool                     graceNotesGroupIsSlashed,
                            Bool                     graceNotesGroupIsBeamed,
                            Bool                     graceNotesGroupIsTied,
                            Bool                     graceNotesGroupIsSlurred,
                            const mfMeasureNumber&   graceNotesGroupMeasureNumber);

    virtual               ~msrGraceNotesGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setGraceNotesGroupUpLinkToNote (
                            const S_msrNote& note)
                              { fGraceNotesGroupUpLinkToNote = note; }

    S_msrNote             getGraceNotesGroupUpLinkToNote () const
                              { return fGraceNotesGroupUpLinkToNote; }

    msrGraceNotesGroupKind
                          getGraceNotesGroupKind () const
                              { return fGraceNotesGroupKind; }

    void                  setGraceNotesGroupKind (
                            msrGraceNotesGroupKind graceNotesGroupKind)
                              { fGraceNotesGroupKind = graceNotesGroupKind; }

    const std::list <S_msrMeasureElement>&
                          getGraceNotesGroupElementsList () const
                              { return fGraceNotesGroupElementsList; }

    Bool                  getGraceNotesGroupIsSlashed () const
                              { return fGraceNotesGroupIsSlashed; }

    void                  setGraceNotesGroupIsTied ()
                              { fGraceNotesGroupIsTied = true; }

    Bool                  getGraceNotesGroupIsTied () const
                              { return fGraceNotesGroupIsTied; }

    void                  setGraceNotesGroupIsSlurred ()
                              { fGraceNotesGroupIsSlurred = true; }

    Bool                  getGraceNotesGroupIsSlurred () const
                              { return fGraceNotesGroupIsSlurred; }

    void                  setGraceNotesGroupIsBeamed ()
                              { fGraceNotesGroupIsBeamed = true; }

    Bool                  getGraceNotesGroupIsBeamed () const
                              { return fGraceNotesGroupIsBeamed; }

    void                  setGraceNotesGroupIsFollowedByNotes (Bool value)
                              { fGraceNotesGroupIsFollowedByNotes = value; }

    Bool                  getGraceNotesGroupIsFollowedByNotes () const
                              { return fGraceNotesGroupIsFollowedByNotes; }

    void                  setGraceNotesGroupMeasureNumber (
                            const mfMeasureNumber& measureNumber)
                              { fGraceNotesGroupMeasureNumber = measureNumber; }

    mfMeasureNumber       getGraceNotesGroupMeasureNumber () const
                              { return fGraceNotesGroupMeasureNumber; }

    // measure positions
    void                  setGraceNotesGroupElementsPositionInMeasures (
                            const S_msrMeasure&        measure,
                            const mfPositionInMeasure& positionInMeasure);

  public:

    // public services
    // ------------------------------------------------------

    // uplink to tuplet
    S_msrTuplet           fetchGraceNoteGroupUpLinkToTuplet () const;

    // part uplink
    S_msrPart             fetchGraceNotesGroupUpLinkToPart () const;

    void                  appendNoteToGraceNotesGroup (const S_msrNote& note);
    void                  appendChordToGraceNotesGroup (const S_msrChord& chord);

//     S_msrNote             removeLastNoteFromGraceNotesGroup (
//                             const mfInputLineNumber& inputLineNumber);


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

    msrGraceNotesGroupKind
                          fGraceNotesGroupKind;

    // note upLink
    S_msrNote             fGraceNotesGroupUpLinkToNote;

    std::list <S_msrMeasureElement>
                          fGraceNotesGroupElementsList;

    Bool                  fGraceNotesGroupIsSlashed;
    Bool                  fGraceNotesGroupIsTied;
    Bool                  fGraceNotesGroupIsSlurred;
    Bool                  fGraceNotesGroupIsBeamed;

    Bool                  fGraceNotesGroupIsFollowedByNotes;

    // LilyPond_Issue_34 may lead to add skip grace notes to voices
    // other than the one containing these grace notes:
    // the measure number is needed to create the first measure
    // in case the grace notes are at the beginning of the voice
    mfMeasureNumber       fGraceNotesGroupMeasureNumber; // JMI ??? 0.9.75
};
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msrGraceNotesGroup& elt);


}


#endif // ___msrGraceNotesGroups___
