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
#include <iomanip>      // setw, setprecision, ...

#include <regex>
#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfStringsHandling.h"
#include "mfIndentedTextOutput.h"

#include "mfcBasicTypes.h"

#include "oahEarlyOptions.h"

#include "mfcLibraryComponent.h"




using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void crackVersionNumber (
  const string& theString,
  int&          generationNumber,
  int&          majorNumber,
  int&          minorNumber)
{
  // obtains the three numbers in "2.19.83" or "2.20" for example

  // decipher theString with a three-number regular expression
  string regularExpression (
    "([[:digit:]]+)" // generation number
    "."
    "([[:digit:]]+)" // major number
    "."
    "([[:digit:]]+)" // minor number
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for version string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          '[' << sm [i] << "] ";
      } // for

      gLogStream << endl;
    }
#endif

  if (smSize == 4) {
    // found an n.x.y specification
    string
      generationNumberValue = sm [1],
      majorNumberValue      = sm [2],
      minorNumberValue      = sm [3];

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
        "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
        "--> minorNumberValue = \"" << minorNumberValue << "\"" <<
        endl;
    }
#endif

    generationNumber = stoi (generationNumberValue);
    majorNumber      = stoi (majorNumberValue);
    minorNumber      = stoi (minorNumberValue);
  }

  else {
    // decipher theString with a two-number regular expression
    string regularExpression (
      "([[:digit:]]+)" // generation number
      "."
      "([[:digit:]]+)" // major number
      );

    regex  e (regularExpression);
    smatch sm;

    regex_match (theString, sm, e);

    unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "There are " << smSize << " matches" <<
        " for chord details string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl <<
        smSize << " elements: ";

        for (unsigned i = 0; i < smSize; ++i) {
          gLogStream <<
            '[' << sm [i] << "] ";
        } // for

        gLogStream << endl;
      }
#endif

    if (smSize == 3) {
      // found an n.x specification
      // assume implicit 0 minor number
      string
        generationNumberValue = sm [1],
        majorNumberValue      = sm [2];

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
        gLogStream <<
          "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
          "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
          endl;
      }
#endif

      generationNumber = stoi (generationNumberValue);
      majorNumber      = stoi (majorNumberValue);
      minorNumber      = 0;
    }

    else {
      stringstream s;

      s <<
        "version number argument '" << theString <<
        "' is ill-formed";

      oahError (s.str ());
    }
  }
}

//______________________________________________________________________________
Bool versionNumberGreaterThanOrEqualTo (
  const string& versionNumber,
  const string& otherVersionNumber)
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
    gLogStream <<
      "versionNumber: " << versionNumber <<
      ", versionGenerationNumber:" << versionGenerationNumber <<
      ", versionMajorNumber:" << versionMajorNumber <<
      ", versionMinorNumber:" << versionMinorNumber <<
      endl;
  }

  if (doTrace) {
    gLogStream <<
      "otherVersionNumber: " << otherVersionNumber <<
      ", otherVersionNumbeGenerationNumber:" << otherVersionNumbeGenerationNumber <<
      ", otherVersionNumbeMajorNumber:" << otherVersionNumbeMajorNumber <<
      ", otherVersionNumbeMinorNumber:" << otherVersionNumbeMinorNumber <<
      endl;
  }

  if (otherVersionNumbeGenerationNumber != 2) {
    gLogStream <<
      "Using verstion \"" <<
      otherVersionNumbeGenerationNumber << ".x.y\" " <<
      "is probably not such a good idea" <<
      endl;
  }

  if (otherVersionNumbeMajorNumber < 19) {
    gLogStream <<
      "Using a verstion older than \"" <<
      otherVersionNumbeGenerationNumber << ".19.y\" " <<
      "is not a good idea: the generated LilyPond code uses features introduced in the latter" <<
      endl;
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
    gLogStream <<
      "----> versionNumberGreaterThanOrEqualTo =====> " <<
      result <<
      endl;
  }

  return result;
}

