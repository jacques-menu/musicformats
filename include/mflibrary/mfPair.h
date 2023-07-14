/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfPair___
#define ___mfPair___

#include <list>

#include "exports.h"
#include "smartpointer.h"


using namespace MusicXML2;

namespace MusicFormats
{

// //______________________________________________________________________________
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// class EXP mfPair : public smartable
// /*
//   C++ std::stack cannot be traversed, so we use std::list instead
//
//   https://www.programiz.com/cpp-programming/typename-templates
//
//   https://en.cppreference.com/w/cpp/language/type_alias
//
//   Do not put templated functions into the cpp file.
//   Since they are not implementations they are not found by the compiler/linker.
//   So leave them in the header file.
//
//   member reference base type 'int' is not a structure or union
//
//   https://stackoverflow.com/questions/580922/identifying-primitive-types-in-templates
//
//   https://en.cppreference.com/w/cpp/language/template_specialization
//
//   https://www.ibm.com/docs/en/zos/2.4.0?topic=only-explicit-specialization-c
//
//   https://dev.to/sandordargo/what-are-type-traits-in-c-18j5
// */
// {
//   public:
//
//     // creation
//     // ------------------------------------------------------
//
//     static SMARTP<mfPair> create ();
//
//     static SMARTP<mfPair> create (
//                             const ELEMENT_TYPE& element);
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           mfPair ();
//
//                           mfPair (const ELEMENT_TYPE& element);
//
//     virtual               ~mfPair ();
//
//   public:
//
//     // set and get
//     // ------------------------------------------------------
//
//     const std::list<ELEMENT_TYPE>&
//                           getElementsStdList () const
//                               { return fElementsStdList;}
//
//   public:
//
//     // public services
//     // ------------------------------------------------------
//
//     // basic list stuff
//     size_t                size () const
//                               { return fElementsStdList.size (); }
//
//     void                  push (
//                             const ELEMENT_TYPE& element)
//                               { fElementsStdList.push_front (element); }
//
//     ELEMENT_TYPE&         top ()
//                               { return fElementsStdList.front (); }
//
//     void                  pop ()
//                               { fElementsStdList.pop_front (); }
//
//     // specific stack stuff
//     Bool                  contains (
//                             const ELEMENT_TYPE& value) const;
//
//   public:
//
//     // print
//     // ------------------------------------------------------
//
//     void                  print (std::ostream& os) const;
//
//     void                  printWithContext (
//                             const std::string& context,
//                             char               evidencer,
//                             std::ostream&      os) const;
//
//   private:
//
//     // private services
//     // ------------------------------------------------------
//
//   private:
//
//     // private fields
//     // ------------------------------------------------------
//
//     std::string           fElementsPairName;
//
//     std::list<ELEMENT_TYPE>
//                           fElementsStdList;
// };
//
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// EXP std::ostream& operator << (std::ostream& os, const mfPair<FIRST_TYPE, SECOND_TYPE>& elt);


}


#endif // ___mfPair___
