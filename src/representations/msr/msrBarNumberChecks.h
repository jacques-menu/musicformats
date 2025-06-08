/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            const mfMeasureNumber&   nextBarOriginalNumber,
                            int                      nextBarPuristNumber);

    static SMARTP<msrBarNumberCheck> create (
                            const mfInputLineNumber& inputLineNumber,
                            const mfMeasureNumber&   nextBarOriginalNumber,
                            int                      nextBarPuristNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBarNumberCheck (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            const mfMeasureNumber&   nextBarOriginalNumber,
                            int                      nextBarPuristNumber);

    virtual               ~msrBarNumberCheck ();

  public:

    // set and get
    // ------------------------------------------------------

    mfMeasureNumber       getNextBarOriginalNumber () const
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

    mfMeasureNumber       fNextBarOriginalNumber;
    int                   fNextBarPuristNumber;
};
typedef SMARTP<msrBarNumberCheck> S_msrBarNumberCheck;
EXP std::ostream& operator << (std::ostream& os, const S_msrBarNumberCheck& elt);


}


#endif // ___msrBarNumberChecks___
