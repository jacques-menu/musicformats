/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2lilypondRegularHandler___
#define ___msr2lilypondRegularHandler___

#include "oahRegularHandlers.h"

#include "msr2lilypondInsiderHandler.h"


namespace MusicFormats
{
//_______________________________________________________________________________
class EXP msr2lilypondRegularHandler : public oahRegularHandler
/*
  An msr2lilypond regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalMsr2lilypondInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2lilypondRegularHandler> create (
                            const string&     serviceName,
                            const string&     handlerHeader,
                            S_msr2lilypondInsiderHandler
                                              insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2lilypondRegularHandler (
                            const string&     serviceName,
                            const string&     handlerHeader,
                            S_msr2lilypondInsiderHandler
                                              insiderOahHandler);

    virtual               ~msr2lilypondRegularHandler ();

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments () const override;

    string                fetchOutputFileNameFromTheOptions () const override
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

    void                  print (ostream& os) const override;

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
typedef SMARTP<msr2lilypondRegularHandler> S_msr2lilypondRegularHandler;
EXP ostream& operator << (ostream& os, const S_msr2lilypondRegularHandler& elt);


}


#endif
