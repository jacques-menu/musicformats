/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include "mfAssert.h"
#include "mfBool.h"
#include "mfStringsHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "ischemeBasicTypes.h"
#include "ischemeDriver.h"

#include "ischemeInterpreterOah.h"

#include "ischemeWae.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
S_ischemeOptionsBlock ischemeOptionsBlock::create (
  const string& optionsBlockName)
{
  ischemeOptionsBlock* o =
    new ischemeOptionsBlock (
      optionsBlockName);
  assert (o != nullptr);
  return o;
}

ischemeOptionsBlock::ischemeOptionsBlock (
  const string& optionsBlockName)
{
  fOptionsBlockName = optionsBlockName;
}

ischemeOptionsBlock::~ischemeOptionsBlock ()
{}

void ischemeOptionsBlock::registerOptionsInOptionsBlock (
  S_oahOption option,
  ischemeDriver& drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceOptionsBlocks ()) {
    gLogStream <<
      "====> Registering option [" <<
      option->asString () <<
      "] in options block \"" <<
      fOptionsBlockName <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  fOptionsBlockOptionsVector.push_back (
    option);
}

string ischemeOptionsBlock::asString () const
{
  stringstream s;

  s <<
    "ischemeOptionsBlock [" <<
    "fOptionsBlockName: " << fOptionsBlockName <<
    ']';

  return s.str ();
}

