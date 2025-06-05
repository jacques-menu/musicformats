/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTuplets___
#define ___msrTuplets___

#include "msrMeasures.h"
#include "msrTupletsEnumTypes.h"
#include "msrTupletFactors.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declaration
class msrTuplet;
using S_msrTuplet = SMARTP<msrTuplet>;

//______________________________________________________________________________
class EXP msrTuplet : public msrTupletElement
{
  public:

    // creation
    // ------------------------------------------------------

    static S_msrTuplet    create (
                            const mfInputLineNumber& inputLineNumber,
                            int                     tupletNumber,
                            const msrTupletFactor&  tupletFactor,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletTypeKind       tupletTypeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            msrPlacementKind        tupletBracketPlacementKind);

    // cloning
    // ------------------------------------------------------

    S_msrTuplet           createTupletNewbornClone ();

    S_msrTuplet           createTupletDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTuplet (
                            const mfInputLineNumber& inputLineNumber,
                            int                     tupletNumber,
                            const msrTupletFactor&  tupletFactor,
                            msrTupletBracketKind    tupletBracketKind,
                            msrTupletLineShapeKind  tupletLineShapeKind,
                            msrTupletTypeKind       tupletTypeKind,
                            msrTupletShowNumberKind tupletShowNumberKind,
                            msrTupletShowTypeKind   tupletShowTypeKind,
                            msrPlacementKind        tupletBracketPlacementKind);

    virtual               ~msrTuplet ();

  public:

    // set and get
    // ------------------------------------------------------

    // number
    int                   getTupletNumber () const
                              { return fTupletNumber; };

    // tuplet factor
    const msrTupletFactor&
                          getTupletFactor () const
                              { return fTupletFactor; };

    // tuplet kind
    void                  setTupletKind (
                            msrTupletInKind tupletKind);

    msrTupletInKind       getTupletKind () const
                              { return fTupletKind; };

    // appearance
    msrTupletBracketKind  getTupletBracketKind () const
                              { return fTupletBracketKind; };

    msrTupletLineShapeKind
                          getTupletLineShapeKind () const
                              { return fTupletLineShapeKind; };

    msrTupletTypeKind     getTupletTypeKind () const
                              { return fTupletTypeKind; };

    msrTupletShowNumberKind
                          getTupletShowNumberKind () const
                              { return fTupletShowNumberKind; };

    msrTupletShowTypeKind getTupletShowTypeKind () const
                              { return fTupletShowTypeKind; };

    // tuplets elements list
    const std::list <S_msrTupletElement>&
                          getTupletElementsList () const
                              { return fTupletElementsList; };

    // tuplet display whole notes
    mfWholeNotes          getTupletDisplayWholeNotes () const
                              { return fTupletDisplayWholeNotes; };

    // tuplet placement
    void                  setTupletBracketPlacementKind (
                            msrPlacementKind placementKind)
                              { fTupletBracketPlacementKind = placementKind; };

    msrPlacementKind      getTupletBracketPlacementKind () const
                              { return fTupletBracketPlacementKind; };

    // uplink to tuplet
    void                  setTupletUpLinkToContainingTuplet (
                            const S_msrTuplet& tuplet)
                              { fTupletUpLinkToContainingTuplet = tuplet; };

    S_msrTuplet           getTupletUpLinkToContainingTuplet () const
                              { return fTupletUpLinkToContainingTuplet; };

    // measure positions
    // this override calls setTupletMembersPositionInMeasures()
    // to set the tuplet's members positions in measure
    void                  setMeasureElementPositionInMeasure (
                            const S_msrMeasure&        measure,
                            const mfPositionInMeasure& positionInMeasure,
                            const std::string&         context) override;

    void                  setTupletMembersPositionInMeasures (
                            const S_msrMeasure&        measure,
                            const mfPositionInMeasure& positionInMeasure);

  public:

    // public services
    // ------------------------------------------------------

    void                  appendNoteToTuplet (const S_msrNote& note);

    void                  appendChordToTuplet (const S_msrChord& chord);

    void                  appendTupletToTuplet (const S_msrTuplet& tuplet);

    void                  appendTupletToTupletClone (const S_msrTuplet& tuplet);

    S_msrNote             fetchTupletFirstNonGraceNote () const;

//     S_msrNote             removeFirstNoteFromTuplet ( // JMI
//                             const mfInputLineNumber& inputLineNumber);

//     S_msrNote             removeLastNoteFromTuplet (
//                             const mfInputLineNumber& inputLineNumber);
//
//     S_msrTupletElement    removeLastElementFromTuplet (
//                             const mfInputLineNumber& inputLineNumber);

 // JMI   void                  applyDisplayFactorToTupletMembers ();

    void                  unapplySoundingFactorToTupletMembers ( // JMI 0.9.70 ???
                            const msrTupletFactor& containingTupletFactor);

    // finalization
//     void                  finalizeTuplet (
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

    std::string           asString () const override;
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // uplink to tuplet
    S_msrTuplet           fTupletUpLinkToContainingTuplet;

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

    // display whole notes
    mfWholeNotes          fTupletDisplayWholeNotes;

    // placement
    msrPlacementKind      fTupletBracketPlacementKind;

    // elements list
    std::list <S_msrTupletElement>
                          fTupletElementsList;
};
EXP std::ostream& operator << (std::ostream& os, const S_msrTuplet& elt);


}


#endif // ___msrTuplets___
