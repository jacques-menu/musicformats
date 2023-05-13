/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___waeHandlersForGUI___
#define ___waeHandlersForGUI___

#include "waeHandlers.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
class EXP waeHandlerForGUI : public waeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<waeHandlerForGUI> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          waeHandlerForGUI ();

    virtual               ~waeHandlerForGUI ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  waeWarning (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message) override;

    void                  waeInternalWarning (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& message) override;


    void                  waeErrorWithoutException (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message) override;

    void                  waeErrorWithoutExceptionWithInputLocation (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message) override;

    void                  waeError (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message) override;

    void                  waeErrorWithInputLocation (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message) override;

    void                  waeErrorWithException (
                            const std::string& context,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except) override;

    void                  waeErrorWithExceptionWithInputLocation (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except) override;


    void                  waeInternalErrorWithInputLocation (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message) override;

    void                  waeInternalErrorWithExceptionWithInputLocation (
                            const std::string& context,
                            const std::string& inputSourceName,
                            int                inputLineNumber,
                            const std::string& sourceCodeFileName,
                            int                sourceCodeLineNumber,
                            const std::string& message,
                            S_mfException      except) override;

    void                  displayWarningsAndErrorsInputLineNumbers () override;

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<waeHandlerForGUI> S_waeHandlerForGUI;
EXP std::ostream& operator << (std::ostream& os, const S_waeHandlerForGUI& elt);


}


#endif // ___waeHandlersForGUI___
