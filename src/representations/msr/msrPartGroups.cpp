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

#include "mfConstants.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrPartGroups.h"

#include "msrParts.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"



namespace MusicFormats
{

//______________________________________________________________________________
S_msrPartGroup msrPartGroup::create (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const std::string&       partGroupName,
  const std::string&       partGroupNameDisplayText,
  const std::string&       partGroupAccidentalText,
  const std::string&       partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupImplicitKind partGroupImplicitKind,
  msrPartGroupBarLineKind  partGroupBarLineKind,
  const S_msrPartGroup&    partGroupUpLinkToPartGroup,
  const S_msrScore&        partGroupUpLinkToScore)
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
      partGroupUpLinkToPartGroup,
      partGroupUpLinkToScore);
  assert (o != nullptr);
  return o;
}

S_msrPartGroup msrPartGroup::createImplicitPartGroup (
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const std::string&       partGroupName,
  const std::string&       partGroupNameDisplayText,
  const std::string&       partGroupAccidentalText,
  const std::string&       partGroupAbbreviation,
  msrPartGroupBarLineKind  partGroupBarLineKind,
  const S_msrScore&        partGroupUpLinkToScore)
{
  msrPartGroup* o =
    new msrPartGroup (
      K_MF_NO_INPUT_LINE_NUMBER,
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupNameDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      msrPartGroupSymbolKind::kPartGroupSymbolNone, // partGroupSymbolKind
      0,                                  // partGroupSymbolDefaultX,
      msrPartGroupImplicitKind::kPartGroupImplicitYes,
      partGroupBarLineKind,
      nullptr,                            // partGroupUpLinkToPartGroup,
                                          // will be set below
      partGroupUpLinkToScore);
  assert (o != nullptr);

  // the implicit part group it the top-most one:
  // set its group upLink to point to itself
  o->fPartGroupUpLinkToPartGroup = o;

  return o;
}

S_msrPartGroup msrPartGroup::create (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const std::string&       partGroupName,
  const S_msrPartGroup&    partGroupUpLinkToPartGroup,
  const S_msrScore&        partGroupUpLinkToScore)
{
  msrPartGroup* o =
    new msrPartGroup (
      inputLineNumber,
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupUpLinkToPartGroup,
      partGroupUpLinkToScore);
  assert (o != nullptr);
  return o;
}

msrPartGroup::msrPartGroup (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const std::string&       partGroupName,
  const std::string&       partGroupNameDisplayText,
  const std::string&       partGroupAccidentalText,
  const std::string&       partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupImplicitKind partGroupImplicitKind,
  msrPartGroupBarLineKind  partGroupBarLineKind,
  const S_msrPartGroup&    partGroupUpLinkToPartGroup,
  const S_msrScore&        partGroupUpLinkToScore)
    : msrPartGroupElement (inputLineNumber)
{
  // no sanity check on partGroupUpLinkToPartGroup here,
  // it will be set after all 'real' (i.e. not implicit)
  // part groups and part have been analyzed
  fPartGroupUpLinkToPartGroup = partGroupUpLinkToPartGroup;

/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fPartGroupUpLinkToScore != nullptr,
    "fPartGroupUpLinkToScore is null");
    */

  fPartGroupUpLinkToScore     = partGroupUpLinkToScore;

  // other fields
  fPartGroupNumber          = partGroupNumber;
  fPartGroupAbsoluteNumber  = partGroupAbsoluteNumber;

  fPartGroupName            = partGroupName;

  size_t partGroupNameLength =
    fPartGroupName.size ();

  if (
    partGroupNameLength
      >
    fPartGroupUpLinkToScore->getScorePartGroupNamesMaxLength ()
  ) {  // JMI sanity check ???
    fPartGroupUpLinkToScore->
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
      std::endl <<
      "Creating part group '" << fPartGroupNumber << "'" <<
      ", partGroupAbsoluteNumber: " << fPartGroupAbsoluteNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif
}