//______________________________________________________________________________
/*
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
  int           majorNumber,
  int           minorNumber,
  int           patchNumber,
  const string& preRelease)
{
  mfcVersionNumber* o =
    new mfcVersionNumber (
      majorNumber,
      minorNumber,
      patchNumber,
      preRelease);
  assert (o != nullptr);
  return o;
}

mfcVersionNumber::mfcVersionNumber (
  int           majorNumber,
  int           minorNumber,
  int           patchNumber,
  const string& preRelease)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcVersionNumber" <<
      ", majorNumber:" << majorNumber <<
      ", minorNumber:" << minorNumber <<
      ", patchNumber:" << patchNumber <<
      ", preRelease:" << preRelease <<
      endl;
  }
#endif

  fMajorNumber = majorNumber;
  fMinorNumber = minorNumber;
  fPatchNumber = patchNumber;
  fPreRelease  = preRelease;
}

mfcVersionNumber::~mfcVersionNumber ()
{}

S_mfcVersionNumber mfcVersionNumber::createFromString (
  const string& theString)
{
  // obtains the three numbers in "2.19.83" or "2.20" for example

  S_mfcVersionNumber result;

// ^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$


  // decipher theString with a regular expression
  string regularExpression (
    "^([[:digit:]]+)"                   // major number
    "."
    "([[:digit:]]+)"                    // minor number
    "."
    "([[:digit:]]+)"                    // patch number
    "(-.*)?"                            // prerelease
//    "(-((-|[[:digit:]]|[[:alpha:]])+))?"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for version string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl <<
      smSize << " elements: ";

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        '[' << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  int
    majorNumber = 0,
    minorNumber = 0,
    patchNumber = 0;

  string
    preReleaseValue;

  if (smSize == 5) {
    // found an n.x.y specification
    string
      majorNumberValue      = sm [1],
      minorNumberValue      = sm [2],
      patchNumberValue      = sm [3];

    preReleaseValue         = sm [4];

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
        "--> minorNumberValue = \"" << minorNumberValue << "\"" <<
        "--> patchNumberValue = \"" << patchNumberValue << "\"" <<
        "--> preReleaseValue = \"" << preReleaseValue << "\"" <<
        endl;
    }
#endif

    majorNumber      = stoi (majorNumberValue);
    minorNumber      = stoi (minorNumberValue);
    patchNumber      = stoi (patchNumberValue);
  }

  else {
    stringstream s;

    s <<
      "version number \"" <<
      theString <<
      "\" is ill-formed";

    oahError (s.str ());
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

  const string
    otherPreRelease =
      other.getPreRelease ();

  Bool doTrace (false); // JMI

  if (doTrace) {
    gLogStream <<
      ", fMajorNumber:" << fMajorNumber <<
      ", fMinorNumber:" << fMinorNumber <<
      ", fPatchNumber:" << fPatchNumber <<
      ", fPreRelease:" << fPreRelease <<
      endl <<
      ", otherMajorNumber:" << otherMajorNumber <<
      ", otherMinorNumber:" << otherMinorNumber <<
      ", otherPatchNumber:" << otherPatchNumber <<
      ", otherPreRelease:" << otherPreRelease <<
      endl;
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

  const string
    otherPreRelease =
      other.getPreRelease ();

  Bool doTrace (false); // JMI

  if (doTrace) {
    gLogStream <<
      ", fMajorNumber:" << fMajorNumber <<
      ", fMinorNumber:" << fMinorNumber <<
      ", fPatchNumber:" << fPatchNumber <<
      ", fPreRelease:" << fPreRelease <<
      endl <<
      ", otherMajorNumber:" << otherMajorNumber <<
      ", otherMinorNumber:" << otherMinorNumber <<
      ", otherPatchNumber:" << otherPatchNumber <<
      ", otherPreRelease:" << otherPreRelease <<
      endl;
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
    gLogStream <<
      "----> mfcVersionNumber::operator>= =====> " <<
      result <<
      endl;
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

  const string
    otherPreRelease =
      other.getPreRelease ();

  Bool doTrace (false); // JMI

  if (doTrace) {
    gLogStream <<
      ", fMajorNumber:" << fMajorNumber <<
      ", fMinorNumber:" << fMinorNumber <<
      ", fPatchNumber:" << fPatchNumber <<
      ", fPreRelease:" << fPreRelease <<
      endl <<
      ", otherMajorNumber:" << otherMajorNumber <<
      ", otherMinorNumber:" << otherMinorNumber <<
      ", otherPatchNumber:" << otherPatchNumber <<
      ", otherPreRelease:" << otherPreRelease <<
      endl;
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
    gLogStream <<
      "----> mfcVersionNumber::operator<= =====> " <<
      result <<
      endl;
  }

  return result;
}

string mfcVersionNumber::asString () const
{
  stringstream s;

  s <<
    "v" <<
    fMajorNumber <<
    "." <<
    fMinorNumber <<
    "." <<
    fPatchNumber;

  if (fPreRelease.size ()) {
    s <<
// JMI      '-' <<
      fPreRelease;
   }

  return s.str ();
}

void mfcVersionNumber::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_mfcVersionNumber& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mfcVersionDescr mfcVersionDescr::create (
  S_mfcVersionNumber  versionNumber,
  const string&       versionDate,
  const list<string>& versionDescriptionItems)
{
  mfcVersionDescr* o =
    new mfcVersionDescr (
      versionNumber,
      versionDate,
      versionDescriptionItems);
  assert (o != nullptr);
  return o;
}

mfcVersionDescr::mfcVersionDescr (
  S_mfcVersionNumber  versionNumber,
  const string&       versionDate,
  const list<string>& versionDescriptionItems)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcVersionDescr" <<
      ", versionNumber:" << versionNumber <<
      ", versionDate:" << versionDate <<
      ", versionDescriptionItems:" <<
      endl;

    ++gIndenter;
    for (string const& item : versionDescriptionItems) {
      gLogStream << item << endl;
    } // for
    --gIndenter;
  }
#endif

  fVersionNumber       = versionNumber;
  fVersionDate         = versionDate;

  fVersionDescriptionItems = versionDescriptionItems;
}

mfcVersionDescr::~mfcVersionDescr ()
{}

string mfcVersionDescr::asString () const
{
  stringstream s;

  s <<
    fVersionNumber->asString () <<
    " (" << fVersionDate << ")";

  return s.str ();
}

void mfcVersionDescr::print (ostream& os) const
{
  os <<
    asString () <<
    ":" <<
    endl;

  ++gIndenter;

  for (string const& item : fVersionDescriptionItems) {
    gLogStream << item << endl;
  } // for

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_mfcVersionDescr& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mfcVersionsHistory mfcVersionsHistory::create ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating mfcVersionsHistory" <<
      endl;
  }
#endif

  mfcVersionsHistory* o =
    new mfcVersionsHistory ();
  assert (o != nullptr);
  return o;
}

mfcVersionsHistory::mfcVersionsHistory ()
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcVersionsHistory" <<
      endl;
  }
#endif
}

mfcVersionsHistory::~mfcVersionsHistory ()
{}

void mfcVersionsHistory::appendVersionDescrToHistory (
  S_mfcVersionDescr versionDescr)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Appending version " <<
      versionDescr <<
      " to history" <<
      endl;
  }
#endif

  fVersionsList.push_back (versionDescr);
}

S_mfcVersionDescr mfcVersionsHistory::fetchMostRecentVersion () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVersionsList.size () > 0,
    "fVersionsList is empty");

  return fVersionsList.back ();
}

void mfcVersionsHistory::print (ostream& os) const
{
  list<S_mfcVersionDescr>::const_iterator
    iBegin = fVersionsList.begin (),
    iEnd   = fVersionsList.end (),
    i      = iBegin;
  for ( ; ; ) {
    S_mfcVersionDescr
      versionDescr = (*i);

    versionDescr->print (os);
    if (++i == iEnd) break;
    os << endl;
  } // for
}

ostream& operator<< (ostream& os, const S_mfcVersionsHistory& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
string componenKindAsString (
  mfcComponenKind componenKind)
{
  string result;

  switch (componenKind) {
    case mfcComponenKind::kComponentRepresentation:
      result = "format";
      break;
    case mfcComponenKind::kComponentPass:
      result = "pass";
      break;
    case mfcComponenKind::kComponentGenerator:
      result = "generator";
      break;
    case mfcComponenKind::kComponentConverter:
      result = "converter";
      break;
    case mfcComponenKind::kComponentLibrary:
      result = "library";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
/* this class is purely virtual
S_mfcComponentDescr mfcComponentDescr::create (
  const string&   componentName,
  mfcComponenKind componenKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating mfcComponentDescr" <<
      ", componentName: " << componentName <<
      endl;
  }
#endif

  mfcComponentDescr* o =
    new mfcComponentDescr (
      componentName,
      componenKind);
  assert (o != nullptr);
  return o;
}
*/

