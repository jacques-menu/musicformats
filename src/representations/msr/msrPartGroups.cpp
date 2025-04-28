/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrPartGroups.h"

#include "msrParts.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrPartGroup msrPartGroup::create (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupSequentialNumber,
  const std::string&       partGroupName,
  const std::string&       partGroupNameDisplayText,
  const std::string&       partGroupAccidentalText,
  const std::string&       partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupImplicitKind partGroupImplicitKind,
  msrPartGroupBarLineKind  partGroupBarLineKind,
  const S_msrPartGroup&    partGroupUpLinkToContainingPartGroup,
  const S_msrScore&        partGroupUpLinkToScore)
{
  msrPartGroup* obj =
    new msrPartGroup (
      inputLineNumber,
      partGroupNumber,
      partGroupSequentialNumber,
      partGroupName,
      partGroupNameDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      partGroupSymbolKind,
      partGroupSymbolDefaultX,
      partGroupImplicitKind,
      partGroupBarLineKind,
      partGroupUpLinkToContainingPartGroup,
      partGroupUpLinkToScore);
  assert (obj != nullptr);
  return obj;
}

S_msrPartGroup msrPartGroup::create (
  int                   inputLineNumber,
  int                   partGroupNumber,
  int                   partGroupSequentialNumber,
  const std::string&    partGroupName,
  const S_msrPartGroup& partGroupUpLinkToContainingPartGroup,
  const S_msrScore&     partGroupUpLinkToScore)
{
  msrPartGroup* obj =
    new msrPartGroup (
      inputLineNumber,
      partGroupNumber,
      partGroupSequentialNumber,
      partGroupName,
      partGroupUpLinkToContainingPartGroup,
      partGroupUpLinkToScore);
  assert (obj != nullptr);
  return obj;
}

