/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrOrnaments___
#define ___msrOrnaments___

#include "msrElements.h"

#include "msrAccidentals.h"
#include "msrNotes.h"
#include "msrPlacements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrOrnamentKind {
  kOrnamentTrill, // JMI kOrnamentDashes, v0.9.66
  kOrnamentTurn, kOrnamentInvertedTurn, kOrnamentDelayedTurn,
  kOrnamentDelayedInvertedTurn, kOrnamentVerticalTurn,
  kOrnamentMordent, kOrnamentInvertedMordent,
  kOrnamentSchleifer,
  kOrnamentShake,
  kOrnamentAccidentalKind
};

std::string msrOrnamentKindAsString (
  msrOrnamentKind ornamentKind);

std::ostream& operator << (std::ostream& os, const msrOrnamentKind& elt);

class EXP msrOrnament : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOrnament> create (
                            int              inputLineNumber,
                            msrOrnamentKind  ornamentKind,
                            msrPlacementKind ornamentPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrOrnament (
                            int              inputLineNumber,
                            msrOrnamentKind  ornamentKind,
                            msrPlacementKind ornamentPlacementKind);

    virtual               ~msrOrnament ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOrnamentUpLinkToNote (const S_msrNote& note)
                              { fOrnamentUpLinkToNote = note; }

    S_msrNote             getOrnamentUpLinkToNote () const
                              { return fOrnamentUpLinkToNote; }

    msrOrnamentKind       getOrnamentKind () const
                              { return fOrnamentKind; }

    void                  setOrnamentPlacementKind (
                            msrPlacementKind
                              ornamentPlacementKind)
                              {
                                fOrnamentPlacementKind =
                                  ornamentPlacementKind;
                              }

    msrPlacementKind      getOrnamentPlacementKind () const
                              { return fOrnamentPlacementKind; }

    void                  setOrnamentAccidentalKind (
                            msrAccidentalKind
                              ornamentAccidentalKind)
                              {
                                fOrnamentAccidentalKind =
                                  ornamentAccidentalKind;
                              }

    msrAccidentalKind getOrnamentAccidentalKind () const
                              { return fOrnamentAccidentalKind; }

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

    S_msrNote             fOrnamentUpLinkToNote;

    msrOrnamentKind       fOrnamentKind;

    msrPlacementKind      fOrnamentPlacementKind;

    msrAccidentalKind     fOrnamentAccidentalKind;
};
typedef SMARTP<msrOrnament> S_msrOrnament;
EXP std::ostream& operator << (std::ostream& os, const S_msrOrnament& elt);


}


#endif // ___msrOrnaments___
