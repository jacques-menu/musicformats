/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrPageElements___
#define ___bsrPageElements___

#include "bsrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrPageElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

/*
    static SMARTP<bsrPageElement> create (
                            int inputLineNumber);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrPageElement (
                            int inputLineNumber);

    virtual               ~bsrPageElement ();

  public:

    // set and get
    // ------------------------------------------------------

/* JMI
    void                  setUpLinkToBsrPage (
                            const S_bsrPage& upLinkToPage);

    S_bsrPage             getUpLinkToBsrPage () const;
*/

    void                  setSpacesBefore (int value)
                              { fSpacesBefore = value; }

    int                   getSpacesBefore () const
                              { return fSpacesBefore; }

/*
    int                   getSpacesAfter () const
                              { return fSpacesAfter; }
*/

  public:

    // public services
    // ------------------------------------------------------

    virtual int           fetchLineContentsNumber () const  // JMI ???
                              { return 0; }

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

    void                  print (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

/*
    S_bsrPage             fUpLinkToBsrPage;
*/

    int                   fSpacesBefore;
 // JMI   int                   fSpacesAfter;
};
typedef SMARTP<bsrPageElement> S_bsrPageElement;
EXP std::ostream& operator << (std::ostream& os, const S_bsrPageElement& elt);


}


#endif // ___bsrPageElements___
