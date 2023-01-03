/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWaeHandlersGerman___
#define ___mfWaeHandlersGerman___

#include "mfWaeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP mfWaeHandlerGerman : public mfWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfWaeHandlerGerman> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfWaeHandlerGerman ();

    virtual               ~mfWaeHandlerGerman ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // pass numbers

    // OAH

    // passes

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfWaeHandlerGerman> S_mfWaeHandlerGerman;
EXP std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerGerman& elt);


}


#endif
