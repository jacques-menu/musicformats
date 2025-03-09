/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrPageHeadings___
#define ___bsrPageHeadings___

#include "bsrLines.h"

#include "bsrCellsLists.h"
#include "bsrPaginations.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrPageHeading : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPageHeading> create (
                            int             inputLineNumber,
                            const std::string&   pageHeadingTitle,
                            const S_bsrPagination& pageHeadingPagination,
                            int             pageHeadingNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrPageHeading (
                            int             inputLineNumber,
                            const std::string&   pageHeadingTitle,
                            const S_bsrPagination& pageHeadingPagination,
                            int             pageHeadingNumber);

    virtual               ~bsrPageHeading ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getPageHeadingTitle () const
                              { return fPageHeadingTitle; }

    S_bsrPagination       getPageHeadingPagination () const
                              { return fPageHeadingPagination; }

    int                   getPageHeadingNumber () const
                              { return fPageHeadingNumber; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

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

    std::string           fPageHeadingTitle;

    S_bsrPagination       fPageHeadingPagination; // before title
    int                   fPageHeadingNumber;     // after title ??? JMI
};
typedef SMARTP<bsrPageHeading> S_bsrPageHeading;
EXP std::ostream& operator << (std::ostream& os, const S_bsrPageHeading& elt);


}


#endif // ___bsrPageHeadings___
