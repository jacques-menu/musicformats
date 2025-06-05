/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrPartGroupElements___
#define ___msrPartGroupElements___

#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
  Parts and part groups can be found in part groups,
  hence class   msrPartGroupElement
*/

class EXP msrPartGroupElement : public msrElement
/*
  a purely virtual common ancestor to the msrPartGroup and msrPart classes,
  which can be inside an msrPartGroup
*/
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<msrPartGroupElement> create (
                            const mfInputLineNumber& inputLineNumber);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPartGroupElement (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~msrPartGroupElement ();

//   public:
//
//     // print
//     // ------------------------------------------------------
//
//     std::string           asString () const override;
//
//     void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    /*
      The part group uplink is declared in the sub-classes,
      i.e. msrPart and msrPartGroup,
      to allow for separate *.h files, C++ constraint
    */
};
typedef SMARTP<msrPartGroupElement> S_msrPartGroupElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrPartGroupElement& elt);


}


#endif // ___msrPartGroupElements___
