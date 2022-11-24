/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrElements___
#define ___msrElements___

#include <string>
#include <ostream>

#include "smartpointer.h"
#include "basevisitor.h"

#include "msrTypesForwardDeclarations.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrElement : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

                          msrElement (
                            int inputLineNumber);

    virtual               ~msrElement ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v) = 0;

    virtual void          browseDataAlongPathToVoice (
                            basevisitor*            v,
                            const S_msrPathToVoice& pathToVoice);

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const;

    virtual std::string   asShortString () const;
    virtual std::string   asShortStringForMeasuresSlices () const;

    virtual void          print (std::ostream& os) const;

    virtual void          printShort (std::ostream& os) const;

    virtual void          printSummary (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    int                   fInputLineNumber;
};
typedef SMARTP<msrElement> S_msrElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrElement& elt);


}


#endif
