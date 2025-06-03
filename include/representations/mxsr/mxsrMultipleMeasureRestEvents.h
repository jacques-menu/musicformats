/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrMultipleMeasureRestEvents___
#define ___mxsrMultipleMeasureRestEvents___

// #include <iomanip>
// #include <string>

#include "exports.h"
#include "smartpointer.h"

// #include "mfBasicTypes.h"
// #include "mfBool.h"
// #include "mfIndentedTextOutput.h"
// #include "mfWrappedValueForArithmetic.h"

// #include "mxsrBasicTypes.h"
#include "mxsrPartEvents.h"

// #include "msrTupletFactors.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
enum class mxsrMultipleMeasureRestEventKind {
  kMultipleMeasureRestEvent_NONE,
  kMultipleMeasureRestEventBegin, kMultipleMeasureRestEventEnd
};

std::string mxsrMultipleMeasureRestEventKindAsString (
  mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind);

std::ostream& operator << (std::ostream& os, const mxsrMultipleMeasureRestEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrMultipleMeasureRestEvent : public mxsrPartEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrMultipleMeasureRestEvent>
                          create (
                            mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind,
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            int                              multipleMeasureRestNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

//     static SMARTP<mxsrMultipleMeasureRestEvent>
//                           createAMultipleMeasureRestEnd (
//                             const std::string&       partName,
//                             const mfMeasureNumber&   measureNumber,
//                             int                      multipleMeasureRestNumber,
//                             const mfInputLineNumber& eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrMultipleMeasureRestEvent (
                            mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind,
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            int                              multipleMeasureRestNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrMultipleMeasureRestEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrMultipleMeasureRestEventKind
                          getMultipleMeasureRestEventKind () const
                              { return fMultipleMeasureRestEventKind; }

    int                   getMultipleMeasureRestNumber () const
                              { return fMultipleMeasureRestNumber; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    mxsrMultipleMeasureRestEventKind
                          fMultipleMeasureRestEventKind;

    int                   fMultipleMeasureRestNumber;

};

using S_mxsrMultipleMeasureRestEvent = SMARTP<mxsrMultipleMeasureRestEvent>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrMultipleMeasureRestEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrMultipleMeasureRestEvent& elt);


}


#endif // ___mxsrMultipleMeasureRestEvents___
