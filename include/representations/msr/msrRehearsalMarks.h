/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrRehearsalMarks___
#define ___msrRehearsalMarks___

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrRehearsalMark : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum class msrRehearsalMarkKind {
      kNone,
      kRectangle, kOval, kCircle, kBracket, kTriangle, kDiamond
    };

    string rehearsalKindAsString (
      msrRehearsalMarkKind rehearsalKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRehearsalMark> create (
                            int                  inputLineNumber,
                            S_msrMeasure         upLinkToMeasure,
                            msrRehearsalMarkKind rehearsalKind,
                            const string&        rehearsalText,
                            msrPlacementKind     rehearsalPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRehearsalMark (
                            int                  inputLineNumber,
                            S_msrMeasure         upLinkToMeasure,
                            msrRehearsalMarkKind rehearsalKind,
                            const string&        rehearsalText,
                            msrPlacementKind     rehearsalPlacementKind);

    virtual               ~msrRehearsalMark ();

  public:

    // set and get
    // ------------------------------------------------------

    msrRehearsalMarkKind  getRehearsalMarkKind () const
                              { return fRehearsalMarkKind; }

    msrPlacementKind      getRehearsalMarkPlacementKind () const
                              { return fRehearsalMarkPlacementKind; }

    string                getRehearsalMarkText () const
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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrRehearsalMarkKind  fRehearsalMarkKind;

    string                fRehearsalMarkText;

    msrPlacementKind      fRehearsalMarkPlacementKind;
};
typedef SMARTP<msrRehearsalMark> S_msrRehearsalMark;
EXP ostream& operator << (ostream& os, const S_msrRehearsalMark& elt);


} // namespace MusicFormats


#endif
