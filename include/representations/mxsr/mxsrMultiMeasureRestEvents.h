/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrMultiMeasureRestEvents___
#define ___mxsrMultiMeasureRestEvents___

#include "exports.h"
#include "smartpointer.h"

#include "mxsrPartEvents.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
enum class mxsrMultiMeasureRestEventKind {
  kMultiMeasureRestEvent_NONE,
  kMultiMeasureRestEventBegin, kMultiMeasureRestEventEnd
};

std::string mxsrMultiMeasureRestEventKindAsString (
  mxsrMultiMeasureRestEventKind multiMeasureRestEventKind);

std::ostream& operator << (std::ostream& os, const mxsrMultiMeasureRestEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrMultiMeasureRestEvent : public mxsrPartEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrMultiMeasureRestEvent>
                          create (
                            mxsrMultiMeasureRestEventKind multiMeasureRestEventKind,
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            int                              multiMeasureRestNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

//     static SMARTP<mxsrMultiMeasureRestEvent>
//                           createAMultiMeasureRestEnd (
//                             const std::string&       partName,
//                             const mfMeasureNumber&   measureNumber,
//                             int                      multiMeasureRestNumber,
//                             const mfInputLineNumber& eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrMultiMeasureRestEvent (
                            mxsrMultiMeasureRestEventKind multiMeasureRestEventKind,
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            int                              multiMeasureRestNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrMultiMeasureRestEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrMultiMeasureRestEventKind
                          getMultiMeasureRestEventKind () const
                              { return fMultiMeasureRestEventKind; }

    int                   getMultiMeasureRestNumber () const
                              { return fMultiMeasureRestNumber; }

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

    mxsrMultiMeasureRestEventKind
                          fMultiMeasureRestEventKind;

    int                   fMultiMeasureRestNumber;

};

using S_mxsrMultiMeasureRestEvent = SMARTP<mxsrMultiMeasureRestEvent>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrMultiMeasureRestEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrMultiMeasureRestEvent& elt);


}


#endif // ___mxsrMultiMeasureRestEvents___
