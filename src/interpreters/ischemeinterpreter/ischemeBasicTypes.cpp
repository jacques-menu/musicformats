/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfBool.h"
#include "mfStringsHandling.h"

#include "mfStaticSettings.h"

#include "oahOah.h"

#include "ischemeBasicTypes.h"
#include "ischemeDriver.h"

#include "ischemeInterpreterOah.h"

#include "ischemeWae.h"


using namespace MusicFormats;

//_______________________________________________________________________________
S_ischemeOptionsBlock ischemeOptionsBlock::create (
  const std::string& optionsBlockName)
{
  ischemeOptionsBlock* o =
    new ischemeOptionsBlock (
      optionsBlockName);
  assert (o != nullptr);
  return o;
}

ischemeOptionsBlock::ischemeOptionsBlock (
  const std::string& optionsBlockName)
{
  fOptionsBlockName = optionsBlockName;
}

ischemeOptionsBlock::~ischemeOptionsBlock ()
{}

void ischemeOptionsBlock::registerOptionsInOptionsBlock (
  const S_oahOption& option,
  ischemeDriver& drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceOptionsBlocks ()) {
    gLog <<
      "====> Registering option [" <<
      option->asString () <<
      "] in options block \"" <<
      fOptionsBlockName <<
      "\", line " << drv.getScannerLocation () <<
      std::endl;
  }

  fOptionsBlockOptionsVector.push_back (
    option);
}

std::string ischemeOptionsBlock::asString () const
{
  std::stringstream ss;

  ss <<
    "ischemeOptionsBlock [" <<
    "fOptionsBlockName: " << fOptionsBlockName <<
    ']';

  return ss.str ();
}

std::string ischemeOptionsBlock::asOptionsString () const
{
  std::stringstream ss;

  if (fOptionsBlockOptionsVector.size ()) {
    std::vector<S_oahOption>::const_iterator
      iBegin = fOptionsBlockOptionsVector.begin (),
      iEnd   = fOptionsBlockOptionsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahOption option = (*i);

      ss <<
        option->
          asStringForCommandLine ();

      if (++i == iEnd) break;
      ss << ' ';
    } // for
  }

  return ss.str ();
}

