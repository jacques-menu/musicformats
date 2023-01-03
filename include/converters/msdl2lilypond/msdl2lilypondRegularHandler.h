/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2lilypondRegularHandler___
#define ___msdl2lilypondRegularHandler___

#include "oahRegularHandlers.h"

#include "msdl2lilypondInsiderHandler.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP msdl2lilypondRegularHandler : public oahRegularHandler
/*
  An msdl2lilypond regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalMsdl2lyInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2lilypondRegularHandler> create (
                          const std::string&     serviceName,
                          const std::string&     handlerHeader,
                          S_msdl2lilypondInsiderHandler
                                            insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2lilypondRegularHandler (
                          const std::string&     serviceName,
                          const std::string&     handlerHeader,
                          S_msdl2lilypondInsiderHandler
                                            insiderOahHandler);

    virtual               ~msdl2lilypondRegularHandler ();

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

    void                  createDisplayRegularGroup ();

    void                  createFilesRegularGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

    void                  createPartsRegularGroup ();
    void                  createStavesRegularGroup ();
    void                  createVoicesRegularGroup ();

    void                  createTranspositionsRegularGroup ();

    void                  createClefsRegularGroup ();
    void                  createKeysRegularGroup ();
    void                  createTimesRegularGroup ();

    void                  createRepeatsRegularGroup ();

    void                  createMeasuresRegularGroup ();

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

    void                  createTupletsRegularGroup ();

    void                  createLyricsRegularGroup ();

    void                  createStringsRegularGroup ();

    void                  createTablaturesRegularGroup ();

    void                  createHarmoniesRegularGroup ();
    void                  createFiguredBassRegularGroup ();

    void                  createBookRegularGroup ();
    void                  createHeaderRegularGroup ();
    void                  createPaperRegularGroup ();
    void                  createLayoutRegularGroup ();

    void                  createOutputRegularGroup ();

    void                  createMidiRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdl2lilypondRegularHandler> S_msdl2lilypondRegularHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2lilypondRegularHandler& elt);


}


#endif
