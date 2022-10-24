/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "oahWae.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
S_mfServiceRunData gGlobalServiceRunData;

S_mfServiceRunData mfServiceRunData::create (
  const string& serviceName)
{
  mfServiceRunData* o = new
    mfServiceRunData (
      serviceName);
  assert (o != nullptr);
  return o;
}

// S_mfServiceRunData mfServiceRunData::create (
//   const string& serviceName,
//   S_oahHandler  runOahHandler)
// {
//   mfServiceRunData* o = new
//     mfServiceRunData (
//       serviceName,
//       runOahHandler);
//   assert (o != nullptr);
//   return o;
// }

mfServiceRunData::mfServiceRunData (
  const string& serviceName)
{
  // service name
  fServiceName = serviceName;

//   // OAH handler
//   fRunOahHandler = runOahHandler;

  // run date
  initializeRunDate ();
}

// mfServiceRunData::mfServiceRunData (
//   const string& serviceName,
//   S_oahHandler  runOahHandler)
// {
//   // service name
//   fServiceName = serviceName;
//
//   // OAH handler
//   fRunOahHandler = runOahHandler;
//
//   // run date
//   initializeRunDate ();
// }

mfServiceRunData::~mfServiceRunData ()
{}

void mfServiceRunData::initializeRunDate ()
{
  time_t      runRawtime;
  struct tm*  runTimeinfo;
  char buffer [80];

  time (&runRawtime);
  runTimeinfo = localtime (&runRawtime);

  strftime (buffer, 80, "%A %F @ %T %Z", runTimeinfo);
  fRunDateFull = buffer;

  strftime (buffer, 80, "%Y-%m-%d", runTimeinfo);
  fRunDateYYYYMMDD = buffer;
}

void mfServiceRunData::print (ostream& os) const
{
  os <<
    "The service run data are:" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 21;

  // service name
  // --------------------------------------

  os << left <<
    setw (fieldWidth) << "Service name" << " : " <<
    fServiceName <<
    endl;

//   // OAH handler
//   // --------------------------------------
//
//   os << left <<
//     setw (fieldWidth) << "OAH handler" << " : " <<
//     fRunOahHandler->getHandlerHeader () <<
//     endl;

  // input source name
  // --------------------------------------

  os << left <<
    setw (fieldWidth) << "fInputSourceName" << " : " << // JMIJMIJMI in oahHandler???
    fInputSourceName <<
    endl;

  // run date
  // --------------------------------------

  os << left <<
    setw (fieldWidth) << "Run date:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (fieldWidth) << "fRunDateFull" << " : " <<
    fRunDateFull <<
    endl <<
    setw (fieldWidth) << "fRunDateYYYYMMDD" << " : " <<
    fRunDateYYYYMMDD <<
    endl;

  --gIndenter;


  --gIndenter;
}

ostream& operator << (ostream& os, const mfServiceRunData& elt)
{
  elt.print (os);
  return os;
}


}
