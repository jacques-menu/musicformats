/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWaeHandlersEnglish___
#define ___mfWaeHandlersEnglish___

#include "mfWaeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP mfWaeHandlerEnglish : public mfWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfWaeHandlerEnglish> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfWaeHandlerEnglish ();

    virtual               ~mfWaeHandlerEnglish ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // pass numbers

    // use default versions in mfWaeHandlers.cpp

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<mfWaeHandlerEnglish> S_mfWaeHandlerEnglish;
EXP std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerEnglish& elt);


}


#endif
