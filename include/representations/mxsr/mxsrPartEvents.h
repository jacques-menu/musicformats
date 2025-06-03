/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef __mxsrPartEvents___
#define __mxsrPartEvents___

// #include <iomanip>
// #include <string>

#include "exports.h"
#include "smartpointer.h"

// #include "mfBasicTypes.h"
// #include "mfBool.h"
// #include "mfIndentedTextOutput.h"
// #include "mfWrappedValueForArithmetic.h"

// #include "mxsrBasicTypes.h"
#include "mxsrEvents.h"

// #include "msrTupletFactors.h"


using namespace MusicXML2;

namespace MusicFormats
{

//------------------------------------------------------------------------
class EXP mxsrPartEvent : public mxsrEvent
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrPartEvent>
                          create (
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrPartEvent (
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrPartEvent ();

  public:

    // set and get
    // ------------------------------------------------------

     const std::string&   getPartName () const
                              { return fPartName; }

     const mfMeasureNumber&
                          getMeasureNumber () const
                              { return fMeasureNumber; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

//     std::string           asString () const override;
//     std::string           asShortString () const override;
//
//     void                  print (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fPartName;
    mfMeasureNumber       fMeasureNumber;
};

using S_mxsrPartEvent = SMARTP<mxsrPartEvent>;

// EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrPartEvent& elt);


}


#endif // __mxsrPartEvents___
