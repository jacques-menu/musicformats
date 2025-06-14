/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrLigatures___
#define ___msrLigatures___

#include "msrElements.h"

#include "msrLineTypes.h"
#include "msrPlacements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrLigatureKind {
  kLigatureNone,
  kLigatureStart, kLigatureContinue, kLigatureStop
};

std::string msrLigatureKindAsString (
  msrLigatureKind ligatureKind);

std::ostream& operator << (std::ostream& os, const msrLigatureKind& elt);

enum class msrLigatureLineEndKind {
  kLigatureLineEndNone,
  kLigatureLineEndUp, kLigatureLineEndDown,
  kLigatureLineEndBoth, kLigatureLineEndArrow
};

std::string msrLigatureLineEndKindAsString (
  msrLigatureLineEndKind ligatureLineEndKind);

std::ostream& operator << (std::ostream& os, const msrLigatureLineEndKind& elt);

//______________________________________________________________________________
// PRE-declaration

class   msrLigature;
typedef SMARTP<msrLigature> S_msrLigature;

class EXP msrLigature : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLigature> create (
                            const mfInputLineNumber& inputLineNumber,
                            int                    ligatureNumber,
                            msrLigatureKind        ligatureKind,
                            msrLigatureLineEndKind ligatureLineEndKind,
                            msrLineTypeKind        ligatureLineTypeKind,
                            msrPlacementKind       ligaturePlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrLigature (
                            const mfInputLineNumber& inputLineNumber,
                            int                    ligatureNumber,
                            msrLigatureKind        ligatureKind,
                            msrLigatureLineEndKind ligatureLineEndKind,
                            msrLineTypeKind        ligatureLineTypeKind,
                            msrPlacementKind       ligaturePlacementKind);

    virtual               ~msrLigature ();

  public:

    // set and get
    // ------------------------------------------------------

    // sideLinks
    void                  setLigatureSideLinkToOtherEnd (
                            const S_msrLigature& sideLinkToOtherEnd);

    S_msrLigature         getLigatureSideLinkToOtherEnd () const
                              { return fLigatureSideLinkToOtherEnd; }

    int                   getLigatureNumber () const
                              { return fLigatureNumber; }

    msrLigatureKind       getLigatureKind () const
                              { return fLigatureKind; }

    msrLigatureLineEndKind
                          getLigatureLineEndKind () const
                              { return fLigatureLineEndKind; }

    msrLineTypeKind       getLigatureLineTypeKind () const
                              { return fLigatureLineTypeKind; }

    msrPlacementKind      getLigaturePlacementKind () const
                              { return fLigaturePlacementKind; }

    void                  setLigaturePlacementKind (
                            msrPlacementKind placementKind)
                              { fLigaturePlacementKind = placementKind; }

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // sideLinks
    // ------------------------------------------------------
    S_msrLigature         fLigatureSideLinkToOtherEnd; // two-way

    // fields
    // ------------------------------------------------------

    int                   fLigatureNumber;

    msrLigatureKind       fLigatureKind;

    msrLigatureLineEndKind
                          fLigatureLineEndKind;

    msrLineTypeKind       fLigatureLineTypeKind;

    msrPlacementKind      fLigaturePlacementKind;
};
typedef SMARTP<msrLigature> S_msrLigature;
EXP std::ostream& operator << (std::ostream& os, const S_msrLigature& elt);


}


#endif // ___msrLigatures___
