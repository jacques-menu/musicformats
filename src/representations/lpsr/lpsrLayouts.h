/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrLayouts___
#define ___lpsrLayouts___

#include "lpsrElements.h"

#include "lpsrScheme.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrLayout : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrLayout> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrLayout (
                            int inputLineNumber);

    virtual               ~lpsrLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    float                 getLayoutGlobalStaffSize () const
                              { return fLayoutGlobalStaffSize; }

    void                  setLayoutGlobalStaffSize (float size)
                              { fLayoutGlobalStaffSize = size; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addSchemeVariable (
                            const S_lpsrSchemeVariable& schemeVariable)
                              {
                                fLpsrSchemeVariablesVector.push_back (schemeVariable);
                              }

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

    float                 fLayoutGlobalStaffSize;

    std::vector <S_lpsrSchemeVariable>
                          fLpsrSchemeVariablesVector;
};
typedef SMARTP<lpsrLayout> S_lpsrLayout;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrLayout& elt);


}


#endif // ___lpsrLayouts___
