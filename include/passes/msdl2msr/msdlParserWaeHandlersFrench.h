/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdrParserHandlersFrench___
#define ___msdrParserHandlersFrench___

// #include "msdlTokens.h"

#include "msdlParserWaeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP msdlParserFrenchWaeHandler : public msdlParserWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParserFrenchWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParserFrenchWaeHandler ();

    virtual               ~msdlParserFrenchWaeHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // identification
    void                  multipleIdentifications () const override;
    void                  stringExpectedAsTitle () const override;
    void                  stringExpectedAsComposer () const override;
    void                  stringExpectedAsOpus () const override;

    void                  unexpectedToken (
                            const msdlToken&   theToken,
                            const std::string& context) const override;

    void                  ignoringToken (
                            const msdlToken&   theToken,
                            const std::string& context) const override;

    void                  inputIsSyntacticallyCorrect () const override;
    void                  inputIsSyntacticallyIncorrect () const override;

    void                  mixedCommasAndQuotesInOctaveIndication () const override;

    void                  malformedNoteDuration () const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlParserFrenchWaeHandler> S_msdlParserFrenchWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlParserFrenchWaeHandler& elt);


}


#endif
