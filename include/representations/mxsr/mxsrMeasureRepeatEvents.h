/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrMeasureRepeatEvents___
#define ___mxsrMeasureRepeatEvents___

#include "exports.h"
#include "smartpointer.h"

#include "mxsrPartEvents.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
enum class mxsrMeasureRepeatEventKind {
  kMeasureRepeatEvent_NONE,
  kMeasureRepeatEventBegin, kMeasureRepeatEventEnd
};

std::string mxsrMeasureRepeatEventKindAsString (
  mxsrMeasureRepeatEventKind measureRepeatEventKind);

std::ostream& operator << (std::ostream& os, const mxsrMeasureRepeatEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrMeasureRepeatEvent : public mxsrPartEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrMeasureRepeatEvent>
                          create (
                            mxsrMeasureRepeatEventKind       measureRepeatEventKind,
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            int                              measureRepeatNumber,
                            int                              measureRepeatSlashes,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrMeasureRepeatEvent (
                            mxsrMeasureRepeatEventKind       measureRepeatEventKind,
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            int                              measureRepeatNumber,
                            int                              measureRepeatSlashes,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrMeasureRepeatEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrMeasureRepeatEventKind
                          getMeasureRepeatEventKind () const
                              { return fMeasureRepeatEventKind; }

    int                   getMeasureRepeatSlashes () const
                              { return fMeasureRepeatSlashes; }

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

    mxsrMeasureRepeatEventKind
                          fMeasureRepeatEventKind;

    int                   fMeasureRepeatSlashes;
};

using S_mxsrMeasureRepeatEvent = SMARTP<mxsrMeasureRepeatEvent>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrMeasureRepeatEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrMeasureRepeatEvent& elt);


}


#endif // ___mxsrMeasureRepeatEvents___
