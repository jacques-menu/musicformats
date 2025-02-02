/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <regex>

#include "visitor.h"
//
#include "mfAssert.h"
#include "mfServices.h"

#include "msrBrowsers.h"
#include "msrClefsKeysTimeSignatures.h"

#include "msrWae.h"

#include "oahEarlyOptions.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// clefs
//______________________________________________________________________________
std::map <std::string, msrClefKeyTimeSignatureOrderKind>
  gGlobalClefKeyTimeSignatureOrderKindsMap;

std::list <std::string>
  gClefKeyTimeSignatureOrderKindsNamesList;

std::string msrClefKeyTimeSignatureOrderKindAsString (
  msrClefKeyTimeSignatureOrderKind clefKeyTimeSignatureOrderKind)
{
  std::string result;

  switch (clefKeyTimeSignatureOrderKind) {
    case msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrder_UNKNOWN_:
      result = "kClefKeyTimeSignatureOrder_UNKNOWN_";
      break;
    case msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderClefKeyTimeSignature:
      result = "kClefKeyTimeSignatureOrderClefKeyTimeSignature";
      break;
    case msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderKeyTimeSignatureClef:
      result = "kClefKeyTimeSignatureOrderClefKeyTimeSignature";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrClefKeyTimeSignatureOrderKind& elt)
{
  os << msrClefKeyTimeSignatureOrderKindAsString (elt);
  return os;
}

msrClefKeyTimeSignatureOrderKind msrClefKeyTimeSignatureOrderKindFromString (
  int                inputLineNumber,
  const std::string& clefKeyTimeSignatureString)
{
  msrClefKeyTimeSignatureOrderKind result = msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrder_UNKNOWN_;

  if      (clefKeyTimeSignatureString == "clef-key-time")
    result = msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderClefKeyTimeSignature;
  else if (clefKeyTimeSignatureString == "key-time-clef")
    result = msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderKeyTimeSignatureClef;
  else {
    std::stringstream ss;

    ss <<
      "clef string \"" <<
      clefKeyTimeSignatureString <<
      "\" is unknown" <<
      ", line: " << inputLineNumber;

    msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  return result;
}

void initializeClefKeyTimeSignatureOrderKinds ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
  //  gGlobalClefKeyTimeSignatureOrderKindsMap [gClefKeyTimeSignatureOrderKindsNamesList.back ()] = kMajorClef;

    gClefKeyTimeSignatureOrderKindsNamesList.push_back ("clef-key-time");
    gGlobalClefKeyTimeSignatureOrderKindsMap ["clef-key-time"] = msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderClefKeyTimeSignature;

    gClefKeyTimeSignatureOrderKindsNamesList.push_back ("key-time-clef");
    gGlobalClefKeyTimeSignatureOrderKindsMap ["key-time-clef"] = msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderKeyTimeSignatureClef;

    pPrivateThisMethodHasBeenRun = true;
  }
}

std::string availableClefKeyTimeSignatureOrderKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t clefKeyTimeSignatureOrderKindsMapSize =
    gGlobalClefKeyTimeSignatureOrderKindsMap.size ();

  if (clefKeyTimeSignatureOrderKindsMapSize) {
    size_t nextToLast =
      clefKeyTimeSignatureOrderKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map <std::string, msrClefKeyTimeSignatureOrderKind>::const_iterator i =
        gGlobalClefKeyTimeSignatureOrderKindsMap.begin ();
      i != gGlobalClefKeyTimeSignatureOrderKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != clefKeyTimeSignatureOrderKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

std::string availableClefKeyTimeSignatureOrderKindsNames (size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t clefKeyTimeSignatureOrderKindsNamesMapSize =
    gClefKeyTimeSignatureOrderKindsNamesList.size ();

  if (clefKeyTimeSignatureOrderKindsNamesMapSize) {
    size_t nextToLast =
      clefKeyTimeSignatureOrderKindsNamesMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::list <std::string>::const_iterator i =
        gClefKeyTimeSignatureOrderKindsNamesList.begin ();
      i != gClefKeyTimeSignatureOrderKindsNamesList.end ();
      ++i
    ) {
      std::string theString = (*i);

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != clefKeyTimeSignatureOrderKindsNamesMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

//______________________________________________________________________________
msrClefKeyTimeSignatureOrderKind
  pClefKeyTimeSignatureOrderKind =
    msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderClefKeyTimeSignature; // musician's order

S_msrClefKeyTimeSignatureGroup msrClefKeyTimeSignatureGroup::create (
  int inputLineNumber,
  int groupInputLineNumber)
{
  msrClefKeyTimeSignatureGroup* obj =
    new msrClefKeyTimeSignatureGroup (
      inputLineNumber,
      groupInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

msrClefKeyTimeSignatureGroup::msrClefKeyTimeSignatureGroup (
  int inputLineNumber,
  int groupInputLineNumber)
    : msrMeasureElement (
        inputLineNumber)
{
  fGroupInputLineNumber = groupInputLineNumber;

  fGroupItemsNumber = 0;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Creating an msrClefKeyTimeSignatureGroup" <<
      ", groupInputLineNumber: " << groupInputLineNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrClefKeyTimeSignatureGroup::~msrClefKeyTimeSignatureGroup ()
{}

S_msrClefKeyTimeSignatureGroup msrClefKeyTimeSignatureGroup::createClefKeyTimeSignatureGroupNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of msrClefKeyTimeSignatureGroup " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrClefKeyTimeSignatureGroup
    newbornClone =
      msrClefKeyTimeSignatureGroup::create (
        fInputStartLineNumber,
        fGroupInputLineNumber);

  // JMI ??? v0.9.67
  if (fClef)
    newbornClone->
      setClef (fClef);

  if (fKey)
    newbornClone->
      setKey (fKey);

  if (fTimeSignature)
    newbornClone->
      setTimeSignature (fTimeSignature);

  return newbornClone;
}

// void msrClefKeyTimeSignatureGroup::setClefKeyTimeSignatureGroupUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceClefs ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of clef " <<
//       asString () <<
//       " to measure " << measure->asString () <<
//       "' in measure '" <<
//       measure->asString () <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fClefKeyTimeSignatureGroupUpLinkToMeasure = measure;
// }

void msrClefKeyTimeSignatureGroup::setClef (const S_msrClef& clef)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    clef != nullptr,
    "clef is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Setting the msrClefKeyTimeSignatureGroup clef to " <<
      clef->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fClef) {
    --fGroupItemsNumber;
  }

  fClef = clef;

  if (fClef) {
    ++fGroupItemsNumber;
  }
}

S_msrClef msrClefKeyTimeSignatureGroup::getClef () const
{
  return fClef;
}

void msrClefKeyTimeSignatureGroup::setKey (const S_msrKey& key)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    key != nullptr,
    "key is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Setting the msrClefKeyTimeSignatureGroup key to " <<
      key->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (key) {
    --fGroupItemsNumber;
  }

  fKey = key;

  if (fKey) {
    ++fGroupItemsNumber;
  }
}

S_msrKey msrClefKeyTimeSignatureGroup::getKey () const
{
  return fKey;
}

void msrClefKeyTimeSignatureGroup::setTimeSignature (
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    timeSignature != nullptr,
    "timeSignature is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Setting the msrClefKeyTimeSignatureGroup time signature to " <<
      timeSignature->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (timeSignature) {
    --fGroupItemsNumber;
  }

  fTimeSignature = timeSignature;

  if (timeSignature) {
    ++fGroupItemsNumber;
  }

  // get the measure
//   S_msrMeasure
//     upLinkToMeasure =
//       getMeasureElementUpLinkToMeasure ();

  if (fMeasureElementUpLinkToMeasure) {
    // set the current time signature of the staff if any
    S_msrStaff
      staff =
        fMeasureElementUpLinkToMeasure->
          fetchMeasureUpLinkToStaff ();

    if (staff) {
      staff->
        setStaffCurrentTimeSignature (fTimeSignature);
    }
  }

  // set the measure full measure whole notes duration JMI v0.9.70 BABASSE
  if (fMeasureElementUpLinkToMeasure) {
    fMeasureElementUpLinkToMeasure->
      setFullMeasureWholeNotesDuration (
        fTimeSignature->
          timeSignatureWholeNotesPerMeasure ());
  }
}

S_msrTimeSignature msrClefKeyTimeSignatureGroup::getTimeSignature () const
{
  return fTimeSignature;
}

void msrClefKeyTimeSignatureGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrClefKeyTimeSignatureGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrClefKeyTimeSignatureGroup>*
    p =
      dynamic_cast<visitor<S_msrClefKeyTimeSignatureGroup>*> (v)) {
        S_msrClefKeyTimeSignatureGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrClefKeyTimeSignatureGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrClefKeyTimeSignatureGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrClefKeyTimeSignatureGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrClefKeyTimeSignatureGroup>*
    p =
      dynamic_cast<visitor<S_msrClefKeyTimeSignatureGroup>*> (v)) {
        S_msrClefKeyTimeSignatureGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrClefKeyTimeSignatureGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrClefKeyTimeSignatureGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrClefKeyTimeSignatureGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "--> msrClefKeyTimeSignatureGroup::browseData(), " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (pClefKeyTimeSignatureOrderKind) {
    case msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrder_UNKNOWN_:
      {
        std::stringstream ss;

        ss <<
          "clef, key and time signature browsing order is unknown in score, msrClefKeyTimeSignatureGroup: \"" <<
          pClefKeyTimeSignatureOrderKind <<
          ", fGroupInputLineNumber: " << fGroupInputLineNumber <<
          ", line " << fInputStartLineNumber;

        msrInternalError ( // JMI ???
          gServiceRunData->getInputSourceName (),
          fInputStartLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      break;

    case msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderClefKeyTimeSignature:
      if (fClef) {
        msrBrowser<msrClef> browser (v);
        browser.browse (*fClef);
      }
      if (fKey) {
        msrBrowser<msrKey> browser (v);
        browser.browse (*fKey);
      }
      if (fTimeSignature) {
        msrBrowser<msrTimeSignature> browser (v);
        browser.browse (*fTimeSignature);
      }
      break;

    case msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderKeyTimeSignatureClef:
      if (fKey) {
        msrBrowser<msrKey> browser (v);
        browser.browse (*fKey);
      }
      if (fTimeSignature) {
        msrBrowser<msrTimeSignature> browser (v);
        browser.browse (*fTimeSignature);
      }
      if (fClef) {
        msrBrowser<msrClef> browser (v);
        browser.browse (*fClef);
      }
      break;
  } // switch
}

std::string msrClefKeyTimeSignatureGroup::asString () const
{
  std::stringstream ss;

  ss <<
    "[msrClefKeyTimeSignatureGroup";

  ss <<
    ", fClef: ";
  if (fClef) {
    ss <<
      fClef->asString ();
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", fKey: ";
  if (fKey) {
    ss <<
      fKey->asString ();
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", fTimeSignature: ";
  if (fTimeSignature) {
    ss <<
      fTimeSignature->asString ();
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", fGroupInputLineNumber: " << fGroupInputLineNumber <<
    ", fGroupItemsNumber: " << fGroupItemsNumber <<
//     ", line " << fInputStartLineNumber << // JMI v0.9.70
    ']';

  return ss.str ();
}

// std::string msrClefKeyTimeSignatureGroup::asStringForMeasuresSlices () const
// {
//   std::stringstream ss;
//
//   ss <<
//     '[' <<
//     "msrClefKeyTimeSignatureGroup: " << fClef <<
//     ", fKey: " << fKey <<
//     ", fTimeSignature: " << fTimeSignature <<
//     ", fGroupInputLineNumber: " << fGroupInputLineNumber <<
//     ", fGroupItemsNumber: " << fGroupItemsNumber <<
//     ']';
//
//   return ss.str ();
// }

void msrClefKeyTimeSignatureGroup::print (std::ostream& os) const
{
  os <<
    "[ClefKeyTimeSignatureGroup" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 22;

  os <<
    std::setw (fieldWidth) <<
    "fGroupInputLineNumber" << ": " << fGroupInputLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGroupItemsNumber" << ": " << fGroupItemsNumber <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "line" << ": " << fInputStartLineNumber << // JMI v0.9.70
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fClef" << ": ";
  if (fClef) {
    os << std::endl;
    ++gIndenter;
    os << fClef;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os <<
    std::setw (fieldWidth) <<
    "fKey" << ": ";
  if (fKey) {
    os << std::endl;
    ++gIndenter;
    os << fKey;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os <<
    std::setw (fieldWidth) <<
    "fTimeSignature" << ": ";
  if (fTimeSignature) {
    os << std::endl;
    ++gIndenter;
    os << fTimeSignature;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  --gIndenter;

  os <<
    ']' <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrClefKeyTimeSignatureGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
EXP void setClefKeyTimeSignatureOrderKind (
  msrClefKeyTimeSignatureOrderKind clefKeyTimeSignatureOrderKind)
{
  pClefKeyTimeSignatureOrderKind =
     clefKeyTimeSignatureOrderKind;
}

EXP msrClefKeyTimeSignatureOrderKind getClefKeyTimeSignatureOrderKind ()
{
  return pClefKeyTimeSignatureOrderKind;
}


}
