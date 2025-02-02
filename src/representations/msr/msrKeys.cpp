/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <regex>
#include <sstream>

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrPitchesNames.h"
#include "msrIntervals.h"

#include "msrMeasureConstants.h"

#include "msrClefsKeysTimeSignatures.h"

#include "oahOah.h"

#include "msrOah.h"

#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// keys
// ------------------------------------------------------

std::string msrKeyKindAsString (
  msrKeyKind keyKind)
{
  std::string result;

  switch (keyKind) {
    case msrKeyKind::kKeyTraditional:
      result = "kKeyTraditional";
      break;
    case msrKeyKind::kKeyHumdrumScot:
      result = "kKeyHumdrumScot";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrKeyKind& elt)
{
  os << msrKeyKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrHumdrumScotKeyItem msrHumdrumScotKeyItem::create (
  int inputLineNumber)
{
  msrHumdrumScotKeyItem* obj =
    new msrHumdrumScotKeyItem (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}

msrHumdrumScotKeyItem::msrHumdrumScotKeyItem (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Creating Humdrum/Scot key item" <<
      ", line: " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fKeyDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fKeyAlterationKind    = msrAlterationKind::kAlteration_UNKNOWN_;
  fKeyOctaveKind        = msrOctaveKind::kOctave_UNKNOWN_;
}

msrHumdrumScotKeyItem::~msrHumdrumScotKeyItem ()
{}

Bool msrHumdrumScotKeyItem::isEqualTo (
  const S_msrHumdrumScotKeyItem&
    otherHumdrumScotKeyItem) const
{
  if (! otherHumdrumScotKeyItem) {
    return false;
  }

  return
    fKeyDiatonicPitchKind == otherHumdrumScotKeyItem->fKeyDiatonicPitchKind
      &&
    fKeyAlterationKind == otherHumdrumScotKeyItem->fKeyAlterationKind
      &&
    fKeyOctaveKind == otherHumdrumScotKeyItem->fKeyOctaveKind;
}

void msrHumdrumScotKeyItem::setKeyItemDiatonicPitchKind (
  msrDiatonicPitchKind diatonicPitchKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Setting Humdrum/Scot key item diatonic pitch to '" <<
      msrDiatonicPitchKindAsString (diatonicPitchKind) <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fKeyDiatonicPitchKind = diatonicPitchKind;
}

void msrHumdrumScotKeyItem::setKeyItemAlterationKind (
  msrAlterationKind alterationKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Setting Humdrum/Scot key item alteration to '" <<
      msrAlterationKindAsString (alterationKind) <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fKeyAlterationKind = alterationKind;
}

void msrHumdrumScotKeyItem::setKeyItemOctaveKind (msrOctaveKind keyOctaveKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Setting Humdrum/Scot key item octave to '" <<
      msrOctaveKindAsString (keyOctaveKind) <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fKeyOctaveKind = keyOctaveKind;
}

void msrHumdrumScotKeyItem::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrHumdrumScotKeyItem::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrHumdrumScotKeyItem::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrHumdrumScotKeyItem::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrHumdrumScotKeyItem::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrHumdrumScotKeyItem::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrHumdrumScotKeyItem::browseData (basevisitor* v)
{}

std::string msrHumdrumScotKeyItem::asString () const
{
  std::stringstream ss;

  ss <<
    "[HumdrumScotKeyItem" <<
    ", fKeyDiatonicPitchKind" << ": " <<
    fKeyDiatonicPitchKind <<
    ", fKeyAlterationKind" << ": " <<
    fKeyAlterationKind <<
    ", fKeyOctaveKind" << ": " <<
    fKeyOctaveKind <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

// std::string msrHumdrumScotKeyItem::asStringForMeasuresSlices () const
// {
//   std::stringstream ss;
//
//   ss <<
//     "HumdrumScotKeyItem" <<
//     ", keyDiatonicPitch" << ": " <<
//     fKeyDiatonicPitchKind <<
//     ", fKeyAlterationKind" << ": " <<
//     fKeyAlterationKind <<
//     ", fKeyOctaveKind" << ": " <<
//     fKeyOctaveKind <<
//     ", line " << fInputStartLineNumber;
//
//   return ss.str ();
// }

void msrHumdrumScotKeyItem::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrHumdrumScotKeyItem& elt)
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
S_msrKey msrKey::createTraditional (
  int                      inputLineNumber,
  const S_msrMeasure&      upLinkToMeasure,
  msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
  msrModeKind              modeKind,
  int                      keyCancel)
{
  msrKey* obj =
    new msrKey (
      inputLineNumber,
      upLinkToMeasure,
      keyTonicQuarterTonesPitchKind,
      modeKind,
      keyCancel);
  assert (obj != nullptr);
  return obj;
}

S_msrKey msrKey::createTraditional (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
  msrModeKind              modeKind,
  int                      keyCancel)
{
  return
    msrKey::createTraditional (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      keyTonicQuarterTonesPitchKind,
      modeKind,
      keyCancel);
}

S_msrKey msrKey::createHumdrumScot ( // for Humdrum/Scot keys
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrKey* obj =
    new msrKey (
      inputLineNumber,
      upLinkToMeasure);
  assert (obj != nullptr);
  return obj;
}

S_msrKey msrKey::createHumdrumScot ( // for Humdrum/Scot keys
  int inputLineNumber)
{
  return
    msrKey::createHumdrumScot (
      inputLineNumber,
      gNullMeasure); // set later in setMeasureElementUpLinkToMeasure()
}

msrKey::msrKey ( // for traditional keys
  int                      inputLineNumber,
  const S_msrMeasure&      upLinkToMeasure,
  msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
  msrModeKind              modeKind,
  int                      keyCancel)
    : msrMeasureElement (
        inputLineNumber)
{
  // this is a traditional key
  fKeyKind = msrKeyKind::kKeyTraditional;

  fModeKind = modeKind;

  /* caution:
    <key>
      <fifths>0</fifths>
      <mode>minor</mode>
    </key>
  is A minor actually, not C minor!
  */

  fKeyTonicQuarterTonesPitchKind = keyTonicQuarterTonesPitchKind;

  switch (fModeKind) {
    case msrModeKind::kMode_UNKNOWN_:
      break;
    case msrModeKind::kModeMajor:
      break;
    case msrModeKind::kModeMinor:
      fKeyTonicQuarterTonesPitchKind =
        noteAtIntervalFromQuarterTonesPitch (
          inputLineNumber,
          msrIntervalKind::kIntervalMajorSixth, // a minor third below actually
          fKeyTonicQuarterTonesPitchKind);
      break;
    case msrModeKind::kModeIonian:
      break;
    case msrModeKind::kModeDorian:
      break;
    case msrModeKind::kModePhrygian:
      break;
    case msrModeKind::kModeLydian:
      break;
    case msrModeKind::kModeMixolydian:
      break;
    case msrModeKind::kModeAeolian:
      break;
    case msrModeKind::kModeLocrian:
      break;
  } // switch

  fKeyCancel = keyCancel;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Creating traditional key '" <<
      this->asString () <<
      '\'' <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
}

msrKey::msrKey ( // for Humdrum/Scot keys
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{
  // this is a Humdrum/Scot key
  fKeyKind = msrKeyKind::kKeyHumdrumScot;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Creating Humdrum/Scot key '" <<
      this->asString () <<
      '\'' <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
}

msrKey::~msrKey ()
{}

// void msrKey::setKeyUpLinkToMeasure (
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
//   if (gTraceOahGroup->getTraceKeys ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of key " <<
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
//   fKeyUpLinkToMeasure = measure;
// }

Bool msrKey::isEqualTo (S_msrKey otherKey) const
{
  if (! otherKey) {
    return false;
  }

  if (
    ! (
        fKeyKind == otherKey->fKeyKind
          &&
        fKeyTonicQuarterTonesPitchKind
          ==
        otherKey->fKeyTonicQuarterTonesPitchKind
          &&
        fModeKind == otherKey->fModeKind
          &&
        fKeyCancel == otherKey->fKeyCancel
      )
    ) {
    return false;
  }

  switch (fKeyKind) {
    case msrKeyKind::kKeyTraditional:
      break;
    case msrKeyKind::kKeyHumdrumScot:
      {
        if (
          ! (
              fKeyItemsOctavesAreSpecified
                ==
              otherKey->fKeyItemsOctavesAreSpecified
                  &&
              fHumdrumScotKeyItemsVector.size ()
                ==
              otherKey->fHumdrumScotKeyItemsVector.size ()
            )
          ) {
          return false;
        }

        for (size_t i = 0; i < fHumdrumScotKeyItemsVector.size (); ++i) {
          if (
            ! (
              fHumdrumScotKeyItemsVector [i]->isEqualTo (
                otherKey->fHumdrumScotKeyItemsVector [i])
              )
            ) {
            return false;
          }
        } // for
      }
      break;
  } // switch

  return true;
}

void msrKey::appendHumdrumScotKeyItem (
  const S_msrHumdrumScotKeyItem& item)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Append item '" <<
      item->asString () <<
      "' to key '" <<
      '\'' <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  // have key items octaves been specified?
  if (item->getKeyItemOctaveKind () != msrOctaveKind::kOctave_UNKNOWN_) {
    fKeyItemsOctavesAreSpecified = true;
  }

  // append the item to the vector
  fHumdrumScotKeyItemsVector.insert (
    fHumdrumScotKeyItemsVector.end (), item);
}

//________________________________________________________________________
S_msrKey msrKey::createTraditionalKeyFromString (
  int           inputLineNumber,
  const std::string& keyString)
{
  /*
    Handles keyString à la LilyPond, such as 'c [major]' or 'bes minor'
  */

  S_msrKey result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating traditional key from string \"" <<
      keyString <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "([[:lower:]]+)"  // tonic pitch
    "[[:space:]]*"
    "?([[:lower:]]*" // mode
    "[[:space:]]*"
    );

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "regularExpression: " <<
      regularExpression;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (keyString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for traditional key string \"" << keyString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  //  Handles keyString à la LilyPond, such as c [major] or bes minor

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      "traditional key string \"" << keyString <<
      "\" is ill-formed";

    msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  std::string
    keyTonic = sm [1],
    keyMode  = sm [2];

  // compute the keyTonicPitchKind from the keyTonic
  msrQuarterTonesPitchKind
    keyQuarterTonesTonicPitchKind =
      quarterTonesPitchKindFromString (
        gMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind (),
        keyTonic);

  // compute the modeKind from the keyMode
  msrModeKind
    keyModeKind =
      modeKindFromString (
        inputLineNumber,
        keyMode);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "keyTonic: \"" <<
      keyTonic <<
      "\"" <<
      std::endl <<
      "keyQuarterTonesTonicPitchKind: \"" <<
      msrQuarterTonesPitchKindAsString (keyQuarterTonesTonicPitchKind) <<
      "\"" <<
      std::endl <<
      "keyMode: \"" <<
      keyMode <<
      "\"" <<
      std::endl <<
      "keyModeKind: \"" <<
      keyModeKind <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the traditional key
  result =
    msrKey::createTraditional (
      __LINE__,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      keyQuarterTonesTonicPitchKind,
      keyModeKind,
      0); // keyCancel JMI

  return result;
}

void msrKey::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrKey::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrKey::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrKey::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrKey::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrKey::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrKey::browseData (basevisitor* v)
{}

std::string msrKey::asString () const
{
  std::stringstream ss;

  ss <<
    "[Key" <<
    ", fKeyKind: " << fKeyKind <<
    ", ";

  switch (fKeyKind) {
    case msrKeyKind::kKeyTraditional:
      ss <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fKeyTonicQuarterTonesPitchKind,
          gMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ()) <<
        ' ' <<
        fModeKind;
      break;

    case msrKeyKind::kKeyHumdrumScot:
      ss <<
        fHumdrumScotKeyItemsVector.size () << "items" <<
        ", keyItemsOctavesAreSpecified: " <<
        fKeyItemsOctavesAreSpecified;
      break;
  } // switch

  ss <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

// std::string msrKey::asStringForMeasuresSlices () const
// {
//   std::stringstream ss;
//
//   ss <<
//     '[';
//
//   switch (fKeyKind) {
//     case msrKeyKind::kKeyTraditional:
//       ss <<
//         msrQuarterTonesPitchKindAsStringInLanguage (
//           fKeyTonicQuarterTonesPitchKind,
//           gMsrOahGroup->
//             getMsrQuarterTonesPitchesLanguageKind ()) <<
//         ' ' <<
//         fModeKind;
//       break;
//
//     case msrKeyKind::kKeyHumdrumScot:
//       ss <<
//         fHumdrumScotKeyItemsVector.size () << "items" <<
//         ", keyItemsOctavesAreSpecified: " <<
//         fKeyItemsOctavesAreSpecified;
//       break;
//   } // switch
//
//   ss <<
// // JMI    ", line " << fInputStartLineNumber <<
//     ']';
//
//   return ss.str ();
// }

void msrKey::print (std::ostream& os) const
{
  os <<
    "[Key" <<
    ", fKeyKind: " << fKeyKind <<
    ",";

  switch (fKeyKind) {
    case msrKeyKind::kKeyTraditional:
      os <<
        ' ' <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fKeyTonicQuarterTonesPitchKind,
          gMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ()) <<
        ' ' <<
        fModeKind <<
        ", line " << fInputStartLineNumber;
      break;

    case msrKeyKind::kKeyHumdrumScot:
      os <<
        ", keyItemsOctavesAreSpecified: " <<
        fKeyItemsOctavesAreSpecified <<
        ", " <<
        fHumdrumScotKeyItemsVector.size () <<
        " items" <<
        ", line " << fInputStartLineNumber <<
        std::endl;

      if (fHumdrumScotKeyItemsVector.size ()) {
        os << std::endl;

        ++gIndenter;

        std::vector <S_msrHumdrumScotKeyItem>::const_iterator
          iBegin = fHumdrumScotKeyItemsVector.begin (),
          iEnd   = fHumdrumScotKeyItemsVector.end (),
          i      = iBegin;

        for ( ; ; ) {
          os << (*i);
          if (++i == iEnd) break;
    // JMI     os << std::endl;
        } // for

        --gIndenter;
      }

      else
        {
          os <<
            " [EMPTY]" <<
            std::endl;
        }
      break;
  } // switch

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrKey& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
