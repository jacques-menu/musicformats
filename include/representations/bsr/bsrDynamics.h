/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrDynamic___
#define ___bsrDynamic___

#include "msrDynamics.h"

#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrDynamic : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrDynamic> create (
                            int             inputLineNumber,
                            msrDynamicKind dynamicKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrDynamic (
                            int             inputLineNumber,
                            msrDynamicKind dynamicKind);

    virtual               ~bsrDynamic ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDynamicKind       getDynamicKind () const
                              { return fDynamicKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fDynamicsCellsList; }


    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        noteValueKindAsCellsList () const;

    S_bsrCellsList        noteOctaveKindAsCellsList () const;

    S_bsrCellsList        buildCellsList () const;

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

    std::string           asDebugString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrDynamicKind        fDynamicKind;

    S_bsrCellsList        fDynamicsCellsList;
};
typedef SMARTP<bsrDynamic> S_bsrDynamic;
EXP std::ostream& operator << (std::ostream& os, const S_bsrDynamic& elt);


}


#endif // ___bsrDynamic___
