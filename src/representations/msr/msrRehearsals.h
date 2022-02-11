/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrRehearsals___
#define ___msrRehearsals___

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrRehearsal : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRehearsalKind {
      kNone,
      kRectangle, kOval, kCircle, kBracket, kTriangle, kDiamond
    };

    static string rehearsalKindAsString (
      msrRehearsalKind rehearsalKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRehearsal> create (
                            int              inputLineNumber,
                            msrRehearsalKind rehearsalKind,
                            const string&    rehearsalText,
                            msrPlacementKind rehearsalPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRehearsal (
                            int              inputLineNumber,
                            msrRehearsalKind rehearsalKind,
                            const string&    rehearsalText,
                            msrPlacementKind rehearsalPlacementKind);

    virtual               ~msrRehearsal ();

  public:

    // set and get
    // ------------------------------------------------------

    msrRehearsalKind      getRehearsalKind () const
                              { return fRehearsalKind; }

    msrPlacementKind      getRehearsalPlacementKind () const
                              { return fRehearsalPlacementKind; }

  public:

    // public services
    string                getRehearsalText () const
                              { return fRehearsalText; }

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

    msrRehearsalKind      fRehearsalKind;

    string                fRehearsalText;

    msrPlacementKind      fRehearsalPlacementKind;
};
typedef SMARTP<msrRehearsal> S_msrRehearsal;
EXP ostream& operator<< (ostream& os, const S_msrRehearsal& elt);


} // namespace MusicFormats


#endif
