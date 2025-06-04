/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrEvents___
#define ___mxsrEvents___

#include "exports.h"
#include "smartpointer.h"

#include "mfBasicTypes.h"

#include "mxsrBasicTypes.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
/*
  score events are collected in mxsr2msrSkeletonBuilder
  to simplify mxsr2msrSkeletonPopulator

  these events are known to occurr one or more notes too late for most of them,
  such as in chords, tuplets and staff changes

  avoid start and stop in naming, to avoid confusion with MusicXML concepts
  the airplane analogy is to avoid confusion with a *sequence* of notes
  displayed on another staff
*/

//------------------------------------------------------------------------
class EXP mxsrEvent : public smartable
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrEvent>
                          create (
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrEvent (
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    const mxsrEventSequentialNumber&
                          getEventSequentialNumber () const
                              { return fEventSequentialNumber; }

    const mfInputLineNumber&
                          getEventInputLineNumber () const
                              { return fEventInputLineNumber; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------


  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const = 0;
    virtual std::string   asShortString () const = 0;

    virtual void          print (std::ostream& os) const = 0;

  protected:

    // protected fields
    // ------------------------------------------------------

    // the input line numbers alone cannot be used as unique identifier,
    // since the end of lines are not mandatory in MusicXML data, hence:
    mxsrEventSequentialNumber
                          fEventSequentialNumber;

    mfInputLineNumber     fEventInputLineNumber;
};

using S_mxsrEvent = SMARTP<mxsrEvent>;

// EXP std::ostream& operator << (std::ostream& os, const S_mxsrEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrEvent& elt);

//________________________________________________________________________
// bool compareEventsByIncreasingSequentialNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second);
//
// bool compareEventsByIncreasingInputLineNumber (
//   const S_mxsrEvent& first,
//   const S_mxsrEvent& second);
//
// bool operator< (
//   const S_mxsrEvent& first,
//   const S_mxsrEvent& second);

/*
Coming on to what you want to achieve, I see two ways of doing so:

  std::map<int,int,compareByVal> hash;

  1:
    Copy all the elements of the map to a std::vector and sort that:

    std::vector<std::pair<int,int> > v(hash.begin(), hash.end());

    std::sort(v.begin(), v.end(), [](const auto& a, const auto& b)
      { return a.second < b.second; });

  2:
    Copy all the elements of the map to another map with keys as values and values as keys.
    If values of your map are not unique, you can use a std::multimap instead.
*/


}


#endif // ___mxsrEvents___
