/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrEyeGlasses___
#define ___msrEyeGlasses___

#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrEyeGlasses : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrEyeGlasses> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrEyeGlasses> create (
                            const mfInputLineNumber& inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrEyeGlasses (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    virtual               ~msrEyeGlasses ();

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
typedef SMARTP<msrEyeGlasses> S_msrEyeGlasses;
EXP std::ostream& operator << (std::ostream& os, const S_msrEyeGlasses& elt);


}


#endif // ___msrEyeGlasses___

