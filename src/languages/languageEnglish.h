/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___languagesEnglish___
#define ___languagesEnglish___

#include "languages.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP languageEnglish : public language
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<languageEnglish> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          languageEnglish ();

    virtual               ~languageEnglish ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // use default (english) versions in // languages.h/.cpp

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<languageEnglish> S_languageEnglish;
EXP std::ostream& operator << (std::ostream& os, const S_languageEnglish& elt);


}


#endif // ___languagesEnglish___
