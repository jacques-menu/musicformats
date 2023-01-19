/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfStaticSettings___
#define ___mfStaticSettings___

// uncomment the following definition if no exceptions display is desired
#define MF_CAUGHT_EXCEPTIONS_DISPLAY_IS_ENABLED

// comment the following definition if abort on internal errors is desired
// CAUTION: DON'T USE THIS IN PRODUCTION CODE,
// since that could kill a session on a web server, for example
#define MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

// uncomment the following definition if abort on internal errors is desired
#define MF_SANITY_CHECKS_ARE_ENABLED

// uncomment the following definition if extra options are wanted
#define MF_HARMONIES_EXTRA_IS_ENABLED


// comment the following definition if no tracing is desired
#define MF_TRACING_IS_ENABLED

#ifdef MF_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif


#endif
