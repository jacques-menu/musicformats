/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, ...

#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string mfTimingItemKindAsString (
  mfTimingItemKind imingItemKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const mfTimingItemKind& elt)
{
  os << mfTimingItemKindAsString (elt);
  return os;
}

//______________________________________________________________________________
mfTimingItemsList mfTimingItemsList::gGlobalTimingItemsList;

S_mfTimingItem mfTimingItem::createTimingItem (
  const std::string& activity,
  const std::string& description,
  mfTimingItemKind   kind,
  clock_t            startClock,
  clock_t            endClock)
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
  const std::string& activity,
  const std::string& description,
  mfTimingItemKind   kind,
  clock_t            startClock,
  clock_t            endClock)
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
  const std::string& activity,
  const std::string& description,
  mfTimingItemKind   kind,
  clock_t            startClock,
  clock_t            endClock)
{
  S_mfTimingItem
    mfTimingItem =
      mfTimingItem::createTimingItem (
        activity,
        description,
        kind,
        startClock,
        endClock);

  fTimingItemsList.push_back (mfTimingItem);
}

void mfTimingItemsList::doPrint (std::ostream& os) const
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

  os << std::left <<
    std::setw (activityWidth) <<
    gGlobalOahEarlyOptions.getMfWaeHandler ()->activity () <<
    "  " <<
    std::setw (descriptionWidth) <<
    gGlobalOahEarlyOptions.getMfWaeHandler ()->description () <<
    "  " <<
    std::setw (kindWidth)     <<
    gGlobalOahEarlyOptions.getMfWaeHandler ()->kind () <<
    "  " <<
    std::setw (secondsWidth)  <<
    gGlobalOahEarlyOptions.getMfWaeHandler ()->CPUSeconds () <<
    std::endl <<

    std::setw (activityWidth) << mfReplicateString ("-", activityWidth) << "  " <<
    std::setw (descriptionWidth) << mfReplicateString ("-", descriptionWidth) << "  " <<
    std::setw (kindWidth) << mfReplicateString ("-", kindWidth) << "  " <<
    std::setw (secondsWidth) << mfReplicateString ("-", secondsWidth) <<
    std::endl << std::endl;

  for (S_mfTimingItem theTimingItem : fTimingItemsList) {
    clock_t timingItemClock =
      theTimingItem->getEndClock () - theTimingItem->getStartClock ();
    totalClock += timingItemClock;

    os << std::left <<
      std::setw (activityWidth) << theTimingItem->getActivity () << "  " <<
      std::setw (descriptionWidth) << theTimingItem->getDescription () << "  ";

    switch (theTimingItem->getKind ()) {
      case mfTimingItemKind::kMandatory:
        totalMandatoryClock += timingItemClock;
        os << std::setw (kindWidth) <<
        std::setw (descriptionWidth) <<
        gGlobalOahEarlyOptions.getMfWaeHandler ()->mandatory ();
        break;
      case mfTimingItemKind::kOptional:
        totalOptionalClock += timingItemClock;
        os << std::setw (kindWidth) <<
        std::setw (descriptionWidth) <<
        gGlobalOahEarlyOptions.getMfWaeHandler ()->optional ();
        break;
    } // switch

    os << "  " <<
      std::right << std::fixed <<
      std::setprecision (secondsPrecision) <<

      std::setw (secondsWidth) <<
      float(timingItemClock) / CLOCKS_PER_SEC <<
      std::endl;
  } // for
  os << std::endl;

  // printing the totals
  const int
    totalClockWidth          = 11,
    totalMandatoryClockWidth =  9,
    totalOptionalClockWidth  =  9,
    totalsPrecision          =  secondsPrecision;

  os << std::left <<
    std::setw (totalClockWidth) <<
    gGlobalOahEarlyOptions.getMfWaeHandler ()->totalSeconds () <<
    "  " <<
    std::setw (totalMandatoryClockWidth) <<
    gGlobalOahEarlyOptions.getMfWaeHandler ()->mandatory () <<
    "  " <<
    std::setw (totalOptionalClockWidth) <<
    gGlobalOahEarlyOptions.getMfWaeHandler ()->optional () <<
    std::endl <<

    std::setw (totalClockWidth) <<
    mfReplicateString ("-", totalClockWidth) <<
    "  " <<
    std::setw (totalMandatoryClockWidth) <<
    mfReplicateString ("-", totalMandatoryClockWidth) <<
    "  " <<
    std::setw (secondsWidth) <<
    mfReplicateString ("-", secondsWidth) <<
    std::endl <<

    std::fixed <<
    std::setprecision (totalsPrecision) <<

    std::setw (totalClockWidth) <<
    float(totalClock) / CLOCKS_PER_SEC <<
    "  " <<
    std::setw (totalMandatoryClockWidth) <<
    float(totalMandatoryClock) / CLOCKS_PER_SEC <<
    "  " <<
    std::setw (totalOptionalClockWidth) <<
    float(totalOptionalClock) / CLOCKS_PER_SEC <<
    std::endl;
}

void mfTimingItemsList::printWithContext (
  const std::string& context,
  std::ostream&      os) const
{
  os << std::left <<
    "Timing information for " <<
    context <<
    ":" <<
    std::endl << std::endl;

  doPrint (os);
}

void mfTimingItemsList::print (std::ostream& os) const
{
  os << std::left <<
    gGlobalOahEarlyOptions.getMfWaeHandler ()->timingInformation () <<
    ":" <<
    std::endl << std::endl;

  doPrint (os);
}

std::ostream& operator << (std::ostream& os, const mfTimingItemsList& tim) {
  tim.print(os);
  return os;
}


}

