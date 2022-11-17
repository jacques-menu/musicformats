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

#include "msrTupletElements.h"

#include "msrTypesForwardDeclarations.h"

// #include "msrMeasures.h"

#include "msrTupletsEnumTypes.h"

#include "msrTupletFactors.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrTuplet : public msrTupletElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
                            int                     inputLineNumber,
                            const S_msrMeasure&     upLinkToMeasure,
                            const std::string&      tupletMeasureNumber,
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
                            const std::string&      tupletMeasureNumber,
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
                            const std::string&      tupletMeasureNumber,
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

    S_msrMeasure          getMeasureElementUpLinkToMeasure () const override;

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
    void                  setTupletDirectUpLinkToTuplet (
                            const S_msrTuplet& tuplet)
                              { fTupletDirectUpLinkToTuplet = tuplet; }

    S_msrTuplet           getTupletDirectUpLinkToTuplet () const
                              { return fTupletDirectUpLinkToTuplet; }

    // members positions in measures
    Rational              setTupletMembersMeasurePositions (
                            const S_msrMeasure&    measure,
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
                            S_msrNote  note,
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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

    virtual void          printShort (std::ostream& os);

  private:

    // private fields
    // ------------------------------------------------------

    // uplink to measure
    S_msrMeasure          fTupletUpLinkToMeasure;

    msrTupletInKind       fTupletKind;

    // uplink to tuplet
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
    std::list<S_msrTupletElement>
                          fTupletElementsList;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP std::ostream& operator << (std::ostream& os, const S_msrTuplet& elt);


}


#endif