mfcComponentDescr::mfcComponentDescr (
  const string&   componentName,
  mfcComponenKind componenKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcComponentDescr \"" <<
      componentName <<
      "\"" <<
      endl;
  }
#endif

  fComponentName = componentName;
  fComponenKind  = componenKind;

  // create the versions history
  fVersionsHistory = mfcVersionsHistory::create ();
}

mfcComponentDescr::~mfcComponentDescr ()
{}

void mfcComponentDescr::appendVersionDescrToComponent (
  S_mfcVersionDescr versionDescr)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Appending version " <<
      versionDescr <<
      " to " <<
      componenKindAsString (fComponenKind) <<
      ' ' <<
      fComponentName <<
      endl;
  }
#endif

  fVersionsHistory->
    appendVersionDescrToHistory (versionDescr);
}

string mfcComponentDescr::asString () const
{
  stringstream s;

  S_mfcVersionDescr
    componentMostRecentVersion =
      fetchComponentMostRecentVersion ();

  s <<
    fComponentName <<
    ' ' <<
    componenKindAsString (fComponenKind) <<
    ' ' <<
    componentMostRecentVersion->
      getVersionNumber ()->
        asString () <<
    " (" <<
    componentMostRecentVersion->
      getVersionDate () <<
    ")" <<
    endl;

  return s.str ();
}

