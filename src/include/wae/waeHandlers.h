/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___waeHandlers___
#define ___waeHandlers___

#include <list>

#include "smartpointer.h"

#include "msdlKeywords.h"


using namespace std;
using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
class EXP waeHandler : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<waeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          waeHandler ();

    virtual               ~waeHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<waeHandler> S_waeHandler;
EXP ostream& operator<< (ostream& os, const S_waeHandler& elt);


}


#endif
