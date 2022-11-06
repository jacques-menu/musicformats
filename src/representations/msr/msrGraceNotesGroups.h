#ifndef ___msrGraceNotesGroups___
#define ___msrGraceNotesGroups___

#include "msrElements.h"
#include "msrMeasures.h"
#include "msrMeasureElements.h"
#include "msrNotes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class   msrChord;
typedef SMARTP<msrChord> S_msrChord;

class   msrTuplet;
typedef SMARTP<msrTuplet> S_msrTuplet;

class   msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

class   msrPart;
typedef SMARTP<msrPart> S_msrPart;

class   msrNote;
typedef SMARTP<msrNote> S_msrNote;

//______________________________________________________________________________
// data types

enum class msrGraceNotesGroupKind {
  kGraceNotesGroupBefore,
  kGraceNotesGroupAfter
};

string msrGraceNotesGroupKindAsString (
  msrGraceNotesGroupKind graceNotesGroupKind);

ostream& operator << (ostream& os, const msrGraceNotesGroupKind& elt);

class EXP msrGraceNotesGroup : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGraceNotesGroup> create (
                            int                    inputLineNumber,
                            msrGraceNotesGroupKind graceNotesGroupKind,
                            Bool                   graceNotesGroupIsSlashed,
                            Bool                   graceNotesGroupIsBeamed,
                            const string&          graceNotesGroupMeasureNumber);

    SMARTP<msrGraceNotesGroup> createGraceNotesGroupNewbornClone ();

    SMARTP<msrGraceNotesGroup> createSkipGraceNotesGroupClone ();

    SMARTP<msrGraceNotesGroup> createGraceNotesGroupDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrGraceNotesGroup (
                            int                    inputLineNumber,
                            msrGraceNotesGroupKind graceNotesGroupKind,
                            Bool                   graceNotesGroupIsSlashed,
                            Bool                   graceNotesGroupIsBeamed,
                            const string&          graceNotesGroupMeasureNumber);

    virtual               ~msrGraceNotesGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setGraceNotesGroupUpLinkToNote (
                            S_msrNote note)
                              { fGraceNotesGroupUpLinkToNote = note; }

    S_msrNote             getGraceNotesGroupUpLinkToNote () const
                              { return fGraceNotesGroupUpLinkToNote; }

    msrGraceNotesGroupKind
                          getGraceNotesGroupKind () const
                              { return fGraceNotesGroupKind; }

    void                  setGraceNotesGroupKind (
                            msrGraceNotesGroupKind graceNotesGroupKind)
                              { fGraceNotesGroupKind = graceNotesGroupKind; }

    const list<S_msrMeasureElement>&
                          getGraceNotesGroupElementsList () const
                              { return fGraceNotesGroupElementsList; }

    Bool                  getGraceNotesGroupIsSlashed () const
                              { return fGraceNotesGroupIsSlashed; }

    void                  setGraceNotesGroupIsTied ()
                              { fGraceNotesGroupIsTied = true; }

    Bool                  getGraceNotesGroupIsTied () const
                              { return fGraceNotesGroupIsTied; }

    void                  setGraceNotesGroupIsBeamed ()
                              { fGraceNotesGroupIsBeamed = true; }

    Bool                  getGraceNotesGroupIsBeamed () const
                              { return fGraceNotesGroupIsBeamed; }

    void                  setGraceNotesGroupIsFollowedByNotes (Bool value)
                              { fGraceNotesGroupIsFollowedByNotes = value; }

    Bool                  getGraceNotesGroupIsFollowedByNotes () const
                              { return fGraceNotesGroupIsFollowedByNotes; }

    void                  setGraceNotesGroupMeasureNumber (
                            const string& measureNumber)
                              {
                                fGraceNotesGroupMeasureNumber = measureNumber;
                              }

    string                getGraceNotesGroupMeasureNumber () const
                              { return fGraceNotesGroupMeasureNumber; }

    // positions in measures
    void                  setGraceNotesGroupElementsMeasurePositions (
                            S_msrMeasure    measure,
                            const Rational& measurePosition);

  public:

    // public services
    // ------------------------------------------------------

    // tuplet uplink
    S_msrTuplet           fetchGraceNoteGroupUpLinkToTuplet () const;

    // part uplink
    S_msrPart             fetchGraceNotesGroupUpLinkToPart () const;

    void                  appendNoteToGraceNotesGroup (S_msrNote note);
    void                  appendChordToGraceNotesGroup (S_msrChord chord);

    S_msrNote             removeLastNoteFromGraceNotesGroup (
                            int inputLineNumber);


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

    msrGraceNotesGroupKind
                          fGraceNotesGroupKind;

    // note upLink
    S_msrNote             fGraceNotesGroupUpLinkToNote;

    list<S_msrMeasureElement>
                          fGraceNotesGroupElementsList;

    Bool                  fGraceNotesGroupIsSlashed;
    Bool                  fGraceNotesGroupIsTied;
    Bool                  fGraceNotesGroupIsBeamed;

    Bool                  fGraceNotesGroupIsFollowedByNotes;

    // LilyPond issue #34 may lead to add skip grace notes to voices
    // other than the one containing these grace notes:
    // the measure number is needed to create the first measure
    // in case the grace notes are at the beginning of the voice
    string                fGraceNotesGroupMeasureNumber; // JMI ???
};
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;
EXP ostream& operator << (ostream& os, const S_msrGraceNotesGroup& elt);


}


#endif
