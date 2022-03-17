/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream> // TEMP JMI

#include "mfBool.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
Bool::Bool ()
{
  fValue = false;
}

Bool::Bool (bool value)
{
  fValue = value;
}

Bool::~Bool ()
{}

Bool::Bool(const Bool& theBool) {
//   cout << "Bool copy constructor " << endl; // JMI CLAR

   // copy the value
   fValue = theBool.getValue ();
}

/*
  // Declare prefix and postfix decrement operators.
  Point& operator--();       // Prefix decrement operator.
  Point operator--(int);     // Postfix decrement operator.
*/

Bool::Bool (const string& theString)
{
  if (theString == "false") {
    fValue = false;
  }
  else if (theString == "true") {
    fValue = true;
  }
  else {
  }
}

Bool& Bool::operator= (const Bool& theBool)
{
  if (this != &theBool) { // JMI costly?
    fValue = theBool.getValue ();
  }

  return *this;
}

Bool Bool::operator! () const
{
  Bool result (! fValue);

  return result;
}

Bool Bool::operator&&(const Bool& otherBool) const
{
  Bool result (fValue && otherBool.fValue);

  return result;
}

Bool Bool::operator||(const Bool& otherBool) const
{
  Bool result (fValue || otherBool.fValue);

  return result;
}

EXP Bool operator&& (const Bool& leftBool, const bool& theBool)
{
  Bool result (leftBool.getValue () && theBool);

  return result;
}

EXP Bool operator&& (const bool& theBool, const Bool& rightBool)
{
  Bool result (theBool && rightBool.getValue ());

  return result;
}

EXP Bool operator|| (const Bool& leftBool, const bool& theBool)
{

  Bool result (leftBool.getValue () || theBool);

  return result;
}

EXP Bool operator|| (const bool& theBool, const Bool& rightBool)
{
  Bool result (theBool || rightBool.getValue ());

  return result;
}

string Bool::asString () const
{
  return
    fValue
      ? "true"
      : "false";
}

void Bool::print (ostream& os) const
{
  os << asString ();
}

ostream& operator<< (ostream& os, const Bool& theBool) {
  theBool.print(os);
  return os;
}

Bool nand (Bool p, Bool q)
{
  return ! (p && q);
}

Bool nor (Bool p, Bool q)
{
  return ! (p || q);
}

Bool xorr (Bool p, Bool q)
{
  // see https://en.wikipedia.org/wiki/XOR_gate
  Bool
    inter =
      nand (
        p,
        q);

  return
    nand (
      nand (
        p,
        inter),
      nand (
        q,
        inter)
      );
}

Bool implies (Bool p, Bool q)
{
  return q || ! p;
}

void testBool ()
{
  Bool a;
  Bool b (true);

  Bool c = a && b;
  Bool d = a || b;

  Bool e (true);

  cout <<
    "a: " << a << endl <<
    "b: " << b << endl <<
    "c: " << c << endl <<
    "d: " << d << endl <<
    "e: " << e << endl <<
    endl;

  if (b) {
    cout << nand (d, e);
  }
  else {
    cout << nor (d, e);
  }
  cout << endl;

  cout <<
    implies (a, a) <<
    endl;
}


}

