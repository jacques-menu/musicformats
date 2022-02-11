/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrPartGroupElements___
#define ___msrPartGroupElements___

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
  Parts and part groups can be found in part groups,
  hence class msrPartGroupElement
*/

class EXP msrPartGroupElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

                          msrPartGroupElement (
                            int inputLineNumber);

    virtual               ~msrPartGroupElement ();

  /*
    The part group uplink is declared in the sub-classes,
    to allow for separate *.h files, C++ constraint
  */
};
typedef SMARTP<msrPartGroupElement> S_msrPartGroupElement;
EXP ostream& operator<< (ostream& os, const S_msrPartGroupElement& elt);


} // namespace MusicFormats


#endif
