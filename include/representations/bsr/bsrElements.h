/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

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
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrElement> create (
                            const mfInputLineNumber& inputLineNumber);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrElement (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~bsrElement ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------
};

using S_bsrElement = SMARTP<bsrElement>;

EXP std::ostream& operator << (std::ostream& os, const S_bsrElement& elt);


}


#endif // ___bsrElements___
