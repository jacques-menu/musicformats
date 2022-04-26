/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrPartGroups.h"

#include "msrParts.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;


namespace MusicFormats
{

//______________________________________________________________________________
S_msrPartGroup msrPartGroup::create (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const string&            partGroupName,
  const string&            partGroupNameDisplayText,
  const string&            partGroupAccidentalText,
  const string&            partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupImplicitKind partGroupImplicitKind,
  msrPartGroupBarLineKind  partGroupBarLineKind,
  S_msrPartGroup           partGroupPartGroupUpLink,
  S_msrScore               partGroupScoreUpLink)
{
  msrPartGroup* o =
    new msrPartGroup (
      inputLineNumber,
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupNameDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      partGroupSymbolKind,
      partGroupSymbolDefaultX,
      partGroupImplicitKind,
      partGroupBarLineKind,
      partGroupPartGroupUpLink,
      partGroupScoreUpLink);
  assert (o != nullptr);

  return o;
}

S_msrPartGroup msrPartGroup::createImplicitPartGroup (
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const string&            partGroupName,
  const string&            partGroupNameDisplayText,
  const string&            partGroupAccidentalText,
  const string&            partGroupAbbreviation,
  msrPartGroupBarLineKind  partGroupBarLineKind,
  S_msrScore               partGroupScoreUpLink)
{
  msrPartGroup* o =
    new msrPartGroup (
      K_NO_INPUT_LINE_NUMBER,
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupNameDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      msrPartGroup::kPartGroupSymbolNone, // partGroupSymbolKind
      0,                                  // partGroupSymbolDefaultX,
      msrPartGroup::kPartGroupImplicitYes,
      partGroupBarLineKind,
      nullptr,                            // partGroupPartGroupUpLink,
                                          // will be set below
      partGroupScoreUpLink);
  assert (o != nullptr);

  // the implicit part group it the top-most one:
  // set its group upLink to point to itself
  o->fPartGroupPartGroupUpLink = o;

  return o;
}

S_msrPartGroup msrPartGroup::create (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const string&            partGroupName,
  S_msrPartGroup           partGroupPartGroupUpLink,
  S_msrScore               partGroupScoreUpLink)
{
  msrPartGroup* o =
    new msrPartGroup (
      inputLineNumber,
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupPartGroupUpLink,
      partGroupScoreUpLink);
  assert (o != nullptr);
  return o;
}

msrPartGroup::msrPartGroup (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const string&            partGroupName,
  const string&            partGroupNameDisplayText,
  const string&            partGroupAccidentalText,
  const string&            partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupImplicitKind partGroupImplicitKind,
  msrPartGroupBarLineKind  partGroupBarLineKind,
  S_msrPartGroup           partGroupPartGroupUpLink,
  S_msrScore               partGroupScoreUpLink)
    : msrPartGroupElement (inputLineNumber)
{
  // no sanity check on partGroupPartGroupUpLink here,
  // it will be set after all 'real' (i.e. not implicit)
  // part groups and part have been analyzed
  fPartGroupPartGroupUpLink = partGroupPartGroupUpLink;

/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fPartGroupScoreUpLink != nullptr,
    "fPartGroupScoreUpLink is null");
    */

  fPartGroupScoreUpLink     = partGroupScoreUpLink;

  // other fields
  fPartGroupNumber          = partGroupNumber;
  fPartGroupAbsoluteNumber  = partGroupAbsoluteNumber;

  fPartGroupName            = partGroupName;

  size_t partGroupNameLength =
    fPartGroupName.size ();

  if (
    partGroupNameLength
      >
    fPartGroupScoreUpLink->getScorePartGroupNamesMaxLength ()
  ) {  // JMI sanity check ???
    fPartGroupScoreUpLink->
      setScorePartGroupNamesMaxLength (
        partGroupNameLength);
  }

  fPartGroupNameDisplayText = partGroupNameDisplayText;

  fPartGroupAccidentalText  = partGroupAccidentalText;

  fPartGroupAbbreviation    = partGroupAbbreviation;

  fPartGroupSymbolKind      = partGroupSymbolKind;
  fPartGroupSymbolDefaultX  = partGroupSymbolDefaultX;

