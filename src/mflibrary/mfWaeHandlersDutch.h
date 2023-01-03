/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWaeHandlersDutch___
#define ___mfWaeHandlersDutch___

#include "mfWaeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP mfWaeHandlerDutch : public mfWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfWaeHandlerDutch> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfWaeHandlerDutch ();

    virtual               ~mfWaeHandlerDutch ();

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
typedef SMARTP<mfWaeHandlerDutch> S_mfWaeHandlerDutch;
EXP std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerDutch& elt);


}


#endif
