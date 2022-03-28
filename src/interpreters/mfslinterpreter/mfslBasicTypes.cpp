/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

// #include <stddef>       // size_t JMI

#include "mfAssert.h"
#include "mfBool.h"
#include "mfStringsHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "mfslBasicTypes.h"
#include "mfslDriver.h"

#include "mfslInterpreterOah.h"

#include "mfslWae.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
S_mfslOptionsBlock mfslOptionsBlock::create (const string& optionsBlockName)
{
  mfslOptionsBlock* o =
    new mfslOptionsBlock (
      optionsBlockName);
  assert (o != nullptr);
  return o;
}

mfslOptionsBlock::mfslOptionsBlock (const string& optionsBlockName)
{
  fOptionsBlockName = optionsBlockName;
}

mfslOptionsBlock::~mfslOptionsBlock ()
{}

void mfslOptionsBlock::registerOptionsInOptionsBlock (S_oahOption option)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceOptionsBlocks ()) {
    gLogStream <<
      "====> Registering option [" <<
      option->asString () <<
      "] in options block \"" <<
      fOptionsBlockName <<
      "\"" <<
      endl;
  }

  fOptionsBlockOptionsVector.push_back (
    option);
}

string mfslOptionsBlock::asString () const
{
  stringstream s;

  s <<
    "mfslOptionsBlock [" <<
    "fOptionsBlockName: " << fOptionsBlockName <<
    "]";

  return s.str ();
}

string mfslOptionsBlock::asOptionsString () const
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

void mfslOptionsBlock::print (ostream& os) const
{
  os <<
    "mfslOptionsBlock [" <<
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

  os << "]" << endl;
}

ostream& operator<< (ostream& os, const S_mfslOptionsBlock& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
string mfslChoiceLabelKindAsString (
  mfslChoiceLabelKind choiceLabelKind)
{
  string result;

  switch (choiceLabelKind) {
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

ostream& operator<< (ostream& os, const mfslChoiceLabelKind& elt)
{
  os << mfslChoiceLabelKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_mfslChoice mfslChoice::create (
  const string& choiceName)
{
  mfslChoice* o =
    new mfslChoice (
      choiceName);
  assert (o != nullptr);
  return o;
}

mfslChoice::mfslChoice (
  const string& choiceName)
{
  fChoiceName = choiceName;

  fChoiceLabelKind =
    mfslChoiceLabelKind::kChoiceLabelNone;
}

mfslChoice::~mfslChoice ()
{}

void mfslChoice::setChoiceLabelSuppliedByAnOption (
  const string& label)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
    gLogStream <<
      "====> Setting the label of choice \"" <<
      fChoiceName <<
      "\" to \"" <<
      label <<
      "\", supplied by an option" <<
      endl;
  }

  // no checks here, the labels are not knoown yet
  fChoiceLabel = label;
}

void mfslChoice::selectChoiceLabel (
  const string& label,
  mfslDriver&   drv)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
    gLogStream <<
      "====> Setting the label of choice \"" <<
      fChoiceName <<
      "\" to \"" <<
      label <<
      "\"" <<
      endl;
  }

  // is this label in the choice's labels set?
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

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  // did fChoiceName get a label by an option?
  const map<string, string>&
    setChoiceToLabelMap =
      gGlobalMfslInterpreterOahGroup->
        getGenerateChoiceToLabelsMap ();

  switch (fChoiceLabelKind) {
    case mfslChoiceLabelKind::kChoiceLabelNone:
      {
        // store into the choice
        string optionSuppliedLabel;

        Bool
          choiceGotAValueByAnOption =
            mfStringIsInStringToStringMap (
              fChoiceName,
              setChoiceToLabelMap,
              optionSuppliedLabel);

        if (choiceGotAValueByAnOption) {
          if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
            gLogStream <<
              "====> Choice name \"" <<
              fChoiceName <<
              "\" has been selected as \"" <<
              optionSuppliedLabel <<
              "\" by an option supplied to the script" <<
              ", ignoring label \"" <<
              label <<
              "\"" <<
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

string mfslChoice::getChoiceLabel (mfslDriver& drv) const
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
        gLogStream <<
          "====> " <<
          " choice \"" <<
          fChoiceName <<
          ", label " <<
          fChoiceLabel <<
          "\", supplied to the script" <<
          endl;
      }
      break;

    case mfslChoiceLabelKind::kChoiceLabelSetInScript:
      if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
        gLogStream <<
          "====>" <<
          "choice \"" <<
          fChoiceName <<
          "\" has label \"" <<
          fChoiceLabel <<
          " \", set in the script" <<
          endl;
      }
      break;
  } // switch

  return fChoiceLabel;
}

