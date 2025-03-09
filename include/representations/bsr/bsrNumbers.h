/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrNumbers___
#define ___bsrNumbers___

#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class bsrNumberSignIsNeededKind {
  kNumberSignIsNeededYes, kNumberSignIsNeededNo
};

std::string bsrNumberSignIsNeededKindAsString (
  bsrNumberSignIsNeededKind numberSignIsNeededKind);

std::ostream& operator << (std::ostream& os, const bsrNumberSignIsNeededKind& elt);

class EXP bsrNumber : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrNumber> create (
                            int       inputLineNumber,
                            int       numberValue,
                            bsrNumberSignIsNeededKind
                                      numberSignIsNeededKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrNumber (
                            int       inputLineNumber,
                            int       numberValue,
                            bsrNumberSignIsNeededKind
                                      numberSignIsNeededKind);

    virtual               ~bsrNumber ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNumberValue () const
                              { return fNumberValue; }

    void                  setNumberSignIsNeededKind (
                            bsrNumberSignIsNeededKind
                              numberSignIsNeededKindvalue)
                              {
                                fNumberSignIsNeededKind =
                                  numberSignIsNeededKindvalue;
                              }

    bsrNumberSignIsNeededKind
                          getNumberSignIsNeededKind () const
                              { return fNumberSignIsNeededKind; }

    S_bsrCellsList        getNumberCellsList () const
                              { return fNumberCellsList; }

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fNumberCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        numberValueAsCellsList () const;

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

    int                   fNumberValue;

    bsrNumberSignIsNeededKind
                          fNumberSignIsNeededKind;

    S_bsrCellsList        fNumberCellsList;
};
typedef SMARTP<bsrNumber> S_bsrNumber;
EXP std::ostream& operator << (std::ostream& os, const S_bsrNumber& elt);


}


#endif // ___bsrNumbers___
