/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrSpaces___
#define ___bsrSpaces___

#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
const bsrCellKind
  kCellSpace = bsrCellKind::kDotsNone;

//______________________________________________________________________________
class EXP bsrSpaces : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrSpaces> create (
                            int inputLineNumber,
                            int numberOfSpaces);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrSpaces (
                            int inputLineNumber,
                            int numberOfSpaces);

    virtual               ~bsrSpaces ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNumberOfSpaces () const
                              { return fNumberOfSpaces; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fSpacesCellsList; }

    int                   fetchCellsNumber () const override;

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

    int                   fNumberOfSpaces;

    S_bsrCellsList        fSpacesCellsList;
};
typedef SMARTP<bsrSpaces> S_bsrSpaces;
EXP std::ostream& operator << (std::ostream& os, const S_bsrSpaces& elt);


}


#endif // ___bsrSpaces___