  fPartGroupImplicitKind    = partGroupImplicitKind;

  fPartGroupBarLineKind     = partGroupBarLineKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "--------------------------------------------" <<
      endl <<
      "Creating part group '" << fPartGroupNumber << "'" <<
      ", partGroupAbsoluteNumber = " << fPartGroupAbsoluteNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

msrPartGroup::msrPartGroup (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const string&            partGroupName,
  S_msrPartGroup           partGroupPartGroupUpLink,
  S_msrScore               partGroupScoreUpLink)
    : msrPartGroupElement (inputLineNumber)
{
  // no sanity check on partGroupPartGroupUpLink here,
  // it will be set after all 'real' (i.e. not implicit)
  // part groups and part have been analyzed
  fPartGroupPartGroupUpLink = partGroupPartGroupUpLink;

/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fPartGroupScoreUpLink != nullptr,
    "fPartGroupScoreUpLink is null");
    */

  fPartGroupScoreUpLink     = partGroupScoreUpLink;

  // other fields
  fPartGroupNumber          = partGroupNumber;
  fPartGroupAbsoluteNumber  = partGroupAbsoluteNumber;

  fPartGroupName            = partGroupName;

  size_t partGroupNameLength =
    fPartGroupName.size ();

  if (
    partGroupNameLength
      >
    fPartGroupScoreUpLink->getScorePartGroupNamesMaxLength ()
  ) {  // JMI sanity check ???
    fPartGroupScoreUpLink->
      setScorePartGroupNamesMaxLength (
        partGroupNameLength);
  }

/* JMI
  fPartGroupNameDisplayText = partGroupNameDisplayText;

  fPartGroupAccidentalText  = partGroupAccidentalText;

  fPartGroupAbbreviation    = partGroupAbbreviation;

  fPartGroupSymbolKind      = partGroupSymbolKind;
  fPartGroupSymbolDefaultX  = partGroupSymbolDefaultX;

  fPartGroupImplicitKind    = partGroupImplicitKind;

  fPartGroupBarLineKind     = partGroupBarLineKind;
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "--------------------------------------------" <<
      endl <<
      "Creating part group '" << fPartGroupNumber << "'" <<
      ", partGroupAbsoluteNumber = " << fPartGroupAbsoluteNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

msrPartGroup::~msrPartGroup ()
{}

S_msrPartGroup msrPartGroup::createPartGroupNewbornClone (
  S_msrPartGroup partGroupClone,
  S_msrScore     scoreClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "--------------------------------------------" <<
      endl <<
      "Creating a newborn clone part group " <<
      getPartGroupCombinedName () <<
      endl;
  }
#endif

  // don't check partGroupClone against 0, since the partGroup stack JMI
  // that it comes from may be empty
/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    partGroupClone != nullptr,
    "partGroupClone is null");
    */

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    scoreClone != nullptr,
    "scoreClone is null");

  S_msrPartGroup
    newbornClone =
      msrPartGroup::create (
        fInputLineNumber,
        fPartGroupNumber,
        fPartGroupAbsoluteNumber,
        fPartGroupName,
        fPartGroupNameDisplayText,
        fPartGroupAccidentalText,
        fPartGroupAbbreviation,
        fPartGroupSymbolKind,
        fPartGroupSymbolDefaultX,
        fPartGroupImplicitKind,
        fPartGroupBarLineKind,
        partGroupClone,
        scoreClone);

  newbornClone->fPartGroupInstrumentName =
    fPartGroupInstrumentName;

  return newbornClone;
}

string msrPartGroup::getPartGroupCombinedName () const
{
  stringstream s;

  s <<
    "PartGroup_" << fPartGroupAbsoluteNumber <<
    " ('" << fPartGroupNumber <<
    "', fPartGroupName \"" << fPartGroupName << "\")";

  return s.str ();
}

