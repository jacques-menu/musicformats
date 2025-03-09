/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrLineContentsElements___
#define ___bsrLineContentsElements___

#include "bsrElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
// this class is purely virtual
class EXP bsrLineContentsElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

/*
    static SMARTP<bsrLineContentsElement> create (
                            int inputLineNumber);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrLineContentsElement (
                            int inputLineNumber);

    virtual               ~bsrLineContentsElement ();

  public:

    // set and get
    // ------------------------------------------------------
/*
    void                  setUpLinkToBsrLineContents (
                            const S_bsrLineContents& lineContents);
*/

    void                  setSpacesBefore (int value)
                              { fSpacesBefore = value; }

    int                   getSpacesBefore () const
                              { return fSpacesBefore; }

  public:

    // public services
    // ------------------------------------------------------


    virtual S_bsrCellsList
                          fetchCellsList () const = 0;

    virtual int           fetchCellsNumber () const = 0;

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

    std::string           asShortString () const override;

    virtual std::string   asDebugString () const = 0;

    void                  print (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    int                   fSpacesBefore;
};
typedef SMARTP<bsrLineContentsElement> S_bsrLineContentsElement;
EXP std::ostream& operator << (std::ostream& os, const S_bsrLineContentsElement& elt);


}


#endif // ___bsrLineContentsElements___
