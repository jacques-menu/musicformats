/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, ...

#include "mfStringsHandling.h"
#include "mfTiming.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string mfTimingItemKindAsString (
  mfTimingItemKind imingItemKind)
{
  string result;

  switch (imingItemKind) {
    case mfTimingItemKind::kMandatory:
      result = "kMandatory";
      break;
    case mfTimingItemKind::kOptional:
      result = "kOptional";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const mfTimingItemKind& elt)
{
  os << mfTimingItemKindAsString (elt);
  return os;
}

//______________________________________________________________________________
mfTimingItemsList mfTimingItemsList::gGlobalTimingItemsList;

S_timingItem mfTimingItem::createTimingItem (
  const string&  activity,
  const string&  description,
  mfTimingItemKind kind,
  clock_t        startClock,
  clock_t        endClock)
{
  mfTimingItem* o = new mfTimingItem (
    activity,
    description,
    kind,
    startClock,
    endClock);
  assert (o != nullptr);
  return o;
}

mfTimingItem::mfTimingItem (
  const string&  activity,
  const string&  description,
  mfTimingItemKind kind,
  clock_t        startClock,
  clock_t        endClock)
{
  fActivity    = activity;
  fDescription = description;
  fKind        = kind;
  fStartClock  = startClock;
  fEndClock    = endClock;
}

mfTimingItemsList::mfTimingItemsList ()
{}

mfTimingItemsList::~mfTimingItemsList ()
{}

void mfTimingItemsList::appendTimingItem (
  const string&     activity,
  const string&     description,
  mfTimingItemKind kind,
  clock_t           startClock,
  clock_t            endClock)
{
  S_timingItem
    mfTimingItem =
      mfTimingItem::createTimingItem (
        activity,
        description,
        kind,
        startClock,
        endClock);

  fTimingItemsList.push_back (mfTimingItem);
}

void mfTimingItemsList::doPrint (ostream& os) const
{
  // printing the details
  const int
    activityWidth     =  8,
    descriptionWidth  = 54,
    kindWidth         =  9,
    secondsWidth      =  9,
    secondsPrecision  = secondsWidth - 4; // to leave room for large numbers

  clock_t
    totalClock          = 0,
    totalMandatoryClock = 0,
    totalOptionalClock  = 0;

  os << left <<
    setw (activityWidth) << "Activity" << "  " <<
    setw (descriptionWidth) << "Description" << "  " <<
    setw (kindWidth)     << "Kind" << "  " <<
    setw (secondsWidth)  << "CPU (sec)" << endl <<
    setw (activityWidth) << mfReplicateString ("-", activityWidth) << "  " <<
    setw (descriptionWidth) << mfReplicateString ("-", descriptionWidth) << "  " <<
    setw (kindWidth) << mfReplicateString ("-", kindWidth) << "  " <<
    setw (secondsWidth) << mfReplicateString ("-", secondsWidth) <<
    endl << endl;

  for (
    list<S_timingItem>::const_iterator i=fTimingItemsList.begin ();
    i!=fTimingItemsList.end ();
    ++i
  ) {
    S_timingItem theTimingItem = (*i);

    clock_t timingItemClock =
      theTimingItem->getEndClock () - theTimingItem->getStartClock ();
    totalClock += timingItemClock;

    os << left <<
      setw (activityWidth) << theTimingItem->getActivity () << "  " <<
      setw (descriptionWidth) << theTimingItem->getDescription () << "  ";

    switch (theTimingItem->getKind ()) {
      case mfTimingItemKind::kMandatory:
        totalMandatoryClock += timingItemClock;
        os << setw (kindWidth) << "mandatory";
        break;
      case mfTimingItemKind::kOptional:
        totalOptionalClock += timingItemClock;
        os << setw (kindWidth) << "optional";
        break;
    } // switch

    os << "  " <<
      right << fixed <<
      setprecision (secondsPrecision) <<

      setw (secondsWidth) <<
      float(timingItemClock) / CLOCKS_PER_SEC <<
      endl;
  } // for
  os << endl;

  // printing the totals
  const int
    totalClockWidth          = 11,
    totalMandatoryClockWidth =  9,
    totalOptionalClockWidth  =  9,
    totalsPrecision          =  secondsPrecision;

  os << left <<
    setw (totalClockWidth)            << "Total (sec)" <<
    "  " <<
    setw (totalMandatoryClockWidth)   << "Mandatory" <<
    "  " <<
    setw (totalOptionalClockWidth)    << "Optional" <<
    endl <<

    setw (totalClockWidth) <<
    mfReplicateString ("-", totalClockWidth) <<
    "  " <<
    setw (totalMandatoryClockWidth) <<
    mfReplicateString ("-", totalMandatoryClockWidth) <<
    "  " <<
    setw (secondsWidth) <<
    mfReplicateString ("-", secondsWidth) <<
    endl <<

    fixed <<
    setprecision (totalsPrecision) <<

    setw (totalClockWidth) <<
    float(totalClock) / CLOCKS_PER_SEC <<
    "  " <<
    setw (totalMandatoryClockWidth) <<
    float(totalMandatoryClock) / CLOCKS_PER_SEC <<
    "  " <<
    setw (totalOptionalClockWidth) <<
    float(totalOptionalClock) / CLOCKS_PER_SEC <<
    endl;
}

void mfTimingItemsList::printWithContext (
  const string& context,
  ostream&      os) const
{
  os << left <<
    "Timing information for " << context << ":" <<
    endl << endl;

  doPrint (os);
}

void mfTimingItemsList::print (ostream& os) const
{
  os << left <<
    "Timing information:" <<
    endl << endl;

  doPrint (os);
}

ostream& operator << (ostream& os, const mfTimingItemsList& tim) {
  tim.print(os);
  return os;
}


}

