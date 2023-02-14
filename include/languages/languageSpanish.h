/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___languagesSpanish___
#define ___languagesSpanish___

#include "languages.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP languageSpanish : public language
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<languageSpanish> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          languageSpanish ();

    virtual               ~languageSpanish ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // use default (english) versions in // languages.h/.cpp
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
typedef SMARTP<languageSpanish> S_languageSpanish;
EXP std::ostream& operator << (std::ostream& os, const S_languageSpanish& elt);


}


#endif // ___languagesSpanish___
