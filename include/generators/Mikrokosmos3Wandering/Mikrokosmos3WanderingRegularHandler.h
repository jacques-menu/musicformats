/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___Mikrokosmos3WanderingRegularHandler___
#define ___Mikrokosmos3WanderingRegularHandler___

#include "oahRegularHandlers.h"

#include "Mikrokosmos3WanderingInsiderHandler.h"

namespace MusicFormats
{

//_______________________________________________________________________________
class EXP Mikrokosmos3WanderingRegularHandler : public oahRegularHandler
/*
  An Mikrokosmos3Wandering regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2gmnInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<Mikrokosmos3WanderingRegularHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const S_Mikrokosmos3WanderingInsiderHandler&
                                              insiderOahHandler,
                            mfMultiGenerationOutputKind multiGenerationOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          Mikrokosmos3WanderingRegularHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const S_Mikrokosmos3WanderingInsiderHandler&
                                              insiderOahHandler,
                            mfMultiGenerationOutputKind multiGenerationOutputKind);

    virtual               ~Mikrokosmos3WanderingRegularHandler ();

  public:

    // set and get
    // ------------------------------------------------------

    // generated output kind
    void                  setMultiGenerationOutputKind (
                            mfMultiGenerationOutputKind value)
                              { fMultiGenerationOutputKind = value; }
    mfMultiGenerationOutputKind   getMultiGenerationOutputKind () const
                              { return fMultiGenerationOutputKind; }

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

    void                  createAPIRegularGroup ();

    void                  createGenerateCodeRegularGroup ();

    void                  createGuidoRegularGroup ();

    void                  createBrailleRegularGroup ();

    void                  createFilesRegularGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

    void                  createPresentationRegularGroup ();

    void                  createPartsRegularGroup ();
    void                  createStavesRegularGroup ();
    void                  createVoicesRegularGroup ();

    void                  createClefsRegularGroup ();
    void                  createKeysRegularGroup ();
    void                  createTimesRegularGroup ();

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

    void                  createHarmoniesRegularGroup ();
    void                  createFiguredBassRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    mfMultiGenerationOutputKind     fMultiGenerationOutputKind;
};
typedef SMARTP<Mikrokosmos3WanderingRegularHandler> S_Mikrokosmos3WanderingRegularHandler;
EXP std::ostream& operator << (std::ostream& os, const S_Mikrokosmos3WanderingRegularHandler& elt);


}


#endif // ___Mikrokosmos3WanderingRegularHandler___
