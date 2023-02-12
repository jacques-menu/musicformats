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
  mfTimingItemKind timingItemKind)
{
  std::string result;

  switch (timingItemKind) {
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
S_mfTimingItem mfTimingItem::createTimingItem (
  const std::string& activity,
  const std::string& description,
  mfTimingItemKind   timingItemKind,
  clock_t            startClock,
  clock_t            endClock)
{
  mfTimingItem* o = new mfTimingItem (
    activity,
    description,
    timingItemKind,
    startClock,
    endClock);
  assert (o != nullptr);
  return o;
}

mfTimingItem::mfTimingItem (
  const std::string& activity,
  const std::string& description,
  mfTimingItemKind   timingItemKind,
  clock_t            startClock,
  clock_t            endClock)
{
  fActivity       = activity;
  fDescription    = description;
  fTimingItemKind = timingItemKind;
  fStartClock     = startClock;
  fEndClock       = endClock;
}

mfTimingItemsList::mfTimingItemsList ()
{}

mfTimingItemsList::~mfTimingItemsList ()
{}

void mfTimingItemsList::appendTimingItem (
  mfPassIDKind       passIDKind,
  const std::string& description,
  mfTimingItemKind   timingItemKind,
  clock_t            startClock,
  clock_t            endClock)
{
  S_mfTimingItem
    mfTimingItem =
      mfTimingItem::createTimingItem (
        gLanguage->passIDKindAsString (passIDKind),
        description,
        timingItemKind,
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

  // set the timing items columns widths
  switch (gEarlyOptions.getEarlyLanguageKind ()) {
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

  // compute the timing items columns widths
  for (S_mfTimingItem theTimingItem : fTimingItemsList) {
    // take care of multi-line descriptions
    std::string
      activity =
        theTimingItem->getActivity ();

    // if activity contains non-diacritic characters (UTF-8),
    // that fouls std::setw() which accounts two characters for them
    int
      activityTwoBytesWideCharactersInString =
        countTwoBytesWideCharactersInString (activity);

    size_t activitySize =
      activity.size () - activityTwoBytesWideCharactersInString;

    gLog << "--> activitySize: " << activitySize << std::endl;
    gLog << "--> activityTwoBytesWideCharactersInString: " << activityTwoBytesWideCharactersInString << std::endl << std::endl;

    if (activitySize > activityWidth) {
      activityWidth = activitySize;
    }

    std::string
      description =
        theTimingItem->getDescription ();

    // if description contains non-diacritic characters (UTF-8),
    // that fouls std::setw() which accounts two characters for them
    int
      descriptionTwoBytesWideCharactersInString =
        countTwoBytesWideCharactersInString (description);

    size_t descriptionSize =
      description.size () - descriptionTwoBytesWideCharactersInString;

    gLog << "--> descriptionSize: " << descriptionSize << std::endl;
    gLog << "--> descriptionTwoBytesWideCharactersInString: " << descriptionTwoBytesWideCharactersInString << std::endl << std::endl;

    if (descriptionSize > descriptionWidth) {
      descriptionWidth = descriptionSize;
    }

    std::string
      kind =
        mfTimingItemKindAsString (
          theTimingItem->getTimingItemKind ());

    // if kind contains non-diacritic characters (UTF-8),
    // that fouls std::setw() which accounts two characters for them
    int
      kindTwoBytesWideCharactersInString =
        countTwoBytesWideCharactersInString (kind);

    size_t kindSize =
      kind.size () - kindTwoBytesWideCharactersInString;

    gLog << "--> kindSize: " << kindSize << std::endl;
    gLog << "--> kindTwoBytesWideCharactersInString: " << kindTwoBytesWideCharactersInString << std::endl << std::endl;

    if (kindSize > kindWidth) {
      kindWidth = kindSize;
    }
  } // for

  secondsPrecision = secondsWidth - 4; // to leave room for large numbers

  gLog << "activityWidth: " << activityWidth << std::endl;
  gLog << "descriptionWidth: " << descriptionWidth << std::endl;
  gLog << "kindWidth: " << kindWidth << std::endl;
  gLog << "secondsWidth: " << secondsWidth << std::endl;
  gLog << "secondsPrecision: " << secondsPrecision << std::endl;

  //  print the timing items columns headers and separators
  os << std::left <<
    std::setw (activityWidth) <<
    gLanguage->activity () <<
    ' ' <<
    std::setw (descriptionWidth) <<
    gLanguage->description () <<
    ' ' <<
    std::setw (kindWidth)     <<
    gLanguage->kind () <<
    ' ' <<
    std::setw (secondsWidth)  <<
    gLanguage->CPUSeconds () <<
    std::endl <<

    std::setw (activityWidth) <<
    mfReplicateString ("-", activityWidth) <<
    ' ' <<
    std::setw (descriptionWidth) <<
    mfReplicateString ("-", descriptionWidth) <<
    ' ' <<
    std::setw (kindWidth) <<
    mfReplicateString ("-", kindWidth) <<
    ' ' <<
    std::setw (secondsWidth) <<
    mfReplicateString ("-", secondsWidth) <<
    std::endl << std::endl;

  // print the timing items
  clock_t
    totalClock          = 0,
    totalMandatoryClock = 0,
    totalOptionalClock  = 0;

  for (S_mfTimingItem theTimingItem : fTimingItemsList) {
    clock_t timingItemClock =
      theTimingItem->getEndClock () - theTimingItem->getStartClock ();
    totalClock += timingItemClock;

    // do not print optional passes as activity
    switch (theTimingItem->getTimingItemKind ()) {
      case mfTimingItemKind::kMandatory:
        os << std::left <<
          std::setw (activityWidth) <<
          theTimingItem->getActivity () <<
          ' ';
        break;
      case mfTimingItemKind::kOptional:
        os << std::left <<
          ' ';
        break;
    } // switch

    // take care of multi-line descriptions
    std::string
      description =
        theTimingItem->getDescription ();

    gIndenter.printMultiLineStringInATable (
      description,
      activityWidth + 1,
      descriptionWidth,
      os);

//     os << std::left <<
//       std::setw (descriptionWidth) <<
//       description;

    // if description contains non-diacritic characters (UTF-8),
    // that fouls std::setw() which accounts two characters for them
    int
      twoBytesWideCharactersInString =
        countTwoBytesWideCharactersInString (description);

//     gLog << "twoBytesWideCharactersInString: " << twoBytesWideCharactersInString << std::endl;

    if (twoBytesWideCharactersInString > 0) {
      os <<
        mfReplicateChar (' ', twoBytesWideCharactersInString);
    }
    os <<
      ' ';

    os <<
      std::left <<
      std::setw (kindWidth);
    switch (theTimingItem->getTimingItemKind ()) {
      case mfTimingItemKind::kMandatory:
        totalMandatoryClock += timingItemClock;
        os <<
//           std::left <<
//           std::setw (kindWidth) <<
          gLanguage->mandatory ();
        break;
      case mfTimingItemKind::kOptional:
        totalOptionalClock += timingItemClock;
        os <<
//           std::left <<
//           std::setw (kindWidth) <<
          gLanguage->optional ();
        break;
    } // switch

    os << ' ' <<
      std::left <<
      std::fixed <<
      std::setprecision (secondsPrecision) <<
      std::setw (secondsWidth) <<
      float(timingItemClock) / CLOCKS_PER_SEC <<
      std::endl;
  } // for
  os << std::endl;

  // set the total spent times columns widths
  int
    totalClockWidth          = 1,
    totalMandatoryClockWidth = 1,
    totalOptionalClockWidth  = 1,
    totalsPrecision          = secondsPrecision;

  switch (gEarlyOptions.getEarlyLanguageKind ()) {
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

  // print the total spent times
  os << std::left <<
    std::setw (totalClockWidth) <<
    gLanguage->totalSeconds () <<
    ' ' <<
    std::setw (totalMandatoryClockWidth) <<
    gLanguage->mandatory () <<
    ' ' <<
    std::setw (totalOptionalClockWidth) <<
    gLanguage->optional () <<
    std::endl <<

    std::setw (totalClockWidth) <<
    mfReplicateString ("-", totalClockWidth) <<
    ' ' <<
    std::setw (totalMandatoryClockWidth) <<
    mfReplicateString ("-", totalMandatoryClockWidth) <<
    ' ' <<
    std::setw (totalOptionalClockWidth) <<
    mfReplicateString ("-", totalOptionalClockWidth) <<
    std::endl <<

    std::fixed <<
    std::setprecision (totalsPrecision) <<

    std::setw (totalClockWidth) <<
    float (totalClock) / CLOCKS_PER_SEC <<
    ' ' <<
    std::setw (totalMandatoryClockWidth) <<
    float(totalMandatoryClock) / CLOCKS_PER_SEC <<
    ' ' <<
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
    gLanguage->timingInformation () <<
    ":" <<
    std::endl << std::endl;

  doPrint (os);
}

std::ostream& operator << (std::ostream& os, const mfTimingItemsList& tim) {
  tim.print(os);
  return os;
}

//________________________________________________________________________
// hidden global timing items list variable
EXP mfTimingItemsList pGlobalTimingItemsList;

EXP mfTimingItemsList& getGlobalTimingItemsList ()
{
  return pGlobalTimingItemsList;
}


}

