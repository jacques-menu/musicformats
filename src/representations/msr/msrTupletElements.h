/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTupletElements___
#define ___msrTupletElements___

#include "msrMeasureElements.h"


namespace MusicFormats
{
//______________________________________________________________________________
/*
  Notes, chords and tuplets can be found in tuplets,
  hence super class msrTupletElement for them
*/

//______________________________________________________________________________
class EXP msrTupletElement : public msrMeasureElement
// class EXP msrTupletElement : public msrElement
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTupletElement (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~msrTupletElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setPositionInTuplet (
                            int positionInTuplet);

    const size_t          getPositionInTuplet () const;

  protected:

    // protected fields
    // ------------------------------------------------------

    /*
      The uplinks to the tuplet and measure element are declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

    int                   fPositionInTuplet;
};

using S_msrTupletElement = SMARTP<msrTupletElement>;

EXP std::ostream& operator << (std::ostream& os, const S_msrTupletElement& elt);
EXP std::ostream& operator << (std::ostream& os, const msrTupletElement& elt);


}


#endif // ___msrTupletElements___