void ischemeOptionsBlock::print (std::ostream& os) const
{
  os <<
    "ischemeOptionsBlock [" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 34;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOptionsBlockName" << ": " << fOptionsBlockName <<
    std::endl;

  // print the options blocks options name and value vector
  gLog <<
    "fOptionsBlockOptionsVector" << ": " <<
    std::endl;

  ++gIndenter;

  for (S_oahOption option : fOptionsBlockOptionsVector) {
    gLog << option;
  } // for

  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_ischemeOptionsBlock& elt)
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
std::string ischemeInputNameKindAsString (
  ischemeInputNameKind inputNameKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const ischemeInputNameKind& elt)
{
  os << ischemeInputNameKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_ischemeInput ischemeInput::create (
  const std::string& inputName)
{
  ischemeInput* o =
    new ischemeInput (
      inputName);
  assert (o != nullptr);
  return o;
}

ischemeInput::ischemeInput (
  const std::string& inputName)
{
  fInputName = inputName;

  fInputNameKind =
    ischemeInputNameKind::kInputNameNone;
}

ischemeInput::~ischemeInput ()
{}

void ischemeInput::setInputNameSuppliedByAnOption (
  const std::string& name,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Setting the name of input \"" <<
      fInputName <<
      "\" to \"" <<
      name <<
      "\", supplied by an option" <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // no checks here, the names are not knoown yet
  fInputName = name;
}

void ischemeInput::selectInputName (
  const std::string& name,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Setting the name of input \"" <<
      fInputName <<
      "\" to \"" <<
      name <<
      "\", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // is this name in the inputs names set?
  std::set<std::string>::const_iterator
    it =
      fNamesSet.find (name);

  if (it == fNamesSet.end ()) {
    // no, issue error message
    std::stringstream ss;

    ss <<
      "input \"" <<
      fInputName <<
      "\" cannot be set to name \"" <<
      name <<
      "\", the names are: " <<
      namesSetAsString ();

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

//   // did fInputName get a name by an option?
//   const std::multimap<std::string, std::string>&
//     setInputToNameMultiMap =
//       gGlobalIschemeInterpreterOahGroup->
//         getSelectInputToNamesMultiMap ();
//
//   switch (fInputNameKind) {
//     case ischemeInputNameKind::kInputNameNone:
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
//           if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
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
//               std::endl;
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
//         std::stringstream ss;
//
//         ss <<
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
//           ss.str (),
//           drv.getScannerLocation ());
//       }
//       break;
//
//     case ischemeInputNameKind::kInputNameSetInScript:
//       {
//         // set in script multiple times
//         std::stringstream ss;
//
//         ss <<
//           "input \"" <<
//           fInputName <<
//           "\"" <<
//           " already has name \"" <<
//           fInputName <<
//           "\" supplied to the script";
//
//         ischemeError (
//           ss.str (),
//           drv.getScannerLocation ());
//       }
//       break;
//   } // switch
}

std::string ischemeInput::getInputName (ischemeDriver& drv) const
{
  switch (fInputNameKind) {
    case ischemeInputNameKind::kInputNameNone:
      {
        std::stringstream ss;

        ss <<
          "input \"" <<
          fInputName <<
          "\" has not got any name";

        ischemeError (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;

    case ischemeInputNameKind::kInputNameOptionSupplied:
      if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
        gLog <<
          "====> " <<
          " input \"" <<
          fInputName <<
          ", name " <<
          fInputName <<
          "\", supplied to the script" <<
          ", line " << drv.getScannerLocation () <<
          std::endl;
      }
      break;

    case ischemeInputNameKind::kInputNameSetInScript:
      if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
        gLog <<
          "====>" <<
          "input \"" <<
          fInputName <<
          "\" has name \"" <<
          fInputName <<
          " \", set in the script" <<
          ", line " << drv.getScannerLocation () <<
          std::endl;
      }
      break;
  } // switch

  return fInputName;
}

std::string ischemeInput::getInputNameWithoutTrace (ischemeDriver& drv) const
{
  switch (fInputNameKind) {
    case ischemeInputNameKind::kInputNameNone:
      {
        std::stringstream ss;

        ss <<
          "input \"" <<
          fInputName <<
          "\" has not got any name";

        ischemeError (
          ss.str (),
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
  const std::string& name,
  ischemeDriver&     drv) const
{
  S_ischemeOptionsBlock result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Fetching name \"" <<
      name <<
      "\" in input \"" <<
      fInputName <<
      "\", line " << drv.getScannerLocation () <<
      std::endl;
  }

  std::map<std::string, S_ischemeOptionsBlock>::const_iterator
    it =
      fInputNamesToOptionsBlocksMap.find (
        name);

  if (it != fInputNamesToOptionsBlocksMap.end ()) {
    result = (*it).second;
  }
  else {
    std::stringstream ss;

    ss <<
      "name \"" <<
      name <<
      "\" is not known to input \"" <<
      fInputName <<
      "\", line " << drv.getScannerLocation ();

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "<==== Result: \"" <<
      result <<
      "\", line " << drv.getScannerLocation () <<
      std::endl;
  }

  return result;
}

void ischemeInput::addName (
  const std::string& name,
  ischemeDriver&   drv)
{
  // is this name in the inputs names set?
  std::set<std::string>::const_iterator
    it =
      fNamesSet.find (name);

  if (it != fNamesSet.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "input name \"" <<
      name <<
      "\" occurs more that once in input \"" <<
      fInputName <<
      "\", line " << drv.getScannerLocation ();

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  else {
    if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
      gLog <<
        "====> Adding name \"" <<
        name <<
        "\"" <<
        " to input \"" <<
        fInputName <<
        "\"" <<
        ", line " << drv.getScannerLocation () <<
        std::endl;
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
  const std::string&      name,
  const S_ischemeOptionsBlock& optionsBlock,
  ischemeDriver&        drv)
{
  const S_ischemeOptionsBlock&
    nameOptionsBlock =
      fInputNamesToOptionsBlocksMap [
        name];

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    nameOptionsBlock != nullptr,
    "nameOptionsBlock is null");
#endif

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
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
      std::endl;
  }

  std::vector<S_oahOption>&
    nameOptionsBlockOptionsVector =
      nameOptionsBlock->
        getOptionsBlockOptionsVectorNonConst ();

  const std::vector<S_oahOption>&
    optionsBlockOptionsVector =
      optionsBlock->
        getOptionsBlockOptionsVector ();

  optionsNameAndValueVectorsPlusEquals (
    nameOptionsBlockOptionsVector,
    optionsBlockOptionsVector);
}

std::string ischemeInput::namesSetAsString () const
{
  std::stringstream ss;

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
        ss << "\n";
        cumulatedLength = 0;
      }

      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != namesSetSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

std::string ischemeInput::asString () const
{
  std::stringstream ss;

  ss <<
    "ischemeInput [" <<
    "fInputName: " << fInputName <<
    ", " <<

    "fInputName: [" << fInputName << ']' <<
    ']';

  return ss.str ();
}

void ischemeInput::displayInputNamesToOptionsBlocksMap (std::ostream& os) const
{
  size_t
    InputNamesToOptionsBlocksMapSize =
      fNamesSet.size ();

  const int fieldWidth = 19;

  os <<
    std::setw (fieldWidth) <<
    "fInputNamesToOptionsBlocksMap" << " [" <<
    std::endl;

  if (InputNamesToOptionsBlocksMapSize) {
//     int counter = 0;
    for (std::pair<std::string, S_ischemeOptionsBlock> thePair : fInputNamesToOptionsBlocksMap) {
      std::string        key = thePair.first;
      const S_ischemeOptionsBlock& optionsBlock = thePair.second;
      os <<
        key << ": " <<
        std::endl;

      ++gIndenter;

      os <<
        optionsBlock;

      --gIndenter;

//       if (++counter < InputNamesToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << ']' << std::endl;
}

void ischemeInput::print (std::ostream& os) const
{
  os <<
    "ischemeInput [" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 19;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fInputName" << ": \"" << fInputName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fInputNameKind" << ": " << fInputNameKind << "" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fInputName" << ": \"" << fInputName << "\"" <<
    std::endl;

  size_t
    namesSetSize =
      fNamesSet.size ();

  os <<
    std::setw (fieldWidth) <<
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
  os << ']' << std::endl;

  size_t
    inputNamesToOptionsBlocksMapSize =
      fNamesSet.size ();

  os <<
    std::setw (fieldWidth) <<
    "fInputNamesToOptionsBlocksMap" << " [" <<
    std::endl;

  if (inputNamesToOptionsBlocksMapSize) {
//     int counter = 0;
    for (std::pair<std::string, S_ischemeOptionsBlock> thePair : fInputNamesToOptionsBlocksMap) {
      std::string        key = thePair.first;
      const S_ischemeOptionsBlock& optionsBlock = thePair.second;
      os <<
        key << ": " <<
        std::endl;

      ++gIndenter;

      if (optionsBlock) {
        os <<
          optionsBlock;
      }
      else {
        os << "[NONE]" << std::endl;
      }

      --gIndenter;

//       if (++counter < InputNamesToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << ']' << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_ischemeInput& elt)
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
  const S_ischemeInput& input,
  ischemeDriver& drv)
{
  std::string
    inputName =
      input->getInputName ();

  // is this inputName in the inputs names set?
  std::map<std::string, S_ischemeInput>::const_iterator
    it =
      fInputsMap.find (
        inputName);

  if (it != fInputsMap.end ()) {
    std::stringstream ss;

    ss <<
      "input \"" <<
      inputName <<
      "\" occurs more that once in the inputs table";

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  fInputsMap [inputName] = input;
}

// S_ischemeInput ischemeInputsTable::lookupInputByName (
//   const std::string& name)
// {
//   S_ischemeInput result;
//
//   // is this inputName in the inputs names set?
//   std::map<std::string, S_ischemeInput>::const_iterator
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
  const std::string&     name,
  const ischemeDriver& drv)
{
  S_ischemeInput result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Fetching input named \"" <<
      name <<
      "\" in inputs table" <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // is this inputName in the inputs names set?
  std::map<std::string, S_ischemeInput>::const_iterator
    it =
      fInputsMap.find (name);

  if (it != fInputsMap.end ()) {
    result = (*it).second;
  }

  else {
    std::stringstream ss;

    ss <<
      "input \"" << name <<
      "\" is unknown in inputs table";

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  return result;
}

S_ischemeInput ischemeInputsTable::fetchInputByNameNonConst (
  const std::string& name,
  ischemeDriver&   drv)
{
  S_ischemeInput result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Fetching input named \"" <<
      name <<
      "\" in inputs table (non-const)" <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // is this inputName in the inputs map?
  std::map<std::string, S_ischemeInput>::const_iterator
    it =
      fInputsMap.find (name);

  if (it != fInputsMap.end ()) {
    result = (*it).second;
  }

  else {
    std::stringstream ss;

    ss <<
      "input \"" << name <<
      "\" is unknown in inputs table (non-const)";

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  return result;
}

std::string ischemeInputsTable::asString () const
{
  std::stringstream ss;

  ss <<
    "ischemeInputsTable [" <<
    ']';

  return ss.str ();
}

void ischemeInputsTable::displayInputsMap (std::ostream& os) const // useless ??? JMI
{
  os <<
    "ischemeInputsTable [" <<
    std::endl;

  if (fInputsMap.size ()) {
    ++gIndenter;

    for (std::pair<std::string, S_ischemeInput> thePair : fInputsMap) {
      S_ischemeInput input = thePair.second;

      input->
        displayInputNamesToOptionsBlocksMap (os);
    } // for

    --gIndenter;
  }

  os << ']' << std::endl;
}

void ischemeInputsTable::print (std::ostream& os) const
{
  os <<
    "ischemeInputsTable [" <<
    std::endl;

  if (fInputsMap.size ()) {
    ++gIndenter;

    for (std::pair<std::string, S_ischemeInput> thePair : fInputsMap) {
      S_ischemeInput input = thePair.second;

      os << input;
    } // for

    --gIndenter;
  }

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_ischemeInputsTable& elt)
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
std::string ischemeChoiceLabelKindAsString (
  ischemeChoiceLabelKind inputNameKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const ischemeChoiceLabelKind& elt)
{
  os << ischemeChoiceLabelKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_ischemeChoice ischemeChoice::create (
  const std::string& choiceName)
{
  ischemeChoice* o =
    new ischemeChoice (
      choiceName);
  assert (o != nullptr);
  return o;
}

ischemeChoice::ischemeChoice (
  const std::string& choiceName)
{
  fChoiceName = choiceName;

  fChoiceLabelKind =
    ischemeChoiceLabelKind::kChoiceLabelNone;
}

ischemeChoice::~ischemeChoice ()
{}

void ischemeChoice::setChoiceLabelSuppliedByAnOption (
  const std::string& label,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Setting the label of choice \"" <<
      fChoiceName <<
      "\" to \"" <<
      label <<
      "\", supplied by an option" <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // no checks here, the labels are not knoown yet
  fChoiceLabel = label;
}

void ischemeChoice::selectChoiceLabel (
  const std::string& label,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Setting the label of choice \"" <<
      fChoiceName <<
      "\" to \"" <<
      label <<
      "\", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // is this label in the choices labels set?
  std::set<std::string>::const_iterator
    it =
      fLabelsSet.find (label);

  if (it == fLabelsSet.end ()) {
    // no, issue error message
    std::stringstream ss;

    ss <<
      "choice \"" <<
      fChoiceName <<
      "\" cannot be set to label \"" <<
      label <<
      "\", the labels are: " <<
      labelsSetAsString ();

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  // did fChoiceName get a label by an option?
  const std::multimap<std::string, std::string>&
    setChoiceToLabelMultiMap =
      gGlobalIschemeInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMap ();

  switch (fChoiceLabelKind) {
    case ischemeChoiceLabelKind::kChoiceLabelNone:
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
          if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
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
              std::endl;
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
        std::stringstream ss;

        ss <<
          "choice \"" <<
          fChoiceName <<
          " already got label \"" <<
          fChoiceLabel <<
          "\" supplied to the script"
          ", ignoring label \"" <<
          label <<
          "\"";

        ischemeWarning (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;

    case ischemeChoiceLabelKind::kChoiceLabelSetInScript:
      {
        // set in script multiple times
        std::stringstream ss;

        ss <<
          "choice \"" <<
          fChoiceName <<
          "\"" <<
          " already has label \"" <<
          fChoiceLabel <<
          "\" supplied to the script";

        ischemeError (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;
  } // switch
}

std::string ischemeChoice::getChoiceLabel (ischemeDriver& drv) const
{
  switch (fChoiceLabelKind) {
    case ischemeChoiceLabelKind::kChoiceLabelNone:
      {
        std::stringstream ss;

        ss <<
          "choice \"" <<
          fChoiceName <<
          "\" has not got any label";

        ischemeError (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;

    case ischemeChoiceLabelKind::kChoiceLabelOptionSupplied:
      if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
        gLog <<
          "====> " <<
          " choice \"" <<
          fChoiceName <<
          ", label " <<
          fChoiceLabel <<
          "\", supplied to the script" <<
          ", line " << drv.getScannerLocation () <<
          std::endl;
      }
      break;

    case ischemeChoiceLabelKind::kChoiceLabelSetInScript:
      if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
        gLog <<
          "====>" <<
          "choice \"" <<
          fChoiceName <<
          "\" has label \"" <<
          fChoiceLabel <<
          " \", set in the script" <<
          ", line " << drv.getScannerLocation () <<
          std::endl;
      }
      break;
  } // switch

  return fChoiceLabel;
}

std::string ischemeChoice::getChoiceLabelWithoutTrace (ischemeDriver& drv) const
{
  switch (fChoiceLabelKind) {
    case ischemeChoiceLabelKind::kChoiceLabelNone:
      {
        std::stringstream ss;

        ss <<
          "choice \"" <<
          fChoiceName <<
          "\" has not got any label";

        ischemeError (
          ss.str (),
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
  const std::string& label,
  ischemeDriver&     drv) const
{
  S_ischemeOptionsBlock result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Fetching label \"" <<
      label <<
      "\" in choice \"" <<
      fChoiceName <<
      "\", line " << drv.getScannerLocation () <<
      std::endl;
  }

  std::map<std::string, S_ischemeOptionsBlock>::const_iterator
    it =
      fChoiceLabelsToOptionsBlocksMap.find (
        label);

  if (it != fChoiceLabelsToOptionsBlocksMap.end ()) {
    result = (*it).second;
  }
  else {
    std::stringstream ss;

    ss <<
      "label \"" <<
      label <<
      "\" is not known to choice \"" <<
      fChoiceName <<
      "\", line " << drv.getScannerLocation ();

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "<==== Result: \"" <<
      result <<
      "\", line " << drv.getScannerLocation () <<
      std::endl;
  }

  return result;
}

void ischemeChoice::addLabel (
  const std::string& label,
  ischemeDriver&   drv)
{
  // is this label in the choices labels set?
  std::set<std::string>::const_iterator
    it =
      fLabelsSet.find (label);

  if (it != fLabelsSet.end ()) {
    // yes, issue error message
    std::stringstream ss;

    ss <<
      "choice label \"" <<
      label <<
      "\" occurs more that once in choice \"" <<
      fChoiceName <<
      "\", line " << drv.getScannerLocation ();

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  else {
    if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
      gLog <<
        "====> Adding label \"" <<
        label <<
        "\"" <<
        " to choice \"" <<
        fChoiceName <<
        "\"" <<
        ", line " << drv.getScannerLocation () <<
        std::endl;
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
  const std::string&      label,
  const S_ischemeOptionsBlock& optionsBlock,
  ischemeDriver&        drv)
{
  const S_ischemeOptionsBlock&
    labelOptionsBlock =
      fChoiceLabelsToOptionsBlocksMap [
        label];

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    labelOptionsBlock != nullptr,
    "labelOptionsBlock is null");
#endif

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
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
      std::endl;
  }

  std::vector<S_oahOption>&
    labelOptionsBlockOptionsVector =
      labelOptionsBlock->
        getOptionsBlockOptionsVectorNonConst ();

  const std::vector<S_oahOption>&
    optionsBlockOptionsVector =
      optionsBlock->
        getOptionsBlockOptionsVector ();

  optionsNameAndValueVectorsPlusEquals (
    labelOptionsBlockOptionsVector,
    optionsBlockOptionsVector);
}

void ischemeChoice::registerChoiceDefaultLabel (
  const std::string& label,
  ischemeDriver&   drv)
{
  if (! mfStringIsInStringSet (label, fLabelsSet)) {
    std::stringstream ss;

    ss <<
      "choice label \"" <<
      label <<
      "\" is not present in choice \"" <<
      fChoiceName <<
      "\"";

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

//   if (fChoiceDefaultLabel.size ()) { // JMI
//     std::stringstream ss;
//
//     ss <<
//       "choice label \"" <<
//       label <<
//       "\" is not present in choice \"" <<
//       fChoiceName <<
//       "\"";
//
//     ischemeInternalError (
//       ss.str (),
//       drv.getScannerLocation ());
//   }

  fChoiceDefaultLabel = label;
}

std::string ischemeChoice::labelsSetAsString () const
{
  std::stringstream ss;

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
        ss << "\n";
        cumulatedLength = 0;
      }

      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != labelsSetSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

std::string ischemeChoice::asString () const
{
  std::stringstream ss;

  ss <<
    "ischemeChoice [" <<
    "fChoiceName: " << fChoiceName <<
    ", " <<

    "fChoiceLabel: [" << fChoiceLabel << ']' <<
    ']';

  return ss.str ();
}

void ischemeChoice::displayChoiceLabelsToOptionsBlocksMap (std::ostream& os) const
{
  size_t
    ChoiceLabelsToOptionsBlocksMapSize =
      fLabelsSet.size ();

  const int fieldWidth = 19;

  os <<
    std::setw (fieldWidth) <<
    "fChoiceLabelsToOptionsBlocksMap" << " [" <<
    std::endl;

  if (ChoiceLabelsToOptionsBlocksMapSize) {
//     int counter = 0;
    for (std::pair<std::string, S_ischemeOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      std::string        key = thePair.first;
      const S_ischemeOptionsBlock& optionsBlock = thePair.second;
      os <<
        key << ": " <<
        std::endl;

      ++gIndenter;

      os <<
        optionsBlock;

      --gIndenter;

//       if (++counter < ChoiceLabelsToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << ']' << std::endl;
}

void ischemeChoice::print (std::ostream& os) const
{
  os <<
    "ischemeChoice [" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 19;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fChoiceName" << ": \"" << fChoiceName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fChoiceLabelKind" << ": " << fChoiceLabelKind << "" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChoiceLabel" << ": \"" << fChoiceLabel << "\"" <<
    std::endl;

  size_t
    labelsSetSize =
      fLabelsSet.size ();

  os <<
    std::setw (fieldWidth) <<
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
  os << ']' << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fChoiceDefaultLabel" << ": \"" << fChoiceDefaultLabel << "\"" <<
    std::endl;

  size_t
    choiceLabelsToOptionsBlocksMapSize =
      fLabelsSet.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChoiceLabelsToOptionsBlocksMap" << " [" <<
    std::endl;

  if (choiceLabelsToOptionsBlocksMapSize) {
//     int counter = 0;
    for (std::pair<std::string, S_ischemeOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      std::string        key = thePair.first;
      const S_ischemeOptionsBlock& optionsBlock = thePair.second;
      os <<
        key << ": " <<
        std::endl;

      ++gIndenter;

      if (optionsBlock) {
        os <<
          optionsBlock;
      }
      else {
        os << "[NONE]" << std::endl;
      }

      --gIndenter;

//       if (++counter < ChoiceLabelsToOptionsBlocksMapSize) { JMI
//         os << " | ";
//       }
    } // for
  }
  os << ']' << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_ischemeChoice& elt)
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
  const S_ischemeChoice& choice,
  ischemeDriver&  drv)
{
  std::string
    choiceName =
      choice->getChoiceName ();

  // is this choiceName in the choices labels set?
  std::map<std::string, S_ischemeChoice>::const_iterator
    it =
      fChoicesMap.find (
        choiceName);

  if (it != fChoicesMap.end ()) {
    std::stringstream ss;

    ss <<
      "choice \"" <<
      choiceName <<
      "\" occurs more that once in the choices table";

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  fChoicesMap [choiceName] = choice;
}

// S_ischemeChoice ischemeChoicesTable::lookupChoiceByName (
//   const std::string& name)
// {
//   S_ischemeChoice result;
//
//   // is this choiceName in the choices labels set?
//   std::map<std::string, S_ischemeChoice>::const_iterator
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
  const std::string&     name,
  const ischemeDriver& drv)
{
  S_ischemeChoice result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Fetching choice named \"" <<
      name <<
      "\" in choices table" <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // is this choiceName in the choices labels set?
  std::map<std::string, S_ischemeChoice>::const_iterator
    it =
      fChoicesMap.find (name);

  if (it != fChoicesMap.end ()) {
    result = (*it).second;
  }

  else {
    std::stringstream ss;

    ss <<
      "choice \"" << name <<
      "\" is unknown in choices table";

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  return result;
}

S_ischemeChoice ischemeChoicesTable::fetchChoiceByNameNonConst (
  const std::string& name,
  ischemeDriver&   drv)
{
  S_ischemeChoice result;

  if (gGlobalIschemeInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Fetching choice named \"" <<
      name <<
      "\" in choices table (non-const)" <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // is this choiceName in the choices map?
  std::map<std::string, S_ischemeChoice>::const_iterator
    it =
      fChoicesMap.find (name);

  if (it != fChoicesMap.end ()) {
    result = (*it).second;
  }

  else {
    std::stringstream ss;

    ss <<
      "choice \"" << name <<
      "\" is unknown in choices table (non-const)";

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  return result;
}

std::string ischemeChoicesTable::asString () const
{
  std::stringstream ss;

  ss <<
    "ischemeChoicesTable [" <<
    ']';

  return ss.str ();
}

void ischemeChoicesTable::displayChoicesMap (std::ostream& os) const // useless ??? JMI
{
  os <<
    "ischemeChoicesTable [" <<
    std::endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (std::pair<std::string, S_ischemeChoice> thePair : fChoicesMap) {
      S_ischemeChoice choice = thePair.second;

      choice->
        displayChoiceLabelsToOptionsBlocksMap (os);
    } // for

    --gIndenter;
  }

  os << ']' << std::endl;
}

void ischemeChoicesTable::print (std::ostream& os) const
{
  os <<
    "ischemeChoicesTable [" <<
    std::endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (std::pair<std::string, S_ischemeChoice> thePair : fChoicesMap) {
      S_ischemeChoice choice = thePair.second;

      os << choice;
    } // for

    --gIndenter;
  }

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_ischemeChoicesTable& elt)
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
S_ischemeCaseChoiceStatement ischemeCaseChoiceStatement::create (
  const S_ischemeChoice& caseChoice,
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
  const S_ischemeChoice& caseChoice,
  ischemeDriver&  drv)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    caseChoice != nullptr,
    "caseChoice is null");
#endif

  fCaseChoice = caseChoice;

  if (gGlobalIschemeInterpreterOahGroup->getTraceCaseChoiceStatements ()) {
    gLog <<
      "====> Creating case statement" <<
      ", caseChoice: [" <<
      fCaseChoice->asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // all labels are initially unused
  for (std::string label : fCaseChoice->getLabelsSet ()) {
    fCaseUnusedLabels.insert (label);
  }
}

ischemeCaseChoiceStatement::~ischemeCaseChoiceStatement ()
{}

void ischemeCaseChoiceStatement::registerCaseChoiceLabel (
  const std::string& label,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceCaseChoiceStatements ()) {
    gLog <<
      "====> Registering case label \"" <<
      label <<
      "\" in case statement: [" <<
      asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  std::set<std::string>
    choiceLabelSet =
      fCaseChoice->
        getLabelsSet ();

  if (! mfStringIsInStringSet (label, choiceLabelSet)) {
    // this label has already been registered in this choice
    std::stringstream ss;

    ss <<
      "label '" <<
      label <<
      "' is not known in choice \"" <<
      fCaseChoice->getChoiceName () <<
      ", the labels are: " <<
      fCaseChoice->labelsSetAsString () <<
      ", line " << drv.getScannerLocation ();

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  if (mfStringIsInStringSet (label, fCaseLabelsSet)) {
    // this label has already been registered
    std::stringstream ss;

    ss <<
      "choice label \"" <<
      label <<
      "\" occurs more that once in this case \"" <<
      fCaseChoice->getChoiceName () <<
      "\" statement";

    ischemeError (
      ss.str (),
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
    std::stringstream ss;

    ss <<
      "The following labels for choice \"" <<
      fCaseChoice->getChoiceName () <<
      "\" have not been used in this case statement: " <<
      mfStringSetAsString (fCaseUnusedLabels);

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }
}

std::string ischemeCaseChoiceStatement::asString () const
{
  std::stringstream ss;

  ss <<
    "ischemeCaseChoiceStatement [" <<
    fCaseChoice->asString () <<
    ']';

  return ss.str ();
}

std::string ischemeCaseChoiceStatement::currentLabelsListAsString () const
{
  std::stringstream ss;

  ss << "[Labels:";

  for (std::string label : fCaseCurrentLabelsList ) {
    ss << ' ' << label;
  } // for

  ss << ']';

  return ss.str ();
}

void ischemeCaseChoiceStatement::print (std::ostream& os) const
{
  os <<
    "ischemeCaseChoiceStatement [" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fCaseChoice " << ": " <<
    std::endl;

  ++gIndenter;
  os <<
    fCaseChoice;
  --gIndenter;

  os <<
    std::setw (fieldWidth) <<
    "fCaseUnusedLabels" << ": " <<
    std::endl;

  ++gIndenter;
  os <<
    mfStringSetAsString (fCaseUnusedLabels) <<
    std::endl;
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_ischemeCaseChoiceStatement& elt)
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
S_ischemeCaseInputStatement ischemeCaseInputStatement::create (
  const S_ischemeInput& caseInput,
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
  const S_ischemeInput& caseInput,
  ischemeDriver& drv)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    caseInput != nullptr,
    "caseInput is null");
#endif

  fCaseInputInput = caseInput;

  if (gGlobalIschemeInterpreterOahGroup->getTraceCaseInputStatements ()) {
    gLog <<
      "====> Creating case statement" <<
      ", caseInput: [" <<
      fCaseInputInput->asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  // all inputs are initially unused
  for (std::string name : fCaseInputInput->getNamesSet ()) {
    fCaseInputUnusedNames.insert (name);
  }
}

ischemeCaseInputStatement::~ischemeCaseInputStatement ()
{}

void ischemeCaseInputStatement::registerCaseInputName (
  const std::string& name,
  ischemeDriver&   drv)
{
  if (gGlobalIschemeInterpreterOahGroup->getTraceCaseInputStatements ()) {
    gLog <<
      "====> Registering case name \"" <<
      name <<
      "\" in case statement: [" <<
      asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      std::endl;
  }

  std::set<std::string>
    choiceNameSet =
      fCaseInputInput->
        getNamesSet ();

  if (! mfStringIsInStringSet (name, choiceNameSet)) {
    // this name has already been registered in this choice
    std::stringstream ss;

    ss <<
      "name '" <<
      name <<
      "' is not known in input \"" <<
      fCaseInputInput->getInputName () <<
      ", the names are: " <<
      fCaseInputInput->namesSetAsString () <<
      ", line " << drv.getScannerLocation ();

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }

  if (mfStringIsInStringSet (name, fCaseInputNamesSet)) {
    // this name has already been registered
    std::stringstream ss;

    ss <<
      "input name \"" <<
      name <<
      "\" occurs more that once in this case \"" <<
      fCaseInputInput->getInputName () <<
      "\" statement";

    ischemeError (
      ss.str (),
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
    std::stringstream ss;

    ss <<
      "The following names for input \"" <<
      fCaseInputInput->getInputName () <<
      "\" have not been used in this case statement: " <<
      mfStringSetAsString (fCaseInputUnusedNames);

    ischemeError (
      ss.str (),
      drv.getScannerLocation ());
  }
}

std::string ischemeCaseInputStatement::asString () const
{
  std::stringstream ss;

  ss <<
    "ischemeCaseInputStatement [" <<
    fCaseInputInput->asString () <<
    ']';

  return ss.str ();
}

std::string ischemeCaseInputStatement::currentNamesListAsString () const
{
  std::stringstream ss;

  ss << "[Inputs:";

  for (std::string name : fCaseInputCurrentNamesList ) {
    ss << ' ' << name;
  } // for

  ss << ']';

  return ss.str ();
}

void ischemeCaseInputStatement::print (std::ostream& os) const
{
  os <<
    "ischemeCaseInputStatement [" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fCaseInputInput " << ": " <<
    std::endl;

  ++gIndenter;
  os <<
    fCaseInputInput;
  --gIndenter;

  os <<
    std::setw (fieldWidth) <<
    "fCaseInputUnusedNames" << ": " <<
    std::endl;

  ++gIndenter;
  os <<
    mfStringSetAsString (fCaseInputUnusedNames) <<
    std::endl;
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_ischemeCaseInputStatement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
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
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah () && ! gEarlyOptions.getEarlyQuietOption ()) {
//       gLog <<
//         "Initializing iScheme basic types handling" <<
//         std::endl;
//     }
// #endif
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
