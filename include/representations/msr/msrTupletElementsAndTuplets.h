/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTuplets___
#define ___msrTuplets___

#include "msrTypesForwardDeclarations.h"

#include "msrMeasures.h"
#include "msrTupletElements.h"
#include "msrTupletsEnumTypes.h"
#include "msrTupletFactors.h"


namespace MusicFormats
{

class EXP msrTupletElement : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

//   protected:

      // constructors/destructor
      // ------------------------------------------------------

                          msrTupletElement (
                            int                 inputLineNumber);

    virtual               ~msrTupletElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setPositionInTuplet (
                            int positionInTuplet)
                              { fPositionInTuplet = positionInTuplet; }

    const size_t          getPositionInTuplet () const
                              { return fPositionInTuplet; }

  protected:

    // protected fields
    // ------------------------------------------------------

    /*
      The uplinks to the tuplet and measure element are declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

    int                   fPositionInTuplet;
};
typedef SMARTP<msrTupletElement> S_msrTupletElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrTupletElement& elt);

//______________________________________________________________________________
class EXP msrTuplet : public msrTupletElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
                            int                     inputLineNumber,
                            const S_msrMeasure&     upLinkToMeasure,
                            const std::string&           tupletMeasureNumber,
                            int                     tupletNumber,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            const msrTupletFactor&  tupletFactor,
                            const Rational&         memberNotesSoundingWholeNotes,
                            const Rational&         memberNotesDisplayWholeNotes);

    static SMARTP<msrTuplet> create (
                            int                     inputLineNumber,
                            const std::string&           tupletMeasureNumber,
                            int                     tupletNumber,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            const msrTupletFactor&  tupletFactor,
                            const Rational&         memberNotesSoundingWholeNotes,
                            const Rational&         memberNotesDisplayWholeNotes);

    SMARTP<msrTuplet> createTupletNewbornClone ();

    SMARTP<msrTuplet> createTupletDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTuplet (
                            int                     inputLineNumber,
                            const S_msrMeasure&     upLinkToMeasure,
                            const std::string&           tupletMeasureNumber,
                            int                     tupletNumber,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            const msrTupletFactor&  tupletFactor,
                            const Rational&         memberNotesSoundingWholeNotes,
                            const Rational&         memberNotesDisplayWholeNotes);

    virtual               ~msrTuplet ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override;

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override;

    void                  setTupletUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getTupletUpLinkToMeasure () const;

    // tuplet kind
    void                  setTupletKind (
                            msrTupletInKind tupletKind)
                              { fTupletKind = tupletKind; }

    msrTupletInKind       getTupletKind () const
                              { return fTupletKind; }

    // uplink to tuplet
    void                  setTupletShortcutUpLinkToTuplet (
                            const S_msrTuplet& tuplet)
                              { fTupletShortcutUpLinkToTuplet = tuplet; }

    S_msrTuplet           getTupletShortcutUpLinkToTuplet () const
                              { return fTupletShortcutUpLinkToTuplet; }

    // measure positions
    // this override calls setTupletMembersMeasurePositions()
    void                  setMeasureElementMeasurePosition (
                            const S_msrMeasure& measure,
                            const Rational&     measurePosition,
                            const std::string&  context) override;

    void                  setTupletMembersMeasurePositions (
                            const S_msrMeasure& measure,
                            const Rational&     measurePosition);

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
    const std::list<S_msrTupletElement>&
                          getTupletElementsList () const
                              { return fTupletElementsList; }

    // tuplet durations
    Rational              getTupletDisplayWholeNotes () const
                              { return fTupletDisplayWholeNotes; }

  public:

    // public services
    // ------------------------------------------------------

    // uplink to tuplet
    S_msrTuplet           fetchTupletUpLinkToTuplet () const;

    void                  appendNoteToTuplet (
                            const S_msrNote&  note,
                            const S_msrVoice& voice);

    void                  appendChordToTuplet (const S_msrChord& chord);

    void                  appendTupletToTuplet (const S_msrTuplet& tuplet);

    void                  appendTupletToTupletClone (const S_msrTuplet& tuplet);

    S_msrNote             fetchTupletFirstNonGraceNote () const;

    S_msrNote             removeFirstNoteFromTuplet ( // JMI
                            int inputLineNumber);
    S_msrNote             removeLastNoteFromTuplet (
                            int inputLineNumber);

 // JMI   void                  applyDisplayFactorToTupletMembers ();

    void                  unapplySoundingFactorToTupletMembers (
                            const msrTupletFactor& containingTupletFactor);

    // finalization
//     void                  finalizeTuplet (
//                             int inputLineNumber);

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
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // uplink to measure
    S_msrMeasure          fTupletUpLinkToMeasure;

    msrTupletInKind       fTupletKind;

    // uplink to tuplet
    S_msrTuplet           fTupletShortcutUpLinkToTuplet;

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

    // display whole notes
    Rational              fTupletDisplayWholeNotes;

    // elements
    std::list<S_msrTupletElement>
                          fTupletElementsList;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP std::ostream& operator << (std::ostream& os, const S_msrTuplet& elt);


}


#endif
