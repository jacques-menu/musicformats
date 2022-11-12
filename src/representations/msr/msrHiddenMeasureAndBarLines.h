/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrHiddenMeasureAndBarLine___
#define ___msrHiddenMeasureAndBarLine___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrHiddenMeasureAndBarLine : public msrMeasureElementLambda
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHiddenMeasureAndBarLine> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHiddenMeasureAndBarLine (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    virtual               ~msrHiddenMeasureAndBarLine ();

  public:

    // set and get
    // ------------------------------------------------------

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
};
typedef SMARTP<msrHiddenMeasureAndBarLine> S_msrHiddenMeasureAndBarLine;
EXP std::ostream& operator << (std::ostream& os, const S_msrHiddenMeasureAndBarLine& elt);


}


#endif
