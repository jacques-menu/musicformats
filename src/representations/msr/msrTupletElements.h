/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTupletElements___
#define ___msrTupletElements___

#include <list>

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
  Notes, chords and tuplets can be found in tuplets,
  hence class msrTupletElement
*/

class EXP msrTupletElement : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

                          msrTupletElement (
                            int inputLineNumber);

    virtual               ~msrTupletElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setPositionInTuplet (
                            int positionInTuplet)
                              { fPositionInTuplet = positionInTuplet; }

    const size_t          getPositionInTuplet () const
                              { return fPositionInTuplet; }

  protected:

    // protected fields
    // ------------------------------------------------------

    /*
      The tuplet uplink is declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

    int                   fPositionInTuplet;
};
typedef SMARTP<msrTupletElement> S_msrTupletElement;
EXP ostream& operator<< (ostream& os, const S_msrTupletElement& elt);


} // namespace MusicFormats


#endif
