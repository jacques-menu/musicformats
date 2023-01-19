/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfEnableAbortToDebugErrorsSetting___
#define ___mfEnableAbortToDebugErrorsSetting___


#ifndef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  // comment the following definition if abort on internal errors is desired
  // CAUTION: DON'T USE THIS IN PRODUCTION CODE,
  // since that could kill a session on a web server, for example
  #define MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
#endif


#endif
