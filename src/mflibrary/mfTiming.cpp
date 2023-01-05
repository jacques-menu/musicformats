/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, ...
#include <format>

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
  int
    activityWidth    = 1,
    descriptionWidth = 1,
    kindWidth        = 1,
    secondsWidth     = 1,
    secondsPrecision = 1;

  switch (gGlobalOahEarlyOptions.getEarlyLanguageKind ()) {
    case mfLanguageKind::kMusicFormatsLanguage_UNKNOWN:
      break;
    case mfLanguageKind::kMusicFormatsLanguageEnglish:
      activityWidth     =  8;
      descriptionWidth  = 54;
      kindWidth         =  9;
      secondsWidth      =  9;
      break;
    case mfLanguageKind::kMusicFormatsLanguageFrench:
      activityWidth     =  8;
      descriptionWidth  = 59;
      kindWidth         = 11;
      secondsWidth      =  9;
      break;
    case mfLanguageKind::kMusicFormatsLanguageItalian:
      activityWidth     =  8;
      descriptionWidth  = 54;
      kindWidth         =  9;
      secondsWidth      =  9;
      break;
    case mfLanguageKind::kMusicFormatsLanguageGerman:
      activityWidth     =  8;
      descriptionWidth  = 54;
      kindWidth         =  9;
      secondsWidth      =  9;
      break;
    case mfLanguageKind::kMusicFormatsLanguageSpanish:
      activityWidth     =  8;
      descriptionWidth  = 54;
      kindWidth         =  9;
      secondsWidth      =  9;
      break;
    case mfLanguageKind::kMusicFormatsLanguageDutch:
      activityWidth     =  8;
      descriptionWidth  = 54;
      kindWidth         =  9;
      secondsWidth      =  9;
      break;
  } // switch
  secondsPrecision = secondsWidth - 4; // to leave room for large numbers

//   gLogStream << "descriptionWidth: " << descriptionWidth << std::endl;
//   gLogStream << "secondsWidth: " << secondsWidth << std::endl;
//   gLogStream << "secondsPrecision: " << secondsPrecision << std::endl;

