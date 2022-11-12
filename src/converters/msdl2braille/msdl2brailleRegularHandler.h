/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2brailleRegularHandler___
#define ___msdl2brailleRegularHandler___

#include "msdl2brailleInsiderHandler.h"

#include "oahRegularHandlers.h"

#include "msdl2brailleInsiderHandler.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP msdl2brailleRegularHandler : public oahRegularHandler
/*
  An msdl2braille regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalMsdl2brlInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2brailleRegularHandler> create (
                            const std::string&     serviceName,
                            const std::string&     handlerHeader,
                            S_msdl2brailleInsiderHandler
                                              insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2brailleRegularHandler (
                            const std::string&     serviceName,
                            const std::string&     handlerHeader,
                            S_msdl2brailleInsiderHandler
                                              insiderOahHandler);

    virtual               ~msdl2brailleRegularHandler ();

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments () const override;

    std::string           fetchOutputFileNameFromTheOptions () const override
                              {
                                return
                                  fInsiderHandler->
                                    fetchOutputFileNameFromTheOptions ();
                              }

    // quiet mode
    void                  enforceHandlerQuietness () override
                              {
                                fInsiderHandler->
                                  checkHandlerOptionsConsistency ();
                              }

    // consistency check
    void                  checkHandlerOptionsConsistency () override
                              {
                                fInsiderHandler->
                                  checkHandlerOptionsConsistency ();
                              }

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // private services
    // ------------------------------------------------------

    // regular handler groups creation

    void                  createRegularHandlerGroups () override;

    // the groups themselves

    void                  createInformationsRegularGroup ();

    void                  createFilesRegularGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

    void                  createPresentationRegularGroup ();

    void                  createPartsRegularGroup ();
    void                  createStavesRegularGroup ();
    void                  createVoicesRegularGroup ();

    void                  createTranspositionsRegularGroup ();

    void                  createClefsRegularGroup ();
    void                  createKeysRegularGroup ();
    void                  createTimesRegularGroup ();

    void                  createRestsRegularGroup ();
    void                  createNotesRegularGroup ();

    void                  creatBeamsRegularGroup ();

    void                  createArticulationsRegularGroup ();

    void                  createOrnamentsRegularGroup ();

    void                  createGraceNotesRegularGroup ();

    void                  createChordsRegularGroup ();

    void                  createTiesRegularGroup ();
    void                  createSlursRegularGroup ();
    void                  createLigaturesRegularGroup ();

    void                  createDynamicRegularGroup ();

    void                  createWedgesRegularGroup ();
    void                  createStringsRegularGroup ();

    void                  createTupletsRegularGroup ();

    void                  createLyricsRegularGroup ();

    void                  createTablaturesRegularGroup ();

    void                  createHarmoniesRegularGroup ();
    void                  createFiguredBassRegularGroup ();

    void                  createOutputRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdl2brailleRegularHandler> S_msdl2brailleRegularHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2brailleRegularHandler& elt);


}


#endif
