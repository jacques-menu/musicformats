/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlParserHandlersFrench___
#define ___msdlParserHandlersFrench___

#include "msdlParserWaeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP msdlParserWaeHandlerFrench : public msdlParserWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParserWaeHandlerFrench> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParserWaeHandlerFrench ();

    virtual               ~msdlParserWaeHandlerFrench ();

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
typedef SMARTP<msdlParserWaeHandlerFrench> S_msdlParserWaeHandlerFrench;
EXP std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandlerFrench& elt);


}


#endif // ___msdlParserHandlersFrench___