string mfslChoice::getChoiceLabelWithoutTrace (mfslDriver& drv) const
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
  const string& label,
  mfslDriver&   drv) const
{
  S_mfslOptionsBlock result;

  // is this label in the choice's labels to options block map?
  map<string, S_mfslOptionsBlock>::const_iterator
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
      "\"";

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

void mfslChoice::addLabel (
  const string& label,
  mfslDriver&   drv)
{
  // is this label in the choice's labels set?
  set<string>::const_iterator
    it =
      fLabelsSet.find (label);

  if (it != fLabelsSet.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "choice label \"" <<
      label <<
      "\" occurs more that once in the \"" <<
      fChoiceName <<
      "\" choice";

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  else {
    if (gGlobalMfslInterpreterOahGroup->getTraceChoices ()) {
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

    // register this choice's label
    fLabelsSet.insert (label);

    // create the options block for it
    fChoiceLabelsToOptionsBlocksMap [label] =
      mfslOptionsBlock::create (
        label);
  }
}

void mfslChoice::enrichLabelOptionsBlock (
  const string&      label,
  S_mfslOptionsBlock optionsBlock)
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
      endl;
  }

  vector<S_oahOption>&
    labelOptionsBlockOptionsVector =
      labelOptionsBlock->
        getOptionsBlockOptionsVectorToModify ();

  const vector<S_oahOption>&
    optionsBlockOptionsVector =
      optionsBlock->
        getOptionsBlockOptionsVector ();

  optionsNameAndValueVectorsPlusEquals (
    labelOptionsBlockOptionsVector,
    optionsBlockOptionsVector);
}

void mfslChoice::registerChoiceDefaultLabel (
  const string& label,
  mfslDriver&   drv)
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

string mfslChoice::labelsSetAsString () const
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

string mfslChoice::asString () const
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

void mfslChoice::displayChoiceLabelsToOptionsBlocksMap (ostream& os) const
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
    for (pair<string, S_mfslOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      string        key = thePair.first;
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

void mfslChoice::print (ostream& os) const
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
    int counter = 0;
    for (string label : fLabelsSet) {
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
    for (pair<string, S_mfslOptionsBlock> thePair : fChoiceLabelsToOptionsBlocksMap) {
      string        key = thePair.first;
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

ostream& operator<< (ostream& os, const S_mfslChoice& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_mfslChoicesTable mfslChoicesTable::create ()
{
  mfslChoicesTable* o =
    new mfslChoicesTable ();
  assert (o != nullptr);
  return o;
}

mfslChoicesTable::mfslChoicesTable ()
{}

mfslChoicesTable::~mfslChoicesTable ()
{}

void mfslChoicesTable::addChoice (
  S_mfslChoice choice,
  mfslDriver&  drv)
{
  string
    choiceName =
      choice->getChoiceName ();

  // is this choiceName in the choice's labels set?
  map<string, S_mfslChoice>::const_iterator
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

S_mfslChoice mfslChoicesTable::lookupChoiceByName (
  const string& name)
{
  S_mfslChoice result;

  // is this choiceName in the choice's labels set?
  map<string, S_mfslChoice>::const_iterator
    it =
      fChoicesMap.find (name);

  if (it != fChoicesMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mfslChoice mfslChoicesTable::fetchChoiceByName (
  const string&     name,
  const mfslDriver& drv)
{
  S_mfslChoice result;

  // is this choiceName in the choice's labels set?
  map<string, S_mfslChoice>::const_iterator
    it =
      fChoicesMap.find (name);

  if (it != fChoicesMap.end ()) {
    result = (*it).second;
  }

  else {
    stringstream s;

    s <<
      "choice \"" << name <<
      "\" is unknown in choice table";

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

S_mfslChoice mfslChoicesTable::fetchChoiceByNameToMofidy (
  const string&    name,
  const mfslDriver drv)
{
  S_mfslChoice result;

  // is this choiceName in the choice's labels set?
  map<string, S_mfslChoice>::const_iterator
    it =
      fChoicesMap.find (name);

  if (it != fChoicesMap.end ()) {
    result = (*it).second;
  }

  else {
    stringstream s;

    s <<
      "choice \"" << name <<
      "\" is unknown in choice table";

    mfslError (
      s.str (),
      drv.getScannerLocation ());
  }

  return result;
}

string mfslChoicesTable::asString () const
{
  stringstream s;

  s <<
    "mfslChoicesTable [" <<
    "]";

  return s.str ();
}

void mfslChoicesTable::displayChoicesMap (ostream& os) const // useless ??? JMI
{
  os <<
    "mfslChoicesTable [" <<
    endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (pair<string, S_mfslChoice> thePair : fChoicesMap) {
      S_mfslChoice choice = thePair.second;

      choice->
        displayChoiceLabelsToOptionsBlocksMap (os);
    } // for

    --gIndenter;
  }

  os << "]" << endl;
}

void mfslChoicesTable::print (ostream& os) const
{
  os <<
    "mfslChoicesTable [" <<
    endl;

  if (fChoicesMap.size ()) {
    ++gIndenter;

    for (pair<string, S_mfslChoice> thePair : fChoicesMap) {
      S_mfslChoice choice = thePair.second;

      os << choice;
    } // for

    --gIndenter;
  }

  os << "]" << endl;
}

ostream& operator<< (ostream& os, const S_mfslChoicesTable& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_mfslCaseStatement mfslCaseStatement::create (
  S_mfslChoice caseChoice)
{
  mfslCaseStatement* o =
    new mfslCaseStatement (
      caseChoice);
  assert (o != nullptr);
  return o;
}

mfslCaseStatement::mfslCaseStatement (
  S_mfslChoice caseChoice)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    caseChoice != nullptr,
    "caseChoice is null");

  fCaseChoice = caseChoice;

  if (gGlobalMfslInterpreterOahGroup->getTraceCaseStatements ()) {
    gLogStream <<
      "====> Creating case statement" <<
      ", caseChoice: [" <<
      fCaseChoice->asString () <<
      "]" <<
      endl;
  }

  // all labels are initially unused
  for (string label : fCaseChoice->getLabelsSet ()) {
    fCaseUnusedLabels.insert (label);
  }
}

mfslCaseStatement::~mfslCaseStatement ()
{}

void mfslCaseStatement::registerCaseLabel (
  const string& label,
  mfslDriver&   drv)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceCaseStatements ()) {
    gLogStream <<
      "====> Registering case label \"" <<
      label <<
      "\" in case statement: [" <<
      asString () <<
      "]" <<
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

void mfslCaseStatement::checkThatAllLabelsHaveBeenUsed (
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

string mfslCaseStatement::asString () const
{
  stringstream s;

  s <<
    "mfslCaseStatement [" <<
    fCaseChoice->asString () <<
    "]";

  return s.str ();
}

string mfslCaseStatement::currentLabelsListAsString () const
{
  stringstream s;

  s << "[Labels:";

  for (string label : fCaseCurrentLabelsList ) {
    s << ' ' << label;
  } // for

  s << ']';

  return s.str ();
}

void mfslCaseStatement::print (ostream& os) const
{
  os <<
    "mfslCaseStatement [" <<
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

  os << "]" << endl;
}

ostream& operator<< (ostream& os, const S_mfslCaseStatement& elt)
{
  elt->print (os);
  return os;
}

// //______________________________________________________________________________
// void initializeMFSLBasicTypes ()
// {
//   // protect library against multiple initializations
//   static Bool pPrivateThisMethodHasBeenRun (false);
//
//   if (! pPrivateThisMethodHasBeenRun) {
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalOahEarlyOptions.getEarlyTracingOah () && ! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
//       gLogStream <<
//         "Initializing MFSL basic types handling" <<
//         endl;
//     }
// #endif
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