//   os << std::right <<
  os << std::left <<
    std::setw (activityWidth) <<
    gWaeHandler->activity () <<
    "  " <<
    std::setw (descriptionWidth) <<
    gWaeHandler->description () <<
    "  " <<
    std::setw (kindWidth)     <<
    gWaeHandler->kind () <<
    "  " <<
    std::setw (secondsWidth)  <<
    gWaeHandler->CPUSeconds () <<
    std::endl <<

    std::setw (activityWidth) <<
    mfReplicateString ("-", activityWidth) <<
    "  " <<
    std::setw (descriptionWidth) <<
    mfReplicateString ("-", descriptionWidth) <<
    "  " <<
    std::setw (kindWidth) <<
    mfReplicateString ("-", kindWidth) <<
    "  " <<
    std::setw (secondsWidth) <<
    mfReplicateString ("-", secondsWidth) <<
    std::endl << std::endl;

  clock_t
    totalClock          = 0,
    totalMandatoryClock = 0,
    totalOptionalClock  = 0;

  for (S_mfTimingItem theTimingItem : fTimingItemsList) {
    clock_t timingItemClock =
      theTimingItem->getEndClock () - theTimingItem->getStartClock ();
    totalClock += timingItemClock;

    os << std::left <<
      std::setw (activityWidth) <<
      theTimingItem->getActivity () <<
      "  ";

    std::string
      description =
        theTimingItem->getDescription ();

    os << std::left <<
      std::setw (descriptionWidth) <<
      description;

    // if description contains non-diacritic characters (UTF-8),
    // that fouls std::setw() which accounts two characters for them

    int
      twoBytesWideCharactersInString =
        countTwoBytesWideCharactersInString (description);

//     gLogStream << "twoBytesWideCharactersInString: " << twoBytesWideCharactersInString << std::endl;


    if (twoBytesWideCharactersInString > 0) {
      os <<
        mfReplicateString (" ", twoBytesWideCharactersInString);
    }

    os <<
      "  ";

    os <<
      std::left <<
      std::setw (kindWidth);
    switch (theTimingItem->getKind ()) {
      case mfTimingItemKind::kMandatory:
        totalMandatoryClock += timingItemClock;
        os <<
//           std::left <<
//           std::setw (kindWidth) <<
          gWaeHandler->mandatory ();
        break;
      case mfTimingItemKind::kOptional:
        totalOptionalClock += timingItemClock;
        os <<
//           std::left <<
//           std::setw (kindWidth) <<
          gWaeHandler->optional ();
        break;
    } // switch

    os << "  " <<
      std::left <<
      std::fixed <<
      std::setprecision (secondsPrecision) <<
      std::setw (secondsWidth) <<
      float(timingItemClock) / CLOCKS_PER_SEC <<
      std::endl;
  } // for
  os << std::endl;

  // printing the totals
  int
    totalClockWidth          = 1,
    totalMandatoryClockWidth = 1,
    totalOptionalClockWidth  = 1,
    totalsPrecision          = secondsPrecision;

  switch (gGlobalOahEarlyOptions.getEarlyLanguageKind ()) {
    case mfLanguageKind::kMusicFormatsLanguage_UNKNOWN:
      break;
    case mfLanguageKind::kMusicFormatsLanguageEnglish:
      totalClockWidth          = 11;
      totalMandatoryClockWidth = 9;
      totalOptionalClockWidth  = 8;
      break;
    case mfLanguageKind::kMusicFormatsLanguageFrench:
      totalClockWidth          = 11;
      totalMandatoryClockWidth = 11;
      totalOptionalClockWidth  = 8;
      break;
    case mfLanguageKind::kMusicFormatsLanguageItalian:
      totalClockWidth          = 11;
      totalMandatoryClockWidth = 11;
      totalOptionalClockWidth  = 8;
      break;
    case mfLanguageKind::kMusicFormatsLanguageGerman:
      totalClockWidth          = 11;
      totalMandatoryClockWidth = 11;
      totalOptionalClockWidth  = 8;
      break;
    case mfLanguageKind::kMusicFormatsLanguageSpanish:
      totalClockWidth          = 11;
      totalMandatoryClockWidth = 11;
      totalOptionalClockWidth  = 8;
      break;
    case mfLanguageKind::kMusicFormatsLanguageDutch:
      totalClockWidth          = 11;
      totalMandatoryClockWidth = 11;
      totalOptionalClockWidth  = 11;
      break;
  } // switch

  os << std::left <<
    std::setw (totalClockWidth) <<
    gWaeHandler->totalSeconds () <<
    "  " <<
    std::setw (totalMandatoryClockWidth) <<
    gWaeHandler->mandatory () <<
    "  " <<
    std::setw (totalOptionalClockWidth) <<
    gWaeHandler->optional () <<
    std::endl <<

    std::setw (totalClockWidth) <<
    mfReplicateString ("-", totalClockWidth) <<
    "  " <<
    std::setw (totalMandatoryClockWidth) <<
    mfReplicateString ("-", totalMandatoryClockWidth) <<
    "  " <<
    std::setw (totalOptionalClockWidth) <<
    mfReplicateString ("-", totalOptionalClockWidth) <<
    std::endl <<

    std::fixed <<
    std::setprecision (totalsPrecision) <<

    std::setw (totalClockWidth) <<
    float (totalClock) / CLOCKS_PER_SEC <<
    "  " <<
    std::setw (totalMandatoryClockWidth) <<
    float(totalMandatoryClock) / CLOCKS_PER_SEC <<
    "  " <<
    std::setw (totalOptionalClockWidth) <<
    float (totalOptionalClock) / CLOCKS_PER_SEC <<
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
    gWaeHandler->timingInformation () <<
    ":" <<
    std::endl << std::endl;

  doPrint (os);
}

std::ostream& operator << (std::ostream& os, const mfTimingItemsList& tim) {
  tim.print(os);
  return os;
}


}

