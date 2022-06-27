/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "msrPartGroupElements.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

//______________________________________________________________________________
/* this class is purely virtual
S_msrPartGroupElement msrPartGroupElement::create (
  int inputLineNumber)
{
  msrPartGroupElement* o =
    new msrPartGroupElement (
      inputLineNumber);
  assert (o != nullptr);

  return o;
}
*/

msrPartGroupElement::msrPartGroupElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrPartGroupElement::~msrPartGroupElement ()
{}

// string msrPartGroupElement::asString () const
// {
//   string result;
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
//     stringstream s;
//
//     s <<
//       "part group element is neither a part group nor a part";
//
//     msrInternalError (s.str ());
//   }
//
//   return result;
// }
//
// void msrPartGroupElement::print (ostream& os) const
// {
// //   os << // JMI
// //     "PartGroupElement" <<
// //     ", line " << fInputLineNumber <<
// //     endl;
//
//   if (
//     // part group?
//     S_msrPartGroup
//       group =
//         dynamic_cast<msrPartGroup*>(&(*element))
//   ) {
//     group->print (os);
//   }
//
//   else if (
//     // part?
//     S_msrPart
//       subGroup =
//         dynamic_cast<msrPart*>(&(*element))
//   ) {
//     group->print (os);
//   }
//
//   else {
//     stringstream s;
//
//     s <<
//       "part group element is neither a part group nor a part";
//
//     msrInternalError (s.str ());
//   }
// }
//
// void msrPartGroupElement::printShort (ostream& os) const
// {
// //   os << // JMI
// //     "PartGroupElement" <<
// //     ", line " << fInputLineNumber <<
// //     endl;
//
//   if (
//     // part group?
//     S_msrPartGroup
//       group =
//         dynamic_cast<msrPartGroup*>(&(*element))
//   ) {
//     group->printShort (os);
//   }
//
//   else if (
//     // part?
//     S_msrPart
//       subGroup =
//         dynamic_cast<msrPart*>(&(*element))
//   ) {
//     group->printShort (os);
//   }
//
//   else {
//     stringstream s;
//
//     s <<
//       "part group element is neither a part group nor a part";
//
//     msrInternalError (s.str ());
//   }
// }

ostream& operator<< (ostream& os, const S_msrPartGroupElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}


}
