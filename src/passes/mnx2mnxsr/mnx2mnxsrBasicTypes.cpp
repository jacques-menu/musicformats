/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include "mfAssert.h"
#include "mfBool.h"
#include "mfStringsHandling.h"

#include "mfPreprocessorSettings.h"

#include "oahOah.h"

#include "mnx2mnxsrBasicTypes.h"
#include "mnx2mnxsrDriver.h"

#include "mnx2mnxsrInterpreterOah.h"

#include "mnx2mnxsrWae.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
S_mnx2mnxsrOptionsBlock mnx2mnxsrOptionsBlock::create (
  const std::string& optionsBlockName)
{
  mnx2mnxsrOptionsBlock* obj =
    new mnx2mnxsrOptionsBlock (
      optionsBlockName);
  assert (obj != nullptr);
  return obj;
}

mnx2mnxsrOptionsBlock::mnx2mnxsrOptionsBlock (
  const std::string& optionsBlockName)
{
  fOptionsBlockName = optionsBlockName;
}

mnx2mnxsrOptionsBlock::~mnx2mnxsrOptionsBlock ()
{}

void mnx2mnxsrOptionsBlock::registerOptionsInOptionsBlock (
  S_oahOption option,
  mnx2mnxsrDriver& drv)
{
  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceOptionsBlocks ()) {
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

std::string mnx2mnxsrOptionsBlock::asString () const
{
  std::stringstream ss;

  ss <<
    "mnx2mnxsrOptionsBlock [" <<
    "fOptionsBlockName: " << fOptionsBlockName <<
    ']';

  return ss.str ();
}

std::string mnx2mnxsrOptionsBlock::asOptionsString () const
{
  std::stringstream ss;

  if (fOptionsBlockOptionsVector.size ()) {
    vector<S_oahOption>::const_iterator
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

void mnx2mnxsrOptionsBlock::print (std::ostream& os) const
{
  os <<
    "mnx2mnxsrOptionsBlock [" <<
    endl;

  ++gIndenter;

  constexpr int fieldWidth = 34;

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

  os << ']' << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrOptionsBlock& elt)
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
std::string mnx2mnxsrInputNameKindAsString (
  mnx2mnxsrInputNameKind inputNameKind)
{
  std::string result;

  switch (inputNameKind) {
    case mnx2mnxsrInputNameKind::kInputNameNone:
      result = "kInputNameNone";
      break;
    case mnx2mnxsrInputNameKind::kInputNameOptionSupplied:
      result = "kInputNameOptionSupplied";
      break;
    case mnx2mnxsrInputNameKind::kInputNameSetInScript:
      result = "kInputNameSetInScript";
      break;
  } // switch

  return result;
}

std::ostream& operator<< (std::ostream& os, const mnx2mnxsrInputNameKind& elt)
{
  os << mnx2mnxsrInputNameKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_mnx2mnxsrInput mnx2mnxsrInput::create (
  const std::string& inputName)
{
  mnx2mnxsrInput* obj =
    new mnx2mnxsrInput (
      inputName);
  assert (obj != nullptr);
  return obj;
}

mnx2mnxsrInput::mnx2mnxsrInput (
  const std::string& inputName)
{
  fInputName = inputName;

  fInputNameKind =
    mnx2mnxsrInputNameKind::kInputNameNone;
}

mnx2mnxsrInput::~mnx2mnxsrInput ()
{}

void mnx2mnxsrInput::setInputNameSuppliedByAnOption (
  const std::string& name,
  mnx2mnxsrDriver&        drv)
{
  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
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

void mnx2mnxsrInput::selectInputName (
  const std::string& name,
  mnx2mnxsrDriver&        drv)
{
  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Setting the name of input \"" <<
      fInputName <<
      "\" to \"" <<
      name <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this name in the inputs names set?
  std::set <std::string>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

//   // did fInputName get a name by an option?
//   const std::multimap <std::string, std::string>&
//     setInputToNameMultiMap =
//       gGlobalMnx2mnxsrInterpreterOahGroup->
//         getSelectInputToNamesMultiMap ();
//
//   switch (fInputNameKind) {
//     case mnx2mnxsrInputNameKind::kInputNameNone:
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
//           if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
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
//             mnx2mnxsrInputNameKind::kInputNameOptionSupplied;
//
//           drv.registerOptionsSuppliedInputsAsUsed (
//             fInputName);
//         }
//
//         else {
//           fInputName = optionSuppliedName;
//           fInputNameKind =
//             mnx2mnxsrInputNameKind::kInputNameSetInScript;
//         }
//       }
//       break;
//
//     case mnx2mnxsrInputNameKind::kInputNameOptionSupplied:
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
//         mnx2mnxsrWarning (
//           ss.str (),
//           drv.getScannerLocation ());
//       }
//       break;
//
//     case mnx2mnxsrInputNameKind::kInputNameSetInScript:
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
//         mnx2mnxsrError (
//           ss.str (),
//           drv.getScannerLocation ());
//       }
//       break;
//   } // switch
}

std::string mnx2mnxsrInput::getInputName (mnx2mnxsrDriver& drv) const
{
  switch (fInputNameKind) {
    case mnx2mnxsrInputNameKind::kInputNameNone:
      {
        std::stringstream ss;

        ss <<
          "input \"" <<
          fInputName <<
          "\" has not got any name";

        mnx2mnxsrError (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;

    case mnx2mnxsrInputNameKind::kInputNameOptionSupplied:
      if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
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

    case mnx2mnxsrInputNameKind::kInputNameSetInScript:
      if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
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

std::string mnx2mnxsrInput::getInputNameWithoutTrace (mnx2mnxsrDriver& drv) const
{
  switch (fInputNameKind) {
    case mnx2mnxsrInputNameKind::kInputNameNone:
      {
        std::stringstream ss;

        ss <<
          "input \"" <<
          fInputName <<
          "\" has not got any name";

        mnx2mnxsrError (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;

    case mnx2mnxsrInputNameKind::kInputNameOptionSupplied:
    case mnx2mnxsrInputNameKind::kInputNameSetInScript:
      break;
  } // switch

  return fInputName;
}

S_mnx2mnxsrOptionsBlock mnx2mnxsrInput::getInputOptionsBlockForName (
  const std::string& name,
  mnx2mnxsrDriver&        drv) const
{
  S_mnx2mnxsrOptionsBlock result;

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Fetching name \"" <<
      name <<
      "\" in input \"" <<
      fInputName <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  map<std::string, S_mnx2mnxsrOptionsBlock>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "<==== Result: \"" <<
      result <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  return result;
}

void mnx2mnxsrInput::addName (
  const std::string& name,
  mnx2mnxsrDriver&        drv)
{
  // is this name in the inputs names set?
  std::set <std::string>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  else {
    if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
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
      mnx2mnxsrOptionsBlock::create (
        name);
  }
}

void mnx2mnxsrInput::enrichNameOptionsBlock (
  const std::string& name,
  S_mnx2mnxsrOptionsBlock optionsBlock,
  mnx2mnxsrDriver&        drv)
{
  S_mnx2mnxsrOptionsBlock
    nameOptionsBlock =
      fInputNamesToOptionsBlocksMap [
        name];

  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    nameOptionsBlock != nullptr,
    "nameOptionsBlock is NULL");

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
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

std::string mnx2mnxsrInput::namesSetAsString () const
{
  std::stringstream ss;

  size_t namesSetSize =
    fNamesSet.size ();

  if (namesSetSize) {
    size_t nextToLast =
      namesSetSize - 1;

    size_t count = 0;

    size_t namesListMaxLength = 60;
    size_t cumulatedLength = 0;

    for (
      std::set <std::string>::const_iterator i =
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

std::string mnx2mnxsrInput::asString () const
{
  std::stringstream ss;

  ss <<
    "mnx2mnxsrInput [" <<
    "fInputName: " << fInputName <<
    ", " <<

    "fInputName: [" << fInputName << ']' <<
    ']';

  return ss.str ();
}

void mnx2mnxsrInput::displayInputNamesToOptionsBlocksMap (std::ostream& os) const
{
  size_t
    InputNamesToOptionsBlocksMapSize =
      fNamesSet.size ();

  constexpr int fieldWidth = 19;

  os <<
    setw (fieldWidth) <<
    "fInputNamesToOptionsBlocksMap" << " [" <<
    endl;

  if (InputNamesToOptionsBlocksMapSize) {
//     int counter = 0;
    for (pair<std::string, S_mnx2mnxsrOptionsBlock> thePair : fInputNamesToOptionsBlocksMap) {
      std::string        key = thePair.first;
      S_mnx2mnxsrOptionsBlock optionsBlock = thePair.second;
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

void mnx2mnxsrInput::print (std::ostream& os) const
{
  os <<
    "mnx2mnxsrInput [" <<
    endl;

  ++gIndenter;

  constexpr int fieldWidth = 19;

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
    for (pair<std::string, S_mnx2mnxsrOptionsBlock> thePair : fInputNamesToOptionsBlocksMap) {
      std::string        key = thePair.first;
      S_mnx2mnxsrOptionsBlock optionsBlock = thePair.second;
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
  os << ']' << endl;

  --gIndenter;

  os << ']' << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrInput& elt)
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
S_mnx2mnxsrInputsTable mnx2mnxsrInputsTable::create ()
{
  mnx2mnxsrInputsTable* obj =
    new mnx2mnxsrInputsTable ();
  assert (obj != nullptr);
  return obj;
}

mnx2mnxsrInputsTable::mnx2mnxsrInputsTable ()
{}

mnx2mnxsrInputsTable::~mnx2mnxsrInputsTable ()
{}

void mnx2mnxsrInputsTable::addInput (
  S_mnx2mnxsrInput input,
  mnx2mnxsrDriver& drv)
{
  std::string
    inputName =
      input->getInputName ();

  // is this inputName in the inputs names set?
  map<std::string, S_mnx2mnxsrInput>::const_iterator
    it =
      fInputsMap.find (
        inputName);

  if (it != fInputsMap.end ()) {
    std::stringstream ss;

    ss <<
      "input \"" <<
      inputName <<
      "\" occurs more that once in the inputs table";

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  fInputsMap [inputName] = input;
}

// S_mnx2mnxsrInput mnx2mnxsrInputsTable::lookupInputByName (
//   const std::string& name)
// {
//   S_mnx2mnxsrInput result;
//
//   // is this inputName in the inputs names set?
//   map<std::string, S_mnx2mnxsrInput>::const_iterator
//     it =
//       fInputsMap.find (name);
//
//   if (it != fInputsMap.end ()) {
//     result = (*it).second;
//   }
//
//   return result;
// }

S_mnx2mnxsrInput mnx2mnxsrInputsTable::fetchInputByName (
  const std::string& name,
  const mnx2mnxsrDriver&  drv)
{
  S_mnx2mnxsrInput result;

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Fetching input named \"" <<
      name <<
      "\" in inputs table" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this inputName in the inputs names set?
  map<std::string, S_mnx2mnxsrInput>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  return result;
}

S_mnx2mnxsrInput mnx2mnxsrInputsTable::fetchInputByNameNonConst (
  const std::string& name,
  mnx2mnxsrDriver&        drv)
{
  S_mnx2mnxsrInput result;

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceInputs ()) {
    gLog <<
      "====> Fetching input named \"" <<
      name <<
      "\" in inputs table (non-const)" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this inputName in the inputs map?
  map<std::string, S_mnx2mnxsrInput>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  return result;
}

std::string mnx2mnxsrInputsTable::asString () const
{
  std::stringstream ss;

  ss <<
    "mnx2mnxsrInputsTable [" <<
    ']';

  return ss.str ();
}

void mnx2mnxsrInputsTable::displayInputsMap (std::ostream& os) const // useless ??? JMI
{
  os <<
    "mnx2mnxsrInputsTable [" <<
    endl;

  if (fInputsMap.size ()) {
    ++gIndenter;

    for (pair<std::string, S_mnx2mnxsrInput> thePair : fInputsMap) {
      S_mnx2mnxsrInput input = thePair.second;

      input->
        displayInputNamesToOptionsBlocksMap (os);
    } // for

    --gIndenter;
  }

  os << ']' << endl;
}

void mnx2mnxsrInputsTable::print (std::ostream& os) const
{
  os <<
    "mnx2mnxsrInputsTable [" <<
    endl;

  if (fInputsMap.size ()) {
    ++gIndenter;

    for (pair<std::string, S_mnx2mnxsrInput> thePair : fInputsMap) {
      S_mnx2mnxsrInput input = thePair.second;

      os << input;
    } // for

    --gIndenter;
  }

  os << ']' << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrInputsTable& elt)
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
std::string mnx2mnxsrChoiceLabelKindAsString (
  mnx2mnxsrChoiceLabelKind inputNameKind)
{
  std::string result;

  switch (inputNameKind) {
    case mnx2mnxsrChoiceLabelKind::kChoiceLabelNone:
      result = "kChoiceLabelNone";
      break;
    case mnx2mnxsrChoiceLabelKind::kChoiceLabelOptionSupplied:
      result = "kChoiceLabelOptionSupplied";
      break;
    case mnx2mnxsrChoiceLabelKind::kChoiceLabelSetInScript:
      result = "kChoiceLabelSetInScript";
      break;
  } // switch

  return result;
}

std::ostream& operator<< (std::ostream& os, const mnx2mnxsrChoiceLabelKind& elt)
{
  os << mnx2mnxsrChoiceLabelKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_mnx2mnxsrChoice mnx2mnxsrChoice::create (
  const std::string& choiceName)
{
  mnx2mnxsrChoice* obj =
    new mnx2mnxsrChoice (
      choiceName);
  assert (obj != nullptr);
  return obj;
}

mnx2mnxsrChoice::mnx2mnxsrChoice (
  const std::string& choiceName)
{
  fChoiceName = choiceName;

  fChoiceLabelKind =
    mnx2mnxsrChoiceLabelKind::kChoiceLabelNone;
}

mnx2mnxsrChoice::~mnx2mnxsrChoice ()
{}

void mnx2mnxsrChoice::setChoiceLabelSuppliedByAnOption (
  const std::string& label,
  mnx2mnxsrDriver&   drv)
{
  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
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

void mnx2mnxsrChoice::selectChoiceLabel (
  const std::string& label,
  mnx2mnxsrDriver&   drv)
{
  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Setting the label of choice \"" <<
      fChoiceName <<
      "\" to \"" <<
      label <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this label in the choices labels set?
  std::set <std::string>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  // did fChoiceName get a label by an option?
  const std::multimap <std::string, std::string>&
    setChoiceToLabelMultiMap =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMap ();

  switch (fChoiceLabelKind) {
    case mnx2mnxsrChoiceLabelKind::kChoiceLabelNone:
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
          if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
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
            mnx2mnxsrChoiceLabelKind::kChoiceLabelOptionSupplied;

          drv.registerOptionsSuppliedChoicesAsUsed (
            fChoiceName);
        }

        else {
          fChoiceLabel = optionSuppliedLabel;
          fChoiceLabelKind =
            mnx2mnxsrChoiceLabelKind::kChoiceLabelSetInScript;
        }
      }
      break;

    case mnx2mnxsrChoiceLabelKind::kChoiceLabelOptionSupplied:
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

        mnx2mnxsrWarning (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;

    case mnx2mnxsrChoiceLabelKind::kChoiceLabelSetInScript:
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

        mnx2mnxsrError (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;
  } // switch
}

std::string mnx2mnxsrChoice::getChoiceLabel (mnx2mnxsrDriver& drv) const
{
  switch (fChoiceLabelKind) {
    case mnx2mnxsrChoiceLabelKind::kChoiceLabelNone:
      {
        std::stringstream ss;

        ss <<
          "choice \"" <<
          fChoiceName <<
          "\" has not got any label";

        mnx2mnxsrError (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;

    case mnx2mnxsrChoiceLabelKind::kChoiceLabelOptionSupplied:
      if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
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

    case mnx2mnxsrChoiceLabelKind::kChoiceLabelSetInScript:
      if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
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

std::string mnx2mnxsrChoice::getChoiceLabelWithoutTrace (mnx2mnxsrDriver& drv) const
{
  switch (fChoiceLabelKind) {
    case mnx2mnxsrChoiceLabelKind::kChoiceLabelNone:
      {
        std::stringstream ss;

        ss <<
          "choice \"" <<
          fChoiceName <<
          "\" has not got any label";

        mnx2mnxsrError (
          ss.str (),
          drv.getScannerLocation ());
      }
      break;

    case mnx2mnxsrChoiceLabelKind::kChoiceLabelOptionSupplied:
    case mnx2mnxsrChoiceLabelKind::kChoiceLabelSetInScript:
      break;
  } // switch

  return fChoiceLabel;
}

S_mnx2mnxsrOptionsBlock mnx2mnxsrChoice::getChoiceOptionsBlockForLabel (
  const std::string& label,
  mnx2mnxsrDriver&        drv) const
{
  S_mnx2mnxsrOptionsBlock result;

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Fetching label \"" <<
      label <<
      "\" in choice \"" <<
      fChoiceName <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  map<std::string, S_mnx2mnxsrOptionsBlock>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "<==== Result: \"" <<
      result <<
      "\", line " << drv.getScannerLocation () <<
      endl;
  }

  return result;
}

void mnx2mnxsrChoice::addLabel (
  const std::string& label,
  mnx2mnxsrDriver&        drv)
{
  // is this label in the choices labels set?
  std::set <std::string>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  else {
    if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
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
      mnx2mnxsrOptionsBlock::create (
        label);
  }
}

void mnx2mnxsrChoice::enrichLabelOptionsBlock (
  const std::string& label,
  S_mnx2mnxsrOptionsBlock optionsBlock,
  mnx2mnxsrDriver&        drv)
{
  S_mnx2mnxsrOptionsBlock
    labelOptionsBlock =
      fChoiceLabelsToOptionsBlocksMap [
        label];

  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    labelOptionsBlock != nullptr,
    "labelOptionsBlock is NULL");

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
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

void mnx2mnxsrChoice::registerChoiceDefaultLabel (
  const std::string& label,
  mnx2mnxsrDriver&        drv)
{
  if (! mfStringIsInStringSet (label, fLabelsSet)) {
    std::stringstream ss;

    ss <<
      "choice label \"" <<
      label <<
      "\" is not present in choice \"" <<
      fChoiceName <<
      "\"";

    mnx2mnxsrError (
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
//     mnx2mnxsrInternalError (
//       ss.str (),
//       drv.getScannerLocation ());
//   }

  fChoiceDefaultLabel = label;
}

std::string mnx2mnxsrChoice::labelsSetAsString () const
{
  std::stringstream ss;

  size_t labelsSetSize =
    fLabelsSet.size ();

  if (labelsSetSize) {
    size_t nextToLast =
      labelsSetSize - 1;

    size_t count = 0;

    size_t namesListMaxLength = 60;
    size_t cumulatedLength = 0;

    for (
      std::set <std::string>::const_iterator i =
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

std::string mnx2mnxsrChoice::asString () const
{
  std::stringstream ss;

  ss <<
    "mnx2mnxsrChoice [" <<
    "fChoiceName: " << fChoiceName <<
    ", " <<

    "fChoiceLabel: [" << fChoiceLabel << ']' <<
    ']';

  return ss.str ();
}

void mnx2mnxsrChoice::displayChoiceLabelsToOptionsBlocksMap (std::ostream& os) const
{
  size_t
    ChoiceLabelsToOptionsBlocksMapSize =
      fLabelsSet.size ();

  constexpr int fieldWidth = 19;

  os <<
    setw (fieldWidth) <<
    "fChoiceLabelsToOptionsBlocksMap" << " [" <<
    endl;

  if (ChoiceLabelsToOptionsBlocksMapSize) {
//     int counter = 0;
    for (pair<std::string, S_mnx2mnxsrOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      std::string        key = thePair.first;
      S_mnx2mnxsrOptionsBlock optionsBlock = thePair.second;
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

void mnx2mnxsrChoice::print (std::ostream& os) const
{
  os <<
    "mnx2mnxsrChoice [" <<
    endl;

  ++gIndenter;

  constexpr int fieldWidth = 19;

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
    for (pair<std::string, S_mnx2mnxsrOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      std::string        key = thePair.first;
      S_mnx2mnxsrOptionsBlock optionsBlock = thePair.second;
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
  os << ']' << endl;

  --gIndenter;

  os << ']' << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrChoice& elt)
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
S_mnx2mnxsrChoicesTable mnx2mnxsrChoicesTable::create ()
{
  mnx2mnxsrChoicesTable* obj =
    new mnx2mnxsrChoicesTable ();
  assert (obj != nullptr);
  return obj;
}

mnx2mnxsrChoicesTable::mnx2mnxsrChoicesTable ()
{}

mnx2mnxsrChoicesTable::~mnx2mnxsrChoicesTable ()
{}

void mnx2mnxsrChoicesTable::addChoice (
  S_mnx2mnxsrChoice choice,
  mnx2mnxsrDriver&  drv)
{
  std::string
    choiceName =
      choice->getChoiceName ();

  // is this choiceName in the choices labels set?
  map<std::string, S_mnx2mnxsrChoice>::const_iterator
    it =
      fChoicesMap.find (
        choiceName);

  if (it != fChoicesMap.end ()) {
    std::stringstream ss;

    ss <<
      "choice \"" <<
      choiceName <<
      "\" occurs more that once in the choices table";

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  fChoicesMap [choiceName] = choice;
}

// S_mnx2mnxsrChoice mnx2mnxsrChoicesTable::lookupChoiceByName (
//   const std::string& name)
// {
//   S_mnx2mnxsrChoice result;
//
//   // is this choiceName in the choices labels set?
//   map<std::string, S_mnx2mnxsrChoice>::const_iterator
//     it =
//       fChoicesMap.find (name);
//
//   if (it != fChoicesMap.end ()) {
//     result = (*it).second;
//   }
//
//   return result;
// }

S_mnx2mnxsrChoice mnx2mnxsrChoicesTable::fetchChoiceByName (
  const std::string& name,
  const mnx2mnxsrDriver&  drv)
{
  S_mnx2mnxsrChoice result;

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Fetching choice named \"" <<
      name <<
      "\" in choices table" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this choiceName in the choices labels set?
  map<std::string, S_mnx2mnxsrChoice>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  return result;
}

S_mnx2mnxsrChoice mnx2mnxsrChoicesTable::fetchChoiceByNameNonConst (
  const std::string& name,
  mnx2mnxsrDriver&        drv)
{
  S_mnx2mnxsrChoice result;

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()) {
    gLog <<
      "====> Fetching choice named \"" <<
      name <<
      "\" in choices table (non-const)" <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // is this choiceName in the choices map?
  map<std::string, S_mnx2mnxsrChoice>::const_iterator
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

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }

  return result;
}

std::string mnx2mnxsrChoicesTable::asString () const
{
  std::stringstream ss;

  ss <<
    "mnx2mnxsrChoicesTable [" <<
    ']';

  return ss.str ();
}

void mnx2mnxsrChoicesTable::displayChoicesMap (std::ostream& os) const // useless ??? JMI
{
  os <<
    "mnx2mnxsrChoicesTable [" <<
    endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (pair<std::string, S_mnx2mnxsrChoice> thePair : fChoicesMap) {
      S_mnx2mnxsrChoice choice = thePair.second;

      choice->
        displayChoiceLabelsToOptionsBlocksMap (os);
    } // for

    --gIndenter;
  }

  os << ']' << endl;
}

void mnx2mnxsrChoicesTable::print (std::ostream& os) const
{
  os <<
    "mnx2mnxsrChoicesTable [" <<
    endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (pair<std::string, S_mnx2mnxsrChoice> thePair : fChoicesMap) {
      S_mnx2mnxsrChoice choice = thePair.second;

      os << choice;
    } // for

    --gIndenter;
  }

  os << ']' << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrChoicesTable& elt)
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
S_mnx2mnxsrCaseChoiceStatement mnx2mnxsrCaseChoiceStatement::create (
  S_mnx2mnxsrChoice caseChoice,
  mnx2mnxsrDriver&  drv)
{
  mnx2mnxsrCaseChoiceStatement* obj =
    new mnx2mnxsrCaseChoiceStatement (
      caseChoice,
      drv);
  assert (obj != nullptr);
  return obj;
}

mnx2mnxsrCaseChoiceStatement::mnx2mnxsrCaseChoiceStatement (
  S_mnx2mnxsrChoice caseChoice,
  mnx2mnxsrDriver&  drv)
{
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    caseChoice != nullptr,
    "caseChoice is NULL");

  fCaseChoice = caseChoice;

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceCaseChoiceStatements ()) {
    gLog <<
      "====> Creating case statement" <<
      ", caseChoice: [" <<
      fCaseChoice->asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // all labels are initially unused
  for (std::string label : fCaseChoice->getLabelsSet ()) {
    fCaseUnusedLabels.insert (label);
  }
}

mnx2mnxsrCaseChoiceStatement::~mnx2mnxsrCaseChoiceStatement ()
{}

void mnx2mnxsrCaseChoiceStatement::registerCaseChoiceLabel (
  const std::string& label,
  mnx2mnxsrDriver&        drv)
{
  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceCaseChoiceStatements ()) {
    gLog <<
      "====> Registering case label \"" <<
      label <<
      "\" in case statement: [" <<
      asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  std::set <std::string>
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

    mnx2mnxsrError (
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

    mnx2mnxsrError (
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

void mnx2mnxsrCaseChoiceStatement::checkThatAllLabelsHaveBeenUsed (
  mnx2mnxsrDriver& drv)
{
  if (fCaseUnusedLabels.size ()) {
    std::stringstream ss;

    ss <<
      "The following labels for choice \"" <<
      fCaseChoice->getChoiceName () <<
      "\" have not been used in this case statement: " <<
      mfStringSetAsString (fCaseUnusedLabels);

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }
}

std::string mnx2mnxsrCaseChoiceStatement::asString () const
{
  std::stringstream ss;

  ss <<
    "mnx2mnxsrCaseChoiceStatement [" <<
    fCaseChoice->asString () <<
    ']';

  return ss.str ();
}

std::string mnx2mnxsrCaseChoiceStatement::currentLabelsListAsString () const
{
  std::stringstream ss;

  ss << "[Labels:";

  for (std::string label : fCaseCurrentLabelsList ) {
    ss << ' ' << label;
  } // for

  ss << ']';

  return ss.str ();
}

void mnx2mnxsrCaseChoiceStatement::print (std::ostream& os) const
{
  os <<
    "mnx2mnxsrCaseChoiceStatement [" <<
    endl;

  ++gIndenter;

  constexpr int fieldWidth = 18;

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

  os << ']' << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrCaseChoiceStatement& elt)
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
S_mnx2mnxsrCaseInputStatement mnx2mnxsrCaseInputStatement::create (
  S_mnx2mnxsrInput caseInput,
  mnx2mnxsrDriver& drv)
{
  mnx2mnxsrCaseInputStatement* obj =
    new mnx2mnxsrCaseInputStatement (
      caseInput,
      drv);
  assert (obj != nullptr);
  return obj;
}

mnx2mnxsrCaseInputStatement::mnx2mnxsrCaseInputStatement (
  S_mnx2mnxsrInput caseInput,
  mnx2mnxsrDriver& drv)
{
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    caseInput != nullptr,
    "caseInput is NULL");

  fCaseInputInput = caseInput;

  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceCaseInputStatements ()) {
    gLog <<
      "====> Creating case statement" <<
      ", caseInput: [" <<
      fCaseInputInput->asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  // all inputs are initially unused
  for (std::string name : fCaseInputInput->getNamesSet ()) {
    fCaseInputUnusedNames.insert (name);
  }
}

mnx2mnxsrCaseInputStatement::~mnx2mnxsrCaseInputStatement ()
{}

void mnx2mnxsrCaseInputStatement::registerCaseInputName (
  const std::string& name,
  mnx2mnxsrDriver&        drv)
{
  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceCaseInputStatements ()) {
    gLog <<
      "====> Registering case name \"" <<
      name <<
      "\" in case statement: [" <<
      asString () <<
      ']' <<
      ", line " << drv.getScannerLocation () <<
      endl;
  }

  std::set <std::string>
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

    mnx2mnxsrError (
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

    mnx2mnxsrError (
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

void mnx2mnxsrCaseInputStatement::checkThatAllNamesHaveBeenUsed (
  mnx2mnxsrDriver& drv)
{
  if (fCaseInputUnusedNames.size ()) {
    std::stringstream ss;

    ss <<
      "The following names for input \"" <<
      fCaseInputInput->getInputName () <<
      "\" have not been used in this case statement: " <<
      mfStringSetAsString (fCaseInputUnusedNames);

    mnx2mnxsrError (
      ss.str (),
      drv.getScannerLocation ());
  }
}

std::string mnx2mnxsrCaseInputStatement::asString () const
{
  std::stringstream ss;

  ss <<
    "mnx2mnxsrCaseInputStatement [" <<
    fCaseInputInput->asString () <<
    ']';

  return ss.str ();
}

std::string mnx2mnxsrCaseInputStatement::currentNamesListAsString () const
{
  std::stringstream ss;

  ss << "[Inputs:";

  for (std::string name : fCaseInputCurrentNamesList ) {
    ss << ' ' << name;
  } // for

  ss << ']';

  return ss.str ();
}

void mnx2mnxsrCaseInputStatement::print (std::ostream& os) const
{
  os <<
    "mnx2mnxsrCaseInputStatement [" <<
    endl;

  ++gIndenter;

  constexpr int fieldWidth = 18;

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

  os << ']' << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrCaseInputStatement& elt)
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
// void initializeMNX2MNXSRBasicTypes ()
// {
//   // protect library against multiple initializations
//   static Bool pPrivateThisMethodHasBeenRun (false);
//
//   if (! pPrivateThisMethodHasBeenRun) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah () && ! gEarlyOptions.getEarlyQuietOption ()) {
//       gLog <<
//         "Initializing MNX2MNXSR basic types handling" <<
//         endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
