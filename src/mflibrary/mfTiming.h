/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfTiming___
#define ___mfTiming___

#include <iostream>
#include <list>

#include "exports.h"

#include "smartpointer.h"


using namespace std;
using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfTimingItem : public smartable
{
  public:

    // data types
    // ------------------------------------------------------

    enum timingItemKind {
      kMandatory, kOptional
    };

    // constructors/destructor
    // ------------------------------------------------------

    static SMARTP<mfTimingItem> createTimingItem (
      const string&  activity,
      const string&  description,
      timingItemKind kind,
      clock_t        startClock,
      clock_t        endClock);

    mfTimingItem (
      const string&  activity,
      const string&  description,
      timingItemKind kind,
      clock_t        startClock,
      clock_t        endClock);

  public:

    // set and get
    // ------------------------------------------------------

    timingItemKind        getKind () const
                              { return fKind; }

    string                getActivity () const
                              { return fActivity; }
    string                getDescription () const
                              { return fDescription; }

    clock_t               getStartClock () const
                              { return fStartClock; }
    clock_t               getEndClock () const
                              { return fEndClock; }

  private:

    // private fields
    // ------------------------------------------------------

    timingItemKind        fKind;

    string                fActivity;
    string                fDescription;

    clock_t               fStartClock;
    clock_t               fEndClock;
};
typedef SMARTP<mfTimingItem> S_timingItem;

//______________________________________________________________________________
class EXP mfTimingItemsList {
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfTimingItemsList ();

    virtual               ~mfTimingItemsList ();

  public:

    // global variable for general use
    // ------------------------------------------------------

    static mfTimingItemsList         gGlobalTimingItemsList;

  public:

    // public services
    // ------------------------------------------------------

    // add an item
    void                  appendTimingItem (
                            const string&              activity,
                            const string&              description,
                            mfTimingItem::timingItemKind kind,
                            clock_t                    startClock,
                            clock_t                    endClock);

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printWithContext (
                            const string& context,
                            ostream&      os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  doPrint (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    list<S_timingItem>    fTimingItemsList;
};
EXP ostream& operator<< (ostream& os, const mfTimingItemsList& tim);



} // namespace MusicFormats


#endif
