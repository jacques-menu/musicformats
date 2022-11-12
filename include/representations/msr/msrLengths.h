/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrLengths___
#define ___msrLengths___

#include <string>
#include <ostream>

#include <map>

#include "smartpointer.h"

#include "mfBool.h"


using namespace MusicXML2;

namespace MusicFormats
{

// length units
//______________________________________________________________________________

enum class msrLengthUnitKind {
  kUnitInch, kUnitCentimeter, kUnitMillimeter
};

std::string msrLengthUnitKindAsString (
  msrLengthUnitKind lengthUnitKind);

std::ostream& operator << (std::ostream& os, const msrLengthUnitKind& elt);

std::string existingMsrLengthUnitKinds (size_t namesListMaxLength);

extern std::map<std::string, msrLengthUnitKind>
  gGlobalMsrLengthUnitKindsMap;

void initializeMsrLengthUnitKindsMap ();

// lengths
//______________________________________________________________________________

class EXP msrLength : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrLength> create (
                            msrLengthUnitKind lengthUnitKind,
                            float             lengthValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrLength (
                            msrLengthUnitKind lengthUnitKind,
                            float             lengthValue);

                          msrLength ();

    virtual               ~msrLength ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLengthUnitKind (msrLengthUnitKind value)
                              { fLengthUnitKind = value; }

    msrLengthUnitKind     getLengthUnitKind () const
                              { return fLengthUnitKind; }

    void                  setLengthValue (float value)
                              { fLengthValue = value; }

    float                 getLengthValue () const
                              { return fLengthValue; };

  public:

    // public services
    // ------------------------------------------------------

    Bool                  operator== (const msrLength& other) const
                              {
                                // JMI convert to same length unit kind before comparing BLARK
                                return
                                  fLengthUnitKind == other.fLengthUnitKind
                                   &&
                                  fLengthValue == other.fLengthValue;
                              }

    Bool                  operator!= (const msrLength& other) const
                              { return ! ((*this) == other); }

    void                  convertToLengthUnit (
                            msrLengthUnitKind lengthUnitKind);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrLengthUnitKind     fLengthUnitKind;
    float                 fLengthValue;
};
typedef SMARTP<msrLength> S_msrLength;
EXP std::ostream& operator << (std::ostream& os, const msrLength& elt);
EXP std::ostream& operator << (std::ostream& os, const S_msrLength& elt);


}

#endif
