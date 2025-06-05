/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "msrPartGroupElements.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
/* this class is purely virtual
S_msrPartGroupElement msrPartGroupElement::create (
  const mfInputLineNumber& inputLineNumber)
{
  msrPartGroupElement* obj =
    new msrPartGroupElement (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

msrPartGroupElement::msrPartGroupElement (
  const mfInputLineNumber& inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrPartGroupElement::~msrPartGroupElement ()
{}

// std::string msrPartGroupElement::asString () const
// {
//   std::string result;
//
//   if (
//     // part group?
//     S_msrPartGroup
//       group =
//         dynamic_cast<msrPartGroup*>(&(*element))
//   ) {
//     result = group->asString ();
//   }
//
//   else if (
//     // part?
//     S_msrPart
//       subGroup =
//         dynamic_cast<msrPart*>(&(*element))
//   ) {
//     result = group->asString ();
//   }
//
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "part group element is neither a part group nor a part";
//
//     msrInternalError (ss.str ());
//   }
//
//   return result;
// }
//
// void msrPartGroupElement::print (std::ostream& os) const
// {
// //   os << // JMI
// //     "PartGroupElement" <<
// //     ", line " << fInputLineNumber <<
// //     std::endl;
//
//   if (
//     // part group?
//     S_msrPartGroup
//       group =
//         dynamic_cast<msrPartGroup*>(&(*element))
//   ) {
//     os << group;
//   }
//
//   else if (
//     // part?
//     S_msrPart
//       subGroup =
//         dynamic_cast<msrPart*>(&(*element))
//   ) {
//     os << group;
//   }
//
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "part group element is neither a part group nor a part";
//
//     msrInternalError (ss.str ());
//   }
// }

std::ostream& operator << (std::ostream& os, const S_msrPartGroupElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
