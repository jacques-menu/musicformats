/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfStringsHandling.h"
#include "mfIndentedTextOutput.h"


#include "oahEarlyOptions.h"

#include "mfcBasicTypes.h"
#include "mfLibraryComponent.h"

#include "oahComponent.h"
#include "oahWae.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
void crackVersionNumber ( // JMI ??? v0.9.66
  const std::string& theString,
  int&  generationNumber,
  int&  majorNumber,
  int&  minorNumber)
{
  // obtains the three numbers in "2.19.83" or "2.20" for example

  // decipher theString with a three-number regular expression
  std::string regularExpression (
    "([[:digit:]]+)" // generation number
    "."
    "([[:digit:]]+)" // major number
    "."
    "([[:digit:]]+)" // minor number
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for version std::string \"" << theString <<
      "\" with std::regex \"" << regularExpression <<
      '\"' <<
      std::endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLog <<
          '[' << sm [i] << "] ";
      } // for

      gLog << std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 4) {
    // found an n.x.y specification
    std::string
      generationNumberValue = sm [1],
      majorNumberValue      = sm [2],
      minorNumberValue      = sm [3];

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
        "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
        "--> minorNumberValue = \"" << minorNumberValue << "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    generationNumber = stoi (generationNumberValue);
    majorNumber      = stoi (majorNumberValue);
    minorNumber      = stoi (minorNumberValue);
  }

  else {
    // decipher theString with a two-number regular expression
    std::string regularExpression (
      "([[:digit:]]+)" // generation number
      "."
      "([[:digit:]]+)" // major number
      );

    std::regex  e (regularExpression);
    std::smatch sm;

    regex_match (theString, sm, e);

    size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "There are " << smSize << " matches" <<
        " for chord details std::string '" << theString <<
        "\" with std::regex '" << regularExpression <<
        '\'' <<
        std::endl <<
        smSize << " elements: ";

        for (unsigned i = 0; i < smSize; ++i) {
          ss <<
            '[' << sm [i] << "] ";
        } // for

        ss << std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

    if (smSize == 3) {
      // found an n.x specification
      // assume implicit 0 minor number
      std::string
        generationNumberValue = sm [1],
        majorNumberValue      = sm [2];

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getEarlyTraceComponents ()) {
        std::stringstream ss;

        ss <<
          "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
          "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      generationNumber = stoi (generationNumberValue);
      majorNumber      = stoi (majorNumberValue);
      minorNumber      = 0;
    }

    else {
      std::stringstream ss;

      ss <<
        "version number argument '" << theString <<
        "\" is ill-formed";

      oahError (ss.str ());
    }
  }
}

//______________________________________________________________________________
Bool versionNumberGreaterThanOrEqualTo (
  const std::string& versionNumber,
  const std::string& otherVersionNumber)
{
  Bool result (false);

  int
    versionGenerationNumber,
    versionMajorNumber,
    versionMinorNumber;

  crackVersionNumber (
    versionNumber,
    versionGenerationNumber,
    versionMajorNumber,
    versionMinorNumber);

  int
    otherVersionNumbeGenerationNumber,
    otherVersionNumbeMajorNumber,
    otherVersionNumbeMinorNumber;

  crackVersionNumber (
    otherVersionNumber,
    otherVersionNumbeGenerationNumber,
    otherVersionNumbeMajorNumber,
    otherVersionNumbeMinorNumber);

  Bool doTrace (false); // JMI

  if (doTrace) {
    gLog <<
      "versionNumber: " << versionNumber <<
      ", versionGenerationNumber:" << versionGenerationNumber <<
      ", versionMajorNumber:" << versionMajorNumber <<
      ", versionMinorNumber:" << versionMinorNumber <<
      std::endl;
  }

  if (doTrace) {
    gLog <<
      "otherVersionNumber: " << otherVersionNumber <<
      ", otherVersionNumbeGenerationNumber:" << otherVersionNumbeGenerationNumber <<
      ", otherVersionNumbeMajorNumber:" << otherVersionNumbeMajorNumber <<
      ", otherVersionNumbeMinorNumber:" << otherVersionNumbeMinorNumber <<
      std::endl;
  }

  if (otherVersionNumbeGenerationNumber != 2) {
    gLog <<
      "Using verstion \"" <<
      otherVersionNumbeGenerationNumber << ".x.y\" " <<
      "is probably not such a good idea" <<
      std::endl;
  }

  if (otherVersionNumbeMajorNumber < 19) {
    gLog <<
      "Using a verstion older than \"" <<
      otherVersionNumbeGenerationNumber << ".19.y\" " <<
      "is not a good idea: the generated LilyPond code uses features introduced in the latter" <<
      std::endl;
  }

  if (versionGenerationNumber < otherVersionNumbeGenerationNumber) {
    result = false;
  }
  else if (versionGenerationNumber > otherVersionNumbeGenerationNumber) {
    result = true;
  }
  else {
    // same generation number
    if (versionMajorNumber < otherVersionNumbeMajorNumber) {
      result = false;
    }
    else if (versionMajorNumber > otherVersionNumbeMajorNumber) {
      result = true;
    }
    else {
      // same major number
      result =
        versionMinorNumber >= otherVersionNumbeMinorNumber;
    }
  }


  if (doTrace) {
    gLog <<
      "----> versionNumberGreaterThanOrEqualTo =====> " <<
      result <<
      std::endl;
  }

  return result;
}

