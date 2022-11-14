/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasureElementsLambda___
#define ___msrMeasureElementsLambda___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMeasureElementLambda : public msrMeasureElement
{
  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureElementLambda (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    virtual               ~msrMeasureElementLambda ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureElementUpLinkToMeasure (
                            S_msrMeasure measure) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // public services
    // ------------------------------------------------------

    // upLink to measure
    S_msrMeasure          fetchMeasureElementUpLinkToMeasure () const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const override
                              {}

  protected:

    // protected fields
    // ------------------------------------------------------

    S_msrMeasure          fMeasureElementLambdaUpLinkToMeasure;
};
typedef SMARTP<msrMeasureElementLambda> S_msrMeasureElementLambda;
EXP std::ostream& operator << (std::ostream& os, const S_msrMeasureElementLambda& elt);


}


#endif
