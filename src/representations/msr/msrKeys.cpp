/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include <regex>

#include "visitor.h"

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrPitchesNames.h"
#include "msrIntervals.h"

#include "msrMeasureConstants.h"

#include "msrKeys.h"

#include "oahOah.h"

#include "msrOah.h"

#include "oahEarlyOptions.h"


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
  msrHumdrumScotKeyItem* o =
    new msrHumdrumScotKeyItem (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

msrHumdrumScotKeyItem::msrHumdrumScotKeyItem (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Creating Humdrum/Scot key item" <<
      ", line: " << inputLineNumber <<
      std::endl;
  }
#endif

  fKeyDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fKeyAlterationKind    = msrAlterationKind::kAlteration_NO_;
  fKeyOctaveKind        = msrOctaveKind::kOctave_NO_;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Setting Humdrum/Scot key item diatonic pitch to '" <<
      msrDiatonicPitchKindAsString (diatonicPitchKind) <<
      "'" <<
      std::endl;
  }
#endif

  fKeyDiatonicPitchKind = diatonicPitchKind;
}

void msrHumdrumScotKeyItem::setKeyItemAlterationKind (
  msrAlterationKind alterationKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Setting Humdrum/Scot key item alteration to '" <<
      msrAlterationKindAsString (alterationKind) <<
      "'" <<
      std::endl;
  }
#endif

  fKeyAlterationKind = alterationKind;
}

void msrHumdrumScotKeyItem::setKeyItemOctaveKind (msrOctaveKind keyOctaveKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Setting Humdrum/Scot key item octave to '" <<
      msrOctaveKindAsString (keyOctaveKind) <<
      "'" <<
      std::endl;
  }
#endif

  fKeyOctaveKind = keyOctaveKind;
}

void msrHumdrumScotKeyItem::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHumdrumScotKeyItem::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHumdrumScotKeyItem::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrHumdrumScotKeyItem::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHumdrumScotKeyItem::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHumdrumScotKeyItem::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrHumdrumScotKeyItem::browseData (basevisitor* v)
{}

std::string msrHumdrumScotKeyItem::asString () const
{
  std::stringstream s;

  s <<
    "[HumdrumScotKeyItem" <<
    ", keyDiatonicPitch" << ": " <<
    msrDiatonicPitchKindAsString (fKeyDiatonicPitchKind) <<
    ", keyAlteration" << ": " <<
    msrAlterationKindAsString (fKeyAlterationKind) <<
    ", keyOctaveKind" << ": " <<
    msrOctaveKindAsString (fKeyOctaveKind) <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrHumdrumScotKeyItem::asShortStringForMeasuresSlices () const
{
  std::stringstream s;

  s <<
    "HumdrumScotKeyItem" <<
    ", keyDiatonicPitch" << ": " <<
    msrDiatonicPitchKindAsString (fKeyDiatonicPitchKind) <<
    ", keyAlteration" << ": " <<
    msrAlterationKindAsString (fKeyAlterationKind) <<
    ", keyOctaveKind" << ": " <<
    msrOctaveKindAsString (fKeyOctaveKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

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
    os << "[NONE]" << std::endl;
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
  msrKey* o =
    new msrKey (
      inputLineNumber,
      upLinkToMeasure,
      keyTonicQuarterTonesPitchKind,
      modeKind,
      keyCancel);
  assert (o != nullptr);
  return o;
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
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      keyTonicQuarterTonesPitchKind,
      modeKind,
      keyCancel);
}

S_msrKey msrKey::createHumdrumScot ( // for Humdrum/Scot keys
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrKey* o =
    new msrKey (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrKey msrKey::createHumdrumScot ( // for Humdrum/Scot keys
  int inputLineNumber)
{
  return
    msrKey::createHumdrumScot (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
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
    case msrModeKind::kMode_NO_:
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Creating traditional key '" <<
      this->asString () <<
      "'" <<
      std::endl;
    }
#endif
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Creating Humdrum/Scot key '" <<
      this->asString () <<
      "'" <<
      std::endl;
    }
#endif
}

msrKey::~msrKey ()
{}

void msrKey::setKeyUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of key " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fKeyUpLinkToMeasure = measure;
}

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Append item '" <<
      item->asString () <<
      "' to key '" <<
      "'" <<
      std::endl;
    }
