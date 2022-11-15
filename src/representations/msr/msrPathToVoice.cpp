/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
S_msrPathToVoice msrPathToVoice::createFromVoice (S_msrVoice voice)
{
  msrPathToVoice* o =
    new msrPathToVoice ();
  assert (o != nullptr);

  o->initializeFromVoice (voice);

  return o;
}

S_msrPathToVoice msrPathToVoice::createFromStaff (S_msrStaff staff)
{
  msrPathToVoice* o =
    new msrPathToVoice ();
  assert (o != nullptr);

  o->initializeFromStaff (staff);

  return o;
}

msrPathToVoice::msrPathToVoice ()
{}

void msrPathToVoice::initializeFromVoice (S_msrVoice voice)
{
  S_msrStaff
    upLinkToStaff =
      voice->getVoiceUpLinkToStaff ();

  initializeFromStaff (upLinkToStaff);

  fVoice = voice; // only now
}

void msrPathToVoice::initializeFromStaff (S_msrStaff staff)
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
        getPartGroupUpLinkToPartGroup ();

  // loop to the top-most part group
//  while (partGroupUpLinkToPartGroup) {
    fPartGroupsList.push_front (partGroup);

    partGroupUpLinkToPartGroup =
      partGroup->
        getPartGroupUpLinkToPartGroup ();
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

Bool msrPathToVoice::pathContainsPartGroup (S_msrPartGroup partGroup)
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

Bool msrPathToVoice::pathContainsStaff (S_msrStaff staff)
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

Bool msrPathToVoice::pathContainsVoice (S_msrVoice voice)
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
//   if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
//     gLogStream <<
//       "% ==> msrPathToVoice::acceptIn ()" <<
//       std::endl;
//   }
//
//   if (visitor<S_msrPathToVoice>*
//     p =
//       dynamic_cast<visitor<S_msrPathToVoice>*> (v)) {
//         S_msrPathToVoice elem = this;
//
//         if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
//           gLogStream <<
//             "% ==> Launching msrPathToVoice::visitStart ()" <<
//             std::endl;
//         }
//         p->visitStart (elem);
//   }
// }
//
// void msrPathToVoice::acceptOut (basevisitor* v)
// {
//   if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
//     gLogStream <<
//       "% ==> msrPathToVoice::acceptOut ()" <<
//       std::endl;
//   }
//
//   if (visitor<S_msrPathToVoice>*
//     p =
//       dynamic_cast<visitor<S_msrPathToVoice>*> (v)) {
//         S_msrPathToVoice elem = this;
//
//         if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
//           gLogStream <<
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
//   std::stringstream s;
//
//   s <<
//     "Tie" << ' ' << msrPathToVoiceKindAsString (fTieKind) <<
//     ", line " << fInputLineNumber;
//
//   return s.str ();
// }

std::string msrPathToVoice::asString() const
{
  std::stringstream s;

  s <<
    "fBook->getBookName ()" << // JMI
    " => " <<
    fScore->getScoreName () <<
    " => ";

  // part groups can be nested
  for (S_msrPartGroup currentPartGroup : fPartGroupsList) {
    s <<
      currentPartGroup->getPartGroupName () <<
      " => ";
  } // for

  s <<
    fPart->getPartName () <<
    " => " <<
    fStaff->getStaffName () <<
    " => " <<
    fVoice->getVoiceName ();

  return s.str ();
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
          "Staff: " << fStaff->getStaffName () <<
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
