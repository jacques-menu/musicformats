/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBarChecks___
#define ___msrBarChecks___

#include <list>

#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrBarCheck : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarCheck> create (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrBarCheck> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrBarCheck> createWithNextBarPuristNumber (
                            const mfInputLineNumber& inputLineNumber,
                            const mfMeasureNumber&   nextBarOriginalNumber,
                            int                      nextBarPuristNumber,
                            const S_msrMeasure&      upLinkToMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBarCheck (const mfInputLineNumber& inputLineNumber);

                          msrBarCheck (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

                          msrBarCheck (
                            const mfInputLineNumber& inputLineNumber,
                            const mfMeasureNumber&   nextBarOriginalNumber,
                            int                      nextBarPuristNumber,
                            const S_msrMeasure&      upLinkToMeasure);

    virtual               ~msrBarCheck ();

  public:

    // set and get
    // ------------------------------------------------------

    mfMeasureNumber       getNextBarOriginalNumber () const
                              { return fNextBarOriginalNumber; }

    void                  setNextBarPuristNumber (int puristNumber);

    int                   getNextBarPuristNumber () const
                              { return fNextBarPuristNumber; }

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

    mfMeasureNumber           fNextBarOriginalNumber;
    int                   fNextBarPuristNumber;
};
typedef SMARTP<msrBarCheck> S_msrBarCheck;
EXP std::ostream& operator << (std::ostream& os, const S_msrBarCheck& elt);


}


#endif // ___msrBarChecks___
