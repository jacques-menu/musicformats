/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrSkeletonBuilderInterface___
#define ___mxsr2msrSkeletonBuilderInterface___


namespace MusicFormats
{

//_______________________________________________________________________________
EXP S_msrScore translateMxsrToMsrSkeleton (
  const Sxmlelement&   theMxsr,
  const S_msrOahGroup& msrOpts,
  mfPassIDKind         passIDKind,
  const std::string&   passDescription);

//_______________________________________________________________________________
// EXP void displayMsrScoreSkeleton ( // UNUSED JMI v0.9.66
//   S_msrOahGroup&     msrOpts,
//   const S_msrScore&  theMsrScore,
//   mfPassIDKind       passIDKind,
//   const std::string& passDescription);


}


#endif // ___mxsr2msrSkeletonBuilderInterface___
