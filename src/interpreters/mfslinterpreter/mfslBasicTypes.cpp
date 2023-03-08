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

#include "mfStaticSettings.h"

#include "oahOah.h"

#include "mfslBasicTypes.h"
#include "mfslDriver.h"

#include "mfslInterpreterOah.h"

#include "mfslWae.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
S_mfslOptionsBlock mfslOptionsBlock::create (
  const std::string& optionsBlockName)
{
  mfslOptionsBlock* obj =
    new mfslOptionsBlock (
      optionsBlockName);
  assert (obj != nullptr);
  return obj;
}

mfslOptionsBlock::mfslOptionsBlock (
  const std::string& optionsBlockName)
{
  fOptionsBlockName = optionsBlockName;
}

mfslOptionsBlock::~mfslOptionsBlock ()
{}

void mfslOptionsBlock::registerOptionsInOptionsBlock (
  S_oahOption option,
  mfslDriver& drv)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceOptionsBlocks ()) {
    gLog <<
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

std::string mfslOptionsBlock::asString () const
{
  stringstream s;

  s <<
    "mfslOptionsBlock [" <<
    "fOptionsBlockName: " << fOptionsBlockName <<
    "]";

  return s.str ();
}

std::string mfslOptionsBlock::asOptionsString () const
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

void mfslOptionsBlock::print (std::ostream& os) const
{
  os <<
    "mfslOptionsBlock [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 34;

  os << left <<
    setw (fieldWidth) <<
    "fOptionsBlockName" << ": " << fOptionsBlockName <<
    endl;

  // print the options blocks options name and value vector
  gLog <<
    "fOptionsBlockOptionsVector" << ": " <<
    endl;

  ++gIndenter;

  for (S_oahOption option : fOptionsBlockOptionsVector) {
    gLog << option;
  } // for

  --gIndenter;

  --gIndenter;

  os << "]" << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mfslOptionsBlock& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
std::string mfslInputNameKindAsString (
  mfslInputNameKind inputNameKind)
{
  std::string result;

  switch (inputNameKind) {
    case mfslInputNameKind::kInputNameNone:
      result = "kInputNameNone";
      break;
    case mfslInputNameKind::kInputNameOptionSupplied:
      result = "kInputNameOptionSupplied";
      break;
    case mfslInputNameKind::kInputNameSetInScript:
      result = "kInputNameSetInScript";
      break;
  } // switch

  return result;
}

std::ostream& operator<< (std::ostream& os, const mfslInputNameKind& elt)
{
  os << mfslInputNameKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_mfslInput mfslInput::create (
  const std::string& inputName)
{
  mfslInput* obj =
    new mfslInput (
      inputName);
  assert (obj != nullptr);
  return obj;
}

mfslInput::mfslInput (
  const std::string& inputName)
{
  fInputName = inputName;

  fInputNameKind =
    mfslInputNameKind::kInputNameNone;
}

mfslInput::~mfslInput ()
{}

void mfslInput::setInputNameSuppliedByAnOption (
  const std::string& name,
  mfslDriver&        drv)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
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

void mfslInput::selectInputName (
  const std::string& name,
  mfslDriver&        drv)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Setting the name of input \"" <<
      fInputName <<
      "\" to \"" <<
      name <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this name in the inputs names set?
  std::set<std::string>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

//   // did fInputName get a name by an option?
//   const std::multimap<std::string, std::string>&
//     setInputToNameMultiMap =
//       gGlobalMfslInterpreterOahGroup->
//         getSelectInputToNamesMultiMap ();
//
//   switch (fInputNameKind) {
//     case mfslInputNameKind::kInputNameNone:
//       {
//         // store into the input
//         std::string optionSuppliedName;
//
//         Bool
//           inputGotAValueByAnOption =
//             mfKeyValuePairIsInStringToStringMultiMap (
//               fInputName,
//               setInputToNameMultiMap,
//               optionSuppliedName);
//
//         if (inputGotAValueByAnOption) {
//           if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
//             gLog <<
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
//             mfslInputNameKind::kInputNameOptionSupplied;
//
//           drv.registerOptionsSuppliedInputsAsUsed (
//             fInputName);
//         }
//
//         else {
//           fInputName = optionSuppliedName;
//           fInputNameKind =
//             mfslInputNameKind::kInputNameSetInScript;
//         }
//       }
//       break;
//
//     case mfslInputNameKind::kInputNameOptionSupplied:
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
//         mfslWarning (
//           s.str (),
//           drv.getScannerLocation ());
//       }
//       break;
//
//     case mfslInputNameKind::kInputNameSetInScript:
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
//         mfslError (
//           s.str (),
//           drv.getScannerLocation ());
//       }
//       break;
//   } // switch
}

std::string mfslInput::getInputName (mfslDriver& drv) const
{
  switch (fInputNameKind) {
    case mfslInputNameKind::kInputNameNone:
      {
        stringstream s;

        s <<
          "input \"" <<
          fInputName <<
          "\" has not got any name";

        mfslError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case mfslInputNameKind::kInputNameOptionSupplied:
      if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
        gLog <<
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

    case mfslInputNameKind::kInputNameSetInScript:
      if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
        gLog <<
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

std::string mfslInput::getInputNameWithoutTrace (mfslDriver& drv) const
{
  switch (fInputNameKind) {
    case mfslInputNameKind::kInputNameNone:
      {
        stringstream s;

        s <<
          "input \"" <<
          fInputName <<
          "\" has not got any name";

        mfslError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case mfslInputNameKind::kInputNameOptionSupplied:
    case mfslInputNameKind::kInputNameSetInScript:
      break;
  } // switch

  return fInputName;
}

S_mfslOptionsBlock mfslInput::getInputOptionsBlockForName (
  const std::string& name,
  mfslDriver&        drv) const
{
  S_mfslOptionsBlock result;

  if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Fetching name \"" <<
      name <<
      "\" in input \"" <<
      fInputName <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  map<std::string, S_mfslOptionsBlock>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "<==== Result: \"" <<
      result <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  return result;
}

void mfslInput::addName (
  const std::string& name,
  mfslDriver&        drv)
{
  // is this name in the inputs names set?
  std::set<std::string>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  else {
    if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
      gLog <<
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
      mfslOptionsBlock::create (
        name);
  }
}

void mfslInput::enrichNameOptionsBlock (
  const std::string& name,
  S_mfslOptionsBlock optionsBlock,
  mfslDriver&        drv)
{
  S_mfslOptionsBlock
    nameOptionsBlock =
      fInputNamesToOptionsBlocksMap [
        name];

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    nameOptionsBlock != nullptr,
    "nameOptionsBlock is null");

  if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
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

std::string mfslInput::namesSetAsString () const
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
      std::set<std::string>::const_iterator i =
        fNamesSet.begin ();
      i != fNamesSet.end ();
      ++i
    ) {
      std::string theString = (*i);

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

std::string mfslInput::asString () const
{
  stringstream s;

  s <<
    "mfslInput [" <<
    "fInputName: " << fInputName <<
    ", " <<

    "fInputName: [" << fInputName << "]" <<
    "]";

  return s.str ();
}

void mfslInput::displayInputNamesToOptionsBlocksMap (std::ostream& os) const
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
    for (pair<std::string, S_mfslOptionsBlock> thePair : fInputNamesToOptionsBlocksMap) {
      std::string        key = thePair.first;
      S_mfslOptionsBlock optionsBlock = thePair.second;
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
  os << "]" << endl;
}

void mfslInput::print (std::ostream& os) const
{
  os <<
    "mfslInput [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 19;

  os << left <<
    setw (fieldWidth) <<
    "fInputName" << " : \"" << fInputName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fInputNameKind" << ": " << fInputNameKind << "" <<
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
    for (std::string name : fNamesSet) {
      os << name;

      if (++counter < namesSetSize) {
        os << " | ";
      }
    } // for
  }
  os << "]" << endl;

  size_t
    inputNamesToOptionsBlocksMapSize =
      fNamesSet.size ();

  os <<
    setw (fieldWidth) <<
    "fInputNamesToOptionsBlocksMap" << " [" <<
    endl;

  if (inputNamesToOptionsBlocksMapSize) {
//     int counter = 0;
    for (pair<std::string, S_mfslOptionsBlock> thePair : fInputNamesToOptionsBlocksMap) {
      std::string        key = thePair.first;
      S_mfslOptionsBlock optionsBlock = thePair.second;
      os <<
        key << ": " <<
        endl;

      ++gIndenter;

      if (optionsBlock) {
        os <<
          optionsBlock;
      }
      else {
        os << "none" << endl;
      }

      --gIndenter;

//       if (++counter < InputNamesToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << "]" << endl;

  --gIndenter;

  os << "]" << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mfslInput& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_mfslInputsTable mfslInputsTable::create ()
{
  mfslInputsTable* obj =
    new mfslInputsTable ();
  assert (obj != nullptr);
  return obj;
}

mfslInputsTable::mfslInputsTable ()
{}

mfslInputsTable::~mfslInputsTable ()
{}

void mfslInputsTable::addInput (
  S_mfslInput input,
  mfslDriver& drv)
{
  std::string
    inputName =
      input->getInputName ();

  // is this inputName in the inputs names set?
  map<std::string, S_mfslInput>::const_iterator
    it =
      fInputsMap.find (
        inputName);

  if (it != fInputsMap.end ()) {
    stringstream s;

    s <<
      "input \"" <<
      inputName <<
      "\" occurs more that once in the inputs table";

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  fInputsMap [inputName] = input;
}

// S_mfslInput mfslInputsTable::lookupInputByName (
//   const std::string& name)
// {
//   S_mfslInput result;
//
//   // is this inputName in the inputs names set?
//   map<std::string, S_mfslInput>::const_iterator
//     it =
//       fInputsMap.find (name);
//
//   if (it != fInputsMap.end ()) {
//     result = (*it).second;
//   }
//
//   return result;
// }

S_mfslInput mfslInputsTable::fetchInputByName (
  const std::string& name,
  const mfslDriver&  drv)
{
  S_mfslInput result;

  if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Fetching input named \"" <<
      name <<
      "\" in inputs table" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this inputName in the inputs names set?
  map<std::string, S_mfslInput>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

S_mfslInput mfslInputsTable::fetchInputByNameNonConst (
  const std::string& name,
  mfslDriver&        drv)
{
  S_mfslInput result;

  if (gGlobalMfslInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Fetching input named \"" <<
      name <<
      "\" in inputs table (non-const)" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this inputName in the inputs map?
  map<std::string, S_mfslInput>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

std::string mfslInputsTable::asString () const
{
  stringstream s;

  s <<
    "mfslInputsTable [" <<
    "]";

  return s.str ();
}

void mfslInputsTable::displayInputsMap (std::ostream& os) const // useless ??? JMI
{
  os <<
    "mfslInputsTable [" <<
    endl;

  if (fInputsMap.size ()) {
    ++gIndenter;

    for (pair<std::string, S_mfslInput> thePair : fInputsMap) {
      S_mfslInput input = thePair.second;

      input->
        displayInputNamesToOptionsBlocksMap (os);
    } // for

    --gIndenter;
  }

  os << "]" << endl;
}

void mfslInputsTable::print (std::ostream& os) const
{
  os <<
    "mfslInputsTable [" <<
    endl;

  if (fInputsMap.size ()) {
    ++gIndenter;

    for (pair<std::string, S_mfslInput> thePair : fInputsMap) {
      S_mfslInput input = thePair.second;

      os << input;
    } // for

    --gIndenter;
  }

  os << "]" << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mfslInputsTable& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
std::string mfslChoiceLabelKindAsString (
  mfslChoiceLabelKind inputNameKind)
{
  std::string result;

  switch (inputNameKind) {
    case mfslChoiceLabelKind::kChoiceLabelNone:
      result = "kChoiceLabelNone";
      break;
    case mfslChoiceLabelKind::kChoiceLabelOptionSupplied:
      result = "kChoiceLabelOptionSupplied";
      break;
    case mfslChoiceLabelKind::kChoiceLabelSetInScript:
      result = "kChoiceLabelSetInScript";
      break;
  } // switch

  return result;
}

std::ostream& operator<< (std::ostream& os, const mfslChoiceLabelKind& elt)
{
  os << mfslChoiceLabelKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_mfslChoice mfslChoice::create (
  const std::string& choiceName)
{
  mfslChoice* obj =
    new mfslChoice (
      choiceName);
  assert (obj != nullptr);
  return obj;
}

mfslChoice::mfslChoice (
  const std::string& choiceName)
{
  fChoiceName = choiceName;

  fChoiceLabelKind =
    mfslChoiceLabelKind::kChoiceLabelNone;
}

mfslChoice::~mfslChoice ()
{}

void mfslChoice::setChoiceLabelSuppliedByAnOption (
  const std::string& label,
  mfslDriver&   drv)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
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

void mfslChoice::selectChoiceLabel (
  const std::string& label,
  mfslDriver&   drv)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Setting the label of choice \"" <<
      fChoiceName <<
      "\" to \"" <<
      label <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this label in the choices labels set?
  std::set<std::string>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  // did fChoiceName get a label by an option?
  const std::multimap<std::string, std::string>&
    setChoiceToLabelMultiMap =
      gGlobalMfslInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMap ();

  switch (fChoiceLabelKind) {
    case mfslChoiceLabelKind::kChoiceLabelNone:
      {
        // store into the choice
        std::string optionSuppliedLabel;

        Bool
          choiceGotAValueByAnOption =
            mfKeyValuePairIsInStringToStringMultiMap (
              fChoiceName,
              setChoiceToLabelMultiMap,
              optionSuppliedLabel);

        if (choiceGotAValueByAnOption) {
          if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
            gLog <<
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
            mfslChoiceLabelKind::kChoiceLabelOptionSupplied;

          drv.registerOptionsSuppliedChoicesAsUsed (
            fChoiceName);
        }

        else {
          fChoiceLabel = optionSuppliedLabel;
          fChoiceLabelKind =
            mfslChoiceLabelKind::kChoiceLabelSetInScript;
        }
      }
      break;

    case mfslChoiceLabelKind::kChoiceLabelOptionSupplied:
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

        mfslWarning (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case mfslChoiceLabelKind::kChoiceLabelSetInScript:
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

        mfslError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;
  } // switch
}

std::string mfslChoice::getChoiceLabel (mfslDriver& drv) const
{
  switch (fChoiceLabelKind) {
    case mfslChoiceLabelKind::kChoiceLabelNone:
      {
        stringstream s;

        s <<
          "choice \"" <<
          fChoiceName <<
          "\" has not got any label";

        mfslError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case mfslChoiceLabelKind::kChoiceLabelOptionSupplied:
      if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
        gLog <<
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

    case mfslChoiceLabelKind::kChoiceLabelSetInScript:
      if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
        gLog <<
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

std::string mfslChoice::getChoiceLabelWithoutTrace (mfslDriver& drv) const
{
  switch (fChoiceLabelKind) {
    case mfslChoiceLabelKind::kChoiceLabelNone:
      {
        stringstream s;

        s <<
          "choice \"" <<
          fChoiceName <<
          "\" has not got any label";

        mfslError (
          s.str (),
          drv.getScannerLocation ());
      }
      break;

    case mfslChoiceLabelKind::kChoiceLabelOptionSupplied:
    case mfslChoiceLabelKind::kChoiceLabelSetInScript:
      break;
  } // switch

  return fChoiceLabel;
}

S_mfslOptionsBlock mfslChoice::getChoiceOptionsBlockForLabel (
  const std::string& label,
  mfslDriver&        drv) const
{
  S_mfslOptionsBlock result;

  if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Fetching label \"" <<
      label <<
      "\" in choice \"" <<
      fChoiceName <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  map<std::string, S_mfslOptionsBlock>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "<==== Result: \"" <<
      result <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  return result;
}

void mfslChoice::addLabel (
  const std::string& label,
  mfslDriver&        drv)
{
  // is this label in the choices labels set?
  std::set<std::string>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  else {
    if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
      gLog <<
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
      mfslOptionsBlock::create (
        label);
  }
}

void mfslChoice::enrichLabelOptionsBlock (
  const std::string& label,
  S_mfslOptionsBlock optionsBlock,
  mfslDriver&        drv)
{
  S_mfslOptionsBlock
    labelOptionsBlock =
      fChoiceLabelsToOptionsBlocksMap [
        label];

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    labelOptionsBlock != nullptr,
    "labelOptionsBlock is null");

  if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
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

void mfslChoice::registerChoiceDefaultLabel (
  const std::string& label,
  mfslDriver&        drv)
{
  if (! mfStringIsInStringSet (label, fLabelsSet)) {
    stringstream s;

    s <<
      "choice label \"" <<
      label <<
      "\" is not present in choice \"" <<
      fChoiceName <<
      "\"";

    mfslError (
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
//     mfslInternalError (
//       s.str (),
//       drv.getScannerLocation ());
//   }

  fChoiceDefaultLabel = label;
}

std::string mfslChoice::labelsSetAsString () const
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
      std::set<std::string>::const_iterator i =
        fLabelsSet.begin ();
      i != fLabelsSet.end ();
      ++i
    ) {
      std::string theString = (*i);

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

std::string mfslChoice::asString () const
{
  stringstream s;

  s <<
    "mfslChoice [" <<
    "fChoiceName: " << fChoiceName <<
    ", " <<

    "fChoiceLabel: [" << fChoiceLabel << "]" <<
    "]";

  return s.str ();
}

void mfslChoice::displayChoiceLabelsToOptionsBlocksMap (std::ostream& os) const
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
    for (pair<std::string, S_mfslOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      std::string        key = thePair.first;
      S_mfslOptionsBlock optionsBlock = thePair.second;
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
  os << "]" << endl;
}

void mfslChoice::print (std::ostream& os) const
{
  os <<
    "mfslChoice [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 19;

  os << left <<
    setw (fieldWidth) <<
    "fChoiceName" << " : \"" << fChoiceName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fChoiceLabelKind" << ": " << fChoiceLabelKind << "" <<
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
    for (std::string label : fLabelsSet) {
      os << label;

      if (++counter < labelsSetSize) {
        os << " | ";
      }
    } // for
  }
  os << "]" << endl;

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
    for (pair<std::string, S_mfslOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      std::string        key = thePair.first;
      S_mfslOptionsBlock optionsBlock = thePair.second;
      os <<
        key << ": " <<
        endl;

      ++gIndenter;

      if (optionsBlock) {
        os <<
          optionsBlock;
      }
      else {
        os << "none" << endl;
      }

      --gIndenter;

//       if (++counter < ChoiceLabelsToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << "]" << endl;

  --gIndenter;

  os << "]" << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mfslChoice& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_mfslChoicesTable mfslChoicesTable::create ()
{
  mfslChoicesTable* obj =
    new mfslChoicesTable ();
  assert (obj != nullptr);
  return obj;
}

mfslChoicesTable::mfslChoicesTable ()
{}

mfslChoicesTable::~mfslChoicesTable ()
{}

void mfslChoicesTable::addChoice (
  S_mfslChoice choice,
  mfslDriver&  drv)
{
  std::string
    choiceName =
      choice->getChoiceName ();

  // is this choiceName in the choices labels set?
  map<std::string, S_mfslChoice>::const_iterator
    it =
      fChoicesMap.find (
        choiceName);

  if (it != fChoicesMap.end ()) {
    stringstream s;

    s <<
      "choice \"" <<
      choiceName <<
      "\" occurs more that once in the choices table";

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  fChoicesMap [choiceName] = choice;
}

// S_mfslChoice mfslChoicesTable::lookupChoiceByName (
//   const std::string& name)
// {
//   S_mfslChoice result;
//
//   // is this choiceName in the choices labels set?
//   map<std::string, S_mfslChoice>::const_iterator
//     it =
//       fChoicesMap.find (name);
//
//   if (it != fChoicesMap.end ()) {
//     result = (*it).second;
//   }
//
//   return result;
// }

S_mfslChoice mfslChoicesTable::fetchChoiceByName (
  const std::string& name,
  const mfslDriver&  drv)
{
  S_mfslChoice result;

  if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Fetching choice named \"" <<
      name <<
      "\" in choices table" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this choiceName in the choices labels set?
  map<std::string, S_mfslChoice>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

S_mfslChoice mfslChoicesTable::fetchChoiceByNameNonConst (
  const std::string& name,
  mfslDriver&        drv)
{
  S_mfslChoice result;

  if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Fetching choice named \"" <<
      name <<
      "\" in choices table (non-const)" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this choiceName in the choices map?
  map<std::string, S_mfslChoice>::const_iterator
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

std::string mfslChoicesTable::asString () const
{
  stringstream s;

  s <<
    "mfslChoicesTable [" <<
    "]";

  return s.str ();
}

void mfslChoicesTable::displayChoicesMap (std::ostream& os) const // useless ??? JMI
{
  os <<
    "mfslChoicesTable [" <<
    endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (pair<std::string, S_mfslChoice> thePair : fChoicesMap) {
      S_mfslChoice choice = thePair.second;

      choice->
        displayChoiceLabelsToOptionsBlocksMap (os);
    } // for

    --gIndenter;
  }

  os << "]" << endl;
}

void mfslChoicesTable::print (std::ostream& os) const
{
  os <<
    "mfslChoicesTable [" <<
    endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (pair<std::string, S_mfslChoice> thePair : fChoicesMap) {
      S_mfslChoice choice = thePair.second;

      os << choice;
    } // for

    --gIndenter;
  }

  os << "]" << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mfslChoicesTable& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_mfslCaseChoiceStatement mfslCaseChoiceStatement::create (
  S_mfslChoice caseChoice,
  mfslDriver&  drv)
{
  mfslCaseChoiceStatement* obj =
    new mfslCaseChoiceStatement (
      caseChoice,
      drv);
  assert (obj != nullptr);
  return obj;
}

mfslCaseChoiceStatement::mfslCaseChoiceStatement (
  S_mfslChoice caseChoice,
  mfslDriver&  drv)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    caseChoice != nullptr,
    "caseChoice is null");

  fCaseChoice = caseChoice;

  if (gGlobalMfslInterpreterOahGroup->getTraceCaseChoiceStatements ()) {
    gLog <<
      "====> Creating case statement" <<
      ", caseChoice: [" <<
      fCaseChoice->asString () <<
      "]" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // all labels are initially unused
  for (std::string label : fCaseChoice->getLabelsSet ()) {
    fCaseUnusedLabels.insert (label);
  }
}

mfslCaseChoiceStatement::~mfslCaseChoiceStatement ()
{}

void mfslCaseChoiceStatement::registerCaseChoiceLabel (
  const std::string& label,
  mfslDriver&        drv)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceCaseChoiceStatements ()) {
    gLog <<
      "====> Registering case label \"" <<
      label <<
      "\" in case statement: [" <<
      asString () <<
      "]" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  std::set<std::string>
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

    mfslError (
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

    mfslError (
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

void mfslCaseChoiceStatement::checkThatAllLabelsHaveBeenUsed (
  mfslDriver& drv)
{
  if (fCaseUnusedLabels.size ()) {
    stringstream s;

    s <<
      "The following labels for choice \"" <<
      fCaseChoice->getChoiceName () <<
      "\" have not been used in this case statement: " <<
      mfStringSetAsString (fCaseUnusedLabels);

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }
}

std::string mfslCaseChoiceStatement::asString () const
{
  stringstream s;

  s <<
    "mfslCaseChoiceStatement [" <<
    fCaseChoice->asString () <<
    "]";

  return s.str ();
}

std::string mfslCaseChoiceStatement::currentLabelsListAsString () const
{
  stringstream s;

  s << "[Labels:";

  for (std::string label : fCaseCurrentLabelsList ) {
    s << ' ' << label;
  } // for

  s << ']';

  return s.str ();
}

void mfslCaseChoiceStatement::print (std::ostream& os) const
{
  os <<
    "mfslCaseChoiceStatement [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "fCaseChoice " << ": " <<
    endl;

  ++gIndenter;
  os <<
    fCaseChoice;
  --gIndenter;

  os <<
    setw (fieldWidth) <<
    "fCaseUnusedLabels" << ": " <<
    endl;

  ++gIndenter;
  os <<
    mfStringSetAsString (fCaseUnusedLabels) <<
    endl;
  --gIndenter;

  --gIndenter;

  os << "]" << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mfslCaseChoiceStatement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_mfslCaseInputStatement mfslCaseInputStatement::create (
  S_mfslInput caseInput,
  mfslDriver& drv)
{
  mfslCaseInputStatement* obj =
    new mfslCaseInputStatement (
      caseInput,
      drv);
  assert (obj != nullptr);
  return obj;
}

mfslCaseInputStatement::mfslCaseInputStatement (
  S_mfslInput caseInput,
  mfslDriver& drv)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    caseInput != nullptr,
    "caseInput is null");

  fCaseInputInput = caseInput;

  if (gGlobalMfslInterpreterOahGroup->getTraceCaseInputStatements ()) {
    gLog <<
      "====> Creating case statement" <<
      ", caseInput: [" <<
      fCaseInputInput->asString () <<
      "]" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // all inputs are initially unused
  for (std::string name : fCaseInputInput->getNamesSet ()) {
    fCaseInputUnusedNames.insert (name);
  }
}

mfslCaseInputStatement::~mfslCaseInputStatement ()
{}

void mfslCaseInputStatement::registerCaseInputName (
  const std::string& name,
  mfslDriver&        drv)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceCaseInputStatements ()) {
    gLog <<
      "====> Registering case name \"" <<
      name <<
      "\" in case statement: [" <<
      asString () <<
      "]" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  std::set<std::string>
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

    mfslError (
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

    mfslError (
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

void mfslCaseInputStatement::checkThatAllNamesHaveBeenUsed (
  mfslDriver& drv)
{
  if (fCaseInputUnusedNames.size ()) {
    stringstream s;

    s <<
      "The following names for input \"" <<
      fCaseInputInput->getInputName () <<
      "\" have not been used in this case statement: " <<
      mfStringSetAsString (fCaseInputUnusedNames);

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }
}

std::string mfslCaseInputStatement::asString () const
{
  stringstream s;

  s <<
    "mfslCaseInputStatement [" <<
    fCaseInputInput->asString () <<
    "]";

  return s.str ();
}

std::string mfslCaseInputStatement::currentNamesListAsString () const
{
  stringstream s;

  s << "[Inputs:";

  for (std::string name : fCaseInputCurrentNamesList ) {
    s << ' ' << name;
  } // for

  s << ']';

  return s.str ();
}

void mfslCaseInputStatement::print (std::ostream& os) const
{
  os <<
    "mfslCaseInputStatement [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "fCaseInputInput " << ": " <<
    endl;

  ++gIndenter;
  os <<
    fCaseInputInput;
  --gIndenter;

  os <<
    setw (fieldWidth) <<
    "fCaseInputUnusedNames" << ": " <<
    endl;

  ++gIndenter;
  os <<
    mfStringSetAsString (fCaseInputUnusedNames) <<
    endl;
  --gIndenter;

  --gIndenter;

  os << "]" << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mfslCaseInputStatement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

// //______________________________________________________________________________
// void initializeMFSLBasicTypes ()
// {
//   // protect library against multiple initializations
//   static Bool pPrivateThisMethodHasBeenRun (false);
//
//   if (! pPrivateThisMethodHasBeenRun) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah () && ! gEarlyOptions.getEarlyQuietOption ()) {
//       gLog <<
//         "Initializing MFSL basic types handling" <<
//         endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
