/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSingleTremolos___
#define ___msrSingleTremolos___

#include "msrElements.h"

// #include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrSingleTremolo : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSingleTremolo> create (
                            int              inputLineNumber,
                            int              singleTremoloMarksNumber,
                            msrPlacementKind singleTremoloPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSingleTremolo (
                            int              inputLineNumber,
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

    void                  setSingleTremoloGraphicDurationKind (
                            msrDurationKind durationKind)
                              {
                                fSingleTremoloGraphicDurationKind =
                                  durationKind;
                              }

    msrDurationKind       getSingleTremoloGraphicDurationKind () const
                              {
                                return fSingleTremoloGraphicDurationKind;
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

    string                singleTremoloPlacementKindAsString () const;

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fSingleTremoloMarksNumber;

    msrPlacementKind      fSingleTremoloPlacementKind;

    msrDurationKind       fSingleTremoloGraphicDurationKind;
};
typedef SMARTP<msrSingleTremolo> S_msrSingleTremolo;
EXP ostream& operator << (ostream& os, const S_msrSingleTremolo& elt);


} // namespace MusicFormats


#endif