string mfcComponentDescr::mostRecentVersionNumberAndDateAsString () const
{
  stringstream s;

  S_mfcVersionDescr
    componentMostRecentVersion =
      fetchComponentMostRecentVersion ();

  s <<
    componentMostRecentVersion->
      getVersionNumber ()->
        asString () <<
    " (" <<
    componentMostRecentVersion->
      getVersionDate () <<
    ")";

  return s.str ();
}

void mfcComponentDescr::print (ostream& os) const
{

  os << asString () << endl;
}

void mfcComponentDescr::printOwnHistory (ostream&  os) const
{
  os <<
    "Own history:" <<
    endl;

  ++gIndenter;
  fVersionsHistory->print (os);
  --gIndenter;
}

void mfcComponentDescr::printVersion (ostream& os) const
{
  S_mfcVersionDescr
    componentMostRecentVersion =
      fetchComponentMostRecentVersion ();

  os <<
    fComponentName <<
    ' ' <<
    componenKindAsString (fComponenKind) <<
    ' ' <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    endl;
}

void mfcComponentDescr::printHistory (ostream&  os) const
{
  fVersionsHistory->print (os);
}

ostream& operator<< (ostream& os, const S_mfcComponentDescr& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mfcRepresentationComponent mfcRepresentationComponent::create (
  const string& formatName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating mfcRepresentationComponent" <<
      ", formatName: " << formatName <<
      endl;
  }
#endif

  mfcRepresentationComponent* o =
    new mfcRepresentationComponent (
      formatName);
  assert (o != nullptr);
  return o;
}

mfcRepresentationComponent::mfcRepresentationComponent (
  const string& formatName)
  : mfcComponentDescr (
      formatName,
      mfcComponenKind::kComponentRepresentation)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcRepresentationComponent \"" <<
      formatName <<
      "\"" <<
      endl;
  }
#endif
}

mfcRepresentationComponent::~mfcRepresentationComponent ()
{}

ostream& operator<< (ostream& os, const S_mfcRepresentationComponent& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mfcPassComponent mfcPassComponent::create (
  const string& passName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating mfcPassComponent" <<
      ", passName: " << passName <<
      endl;
  }
#endif

  mfcPassComponent* o =
    new mfcPassComponent (
      passName);
  assert (o != nullptr);
  return o;
}

mfcPassComponent::mfcPassComponent (
  const string& passName)
  : mfcComponentDescr (
      passName,
      mfcComponenKind::kComponentPass)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcPassComponent \"" <<
      passName <<
      "\"" <<
      endl;
  }
#endif
}

mfcPassComponent::~mfcPassComponent ()
{}