string msrPartGroup::getPartGroupCombinedNameWithoutEndOfLines () const
{
  list<string> chunksList;

  mfSplitRegularStringAtEndOfLines (
    fPartGroupName,
    chunksList);

  stringstream s;

  s <<
    "PartGroup_" << fPartGroupAbsoluteNumber <<
    " ('" << fPartGroupNumber <<
    "', partGroupName \"";

  if (chunksList.size ()) {
    // used the chunks separated by a space
    list<string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s <<(*i);
      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  s <<
    "\"" <<
    ", fPartGroupImplicitKind: " <<
    partGroupImplicitKindAsString ();

  return s.str ();
}

void msrPartGroup::setPartGroupInstrumentName (
  const string& partGroupInstrumentName)
{
  fPartGroupInstrumentName = partGroupInstrumentName;

  S_msrScore
    score =
      fPartGroupScoreUpLink;

  size_t partGroupInstrumentNameLength =
    fPartGroupInstrumentName.size ();

  if (
    partGroupInstrumentNameLength
      >
    score->getScoreInstrumentNamesMaxLength ()
  ) {
    score->
      setIdentificationScoreInstrumentNamesMaxLength (
        partGroupInstrumentNameLength);
  }
}

void msrPartGroup::checkPartGroupElement (
  S_msrPartGroupElement partGroupElement) const // TEMP JMI v0.9.63
{
  // sanity check
  if (
    ((void*) partGroupElement) == (void*) 0x0000000000000001
      ||
    ((void*) partGroupElement) == (void*) 0x0000000000000011
  ) {
    stringstream s;

    s <<
      "###### partGroupElement is " <<
      (void*) partGroupElement <<
      " ######"; // JMI v0.9.63

    msrInternalError(
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber, // inputLineNumber // TEMP JMI v0.9.63
      __FILE__, __LINE__,
      s.str ());
  }
}

S_msrPart msrPartGroup::appendPartToPartGroupByItsPartID (
  int           inputLineNumber,
  const string& partID)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    partID.size () > 0,
    "partID is empty");

  // has this partID already been added to this part?
  if (fPartGroupPartsMap.count (partID)) {
    stringstream s;

    s <<
      "partID \"" << partID <<
      "\" already exists in part group " <<
      getPartGroupCombinedName ();

    msrWarning ( // JMI
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());

    return fPartGroupPartsMap [partID];
  }

  // create the part
  S_msrPart
    part =
      msrPart::create (
        inputLineNumber,
        partID,
        this);

  // register it in this part group
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Appending part " <<
      part->getPartCombinedName () <<
      " to part group '" <<
      asString () <<
      "'" <<
      endl;

    this->printShort (gLogStream);
    part->printShort (gLogStream);
  }
#endif

  // sanity check
  checkPartGroupElement (part);

  fPartGroupPartsMap [partID] = part;

  fPartGroupElementsList.push_back (part);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    gLogStream <<
      endl <<
      "After appendPartToPartGroupByItsID, fPartGroupPartsMap contains:" <<
      endl;

    ++gIndenter;

    for (
        map<string, S_msrPart>::const_iterator i = fPartGroupPartsMap.begin ();
        i != fPartGroupPartsMap.end ();
        ++i
      ) {
      gLogStream <<
        "\"" << (*i).first << "\" --% --> " <<
        (*i).second->
          getPartCombinedName () <<
        endl;
    } // for

    --gIndenter;

    gLogStream <<
      "After appendPartToPartGroupByItsID, fPartGroupElementsList contains:" <<
      endl;

    ++gIndenter;

    if (fPartGroupElementsList.size ()) {
      list<S_msrPartGroupElement>::const_iterator
        iBegin = fPartGroupElementsList.begin (),
        iEnd   = fPartGroupElementsList.end (),
        i      = iBegin;

      for ( ; ; ) {
        gLogStream <<
          (*i);
        if (++i == iEnd) break;
        gLogStream << endl;
      } // for
    }

    --gIndenter;
  }
#endif

  // return the part
  return part;
}

void msrPartGroup::appendPartToPartGroup (S_msrPart part)
{
  // register part in this part group
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << asString () <<
      endl;

    this->printShort (gLogStream);
    part->printShort (gLogStream);
  }
#endif

  // register part into this part group's data
  fPartGroupPartsMap [part->getPartID ()] = part;

  // sanity check
  checkPartGroupElement (part);

  fPartGroupElementsList.push_back (part);

  // set part's partgroup upLink
  part->setPartPartGroupUpLink (this);
}