msrPartGroup::msrPartGroup (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  const std::string&       partGroupName,
  const S_msrPartGroup&    partGroupUpLinkToPartGroup,
  const S_msrScore&        partGroupUpLinkToScore)
    : msrPartGroupElement (inputLineNumber)
{
  // no sanity check on partGroupUpLinkToPartGroup here,
  // it will be set after all 'real' (i.e. not implicit)
  // part groups and part have been analyzed
  fPartGroupUpLinkToPartGroup = partGroupUpLinkToPartGroup;

/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fPartGroupUpLinkToScore != nullptr,
    "fPartGroupUpLinkToScore is null");
    */

  fPartGroupUpLinkToScore     = partGroupUpLinkToScore;

  // other fields
  fPartGroupNumber          = partGroupNumber;
  fPartGroupAbsoluteNumber  = partGroupAbsoluteNumber;

  fPartGroupName            = partGroupName;

  size_t partGroupNameLength =
    fPartGroupName.size ();

  if (
    partGroupNameLength
      >
    fPartGroupUpLinkToScore->getScorePartGroupNamesMaxLength ()
  ) {  // JMI sanity check ???
    fPartGroupUpLinkToScore->
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
      std::endl <<
      "Creating part group '" << fPartGroupNumber << "'" <<
      ", partGroupAbsoluteNumber: " << fPartGroupAbsoluteNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif
}

msrPartGroup::~msrPartGroup ()
{}

S_msrPartGroup msrPartGroup::createPartGroupNewbornClone (
  const S_msrPartGroup& partGroupClone,
  const S_msrScore&     scoreClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "--------------------------------------------" <<
      std::endl <<
      "Creating a newborn clone part group " <<
      getPartGroupCombinedName () <<
      std::endl;
  }
#endif

  // don't check partGroupClone against 0, since the partGroup std::stack JMI
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

std::string msrPartGroup::getPartGroupCombinedName () const
{
  std::stringstream s;

  s <<
    "PartGroup_" << fPartGroupAbsoluteNumber <<
    " ('" << fPartGroupNumber <<
    "', fPartGroupName \"" << fPartGroupName << "\")";

  return s.str ();
}