ostream& operator<< (ostream& os, const S_mfcPassComponent& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
string componentUsedFromTheCLIKindAsString (
  mfcMultiComponentUsedFromTheCLIKind componentUsedFromTheCLIKind)
{
  string result;

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

string componentEntropicityKindAsString (
  mfcMultiComponentEntropicityKind componentEntropicityKind)
{
  string result;

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
  const string&   multiComponentName,
  mfcComponenKind componenKind,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating mfcMultiComponent" <<
      ", multiComponentName: " << multiComponentName <<
      endl;
  }
#endif

  mfcMultiComponent* o =
    new mfcMultiComponent (
      multiComponentName,
      componenKind,
      componentEntropicityKind,
      componentUsedFromTheCLIKind);
  assert (o != nullptr);
  return o;
}
*/

mfcMultiComponent::mfcMultiComponent (
  const string&   multiComponentName,
  mfcComponenKind componenKind,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
  : mfcComponentDescr (
      multiComponentName,
      componenKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcMultiComponent \"" <<
      multiComponentName <<
      "\"" <<
      endl;
  }
#endif

  fComponentEntropicityKind = componentEntropicityKind;

  fComponentUsedFromTheCLIKind = componentUsedFromTheCLIKind;
}

mfcMultiComponent::~mfcMultiComponent ()
{}

void mfcMultiComponent::appendRepresentationToMultiComponent (
  S_mfcRepresentationComponent format)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Appending format " <<
      format <<
      " to " <<
      fComponentName <<
      ' ' <<
      componenKindAsString (fComponenKind) <<
      ' ' <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    format != nullptr,
    "format is null");

  switch (fComponentEntropicityKind) {
    case mfcMultiComponentEntropicityKind::kComponentEntropicityYes:
      {
        // is the multi component versions numbering consistent?
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
          stringstream s;

          s <<
            "Versions numbering inconsistency: multi component " <<
            fComponentName <<
            ' ' <<
            componenKindAsString (fComponenKind) <<
            " has version number " <<
            multiComponentMostRecentVersionNumber->asString () <<
            " while " <<
            format->getComponentName () <<
            " has version number " <<
            formatComponentMostRecentVersionNumber->asString ();

          oahInternalError (s.str ());
        }
      }

      break;
    case mfcMultiComponentEntropicityKind::kComponentEntropicityNo:
      break;
  } // switch

  fRepresentationComponentsList.push_back (format);
}

void mfcMultiComponent::appendPassToMultiComponent (
  S_mfcPassComponent pass)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Appending pass " <<
      pass <<
      " to multi component \"" << fComponentName << "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    pass != nullptr,
    "pass is null");

  switch (fComponentEntropicityKind) {
    case mfcMultiComponentEntropicityKind::kComponentEntropicityYes:
      {
        // is the multi component versions numbering consistent?
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
          stringstream s;

          s <<
            "Versions numbering inconsistency: multi component " <<
            fComponentName <<
            ' ' <<
            componenKindAsString (fComponenKind) <<
            " has version number " <<
            multiComponentMostRecentVersionNumber->asString () <<
            " while " <<
            pass->getComponentName () <<
            " has version number " <<
            passComponentMostRecentVersionNumber->asString ();

          oahInternalError (s.str ());
        }
      }

      break;
    case mfcMultiComponentEntropicityKind::kComponentEntropicityNo:
      break;
  } // switch

  fPassComponentsList.push_back (pass);
}

void mfcMultiComponent::printRepresentationsVersions (ostream&  os) const
{
  os <<
    "Representations versions:" <<
    endl;

  ++gIndenter;

  if (fRepresentationComponentsList.size ()) {
    list<S_mfcRepresentationComponent>::const_iterator
      iBegin = fRepresentationComponentsList.begin (),
      iEnd   = fRepresentationComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcRepresentationComponent
        formatComponent = (*i);

      const int fieldWidth = 20;

      os << left <<
        setw (fieldWidth) <<
        formatComponent->
          getComponentName () <<
          endl;

      ++gIndenter;
      os <<
        formatComponent->mostRecentVersionNumberAndDateAsString () <<
        endl;
      --gIndenter;

      if (++i == iEnd) break;
//      os << endl;
    } // for
  }
  else {
    os << " none" << endl;
  }

  --gIndenter;
}

