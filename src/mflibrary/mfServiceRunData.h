  /*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfServiceRunData___
#define ___mfServiceRunData___

#include <set>

#include "smartpointer.h"
#include "exports.h"

#include "oahBasicTypes.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfServiceRunData : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfServiceRunData> create (
                            const std::string& serviceName);

//     static SMARTP<mfServiceRunData> create ( // JMI
//                             const std::string& serviceName,
//                             S_oahHandler  runOahHandler);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfServiceRunData (
                            const std::string& serviceName);

//                           mfServiceRunData (
//                             const std::string& serviceName,
//                             S_oahHandler  runOahHandler);

    virtual               ~mfServiceRunData ();

  public:

    // set and get
    // ------------------------------------------------------

    // service name
    std::string           getServiceName () const
                              { return fServiceName; }

//     // OAH handler
//     S_oahHandler          getRunOahHandler () const
//                               { return fRunOahHandler; }

    // input source name
    void                  setInputSourceName (const std::string& name)
                              { fInputSourceName = name; }

    std::string           getInputSourceName () const
                              { return fInputSourceName; }

    // run date
    std::string           getRunDateFull () const
                              { return fRunDateFull; }

    std::string           getRunDateYYYYMMDD () const
                              { return fRunDateYYYYMMDD; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeRunData ();

    void                  initializeRunDate ();

  private:

    // private fields
    // ------------------------------------------------------

    // service name
    std::string           fServiceName;

//     // OAH handler
//     S_oahHandler          fRunOahHandler;

    // input source name
    std::string           fInputSourceName;

    // run date
    std::string           fRunDateFull;
    std::string           fRunDateYYYYMMDD;

  private:

    // private work fields
    // ------------------------------------------------------

  // JMIJMIJMI
};
typedef SMARTP<mfServiceRunData> S_mfServiceRunData;
EXP std::ostream& operator << (std::ostream& os, const S_mfServiceRunData& elt);

EXP extern S_mfServiceRunData gGlobalServiceRunData;


}


#endif