//______________________________________________________________________________
/*
*** MusicFormats uses semantic versioning inspired from https://semver.org

Once a versioned package has been released, the contents of that version MUST NOT be modified. Any modifications MUST be released as a new version.

Major version zero (0.y.z) is for initial development. Anything MAY change at any time. The public API SHOULD NOT be considered stable.

Version 1.0.0 defines the public API. The way in which the version number is incremented after this release is dependent on this public API and how it changes.


Patch version Z (x.y.Z | x > 0) MUST be incremented if only backwards compatible bug fixes are introduced. A bug fix is defined as an internal change that fixes incorrect behavior.

Minor version Y (x.Y.z | x > 0) MUST be incremented if new, backwards compatible functionality is introduced to the public API. It MUST be incremented if any public API functionality is marked as deprecated. It MAY be incremented if substantial new functionality or improvements are introduced within the private code. It MAY include patch level changes. Patch version MUST be reset to 0 when minor version is incremented.

Major version X (X.y.z | X > 0) MUST be incremented if any backwards incompatible changes are introduced to the public API. It MAY also include minor and patch level changes. Patch and minor versions MUST be reset to 0 when major version is incremented.
*/


/*
A pre-release version MAY be denoted by appending a hyphen and a series of dot separated identifiers immediately following the patch version. Identifiers MUST comprise only ASCII alphanumerics and hyphens [0-9A-Za-z-]. Identifiers MUST NOT be empty. Numeric identifiers MUST NOT include leading zeroes. Pre-release versions have a lower precedence than the associated normal version. A pre-release version indicates that the version is unstable and might not satisfy the intended compatibility requirements as denoted by its associated normal version. Examples: 1.0.0-alpha, 1.0.0-alpha.1, 1.0.0-0.3.7, 1.0.0-x.7.z.92, 1.0.0-x-y-z.–.

Build metadata MAY be denoted by appending a plus sign and a series of dot separated identifiers immediately following the patch or pre-release version. Identifiers MUST comprise only ASCII alphanumerics and hyphens [0-9A-Za-z-]. Identifiers MUST NOT be empty. Build metadata MUST be ignored when determining version precedence. Thus two versions that differ only in the build metadata, have the same precedence. Examples: 1.0.0-alpha+001, 1.0.0+20130313144700, 1.0.0-beta+exp.sha.5114f85, 1.0.0+21AF26D3—-117B344092BD.

Precedence refers to how versions are compared to each other when ordered.

Precedence MUST be calculated by separating the version into major, minor, patch and pre-release identifiers in that order (Build metadata does not figure into precedence).

Precedence is determined by the first difference when comparing each of these identifiers from left to right as follows: Major, minor, and patch versions are always compared numerically.

Example: 1.0.0 < 2.0.0 < 2.1.0 < 2.1.1.

When major, minor, and patch are equal, a pre-release version has lower precedence than a normal version:

Example: 1.0.0-alpha < 1.0.0.

Precedence for two pre-release versions with the same major, minor, and patch version MUST be determined by comparing each dot separated identifier from left to right until a difference is found as follows:

Identifiers consisting of only digits are compared numerically.

Identifiers with letters or hyphens are compared lexically in ASCII sort order.

Numeric identifiers always have lower precedence than non-numeric identifiers.

A larger set of pre-release fields has a higher precedence than a smaller set, if all of the preceding identifiers are equal.

Example: 1.0.0-alpha < 1.0.0-alpha.1 < 1.0.0-alpha.beta < 1.0.0-beta < 1.0.0-beta.2 < 1.0.0-beta.11 < 1.0.0-rc.1 < 1.0.0.
*/

S_mfcVersionNumber mfcVersionNumber::create (
  int                majorNumber,
  int                minorNumber,
  int                patchNumber,
  const std::string& preRelease)
{
  mfcVersionNumber* obj =
    new mfcVersionNumber (
      majorNumber,
      minorNumber,
      patchNumber,
      preRelease);
  assert (obj != nullptr);
  return obj;
}

mfcVersionNumber::mfcVersionNumber (
  int                majorNumber,
  int                minorNumber,
  int                patchNumber,
  const std::string& preRelease)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcVersionNumber" <<
      ", majorNumber:" << majorNumber <<
      ", minorNumber:" << minorNumber <<
      ", patchNumber:" << patchNumber <<
      ", preRelease:" << preRelease;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMajorNumber = majorNumber;
  fMinorNumber = minorNumber;
  fPatchNumber = patchNumber;
  fPreRelease  = preRelease;
}

mfcVersionNumber::~mfcVersionNumber ()
{}

