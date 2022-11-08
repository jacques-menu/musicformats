/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrOctaves.h"


namespace MusicFormats
{

// semitone pitches and absolute octave
//______________________________________________________________________________
S_msrSemiTonesPitchAndOctave msrSemiTonesPitchAndOctave::create (
  msrSemiTonesPitchKind semiTonesPitchKind,
  msrOctaveKind         octaveKind)
{
  msrSemiTonesPitchAndOctave* o =
    new msrSemiTonesPitchAndOctave (
      semiTonesPitchKind,
      octaveKind);
  assert (o != nullptr);

  return o;
}

msrSemiTonesPitchAndOctave::msrSemiTonesPitchAndOctave (
  msrSemiTonesPitchKind semiTonesPitchKind,
  msrOctaveKind         octaveKind)
{
  fSemiTonesPitchKind = semiTonesPitchKind;
  fOctaveKind         = octaveKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating pitch and octave '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrSemiTonesPitchAndOctave::~msrSemiTonesPitchAndOctave ()
{}

S_msrSemiTonesPitchAndOctave msrSemiTonesPitchAndOctave::createSemiTonesPitchAndOctaveNewbornClone ()
{
  S_msrSemiTonesPitchAndOctave
    newbornClone =
      msrSemiTonesPitchAndOctave::create (
        fSemiTonesPitchKind,
        fOctaveKind);

  return newbornClone;
}

S_msrSemiTonesPitchAndOctave msrSemiTonesPitchAndOctave::createFromString (
  int           inputLineNumber,
  const string& theString)
{
  S_msrSemiTonesPitchAndOctave result;

  // decipher theString
  string regularExpression (
    "([[:lower:]]+)" // pitch
    "([,\']*)"       // octaveIndication
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for semitones pitch and octave string '" << theString <<
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
    // found a well-formed specification,
    // need to check its ',' and '\'' contents
    string
      pitch            = sm [1],
      octaveIndication = sm [2];

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "--> pitch = \"" << pitch << "\", " <<
        "--> octaveIndication = \"" << octaveIndication << "\"" <<
        endl;
    }
#endif

    // fetch semitones pitch
    msrSemiTonesPitchKind
      semiTonesPitchKind =
        semiTonesPitchKindFromString (
          pitch);

    // compute the octave from octaveIndication
    msrOctaveKind
      octaveKind =
        msrOctaveKindFromCommasOrQuotes (
          inputLineNumber,
          octaveIndication);

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "--> semiTonesPitchKind = \"" <<
        msrSemiTonesPitchKindAsString (
          semiTonesPitchKind) << "\", " <<
        "--> octaveKind = " <<
        msrOctaveKindAsString (octaveKind) <<
        endl;
    }
#endif

    // create the semiTonesPitchAndOctave
    result =
      msrSemiTonesPitchAndOctave::create (
       semiTonesPitchKind,
       octaveKind);
  }

  else {
    stringstream s;

    s <<
      "semitones pitch and octave argument '" << theString <<
      "' is ill-formed";

    msrError (
//    msrWarning ( //  JMI
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void msrSemiTonesPitchAndOctave::incrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::kOctave_NO_:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave1:
      fOctaveKind = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave2:
      fOctaveKind = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave3:
      fOctaveKind = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave4:
      fOctaveKind = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave5:
      fOctaveKind = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave6:
      fOctaveKind = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave7:
      fOctaveKind = msrOctaveKind::kOctave8;
      break;
    case msrOctaveKind::kOctave8:
      fOctaveKind = msrOctaveKind::kOctave9;
      break;
    case msrOctaveKind::kOctave9:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
  } // switch
}

void msrSemiTonesPitchAndOctave::decrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::kOctave_NO_:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
    case msrOctaveKind::kOctave1:
      fOctaveKind = msrOctaveKind::kOctave0;
      break;
    case msrOctaveKind::kOctave2:
      fOctaveKind = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave3:
      fOctaveKind = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave4:
      fOctaveKind = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave5:
      fOctaveKind = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave6:
      fOctaveKind = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave7:
      fOctaveKind = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave8:
      fOctaveKind = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave9:
      fOctaveKind = msrOctaveKind::kOctave8;
      break;
  } // switch
}

string msrSemiTonesPitchAndOctave::asString () const
{
  stringstream s;

  s << left <<
    "[SemiTonesPitchAndOctave" <<
    ": " <<
    "semiTonesPitchKind: " <<
    msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    ", octave: " <<
    msrOctaveKindAsString (fOctaveKind) <<
    ']';

  return s.str ();
}

