/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdrParserWaeHandlers___
#define ___msdrParserWaeHandlers___

#include "msdlTokens.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP msdlParserWaeHandler : public waeHandler
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParserWaeHandler> create ();
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParserWaeHandler ();

    virtual               ~msdlParserWaeHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // identification
    virtual void          multipleIdentifications () const = 0;
    virtual void          stringExpectedAsTitle () const = 0;
    virtual void          stringExpectedAsComposer () const = 0;
    virtual void          stringExpectedAsOpus () const = 0;

    virtual void          unexpectedToken (
                            const msdlToken&   theToken,
                            const std::string& context) const = 0;

    virtual void          ignoringToken (
                            const msdlToken&   theToken,
                            const std::string& context) const = 0;

    virtual void          inputIsSyntacticallyCorrect () const = 0;
    virtual void          inputIsSyntacticallyIncorrect () const = 0;

    virtual void          mixedCommasAndQuotesInOctaveIndication () const = 0;

    virtual void          malformedNoteDuration () const = 0;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlParserWaeHandler> S_msdlParserWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandler& elt);


}


#endif
