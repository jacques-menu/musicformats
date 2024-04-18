/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___languagesItalian___
#define ___languagesItalian___

#include "languages.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP languageItalian : public language
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<languageItalian> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          languageItalian ();

    virtual               ~languageItalian ();

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
typedef SMARTP<languageItalian> S_languageItalian;
EXP std::ostream& operator << (std::ostream& os, const S_languageItalian& elt);


}


#endif // ___languagesItalian___
