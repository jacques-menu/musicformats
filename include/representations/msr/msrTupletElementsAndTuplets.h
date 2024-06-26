/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTupletElementsAndTuplets___
#define ___msrTupletElementsAndTuplets___

#include "msrTypesForwardDeclarations.h"

#include "msrMeasures.h"
#include "msrTypesForwardDeclarations.h"

#include "msrMeasureElements.h"
#include "msrMeasures.h"
#include "msrTupletsEnumTypes.h"
#include "msrTupletFactors.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrTupletElement : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

//   protected:

      // constructors/destructor
      // ------------------------------------------------------

                          msrTupletElement (
                            int inputLineNumber);

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

    // creation
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
                            int inputLineNumber,
                            int fullTupletElementsNumber);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
                            int                     inputLineNumber,
                            int                     fullTupletElementsNumber,
                            int                     tupletNumber,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletTypeKind       tupletTypeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            const msrTupletFactor&  tupletFactor);

    static SMARTP<msrTuplet> create (
                            int                     inputLineNumber,
                            int                     fullTupletElementsNumber,
                            const S_msrMeasure&     upLinkToMeasure,
                            int                     tupletNumber,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletTypeKind       tupletTypeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            const msrTupletFactor&  tupletFactor);

    // cloning
    // ------------------------------------------------------

    SMARTP<msrTuplet> createTupletNewbornClone ();

    SMARTP<msrTuplet> createTupletDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTuplet (
                            int inputLineNumber,
                            int fullTupletElementsNumber);

                          msrTuplet (
                            int                     inputLineNumber,
                            int                     fullTupletElementsNumber,
                            int                     tupletNumber,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletTypeKind       tupletTypeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            const msrTupletFactor&  tupletFactor);

                          msrTuplet (
                            int                     inputLineNumber,
                            int                     fullTupletElementsNumber,
                            const S_msrMeasure&     upLinkToMeasure,
                            int                     tupletNumber,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletTypeKind       tupletTypeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            const msrTupletFactor&  tupletFactor);

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

    // uplink to tuplet
    void                  setTupletShortcutUpLinkToTuplet (
                            const S_msrTuplet& tuplet)
                              { fTupletShortcutUpLinkToTuplet = tuplet; }

    S_msrTuplet           getTupletShortcutUpLinkToTuplet () const
                              { return fTupletShortcutUpLinkToTuplet; }

    // measure positions
    // this override calls setTupletMembersMeasurePositions()
    void                  setMeasureElementMeasurePosition (
                            const S_msrMeasure&  measure,
                            const msrWholeNotes& measurePosition,
                            const std::string&   context) override;

    void                  setTupletMembersMeasurePositions (
                            const S_msrMeasure&  measure,
                            const msrWholeNotes& measurePosition);

    // number
    int                   getTupletNumber () const
                              { return fTupletNumber; }

    // appearance
    msrTupletBracketKind  getTupletBracketKind () const
                              { return fTupletBracketKind; }

    msrTupletLineShapeKind
                          getTupletLineShapeKind () const
                              { return fTupletLineShapeKind; }

    msrTupletTypeKind     getTupletTypeKind () const
                              { return fTupletTypeKind; }

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
    msrWholeNotes         getMemberNotesSoundingWholeNotes () const
                              { return fMemberNotesSoundingWholeNotes; }
    msrWholeNotes         getMemberNotesDisplayWholeNotes () const
                              { return fMemberNotesDisplayWholeNotes; }

    // elements
    const std::list<S_msrTupletElement>&
                          getTupletElementsList () const
                              { return fTupletElementsList; }

//     void                  setFullTupletElementsNumber (int number)
//                               { fFullTupletElementsNumber = number; }

    int                   getFullTupletElementsNumber () const
                              { return fFullTupletElementsNumber; }

    // tuplet durations
    msrWholeNotes         getTupletDisplayWholeNotes () const
                              { return fTupletDisplayWholeNotes; }

  public:

    // public services
    // ------------------------------------------------------

    // uplink to tuplet
    S_msrTuplet           fetchTupletUpLinkToTuplet () const;

    void                  appendNoteToTuplet (const S_msrNote& note);

    void                  appendChordToTuplet (const S_msrChord& chord);

    void                  appendTupletToTuplet (const S_msrTuplet& tuplet);

    void                  appendTupletToTupletClone (const S_msrTuplet& tuplet);

    Bool                  getTupletHasBeenFilled () const
                              {
                                return
                                  fTupletElementsList.size () == fFullTupletElementsNumber;
                              }

    S_msrNote             fetchTupletFirstNonGraceNote () const;

    S_msrNote             removeFirstNoteFromTuplet ( // JMI
                            int inputLineNumber);

    S_msrNote             removeLastNoteFromTuplet (
                            int inputLineNumber);

    S_msrTupletElement    removeLastElementFromTuplet (
                            int inputLineNumber);

 // JMI   void                  applyDisplayFactorToTupletMembers ();

    void                  unapplySoundingFactorToTupletMembers ( // JMI v0.9.70 ???
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
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // uplink to tuplet
    S_msrTuplet           fTupletShortcutUpLinkToTuplet;

    // tuplet kind
    msrTupletInKind       fTupletKind;

    // number
    int                   fTupletNumber;

    // appearance
    msrTupletBracketKind  fTupletBracketKind;

    msrTupletLineShapeKind
                          fTupletLineShapeKind;

    msrTupletTypeKind     fTupletTypeKind;

    msrTupletShowNumberKind
                          fTupletShowNumberKind;

    msrTupletShowTypeKind fTupletShowTypeKind;

    // tuplet factor
    msrTupletFactor       fTupletFactor;

    // member notes durations
    msrWholeNotes         fMemberNotesSoundingWholeNotes;
    msrWholeNotes         fMemberNotesDisplayWholeNotes;

    // display whole notes
    msrWholeNotes         fTupletDisplayWholeNotes;

    // elements
    std::list<S_msrTupletElement>
                          fTupletElementsList;

    // we need to know when the tuplet has been filled in mxsr2msr
    int                   fFullTupletElementsNumber;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP std::ostream& operator << (std::ostream& os, const S_msrTuplet& elt);


}


#endif // ___msrTupletElementsAndTuplets___
