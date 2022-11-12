/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSegnos___
#define ___msrSegnos___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrSegno : public msrMeasureElementLambda
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegno> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure,
                            int          staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSegno (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure,
                            int          staffNumber);

    virtual               ~msrSegno ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getStaffNumber () const
                              { return fStaffNumber; }

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

    int                   fStaffNumber;
};
typedef SMARTP<msrSegno> S_msrSegno;
EXP std::ostream& operator << (std::ostream& os, const S_msrSegno& elt);


}


#endif