void msrPartGroup::removePartFromPartGroup (
  int       inputLineNumber,
  S_msrPart partToBeRemoved)
{
  // register part in this part group
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Removing part " <<
      partToBeRemoved->getPartCombinedName () <<
      " from part group " << asString () <<
      endl;

    this->printShort (gLogStream);
    partToBeRemoved->printShort (gLogStream);
  }
#endif

  ++gIndenter;

  for (
    list<S_msrPartGroupElement>::iterator i = fPartGroupElementsList.begin ();
    i != fPartGroupElementsList.end ();
    ++i
  ) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        nestedPartGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
    ) {
      // this is a part
      if (part == partToBeRemoved) {
        i = fPartGroupElementsList.erase (i);
        break;
      }
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for

  --gIndenter;
}

void msrPartGroup::prependSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Prepending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;

    this->printShort (gLogStream);
    partGroup->printShort (gLogStream);
  }
#endif

  // register it in this part group

  // sanity check
  checkPartGroupElement (partGroup);

  fPartGroupElementsList.push_front (partGroup);
}

void msrPartGroup::appendSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Appending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;

    this->printShort (gLogStream);
    partGroup->printShort (gLogStream);
  }
#endif

  // register it in this part group

  // sanity check
  checkPartGroupElement (partGroup);

  fPartGroupElementsList.push_back (partGroup);
}

void msrPartGroup::printPartGroupElementsList (
  int      inputLineNumber,
  ostream& os) const
{
  os <<
    "fPartGroupElementsList:";

  if (fPartGroupElementsList.size ()) {
    os << endl;
    ++gIndenter;

    list<S_msrPartGroupElement>::const_iterator
      iBegin = fPartGroupElementsList.begin (),
      iEnd   = fPartGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      if (
        (void*) (*i) == (void*) 0x11
          ||
        (void*) (*i) == (void*) 0x1
          ||
        (void*) (*i) == (void*) 0x9
          ||
        (void*) (*i) == (void*) 0x0000000000000001
          ||
        (void*) (*i) == (void*) 0x0000000000000011
          ||
        (void*) (*i) <  (void*) 0x0000000001000000
      ) {
        stringstream s;

        s <<
          "###### (*i) is " <<
          (*i) <<
          " ######"; // JMI v0.9.63

        msrInternalWarning (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber, // inputLineNumber // TEMP JMI v0.9.63
//           __FILE__, __LINE__,
          s.str ());
      }

      else {
        S_msrElement
          element = (*i);

        if (
          S_msrPartGroup
            nestedPartGroup =
              dynamic_cast<msrPartGroup*>(&(*element))
          ) {
          // this is a part group
          gLogStream <<
            nestedPartGroup->
              getPartGroupCombinedNameWithoutEndOfLines () <<
            endl;

          ++gIndenter;

          nestedPartGroup->
            printPartGroupElementsList (
              inputLineNumber,
              os);

          --gIndenter;
        }

        else if (
          S_msrPart
            part =
              dynamic_cast<msrPart*>(&(*element))
          ) {
          // this is a part
          gLogStream <<
            part->
              getPartCombinedName () <<
            endl;
        }

        else {
          stringstream s;

          s <<
            "an element of partgroup " <<
            getPartGroupCombinedName () <<
            " is not a part group nor a part";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }

      if (++i == iEnd) break;
   // JMI   os << endl;
    } // for

    --gIndenter;
  }

  else {
    os << ' ' << "none" << endl;
  }
}

S_msrPart msrPartGroup::fetchPartFromPartGroupByItsPartID (
  int           inputLineNumber,
  const string& partID)
{
  S_msrPart result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    gLogStream <<
      "fetchPartFromPartGroupByItsPartID(" << partID <<
      "), fPartGroupElementsList contains:" <<
      endl;

    ++gIndenter;

    printPartGroupElementsList (
      inputLineNumber,
      gLogStream);

    --gIndenter;

    gLogStream <<
      "<=- fetchPartFromPartGroupByItsPartID(" << partID << ")" <<
      endl << endl;
  }
