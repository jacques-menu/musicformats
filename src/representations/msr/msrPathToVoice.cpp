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


using namespace std;

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
    staffUpLink =
      voice->getVoiceStaffUpLink ();

  initializeFromStaff (staffUpLink);

  fVoice = voice; // only now
}

void msrPathToVoice::initializeFromStaff (S_msrStaff staff)
{
  // leave fVoice as nullptr to indicate all the voices in the staff

  fStaff = staff;

  fPart =
    fStaff->getStaffPartUpLink ();

  S_msrPartGroup
    partGroup =
      fPart->getPartPartGroupUpLink ();

  fPartGroupsList.push_front (partGroup);

  // part groups can be nested
  S_msrPartGroup
    partGroupPartGroupUpLink =
      partGroup->
        getPartGroupPartGroupUpLink ();

  // loop to the top-most part group
//  while (partGroupPartGroupUpLink) {
    fPartGroupsList.push_front (partGroup);

    partGroupPartGroupUpLink =
      partGroup->
        getPartGroupPartGroupUpLink ();
//  } // while

  fScore =
    partGroupPartGroupUpLink->
      getPartGroupScoreUpLink ();

  fBook =
    fScore->
      getScoreBookUpLink ();
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
      staff->getStaffPartUpLink () == fPart;
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
      voice->getVoiceStaffUpLink () == fStaff;
  }

  return result;
}

// void msrPathToVoice::acceptIn (basevisitor* v)
// {
//   if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
//     gLogStream <<
//       "% ==> msrPathToVoice::acceptIn ()" <<
//       endl;
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
//             endl;
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
//       endl;
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
//             endl;
//         }
//         p->visitEnd (elem);
//   }
// }
//
// void msrPathToVoice::browseData (basevisitor* v)
// {}
//
// string msrPathToVoice::asString () const
// {
//   stringstream s;
//
//   s <<
//     "Tie" << ' ' << msrPathToVoiceKindAsString (fTieKind) <<
//     ", line " << fInputLineNumber;
//
//   return s.str ();
// }

string msrPathToVoice::asString() const
{
  stringstream s;

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

void msrPathToVoice::print (ostream& os) const
{
  os <<
    "Book: " << "fBook->getBookName ()" << // JMI
  endl;

  ++gIndenter;

    os <<
      "Score: " << fScore->getScoreName () <<
    endl;

    // part groups can be nested
    for (S_msrPartGroup currentPartGroup : fPartGroupsList) {
      ++gIndenter;

      os <<
        "PartGroup: " << currentPartGroup->getPartGroupName () <<
        endl;
    } // for

    ++gIndenter;

      os <<
        "Part: " << fPart->getPartName () <<
      endl;

      ++gIndenter;

        os <<
          "Staff: " << fStaff->getStaffName () <<
        endl;

        ++gIndenter;

          os <<
            "Voice: " << fVoice->getVoiceName () <<
          endl;

        for (S_msrPartGroup currentPartGroup : fPartGroupsList) {
          --gIndenter;
        } // for

        --gIndenter;

      --gIndenter;

    --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrPathToVoice& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}


}
