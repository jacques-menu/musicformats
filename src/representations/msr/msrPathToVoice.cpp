/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "msrPathToVoice.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrPathToVoice msrPathToVoice::createFromVoice (const S_msrVoice& voice)
{
  msrPathToVoice* obj =
    new msrPathToVoice ();
  assert (obj != nullptr);

  obj->initializeFromVoice (voice);

  return obj;
}

S_msrPathToVoice msrPathToVoice::createFromStaff (const S_msrStaff& staff)
{
  msrPathToVoice* obj =
    new msrPathToVoice ();
  assert (obj != nullptr);

  obj->initializeFromStaff (staff);

  return obj;
}

msrPathToVoice::msrPathToVoice ()
{}

void msrPathToVoice::initializeFromVoice (const S_msrVoice& voice)
{
  S_msrStaff
    upLinkToStaff =
      voice->getVoiceUpLinkToStaff ();

  initializeFromStaff (upLinkToStaff);

  fVoice = voice; // only now
}

void msrPathToVoice::initializeFromStaff (const S_msrStaff& staff)
{
  // leave fVoice as nullptr to indicate all the voices in the staff

  fStaff = staff;

  fPart =
    fStaff->getStaffUpLinkToPart ();

  S_msrPartGroup
    partGroup =
      fPart->getPartUpLinkToPartGroup ();

  fPartGroupsList.push_front (partGroup);

  // part groups can be nested
  S_msrPartGroup
    partGroupUpLinkToPartGroup =
      partGroup->
        getPartGroupUpLinkToContainingPartGroup ();

  // loop to the top-most part group
// while (partGroupUpLinkToPartGroup) { // JMI 0.9.69
    fPartGroupsList.push_front (partGroup);

    partGroupUpLinkToPartGroup =
      partGroup->
        getPartGroupUpLinkToContainingPartGroup ();
//  } // while

  fScore =
    partGroupUpLinkToPartGroup->
      getPartGroupUpLinkToScore ();

  fBook =
    fScore->
      getScoreUpLinkToBook ();
}

msrPathToVoice::~msrPathToVoice ()
{}

Bool msrPathToVoice::pathContainsPartGroup (const S_msrPartGroup& partGroup)
{
  Bool result;

  for (S_msrPartGroup currentPartGroup : fPartGroupsList) {
    if (partGroup == currentPartGroup) {
      result = true;
      break;
    }
  } // for

  return result;
}

Bool msrPathToVoice::pathContainsStaff (const S_msrStaff& staff)
{
  Bool result;

  if (fStaff) {
    // the path contains explicit staff fStaff
    if (staff == fStaff) {
      result = true;
    }
    else {
      /* JMI what else??? */
    }
  }

  else {
    // the path contains all the staves in fPart
    result =
      staff->getStaffUpLinkToPart () == fPart;
  }

//   for (S_msrPartGroup currentPartGroup : fPartGroupsList) {
//     if (partGroup == currentPartGroup) {
//       result = true;
//       break;
//     }
//   } // for

  return result;
}

Bool msrPathToVoice::pathContainsVoice (const S_msrVoice& voice)
{
  Bool result;

  if (fVoice) {
    // the path contains explicit voice fVoice
    if (voice == fVoice) {
      result = true;
    }
    else {
      /* JMI see harmonies and figured bass voices */
    }
  }

  else {
    // the path contains all the voices in fStaff
    result =
      voice->getVoiceUpLinkToStaff () == fStaff;
  }

  return result;
}

// void msrPathToVoice::acceptIn (basevisitor* v)
// {
//   if (gMsrOahGroup->getTraceMsrVisitors ()) {
//     gLog <<
//       "% ==> msrPathToVoice::acceptIn ()" <<
//       std::endl;
//   }
//
//   if (visitor<S_msrPathToVoice>*
//     p =
//       dynamic_cast<visitor<S_msrPathToVoice>*> (v)) {
//         S_msrPathToVoice elem = this;
//
//         if (gMsrOahGroup->getTraceMsrVisitors ()) {
//           gLog <<
//             "% ==> Launching msrPathToVoice::visitStart ()" <<
//             std::endl;
//         }
//         p->visitStart (elem);
//   }
// }
//
// void msrPathToVoice::acceptOut (basevisitor* v)
// {
//   if (gMsrOahGroup->getTraceMsrVisitors ()) {
//     gLog <<
//       "% ==> msrPathToVoice::acceptOut ()" <<
//       std::endl;
//   }
//
//   if (visitor<S_msrPathToVoice>*
//     p =
//       dynamic_cast<visitor<S_msrPathToVoice>*> (v)) {
//         S_msrPathToVoice elem = this;
//
//         if (gMsrOahGroup->getTraceMsrVisitors ()) {
//           gLog <<
//             "% ==> Launching msrPathToVoice::visitEnd ()" <<
//             std::endl;
//         }
//         p->visitEnd (elem);
//   }
// }
//
// void msrPathToVoice::browseData (basevisitor* v)
// {}
//
// std::string msrPathToVoice::asString () const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Tie" << ' ' << msrPathToVoiceKindAsString (fTieKind) <<
//     ", line " << fInputLineNumber;
//
//   return ss.str ();
// }

std::string msrPathToVoice::asString() const
{
  std::stringstream ss;

  ss <<
    "fBook->getBookName ()" << // JMI
    " => " <<
    fScore->getScoreName () <<
    " => ";

  // part groups can be nested
  for (S_msrPartGroup currentPartGroup : fPartGroupsList) {
    ss <<
      currentPartGroup->getPartGroupName () <<
      " => ";
  } // for

  ss <<
    fPart->getPartName () <<
    " => " <<
    fStaff->getStaffPathLikeName () <<
    " => " <<
    fVoice->getVoiceName ();

  return ss.str ();
}

void msrPathToVoice::print (std::ostream& os) const
{
  os <<
    "Book: " << "fBook->getBookName ()" << // JMI
  std::endl;

  ++gIndenter;

    os <<
      "Score: " << fScore->getScoreName () <<
    std::endl;

    // part groups can be nested
    for (S_msrPartGroup currentPartGroup : fPartGroupsList) {
      ++gIndenter;

      os <<
        "PartGroup: " << currentPartGroup->getPartGroupName () <<
        std::endl;
    } // for

    ++gIndenter;

      os <<
        "Part: " << fPart->getPartName () <<
      std::endl;

      ++gIndenter;

        os <<
          "Staff: " << fStaff->getStaffPathLikeName () <<
        std::endl;

        ++gIndenter;

          os <<
            "Voice: " << fVoice->getVoiceName () <<
          std::endl;

        for (S_msrPartGroup currentPartGroup : fPartGroupsList) {
          --gIndenter;
        } // for

        --gIndenter;

      --gIndenter;

    --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrPathToVoice& elt)
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
