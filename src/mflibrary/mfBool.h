/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfBool___
#define ___mfBool___

#include <string>
#include <ostream>

#include "exports.h"

namespace MusicFormats
{

//______________________________________________________________________________
class EXP Bool
/*
  this class   encapsulates a 'bool' value to guarantee it is initialized
*/
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          Bool ();

                          Bool (bool value);

                          Bool (const std::string& theString);

    virtual               ~Bool ();

    // copy constructor
    // ------------------------------------------------------

                          Bool (const Bool& obj);

public:

    // set and get
    // ------------------------------------------------------

    // set the value
    void                  setValue (bool value)
                              { fValue = value; }

    // get the value
    bool                  getValue () const
                              { return fValue; }

  public:

    // public services
    // ------------------------------------------------------

    // explicit conversion
    explicit              operator bool () const { return fValue; }

    // copy assignment operator
    // ------------------------------------------------------

    Bool&                 operator= (const Bool& theBool);

/*
    // Declare prefix and postfix decrement operators.
    Point& operator--();       // Prefix decrement operator.
    Point operator--(int);     // Postfix decrement operator.
*/

    // negation, prefix operator
    Bool                  operator! () const;

    // dyadic operators
    Bool                  operator&& (const Bool& otherBool) const;
    Bool                  operator|| (const Bool& otherBool) const;

    // compare value JMI ???
    Bool                  operator == (const Bool &otherBool) const
                              { return fValue == otherBool.fValue; }
    Bool                  operator != (const Bool &otherBool) const
                              { return fValue != otherBool.fValue; }

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    bool                  fValue;
};

//EXP Bool operator&& (const Bool& leftBool, const Bool& rightBool);
EXP Bool operator&& (const Bool& leftBool, const bool& theBool);
EXP Bool operator&& (const bool& theBool, const Bool& rightBool);

//EXP Bool operator|| (const Bool& leftBool, const Bool& rightBool);
EXP Bool operator|| (const Bool& leftBool, const bool& theBool);
EXP Bool operator|| (const bool& theBool, const Bool& rightBool);

EXP std::ostream& operator << (std::ostream& os, const Bool& theBool);

EXP Bool nand (Bool p, Bool q);
EXP Bool nor (Bool p, Bool q);
EXP Bool xorr (Bool p, Bool q);

EXP Bool implies (Bool p, Bool q);

EXP void testBool ();


}


#endif