S_mfcVersionNumber mfcVersionNumber::createFromString (
  const std::string& theString)
{
  // obtains the three numbers in "2.19.83" or "2.20" for example

  S_mfcVersionNumber result;

// ^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$


  // decipher theString with a regular expression
  std::string regularExpression (
    "^([[:digit:]]+)"                   // major number
    "."
    "([[:digit:]]+)"                    // minor number
    "."
    "([[:digit:]]+)"                    // patch number
    "(-.*)?"                            // prerelease
//    "(-((-|[[:digit:]]|[[:alpha:]])+))?"
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for version std::string '" << theString <<
      "\" with std::regex '" << regularExpression <<
      '\'' <<
      std::endl <<
      smSize << " elements: ";

    for (unsigned i = 0; i < smSize; ++i) {
      ss <<
        '[' << sm [i] << "] ";
    } // for

    ss << std::endl;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int
    majorNumber = 0,
    minorNumber = 0,
    patchNumber = 0;

  std::string
    preReleaseValue;

  if (smSize == 5) {
    // found an n.x.y specification
    std::string
      majorNumberValue      = sm [1],
      minorNumberValue      = sm [2],
      patchNumberValue      = sm [3];

    preReleaseValue         = sm [4];

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
        "--> minorNumberValue = \"" << minorNumberValue << "\"" <<
        "--> patchNumberValue = \"" << patchNumberValue << "\"" <<
        "--> preReleaseValue = \"" << preReleaseValue << "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    majorNumber      = stoi (majorNumberValue);
    minorNumber      = stoi (minorNumberValue);
    patchNumber      = stoi (patchNumberValue);
  }

  else {
    std::stringstream ss;

    ss <<
      "version number \"" <<
      theString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  // create the version number
  result =
    mfcVersionNumber::create (
      majorNumber,
      minorNumber,
      patchNumber,
      preReleaseValue);

  return result;
}

Bool mfcVersionNumber::operator== (const mfcVersionNumber& other) const
{
  const int
    otherMajorNumber =
      other.getMajorNumber (),
    otherMinorNumber =
      other.getMinorNumber (),
    otherPatchNumber =
      other.getPatchNumber ();

  const std::string
    otherPreRelease =
      other.getPreRelease ();

  Bool doTrace (false); // JMI

  if (doTrace) {
    gLog <<
      ", fMajorNumber:" << fMajorNumber <<
      ", fMinorNumber:" << fMinorNumber <<
      ", fPatchNumber:" << fPatchNumber <<
      ", fPreRelease:" << fPreRelease <<
      std::endl <<
      ", otherMajorNumber:" << otherMajorNumber <<
      ", otherMinorNumber:" << otherMinorNumber <<
      ", otherPatchNumber:" << otherPatchNumber <<
      ", otherPreRelease:" << otherPreRelease <<
      std::endl;
  }

  return
    fMinorNumber == otherMinorNumber
      &&
    fMinorNumber == otherMinorNumber
      &&
    fPatchNumber == otherPatchNumber
      &&
    fPreRelease  == otherPreRelease;
}

Bool mfcVersionNumber::operator!= (const mfcVersionNumber& other) const
{
  return ! (*this == other);
}

Bool mfcVersionNumber::operator< (const mfcVersionNumber& other) const
{
  return ! (*this >= other);
}

Bool mfcVersionNumber::operator>= (const mfcVersionNumber& other) const
{
  Bool result (false);

  const int
    otherMajorNumber =
      other.getMajorNumber (),
    otherMinorNumber =
      other.getMinorNumber (),
    otherPatchNumber =
      other.getPatchNumber ();

  const std::string
    otherPreRelease =
      other.getPreRelease ();

  Bool doTrace (false); // JMI

  if (doTrace) {
    gLog <<
      ", fMajorNumber:" << fMajorNumber <<
      ", fMinorNumber:" << fMinorNumber <<
      ", fPatchNumber:" << fPatchNumber <<
      ", fPreRelease:" << fPreRelease <<
      std::endl <<
      ", otherMajorNumber:" << otherMajorNumber <<
      ", otherMinorNumber:" << otherMinorNumber <<
      ", otherPatchNumber:" << otherPatchNumber <<
      ", otherPreRelease:" << otherPreRelease <<
      std::endl;
  }

  if (fMajorNumber < otherMajorNumber) {
    result = false;
  }
  else if (fMajorNumber > otherMajorNumber) {
    result = true;
  }

  else {
    // same major number
    if (fMinorNumber < otherMinorNumber) {
      result = false;
    }
    else if (fMinorNumber > otherMinorNumber) {
      result = true;
    }

    else {
      // same minor number
      if (fPatchNumber < otherPatchNumber) {
        result = false;
      }
      else if (fPatchNumber > otherPatchNumber) {
        result = true;
      }

      else {
        // same patch number
        result =
          fPreRelease >= otherPreRelease;
      }
    }
  }

  if (doTrace) {
    gLog <<
      "----> mfcVersionNumber::operator>= =====> " <<
      result <<
      std::endl;
  }

  return result;
}

Bool mfcVersionNumber::operator> (const mfcVersionNumber& other) const
{
  return ! (*this <= other);
}

Bool mfcVersionNumber::operator<= (const mfcVersionNumber& other) const
{
  Bool result (false);

  const int
    otherMajorNumber =
      other.getMajorNumber (),
    otherMinorNumber =
      other.getMinorNumber (),
    otherPatchNumber =
      other.getPatchNumber ();

  const std::string
    otherPreRelease =
      other.getPreRelease ();

  Bool doTrace (false); // JMI

  if (doTrace) {
    gLog <<
      ", fMajorNumber:" << fMajorNumber <<
      ", fMinorNumber:" << fMinorNumber <<
      ", fPatchNumber:" << fPatchNumber <<
      ", fPreRelease:" << fPreRelease <<
      std::endl <<
      ", otherMajorNumber:" << otherMajorNumber <<
      ", otherMinorNumber:" << otherMinorNumber <<
      ", otherPatchNumber:" << otherPatchNumber <<
      ", otherPreRelease:" << otherPreRelease <<
      std::endl;
  }

  if (fMajorNumber > otherMajorNumber) {
    result = false;
  }
  else if (fMajorNumber < otherMajorNumber) {
    result = true;
  }

  else {
    // same major number
    if (fMinorNumber > otherMinorNumber) {
      result = false;
    }
    else if (fMinorNumber < otherMinorNumber) {
      result = true;
    }

    else {
      // same minor number
      if (fPatchNumber > otherPatchNumber) {
        result = false;
      }
      else if (fPatchNumber < otherPatchNumber) {
        result = true;
      }

      else {
        // same patch number
        result =
          fPreRelease <= otherPreRelease;
      }
    }
  }

  if (doTrace) {
    gLog <<
      "----> mfcVersionNumber::operator<= =====> " <<
      result <<
      std::endl;
  }

  return result;
}

std::string mfcVersionNumber::asString () const
{
  std::stringstream ss;

  ss <<
    fMajorNumber <<
    "." <<
    fMinorNumber <<
    "." <<
    fPatchNumber;

  if (fPreRelease.size ()) {
    ss <<
      fPreRelease;
   }

  return ss.str ();
}

void mfcVersionNumber::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfcVersionNumber& elt)
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
S_mfcVersion mfcVersion::create (
  const S_mfcVersionNumber&     versionNumber,
  const std::string&            versionDate,
  const std::list <std::string>& versionDescriptionItems)
{
  mfcVersion* obj =
    new mfcVersion (
      versionNumber,
      versionDate,
      versionDescriptionItems);
  assert (obj != nullptr);
  return obj;
}

mfcVersion::mfcVersion (
  const S_mfcVersionNumber&     versionNumber,
  const std::string&            versionDate,
  const std::list <std::string>& versionDescriptionItems)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcVersion" <<
      ", versionNumber:" << versionNumber <<
      ", versionDate:" << versionDate <<
      ", versionDescriptionItems:" <<
      std::endl;

    ++gIndenter;
    for (std::string const& item : versionDescriptionItems) {
      gLog << item << std::endl;
    } // for
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fVersionNumber       = versionNumber;
  fVersionDate         = versionDate;

  fVersionDescriptionItems = versionDescriptionItems;
}

