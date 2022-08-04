/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___LilyPondIssue34RegularHandler___
#define ___LilyPondIssue34RegularHandler___

#include "oahRegularHandlers.h"

#include "LilyPondIssue34InsiderHandler.h"

namespace MusicFormats
{
//_______________________________________________________________________________
class EXP LilyPondIssue34RegularHandler : public oahRegularHandler
/*
  An LilyPondIssue34 regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2gmnInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<LilyPondIssue34RegularHandler> create (
                            const string&     serviceName,
                            const string&     handlerHeader,
                            S_LilyPondIssue34InsiderHandler
                                              insiderOahHandler,
                            mfMultiGenerationOutputKind
                                              multiGenerationOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          LilyPondIssue34RegularHandler (
                            const string&     serviceName,
                            const string&     handlerHeader,
                            S_LilyPondIssue34InsiderHandler
                                              insiderOahHandler,
                            mfMultiGenerationOutputKind
                                              multiGenerationOutputKind);

    virtual               ~LilyPondIssue34RegularHandler ();

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
    mfMultiGenerationOutputKind
                          fMultiGenerationOutputKind;
};
typedef SMARTP<LilyPondIssue34RegularHandler> S_LilyPondIssue34RegularHandler;
EXP ostream& operator<< (ostream& os, const S_LilyPondIssue34RegularHandler& elt);


}


#endif
