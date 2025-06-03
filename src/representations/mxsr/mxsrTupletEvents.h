/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrTupletEvents___
#define ___mxsrTupletEvents___

// #include <iomanip>
// #include <string>

#include "exports.h"
#include "smartpointer.h"

// #include "mfBasicTypes.h"
// #include "mfBool.h"
// #include "mfIndentedTextOutput.h"
// #include "mfWrappedValueForArithmetic.h"

#include "mxsrNoteEvents.h"

#include "msrTupletFactors.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
enum class mxsrTupletEventKind {
  kTupletEvent_NONE,
  kTupletEventBegin, kTupletEventEnd
};

std::string mxsrTupletEventKindAsString (
  mxsrTupletEventKind noteTupletEventKind);

std::ostream& operator << (std::ostream& os, const mxsrTupletEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrTupletEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrTupletEvent>
                          create (
                            mxsrTupletEventKind              tupletEventKind,
                            const mxsrTupletNumber&          tupletNumber,
                            const msrTupletFactor&           tupletFactor,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrTupletEvent (
                            mxsrTupletEventKind              tupletEventKind,
                            const mxsrTupletNumber&          tupletNumber,
                            const msrTupletFactor&           tupletFactor,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrTupletEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrTupletEventKind   getTupletEventKind () const
                              { return fTupletEventKind; }

    const mxsrTupletNumber&
                          getTupletNumber () const
                              { return fTupletNumber; }

    const msrTupletFactor&
                          getTupletFactor () const
                              { return fTupletFactor; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  tupletEventIsForTupletNumber (int tupletNumber)
                              { return fTupletNumber == tupletNumber; }


  private:

    // private services
    // ------------------------------------------------------


  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    mxsrTupletEventKind   fTupletEventKind;

    mxsrTupletNumber      fTupletNumber;
    msrTupletFactor       fTupletFactor;
};

using S_mxsrTupletEvent = SMARTP<mxsrTupletEvent>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrTupletEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrTupletEvent& elt);


}


#endif // ___mxsrTupletEvents___