mfcVersion::~mfcVersion ()
{}

std::string mfcVersion::asString () const
{
  std::stringstream ss;

  ss <<
    fVersionNumber->asString () <<
    " (" << fVersionDate << ")";

  return ss.str ();
}

void mfcVersion::print (std::ostream& os) const
{
  os <<
    asString () <<
    ":" <<
    std::endl;

  ++gIndenter;

  for (std::string const& item : fVersionDescriptionItems) {
    gLog << item << std::endl;
  } // for

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_mfcVersion& elt)
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
S_mfcVersionsHistory mfcVersionsHistory::create ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating mfcVersionsHistory";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfcVersionsHistory* obj =
    new mfcVersionsHistory ();
  assert (obj != nullptr);
  return obj;
}

mfcVersionsHistory::mfcVersionsHistory ()
{

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcVersionsHistory";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

mfcVersionsHistory::~mfcVersionsHistory ()
{}

void mfcVersionsHistory::appendVersionToHistory (
  const S_mfcVersion& version)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Appending version " <<
      version <<
      " to history";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVersionsList.push_back (version);
}

S_mfcVersion mfcVersionsHistory::fetchMostRecentVersion () const
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVersionsList.size () > 0,
    "fVersionsList is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  return fVersionsList.back ();
}

void mfcVersionsHistory::print (std::ostream& os) const
{
  std::list <S_mfcVersion>::const_iterator
    iBegin = fVersionsList.begin (),
    iEnd   = fVersionsList.end (),
    i      = iBegin;
  for ( ; ; ) {
    S_mfcVersion
      version = (*i);

    version->print (os);
    if (++i == iEnd) break;
    os << std::endl;
  } // for
}

