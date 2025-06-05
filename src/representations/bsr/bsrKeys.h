/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrKeys___
#define ___bsrKeys___

#include "bsrLineContentsElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class bsrKeyKind {
  kKeyKindNone,
  kKeyKindFlats, kKeyKindNaturals, kKeyKindSharps
};

std::string bsrKeyKindAsString (
  bsrKeyKind keyKind);

std::ostream& operator << (std::ostream& os, const bsrKeyKind& elt);

//______________________________________________________________________________
class EXP bsrKey : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrKey> create (
                            const mfInputLineNumber& inputLineNumber,
                            bsrKeyKind   keyKind,
                            size_t numberOfAlterations);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrKey (
                            const mfInputLineNumber& inputLineNumber,
                            bsrKeyKind   keyKind,
                            size_t numberOfAlterations);

    virtual               ~bsrKey ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setKeyKind (bsrKeyKind keyKind)
                              { fKeyKind = keyKind; }

    bsrKeyKind            getKeyKind () const
                              { return fKeyKind; }

    void                  setNumberOfAlterations (size_t numberOfAlterations)
                              { fNumberOfAlterations = numberOfAlterations; }

    size_t                getNumberOfAlterations () const
                              { return fNumberOfAlterations; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fKeyCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        keyKindAsCellsList () const;

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

    bsrKeyKind            fKeyKind;
    size_t                fNumberOfAlterations;

    S_bsrCellsList        fKeyCellsList;
};
typedef SMARTP<bsrKey> S_bsrKey;
EXP std::ostream& operator << (std::ostream& os, const S_bsrKey& elt);


}


#endif // ___bsrKeys___
