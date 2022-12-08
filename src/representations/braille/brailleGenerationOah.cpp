/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "oahOah.h"
#include "outputFileOah.h"

#include "brailleGenerationOah.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
// constants
const std::string K_BRAILLE_USE_ENCODING_IN_FILE_NAME_LONG_NAME  = "use-encoding-in-file-name";
const std::string K_BRAILLE_USE_ENCODING_IN_FILE_NAME_SHORT_NAME = "ueifn";

// const std::string K_BRAILLE_OUTPUT_KIND_LONG_NAME  = "use-encoding-in-file-name";
// const std::string K_BRAILLE_OUTPUT_KIND_SHORT_NAME = "ueifn";
//
// const std::string K_BRAILLE_UTF_KIND_LONG_NAME  = "use-encoding-in-file-name";
// const std::string K_BRAILLE_UTF_KIND_SHORT_NAME = "ueifn";

const std::string K_BRAILLE_BYTE_ORDERING_KIND_LONG_NAME  = "byte-ordering-kind";
const std::string K_BRAILLE_BYTE_ORDERING_KIND_SHORT_NAME = "bok";

//______________________________________________________________________________
S_brailleOutputKindAtom brailleOutputKindAtom::create (
  const std::string&    longName,
  const std::string&    shortName,
  const std::string&    description,
  const std::string&    variableName,
  bsrBrailleOutputKind& brailleOutputKindVariable,
  bsrBrailleOutputKind  brailleOutputKindValue)
{
  brailleOutputKindAtom* o = new
    brailleOutputKindAtom (
      longName,
      shortName,
      description,
      variableName,
      brailleOutputKindVariable,
      brailleOutputKindValue);
  assert (o != nullptr);
  return o;
}

brailleOutputKindAtom::brailleOutputKindAtom (
  const std::string&    longName,
  const std::string&    shortName,
  const std::string&    description,
  const std::string&    variableName,
  bsrBrailleOutputKind& brailleOutputKindVariable,
  bsrBrailleOutputKind  brailleOutputKindValue)
  : oahAtomImplicitlyStoringAValue (
      longName,
      shortName,
      description,
      variableName),
    fBrailleOutputKindVariable (
      brailleOutputKindVariable),
    fBrailleOutputKindValue ( // this is where the value is supplied
      brailleOutputKindValue)
{
  fBrailleOutputKindVariable =
    bsrBrailleOutputKind::kBrailleOutputUTF8Debug; // default value
}

brailleOutputKindAtom::~brailleOutputKindAtom ()
{}

void brailleOutputKindAtom::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> handling atom '" <<
      fetchNames () <<
      "; which is of type 'brailleOutputKindAtom'" <<
      std::endl;
  }
#endif

  if (fSetByAnOption) {
    std::stringstream s;

    s <<
      "brailleOutputKindAtom generated output kind" << // JMI ???
      " set more than once" <<
      std::endl;

    oahError (s.str ());
  }
  else {
    fBrailleOutputKindVariable = fBrailleOutputKindValue;
    fSetByAnOption = true;
  }
}

void brailleOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOutputKindAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_brailleOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleOutputKindAtom>*> (v)) {
        S_brailleOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleOutputKindAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOutputKindAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_brailleOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleOutputKindAtom>*> (v)) {
        S_brailleOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleOutputKindAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOutputKindAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string brailleOutputKindAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    bsrBrailleOutputKindAsString (fBrailleOutputKindVariable);

  return s.str ();
}

std::string brailleOutputKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    bsrBrailleOutputKindAsString (fBrailleOutputKindVariable);

  return s.str ();
}

void brailleOutputKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "brailleOutputKindAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fBrailleOutputKindVariable" << ": \"" <<
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindVariable) <<
      "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fBrailleOutputKindValue" << ": \"" <<
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindValue) <<
      "\"" <<
    std::endl;

  --gIndenter;
}

void brailleOutputKindAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindVariable);
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_brailleOutputKindAtom& elt)
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
S_brailleUTFKindAtom brailleUTFKindAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  bsrUTFKind&   brailleUTFKindVariable)
{
  brailleUTFKindAtom* o = new
    brailleUTFKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      brailleUTFKindVariable);
  assert (o != nullptr);
  return o;
}

brailleUTFKindAtom::brailleUTFKindAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  bsrUTFKind&   brailleUTFKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fBsrUTFKindVariable (
      brailleUTFKindVariable)
{}

brailleUTFKindAtom::~brailleUTFKindAtom ()
{}

void brailleUTFKindAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'brailleUTFKindAtom'" <<
      " with value \"" << theString << "\"" <<
      std::endl;
  }
