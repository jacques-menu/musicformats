/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___languagesDutch___
#define ___languagesDutch___

#include "languages.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP languageDutch : public language
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<languageDutch> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          languageDutch ();

    virtual               ~languageDutch ();

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
typedef SMARTP<languageDutch> S_languageDutch;
EXP std::ostream& operator << (std::ostream& os, const S_languageDutch& elt);


}


#endif // ___languagesDutch___
