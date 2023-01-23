/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrElementsBase___
#define ___msrElementsBase___


// #ifdef WIN32
// #pragma warning (disable : 4251 4275)
// #endif

namespace MusicXML2
{

class msrElementsBase
{
  public:
    virtual ~msrElementsBase () {}
};

template<class C> class msrElementsBaseVisitor : virtual public msrElementsBase
{
    public:
    virtual ~msrElementsBaseVisitor() {}
    virtual void visitStart( C& elt ) {};
    virtual void visitEnd  ( C& elt ) {};
};


}


#endif
