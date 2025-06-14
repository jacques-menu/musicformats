/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrDoubleTremolos___
#define ___msrDoubleTremolos___

#include "msrMeasureElements.h"
#include "msrPlacements.h"
#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

// tremolo types
//______________________________________________________________________________
enum class msrDoubleTremoloTypeKind {
  kDoubleTremoloType_UNKNOWN_,
  kDoubleTremoloTypeSingle, kDoubleTremoloTypeStart, kDoubleTremoloTypeStop
};

std::string msrDoubleTremoloTypeKindAsString (
  msrDoubleTremoloTypeKind tremoloTypeKind);

std::ostream& operator << (std::ostream& os, const msrDoubleTremoloTypeKind& elt);

//______________________________________________________________________________
// data types

enum class msrDoubleTremoloKind {
  kDoubleTremoloNotes, kDoubleTremoloChords
};

std::string msrDoubleTremoloKindAsString (
  msrDoubleTremoloKind doubleTremolotKind);

std::ostream& operator << (std::ostream& os, const msrDoubleTremoloKind& elt);

class EXP msrDoubleTremolo : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDoubleTremolo> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&       upLinkToMeasure,
                            msrDoubleTremoloKind      doubleTremoloKind,
                            msrDoubleTremoloTypeKind  doubleDoubleTremoloTypeKind,
                            int                       doubleTremoloMarksNumber,
                            msrPlacementKind          doubleTremoloPlacementKind);


    static SMARTP<msrDoubleTremolo> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrDoubleTremoloKind      doubleTremoloKind,
                            msrDoubleTremoloTypeKind  doubleDoubleTremoloTypeKind,
                            int                       doubleTremoloMarksNumber,
                            msrPlacementKind          doubleTremoloPlacementKind);


    SMARTP<msrDoubleTremolo> createDoubleTremoloNewbornClone (
                            const S_msrElement& doubleTremoloFirstElement,
                            const S_msrElement& doubleTremoloSecondElement);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDoubleTremolo (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&       upLinkToMeasure,
                            msrDoubleTremoloKind      doubleTremoloKind,
                            msrDoubleTremoloTypeKind  doubleDoubleTremoloTypeKind,
                            int                       doubleTremoloMarksNumber,
                            msrPlacementKind          doubleTremoloPlacementKind);

    virtual               ~msrDoubleTremolo ();

  public:

    // set and get
    // ------------------------------------------------------

    // double tremolo kind

    void                  setDoubleTremoloKind (
                            msrDoubleTremoloKind doubleTremoloKind)
                              { fDoubleTremoloKind = doubleTremoloKind; }

    msrDoubleTremoloKind  getDoubleTremoloKind () const
                              { return fDoubleTremoloKind; }

    // double tremolo type kind

    void                  setDoubleDoubleTremoloTypeKind (
                            msrDoubleTremoloTypeKind doubleDoubleTremoloTypeKind)
                              {
                                fDoubleDoubleTremoloTypeKind =
                                  doubleDoubleTremoloTypeKind;
                              }

    msrDoubleTremoloTypeKind
                          getDoubleDoubleTremoloTypeKind () const
                              { return fDoubleDoubleTremoloTypeKind; }

    // double tremolo placement

    void                  setDoubleTremoloPlacementKind (
                            msrPlacementKind
                              doubleTremoloPlacementKind)
                              {
                                fDoubleTremoloPlacementKind =
                                  doubleTremoloPlacementKind;
                              }

    msrPlacementKind      getDoubleTremoloPlacementKind () const
                              { return fDoubleTremoloPlacementKind; }

    // double tremolo marks number

    int                   getDoubleTremoloMarksNumber () const
                              { return fDoubleTremoloMarksNumber; }

    // double tremolo elements duration

    mfWholeNotes          getDoubleTremoloElementsNotesDuration () const
                              { return fDoubleTremoloElementsWholeNotes; }

    // double tremolo number of repeats

    void                  setDoubleTremoloNumberOfRepeats (
                            int doubleTremoloNumberOfRepeats)
                              {
                                fDoubleTremoloNumberOfRepeats =
                                  doubleTremoloNumberOfRepeats;
                              }

    int                   getDoubleTremoloNumberOfRepeats () const
                              {
                                return
                                  fDoubleTremoloNumberOfRepeats;
                              }


    // double tremolo placement

    void                  setDoubleTremoloNoteFirstElement (
                            const S_msrNote& note);

    void                  setDoubleTremoloChordFirstElement (
                            const S_msrChord& chord);

    // double tremolo first element

    S_msrElement          getDoubleTremoloFirstElement () const
                              { return fDoubleTremoloFirstElement; }

    // double tremolo second element

    void                  setDoubleTremoloNoteSecondElement (
                            const S_msrNote& note);

    void                  setDoubleTremoloChordSecondElement (
                            const S_msrChord& chord);

    S_msrElement          getDoubleTremoloSecondElement () const
                              { return fDoubleTremoloSecondElement; }

    // measure number

    void                  setDoubleTremoloMeasureNumber (
                            const mfMeasureNumber& measureNumber);

  public:

    // public services
    // ------------------------------------------------------

    // tremolo first note

    void                  setDoubleTremoloFirstNoteMeasureNumber (
                            const mfMeasureNumber& measureNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           doubleTremoloPlacementKindAsString () const;

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // sounding whole notes JMI
    // the same as the displayed divisions of both members
// JMI    mfWholeNotes           fDoubleTremoloSoundingWholeNotes;
//     mfWholeNotes          fDoubleTremoloSoundingWholeNotes;

    msrDoubleTremoloKind  fDoubleTremoloKind;

    msrDoubleTremoloTypeKind
                          fDoubleDoubleTremoloTypeKind;

    int                   fDoubleTremoloMarksNumber;

    mfWholeNotes          fDoubleTremoloElementsWholeNotes;

    int                   fDoubleTremoloNumberOfRepeats;

    msrPlacementKind      fDoubleTremoloPlacementKind;

    // the two elements of a double tremolo are notes or chords
    S_msrElement          fDoubleTremoloFirstElement;
    S_msrElement          fDoubleTremoloSecondElement;
};
typedef SMARTP<msrDoubleTremolo> S_msrDoubleTremolo;
EXP std::ostream& operator << (std::ostream& os, const S_msrDoubleTremolo& elt);


}


#endif // ___msrDoubleTremolos___