#endif

  for (
    list<S_msrPartGroupElement>::const_iterator i =
      fPartGroupElementsList.begin ();
    i != fPartGroupElementsList.end ();
    ++i
  ) {
    S_msrPartGroupElement
      element = (*i);

    if (
      S_msrPartGroup
        partGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group
      S_msrPart
        inter =
          partGroup->
            fetchPartFromPartGroupByItsPartID (
              inputLineNumber,
              partID);

      if (inter) {
        result = inter;
        break;
      }
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part
      if (part->getPartID () == partID) {
        result = part;
        break;
      }
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for

  return result;
}

void msrPartGroup::collectPartGroupPartsList (
  int              inputLineNumber,
  list<S_msrPart>& partsList)
{
  for (
    list<S_msrPartGroupElement>::const_iterator i =
      fPartGroupElementsList.begin ();
    i != fPartGroupElementsList.end ();
    ++i
  ) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        partGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group
      partGroup->
        collectPartGroupPartsList (
          inputLineNumber,
          partsList);
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part

      // sanity check
      checkPartGroupElement (part);

      partsList.push_back (part);
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for
}

void msrPartGroup::registerVoiceInPartGroupAllVoicesList (
  S_msrVoice voice)
{
  // register voice in this staff
  fPartGroupAllVoicesList.push_back (voice);

  // register it in the partgroup uplink
  fPartGroupScoreUpLink->
    registerVoiceInScoreAllVoicesList (voice);
}

void msrPartGroup::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPartGroup::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPartGroup::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPartGroup::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPartGroup::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPartGroup::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPartGroup::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPartGroup::browseData ()" <<
      endl;
  }

  for (S_msrPartGroupElement partGroupElement : fPartGroupElementsList) {
    // browse the part group element
    msrBrowser<msrElement> browser (v);
    browser.browse (*partGroupElement);
  } // for

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrPartGroup::browseData ()" <<
      endl;
  }
}

string msrPartGroup::partGroupImplicitKindAsString (
  msrPartGroupImplicitKind partGroupImplicitKind)
{
  string result;

  switch (partGroupImplicitKind) {
    case msrPartGroup::kPartGroupImplicitYes:
      result = "kPartGroupImplicitYes";
      break;
    case msrPartGroup::kPartGroupImplicitNo:
      result = "kPartGroupImplicitNo";
      break;
  } // switch

  return result;
}

string msrPartGroup::partGroupImplicitKindAsString () const
{
  return
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind);
}

string msrPartGroup::partGroupTypeKindAsString (
  msrPartGroupTypeKind partGroupTypeKind)
{
  string result;

  switch (partGroupTypeKind) {
    case msrPartGroup::kPartGroupTypeNone:
      result = "kPartGroupTypeNone";
      break;
    case msrPartGroup::kPartGroupTypeStart:
      result = "kPartGroupTypeStart";
      break;
    case msrPartGroup::kPartGroupTypeStop:
      result = "kPartGroupTypeStop";
      break;
  } // switch

  return result;
}

string msrPartGroup::partGroupBarLineKindAsString () const
{
  return
    partGroupBarLineKindAsString (
      fPartGroupBarLineKind);
}

string msrPartGroup::asString () const
{
  stringstream s;

  s <<
    "PartGroup \"" <<
    getPartGroupCombinedName () <<
    "\", line " << fInputLineNumber;

  return s.str ();
}

string msrPartGroup::partGroupSymbolKindAsString (
  msrPartGroupSymbolKind partGroupSymbolKind)
{
  string result;

  switch (partGroupSymbolKind) {
    case msrPartGroup::kPartGroupSymbolNone:
      result = "kPartGroupSymbolNone";
      break;
    case msrPartGroup::kPartGroupSymbolBrace:
      result = "kPartGroupSymbolBrace";
      break;
    case msrPartGroup::kPartGroupSymbolBracket:
      result = "kPartGroupSymbolBracket";
      break;
    case msrPartGroup::kPartGroupSymbolLine:
      result = "kPartGroupSymbolLine";
      break;
    case msrPartGroup::kPartGroupSymbolSquare:
      result = "kPartGroupSymbolSquare";
      break;
  } // switch

  return result;
}

string msrPartGroup::partGroupBarLineKindAsString (
  msrPartGroupBarLineKind partGroupBarLineKind)
{
  string result;

  switch (partGroupBarLineKind) {
    case msrPartGroup::kPartGroupBarLineYes:
      result = "kPartGroupBarLineYes";
      break;
    case msrPartGroup::kPartGroupBarLineNo:
      result = "kPartGroupBarLineNo";
      break;
  } // switch

  return result;
}