#endif

  bsrUTFKind UTFKind = bsrUTFKind::kUTF8; // default value

  if (theString == "8") {
    UTFKind = bsrUTFKind::kUTF8;
  }
  else if (theString == "16") {
    UTFKind = bsrUTFKind::kUTF16;
  }
  else {
    // no, theString is unknown
    std::stringstream s;

    s <<
      "UTF kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The possible values are 8 and 16" <<
      std::endl;

    oahError (s.str ());
  }

  fBsrUTFKindVariable = UTFKind;
  fSetByAnOption = true;
}

void brailleUTFKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleUTFKindAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_brailleUTFKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleUTFKindAtom>*> (v)) {
        S_brailleUTFKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleUTFKindAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleUTFKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleUTFKindAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_brailleUTFKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleUTFKindAtom>*> (v)) {
        S_brailleUTFKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleUTFKindAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleUTFKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleUTFKindAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string brailleUTFKindAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    bsrUTFKindAsString (fBsrUTFKindVariable);

  return s.str ();
}

std::string brailleUTFKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    bsrUTFKindAsString (fBsrUTFKindVariable);

  return s.str ();
}

void brailleUTFKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "brailleUTFKindAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBrailleOutputKindVariable" << ": " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fBsrUTFKindVariable" << ": " <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
    std::endl;

  --gIndenter;
}

void brailleUTFKindAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": \"" <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
    "\"";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_brailleUTFKindAtom& elt)
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
S_brailleByteOrderingKindAtom brailleByteOrderingKindAtom::create (
  const std::string&   longName,
  const std::string&   shortName,
  const std::string&   description,
  const std::string&   valueSpecification,
  const std::string&   variableName,
  bsrByteOrderingKind& brailleByteOrderingKindVariable)
{
  brailleByteOrderingKindAtom* o = new
    brailleByteOrderingKindAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      brailleByteOrderingKindVariable);
  assert (o != nullptr);
  return o;
}

brailleByteOrderingKindAtom::brailleByteOrderingKindAtom (
  const std::string&   longName,
  const std::string&   shortName,
  const std::string&   description,
  const std::string&   valueSpecification,
  const std::string&   variableName,
  bsrByteOrderingKind& brailleByteOrderingKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fBsrByteOrderingKindVariable (
      brailleByteOrderingKindVariable)
{}

brailleByteOrderingKindAtom::~brailleByteOrderingKindAtom ()
{}

void brailleByteOrderingKindAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'brailleByteOrderingKindAtom'" <<
      " with value \"" << theString << "\"" <<
      std::endl;
  }
#endif

  bsrByteOrderingKind byteOrderingKind = bsrByteOrderingKind::kByteOrderingNone;

  if (theString == "big") {
    byteOrderingKind = bsrByteOrderingKind::kByteOrderingBigEndian;
  }
  else if (theString == "small") {
    byteOrderingKind = bsrByteOrderingKind::kByteOrderingSmallEndian;
  }
  else {
    // no, theString is unknown
    std::stringstream s;

    s <<
      "byte ordering '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The possible values are big and small" <<
      std::endl;

    oahError (s.str ());
  }

  fBsrByteOrderingKindVariable = byteOrderingKind;
  fSetByAnOption = true;
}

void brailleByteOrderingKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleByteOrderingKindAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_brailleByteOrderingKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleByteOrderingKindAtom>*> (v)) {
        S_brailleByteOrderingKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleByteOrderingKindAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleByteOrderingKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleByteOrderingKindAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_brailleByteOrderingKindAtom>*
    p =
      dynamic_cast<visitor<S_brailleByteOrderingKindAtom>*> (v)) {
        S_brailleByteOrderingKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleByteOrderingKindAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleByteOrderingKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleByteOrderingKindAtom::browseData ()" <<
      std::endl;
  }
#endif
}

std::string brailleByteOrderingKindAtom::asShortNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fShortName << ' ' <<
    bsrByteOrderingKindAsString (fBsrByteOrderingKindVariable);

  return s.str ();
}

std::string brailleByteOrderingKindAtom::asActualLongNamedOptionString () const
{
  std::stringstream s;

  s <<
    '-' << fLongName << ' ' <<
    bsrByteOrderingKindAsString (fBsrByteOrderingKindVariable);

  return s.str ();
}

void brailleByteOrderingKindAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "brailleByteOrderingKindAtom:" <<
    std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBsrUTFKindVariable" << ": " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fBsrByteOrderingKindVariable" << ": \"" <<
    bsrByteOrderingKindAsString (
      fBsrByteOrderingKindVariable) <<
      "\"" <<
    std::endl;

  --gIndenter;
}

void brailleByteOrderingKindAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": " <<
    bsrByteOrderingKindAsString (
      fBsrByteOrderingKindVariable);
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_brailleByteOrderingKindAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }
  
  return os;
}