std::ostream& operator << (std::ostream& os, const S_mfcVersionsHistory& elt)
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
std::string mfcComponentKindAsString (
  mfcComponentKind componentKind)
{
  std::string result;

  switch (componentKind) {
    case mfcComponentKind::kComponentRepresentation:
      result = "representation";
      break;
    case mfcComponentKind::kComponentPass:
      result = "pass";
      break;
    case mfcComponentKind::kComponentGenerator:
      result = "generator";
      break;
    case mfcComponentKind::kComponentConverter:
      result = "converter";
      break;
    case mfcComponentKind::kComponentLibrary:
      result = "library";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mfcComponentKind& elt)
{
  os << mfcComponentKindAsString (elt);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_mfcComponent mfcComponent::create (
  const std::string& componentName,
  mfcComponentKind   componentKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating mfcComponent" <<
      ", componentName: " << componentName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfcComponent* obj =
    new mfcComponent (
      componentName,
      componentKind);
  assert (obj != nullptr);
  return obj;
}
*/

mfcComponent::mfcComponent (
  const std::string& componentName,
  mfcComponentKind   componentKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcComponent \"" <<
      componentName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fComponentName = componentName;
  fComponentKind = componentKind;

  // create the versions history
  fVersionsHistory = mfcVersionsHistory::create ();
}

mfcComponent::~mfcComponent ()
{}

void mfcComponent::appendVersionToComponent (
  const S_mfcVersion& version)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Appending version " <<
      version <<
      " to " <<
      fComponentKind <<
      ' ' <<
      fComponentName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVersionsHistory->
    appendVersionToHistory (version);
}

std::string mfcComponent::asString () const
{
  std::stringstream ss;

  S_mfcVersion
    componentMostRecentVersion =
      fetchComponentMostRecentVersion ();

  ss <<
    fComponentName <<
    ' ' <<
    fComponentKind <<
    ' ' <<
    componentMostRecentVersion->
      getVersionNumber ()->
        asString () <<
    " (" <<
    componentMostRecentVersion->
      getVersionDate () <<
    ")" <<
    std::endl;

  return ss.str ();
}

std::string mfcComponent::mostRecentVersionNumberAndDateAsString () const
{
  std::stringstream ss;

  S_mfcVersion
    componentMostRecentVersion =
      fetchComponentMostRecentVersion ();

  ss <<
    componentMostRecentVersion->
      getVersionNumber ()->
        asString () <<
    " (" <<
    componentMostRecentVersion->
      getVersionDate () <<
    ")";

  return ss.str ();
}

void mfcComponent::print (std::ostream& os) const
{

  os << asString () << std::endl;
}

void mfcComponent::printOwnHistory (std::ostream&  os) const
{
  os <<
    "Own history:" <<
    std::endl;

  ++gIndenter;
  fVersionsHistory->print (os);
  --gIndenter;
}

void mfcComponent::printVersion (std::ostream& os) const
{
  S_mfcVersion
    componentMostRecentVersion =
      fetchComponentMostRecentVersion ();

  os <<
    fComponentName <<
    ' ' <<
    fComponentKind <<
    ' ' <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    std::endl;
}

void mfcComponent::printHistory (std::ostream&  os) const
{
  fVersionsHistory->print (os);
}

std::ostream& operator << (std::ostream& os, const S_mfcComponent& elt)
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
S_mfcOahComponent mfcOahComponent::create (
  const std::string& formatName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating mfcOahComponent" <<
      ", formatName: " << formatName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfcOahComponent* obj =
    new mfcOahComponent (
      formatName);
  assert (obj != nullptr);
  return obj;
}

mfcOahComponent::mfcOahComponent (
  const std::string& formatName)
  : mfcComponent (
      formatName,
      mfcComponentKind::kComponentRepresentation)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcOahComponent \"" <<
      formatName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

mfcOahComponent::~mfcOahComponent ()
{}

std::ostream& operator << (std::ostream& os, const S_mfcOahComponent& elt)
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
S_mfcRepresentationComponent mfcRepresentationComponent::create (
  const std::string& formatName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating mfcRepresentationComponent" <<
      ", formatName: " << formatName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfcRepresentationComponent* obj =
    new mfcRepresentationComponent (
      formatName);
  assert (obj != nullptr);
  return obj;
}

mfcRepresentationComponent::mfcRepresentationComponent (
  const std::string& formatName)
  : mfcComponent (
      formatName,
      mfcComponentKind::kComponentRepresentation)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcRepresentationComponent \"" <<
      formatName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

mfcRepresentationComponent::~mfcRepresentationComponent ()
{}

std::ostream& operator << (std::ostream& os, const S_mfcRepresentationComponent& elt)
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
S_mfcPassComponent mfcPassComponent::create (
  const std::string& passName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating mfcPassComponent" <<
      ", passName: " << passName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfcPassComponent* obj =
    new mfcPassComponent (
      passName);
  assert (obj != nullptr);
  return obj;
}

mfcPassComponent::mfcPassComponent (
  const std::string& passName)
  : mfcComponent (
      passName,
      mfcComponentKind::kComponentPass)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcPassComponent \"" <<
      passName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

mfcPassComponent::~mfcPassComponent ()
{}

std::ostream& operator << (std::ostream& os, const S_mfcPassComponent& elt)
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
std::string mfcComponentUsedFromTheCLIKindAsString (
  mfcMultiComponentUsedFromTheCLIKind componentUsedFromTheCLIKind)
{
  std::string result;

  switch (componentUsedFromTheCLIKind) {
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes:
      result = "kComponentUsedFromTheCLIYes";
      break;
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLINo:
      result = "kComponentUsedFromTheCLINo";
      break;
  } // switch

  return result;
}

std::string mfcComponentEntropicityKindAsString (
  mfcMultiComponentEntropicityKind componentEntropicityKind)
{
  std::string result;

  switch (componentEntropicityKind) {
    case mfcMultiComponentEntropicityKind::kComponentEntropicityYes:
      result = "kComponentEntropicityYes";
      break;
    case mfcMultiComponentEntropicityKind::kComponentEntropicityNo:
      result = "kComponentEntropicityNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
/* this class is purely virtual
S_mfcMultiComponent mfcMultiComponent::create (
  const std::string& multiComponentName,
  mfcComponentKind   componentKind,
  mfcMultiComponentEntropicityKind
                     componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                     componentUsedFromTheCLIKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating mfcMultiComponent" <<
      ", multiComponentName: " << multiComponentName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfcMultiComponent* obj =
    new mfcMultiComponent (
      multiComponentName,
      componentKind,
      componentEntropicityKind,
      componentUsedFromTheCLIKind);
  assert (obj != nullptr);
  return obj;
}
*/

mfcMultiComponent::mfcMultiComponent (
  const std::string& multiComponentName,
  mfcComponentKind   componentKind,
  mfcMultiComponentEntropicityKind
                     componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                     componentUsedFromTheCLIKind)
  : mfcComponent (
      multiComponentName,
      componentKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcMultiComponent \"" <<
      multiComponentName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fComponentEntropicityKind =
    mfcMultiComponentEntropicityKind::kComponentEntropicityYes; // JMI v0.9.66 componentEntropicityKind; SUPERFLOUS ???

  fComponentUsedFromTheCLIKind = componentUsedFromTheCLIKind;

  // create the OAH component
  fOahComponent =
    createOahComponent ();
}

mfcMultiComponent::~mfcMultiComponent ()
{}

void mfcMultiComponent::appendRepresentationToMultiComponent (
  const S_mfcRepresentationComponent& format)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Appending format " <<
      format <<
      " to " <<
      fComponentName <<
      ' ' <<
      fComponentKind <<
      ' ' <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    format != nullptr,
    "format is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  switch (fComponentEntropicityKind) {
    case mfcMultiComponentEntropicityKind::kComponentEntropicityYes:
      {
        // is the multi-component versions numbering consistent?
        S_mfcVersionNumber
          multiComponentMostRecentVersionNumber =
            fetchComponentMostRecentVersionNumber (),
          formatComponentMostRecentVersionNumber =
            format->
              fetchComponentMostRecentVersionNumber ();

        if (false && // JMI v0.9.61 ???
          *multiComponentMostRecentVersionNumber
            <
          *formatComponentMostRecentVersionNumber
        ) {
          std::stringstream ss;

          ss <<
            "Versions numbering inconsistency: multi-component " <<
            fComponentName <<
            ' ' <<
            fComponentKind <<
            " has version number " <<
            multiComponentMostRecentVersionNumber->asString () <<
            " while " <<
            format->getComponentName () <<
            " has version number " <<
            formatComponentMostRecentVersionNumber->asString ();

          oahInternalError (ss.str ());
        }
      }

      break;
    case mfcMultiComponentEntropicityKind::kComponentEntropicityNo:
      break;
  } // switch

  fRepresentationComponentsList.push_back (format);
}

void mfcMultiComponent::appendPassToMultiComponent (
  const S_mfcPassComponent& pass)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Appending pass " <<
      pass <<
      " to multi-component \"" << fComponentName << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    pass != nullptr,
    "pass is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  switch (fComponentEntropicityKind) {
    case mfcMultiComponentEntropicityKind::kComponentEntropicityYes:
      {
        // is the multi-component versions numbering consistent?
        S_mfcVersionNumber
          multiComponentMostRecentVersionNumber =
            fetchComponentMostRecentVersionNumber (),
          passComponentMostRecentVersionNumber =
            pass->
              fetchComponentMostRecentVersionNumber ();

        if (false && // JMI v0.9.61 ???
          *multiComponentMostRecentVersionNumber
            <
          *passComponentMostRecentVersionNumber
        ) {
          std::stringstream ss;

          ss <<
            "Versions numbering inconsistency: multi-component " <<
            fComponentName <<
            ' ' <<
            fComponentKind <<
            " has version number " <<
            multiComponentMostRecentVersionNumber->asString () <<
            " while " <<
            pass->getComponentName () <<
            " has version number " <<
            passComponentMostRecentVersionNumber->asString ();

          oahInternalError (ss.str ());
        }
      }

      break;
    case mfcMultiComponentEntropicityKind::kComponentEntropicityNo:
      break;
  } // switch

  fPassComponentsList.push_back (pass);
}