void mfcMultiComponent::printPassesVersions (ostream&  os) const
{
  os <<
    "Passes versions:" <<
    endl;

  ++gIndenter;

  if (fPassComponentsList.size ()) {
    list<S_mfcPassComponent>::const_iterator
      iBegin = fPassComponentsList.begin (),
      iEnd   = fPassComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcPassComponent
        passComponent = (*i);

      const int fieldWidth = 20;

      os << left <<
        setw (fieldWidth) <<
        passComponent->
          getComponentName () <<
          endl;

      ++gIndenter;
      os <<
        passComponent->mostRecentVersionNumberAndDateAsString () <<
        endl;
      --gIndenter;

      if (++i == iEnd) break;
//      os << endl;
    } // for
  }
  else {
    os << " none" << endl;
  }

  --gIndenter;
}

void mfcMultiComponent::printRepresentationsHistory (ostream&  os) const
{
  os <<
    "Representations history:" <<
    endl;

  ++gIndenter;

  if (fRepresentationComponentsList.size ()) {
    list<S_mfcRepresentationComponent>::const_iterator
      iBegin = fRepresentationComponentsList.begin (),
      iEnd   = fRepresentationComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcRepresentationComponent
        formatComponent = (*i);

      const int fieldWidth = 20;

      os << left <<
        setw (fieldWidth) <<
        formatComponent->
          getComponentName () <<
        endl;

      ++gIndenter;
      formatComponent->printHistory (os);
      --gIndenter;

      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  else {
    os << " none" << endl;
  }

  --gIndenter;
}

void mfcMultiComponent::printPassesHistory (ostream&  os) const
{
  os <<
    "Passes history:" <<
    endl;

  ++gIndenter;

  if (fPassComponentsList.size ()) {
    list<S_mfcPassComponent>::const_iterator
      iBegin = fPassComponentsList.begin (),
      iEnd   = fPassComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcPassComponent
        passComponent = (*i);

      const int fieldWidth = 20;

      os << left <<
        setw (fieldWidth) <<
        passComponent->
          getComponentName () <<
        endl;

      ++gIndenter;
      passComponent->printHistory (os);
      --gIndenter;

      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  else {
    os << " none" << endl;
  }

  --gIndenter;
}

void mfcMultiComponent::print (ostream& os) const
{
  switch (fComponentUsedFromTheCLIKind) {
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes:
      os <<
        "Command line version of " <<
        fComponentName <<
        ' ' <<
        componenKindAsString (fComponenKind) <<
        ' ';
      break;

    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLINo:
      os <<
        fComponentName <<
        ' ' <<
        componenKindAsString (fComponenKind) <<
        " API version ";
      break;
  } // switch

  os <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    endl;

  os <<
    "fRepresentationComponentsList:";

  if (fRepresentationComponentsList.size ()) {
    os << endl;
    ++gIndenter;

    list<S_mfcRepresentationComponent>::const_iterator
      iBegin = fRepresentationComponentsList.begin (),
      iEnd   = fRepresentationComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcRepresentationComponent
        formatComponent = (*i);

      const int fieldWidth = 20;

      os << left <<
        setw (fieldWidth) <<
        formatComponent->getComponentName () <<
        formatComponent->mostRecentVersionNumberAndDateAsString ();

      if (++i == iEnd) break;
    // JMI  os << endl;
    } // for

    --gIndenter;
  }
  else {
    os << " none" << endl;
  }

  os <<
    "fPassComponentsList:";

  if (fPassComponentsList.size ()) {
    os << endl;
    ++gIndenter;

    list<S_mfcPassComponent>::const_iterator
      iBegin = fPassComponentsList.begin (),
      iEnd   = fPassComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcPassComponent
        passComponent = (*i);

      const int fieldWidth = 20;

      os << left <<
        setw (fieldWidth) <<
        passComponent->getComponentName () <<
        passComponent->fetchComponentMostRecentVersionNumber ();

      if (++i == iEnd) break;
    // JMI  os << endl;
    } // for

    --gIndenter;
  }
  else {
    os << " none" << endl;
  }
}