std::string msrPartGroup::getPartGroupCombinedNameWithoutEndOfLines () const
{
  std::list<std::string> chunksList;

  mfSplitRegularStringAtEndOfLines (
    fPartGroupName,
    chunksList);

  std::stringstream s;

  s <<
    "PartGroup_" << fPartGroupAbsoluteNumber <<
    " ('" << fPartGroupNumber <<
    "', partGroupName \"";

  if (chunksList.size ()) {
    // used the chunks separated by a space
    std::list<std::string>::const_iterator
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
    msrPartGroupImplicitKindAsString (
      fPartGroupImplicitKind);

  return s.str ();
}

void msrPartGroup::setPartGroupInstrumentName (
  const std::string& partGroupInstrumentName)
{
  fPartGroupInstrumentName = partGroupInstrumentName;

  S_msrScore
    score =
      fPartGroupUpLinkToScore;

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
  const S_msrPartGroupElement& partGroupElement) const // TEMP JMI v0.9.63
{
  // sanity check
  if (
    ((void*) partGroupElement) == (void*) 0x0000000000000001
      ||
    ((void*) partGroupElement) == (void*) 0x0000000000000009
      ||
    ((void*) partGroupElement) == (void*) 0x0000000000000011
  ) {
    std::stringstream s;

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
  int                inputLineNumber,
  const std::string& partID)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    partID.size () > 0,
    "partID is empty");

  // has this partID already been added to this part?
  if (fPartGroupPartsMap.count (partID)) {
    std::stringstream s;

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
      std::endl;

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
      std::endl <<
      "After appendPartToPartGroupByItsID, fPartGroupPartsMap contains:" <<
      std::endl;

    ++gIndenter;

    for (
        std::map<std::string, S_msrPart>::const_iterator i = fPartGroupPartsMap.begin ();
        i != fPartGroupPartsMap.end ();
        ++i
      ) {
      gLogStream <<
        "\"" << (*i).first << "\" --% --> " <<
        (*i).second->
          getPartCombinedName () <<
        std::endl;
    } // for

    --gIndenter;

    gLogStream <<
      "After appendPartToPartGroupByItsID, fPartGroupElementsList contains:" <<
      std::endl;

    ++gIndenter;

    if (fPartGroupElementsList.size ()) {
      std::list<S_msrPartGroupElement>::const_iterator
        iBegin = fPartGroupElementsList.begin (),
        iEnd   = fPartGroupElementsList.end (),
        i      = iBegin;

      for ( ; ; ) {
        gLogStream <<
          (*i);
        if (++i == iEnd) break;
        gLogStream << std::endl;
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
      std::endl;

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
  part->setPartUpLinkToPartGroup (this);
}

void msrPartGroup::removePartFromPartGroup (
  int              inputLineNumber,
  const S_msrPart& partToBeRemoved)
{
  // register part in this part group
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Removing part " <<
      partToBeRemoved->getPartCombinedName () <<
      " from part group " << asString () <<
      std::endl;

    this->printShort (gLogStream);
    partToBeRemoved->printShort (gLogStream);
  }
#endif

  ++gIndenter;

  for (
    std::list<S_msrPartGroupElement>::iterator i = fPartGroupElementsList.begin ();
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
      std::stringstream s;

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
  const S_msrPartGroup& partGroup)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Prepending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << std::endl;

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
  const S_msrPartGroup& partGroup)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Appending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << std::endl;

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
  std::ostream& os) const
{
  os <<
    "fPartGroupElementsList:";

  if (fPartGroupElementsList.size ()) {
    os << std::endl;

    std::list<S_msrPartGroupElement>::const_iterator
      iBegin = fPartGroupElementsList.begin (),
      iEnd   = fPartGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrElement
        element = (*i);

      if (
        S_msrPartGroup
          nestedPartGroup =
            dynamic_cast<msrPartGroup*>(&(*element))
        ) {
        // this is a part group
//         gLogStream << // KAKA
//           nestedPartGroup->
//             getPartGroupCombinedNameWithoutEndOfLines () <<
//           std::endl;

        ++gIndenter;

        gLogStream <<
          nestedPartGroup <<
          std::endl;

        --gIndenter;
      }

      else if (
        S_msrPart
          part =
            dynamic_cast<msrPart*>(&(*element))
        ) {
        // this is a part
        ++gIndenter;

        gLogStream <<
          part <<
          std::endl;

        --gIndenter;
      }

      else {
        std::stringstream s;

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

      if (++i == iEnd) break;
   // JMI   os << std::endl;
    } // for
  }

  else {
    os << ' ' << "[NONE]" << std::endl;
  }
}

void msrPartGroup::printPartGroupElementsListShort (
  int      inputLineNumber,
  std::ostream& os) const
{
  os <<
    "fPartGroupElementsList";

  if (fPartGroupElementsList.size ()) {
    os << std::endl;

    std::list<S_msrPartGroupElement>::const_iterator
      iBegin = fPartGroupElementsList.begin (),
      iEnd   = fPartGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrElement
        element = (*i);

      if (
        S_msrPartGroup
          nestedPartGroup =
            dynamic_cast<msrPartGroup*>(&(*element))
        ) {
        // this is a part group
//         gLogStream << // KAKA
//           nestedPartGroup->
//             getPartGroupCombinedNameWithoutEndOfLines () <<
//           std::endl;

        ++gIndenter;

        nestedPartGroup->
          printShort (gLogStream);

        --gIndenter;
      }

      else if (
        S_msrPart
          part =
            dynamic_cast<msrPart*>(&(*element))
        ) {
        // this is a part
        ++gIndenter;

        part->
          printShort (gLogStream);

        --gIndenter;
      }

      else {
        std::stringstream s;

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

      if (++i == iEnd) break;
   // JMI   os << std::endl;
    } // for
  }

  else {
    os << ' ' << "[NONE]" << std::endl;
  }
}

S_msrPart msrPartGroup::fetchPartFromPartGroupByItsPartID (
  int                inputLineNumber,
  const std::string& partID)
{
  S_msrPart result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    gLogStream <<
      "fetchPartFromPartGroupByItsPartID(" << partID <<
      "), fPartGroupElementsList contains:" <<
      std::endl;

    ++gIndenter;

//     printPartGroupElementsList ( JMI v0.9.66
    printPartGroupElementsListShort (
      inputLineNumber,
      gLogStream);

    --gIndenter;

    gLogStream <<
      "<=- fetchPartFromPartGroupByItsPartID(" << partID << ")" <<
      std::endl << std::endl;
  }
#endif

  for (
    std::list<S_msrPartGroupElement>::const_iterator i =
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
      std::stringstream s;

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
  int                   inputLineNumber,
  std::list<S_msrPart>& partsList)
{
  for (
    std::list<S_msrPartGroupElement>::const_iterator i =
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
      std::stringstream s;

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
  const S_msrVoice& voice)
{
  // register voice in this staff
  fPartGroupAllVoicesList.push_back (voice);

  // register it in the partgroup uplink
  fPartGroupUpLinkToScore->
    registerVoiceInScoreAllVoicesList (voice);
}

void msrPartGroup::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPartGroup::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPartGroup::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrPartGroup::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPartGroup::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPartGroup::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrPartGroup::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPartGroup::browseData ()" <<
      std::endl;
  }

  for (S_msrPartGroupElement partGroupElement : fPartGroupElementsList) {
    // browse the part group element
    msrBrowser<msrElement> browser (v);
    browser.browse (*partGroupElement);
  } // for

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrPartGroup::browseData ()" <<
      std::endl;
  }
}