void mfcMultiComponent::printOahVersion (std::ostream&  os) const
{
  os <<
    "OAH version:" <<
    std::endl;

  ++gIndenter;

  if (fOahComponent) {
    const int fieldWidth = 20;

    os << std::left <<
      std::setw (fieldWidth) <<
      fOahComponent->
        getComponentName () <<
        std::endl;

    ++gIndenter;
    os <<
      fOahComponent->mostRecentVersionNumberAndDateAsString () <<
      std::endl;
    --gIndenter;
  }
  else {
    os << " [NONE]" << std::endl;
  }

  --gIndenter;
}

void mfcMultiComponent::printOahHistory (std::ostream&  os) const
{
  os <<
    "OAH history:" <<
    std::endl;

  ++gIndenter;

  if (fOahComponent) {
    const int fieldWidth = 20;

    os << std::left <<
      std::setw (fieldWidth) <<
      fOahComponent->
        getComponentName () <<
      std::endl;

    ++gIndenter;
    fOahComponent->printHistory (os);
    --gIndenter;
  }
  else {
    os << " [NONE]" << std::endl;
  }

  --gIndenter;
}

void mfcMultiComponent::printRepresentationsVersions (std::ostream&  os) const
{
  os <<
    "Representations versions:" <<
    std::endl;

  ++gIndenter;

  if (fRepresentationComponentsList.size ()) {
    std::list <S_mfcRepresentationComponent>::const_iterator
      iBegin = fRepresentationComponentsList.begin (),
      iEnd   = fRepresentationComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcRepresentationComponent
        representationComponent = (*i);

      const int fieldWidth = 20;

      os << std::left <<
        std::setw (fieldWidth) <<
        representationComponent->
          getComponentName () <<
          std::endl;

      ++gIndenter;
      os <<
        representationComponent->mostRecentVersionNumberAndDateAsString () <<
        std::endl;
      --gIndenter;

      if (++i == iEnd) break;
//      os << std::endl;
    } // for
  }
  else {
    os << " [NONE]" << std::endl;
  }

  --gIndenter;
}

void mfcMultiComponent::printPassesVersions (std::ostream&  os) const
{
  os <<
    "Passes versions:" <<
    std::endl;

  ++gIndenter;

  if (fPassComponentsList.size ()) {
    std::list <S_mfcPassComponent>::const_iterator
      iBegin = fPassComponentsList.begin (),
      iEnd   = fPassComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcPassComponent
        passComponent = (*i);

      const int fieldWidth = 20;

      os << std::left <<
        std::setw (fieldWidth) <<
        passComponent->
          getComponentName () <<
          std::endl;

      ++gIndenter;
      os <<
        passComponent->mostRecentVersionNumberAndDateAsString () <<
        std::endl;
      --gIndenter;

      if (++i == iEnd) break;
//      os << std::endl;
    } // for
  }
  else {
    os << " [NONE]" << std::endl;
  }

  --gIndenter;
}

void mfcMultiComponent::printRepresentationsHistory (std::ostream&  os) const
{
  os <<
    "Representations history:" <<
    std::endl;

  ++gIndenter;

  if (fRepresentationComponentsList.size ()) {
    std::list <S_mfcRepresentationComponent>::const_iterator
      iBegin = fRepresentationComponentsList.begin (),
      iEnd   = fRepresentationComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcRepresentationComponent
        representationComponent = (*i);

      const int fieldWidth = 20;

      os << std::left <<
        std::setw (fieldWidth) <<
        representationComponent->
          getComponentName () <<
        std::endl;

      ++gIndenter;
      representationComponent->printHistory (os);
      --gIndenter;

      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }
  else {
    os << " [NONE]" << std::endl;
  }

  --gIndenter;
}