//_______________________________________________________________________________

S_brailleGenerationOahGroup gGlobalBrailleGenerationOahGroup;

S_brailleGenerationOahGroup brailleGenerationOahGroup::create ()
{
  brailleGenerationOahGroup* o = new brailleGenerationOahGroup ();
  assert (o != nullptr);
  return o;
}

brailleGenerationOahGroup::brailleGenerationOahGroup ()
  : oahGroup (
    "Braille generation",
    "help-braille-generation", "hbg",
R"(These options control the way BSR data is translated to Braille.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  fBrailleOutputKind =
    bsrBrailleOutputKind::kBrailleOutputAscii; // default value

  fBrailleUTFKind = bsrUTFKind::kUTF8; // default value

  fByteOrderingKind =
    bsrByteOrderingKind::kByteOrderingNone;

  fBrailleUTFKind = bsrUTFKind::kUTF8; // default value

  fBrailleOutputKind =
    bsrBrailleOutputKind::kBrailleOutputAscii; // default value

  fByteOrderingKind =
    bsrByteOrderingKind::kByteOrderingNone;

  // initialize it
  initializeBrailleGenerationOahGroup ();
}

brailleGenerationOahGroup::~brailleGenerationOahGroup ()
{}

void brailleGenerationOahGroup::initializeBrailleFileNameOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Braille file name",
        "help-braille-music-file-name", "hlpbmfn",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // encoding in file name

  fUseEncodingInFileNameAtom =
    oahBooleanAtom::create (
      K_BRAILLE_USE_ENCODING_IN_FILE_NAME_LONG_NAME,
      K_BRAILLE_USE_ENCODING_IN_FILE_NAME_SHORT_NAME,
R"(Append a description of the encoding used
and the presence of a BOM if any to the file name before the '.'.)",
      "fUseEncodingInFileName",
      fUseEncodingInFileName);

  subGroup->
    appendAtomToSubGroup (
      fUseEncodingInFileNameAtom);
}

void brailleGenerationOahGroup::initializeBrailleOutputOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Braille output",
        "help-braille-output", "hbo",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // ascii output

  subGroup->
    appendAtomToSubGroup (
      brailleOutputKindAtom::create (
        "ascii", "",
R"(Generate ASCII braille as output.)",
        "fBrailleOutputKind",
        fBrailleOutputKind,
        bsrBrailleOutputKind::kBrailleOutputAscii));

  // UTF-8 output

  subGroup->
    appendAtomToSubGroup (
      brailleOutputKindAtom::create (
        "utf8", "",
R"(Generate UTF-8 braille as output.)",
        "fBrailleOutputKind",
        fBrailleOutputKind,
        bsrBrailleOutputKind::kBrailleOutputUTF8));

  // UTF-8 debug output

  subGroup->
    appendAtomToSubGroup (
      brailleOutputKindAtom::create (
        "utf8d", "",
R"(Generate UTF-8 debug braille as output.)",
        "fBrailleOutputKind",
        fBrailleOutputKind,
        bsrBrailleOutputKind::kBrailleOutputUTF8Debug));

  // UTF-16 output

  subGroup->
    appendAtomToSubGroup (
      brailleOutputKindAtom::create (
        "utf16", "",
R"(Generate UTF-16 braille as output.)",
        "fBrailleOutputKind",
        fBrailleOutputKind,
        bsrBrailleOutputKind::kBrailleOutputUTF16));

  // byte ordering

  fByteOrderingKind =
    bsrByteOrderingKind::kByteOrderingBigEndian; // default value

  fByteOrderingKindAtom =
    brailleByteOrderingKindAtom::create (
    K_BRAILLE_BYTE_ORDERING_KIND_LONG_NAME, K_BRAILLE_BYTE_ORDERING_KIND_SHORT_NAME,
R"(Generate an initial BOM_ENDIAN byte ordering mark (BOM)
ahead of the Braille nusic code,
which can be one of 'big' or 'small'.
By default, a big endian BOM is generated.)",
    "BOM_ENDIAN",
    "fByteOrderingKind",
    fByteOrderingKind);

  subGroup->
    appendAtomToSubGroup (
      fByteOrderingKindAtom);
}

void brailleGenerationOahGroup::initializePageParametersOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Page parameters",
        "help-braille-pages-parameters", "hlpbpp",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // cells per line

  fCellsPerLine = 30;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "cells-per-line", "cpl",
R"(Set the number of Braille cells per line to N. Default is 30 for A4 paper.)",
        "N",
        "fCellsPerLine",
        fCellsPerLine));

  // measures per line

  fMeasuresPerLine = 7;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "measures-per-line", "mpl",
