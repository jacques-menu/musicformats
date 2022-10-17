/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrLigatures___
#define ___msrLigatures___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
// pre-declaration
class msrLigature;
typedef SMARTP<msrLigature> S_msrLigature;

//______________________________________________________________________________
class EXP msrLigature : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrLigatureKind {
      kLigatureNone,
      kLigatureStart, kLigatureContinue, kLigatureStop
    };

    static string ligatureKindAsString (
      msrLigatureKind ligatureKind);

    enum msrLigatureLineEndKind {
      kLigatureLineEndNone,
      kLigatureLineEndUp, kLigatureLineEndDown,
      kLigatureLineEndBoth, kLigatureLineEndArrow
    };

    static string ligatureLineEndKindAsString (
      msrLigatureLineEndKind ligatureLineEndKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLigature> create (
                            int                    inputLineNumber,
                            int                    ligatureNumber,
                            msrLigatureKind        ligatureKind,
                            msrLigatureLineEndKind ligatureLineEndKind,
                            msrLineTypeKind        ligatureLineTypeKind,
                            msrPlacementKind       ligaturePlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrLigature (
                            int                    inputLineNumber,
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
                            S_msrLigature sideLinkToOtherEnd);

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

    string                ligatureKindAsString () const;

    string                asString () const override;

    void                  print (ostream& os) const override;

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
EXP ostream& operator<< (ostream& os, const S_msrLigature& elt);


} // namespace MusicFormats


#endif
