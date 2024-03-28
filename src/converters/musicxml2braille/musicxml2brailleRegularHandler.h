/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2brailleRegularHandler___
#define ___musicxml2brailleRegularHandler___

#include "musicxml2brailleInsiderHandler.h"

#include "oahRegularHandlers.h"

#include "musicxml2brailleInsiderHandler.h"


namespace MusicFormats
{

//_______________________________________________________________________________
class EXP xml2brlRegularHandler : public oahRegularHandler
/*
  An xml2brl regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2brlInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlRegularHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const S_xml2brlInsiderHandler&
                                              insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          xml2brlRegularHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const S_xml2brlInsiderHandler&
                                              insiderOahHandler);

    virtual               ~xml2brlRegularHandler ();

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

    void                  createInputRegularGroup ();

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
typedef SMARTP<xml2brlRegularHandler> S_xml2brlRegularHandler;
EXP std::ostream& operator << (std::ostream& os, const S_xml2brlRegularHandler& elt);


}


#endif // ___musicxml2brailleRegularHandler___
