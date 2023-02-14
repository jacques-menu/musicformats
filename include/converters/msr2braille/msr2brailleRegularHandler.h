/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2brailleRegularHandler___
#define ___msr2brailleRegularHandler___

#include "oahRegularHandlers.h"

#include "msr2brailleInsiderHandler.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP msr2brailleRegularHandler : public oahRegularHandler
/*
  An msr2braille regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2brailleInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2brailleRegularHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const S_msr2brailleInsiderHandler&
                                              insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2brailleRegularHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const S_msr2brailleInsiderHandler&
                                              insiderOahHandler);

    virtual               ~msr2brailleRegularHandler ();

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
typedef SMARTP<msr2brailleRegularHandler> S_msr2brailleRegularHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msr2brailleRegularHandler& elt);


}


#endif // ___msr2brailleRegularHandler___
