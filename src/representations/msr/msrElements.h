/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrElements___
#define ___msrElements___

#include <string>
#include <ostream>

#include "smartpointer.h"
#include "basevisitor.h"

#include "mfBasicTypes.h"

#include "msrTypesForwardDeclarations.h"


using namespace MusicXML2;

namespace MusicFormats
{
//______________________________________________________________________________
// pre-declaration

class msrElement;
using S_msrElement = SMARTP<msrElement>;

//______________________________________________________________________________
class EXP msrElement : public smartable
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------
*/

  protected:

                          msrElement (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~msrElement ();

  public:

    // set and get
    // ------------------------------------------------------

    mfInputLineNumber     getInputLineNumber () const
                              { return fInputLineNumber; }

  public:

    // public services
    // ------------------------------------------------------

    operator              std::string() const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) = 0;
    virtual void          acceptOut (basevisitor* v) = 0;

    virtual void          browseData (basevisitor* v)
                              {
                                // there is nothing to browse in simple elements
                              };

    virtual void          browseDataAlongPathToVoice (
                            basevisitor*            v,
                            const S_msrPathToVoice& pathToVoice);

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const = 0;

    virtual std::string   asShortString () const
                              { return asString (); }

    virtual std::string   asStringForMeasuresSlices () const;

    virtual void          print (std::ostream& os) const
                              {
                                os << asString () << std::endl;
                                // asString() is enough for simple elements
                              }

    virtual void          printFull (std::ostream& os) const
                              { print (os); }

    virtual void          printSummary (std::ostream& os) const
                              { print (os); }

                          operator std::string ()
                              { return asString (); }

  protected:

    // protected fields
    // ------------------------------------------------------

    mfInputLineNumber     fInputLineNumber;
};

using S_msrElement = SMARTP<msrElement>;

EXP std::ostream& operator << (std::ostream& os, const S_msrElement& elt);

EXP std::ostream& operator << (std::ostream& os, const msrElement& elt);

std::string msrElementAsStringOrNULL(
  S_msrElement element);


}


#endif // ___msrElements___
