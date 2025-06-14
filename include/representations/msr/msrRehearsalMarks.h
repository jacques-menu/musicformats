/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrRehearsalMarks___
#define ___msrRehearsalMarks___

#include "msrMeasureElements.h"

#include "msrPlacements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrRehearsalMarkKind {
  kRehearsalMarkNone,

  kRehearsalMarkRectangle,
  kRehearsalMarkSquare,
  kRehearsalMarkOval,
  kRehearsalMarkCircle,
  kRehearsalMarkBracket,
  kRehearsalMarkTriangle,
  kRehearsalMarkDiamond,
  kRehearsalMarkPentagon,
  kRehearsalMarkHexagon,
  kRehearsalMarkHeptagon,
  kRehearsalMarkOctagon,
  kRehearsalMarkNonagon,
  kRehearsalMarkDecagon
};

std::string msrRrehearsalKindAsString (
  msrRehearsalMarkKind rehearsalKind);

std::ostream& operator << (std::ostream& os, const msrRehearsalMarkKind& elt);

//______________________________________________________________________________
class EXP msrRehearsalMark : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRehearsalMark> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,
                            msrRehearsalMarkKind rehearsalKind,
                            const std::string&   rehearsalText,
                            msrPlacementKind     rehearsalPlacementKind);

    static SMARTP<msrRehearsalMark> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrRehearsalMarkKind rehearsalKind,
                            const std::string&   rehearsalText,
                            msrPlacementKind     rehearsalPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRehearsalMark (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,
                            msrRehearsalMarkKind rehearsalKind,
                            const std::string&   rehearsalText,
                            msrPlacementKind     rehearsalPlacementKind);

    virtual               ~msrRehearsalMark ();

  public:

    // set and get
    // ------------------------------------------------------

    msrRehearsalMarkKind  getRehearsalMarkKind () const
                              { return fRehearsalMarkKind; }

    msrPlacementKind      getRehearsalMarkPlacementKind () const
                              { return fRehearsalMarkPlacementKind; }

    std::string           getRehearsalMarkText () const
                              { return fRehearsalMarkText; }

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

    msrRehearsalMarkKind  fRehearsalMarkKind;

    std::string           fRehearsalMarkText;

    msrPlacementKind      fRehearsalMarkPlacementKind;
};
typedef SMARTP<msrRehearsalMark> S_msrRehearsalMark;
EXP std::ostream& operator << (std::ostream& os, const S_msrRehearsalMark& elt);


}


#endif // ___msrRehearsalMarks___
