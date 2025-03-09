/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBookElements___
#define ___msrBookElements___

#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
  Parts and part groups can be found in part groups,
  hence class msrBookElement
*/

class EXP msrBookElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

                          msrBookElement (
                            int inputLineNumber);

    virtual               ~msrBookElement ();

  /*
    The part group uplink is declared in the sub-classes,
    to allow for separate *.h files, C++ constraint
  */
};
typedef SMARTP<msrBookElement> S_msrBookElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrBookElement& elt);


}


#endif // ___msrBookElements___
