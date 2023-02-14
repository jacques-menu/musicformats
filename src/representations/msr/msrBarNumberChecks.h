/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBarNumberChecks___
#define ___msrBarNumberChecks___

#include <list>

#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrBarNumberCheck : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarNumberCheck> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const std::string&  nextBarOriginalNumber,
                            int                 nextBarPuristNumber);

    static SMARTP<msrBarNumberCheck> create (
                            int                 inputLineNumber,
                            const std::string&  nextBarOriginalNumber,
                            int                 nextBarPuristNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBarNumberCheck (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const std::string&  nextBarOriginalNumber,
                            int                 nextBarPuristNumber);

    virtual               ~msrBarNumberCheck ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setBarNumberCheckUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getBarNumberCheckUpLinkToMeasure (); }

    void                  setBarNumberCheckUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getBarNumberCheckUpLinkToMeasure () const
                            { return fBarNumberCheckUpLinkToMeasure; }

    std::string           getNextBarOriginalNumber () const
                              { return fNextBarOriginalNumber; }

    int                   getNextBarPuristNumber () const
                              { return fNextBarPuristNumber; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           asString () const override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrMeasure          fBarNumberCheckUpLinkToMeasure;

    std::string           fNextBarOriginalNumber;
    int                   fNextBarPuristNumber;
};
typedef SMARTP<msrBarNumberCheck> S_msrBarNumberCheck;
EXP std::ostream& operator << (std::ostream& os, const S_msrBarNumberCheck& elt);


}


#endif // ___msrBarNumberChecks___
