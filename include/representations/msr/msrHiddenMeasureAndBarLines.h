/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrHiddenMeasureAndBarLine___
#define ___msrHiddenMeasureAndBarLine___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrHiddenMeasureAndBarLine : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHiddenMeasureAndBarLine> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrHiddenMeasureAndBarLine> create (
                            int                 inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHiddenMeasureAndBarLine (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    virtual               ~msrHiddenMeasureAndBarLine ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setHiddenMeasureAndBarLineUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getHiddenMeasureAndBarLineUpLinkToMeasure (); }

    void                  setHiddenMeasureAndBarLineUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getHiddenMeasureAndBarLineUpLinkToMeasure () const
                            { return fHiddenMeasureAndBarLineUpLinkToMeasure; }

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

    S_msrMeasure          fHiddenMeasureAndBarLineUpLinkToMeasure;
};
typedef SMARTP<msrHiddenMeasureAndBarLine> S_msrHiddenMeasureAndBarLine;
EXP std::ostream& operator << (std::ostream& os, const S_msrHiddenMeasureAndBarLine& elt);


}


#endif
