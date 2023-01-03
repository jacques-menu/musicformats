/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfTiming___
#define ___mfTiming___

#include <iostream>
#include <list>

#include "exports.h"

#include "smartpointer.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum mfTimingItemKind {
  kMandatory, kOptional
};

std::string mfTimingItemKindAsString (
  mfTimingItemKind imingItemKind);

std::ostream& operator << (std::ostream& os, const mfTimingItemKind& elt);

class EXP mfTimingItem : public smartable
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

    static SMARTP<mfTimingItem> createTimingItem (
                          const std::string& activity,
                          const std::string& description,
                          mfTimingItemKind   kind,
                          clock_t            startClock,
                          clock_t            endClock);

                        mfTimingItem (
                          const std::string& activity,
                          const std::string& description,
                          mfTimingItemKind   kind,
                          clock_t            startClock,
                          clock_t            endClock);

  public:

    // set and get
    // ------------------------------------------------------

    mfTimingItemKind      getKind () const
                              { return fKind; }

    std::string           getActivity () const
                              { return fActivity; }
    std::string           getDescription () const
                              { return fDescription; }

    clock_t               getStartClock () const
                              { return fStartClock; }
    clock_t               getEndClock () const
                              { return fEndClock; }

  private:

    // private fields
    // ------------------------------------------------------

    mfTimingItemKind      fKind;

    std::string           fActivity;
    std::string           fDescription;

    clock_t               fStartClock;
    clock_t               fEndClock;
};
typedef SMARTP<mfTimingItem> S_mfTimingItem;

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

    static mfTimingItemsList
                          gGlobalTimingItemsList;

  public:

    // public services
    // ------------------------------------------------------

    // add an item
    void                  appendTimingItem (
                            const std::string& activity,
                            const std::string& description,
                            mfTimingItemKind   kind,
                            clock_t            startClock,
                            clock_t            endClock);

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

    void                  printWithContext (
                            const std::string& context,
                            std::ostream&      os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  doPrint (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    std::list<S_mfTimingItem>
                          fTimingItemsList;
};
EXP std::ostream& operator << (std::ostream& os, const mfTimingItemsList& tim);



}


#endif
