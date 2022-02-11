/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrElements___
#define ___bsrElements___

#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrElement : public msrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrElement> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrElement (
                            int inputLineNumber);

    virtual               ~bsrElement ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    string                asShortString () const override;

    void                  print (ostream& os) const override;
};
typedef SMARTP<bsrElement> S_bsrElement;
EXP ostream& operator<< (ostream& os, const S_bsrElement& elt);


}


#endif
