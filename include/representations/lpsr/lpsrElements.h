/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrElements___
#define ___lpsrElements___

#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrElement : public msrElement
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrElement> create (
                            const mfInputLineNumber& inputLineNumber);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrElement (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~lpsrElement ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------
};

using S_lpsrElement = SMARTP<lpsrElement>;


}


#endif // ___lpsrElements___
