/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___waeHandlers___
#define ___waeHandlers___

#include <set>

#include "smartpointer.h"

#include "mfStaticSettings.h"

#include "mfExceptions.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
class EXP waeHandler : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<waeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          waeHandler ();

    virtual               ~waeHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // warnings
    // ------------------------------------------------------

    virtual void          waeWarning (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message);

    virtual void          waeWarningWithMeasureInfo (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    // internal warnings
    // ------------------------------------------------------

    virtual void          waeInternalWarning (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message);

    virtual void          waeInternalWarningWithMeasureInfo (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);


    // errors without exception
    // ------------------------------------------------------

    virtual void          waeErrorWithoutException (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeErrorWithoutExceptionWithMeasureInfo (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    virtual void          waeErrorWithoutException (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeErrorWithoutExceptionWithMeasureInfo (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    // errors with exception
    // ------------------------------------------------------

    virtual void          waeErrorWithException (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except);

    virtual void          waeErrorWithExceptionWithMeasureInfo (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    virtual void          waeErrorWithException (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except);

    virtual void          waeErrorWithExceptionWithMeasureInfo (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    // internal errors with exceptions
    // ------------------------------------------------------

    virtual void          waeInternalErrorWithException (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except);

    virtual void          waeInternalErrorWithExceptionWithMeasureInfo (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    // errors
    // ------------------------------------------------------

    virtual void          waeError (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeErrorWithMeasureInfo (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    virtual void          waeError (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeErrorWithMeasureInfo (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    // internal errors
    // ------------------------------------------------------

    virtual void          waeInternalError (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeInternalErrorWithMeasureInfo (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    // display input line numbers
    // ------------------------------------------------------

    virtual void          displayWarningsAndErrorsInputLineNumbers ();

    // trace

#ifdef MF_TRACE_IS_ENABLED
    virtual void          waeTrace (
//                             const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message);

    virtual void          waeTraceWithMeasureInfo (
//                             const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);

    virtual void          waeTraceToStream (
//                             const std::string& context,
                            std::ostream&      os,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message);

    virtual void          waeTraceToStreamWithMeasureInfo (
//                             const std::string& context,
                            std::ostream&      os,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message,
                            const std::string  measureNumber,
                            int                scoreMeasuresNumber);
#endif // MF_TRACE_IS_ENABLED

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::set<int>         fWarningsInputLineNumbers;
    std::set<int>         fErrorsInputLineNumbers;
};
typedef SMARTP<waeHandler> S_waeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_waeHandler& elt);

//________________________________________________________________________
// hidden global WAE handler variable
EXP void setGlobalWaeHandler (S_waeHandler handler);

EXP S_waeHandler getGlobalWaeHandler ();

// a handy shortcut
#define gWaeHandler getGlobalWaeHandler ()

//________________________________________________________________________
EXP void initializeWAE ();
}


#endif // ___waeHandlers___