string ischemeOptionsBlock::asOptionsString () const
{
  stringstream s;

  if (fOptionsBlockOptionsVector.size ()) {
    vector<S_oahOption>::const_iterator
      iBegin = fOptionsBlockOptionsVector.begin (),
      iEnd   = fOptionsBlockOptionsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahOption option = (*i);

      s <<
        option->
          asStringForCommandLine ();

      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  return s.str ();
}

void ischemeOptionsBlock::print (ostream& os) const
{
  os <<
    "ischemeOptionsBlock [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 34;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsBlockName" << " : " << fOptionsBlockName <<
    endl;

  // print the options blocks options name and value vector
  gLogStream <<
    "fOptionsBlockOptionsVector" << " : " <<
    endl;

  ++gIndenter;

  for (S_oahOption option : fOptionsBlockOptionsVector) {
    gLogStream << option;
  } // for

  --gIndenter;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_ischemeOptionsBlock& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//_______________________________________________________________________________
string ischemeInputNameKindAsString (
  ischemeInputNameKind inputNameKind)
{
  string result;

  switch (inputNameKind) {
    case ischemeInputNameKind::kInputNameNone:
      result = "kInputNameNone";
      break;
    case ischemeInputNameKind::kInputNameOptionSupplied:
      result = "kInputNameOptionSupplied";
      break;
    case ischemeInputNameKind::kInputNameSetInScript:
      result = "kInputNameSetInScript";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const ischemeInputNameKind& elt)
{
  os << ischemeInputNameKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_ischemeInput ischemeInput::create (
  const string& inputName)
{
  ischemeInput* o =
    new ischemeInput (
      inputName);
  assert (o != nullptr);
  return o;
}

ischemeInput::ischemeInput (
  const string& inputName)
{
  fInputName = inputName;

  fInputNameKind =
    ischemeInputNameKind::kInputNameNone;
}

ischemeInput::~ischemeInput ()
{}

void ischemeInput::setInputNameSuppliedByAnOption (
  const string& name,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLogStream <<
      "====> Setting the name of input \"" <<
      fInputName <<
      "\" to \"" <<
      name <<
      "\", supplied by an option" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // no checks here, the names are not knoown yet
  fInputName = name;
}

void ischemeInput::selectInputName (
  const string& name,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLogStream <<
      "====> Setting the name of input \"" <<
      fInputName <<
      "\" to \"" <<
      name <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this name in the inputs names set?
  set<string>::const_iterator
    it =
      fNamesSet.find (name);

  if (it == fNamesSet.end ()) {
    // no, issue error message
    stringstream s;

    s <<
      "input \"" <<
      fInputName <<
      "\" cannot be set to name \"" <<
      name <<
      "\", the names are: " <<
      namesSetAsString ();

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

//   // did fInputName get a name by an option?
//   const multimap<string, string>&
//     setInputToNameMultiMap =
//       gGlobalIschemeInterpreterOahGroup->
//         getSelectInputToNamesMultiMap ();
//
//   switch (fInputNameKind) {
//     case ischemeInputNameKind::kInputNameNone:
//       {
//         // store into the input
//         string optionSuppliedName;
//
//         Bool
//           inputGotAValueByAnOption =
//             mfKeyValuePairIsInStringToStringMultiMap (
//               fInputName,
//               setInputToNameMultiMap,
//               optionSuppliedName);
//
//         if (inputGotAValueByAnOption) {
//           if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
//             gLogStream <<
//               "====> Input name \"" <<
//               fInputName <<
//               "\" has been selected as \"" <<
//               optionSuppliedName <<
//               "\" by an option-supplied to the script" <<
//               ", ignoring name \"" <<
//               name <<
//               "\"" <<
//               "\", line " << drv.getScannerLocation () <<
//               endl;
//           }
//
//           fInputName = optionSuppliedName;
//           fInputNameKind =
//             ischemeInputNameKind::kInputNameOptionSupplied;
//
//           drv.registerOptionsSuppliedInputsAsUsed (
//             fInputName);
//         }
//
//         else {
//           fInputName = optionSuppliedName;
//           fInputNameKind =
//             ischemeInputNameKind::kInputNameSetInScript;
//         }
//       }
//       break;
//
//     case ischemeInputNameKind::kInputNameOptionSupplied:
//       {
//         // ignore this name, and keep name supplied to the script
//         stringstream s;
//
//         s <<
//           "input \"" <<
//           fInputName <<
//           " already got name \"" <<
//           fInputName <<
//           "\" supplied to the script"
//           ", ignoring name \"" <<
//           name <<
//           "\"";
//
//         ischemeWarning (
//           s.str (),
//           drv.getScannerLocation ());
//       }
//       break;
//
//     case ischemeInputNameKind::kInputNameSetInScript:
//       {
//         // set in script multiple times
//         stringstream s;
//
//         s <<
//           "input \"" <<
//           fInputName <<
//           "\"" <<
//           " already has name \"" <<
//           fInputName <<
//           "\" supplied to the script";
//
//         ischemeError (
//           s.str (),
//           drv.getScannerLocation ());
//       }
//       break;
//   } // switch
}

string ischemeInput::getInputName (ischemeDriver& drv) const
{
  switch (fInputNameKind) {
    case ischemeInputNameKind::kInputNameNone:
      {
        stringstream s;

        s <<
          "input \"" <<
          fInputName <<
          "\" has not got any name";

        ischemeError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case ischemeInputNameKind::kInputNameOptionSupplied:
      if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
        gLogStream <<
          "====> " <<
          " input \"" <<
          fInputName <<
          ", name " <<
          fInputName <<
          "\", supplied to the script" <<
          ", line " << drv.getScannerLocation () <<
          endl;
      }
      break;

    case ischemeInputNameKind::kInputNameSetInScript:
      if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
        gLogStream <<
          "====>" <<
          "input \"" <<
          fInputName <<
          "\" has name \"" <<
          fInputName <<
          " \", set in the script" <<
          ", line " << drv.getScannerLocation () <<
          endl;
      }
      break;
  } // switch

  return fInputName;
}

string ischemeInput::getInputNameWithoutTrace (ischemeDriver& drv) const
{
  switch (fInputNameKind) {
    case ischemeInputNameKind::kInputNameNone:
      {
        stringstream s;

        s <<
          "input \"" <<
          fInputName <<
          "\" has not got any name";

        ischemeError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case ischemeInputNameKind::kInputNameOptionSupplied:
    case ischemeInputNameKind::kInputNameSetInScript:
      break;
  } // switch

  return fInputName;
}

S_ischemeOptionsBlock ischemeInput::getInputOptionsBlockForName (
  const string& name,
  ischemeDriver&   drv) const
{
  S_ischemeOptionsBlock result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLogStream <<
      "====> Fetching name \"" <<
      name <<
      "\" in input \"" <<
      fInputName <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  map<string, S_ischemeOptionsBlock>::const_iterator
    it =
      fInputNamesToOptionsBlocksMap.find (
        name);

  if (it != fInputNamesToOptionsBlocksMap.end ()) {
    result = (*it).second;
  }
  else {
    stringstream s;

    s <<
      "name \"" <<
      name <<
      "\" is not known to input \"" <<
      fInputName <<
      "\", line " << drv.getScannerLocation ();

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLogStream <<
      "<==== Result: \"" <<
      result <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  return result;
}

void ischemeInput::addName (
  const string& name,
  ischemeDriver&   drv)
{
  // is this name in the inputs names set?
  set<string>::const_iterator
    it =
      fNamesSet.find (name);

  if (it != fNamesSet.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "input name \"" <<
      name <<
      "\" occurs more that once in input \"" <<
      fInputName <<
      "\", line " << drv.getScannerLocation ();

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  else {
    if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
      gLogStream <<
        "====> Adding name \"" <<
        name <<
        "\"" <<
        " to input \"" <<
        fInputName <<
        "\"" <<
        ", line " << drv.getScannerLocation () <<
        endl;
    }

    // register this inputs name
    fNamesSet.insert (name);

    // create the options block for it
    fInputNamesToOptionsBlocksMap [name] =
      ischemeOptionsBlock::create (
        name);
  }
}

void ischemeInput::enrichNameOptionsBlock (
  const string&      name,
  S_ischemeOptionsBlock optionsBlock,
  ischemeDriver&        drv)
{
  S_ischemeOptionsBlock
    nameOptionsBlock =
      fInputNamesToOptionsBlocksMap [
        name];

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    nameOptionsBlock != nullptr,
    "nameOptionsBlock is null");

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLogStream <<
      "====> Enriching options block \"" <<
      nameOptionsBlock->getOptionsBlockName () <<
      "\" (" <<
      mfSingularOrPlural (
        nameOptionsBlock->getOptionsBlockSize (),
        "option", "options") <<
      ") with the contents of " <<
      optionsBlock->getOptionsBlockName () <<
      "\" (" <<
      mfSingularOrPlural (
        optionsBlock->getOptionsBlockSize (),
        "option", "options") <<
      ')' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  vector<S_oahOption>&
    nameOptionsBlockOptionsVector =
      nameOptionsBlock->
        getOptionsBlockOptionsVectorNonConst ();

  const vector<S_oahOption>&
    optionsBlockOptionsVector =
      optionsBlock->
        getOptionsBlockOptionsVector ();

  optionsNameAndValueVectorsPlusEquals (
    nameOptionsBlockOptionsVector,
    optionsBlockOptionsVector);
}

string ischemeInput::namesSetAsString () const
{
  stringstream s;

  size_t namesSetSize =
    fNamesSet.size ();

  if (namesSetSize) {
    size_t nextToLast =
      namesSetSize - 1;

    size_t count = 0;

    size_t namesListMaxLength = 60;
    size_t cumulatedLength    = 0;

    for (
      set<string>::const_iterator i =
        fNamesSet.begin ();
      i != fNamesSet.end ();
      ++i
    ) {
      string theString = (*i);

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != namesSetSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

string ischemeInput::asString () const
{
  stringstream s;

  s <<
    "ischemeInput [" <<
    "fInputName: " << fInputName <<
    ", " <<

    "fInputName: [" << fInputName << ']' <<
    ']';

  return s.str ();
}

void ischemeInput::displayInputNamesToOptionsBlocksMap (ostream& os) const
{
  size_t
    InputNamesToOptionsBlocksMapSize =
      fNamesSet.size ();

  const int fieldWidth = 19;

  os <<
    setw (fieldWidth) <<
    "fInputNamesToOptionsBlocksMap" << " [" <<
    endl;

  if (InputNamesToOptionsBlocksMapSize) {
//     int counter = 0;
    for (pair<string, S_ischemeOptionsBlock> thePair : fInputNamesToOptionsBlocksMap) {
      string        key = thePair.first;
      S_ischemeOptionsBlock optionsBlock = thePair.second;
      os <<
        key << ": " <<
        endl;

      ++gIndenter;

      os <<
        optionsBlock;

      --gIndenter;

//       if (++counter < InputNamesToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << ']' << endl;
}

void ischemeInput::print (ostream& os) const
{
  os <<
    "ischemeInput [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 19;

  os << left <<
    setw (fieldWidth) <<
    "fInputName" << " : \"" << fInputName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fInputNameKind" << " : " << fInputNameKind << "" <<
    endl <<
    setw (fieldWidth) <<
    "fInputName" << " : \"" << fInputName << "\"" <<
    endl;

  size_t
    namesSetSize =
      fNamesSet.size ();

  os <<
    setw (fieldWidth) <<
    "fNamesSet" << " : [";

  if (namesSetSize) {
    size_t counter = 0;
    for (string name : fNamesSet) {
      os << name;

      if (++counter < namesSetSize) {
        os << " | ";
      }
    } // for
  }
  os << ']' << endl;

  size_t
    inputNamesToOptionsBlocksMapSize =
      fNamesSet.size ();

  os <<
    setw (fieldWidth) <<
    "fInputNamesToOptionsBlocksMap" << " [" <<
    endl;

  if (inputNamesToOptionsBlocksMapSize) {
//     int counter = 0;
    for (pair<string, S_ischemeOptionsBlock> thePair : fInputNamesToOptionsBlocksMap) {
      string        key = thePair.first;
      S_ischemeOptionsBlock optionsBlock = thePair.second;
      os <<
        key << ": " <<
        endl;

      ++gIndenter;

      if (optionsBlock) {
        os <<
          optionsBlock;
      }
      else {
        os << "[NONE]" << endl;
      }

      --gIndenter;

//       if (++counter < InputNamesToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << ']' << endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_ischemeInput& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_ischemeInputsTable ischemeInputsTable::create ()
{
  ischemeInputsTable* o =
    new ischemeInputsTable ();
  assert (o != nullptr);
  return o;
}

ischemeInputsTable::ischemeInputsTable ()
{}

ischemeInputsTable::~ischemeInputsTable ()
{}

void ischemeInputsTable::addInput (
  S_ischemeInput input,
  ischemeDriver& drv)
{
  string
    inputName =
      input->getInputName ();

  // is this inputName in the inputs names set?
  map<string, S_ischemeInput>::const_iterator
    it =
      fInputsMap.find (
        inputName);

  if (it != fInputsMap.end ()) {
    stringstream s;

    s <<
      "input \"" <<
      inputName <<
      "\" occurs more that once in the inputs table";

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  fInputsMap [inputName] = input;
}

// S_ischemeInput ischemeInputsTable::lookupInputByName (
//   const string& name)
// {
//   S_ischemeInput result;
//
//   // is this inputName in the inputs names set?
//   map<string, S_ischemeInput>::const_iterator
//     it =
//       fInputsMap.find (name);
//
//   if (it != fInputsMap.end ()) {
//     result = (*it).second;
//   }
//
//   return result;
// }

S_ischemeInput ischemeInputsTable::fetchInputByName (
  const string&     name,
  const ischemeDriver& drv)
{
  S_ischemeInput result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLogStream <<
      "====> Fetching input named \"" <<
      name <<
      "\" in inputs table" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this inputName in the inputs names set?
  map<string, S_ischemeInput>::const_iterator
    it =
      fInputsMap.find (name);

  if (it != fInputsMap.end ()) {
    result = (*it).second;
  }

  else {
    stringstream s;

    s <<
      "input \"" << name <<
      "\" is unknown in inputs table";

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

S_ischemeInput ischemeInputsTable::fetchInputByNameNonConst (
  const string& name,
  ischemeDriver&   drv)
{
  S_ischemeInput result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLogStream <<
      "====> Fetching input named \"" <<
      name <<
      "\" in inputs table (non-const)" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this inputName in the inputs map?
  map<string, S_ischemeInput>::const_iterator
    it =
      fInputsMap.find (name);

  if (it != fInputsMap.end ()) {
    result = (*it).second;
  }

  else {
    stringstream s;

    s <<
      "input \"" << name <<
      "\" is unknown in inputs table (non-const)";

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

string ischemeInputsTable::asString () const
{
  stringstream s;

  s <<
    "ischemeInputsTable [" <<
    ']';

  return s.str ();
}

void ischemeInputsTable::displayInputsMap (ostream& os) const // useless ??? JMI
{
  os <<
    "ischemeInputsTable [" <<
    endl;

  if (fInputsMap.size ()) {
    ++gIndenter;

    for (pair<string, S_ischemeInput> thePair : fInputsMap) {
      S_ischemeInput input = thePair.second;

      input->
        displayInputNamesToOptionsBlocksMap (os);
    } // for

    --gIndenter;
  }

  os << ']' << endl;
}

void ischemeInputsTable::print (ostream& os) const
{
  os <<
    "ischemeInputsTable [" <<
    endl;

  if (fInputsMap.size ()) {
    ++gIndenter;

    for (pair<string, S_ischemeInput> thePair : fInputsMap) {
      S_ischemeInput input = thePair.second;

      os << input;
    } // for

    --gIndenter;
  }

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_ischemeInputsTable& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//_______________________________________________________________________________
string ischemeChoiceLabelKindAsString (
  ischemeChoiceLabelKind inputNameKind)
{
  string result;

  switch (inputNameKind) {
    case ischemeChoiceLabelKind::kChoiceLabelNone:
      result = "kChoiceLabelNone";
      break;
    case ischemeChoiceLabelKind::kChoiceLabelOptionSupplied:
      result = "kChoiceLabelOptionSupplied";
      break;
    case ischemeChoiceLabelKind::kChoiceLabelSetInScript:
      result = "kChoiceLabelSetInScript";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const ischemeChoiceLabelKind& elt)
{
  os << ischemeChoiceLabelKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_ischemeChoice ischemeChoice::create (
  const string& choiceName)
{
  ischemeChoice* o =
    new ischemeChoice (
      choiceName);
  assert (o != nullptr);
  return o;
}

ischemeChoice::ischemeChoice (
  const string& choiceName)
{
  fChoiceName = choiceName;

  fChoiceLabelKind =
    ischemeChoiceLabelKind::kChoiceLabelNone;
}

ischemeChoice::~ischemeChoice ()
{}

void ischemeChoice::setChoiceLabelSuppliedByAnOption (
  const string& label,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLogStream <<
      "====> Setting the label of choice \"" <<
      fChoiceName <<
      "\" to \"" <<
      label <<
      "\", supplied by an option" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // no checks here, the labels are not knoown yet
  fChoiceLabel = label;
}

void ischemeChoice::selectChoiceLabel (
  const string& label,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLogStream <<
      "====> Setting the label of choice \"" <<
      fChoiceName <<
      "\" to \"" <<
      label <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this label in the choices labels set?
  set<string>::const_iterator
    it =
      fLabelsSet.find (label);

  if (it == fLabelsSet.end ()) {
    // no, issue error message
    stringstream s;

    s <<
      "choice \"" <<
      fChoiceName <<
      "\" cannot be set to label \"" <<
      label <<
      "\", the labels are: " <<
      labelsSetAsString ();

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  // did fChoiceName get a label by an option?
  const multimap<string, string>&
    setChoiceToLabelMultiMap =
      gGlobalIschemeInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMap ();

  switch (fChoiceLabelKind) {
    case ischemeChoiceLabelKind::kChoiceLabelNone:
      {
        // store into the choice
        string optionSuppliedLabel;

        Bool
          choiceGotAValueByAnOption =
            mfKeyValuePairIsInStringToStringMultiMap (
              fChoiceName,
              setChoiceToLabelMultiMap,
              optionSuppliedLabel);

        if (choiceGotAValueByAnOption) {
          if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
            gLogStream <<
              "====> Choice name \"" <<
              fChoiceName <<
              "\" has been selected as \"" <<
              optionSuppliedLabel <<
              "\" by an option-supplied to the script" <<
              ", ignoring label \"" <<
              label <<
              "\"" <<
              "\", line " << drv.getScannerLocation () <<
              endl;
          }

          fChoiceLabel = optionSuppliedLabel;
          fChoiceLabelKind =
            ischemeChoiceLabelKind::kChoiceLabelOptionSupplied;

          drv.registerOptionsSuppliedChoicesAsUsed (
            fChoiceName);
        }

        else {
          fChoiceLabel = optionSuppliedLabel;
          fChoiceLabelKind =
            ischemeChoiceLabelKind::kChoiceLabelSetInScript;
        }
      }
      break;

    case ischemeChoiceLabelKind::kChoiceLabelOptionSupplied:
      {
        // ignore this label, and keep label supplied to the script
        stringstream s;

        s <<
          "choice \"" <<
          fChoiceName <<
          " already got label \"" <<
          fChoiceLabel <<
          "\" supplied to the script"
          ", ignoring label \"" <<
          label <<
          "\"";

        ischemeWarning (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case ischemeChoiceLabelKind::kChoiceLabelSetInScript:
      {
        // set in script multiple times
        stringstream s;

        s <<
          "choice \"" <<
          fChoiceName <<
          "\"" <<
          " already has label \"" <<
          fChoiceLabel <<
          "\" supplied to the script";

        ischemeError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;
  } // switch
}

string ischemeChoice::getChoiceLabel (ischemeDriver& drv) const
{
  switch (fChoiceLabelKind) {
    case ischemeChoiceLabelKind::kChoiceLabelNone:
      {
        stringstream s;

        s <<
          "choice \"" <<
          fChoiceName <<
          "\" has not got any label";

        ischemeError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case ischemeChoiceLabelKind::kChoiceLabelOptionSupplied:
      if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
        gLogStream <<
          "====> " <<
          " choice \"" <<
          fChoiceName <<
          ", label " <<
          fChoiceLabel <<
          "\", supplied to the script" <<
          ", line " << drv.getScannerLocation () <<
          endl;
      }
      break;

    case ischemeChoiceLabelKind::kChoiceLabelSetInScript:
      if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
        gLogStream <<
          "====>" <<
          "choice \"" <<
          fChoiceName <<
          "\" has label \"" <<
          fChoiceLabel <<
          " \", set in the script" <<
          ", line " << drv.getScannerLocation () <<
          endl;
      }
      break;
  } // switch

  return fChoiceLabel;
}

string ischemeChoice::getChoiceLabelWithoutTrace (ischemeDriver& drv) const
{
  switch (fChoiceLabelKind) {
    case ischemeChoiceLabelKind::kChoiceLabelNone:
      {
        stringstream s;

        s <<
          "choice \"" <<
          fChoiceName <<
          "\" has not got any label";

        ischemeError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case ischemeChoiceLabelKind::kChoiceLabelOptionSupplied:
    case ischemeChoiceLabelKind::kChoiceLabelSetInScript:
      break;
  } // switch

  return fChoiceLabel;
}

S_ischemeOptionsBlock ischemeChoice::getChoiceOptionsBlockForLabel (
  const string& label,
  ischemeDriver&   drv) const
{
  S_ischemeOptionsBlock result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLogStream <<
      "====> Fetching label \"" <<
      label <<
      "\" in choice \"" <<
      fChoiceName <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  map<string, S_ischemeOptionsBlock>::const_iterator
    it =
      fChoiceLabelsToOptionsBlocksMap.find (
        label);

  if (it != fChoiceLabelsToOptionsBlocksMap.end ()) {
    result = (*it).second;
  }
  else {
    stringstream s;

    s <<
      "label \"" <<
      label <<
      "\" is not known to choice \"" <<
      fChoiceName <<
      "\", line " << drv.getScannerLocation ();

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLogStream <<
      "<==== Result: \"" <<
      result <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  return result;
}

void ischemeChoice::addLabel (
  const string& label,
  ischemeDriver&   drv)
{
  // is this label in the choices labels set?
  set<string>::const_iterator
    it =
      fLabelsSet.find (label);

  if (it != fLabelsSet.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "choice label \"" <<
      label <<
      "\" occurs more that once in choice \"" <<
      fChoiceName <<
      "\", line " << drv.getScannerLocation ();

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  else {
    if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
      gLogStream <<
        "====> Adding label \"" <<
        label <<
        "\"" <<
        " to choice \"" <<
        fChoiceName <<
        "\"" <<
        ", line " << drv.getScannerLocation () <<
        endl;
    }

    // register this choices label
    fLabelsSet.insert (label);

    // create the options block for it
    fChoiceLabelsToOptionsBlocksMap [label] =
      ischemeOptionsBlock::create (
        label);
  }
}

void ischemeChoice::enrichLabelOptionsBlock (
  const string&      label,
  S_ischemeOptionsBlock optionsBlock,
  ischemeDriver&        drv)
{
  S_ischemeOptionsBlock
    labelOptionsBlock =
      fChoiceLabelsToOptionsBlocksMap [
        label];

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    labelOptionsBlock != nullptr,
    "labelOptionsBlock is null");

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLogStream <<
      "====> Enriching options block \"" <<
      labelOptionsBlock->getOptionsBlockName () <<
      "\" (" <<
      mfSingularOrPlural (
        labelOptionsBlock->getOptionsBlockSize (),
        "option", "options") <<
      ") with the contents of " <<
      optionsBlock->getOptionsBlockName () <<
      "\" (" <<
      mfSingularOrPlural (
        optionsBlock->getOptionsBlockSize (),
        "option", "options") <<
      ')' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  vector<S_oahOption>&
    labelOptionsBlockOptionsVector =
      labelOptionsBlock->
        getOptionsBlockOptionsVectorNonConst ();

  const vector<S_oahOption>&
    optionsBlockOptionsVector =
      optionsBlock->
        getOptionsBlockOptionsVector ();

  optionsNameAndValueVectorsPlusEquals (
    labelOptionsBlockOptionsVector,
    optionsBlockOptionsVector);
}

void ischemeChoice::registerChoiceDefaultLabel (
  const string& label,
  ischemeDriver&   drv)
{
  if (! mfStringIsInStringSet (label, fLabelsSet)) {
    stringstream s;

    s <<
      "choice label \"" <<
      label <<
      "\" is not present in choice \"" <<
      fChoiceName <<
      "\"";

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

//   if (fChoiceDefaultLabel.size ()) { // JMI
//     stringstream s;
//
//     s <<
//       "choice label \"" <<
//       label <<
//       "\" is not present in choice \"" <<
//       fChoiceName <<
//       "\"";
//
//     ischemeInternalError (
//       s.str (),
//       drv.getScannerLocation ());
//   }

  fChoiceDefaultLabel = label;
}

string ischemeChoice::labelsSetAsString () const
{
  stringstream s;

  size_t labelsSetSize =
    fLabelsSet.size ();

  if (labelsSetSize) {
    size_t nextToLast =
      labelsSetSize - 1;

    size_t count = 0;

    size_t namesListMaxLength = 60;
    size_t cumulatedLength    = 0;

    for (
      set<string>::const_iterator i =
        fLabelsSet.begin ();
      i != fLabelsSet.end ();
      ++i
    ) {
      string theString = (*i);

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != labelsSetSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

string ischemeChoice::asString () const
{
  stringstream s;

  s <<
    "ischemeChoice [" <<
    "fChoiceName: " << fChoiceName <<
    ", " <<

    "fChoiceLabel: [" << fChoiceLabel << ']' <<
    ']';

  return s.str ();
}

void ischemeChoice::displayChoiceLabelsToOptionsBlocksMap (ostream& os) const
{
  size_t
    ChoiceLabelsToOptionsBlocksMapSize =
      fLabelsSet.size ();

  const int fieldWidth = 19;

  os <<
    setw (fieldWidth) <<
    "fChoiceLabelsToOptionsBlocksMap" << " [" <<
    endl;

  if (ChoiceLabelsToOptionsBlocksMapSize) {
//     int counter = 0;
    for (pair<string, S_ischemeOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      string        key = thePair.first;
      S_ischemeOptionsBlock optionsBlock = thePair.second;
      os <<
        key << ": " <<
        endl;

      ++gIndenter;

      os <<
        optionsBlock;

      --gIndenter;

//       if (++counter < ChoiceLabelsToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << ']' << endl;
}

void ischemeChoice::print (ostream& os) const
{
  os <<
    "ischemeChoice [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 19;

  os << left <<
    setw (fieldWidth) <<
    "fChoiceName" << " : \"" << fChoiceName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fChoiceLabelKind" << " : " << fChoiceLabelKind << "" <<
    endl <<
    setw (fieldWidth) <<
    "fChoiceLabel" << " : \"" << fChoiceLabel << "\"" <<
    endl;

  size_t
    labelsSetSize =
      fLabelsSet.size ();

  os <<
    setw (fieldWidth) <<
    "fLabelsSet" << " : [";

  if (labelsSetSize) {
    size_t counter = 0;
    for (string label : fLabelsSet) {
      os << label;

      if (++counter < labelsSetSize) {
        os << " | ";
      }
    } // for
  }
  os << ']' << endl;

  os << left <<
    setw (fieldWidth) <<
    "fChoiceDefaultLabel" << " : \"" << fChoiceDefaultLabel << "\"" <<
    endl;

  size_t
    choiceLabelsToOptionsBlocksMapSize =
      fLabelsSet.size ();

  os <<
    setw (fieldWidth) <<
    "fChoiceLabelsToOptionsBlocksMap" << " [" <<
    endl;

  if (choiceLabelsToOptionsBlocksMapSize) {
//     int counter = 0;
    for (pair<string, S_ischemeOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      string        key = thePair.first;
      S_ischemeOptionsBlock optionsBlock = thePair.second;
      os <<
        key << ": " <<
        endl;

      ++gIndenter;

      if (optionsBlock) {
        os <<
          optionsBlock;
      }
      else {
        os << "[NONE]" << endl;
      }

      --gIndenter;

//       if (++counter < ChoiceLabelsToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << ']' << endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_ischemeChoice& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_ischemeChoicesTable ischemeChoicesTable::create ()
{
  ischemeChoicesTable* o =
    new ischemeChoicesTable ();
  assert (o != nullptr);
  return o;
}

ischemeChoicesTable::ischemeChoicesTable ()
{}

ischemeChoicesTable::~ischemeChoicesTable ()
{}

void ischemeChoicesTable::addChoice (
  S_ischemeChoice choice,
  ischemeDriver&  drv)
{
  string
    choiceName =
      choice->getChoiceName ();

  // is this choiceName in the choices labels set?
  map<string, S_ischemeChoice>::const_iterator
    it =
      fChoicesMap.find (
        choiceName);

  if (it != fChoicesMap.end ()) {
    stringstream s;

    s <<
      "choice \"" <<
      choiceName <<
      "\" occurs more that once in the choices table";

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  fChoicesMap [choiceName] = choice;
}

// S_ischemeChoice ischemeChoicesTable::lookupChoiceByName (
//   const string& name)
// {
//   S_ischemeChoice result;
//
//   // is this choiceName in the choices labels set?
//   map<string, S_ischemeChoice>::const_iterator
//     it =
//       fChoicesMap.find (name);
//
//   if (it != fChoicesMap.end ()) {
//     result = (*it).second;
//   }
//
//   return result;
// }

S_ischemeChoice ischemeChoicesTable::fetchChoiceByName (
  const string&     name,
  const ischemeDriver& drv)
{
  S_ischemeChoice result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLogStream <<
      "====> Fetching choice named \"" <<
      name <<
      "\" in choices table" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this choiceName in the choices labels set?
  map<string, S_ischemeChoice>::const_iterator
    it =
      fChoicesMap.find (name);

  if (it != fChoicesMap.end ()) {
    result = (*it).second;
  }

  else {
    stringstream s;

    s <<
      "choice \"" << name <<
      "\" is unknown in choices table";

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

S_ischemeChoice ischemeChoicesTable::fetchChoiceByNameNonConst (
  const string& name,
  ischemeDriver&   drv)
{
  S_ischemeChoice result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLogStream <<
      "====> Fetching choice named \"" <<
      name <<
      "\" in choices table (non-const)" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this choiceName in the choices map?
  map<string, S_ischemeChoice>::const_iterator
    it =
      fChoicesMap.find (name);

  if (it != fChoicesMap.end ()) {
    result = (*it).second;
  }

  else {
    stringstream s;

    s <<
      "choice \"" << name <<
      "\" is unknown in choices table (non-const)";

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

string ischemeChoicesTable::asString () const
{
  stringstream s;

  s <<
    "ischemeChoicesTable [" <<
    ']';

  return s.str ();
}

void ischemeChoicesTable::displayChoicesMap (ostream& os) const // useless ??? JMI
{
  os <<
    "ischemeChoicesTable [" <<
    endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (pair<string, S_ischemeChoice> thePair : fChoicesMap) {
      S_ischemeChoice choice = thePair.second;

      choice->
        displayChoiceLabelsToOptionsBlocksMap (os);
    } // for

    --gIndenter;
  }

  os << ']' << endl;
}

void ischemeChoicesTable::print (ostream& os) const
{
  os <<
    "ischemeChoicesTable [" <<
    endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (pair<string, S_ischemeChoice> thePair : fChoicesMap) {
      S_ischemeChoice choice = thePair.second;

      os << choice;
    } // for

    --gIndenter;
  }

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_ischemeChoicesTable& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_ischemeCaseChoiceStatement ischemeCaseChoiceStatement::create (
  S_ischemeChoice caseChoice,
  ischemeDriver&  drv)
{
  ischemeCaseChoiceStatement* o =
    new ischemeCaseChoiceStatement (
      caseChoice,
      drv);
  assert (o != nullptr);
  return o;
}

ischemeCaseChoiceStatement::ischemeCaseChoiceStatement (
  S_ischemeChoice caseChoice,
  ischemeDriver&  drv)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    caseChoice != nullptr,
    "caseChoice is null");

  fCaseChoice = caseChoice;

  if (gGlobalIschemeInterpreterOahGroup->getTraceCaseChoiceStatements ()) {
    gLogStream <<
      "====> Creating case statement" <<
      ", caseChoice: [" <<
      fCaseChoice->asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // all labels are initially unused
  for (string label : fCaseChoice->getLabelsSet ()) {
    fCaseUnusedLabels.insert (label);
  }
}

ischemeCaseChoiceStatement::~ischemeCaseChoiceStatement ()
{}

void ischemeCaseChoiceStatement::registerCaseChoiceLabel (
  const string& label,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceCaseChoiceStatements ()) {
    gLogStream <<
      "====> Registering case label \"" <<
      label <<
      "\" in case statement: [" <<
      asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  set<string>
    choiceLabelSet =
      fCaseChoice->
        getLabelsSet ();

  if (! mfStringIsInStringSet (label, choiceLabelSet)) {
    // this label has already been registered in this choice
    stringstream s;

    s <<
      "label '" <<
      label <<
      "' is not known in choice \"" <<
      fCaseChoice->getChoiceName () <<
      ", the labels are: " <<
      fCaseChoice->labelsSetAsString () <<
      ", line " << drv.getScannerLocation ();

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  if (mfStringIsInStringSet (label, fCaseLabelsSet)) {
    // this label has already been registered
    stringstream s;

    s <<
      "choice label \"" <<
      label <<
      "\" occurs more that once in this case \"" <<
      fCaseChoice->getChoiceName () <<
      "\" statement";

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  // register label as a case label set
  fCaseLabelsSet.insert (label);

  fUsedLabels.insert (label);

  // register label in the current labels list
  fCaseCurrentLabelsList.push_back (label);

  // label is no longer unused
  fCaseUnusedLabels.erase (label);
}

void ischemeCaseChoiceStatement::checkThatAllLabelsHaveBeenUsed (
  ischemeDriver& drv)
{
  if (fCaseUnusedLabels.size ()) {
    stringstream s;

    s <<
      "The following labels for choice \"" <<
      fCaseChoice->getChoiceName () <<
      "\" have not been used in this case statement: " <<
      mfStringSetAsString (fCaseUnusedLabels);

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }
}

string ischemeCaseChoiceStatement::asString () const
{
  stringstream s;

  s <<
    "ischemeCaseChoiceStatement [" <<
    fCaseChoice->asString () <<
    ']';

  return s.str ();
}

string ischemeCaseChoiceStatement::currentLabelsListAsString () const
{
  stringstream s;

  s << "[Labels:";

  for (string label : fCaseCurrentLabelsList ) {
    s << ' ' << label;
  } // for

  s << ']';

  return s.str ();
}

void ischemeCaseChoiceStatement::print (ostream& os) const
{
  os <<
    "ischemeCaseChoiceStatement [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "fCaseChoice " << " : " <<
    endl;

  ++gIndenter;
  os <<
    fCaseChoice;
  --gIndenter;

  os <<
    setw (fieldWidth) <<
    "fCaseUnusedLabels" << " : " <<
    endl;

  ++gIndenter;
  os <<
    mfStringSetAsString (fCaseUnusedLabels) <<
    endl;
  --gIndenter;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_ischemeCaseChoiceStatement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_ischemeCaseInputStatement ischemeCaseInputStatement::create (
  S_ischemeInput caseInput,
  ischemeDriver& drv)
{
  ischemeCaseInputStatement* o =
    new ischemeCaseInputStatement (
      caseInput,
      drv);
  assert (o != nullptr);
  return o;
}

ischemeCaseInputStatement::ischemeCaseInputStatement (
  S_ischemeInput caseInput,
  ischemeDriver& drv)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    caseInput != nullptr,
    "caseInput is null");

  fCaseInputInput = caseInput;

  if (gGlobalIschemeInterpreterOahGroup->getTraceCaseInputStatements ()) {
    gLogStream <<
      "====> Creating case statement" <<
      ", caseInput: [" <<
      fCaseInputInput->asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // all inputs are initially unused
  for (string name : fCaseInputInput->getNamesSet ()) {
    fCaseInputUnusedNames.insert (name);
  }
}

ischemeCaseInputStatement::~ischemeCaseInputStatement ()
{}

void ischemeCaseInputStatement::registerCaseInputName (
  const string& name,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceCaseInputStatements ()) {
    gLogStream <<
      "====> Registering case name \"" <<
      name <<
      "\" in case statement: [" <<
      asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  set<string>
    choiceNameSet =
      fCaseInputInput->
        getNamesSet ();

  if (! mfStringIsInStringSet (name, choiceNameSet)) {
    // this name has already been registered in this choice
    stringstream s;

    s <<
      "name '" <<
      name <<
      "' is not known in input \"" <<
      fCaseInputInput->getInputName () <<
      ", the names are: " <<
      fCaseInputInput->namesSetAsString () <<
      ", line " << drv.getScannerLocation ();

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  if (mfStringIsInStringSet (name, fCaseInputNamesSet)) {
    // this name has already been registered
    stringstream s;

    s <<
      "input name \"" <<
      name <<
      "\" occurs more that once in this case \"" <<
      fCaseInputInput->getInputName () <<
      "\" statement";

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }

  // register name as a case name set
  fCaseInputNamesSet.insert (name);

  fUsedNames.insert (name);

  // register name in the current names list
  fCaseInputCurrentNamesList.push_back (name);

  // name is no longer unused
  fCaseInputUnusedNames.erase (name);
}

void ischemeCaseInputStatement::checkThatAllNamesHaveBeenUsed (
  ischemeDriver& drv)
{
  if (fCaseInputUnusedNames.size ()) {
    stringstream s;

    s <<
      "The following names for input \"" <<
      fCaseInputInput->getInputName () <<
      "\" have not been used in this case statement: " <<
      mfStringSetAsString (fCaseInputUnusedNames);

    ischemeError (
      s.str (),
      drv.getScannerLocation ());
  }
}

string ischemeCaseInputStatement::asString () const
{
  stringstream s;

  s <<
    "ischemeCaseInputStatement [" <<
    fCaseInputInput->asString () <<
    ']';

  return s.str ();
}

string ischemeCaseInputStatement::currentNamesListAsString () const
{
  stringstream s;

  s << "[Inputs:";

  for (string name : fCaseInputCurrentNamesList ) {
    s << ' ' << name;
  } // for

  s << ']';

  return s.str ();
}

void ischemeCaseInputStatement::print (ostream& os) const
{
  os <<
    "ischemeCaseInputStatement [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "fCaseInputInput " << " : " <<
    endl;

  ++gIndenter;
  os <<
    fCaseInputInput;
  --gIndenter;

  os <<
    setw (fieldWidth) <<
    "fCaseInputUnusedNames" << " : " <<
    endl;

  ++gIndenter;
  os <<
    mfStringSetAsString (fCaseInputUnusedNames) <<
    endl;
  --gIndenter;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_ischemeCaseInputStatement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

// //______________________________________________________________________________
// void initializeiSchemeBasicTypes ()
// {
//   // protect library against multiple initializations
//   static Bool pPrivateThisMethodHasBeenRun (false);
//
//   if (! pPrivateThisMethodHasBeenRun) {
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalOahEarlyOptions.getEarlyTracingOah () && ! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
//       gLogStream <<
//         "Initializing iScheme basic types handling" <<
//         endl;
//     }
// #endif
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
