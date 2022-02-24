/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "oahOah.h"
#include "outputFileOah.h"

#include "brailleGenerationOah.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// constants
const string K_BRAILLE_USE_ENCODING_IN_FILE_NAME_LONG_NAME  = "use-encoding-in-file-name";
const string K_BRAILLE_USE_ENCODING_IN_FILE_NAME_SHORT_NAME = "ueifn";

// const string K_BRAILLE_OUTPUT_KIND_LONG_NAME  = "use-encoding-in-file-name";
// const string K_BRAILLE_OUTPUT_KIND_SHORT_NAME = "ueifn";
//
// const string K_BRAILLE_UTF_KIND_LONG_NAME  = "use-encoding-in-file-name";
// const string K_BRAILLE_UTF_KIND_SHORT_NAME = "ueifn";

const string K_BRAILLE_BYTE_ORDERING_KIND_LONG_NAME  = "byte-ordering-kind";
const string K_BRAILLE_BYTE_ORDERING_KIND_SHORT_NAME = "bok";

//______________________________________________________________________________
S_brailleOutputKindAtom brailleOutputKindAtom::create (
  const string&         longName,
  const string&         shortName,
  const string&         description,
  const string&         variableName,
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
  const string&         longName,
  const string&         shortName,
  const string&         description,
  const string&         variableName,
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

void brailleOutputKindAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> handling atom '" <<
      fetchNames () <<
      "; which is of type 'brailleOutputKindAtom'" <<
      endl;
  }
#endif

  if (fSetByUser) {
    stringstream s;

    s <<
      "brailleOutputKindAtom generated output kind" << // JMI ???
      " set more than once" <<
      endl;

    oahError (s.str ());
  }
  else {
    fBrailleOutputKindVariable = fBrailleOutputKindValue;
    fSetByUser = true;
  }
}

void brailleOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOutputKindAtom::acceptIn ()" <<
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string brailleOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrBrailleOutputKindAsString (fBrailleOutputKindVariable);

  return s.str ();
}

string brailleOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrBrailleOutputKindAsString (fBrailleOutputKindVariable);

  return s.str ();
}

void brailleOutputKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "brailleOutputKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fBrailleOutputKindVariable" << " : \"" <<
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindVariable) <<
      "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fBrailleOutputKindValue" << " : \"" <<
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindValue) <<
      "\"" <<
    endl;

  --gIndenter;
}

void brailleOutputKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    bsrBrailleOutputKindAsString (
      fBrailleOutputKindVariable);
  if (fSetByUser) {
    os <<
      ", set by user";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_brailleOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_brailleUTFKindAtom brailleUTFKindAtom::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName,
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'brailleUTFKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
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
    stringstream s;

    s <<
      "UTF kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The possible values are 8 and 16" <<
      endl;

    oahError (s.str ());
  }

  fBsrUTFKindVariable = UTFKind;
  fSetByUser = true;
}

void brailleUTFKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleUTFKindAtom::acceptIn ()" <<
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string brailleUTFKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrUTFKindAsString (fBsrUTFKindVariable);

  return s.str ();
}

string brailleUTFKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrUTFKindAsString (fBsrUTFKindVariable);

  return s.str ();
}

void brailleUTFKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "brailleUTFKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fBrailleOutputKindVariable" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fBsrUTFKindVariable" << " : " <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
    endl;

  --gIndenter;
}

void brailleUTFKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
    "\"";
  if (fSetByUser) {
    os <<
      ", set by user";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_brailleUTFKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_brailleByteOrderingKindAtom brailleByteOrderingKindAtom::create (
  const string&        longName,
  const string&        shortName,
  const string&        description,
  const string&        valueSpecification,
  const string&        variableName,
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
  const string&        longName,
  const string&        shortName,
  const string&        description,
  const string&        valueSpecification,
  const string&        variableName,
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
  const string& theString,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'brailleByteOrderingKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
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
    stringstream s;

    s <<
      "byte ordering '" << theString <<
      "' is unknown" <<
      endl <<
      "The possible values are big and small" <<
      endl;

    oahError (s.str ());
  }

  fBsrByteOrderingKindVariable = byteOrderingKind;
  fSetByUser = true;
}

void brailleByteOrderingKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleByteOrderingKindAtom::acceptIn ()" <<
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string brailleByteOrderingKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrByteOrderingKindAsString (fBsrByteOrderingKindVariable);

  return s.str ();
}

string brailleByteOrderingKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrByteOrderingKindAsString (fBsrByteOrderingKindVariable);

  return s.str ();
}

void brailleByteOrderingKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "brailleByteOrderingKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fBsrUTFKindVariable" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fBsrByteOrderingKindVariable" << " : \"" <<
    bsrByteOrderingKindAsString (
      fBsrByteOrderingKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void brailleByteOrderingKindAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    bsrByteOrderingKindAsString (
      fBsrByteOrderingKindVariable);
  if (fSetByUser) {
    os <<
      ", set by user";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_brailleByteOrderingKindAtom& elt)
{
  elt->print (os);
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

//______________________________________________________________________________
void brailleGenerationOahGroup::printBrailleGenerationOahValues (int fieldWidth)
{
  gLogStream <<
    "The Bsr2braille options are:" <<
    endl;

  ++gIndenter;

  // code generation
  // --------------------------------------

  gLogStream <<
    "Braille code generation:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fBrailleUTFKind" << " : " <<
      bsrUTFKindAsString (fBrailleUTFKind) <<
      endl <<

    setw (fieldWidth) << "fByteOrderingKind" << " : " <<
      bsrByteOrderingKindAsString (fByteOrderingKind) <<
      endl <<

    setw (fieldWidth) << "fUseEncodingInFileName" << " : " <<
      fUseEncodingInFileName <<
      endl <<

    setw (fieldWidth) << "fCellsPerLine" << " : " <<
      fCellsPerLine <<
      endl <<
    setw (fieldWidth) << "fMeasuresPerLine" << " : " <<
      fMeasuresPerLine <<
      endl <<
    setw (fieldWidth) << "fLinesPerPage" << " : " <<
      fLinesPerPage <<
      endl <<

    setw (fieldWidth) << "fXml2brlInfos" << " : " <<
      fXml2brlInfos <<
      endl <<

    setw (fieldWidth) << "fNoBrailleCode" << " : " <<
      fNoBrailleCode <<
      endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_brailleGenerationOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_brailleGenerationOahGroup createGlobalBrailleGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global bsr2braille OAH group" <<
      endl;
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