R"(Set the number of Braille measures per line to N. Default is 7.)",
        "N",
        "fMeasuresPerLine",
        fMeasuresPerLine));

  // lines per page

  fLinesPerPage = 27;

  subGroup->
    appendAtomToSubGroup (
      oahIntegerAtom::create (
        "lines-per-page", "lpp",
R"(Set the number of Braille lines per page to N. Default is 27 for A4 paper.)",
        "N",
        "fLinesPerPage",
        fLinesPerPage));
}

void brailleGenerationOahGroup::initializeCodeGenerationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "help-braille-code-generation", "hlpcg",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // braille generation infos
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "braille-generation-infos", "bgi",
R"(Generate initial comments showing the braille generation informations.)",
        "fXml2brlInfos",
        fXml2brlInfos));

  // braille code
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "no-braille-code", "nobc",
R"(Don't generate any Braille code.
That can be useful if only a summary of the score is needed.)",
        "fNoBrailleCode",
        fNoBrailleCode));
}

void brailleGenerationOahGroup::initializeMacroOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Macros",
        "help-braille-generation-macros", "hbgm",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // create the auto utfd8 macro

  S_oahMacroAtom
    autoUTFd8MacroAtom =
      oahMacroAtom::create (
        "auto-utf8d", "au8d",
        "To ease the production of braille files.");

  subGroup->
    appendAtomToSubGroup (
      autoUTFd8MacroAtom);

  // populate it
  autoUTFd8MacroAtom->
    appendAtomToMacro (
      gGlobalOutputFileOahGroup->
        getAutoOutputFileNameAtom ());

  autoUTFd8MacroAtom->
    appendAtomToMacro (
      brailleOutputKindAtom::create (
        "utf8d", "",
R"(Generate UTF-8 debug braille as output.)",
        "fBrailleOutputKind",
        fBrailleOutputKind,
        bsrBrailleOutputKind::kBrailleOutputUTF8Debug));

  autoUTFd8MacroAtom->
    appendAtomToMacro (
      fUseEncodingInFileNameAtom);
}

void brailleGenerationOahGroup::initializeBrailleGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeBsr2brailletracingOah ();
#endif

  // braille file name
  // --------------------------------------
  initializeBrailleFileNameOptions ();

  // braille output
  // --------------------------------------
  initializeBrailleOutputOptions ();

  // page parameters
  // --------------------------------------
  initializePageParametersOptions ();

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions ();

  // macro options
  // --------------------------------------
  initializeMacroOptions ();
}

//______________________________________________________________________________
void brailleGenerationOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void brailleGenerationOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void brailleGenerationOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleGenerationOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_brailleGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_brailleGenerationOahGroup>*> (v)) {
        S_brailleGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleGenerationOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleGenerationOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleGenerationOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_brailleGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_brailleGenerationOahGroup>*> (v)) {
        S_brailleGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleGenerationOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleGenerationOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleGenerationOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void brailleGenerationOahGroup::printBrailleGenerationOahValues (int fieldWidth)
{
  gLogStream <<
    "The Bsr2braille options are:" <<
    std::endl;

  ++gIndenter;

  // code generation
  // --------------------------------------

  gLogStream <<
    "Braille code generation:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fBrailleUTFKind" << ": " <<
      bsrUTFKindAsString (fBrailleUTFKind) <<
      std::endl <<

    std::setw (fieldWidth) << "fByteOrderingKind" << ": " <<
      bsrByteOrderingKindAsString (fByteOrderingKind) <<
      std::endl <<

    std::setw (fieldWidth) << "fUseEncodingInFileName" << ": " <<
      fUseEncodingInFileName <<
      std::endl <<

    std::setw (fieldWidth) << "fCellsPerLine" << ": " <<
      fCellsPerLine <<
      std::endl <<
    std::setw (fieldWidth) << "fMeasuresPerLine" << ": " <<
      fMeasuresPerLine <<
      std::endl <<
    std::setw (fieldWidth) << "fLinesPerPage" << ": " <<
      fLinesPerPage <<
      std::endl <<

    std::setw (fieldWidth) << "fXml2brlInfos" << ": " <<
      fXml2brlInfos <<
      std::endl <<

    std::setw (fieldWidth) << "fNoBrailleCode" << ": " <<
      fNoBrailleCode <<
      std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_brailleGenerationOahGroup& elt)
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
S_brailleGenerationOahGroup createGlobalBrailleGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global bsr2braille OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalBrailleGenerationOahGroup) {
    // create the Bsr2braille options
    // ------------------------------------------------------

    gGlobalBrailleGenerationOahGroup =
      brailleGenerationOahGroup::create ();
    assert (gGlobalBrailleGenerationOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalBrailleGenerationOahGroup;
}


}