void mfcMultiComponent::printPassesHistory (std::ostream&  os) const
{
  os <<
    "Passes history:" <<
    std::endl;

  ++gIndenter;

  if (fPassComponentsList.size ()) {
    std::list <S_mfcPassComponent>::const_iterator
      iBegin = fPassComponentsList.begin (),
      iEnd   = fPassComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcPassComponent
        passComponent = (*i);

      const int fieldWidth = 20;

      os << std::left <<
        std::setw (fieldWidth) <<
        passComponent->
          getComponentName () <<
        std::endl;

      ++gIndenter;
      passComponent->printHistory (os);
      --gIndenter;

      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }
  else {
    os << " [NONE]" << std::endl;
  }

  --gIndenter;
}

void mfcMultiComponent::print (std::ostream& os) const
{
  switch (fComponentUsedFromTheCLIKind) {
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes:
      os <<
        "Command line version of " <<
        fComponentName <<
        ' ' <<
        fComponentKind <<
        ' ';
      break;

    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLINo:
      os <<
        fComponentName <<
        ' ' <<
        fComponentKind <<
        " API version ";
      break;
  } // switch

  os <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    std::endl;

  os <<
    "fRepresentationComponentsList:";

  if (fRepresentationComponentsList.size ()) {
    os << std::endl;
    ++gIndenter;

    std::list <S_mfcRepresentationComponent>::const_iterator
      iBegin = fRepresentationComponentsList.begin (),
      iEnd   = fRepresentationComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcRepresentationComponent
        formatComponent = (*i);

      const int fieldWidth = 20;

      os << std::left <<
        std::setw (fieldWidth) <<
        formatComponent->getComponentName () <<
        formatComponent->mostRecentVersionNumberAndDateAsString ();

      if (++i == iEnd) break;
    // JMI  os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << " [NONE]" << std::endl;
  }

  os <<
    "fPassComponentsList:";

  if (fPassComponentsList.size ()) {
    os << std::endl;
    ++gIndenter;

    std::list <S_mfcPassComponent>::const_iterator
      iBegin = fPassComponentsList.begin (),
      iEnd   = fPassComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcPassComponent
        passComponent = (*i);

      const int fieldWidth = 20;

      os << std::left <<
        std::setw (fieldWidth) <<
        passComponent->getComponentName () <<
        passComponent->fetchComponentMostRecentVersionNumber ();

      if (++i == iEnd) break;
    // JMI  os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << " [NONE]" << std::endl;
  }
}

void mfcMultiComponent::printVersionShort (std::ostream& os) const
{
  switch (fComponentUsedFromTheCLIKind) {
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes:
      os <<
        "Command line version of " <<
        fComponentName <<
        ' ' <<
        fComponentKind <<
        ' ';
      break;

    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLINo:
      os <<
        fComponentName <<
        ' ' <<
        fComponentKind <<
      " API version ";
      break;
  } // switch

  os <<
    getGlobalMusicFormatsVersionNumberAndDate  () <<
    std::endl;
}

void mfcMultiComponent::printVersionFull (std::ostream& os) const
{
  switch (fComponentUsedFromTheCLIKind) {
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes:
      os <<
        "Command line version of " <<
        fComponentName <<
        ' ' <<
        fComponentKind <<
        ' ';
      break;

    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLINo:
      os <<
        fComponentName <<
        ' ' <<
        fComponentKind <<
        " API version ";
      break;
  } // switch

  os <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    std::endl;

  os << std::endl;

  printOahVersion (os);

  os << std::endl;

  printRepresentationsVersions (os);

  os << std::endl;

  printPassesVersions (os);
}

void mfcMultiComponent::printHistory (std::ostream&  os) const
{
    switch (fComponentUsedFromTheCLIKind) {
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes:
      os <<
        "Command line version of " <<
        fComponentName <<
        ' ' <<
        fComponentKind <<
        ' ';
      break;

    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLINo:
      os <<
        fComponentName <<
        ' ' <<
        fComponentKind <<
      " API version ";
      break;
  } // switch

  os <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    std::endl;

  printOwnHistory (os);

  os << std::endl;

  printOahHistory (os);

  os << std::endl;

  printRepresentationsHistory (os);

  os << std::endl;

  printPassesHistory (os);
}

std::ostream& operator << (std::ostream& os, const S_mfcMultiComponent& elt)
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
S_mfcGeneratorComponent mfcGeneratorComponent::create (
  const std::string& generatorName,
  mfcMultiComponentEntropicityKind
                     componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                     componentUsedFromTheCLIKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating mfcGeneratorComponent" <<
      ", generatorName: " << generatorName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfcGeneratorComponent* obj =
    new mfcGeneratorComponent (
      generatorName,
      componentEntropicityKind,
      componentUsedFromTheCLIKind);
  assert (obj != nullptr);
  return obj;
}

