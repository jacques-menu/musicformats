#ifndef ___msrTuplets___
#define ___msrTuplets___

#include "msrFiguredBasses.h"
#include "msrGraceNotesGroups.h"
#include "msrMeasures.h"
#include "msrTupletElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

class msrChord;
typedef SMARTP<msrChord> S_msrChord;

//______________________________________________________________________________
// PRE-declarations for classes mutual dependencies
class msrTuplet;
typedef SMARTP<msrTuplet> S_msrTuplet;

//______________________________________________________________________________
// data types

enum class msrTupletTypeKind {
  kTupletTypeNone,
  kTupletTypeStart, kTupletTypeContinue, kTupletTypeStop,
  kTupletTypeStartAndStopInARow
};

string tupletTypeKindAsString (
  msrTupletTypeKind tupletTypeKind);

enum class msrTupletBracketKind {
  kTupletBracketYes, kTupletBracketNo
};

string tupletBracketKindAsString (
  msrTupletBracketKind tupletBracketKind);

enum class msrTupletLineShapeKind {
  kTupletLineShapeStraight, kTupletLineShapeCurved
};

string tupletLineShapeKindAsString (
  msrTupletLineShapeKind tupletLineShapeKind);

enum class msrTupletShowNumberKind {
  kTupletShowNumberActual, kTupletShowNumberBoth, kTupletShowNumberNone
};

string tupletShowNumberKindAsString (
  msrTupletShowNumberKind tupletShowNumberKind);

enum class msrTupletShowTypeKind {
  kTupletShowTypeActual, kTupletShowTypeBoth, kTupletShowTypeNone
};

string tupletShowTypeKindAsString (
  msrTupletShowTypeKind tupletShowTypeKind);

class EXP msrTuplet : public msrTupletElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
                            int                     inputLineNumber,
                            S_msrMeasure            upLinkToMeasure,
                            const string&           tupletMeasureNumber,
                            int                     tupletNumber,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            msrTupletFactor         tupletFactor,
                            const Rational&         memberNotesSoundingWholeNotes,
                            const Rational&         memberNotesDisplayWholeNotes);

    SMARTP<msrTuplet> createTupletNewbornClone ();

    SMARTP<msrTuplet> createTupletDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTuplet (
                            int                     inputLineNumber,
                            S_msrMeasure            upLinkToMeasure,
                            const string&           tupletMeasureNumber,
                            int                     tupletNumber,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            msrTupletFactor         tupletFactor,
                            const Rational&         memberNotesSoundingWholeNotes,
                            const Rational&         memberNotesDisplayWholeNotes);

    virtual               ~msrTuplet ();

  public:

    // set and get
    // ------------------------------------------------------

    // tuplet kind
    void                  setTupletKind (
                            msrTupletInKind tupletKind)
                              { fTupletKind = tupletKind; }

    msrTupletInKind       getTupletKind () const
                              { return fTupletKind; }

    // tuplet uplink
    void                  setTupletDirectUpLinkToTuplet (
                            const S_msrTuplet& tuplet)
                              { fTupletDirectUpLinkToTuplet = tuplet; }

    S_msrTuplet           getTupletDirectUpLinkToTuplet () const
                              { return fTupletDirectUpLinkToTuplet; }

    // position in measure
    void                  setMeasureElementMeasurePosition (
                            const S_msrMeasure measure,
                            const Rational&    measurePosition,
                            const string&      context) override
                              {
                                setTupletMeasurePosition (
                                  measure,
                                  measurePosition,
                                  context);
                              }

    void                  setTupletMeasurePosition (
                            const S_msrMeasure measure,
                            const Rational&    measurePosition,
                            const string&      context);

    // members positions in measures
    Rational              setTupletMembersMeasurePositions (
                            S_msrMeasure    measure,
                            const Rational& measurePosition);
                              // returns the position after the tuplet JMI ??? v0.9.66 SUPERFLOUS ???

    // number
    int                   getTupletNumber () const
                              { return fTupletNumber; }

    // appearance
    msrTupletBracketKind  getTupletBracketKind () const
                              { return fTupletBracketKind; }

    msrTupletLineShapeKind
                          getTupletLineShapeKind () const
                              { return fTupletLineShapeKind; }

    msrTupletShowNumberKind
                          getTupletShowNumberKind () const
                              { return fTupletShowNumberKind; }

    msrTupletShowTypeKind getTupletShowTypeKind () const
                              { return fTupletShowTypeKind; }

    // tuplet factor
    const msrTupletFactor&
                          getTupletFactor () const
                              { return fTupletFactor; }

    // member notes durations
    Rational              getMemberNotesSoundingWholeNotes () const
                              { return fMemberNotesSoundingWholeNotes; }
    Rational              getMemberNotesDisplayWholeNotes () const
                              { return fMemberNotesDisplayWholeNotes; }

    // elements
    const list<S_msrTupletElement>&
                          getTupletElementsList () const
                              { return fTupletElementsList; }

    // tuplet durations
    Rational              getTupletDisplayWholeNotes () const
                              { return fTupletDisplayWholeNotes; }

  public:

    // public services
    // ------------------------------------------------------

    // measure upLink
    S_msrMeasure          fetchTupletUpLinkToMeasure () const;

    // tuplet upLink
    S_msrTuplet           fetchTupletUpLinkToTuplet () const;

    void                  appendNoteToTuplet (
                            S_msrNote  note,
                            S_msrVoice voice);

    void                  appendChordToTuplet (S_msrChord chord);

    void                  appendTupletToTuplet (S_msrTuplet tuplet);

    void                  appendTupletToTupletClone (S_msrTuplet tuplet);

    S_msrNote             fetchTupletFirstNonGraceNote () const;

    S_msrNote             removeFirstNoteFromTuplet ( // JMI
                            int inputLineNumber);
    S_msrNote             removeLastNoteFromTuplet (
                            int inputLineNumber);

 // JMI   void                  applyDisplayFactorToTupletMembers ();

    void                  unapplySoundingFactorToTupletMembers (
                            const msrTupletFactor& containingTupletFactor);

    // finalization
    /* JMI
    void                  finalizeTuplet (
                            int inputLineNumber);
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

    virtual void          printShort (ostream& os);

  private:

    // private fields
    // ------------------------------------------------------

    msrTupletInKind       fTupletKind;

    // tuplet uplink
    S_msrTuplet           fTupletDirectUpLinkToTuplet;

    // number
    int                   fTupletNumber;

    // appearance
    msrTupletBracketKind  fTupletBracketKind;

    msrTupletLineShapeKind
                          fTupletLineShapeKind;

    msrTupletShowNumberKind
                          fTupletShowNumberKind;

    msrTupletShowTypeKind fTupletShowTypeKind;

    // tuplet factor
    msrTupletFactor       fTupletFactor;

    // member notes durations
    Rational              fMemberNotesSoundingWholeNotes;
    Rational              fMemberNotesDisplayWholeNotes;

// JMI    Rational              fTupletSoundingWholeNotes;
    // display whole notes
    Rational              fTupletDisplayWholeNotes;

    // elements
    list<S_msrTupletElement>
                          fTupletElementsList;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP ostream& operator << (ostream& os, const S_msrTuplet& elt);


}


#endif