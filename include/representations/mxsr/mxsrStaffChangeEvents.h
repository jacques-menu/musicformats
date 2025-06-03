/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrStaffChangeEvents___
#define ___mxsrStaffChangeEvents___

// #include <iomanip>
// #include <string>

#include "exports.h"
#include "smartpointer.h"

// #include "mfBasicTypes.h"
// #include "mfBool.h"
// #include "mfIndentedTextOutput.h"
// #include "mfWrappedValueForArithmetic.h"

#include "mxsrNoteEvents.h"

// #include "msrTupletFactors.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
enum class mxsrStaffChangeEventKind {
  kStaffChangeEvent_NONE,
  kStaffChangeEventTakeOff, kStaffChangeEventLanding
};

std::string mxsrStaffChangeEventKindAsString (
  mxsrStaffChangeEventKind noteStaffChangeEventKind);

std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrStaffChangeEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrStaffChangeEvent>
                          create (
                            mxsrStaffChangeEventKind         staffChangeEventKind,
                            const mxsrNoteSequentialNumber & noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mfStaffNumber&             takeOffStaffNumber,
                            const mfInputLineNumber&         takeOffInputLineNumber, // superflous ???
                            const mfStaffNumber&             landingStaffNumber,
                            const mfInputLineNumber&         landingInputLineNumber, // superflous ???
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrStaffChangeEvent (
                            mxsrStaffChangeEventKind         staffChangeEventKind,
                            const mxsrNoteSequentialNumber & noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mfStaffNumber&             takeOffStaffNumber,
                            const mfInputLineNumber&         takeOffInputLineNumber, // superflous ???
                            const mfStaffNumber&             landingStaffNumber,
                            const mfInputLineNumber&         landingInputLineNumber, // superflous ???
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrStaffChangeEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrStaffChangeEventKind
                          getStaffChangeEventKind () const
                              { return fStaffChangeEventKind; }

    // the staff change voice number is the one of the note
    // upon which the staff change occurds
    const mfVoiceNumber&         getNoteVoiceNumber () const
                              { return fNoteVoiceNumber; }

    const mfStaffNumber&  getTakeOffStaffNumber () const
                              { return fTakeOffStaffNumber; }

    const mfInputLineNumber&
                          getTakeOffInputLineNumber () const
                              { return fTakeOffInputLineNumber; }

    const mfStaffNumber&  getLandingStaffNumber () const
                              { return fLandingStaffNumber; }

    const mfInputLineNumber&
                          getLandingInputLineNumber () const
                              { return fLandingInputLineNumber; }

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

  private:

    // private fields
    // ------------------------------------------------------

    mxsrStaffChangeEventKind
                          fStaffChangeEventKind;

    // the staff change voice number is the one of the note
    // upon which the staff change occurds
//     int                   fStaffChangeVoicefNumber;

    // a staff change event is attached to the take off note,
    // so as to know early that there is staff change

    // the staff numbers differ for take off and landing

    // the take off staff number is that of the note event,
    // and we need to store the landing staff number
    mfStaffNumber         fTakeOffStaffNumber;
    mfInputLineNumber     fTakeOffInputLineNumber;

    mfStaffNumber         fLandingStaffNumber;
    mfInputLineNumber     fLandingInputLineNumber;
};

using S_mxsrStaffChangeEvent = SMARTP<mxsrStaffChangeEvent>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrStaffChangeEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEvent& elt);

// bool compareStaffChangeEventsByIncreasingInputLineNumber (
//   S_mxsrStaffChangeEvent& first,
//   S_mxsrStaffChangeEvent& second);


}


#endif // ___mxsrStaffChangeEvents___