void mfcMultiComponent::printVersionShort (ostream& os) const
{
  switch (fComponentUsedFromTheCLIKind) {
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes:
      os <<
        "Command line version of " <<
        fComponentName <<
        ' ' <<
        componenKindAsString (fComponenKind) <<
        ' ';
      break;

    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLINo:
      os <<
        fComponentName <<
        ' ' <<
        componenKindAsString (fComponenKind) <<
      " API version ";
      break;
  } // switch

  os <<
    getGlobalMusicFormatsVersionNumberAndDate  () <<
    endl;
}

void mfcMultiComponent::printVersionFull (ostream& os) const
{
  switch (fComponentUsedFromTheCLIKind) {
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes:
      os <<
        "Command line version of " <<
        fComponentName <<
        ' ' <<
        componenKindAsString (fComponenKind) <<
        ' ';
      break;

    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLINo:
      os <<
        fComponentName <<
        ' ' <<
        componenKindAsString (fComponenKind) <<
        " API version ";
      break;
  } // switch

  os <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    endl;

  os << endl;

  printRepresentationsVersions (os);

  os << endl;

  printPassesVersions (os);
}

void mfcMultiComponent::printHistory (ostream&  os) const
{
    switch (fComponentUsedFromTheCLIKind) {
    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLIYes:
      os <<
        "Command line version of " <<
        fComponentName <<
        ' ' <<
        componenKindAsString (fComponenKind) <<
        ' ';
      break;

    case mfcMultiComponentUsedFromTheCLIKind::kComponentUsedFromTheCLINo:
      os <<
        fComponentName <<
        ' ' <<
        componenKindAsString (fComponenKind) <<
      " API version ";
      break;
  } // switch

  os <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    endl;

  printOwnHistory (os);

  os << endl;

  printRepresentationsHistory (os);

  os << endl;

  printPassesHistory (os);
}

ostream& operator<< (ostream& os, const S_mfcMultiComponent& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mfcGeneratorComponent mfcGeneratorComponent::create (
  const string&   generatorName,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating mfcGeneratorComponent" <<
      ", generatorName: " << generatorName <<
      endl;
  }
#endif

  mfcGeneratorComponent* o =
    new mfcGeneratorComponent (
      generatorName,
      componentEntropicityKind,
      componentUsedFromTheCLIKind);
  assert (o != nullptr);
  return o;
}

mfcGeneratorComponent::mfcGeneratorComponent (
  const string&   generatorName,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
  : mfcMultiComponent (
      generatorName,
      mfcComponenKind::kComponentGenerator,
      componentEntropicityKind,
      componentUsedFromTheCLIKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcGeneratorComponent \"" <<
      generatorName <<
      "\"" <<
      endl;
  }
#endif
}

mfcGeneratorComponent::~mfcGeneratorComponent ()
{}

ostream& operator<< (ostream& os, const S_mfcGeneratorComponent& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mfcConverterComponent mfcConverterComponent::create (
  const string&   converterName,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating mfcConverterComponent" <<
      ", converterName: " << converterName <<
      endl;
  }
#endif

  mfcConverterComponent* o =
    new mfcConverterComponent (
      converterName,
      componentEntropicityKind,
      componentUsedFromTheCLIKind);
  assert (o != nullptr);
  return o;
}

mfcConverterComponent::mfcConverterComponent (
  const string&   converterName,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
  : mfcMultiComponent (
      converterName,
      mfcComponenKind::kComponentConverter,
      componentEntropicityKind,
      componentUsedFromTheCLIKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcConverterComponent \"" <<
      converterName <<
      "\"" <<
      endl;
  }
#endif
}

mfcConverterComponent::~mfcConverterComponent ()
{}

ostream& operator<< (ostream& os, const S_mfcConverterComponent& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mfcLibraryComponent mfcLibraryComponent::create (
  const string&   libraryVersionsName,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Creating mfcLibraryComponent" <<
      ", libraryVersionsName: " << libraryVersionsName <<
      endl;
  }
#endif

  mfcLibraryComponent* o =
    new mfcLibraryComponent (
      libraryVersionsName,
      componentEntropicityKind,
      componentUsedFromTheCLIKind);
  assert (o != nullptr);
  return o;
}

