/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlParserHandlersDutch___
#define ___msdlParserHandlersDutch___

#include "msdlParserWaeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP msdlParserWaeHandlerDutch : public msdlParserWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParserWaeHandlerDutch> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParserWaeHandlerDutch ();

    virtual               ~msdlParserWaeHandlerDutch ();

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

    void                  malformedNoteNotesDuration () const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlParserWaeHandlerDutch> S_msdlParserWaeHandlerDutch;
EXP std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandlerDutch& elt);


}


#endif // ___msdlParserHandlersDutch___
