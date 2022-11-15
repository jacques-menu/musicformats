/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___waeEnableAbortToDebugErrors___
#define ___waeEnableAbortToDebugErrors___


// comment the following definition if abort on internal errors is desired
// CAUTION: DON'T USE THIS IN PRODUCTION CODE,
// since that could kill a session on a web server, for example

#ifndef ABORT_TO_DEBUG_ERRORS
  #define ABORT_TO_DEBUG_ERRORS
#endif


#endif