#endif

  // have key items octaves been specified?
  if (item->getKeyItemOctaveKind () != msrOctaveKind::kOctave_NO_) {
    fKeyItemsOctavesAreSpecified = true;
  }

  // append the item to the std::vector
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating traditional key from std::string \"" <<
      keyString <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string regularExpression (
    "[[:space:]]*"
    "([[:lower:]]+)"  // tonic pitch
    "[[:space:]]*"
    "?([[:lower:]]*" // mode
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      std::endl;
  }
#endif

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (keyString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for traditional key std::string \"" << keyString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  //  Handles keyString à la LilyPond, such as c [major] or bes minor

  if (smSize != 3) {
    std::stringstream s;

    s <<
      "traditional key std::string \"" << keyString <<
      "\" is ill-formed";

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  std::string
    keyTonic = sm [1],
    keyMode  = sm [2];

  // compute the keyTonicPitchKind from the keyTonic
  msrQuarterTonesPitchKind
    keyQuarterTonesTonicPitchKind =
      quarterTonesPitchKindFromString (
        gGlobalMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind (),
        keyTonic);

  // compute the modeKind from the keyMode
  msrModeKind
    keyModeKind =
      modeKindFromString (
        inputLineNumber,
        keyMode);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "keyTonic = \"" <<
      keyTonic <<
      "\"" <<
      std::endl <<
      "keyQuarterTonesTonicPitchKind = \"" <<
      msrQuarterTonesPitchKindAsString (keyQuarterTonesTonicPitchKind) <<
      "\"" <<
      std::endl <<
      "keyMode = \"" <<
      keyMode <<
      "\"" <<
      std::endl <<
      "keyModeKind = \"" <<
      keyModeKind <<
      "\"" <<
      std::endl;
  }
#endif

  // create the traditional key
  result =
    msrKey::createTraditional (
      __LINE__,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      keyQuarterTonesTonicPitchKind,
      keyModeKind,
      0); // keyCancel JMI

  return result;
}

void msrKey::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrKey::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrKey::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrKey::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrKey::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrKey::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrKey::browseData (basevisitor* v)
{}

std::string msrKey::asString () const
{
  std::stringstream s;

  s <<
    "[Key" <<
    ", fKeyKind: " << fKeyKind <<
    ", ";

  switch (fKeyKind) {
    case msrKeyKind::kKeyTraditional:
      s <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fKeyTonicQuarterTonesPitchKind,
          gGlobalMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ()) <<
        ' ' <<
        fModeKind;
      break;

    case msrKeyKind::kKeyHumdrumScot:
      s <<
        fHumdrumScotKeyItemsVector.size () << "items" <<
        ", keyItemsOctavesAreSpecified: " <<
        fKeyItemsOctavesAreSpecified;
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrKey::asShortStringForMeasuresSlices () const
{
  std::stringstream s;

  s <<
    '[';

  switch (fKeyKind) {
    case msrKeyKind::kKeyTraditional:
      s <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fKeyTonicQuarterTonesPitchKind,
          gGlobalMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ()) <<
        ' ' <<
        fModeKind;
      break;

    case msrKeyKind::kKeyHumdrumScot:
      s <<
        fHumdrumScotKeyItemsVector.size () << "items" <<
        ", keyItemsOctavesAreSpecified: " <<
        fKeyItemsOctavesAreSpecified;
      break;
  } // switch

  s <<
// JMI    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

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
          gGlobalMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ()) <<
        ' ' <<
        fModeKind <<
        ", line " << fInputLineNumber;
      break;

    case msrKeyKind::kKeyHumdrumScot:
      os <<
        ", keyItemsOctavesAreSpecified: " <<
        fKeyItemsOctavesAreSpecified <<
        ", " <<
        fHumdrumScotKeyItemsVector.size () <<
        " items" <<
        ", line " << fInputLineNumber <<
        std::endl;

      if (fHumdrumScotKeyItemsVector.size ()) {
        os << std::endl;

        ++gIndenter;

        std::vector<S_msrHumdrumScotKeyItem>::const_iterator
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
            " [NONE]" <<
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
