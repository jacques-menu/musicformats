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

#include "smartpointer.h"

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

    // warning and errors

    virtual void          waeWarning (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message);

    virtual void          waeInternalWarning (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message);


    virtual void          waeErrorWithoutException (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeErrorWithoutException (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeError (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeError (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeErrorWithException (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except);

    virtual void          waeErrorWithException (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except);


    virtual void          waeInternalError (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message);

    virtual void          waeInternalErrorWithException (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except);

    virtual void          displayWarningsAndErrorsInputLineNumbers ();

    // trace

    virtual void          waeTrace (
//                             const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message);

    virtual void          waeTrace (
//                             const std::string& context,
                            std::ostream&      os,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message);

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
extern EXP S_waeHandler gGlobalWaeHandler;

EXP void initializeWAE ();
}


#endif