mfcLibraryComponent::mfcLibraryComponent (
  const string&   libraryVersionsName,
  mfcMultiComponentEntropicityKind
                  componentEntropicityKind,
  mfcMultiComponentUsedFromTheCLIKind
                  componentUsedFromTheCLIKind)
  : mfcMultiComponent (
      libraryVersionsName,
      mfcComponenKind::kComponentLibrary,
      componentEntropicityKind,
      componentUsedFromTheCLIKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Constructing mfcLibraryComponent \"" <<
      libraryVersionsName <<
      "\"" <<
      endl;
  }
#endif
}

mfcLibraryComponent::~mfcLibraryComponent ()
{}

void mfcLibraryComponent::appendConverterToMultiComponent (
  S_mfcConverterComponent converter)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Appending converter " <<
      converter <<
      " to " <<
      fComponentName <<
      ' ' <<
      componenKindAsString (fComponenKind) <<
      ' ' <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    converter != nullptr,
    "converter is null");

  switch (fComponentEntropicityKind) {
    case mfcMultiComponentEntropicityKind::kComponentEntropicityYes:
      {
        // is the multi component versions numbering consistent?
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
          stringstream s;

          s <<
            "Versions numbering inconsistency: multi component " <<
            fComponentName <<
            ' ' <<
            componenKindAsString (fComponenKind) <<
            " has version number " <<
            multiComponentMostRecentVersionNumber->asString () <<
            " while " <<
            converter->getComponentName () <<
            " has version number " <<
            converterComponentMostRecentVersionNumber->asString ();

          oahInternalError (s.str ());
        }
      }

      break;
    case mfcMultiComponentEntropicityKind::kComponentEntropicityNo:
      break;
  } // switch

  fConverterComponentsList.push_back (converter);
}

void mfcLibraryComponent::printConvertersVersions (ostream&  os) const
{
  os <<
    "Converters versions:" <<
    endl;

  ++gIndenter;

  if (fConverterComponentsList.size ()) {
    list<S_mfcConverterComponent>::const_iterator
      iBegin = fConverterComponentsList.begin (),
      iEnd   = fConverterComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcConverterComponent
        converterComponent = (*i);

      const int fieldWidth = 20;

      os << left <<
        setw (fieldWidth) <<
        converterComponent->
          getComponentName () <<
          endl;

      ++gIndenter;
      os <<
        converterComponent->mostRecentVersionNumberAndDateAsString () <<
        endl;
      --gIndenter;

      if (++i == iEnd) break;
//      os << endl;
    } // for
  }
  else {
    os << " none" << endl;
  }

  --gIndenter;
}


void mfcLibraryComponent::printConvertersHistory (ostream&  os) const
{
  os <<
    "Converters history:" <<
    endl;

  ++gIndenter;

  if (fConverterComponentsList.size ()) {
    list<S_mfcConverterComponent>::const_iterator
      iBegin = fConverterComponentsList.begin (),
      iEnd   = fConverterComponentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_mfcConverterComponent
        converterComponent = (*i);

      const int fieldWidth = 20;

      os << left <<
        setw (fieldWidth) <<
        converterComponent->
          getComponentName () <<
        endl;

      ++gIndenter;
      converterComponent->printOwnHistory (os);
      --gIndenter;

      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  else {
    os << " none" << endl;
  }

  --gIndenter;
}

void mfcLibraryComponent::printVersionShort (ostream& os) const
{
  os <<
    fComponentName <<
    " library " <<
    getGlobalMusicFormatsVersionNumberAndDate  () <<
    endl << endl;

  os << endl;

  printRepresentationsVersions (os);

  os << endl;

  printPassesVersions (os);

  os << endl;

  printConvertersVersions (os);
}

void mfcLibraryComponent::printVersionFull (ostream& os) const
{
  os <<
    fComponentName <<
    " library " <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    endl << endl;

  os << endl;

  printRepresentationsVersions (os);

  os << endl;

  printPassesVersions (os);

  os << endl;

  printConvertersVersions (os);
}

void mfcLibraryComponent::printHistory (ostream&  os) const
{
  os <<
    fComponentName <<
    " library " <<
    getGlobalMusicFormatsVersionNumberAndDate () <<
    endl << endl;

  printOwnHistory (os);

  os << endl;

  printRepresentationsHistory (os);

  os << endl;

  printPassesHistory (os);

  os << endl;

  printConvertersHistory (os);
}

ostream& operator<< (ostream& os, const S_mfcLibraryComponent& elt)
{
  elt->print (os);
  return os;
}


}
