/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWaeHandlersItalian___
#define ___mfWaeHandlersItalian___

#include "mfWaeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP mfWaeHandlerItalian : public mfWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfWaeHandlerItalian> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfWaeHandlerItalian ();

    virtual               ~mfWaeHandlerItalian ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // use default (english) versions in mfWaeHandlers.h/.cpp
    // until they are overridden here

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfWaeHandlerItalian> S_mfWaeHandlerItalian;
EXP std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerItalian& elt);


}


#endif