/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTies___
#define ___msrTies___

#include "msrElements.h"

#include "msrPlacements.h"


namespace MusicFormats
{

// ties
// ------------------------------------------------------

enum class msrTieKind {
  kTieNone,
  kTieStart, kTieContinue, kTieStop,
  kTieLetRing // MusicXML 4.0
};

std::string msrTieKindAsString (
  msrTieKind tieKind);

std::ostream& operator << (std::ostream& os, const msrTieKind& elt);

//______________________________________________________________________________
class EXP msrTie : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTie> create (
                            int              inputLineNumber,
                            msrTieKind       tieKind,
                            msrPlacementKind placementKind);

    static SMARTP<msrTie> create (
                            int        inputLineNumber,
                            msrTieKind tieKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTie (
                            int              inputLineNumber,
                            msrTieKind       tieKind,
                            msrPlacementKind placementKind);

    virtual               ~msrTie ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTieKind (msrTieKind tieKind)
                              { fTieKind = tieKind; }

    msrTieKind            getTieKind () const
                              { return fTieKind; }

    void                  setTiePlacementKind (msrPlacementKind placementKind)
                              { fTiePlacementKind = placementKind; }

    msrPlacementKind      getTiePlacementKind () const
                              { return fTiePlacementKind; }

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

    // private fields
    // ------------------------------------------------------

    msrTieKind            fTieKind;

    msrPlacementKind      fTiePlacementKind;
};
typedef SMARTP<msrTie> S_msrTie;
EXP std::ostream& operator << (std::ostream& os, const S_msrTie& elt);


}


#endif // ___msrTies___
