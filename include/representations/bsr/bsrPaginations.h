/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrPaginations___
#define ___bsrPaginations___

#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrPagination : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPagination> create (
                            int inputLineNumber,
                            int printPageNumber,
                            int braillePageNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrPagination (
                            int inputLineNumber,
                            int printPageNumber,
                            int braillePageNumber);

    virtual               ~bsrPagination ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintPageNumber () const
                              { return fPrintPageNumber; }

    int                   getBraillePageNumber () const
                              { return fBraillePageNumber; }

    S_bsrCellsList        fetchCellsList () const override
                              { return fPaginationCellsList; }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

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

    int                   fPrintPageNumber;
    int                   fBraillePageNumber;

    S_bsrCellsList        fPaginationCellsList;
};
typedef SMARTP<bsrPagination> S_bsrPagination;
EXP std::ostream& operator << (std::ostream& os, const S_bsrPagination& elt);


}


#endif // ___bsrPaginations___