mfcGeneratorComponent::mfcGeneratorComponent (
  const std::string& generatorName,
  mfcMultiComponentEntropicityKind
                     componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                     componentUsedFromTheCLIKind)
  : mfcMultiComponent (
      generatorName,
      mfcComponentKind::kComponentGenerator,
      componentEntropicityKind,
      componentUsedFromTheCLIKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcGeneratorComponent \"" <<
      generatorName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

mfcGeneratorComponent::~mfcGeneratorComponent ()
{}

std::ostream& operator << (std::ostream& os, const S_mfcGeneratorComponent& elt)
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
S_mfcConverterComponent mfcConverterComponent::create (
  const std::string&   converterName,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating mfcConverterComponent" <<
      ", converterName: " << converterName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfcConverterComponent* obj =
    new mfcConverterComponent (
      converterName,
      componentEntropicityKind,
      componentUsedFromTheCLIKind);
  assert (obj != nullptr);
  return obj;
}

mfcConverterComponent::mfcConverterComponent (
  const std::string&   converterName,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
  : mfcMultiComponent (
      converterName,
      mfcComponentKind::kComponentConverter,
      componentEntropicityKind,
      componentUsedFromTheCLIKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcConverterComponent \"" <<
      converterName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

mfcConverterComponent::~mfcConverterComponent ()
{}

std::ostream& operator << (std::ostream& os, const S_mfcConverterComponent& elt)
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
S_mfcLibraryComponent mfcLibraryComponent::create (
  const std::string& libraryVersionsName,
  mfcMultiComponentEntropicityKind
                     componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                     componentUsedFromTheCLIKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Creating mfcLibraryComponent" <<
      ", libraryVersionsName: " << libraryVersionsName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfcLibraryComponent* obj =
    new mfcLibraryComponent (
      libraryVersionsName,
      componentEntropicityKind,
      componentUsedFromTheCLIKind);
  assert (obj != nullptr);
  return obj;
}

mfcLibraryComponent::mfcLibraryComponent (
  const std::string& libraryVersionsName,
  mfcMultiComponentEntropicityKind
                     componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                     componentUsedFromTheCLIKind)
  : mfcMultiComponent (
      libraryVersionsName,
      mfcComponentKind::kComponentLibrary,
      componentEntropicityKind,
      componentUsedFromTheCLIKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Constructing mfcLibraryComponent \"" <<
      libraryVersionsName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

mfcLibraryComponent::~mfcLibraryComponent ()
{}

void mfcLibraryComponent::appendConverterToMultiComponent (
  const S_mfcConverterComponent& converter)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Appending converter " <<
      converter <<
      " to " <<
      fComponentName <<
      ' ' <<
      fComponentKind <<
      ' ' <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    converter != nullptr,
    "converter is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  switch (fComponentEntropicityKind) {
    case mfcMultiComponentEntropicityKind::kComponentEntropicityYes:
      {
        // is the multi-component versions numbering consistent?
        S_mfcVersionNumber
          multiComponentMostRecentVersionNumber =
            fetchComponentMostRecentVersionNumber (),
          converterComponentMostRecentVersionNumber =
            converter->
              fetchComponentMostRecentVersionNumber ();

        if (false && // JMI v0.9.61 ???
          *multiComponentMostRecentVersionNumber
            <
          *converterComponentMostRecentVersionNumber
        ) {
          std::stringstream ss;

          ss <<
            "Versions numbering inconsistency: multi-component " <<
            fComponentName <<
            ' ' <<
            fComponentKind <<
            " has version number " <<
            multiComponentMostRecentVersionNumber->asString () <<
            " while " <<
            converter->getComponentName () <<
            " has version number " <<
            converterComponentMostRecentVersionNumber->asString ();

          oahInternalError (ss.str ());
        }
      }

      break;
    case mfcMultiComponentEntropicityKind::kComponentEntropicityNo:
      break;
  } // switch

  fConverterComponentsList.push_back (converter);
}

void mfcLibraryComponent::printConvertersVersions (std::ostream&  os) const
{
  os <<
    "Converters versions:" <<
    std::endl;

  ++gIndenter;

  if (fConverterComponentsList.size ()) {
    std::list <S_mfcConverterComponent>::const_iterator
      iBegin = fConverterComponentsList.begin (),
      iEnd   = fConverterComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcConverterComponent
        converterComponent = (*i);

      const int fieldWidth = 20;

      os << std::left <<
        std::setw (fieldWidth) <<
        converterComponent->
          getComponentName () <<
          std::endl;

      ++gIndenter;
      os <<
        converterComponent->mostRecentVersionNumberAndDateAsString () <<
        std::endl;
      --gIndenter;

      if (++i == iEnd) break;
//      os << std::endl;
    } // for
  }
  else {
    os << " [NONE]" << std::endl;
  }

  --gIndenter;
}


void mfcLibraryComponent::printConvertersHistory (std::ostream&  os) const
{
  os <<
    "Converters history:" <<
    std::endl;

  ++gIndenter;

  if (fConverterComponentsList.size ()) {
    std::list <S_mfcConverterComponent>::const_iterator
      iBegin = fConverterComponentsList.begin (),
      iEnd   = fConverterComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcConverterComponent
        converterComponent = (*i);

      const int fieldWidth = 20;

      os << std::left <<
        std::setw (fieldWidth) <<
        converterComponent->
          getComponentName () <<
        std::endl;

      ++gIndenter;
      converterComponent->printOwnHistory (os);
      --gIndenter;

      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }
  else {
    os << " [NONE]" << std::endl;
  }

  --gIndenter;
}

void mfcLibraryComponent::printVersionShort (std::ostream& os) const
{
  os <<
    fComponentName <<
    " library " <<
    getGlobalMusicFormatsVersionNumberAndDate  () <<
    std::endl << std::endl;

  os << std::endl;

  printOahVersion (os);

  os << std::endl;

  printRepresentationsVersions (os);

  os << std::endl;

  printPassesVersions (os);

  os << std::endl;

  printConvertersVersions (os);
}

void mfcLibraryComponent::printVersionFull (std::ostream& os) const
{
  os <<
    fComponentName <<
    " library " <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    std::endl << std::endl;

  os << std::endl;

  printRepresentationsVersions (os);

  os << std::endl;

  printPassesVersions (os);

  os << std::endl;

  printConvertersVersions (os);
}

void mfcLibraryComponent::printHistory (std::ostream&  os) const
{
  os <<
    fComponentName <<
    " library " <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    std::endl << std::endl;

  printOwnHistory (os);

  os << std::endl;

  printRepresentationsHistory (os);

  os << std::endl;

  printPassesHistory (os);

  os << std::endl;

  printConvertersHistory (os);
}

std::ostream& operator << (std::ostream& os, const S_mfcLibraryComponent& elt)
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
