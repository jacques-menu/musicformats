/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// formats
// JMI ???

// components
#include "components/mfcBasicTypes.h"
#include "components/mfcComponents.h"
#include "components/mfcLibraryComponent.h"

// representations
#include "msr/msrInterface.h"
#include "lpsr/lpsrInterface.h"
#include "bsr/bsrInterface.h"

// passes
#include "mxsr2musicxml/mxsr2musicxmlTranlatorInterface.h"
#include "bsr2bsr/bsr2bsrFinalizerInterface.h"
#include "msr2mxsr/msr2mxsrInterface.h"
#include "mxsr2msr/mxsr2msrSkeletonBuilderInterface.h"
#include "mxsr2msr/mxsr2msrTranslatorInterface.h"
#include "msr2msr/msr2msrInterface.h"
#include "lpsr2lilypond/lpsr2lilypondInterface.h"
#include "msr2lpsr/msr2lpsrInterface.h"
#include "bsr2braille/bsr2brailleTranslatorInterface.h"
#include "msr2bsr/msr2bsrInterface.h"
#include "musicxml2mxsr/musicxml2mxsrInterface.h"
#include "mxsr2guido/mxsr2guidoTranlatorInterface.h"

// converters
#include "msr2guido/msr2guidoInterface.h"
#include "msr2braille/msr2brailleInterface.h"
#include "msdl2braille/msdl2brailleInterface.h"
#include "msdl2guido/msdl2guidoInterface.h"
#include "msdlconverter/msdlConverterInterface.h"
#include "msdl2musicxml/msdl2musicxmlInterface.h"
#include "msdl2lilypond/msdl2lilypondInterface.h"
#include "musicxml2braille/musicxml2brailleInterface.h"
#include "msr2lilypond/msr2lilypondInterface.h"
#include "msr2musicxml/msr2musicxmlInterface.h"
#include "musicxml2musicxml/musicxml2musicxmlInterface.h"
#include "musicxml2lilypond/musicxml2lilypondInterface.h"
#include "musicxml2guido/musicxml2guidoInterface.h"

// mflibrary
#include "mflibrary/enebleCaughtExceptionsDisplay.h"
#include "mflibrary/mfMultiGenerationOah.h"
#include "mflibrary/mfAssert.h"
#include "mflibrary/mfMusicformatsError.h"
#include "mflibrary/mfBool.h"
#include "mflibrary/mfServiceRunData.h"
#include "mflibrary/mfEnumAll.h"
#include "mflibrary/mfStringsHandling.h"
#include "mflibrary/mfExceptions.h"
#include "mflibrary/mfTiming.h"
#include "mflibrary/mfIndentedTextOutput.h"

// oah
#include "oah/oahAtomsCollection.h""
#include "oah/displayOah.h"
#include "oah/oahBasicTypes.h"
#include "oah/oahRegularHandlers.h"
#include "oah/enableHarmoniesExtraOahIfDesired.h"
#include "oah/oahBrowsers.h"
#include "oah/oahVisitor.h"
#include "oah/enableTracingIfDesired.h"
#include "oah/oahEarlyOptions.h"
#include "oah/oahWae.h"
#include "oah/harmoniesExtraOah.h"
#include "oah/oahElements.h"
#include "oah/outputFileOah.h"
#include "oah/harmoniesExtraOah2manPage.h"
#include "oah/oahInsiderHandlers.h"
#include "oah/tracingOah.h"
#include "oah/oahOah.h"

// wae
#include "wae/enableAbortToDebugErrors.h"
#include "wae/wae.h"
#include "wae/waeHandlers.h"
#include "wae/waeOah.h"
