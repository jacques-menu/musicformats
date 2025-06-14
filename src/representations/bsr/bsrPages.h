/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrPages___
#define ___bsrPages___

#include "bsrElements.h"

#include "bsrFootNotes.h"
#include "bsrLines.h"
#include "bsrMusicHeadings.h"
#include "bsrPageHeadings.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrPage : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPage> create (
                            const mfInputLineNumber& inputLineNumber,
                            int printPageNumber,
                            int linesPerPage);

    SMARTP<bsrPage> createPageNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrPage (
                            const mfInputLineNumber& inputLineNumber,
                            int printPageNumber,
                            int linesPerPage);

    virtual               ~bsrPage ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintPageNumber () const
                              { return fPrintPageNumber; }

    int                   getBraillePageNumber () const
                              { return fBraillePageNumber; }

    int                   getLinesPerPage () const
                              { return fLinesPerPage; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendFootNotesToPage (
                            const S_bsrFootNotes& footNotes)
                              { fPageElementsList.push_back (footNotes); }

    void                  appendLineToPage (
                            const S_bsrLine& line)
                              { fPageElementsList.push_back (line); }

    void                  appendMusicHeadingToPage (
                            const S_bsrMusicHeading& musicHeading)
                              { fPageElementsList.push_back (musicHeading); }

    void                  appendPageHeadingToPage (
                            const S_bsrPageHeading& pageHeading)
                              { fPageElementsList.push_back (pageHeading); }

    int                   fetchLineContentsNumber () const;

  public:


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

    int                   fPrintPageNumber;
    int                   fBraillePageNumber;

    int                   fLinesPerPage;

    std::list <S_bsrPageElement>
                          fPageElementsList;
};
typedef SMARTP<bsrPage> S_bsrPage;
EXP std::ostream& operator << (std::ostream& os, const S_bsrPage& elt);


}


#endif // ___bsrPages___
