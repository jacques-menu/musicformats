/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrVoiceStaffChange___
#define ___msrVoiceStaffChange___

#include "msrMeasureElements.h"

#include "msrMeasures.h"

#include "msrStaves.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrVoiceStaffChange : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoiceStaffChange> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const S_msrStaff&   staffToChangeTo);

    static SMARTP<msrVoiceStaffChange> create (
                            int                 inputLineNumber,
                            const S_msrStaff&   staffToChangeTo);

    SMARTP<msrVoiceStaffChange> createStaffChangeNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrVoiceStaffChange (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const S_msrStaff&   staffToChangeTo);

    virtual               ~msrVoiceStaffChange ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setVoiceStaffChangeUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getVoiceStaffChangeUpLinkToMeasure (); }

    void                  setVoiceStaffChangeUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getVoiceStaffChangeUpLinkToMeasure () const
                            { return fVoiceStaffChangeUpLinkToMeasure; }

    S_msrStaff            getStaffToChangeTo () const
                              { return fStaffToChangeTo; }

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

    S_msrMeasure          fVoiceStaffChangeUpLinkToMeasure;

    S_msrStaff            fStaffToChangeTo;
};
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;
EXP std::ostream& operator << (std::ostream& os, const S_msrVoiceStaffChange& elt);


}


#endif