std::string msrPartGroupImplicitKindAsString (
  msrPartGroupImplicitKind partGroupImplicitKind)
{
  std::string result;

  switch (partGroupImplicitKind) {
    case msrPartGroupImplicitKind::kPartGroupImplicitYes:
      result = "kPartGroupImplicitYes";
      break;
    case msrPartGroupImplicitKind::kPartGroupImplicitNo:
      result = "kPartGroupImplicitNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPartGroupImplicitKind& elt)
{
  os << msrPartGroupImplicitKindAsString (elt);
  return os;
}

std::string msrPartGroupTypeKindAsString (
  msrPartGroupTypeKind partGroupTypeKind)
{
  std::string result;

  switch (partGroupTypeKind) {
    case msrPartGroupTypeKind::kPartGroupTypeNone:
      result = "kPartGroupTypeNone";
      break;
    case msrPartGroupTypeKind::kPartGroupTypeStart:
      result = "kPartGroupTypeStart";
      break;
    case msrPartGroupTypeKind::kPartGroupTypeStop:
      result = "kPartGroupTypeStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPartGroupTypeKind& elt)
{
  os << msrPartGroupTypeKindAsString (elt);
  return os;
}

std::string msrPartGroupSymbolKindAsString (
  msrPartGroupSymbolKind partGroupSymbolKind)
{
  std::string result;

  switch (partGroupSymbolKind) {
    case msrPartGroupSymbolKind::kPartGroupSymbolNone:
      result = "kPartGroupSymbolNone";
      break;
    case msrPartGroupSymbolKind::kPartGroupSymbolBrace:
      result = "kPartGroupSymbolBrace";
      break;
    case msrPartGroupSymbolKind::kPartGroupSymbolBracket:
      result = "kPartGroupSymbolBracket";
      break;
    case msrPartGroupSymbolKind::kPartGroupSymbolLine:
      result = "kPartGroupSymbolLine";
      break;
    case msrPartGroupSymbolKind::kPartGroupSymbolSquare:
      result = "kPartGroupSymbolSquare";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPartGroupSymbolKind& elt)
{
  os << msrPartGroupSymbolKindAsString (elt);
  return os;
}

std::string msrPartGroupBarLineKindAsString (
  msrPartGroupBarLineKind partGroupBarLineKind)
{
  std::string result;

  switch (partGroupBarLineKind) {
    case msrPartGroupBarLineKind::kPartGroupBarLineYes:
      result = "kPartGroupBarLineYes";
      break;
    case msrPartGroupBarLineKind::kPartGroupBarLineNo:
      result = "kPartGroupBarLineNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPartGroupBarLineKind& elt)
{
  os << msrPartGroupBarLineKindAsString (elt);
  return os;
}

std::string msrPartGroup::asString () const
{
  std::stringstream s;

  s <<
    "PartGroup \"" <<
    getPartGroupCombinedName () <<
    "\", line " << fInputLineNumber;

  return s.str ();
}

void msrPartGroup::print (std::ostream& os) const
{
  os <<
    "[PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    mfSingularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 25;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupName" << ": \"" <<
    fPartGroupName <<
    "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupUpLinkToPartGroup" << ": ";
  if (fPartGroupUpLinkToPartGroup) {
    os <<
      "\"" <<
      fPartGroupUpLinkToPartGroup->
        getPartGroupCombinedName () <<
      "\"";
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
   std::setw (fieldWidth) <<
    "fPartGroupNameDisplayText" << ": \"" <<
    fPartGroupNameDisplayText <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartGroupAccidentalText" << ": \"" <<
    fPartGroupAccidentalText <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartGroupAbbrevation" << ": \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartGroupSymbolDefaultX" << ": " <<
    fPartGroupSymbolDefaultX <<
      std::endl <<
    std::setw (fieldWidth) <<
    "fPartGroupSymbolKind" << ": " <<
    msrPartGroupSymbolKindAsString (
      fPartGroupSymbolKind) <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupImplicitKind" << ": " <<
    msrPartGroupImplicitKindAsString (
      fPartGroupImplicitKind) <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupBarLineKind" << ": " <<
    msrPartGroupBarLineKindAsString (
      fPartGroupBarLineKind) <<
    std::endl;

  os << std::endl;

  // print all the voices if any
  size_t partGroupAllVoicesListSize =
    fPartGroupAllVoicesList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupAllVoicesList" << ": ";
  if (partGroupAllVoicesListSize) {
    os << std::endl;
    ++gIndenter;

    std::list<S_msrVoice>::const_iterator
      iBegin = fPartGroupAllVoicesList.begin (),
      iEnd   = fPartGroupAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      const S_msrVoice& voice = (*i);

      os << "\"" << voice->getVoiceName () << "\"" << std::endl;
      if (++i == iEnd) break;
      // os << std::endl;
    } // for
//    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
  }

  os << std::endl << std::endl;

  // print the part group elements if any

  printPartGroupElementsList (
    fInputLineNumber,
    gLogStream);

  --gIndenter;

  os << ']' << std::endl;
}

void msrPartGroup::printShort (std::ostream& os) const
{
  os <<
    "[PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    mfSingularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 25;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupName" << ": \"" <<
    fPartGroupName <<
    "\"" <<
    std::endl << std::endl;

  // print the part group elements if any
  printPartGroupElementsListShort (
    fInputLineNumber,
    gLogStream);

  --gIndenter;

  os << ']' << std::endl;
}

void msrPartGroup::printSummary (std::ostream& os) const
{
  os <<
    "[PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    mfSingularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 24;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupName" << ": \"" <<
    fPartGroupName <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartGroupAbbrevation" << ": \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartGroupSymbolDefaultX" << ": " <<
    fPartGroupSymbolDefaultX <<
      std::endl <<
    std::setw (fieldWidth) <<
    "fPartGroupSymbolKind" << ": \"" <<
    msrPartGroupSymbolKindAsString (
      fPartGroupSymbolKind) <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartGroupImplicit" << ": " <<
    msrPartGroupImplicitKindAsString (
      fPartGroupImplicitKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartGroupBarLine" << ": " <<
    msrPartGroupBarLineKindAsString (
      fPartGroupBarLineKind) <<
    std::endl;

  if (fPartGroupElementsList.size ()) {
    os << std::endl;
    std::list<S_msrPartGroupElement>::const_iterator
      iBegin = fPartGroupElementsList.begin (),
      iEnd   = fPartGroupElementsList.end (),
      i      = iBegin;

    ++gIndenter;
    for ( ; ; ) {
      (*i)->printSummary (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrPartGroup::printSlices (std::ostream& os) const
{
  os << "MSR part group slices";

  os << std::endl << std::endl;

  ++gIndenter;

//   if (fPartGroupElementsList.size ()) {
//     std::list<S_msrPartGroupElement>::const_iterator
//       iBegin = fPartGroupElementsList.begin (),
//       iEnd   = fPartGroupElementsList.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       (*i)->
//         printSlices (os);
//       if (++i == iEnd) break;
//       os << std::endl;
//     } // for
//   }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrPartGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
