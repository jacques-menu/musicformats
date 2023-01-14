/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfPasses___
#define ___mfPasses___

#include <string>
#include <ostream>

#include <map>

#include "exports.h"
#include "smartpointer.h"

#include "mfBool.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
enum class mfPassIDKind {
  // a given pass may have different IDs depending on the service
  kMfPassID_UNKNOWN,

  kMfPassID_0, // options and arguments handling

  kMfPassID_1,
  kMfPassID_2, kMfPassID_2a, kMfPassID_2b,
  kMfPassID_3, kMfPassID_3a, kMfPassID_3b,
  kMfPassID_4, kMfPassID_4a, kMfPassID_4b,
  kMfPassID_5
};

std::string mfPassIDKindAsString (
  mfPassIDKind passIDKind);

std::ostream& operator << (std::ostream& os, const mfPassIDKind elt);

mfPassIDKind mfPassIDKindFromString (
  const std::string& theString);

extern std::map<std::string, mfPassIDKind>
  gGlobalMusicFormatsPassIDKindsMap;

std::string existingMusicFormatsPassIDKinds (size_t namesListMaxLength);

void initializeMusicFormatsPassIDKindsMap ();

void initializeMusicFormatsPassIDs ();

//______________________________________________________________________________
class EXP mfPassDescription : public smartable
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfPassDescription (
                            mfPassIDKind passIDKind,
                            std::string  passDescription);

    virtual               ~mfPassDescription ();

public:

    // set and get
    // ------------------------------------------------------

    mfPassIDKind          getPassIDKind () const
                              { return fPassIDKind; }

     std::string          getPassDescription () const
                              { return fPassDescription; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;


  private:

    // private fields
    // ------------------------------------------------------


    mfPassIDKind          fPassIDKind;
    std::string           fPassDescription;
};
typedef SMARTP<mfPassDescription> S_mfPassDescription;
EXP std::ostream& operator << (std::ostream& os, const S_mfPassDescription& elt);


}


#endif
