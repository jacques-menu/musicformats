/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSingleTremolos___
#define ___msrSingleTremolos___

#include "msrElements.h"

#include "msrTypesForwardDeclarations.h"

#include "mfDurationsAndPositionInMeasures.h"
#include "msrPlacements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrSingleTremolo : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSingleTremolo> create (
                            const mfInputLineNumber& inputLineNumber,
                            int              singleTremoloMarksNumber,
                            msrPlacementKind singleTremoloPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSingleTremolo (
                            const mfInputLineNumber& inputLineNumber,
                            int              singleTremoloMarksNumber,
                            msrPlacementKind singleTremoloPlacementKind);

    virtual               ~msrSingleTremolo ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSingleTremoloPlacementKind (
                            msrPlacementKind
                              SingleTremoloPlacementKind)
                              {
                                fSingleTremoloPlacementKind =
                                  SingleTremoloPlacementKind;
                              }

    int                   getSingleTremoloMarksNumber () const
                              { return fSingleTremoloMarksNumber; }

    msrPlacementKind      getSingleTremoloPlacementKind () const
                              { return fSingleTremoloPlacementKind; }

    void                  setSingleTremoloGraphicNotesDurationKind (
                            mfDurationKind notesDurationKind)
                              {
                                fSingleTremoloGraphicNotesDurationKind =
                                  notesDurationKind;
                              }

    mfDurationKind  getSingleTremoloGraphicNotesDurationKind () const
                              {
                                return fSingleTremoloGraphicNotesDurationKind;
                              }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           singleTremoloPlacementKindAsString () const;

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fSingleTremoloMarksNumber;

    msrPlacementKind      fSingleTremoloPlacementKind;

    mfDurationKind  fSingleTremoloGraphicNotesDurationKind;
};
typedef SMARTP<msrSingleTremolo> S_msrSingleTremolo;
EXP std::ostream& operator << (std::ostream& os, const S_msrSingleTremolo& elt);


}


#endif // ___msrSingleTremolos___
