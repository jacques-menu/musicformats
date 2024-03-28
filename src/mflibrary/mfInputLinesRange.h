/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfInputLinesRange___
#define ___mfInputLinesRange___

#include <ostream>
#include <sstream>

#include "exports.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfInputLinesRange
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfInputLinesRange (
                              int inputStartLineNumber,
                              int inputEndLineNumber);

                          mfInputLinesRange (
                              int inputStartLineNumber);

    virtual               ~mfInputLinesRange ();

public:

    // set and get
    // ------------------------------------------------------

    int                   getInputStartLineNumber () const
                              { return fInputStartLineNumber; }

    void                  setInputStartLineNumber (int value)
                              { fInputStartLineNumber = value; }

    int                   getInputEndLineNumber () const
                              { return fInputEndLineNumber; }

    void                  setInputEndLineNumber (int value)
                              { fInputEndLineNumber = value; }

  public:

    // public services
    // ------------------------------------------------------


  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------

    int                  fInputStartLineNumber;
    int                  fInputEndLineNumber;
};

}

#endif // ___mfInputLinesRange___
