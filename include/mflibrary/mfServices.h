  /*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfServiceRunData___
#define ___mfServiceRunData___

#include <list>

#include "smartpointer.h"
#include "exports.h"

#include "mfPasses.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfService : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfService> create (
                            const std::string& serviceName);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfService (
                            const std::string& serviceName);

    virtual               ~mfService ();

  public:

    // set and get
    // ------------------------------------------------------

    // service name
    std::string           getServiceName () const
                              { return fServiceName; }

    // service pass descriptions
    const std::list<S_mfPassDescription>&
                          getServicePassDescriptionsList () const
                              { return fServicePassDescriptionsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPassToService (
                            S_mfPassDescription passDescription)
                              {
                                fServicePassDescriptionsList.push_back (
                                  passDescription);
                              }


    void                  fetchPassIDKindList (
                            std::list<mfPassIDKind>& passIDKindList) const;

    std::string           fetchServicePassDescriptionsAsString () const;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

    void                  printServiceForAboutOption (std::ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    size_t                fetchMaxPassIDKindAsStringLength () const;

  private:

    // private fields
    // ------------------------------------------------------

    // service name
    std::string           fServiceName;

    // service pass descriptions
    std::list<S_mfPassDescription>
                          fServicePassDescriptionsList;

  private:

    // private work fields
    // ------------------------------------------------------

  // JMIJMIJMI
};
typedef SMARTP<mfService> S_mfService;
EXP std::ostream& operator << (std::ostream& os, const S_mfService& elt);

EXP extern S_mfService gGlobalService;

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
//                             const S_oahHandler&  runOahHandler);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfServiceRunData (
                            const std::string& serviceName);

//                           mfServiceRunData (
//                             const std::string& serviceName,
//                             const S_oahHandler&  runOahHandler);

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
//     S_oahHandler          fRunOahHandler; JMI v0.9.67

    // input source name
    std::string           fInputSourceName;

    // run date
    std::string           fRunDateFull;
    std::string           fRunDateYYYYMMDD;

  private:

    // private work fields
    // ------------------------------------------------------

  // JMIJMIJMI v0.9.67
};
typedef SMARTP<mfServiceRunData> S_mfServiceRunData;
EXP std::ostream& operator << (std::ostream& os, const S_mfServiceRunData& elt);

EXP extern S_mfServiceRunData gGlobalCurrentServiceRunData;

EXP extern S_mfService gGlobalCurrentService;

}


#endif