msrPartGroup::msrPartGroup (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupSequentialNumber,
  const std::string&       partGroupName,
  const std::string&       partGroupNameDisplayText,
  const std::string&       partGroupAccidentalText,
  const std::string&       partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupImplicitKind partGroupImplicitKind,
  msrPartGroupBarLineKind  partGroupBarLineKind,
  const S_msrPartGroup&    partGroupUpLinkToContainingPartGroup,
  const S_msrScore&        partGroupUpLinkToScore)
    : msrPartGroupElement (inputLineNumber)
{
  // no sanity check on partGroupUpLinkToContainingPartGroup here,
  // it will be set after all 'real' (i.e. not implicit)
  // part groups and part have been analyzed JMI 0.9.69
  fPartGroupUpLinkToContainingPartGroup = partGroupUpLinkToContainingPartGroup;

/* JMI 0.9.69
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fPartGroupUpLinkToScore != nullptr,
    "fPartGroupUpLinkToScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  fPartGroupUpLinkToScore = partGroupUpLinkToScore;

  // other fields
  fPartGroupNumber = partGroupNumber;
  fPartGroupSequentialNumber = partGroupSequentialNumber;

  fPartGroupName = partGroupName;

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

  fPartGroupAccidentalText = partGroupAccidentalText;

  fPartGroupAbbreviation = partGroupAbbreviation;

  fPartGroupSymbolKind = partGroupSymbolKind;
  fPartGroupSymbolDefaultX = partGroupSymbolDefaultX;

  fPartGroupImplicitKind = partGroupImplicitKind;

  fPartGroupBarLineKind = partGroupBarLineKind;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Creating part group '" << fPartGroupNumber << '\'' <<
      ", partGroupSequentialNumber: " << fPartGroupSequentialNumber <<
      ", nested in part group '" << fPartGroupUpLinkToContainingPartGroup << '\'' <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrPartGroup::msrPartGroup (
  int                   inputLineNumber,
  int                   partGroupNumber,
  int                   partGroupSequentialNumber,
  const std::string&    partGroupName,
  const S_msrPartGroup& partGroupUpLinkToContainingPartGroup,
  const S_msrScore&     partGroupUpLinkToScore)
    : msrPartGroupElement (inputLineNumber)
{
  // no sanity check on partGroupUpLinkToContainingPartGroup here,
  // it will be set after all 'real' (i.e. not the implicit outer-most)
  // part groups and parts have been analyzed JMI 0.9.69
  fPartGroupUpLinkToContainingPartGroup = partGroupUpLinkToContainingPartGroup;

/* JMI
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fPartGroupUpLinkToScore != nullptr,
    "fPartGroupUpLinkToScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  fPartGroupUpLinkToScore = partGroupUpLinkToScore;

  // other fields
  fPartGroupNumber = partGroupNumber;
  fPartGroupSequentialNumber = partGroupSequentialNumber;

  fPartGroupName = partGroupName;

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Creating part group '" << fPartGroupNumber << '\'' <<
      ", partGroupSequentialNumber: " << fPartGroupSequentialNumber <<
      ", nested in part group '" << fPartGroupUpLinkToContainingPartGroup << '\'' <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrPartGroup::~msrPartGroup ()
{}

S_msrPartGroup msrPartGroup::createPartGroupNewbornClone (
  const S_msrPartGroup& containingartGroupClone,
  const S_msrScore&     scoreClone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Creating a part group newborn clone of " <<
      fPartGroupName <<
      ", " <<
      fetchPartGroupInformationForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // don't check containingartGroupClone against 0, since the partGroup stack JMI
  // that it comes from may be empty
/* JMI
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingartGroupClone != nullptr,
    "containingartGroupClone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    scoreClone != nullptr,
    "scoreClone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrPartGroup
    newbornClone =
      msrPartGroup::create (
        fInputLineNumber,
        fPartGroupNumber,
        fPartGroupSequentialNumber,
        fPartGroupName,
        fPartGroupNameDisplayText,
        fPartGroupAccidentalText,
        fPartGroupAbbreviation,
        fPartGroupSymbolKind,
        fPartGroupSymbolDefaultX,
        fPartGroupImplicitKind,
        fPartGroupBarLineKind,
        containingartGroupClone,
        scoreClone);

  newbornClone->fPartGroupInstrumentName =
    fPartGroupInstrumentName;

  return newbornClone;
}

void msrPartGroup::setPartGroupUpLinkToContainingPartGroup (
  int                   inputLineNumber,
  const S_msrPartGroup& containingPartGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Setting part group " <<
      asString () <<
      " part group uplink to " <<
      containingPartGroup->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  std::stringstream ss;

  ss <<
    "Setting the uplink of part group " <<
    asString () <<
    " to itself";

  mfAssert (
    __FILE__, __LINE__,
    this != containingPartGroup,
    ss.str ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//     switch (fPartGroupImplicitKind) { JMI 0.9.69
//       case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
//         result = "kPartGroupImplicitOuterMostYes";
//         break;
//       case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
//         result = "kPartGroupImplicitOuterMostNo";
//         break;
//     } // switch

  fPartGroupUpLinkToContainingPartGroup =
    containingPartGroup;
}

std::string msrPartGroup::fetchPartGroupInformationForTrace () const
{
  std::stringstream ss;

  ss <<
    "[fPartGroupSequentialNumber: " << fPartGroupSequentialNumber <<
    "\", fPartGroupNumber: " << fPartGroupNumber <<
    ']';

  return ss.str ();
}

std::string msrPartGroup::fetchPartGroupInformationForTraceWithoutEndOfLines () const
{
  std::list <std::string> chunksList;

  mfSplitRegularStringAtEndOfLines (
    fPartGroupName,
    chunksList);

  std::stringstream ss;

  ss <<
    "PartGroup_" << fPartGroupSequentialNumber <<
    " ('" << fPartGroupNumber <<
    "', partGroupName \"";

  if (chunksList.size ()) {
    // used the chunks separated by a space
    std::list <std::string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss <<(*i);
      if (++i == iEnd) break;
      ss << ' ';
    } // for
  }

  ss <<
    "\"" <<
    ", fPartGroupImplicitKind: " <<
    fPartGroupImplicitKind;

  return ss.str ();
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

// void msrPartGroup::checkPartGroupElement (
//   const S_msrPartGroupElement& partGroupElement) const // TEMP JMI 0.9.66
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check // TEMP JMI 0.9.69
//   if (
//     ((void*) partGroupElement) == (void*) 0x0000000000000001
//       ||
//     ((void*) partGroupElement) == (void*) 0x0000000000000009
//       ||
//     ((void*) partGroupElement) == (void*) 0x0000000000000011
//   ) {
//     std::stringstream ss;
//
//     ss <<
//       "###### partGroupElement is " <<
//       (void*) partGroupElement <<
//       " ######"; // JMI 0.9.63
//
//     msrInternalError(
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber, // inputLineNumber // TEMP JMI 0.9.63
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
// }

// S_msrPart msrPartGroup::appendPartToPartGroupByItsPartID (
//   int                inputLineNumber,
//   const std::string& partMusicXMLID)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     partMusicXMLID.size () > 0,
//     "partMusicXMLID is empty");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // has this partMusicXMLID already been added to this part?
//   if (fPartGroupPartsMap.count (partMusicXMLID)) {
//     std::stringstream ss;
//
//     ss <<
//       "partMusicXMLID \"" << partMusicXMLID <<
//       "\" already exists in part group " <<
//       fetchPartGroupInformationForTrace ();
//
//     msrWarning ( // JMI
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       ss.str ());
//
//     return fPartGroupPartsMap [partMusicXMLID];
//   }
//
//   // create the part
//   S_msrPart
//     part =
//       msrPart::create (
//         inputLineNumber,
//         partMusicXMLID,
//         this);
//
//   // register it in this part group
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePartGroups ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending part " <<
//       part->fetchPartNameForTrace () <<
//       " to part group " <<
//       asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // register part in part group
//   fPartGroupPartsMap [partMusicXMLID] = part;
//
//   fPartGroupElementsList.push_back (part);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePartGroupsDetails ()) {
//     std::stringstream ss;
//
//     ss <<
//       std::endl <<
//       "After appendPartToPartGroupByItsID, fPartGroupPartsMap contains:" <<
//       std::endl;
//
//     ++gIndenter;
//
//     for (
//       std::map <std::string, S_msrPart>::const_iterator i = fPartGroupPartsMap.begin ();
//       i != fPartGroupPartsMap.end ();
//       ++i
//     ) {
//       std::stringstream ss;
//
//       ss <<
//         "\"" << (*i).first << "\" --% --> " <<
//         (*i).second->
//           fetchPartNameForTrace () <<
//         std::endl;
//     } // for
//
//     --gIndenter;
//
//     ss <<
//       "After appendPartToPartGroupByItsID, fPartGroupElementsList contains:" <<
//       std::endl;
//
//     ++gIndenter;
//
//     if (fPartGroupElementsList.size ()) {
//       std::list <S_msrPartGroupElement>::const_iterator
//         iBegin = fPartGroupElementsList.begin (),
//         iEnd   = fPartGroupElementsList.end (),
//         i      = iBegin;
//
//       for ( ; ; ) {
//         ss << (*i);
//         if (++i == iEnd) break;
// //         gLog << std::endl; // JMI 0.9.69
//       } // for
//     }
//
//     --gIndenter;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // return the part
//   return part;
// }

void msrPartGroup::appendPartToPartGroup (S_msrPart part)
{
  // register part in this part group
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Adding part " <<
      part->fetchPartNameForTrace () <<
      " to part group " << asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register part into this part group's data
  fPartGroupPartsMap [part->getPartMusicXMLID ()] = part;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayPartGroupPartsMap (
      part->getInputLineNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartGroupElementsList.push_back (part);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayPartGroupElementsList (
      part->getInputLineNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set part's partgroup upLink
  part->setPartUpLinkToPartGroup (this);
}

void msrPartGroup::removePartFromPartGroup (
  int              inputLineNumber,
  const S_msrPart& partToBeRemoved)
{
  // register part in this part group
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Removing part " <<
      partToBeRemoved->fetchPartNameForTrace () <<
      " from part group " << asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  for (
    std::list <S_msrPartGroupElement>::iterator i = fPartGroupElementsList.begin ();
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
      std::stringstream ss;

      ss <<
        "an element of partgroup " <<
        fPartGroupName <<
        ", " <<
        fetchPartGroupInformationForTrace () <<
        " is not a part group nor a part";

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  } // for

  --gIndenter;
}

void msrPartGroup::prependNestedGroupToPartGroup (
  const S_msrPartGroup& nestedPartGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Prepending nested part group " <<
      nestedPartGroup->asString () <<
      " to part group " << asString ()  << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register it in this part group

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  std::stringstream ss;

  ss <<
    "Prepending part group " <<
    asString () <<
    " to itself";

  mfAssert (
    __FILE__, __LINE__,
    this != nestedPartGroup,
    ss.str ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fPartGroupElementsList.push_front (nestedPartGroup);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayPartGroupElementsList (
      nestedPartGroup->getInputLineNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fPartGroupImplicitKind) {
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
      switch (nestedPartGroup->getPartGroupImplicitKind ()) {
        case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
          {
            std::stringstream ss;

            ss <<
              "Prepending implicit outer-most part group " <<
              nestedPartGroup->asString () <<
              " to outer-most part group " <<
              asString () <<
              ", line " << nestedPartGroup->getInputLineNumber ();

              mfAssert (
                __FILE__, __LINE__,
                false,
                ss.str ());
            }
          break;
        case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
          break;
      } // switch
      break;
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
      break;
  } // switch
}

void msrPartGroup::appendNestedPartGroupToPartGroup (
  const S_msrPartGroup& nestedPartGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending nested part group " <<
      nestedPartGroup->asString () <<
      " to part group " << asString ()  << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register it in this part group

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  std::stringstream ss;

  ss <<
    "Appending part group " <<
    asString () <<
    " to itself";

  mfAssert (
    __FILE__, __LINE__,
    this != nestedPartGroup,
    ss.str ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fPartGroupElementsList.push_back (nestedPartGroup);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayPartGroupElementsList (
      nestedPartGroup->getInputLineNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fPartGroupImplicitKind) {
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
      switch (nestedPartGroup->getPartGroupImplicitKind ()) {
        case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
          {
            std::stringstream ss;

            ss <<
              "Appending implicit outer-most part group " <<
              nestedPartGroup->asString () <<
              " to outer-most part group " <<
              asString () <<
              ", line " << nestedPartGroup->getInputLineNumber ();

              mfAssert (
                __FILE__, __LINE__,
                false,
                ss.str ());
            }
          break;
        case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
          break;
      } // switch
      break;
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
      break;
  } // switch
}

void msrPartGroup::displayPartGroupPartsMap (
  int inputLineNumber)
{
  gLog <<
    std::endl <<
    "fPartGroupPartsMap contains" <<
    ", line : " << inputLineNumber <<
    std::endl;

  ++gIndenter;

  for (std::pair <std::string, S_msrPart> thePair : fPartGroupPartsMap) {
    gLog <<
      "\"" << thePair.first << "\" --% --> " <<
      thePair.second->fetchPartNameForTrace () <<
      std::endl;
  } // for

  --gIndenter;
}

void msrPartGroup::displayPartGroupElementsListFull (
  int inputLineNumber) const
{
  size_t
    partGroupElementsListSize =
      fPartGroupElementsList.size ();

  gLog <<
    "fPartGroupElementsList contains " <<
    mfSingularOrPlural (
      partGroupElementsListSize, "element", "elements") <<
    ", line : " << inputLineNumber;

  if (partGroupElementsListSize) {
    gLog << std::endl;

    std::list <S_msrPartGroupElement>::const_iterator
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
//         gLog << // JMI 0.9.67
//           nestedPartGroup->
//             fetchPartGroupInformationForTraceWithoutEndOfLines () <<
//           std::endl;

        ++gIndenter;

        nestedPartGroup->
          printFull (gLog);
        gLog <<
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

        part->
          printFull (gLog);
        gLog <<
          std::endl;

        --gIndenter;
      }

      else {
        std::stringstream ss;

        ss <<
          "an element of partgroup " <<
          fPartGroupName <<
          ", " <<
          fetchPartGroupInformationForTrace () <<
          " is not a part group nor a part";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }

      if (++i == iEnd) break;
   // JMI   gLog << std::endl;
    } // for
  }

  else {
    gLog << ": [EMPTY]" << std::endl;
  }
}

void msrPartGroup::displayPartGroupElementsList (
  int inputLineNumber) const
{
  size_t
    partGroupElementsListSize =
     fPartGroupElementsList.size ();

  gLog <<
    "fPartGroupElementsList contains " <<
    mfSingularOrPlural (
      partGroupElementsListSize, "element", "elements") <<
    ", line : " << inputLineNumber;

  if (partGroupElementsListSize) {
    gLog << std::endl;

    std::list <S_msrPartGroupElement>::const_iterator
      iBegin = fPartGroupElementsList.begin (),
      iEnd   = fPartGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrElement
        element = (*i);

//       gLog << std::endl << "FII before partGroup element" << std::endl;

      if (
        S_msrPartGroup
          nestedPartGroup =
            dynamic_cast<msrPartGroup*>(&(*element))
      ) {
        // this is a part group
//         gLog << // JMI 0.9.67
//           nestedPartGroup->
//             fetchPartGroupInformationForTraceWithoutEndOfLines () <<
//           std::endl;

        ++gIndenter;

        gLog << nestedPartGroup;

        --gIndenter;
      }

      else if (
        S_msrPart
          part =
            dynamic_cast<msrPart*>(&(*element))
      ) {
        // this is a part
        ++gIndenter;

        gLog << part;

        --gIndenter;
      }

      else {
        std::stringstream ss;

        ss <<
          "an element of partgroup " <<
          fPartGroupName <<
          ", " <<
          fetchPartGroupInformationForTrace () <<
          " is not a part group nor a part";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }

//       gLog << std::endl << "FUU after partGroup element" << std::endl;

      if (++i == iEnd) break;

      gLog << std::endl; // JMI 0.9.69
    } // for
  }

  else {
    gLog << ": [EMPTY]" << std::endl;
  }
}

S_msrPart msrPartGroup::fetchPartFromPartGroupByItsPartID (
  int                inputLineNumber,
  const std::string& partMusicXMLID)
{
  S_msrPart result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroupsDetails ()) {
    std::stringstream ss;

    ss <<
      "fetchPartFromPartGroupByItsPartID(" << partMusicXMLID <<
      "), fPartGroupElementsList contains:" <<
      std::endl;

    ++gIndenter;

    displayPartGroupElementsList (
      inputLineNumber);

    --gIndenter;

    ss <<
      "<=- fetchPartFromPartGroupByItsPartID(" << partMusicXMLID << ")";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list <S_msrPartGroupElement>::const_iterator i =
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
              partMusicXMLID);

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
      if (part->getPartMusicXMLID () == partMusicXMLID) {
        result = part;
        break;
      }
    }

    else {
      std::stringstream ss;

      ss <<
        "an element of partgroup " <<
        fPartGroupName <<
        ", " <<
        fetchPartGroupInformationForTrace () <<
        " is not a part group nor a part";

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  } // for

  return result;
}

void msrPartGroup::collectPartGroupPartsList (
  int                   inputLineNumber,
  std::list <S_msrPart>& partsList)
{
  for (S_msrPartGroupElement partGroupElement : fPartGroupElementsList) {
    if (
      S_msrPartGroup
        partGroup =
          dynamic_cast<msrPartGroup*>(&(*partGroupElement))
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
          dynamic_cast<msrPart*>(&(*partGroupElement))
    ) {
      // this is a part

      partsList.push_back (part);
    }

    else {
      std::stringstream ss;

      ss <<
        "partGroupElement " <<
        partGroupElement <<
        "of part group " <<
        fPartGroupName <<
        ", " << // JMI 0.9.72
        fetchPartGroupInformationForTrace () <<
        " is not a part group nor a part";

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  } // for
}

void msrPartGroup::collectPartGroupPartsMap (
  int                                inputLineNumber,
  std::map <std::string, S_msrPart>& partsMap)
{
  for (S_msrPartGroupElement partGroupElement : fPartGroupElementsList) {
    if (
      S_msrPartGroup
        partGroup =
          dynamic_cast<msrPartGroup*>(&(*partGroupElement))
      ) {
      // this is a part group
      partGroup->
        collectPartGroupPartsMap (
          inputLineNumber,
          partsMap);
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*partGroupElement))
    ) {
      // this is a part
      partsMap [part->getPartMusicXMLID ()] = part;
    }

    else {
      std::stringstream ss;

      ss <<
        "partGroupElement " <<
        partGroupElement <<
        "of part group " <<
        fPartGroupName <<
        ", " << // JMI 0.9.72
        fetchPartGroupInformationForTrace () <<
        " is not a part group nor a part";

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPartGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPartGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrPartGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPartGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPartGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrPartGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPartGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrPartGroupElement partGroupElement : fPartGroupElementsList) {
    // browse the part group element
    msrBrowser<msrElement> browser (v);
    browser.browse (*partGroupElement);
  } // for

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrPartGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrPartGroupImplicitKindAsString (
  msrPartGroupImplicitKind partGroupImplicitKind)
{
  std::string result;

  switch (partGroupImplicitKind) {
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
      result = "kPartGroupImplicitOuterMostYes";
      break;
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
      result = "kPartGroupImplicitOuterMostNo";
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
  std::stringstream ss;

  ss <<
    "[PartGroup " <<
    fPartGroupName <<
    ", " <<
    fetchPartGroupInformationForTrace () <<
    ", line " << fInputLineNumber << // JMI non significant
    ']';

  return ss.str ();
}

void msrPartGroup::printFull (std::ostream& os) const
{
  os <<
    "[PartGroup " <<
    fPartGroupName <<
    ", " <<
    fetchPartGroupInformationForTrace () <<
//     ", fPartGroupPartsMap.size(): " << fPartGroupPartsMap.size () <<
    ", line " << fInputLineNumber << // JMI non significant
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 26;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupName" << ": \"" <<
    fPartGroupName <<
    "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupUpLinkToContainingPartGroup" << ": ";
  if (fPartGroupUpLinkToContainingPartGroup) {
    os <<
      fPartGroupUpLinkToContainingPartGroup->
        fetchPartGroupInformationForTrace ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupPartsMap.size():" << ": " <<
    fPartGroupPartsMap.size () <<
    std::endl <<

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
    fPartGroupImplicitKind <<
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
    "fPartGroupAllVoicesList";
  if (partGroupAllVoicesListSize) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrVoice>::const_iterator
      iBegin = fPartGroupAllVoicesList.begin (),
      iEnd   = fPartGroupAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << "\"" << voice->getVoiceName () << "\"" << std::endl;
      if (++i == iEnd) break;
      // os << std::endl;
    } // for
//    os << std::endl;

    --gIndenter;
  }
  else {
    os << ": [EMPTY]" << std::endl;
  }

  os << std::endl << std::endl;

  // print the part group elements if any

  displayPartGroupElementsListFull (
    fInputLineNumber);

  --gIndenter;

  os << ']' << std::endl;
}

void msrPartGroup::print (std::ostream& os) const
{
//   static int simultaneousCalls = 0; // JMI 0.9.69
//
//   ++simultaneousCalls;

//   if (simultaneousCalls == 100) abort (); // JMI 0.9.69 for the implicit outer-most part group...

  os <<
    "[PartGroup " <<
    fPartGroupName <<
    ", " <<
    fetchPartGroupInformationForTrace () <<
    ", line " << fInputLineNumber << // JMI non significant
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartGroupName" << ": \"" <<
    fPartGroupName <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartGroupImplicitKind" << ": " <<
    fPartGroupImplicitKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartGroupPartsMap.size():" << ": " <<
    fPartGroupPartsMap.size () <<
    std::endl;

  // print the part group elements if any
  displayPartGroupElementsList (
    fInputLineNumber);

  --gIndenter;

  os << ']' << std::endl;

//   --simultaneousCalls;
}

void msrPartGroup::printSummary (std::ostream& os) const
{
  os <<
    "[PartGroup " <<
    fPartGroupName <<
    ", " <<
    fetchPartGroupInformationForTrace () <<
    ", fPartGroupPartsMap.size():: " << fPartGroupPartsMap.size () <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 26;

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
    fPartGroupImplicitKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartGroupBarLine" << ": " <<
    msrPartGroupBarLineKindAsString (
      fPartGroupBarLineKind) <<
    std::endl;

  if (fPartGroupElementsList.size ()) {
    os << std::endl;
    std::list <S_msrPartGroupElement>::const_iterator
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
//     std::list <S_msrPartGroupElement>::const_iterator
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