void msrPartGroup::print (ostream& os) const
{
  os <<
    "[PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    mfSingularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 25;

  os << left <<
    setw (fieldWidth) <<
    "fPartGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fPartGroupPartGroupUpLink" << " : ";
  if (fPartGroupPartGroupUpLink) {
    os <<
      "\"" <<
      fPartGroupPartGroupUpLink->
        getPartGroupCombinedName () <<
      "\"";
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
   setw (fieldWidth) <<
    "fPartGroupNameDisplayText" << " : \"" <<
    fPartGroupNameDisplayText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartGroupAccidentalText" << " : \"" <<
    fPartGroupAccidentalText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartGroupAbbrevation" << " : \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartGroupSymbolDefaultX" << " : " <<
    fPartGroupSymbolDefaultX <<
      endl <<
    setw (fieldWidth) <<
    "fPartGroupSymbolKind" << " : " <<
    partGroupSymbolKindAsString (
      fPartGroupSymbolKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fPartGroupImplicitKind" << " : " <<
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fPartGroupBarLineKind" << " : " <<
    partGroupBarLineKindAsString (
      fPartGroupBarLineKind) <<
    endl;

  os << endl;

  // print all the voices if any
  size_t partGroupAllVoicesListSize =
    fPartGroupAllVoicesList.size ();

  os << left <<
    setw (fieldWidth) <<
    "fPartGroupAllVoicesList" << " : ";
  if (partGroupAllVoicesListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrVoice>::const_iterator
      iBegin = fPartGroupAllVoicesList.begin (),
      iEnd   = fPartGroupAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << "\"" << voice->getVoiceName () << "\"" << endl;
      if (++i == iEnd) break;
      // os << endl;
    } // for
//    os << endl;

    --gIndenter;
  }
  else {
    os << "none" << endl;
  }

  // print the part group elements if any

  printPartGroupElementsList (
    fInputLineNumber,
    gLogStream);

  --gIndenter;

  os << ']' << endl;
}

void msrPartGroup::printShort (ostream& os) const
{
  os <<
    "[PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    mfSingularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 25;

  os << left <<
    setw (fieldWidth) <<
    "fPartGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl;

  // print the part group elements if any
  printPartGroupElementsList (
    fInputLineNumber,
    gLogStream);

  --gIndenter;

  os << ']' << endl;
}

void msrPartGroup::printSummary (ostream& os) const
{
  os <<
    "[PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    mfSingularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 24;

  os << left <<
    setw (fieldWidth) <<
    "fPartGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartGroupAbbrevation" << " : \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartGroupSymbolDefaultX" << " : " <<
    fPartGroupSymbolDefaultX <<
      endl <<
    setw (fieldWidth) <<
    "fPartGroupSymbolKind" << " : \"" <<
    partGroupSymbolKindAsString (
      fPartGroupSymbolKind) <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartGroupImplicit" << " : " <<
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind) <<
    endl <<

    setw (fieldWidth) <<
    "fPartGroupBarLine" << " : " <<
    partGroupBarLineKindAsString (
      fPartGroupBarLineKind) <<
    endl;

  if (fPartGroupElementsList.size ()) {
    os << endl;
    list<S_msrPartGroupElement>::const_iterator
      iBegin = fPartGroupElementsList.begin (),
      iEnd   = fPartGroupElementsList.end (),
      i      = iBegin;

    ++gIndenter;
    for ( ; ; ) {
      (*i)->printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrPartGroup::printSlices (ostream& os) const
{
  os << "MSR part group slices";

  os << endl << endl;

  ++gIndenter;

//   if (fPartGroupElementsList.size ()) {
//     list<S_msrPartGroupElement>::const_iterator
//       iBegin = fPartGroupElementsList.begin (),
//       iEnd   = fPartGroupElementsList.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       (*i)->
//         printSlices (os);
//       if (++i == iEnd) break;
//       os << endl;
//     } // for
//   }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrPartGroup& elt)
{
  elt->print (os);
  return os;
}


}