void msrSemiTonesPitchAndOctave::print (ostream& os) const
{
  os <<
    "SemiTonesPitchAndOctave" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "fSemiTonesPitchKind" << " : " <<
      msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "fOctaveKind: " << " : " <<
    msrOctaveKindAsString (fOctaveKind) <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrSemiTonesPitchAndOctave& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

// quartertone pitches and absolute octave
//______________________________________________________________________________
S_msrQuarterTonesPitchAndOctave msrQuarterTonesPitchAndOctave::create (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrOctaveKind            octaveKind)
{
  msrQuarterTonesPitchAndOctave* o =
    new msrQuarterTonesPitchAndOctave (
      quarterTonesPitchKind,
      octaveKind);
  assert (o != nullptr);

  return o;
}

msrQuarterTonesPitchAndOctave::msrQuarterTonesPitchAndOctave (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrOctaveKind            octaveKind)
{
  fQuarterTonesPitchKind = quarterTonesPitchKind;
  fOctaveKind            = octaveKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating pitch and octave '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrQuarterTonesPitchAndOctave::~msrQuarterTonesPitchAndOctave ()
{}

S_msrQuarterTonesPitchAndOctave msrQuarterTonesPitchAndOctave::createQuarterTonesPitchAndOctaveNewbornClone ()
{
  S_msrQuarterTonesPitchAndOctave
    newbornClone =
      msrQuarterTonesPitchAndOctave::create (
        fQuarterTonesPitchKind,
        fOctaveKind);

  return newbornClone;
}

S_msrQuarterTonesPitchAndOctave msrQuarterTonesPitchAndOctave::createFromString (
  int           inputLineNumber,
  const string& theString)
{
  S_msrQuarterTonesPitchAndOctave result;

  // decipher theString
  string regularExpression (
    "([[:lower:]]+)" // pitch
    "([,\']*)"       // octaveIndication
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for quartertones pitch and octave string '" << theString <<
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
    // found a well-formed specification,
    // need to check its ',' and '\'' contents
    string
      pitch            = sm [1],
      octaveIndication = sm [2];

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "--> pitch = \"" << pitch << "\", " <<
        "--> octaveIndication = \"" << octaveIndication << "\"" <<
        endl;
    }
#endif

    // fetch quartertones pitch
    msrQuarterTonesPitchKind
      quarterTonesPitchKind =
        quarterTonesPitchKindFromString (
          gGlobalMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind (),
          pitch);

    // compute the octave from octaveIndication
    msrOctaveKind
      octaveKind =
        msrOctaveKindFromCommasOrQuotes (
          inputLineNumber,
          octaveIndication);

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "--> quarterTonesPitchKind = \"" <<
          msrQuarterTonesPitchKindAsString (
            quarterTonesPitchKind) <<
        "\", " <<
        "--> octaveKind = " << msrOctaveKindAsString (octaveKind) <<
        endl;
    }
#endif

    // create the quarterTonesPitchAndOctave
    result =
      msrQuarterTonesPitchAndOctave::create (
       quarterTonesPitchKind,
       octaveKind);
  }

  else {
    stringstream s;

    s <<
      "quartertones pitch and octave argument '" << theString <<
      "' is ill-formed";

    msrError (
//    msrWarning ( //  JMI
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void msrQuarterTonesPitchAndOctave::incrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::kOctave_NO_:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave1:
      fOctaveKind = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave2:
      fOctaveKind = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave3:
      fOctaveKind = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave4:
      fOctaveKind = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave5:
      fOctaveKind = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave6:
      fOctaveKind = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave7:
      fOctaveKind = msrOctaveKind::kOctave8;
      break;
    case msrOctaveKind::kOctave8:
      fOctaveKind = msrOctaveKind::kOctave9;
      break;
    case msrOctaveKind::kOctave9:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
  } // switch
}

void msrQuarterTonesPitchAndOctave::decrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::kOctave_NO_:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
    case msrOctaveKind::kOctave1:
      fOctaveKind = msrOctaveKind::kOctave0;
      break;
    case msrOctaveKind::kOctave2:
      fOctaveKind = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave3:
      fOctaveKind = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave4:
      fOctaveKind = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave5:
      fOctaveKind = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave6:
      fOctaveKind = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave7:
      fOctaveKind = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave8:
      fOctaveKind = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave9:
      fOctaveKind = msrOctaveKind::kOctave8;
      break;
  } // switch
}

string msrQuarterTonesPitchAndOctave::asString () const
{
  stringstream s;

  s << left <<
    "[QuarterTonesPitchAndOctave" <<
    ": " <<
    "fQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsString (
      fQuarterTonesPitchKind) <<
    ", fOctaveKind: " << msrOctaveKindAsString (fOctaveKind) <<
    ']';

  return s.str ();
}

void msrQuarterTonesPitchAndOctave::print (ostream& os) const
{
  os <<
    "QuarterTonesPitchAndOctave" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "quarterTonesPitchKind" << " : " <<
      msrQuarterTonesPitchKindAsString (fQuarterTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "octave: " << " : " <<
    msrOctaveKindAsString (fOctaveKind) <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrQuarterTonesPitchAndOctave& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}

