/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdrParserHandler___
#define ___msdrParserHandler___

#include "msdlTokens.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP msdlParserWaeHandler : public waeHandler
{
/* this class   is purely virtual
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
                            const msdlToken& theToken,
                            const std::string&    context) const = 0;

    virtual void          ignoringToken (
                            const msdlToken& theToken,
                            const std::string&    context) const = 0;

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

//________________________________________________________________________
class EXP msdlParserEnglishWaeHandler : public msdlParserWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParserEnglishWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParserEnglishWaeHandler ();

    virtual               ~msdlParserEnglishWaeHandler ();

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
                            const msdlToken& theToken,
                            const std::string&    context) const override;

    void                  ignoringToken (
                            const msdlToken& theToken,
                            const std::string&    context) const override;

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
typedef SMARTP<msdlParserEnglishWaeHandler> S_msdlParserEnglishWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlParserEnglishWaeHandler& elt);

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
                            const msdlToken& theToken,
                            const std::string&    context) const override;

    void                  ignoringToken (
                            const msdlToken& theToken,
                            const std::string&    context) const override;

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

//________________________________________________________________________
class EXP msdlParserItalianWaeHandler : public msdlParserWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParserItalianWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParserItalianWaeHandler ();

    virtual               ~msdlParserItalianWaeHandler ();

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
                            const msdlToken& theToken,
                            const std::string&    context) const override;

    void                  ignoringToken (
                            const msdlToken& theToken,
                            const std::string&    context) const override;

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
typedef SMARTP<msdlParserItalianWaeHandler> S_msdlParserItalianWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlParserItalianWaeHandler& elt);

//________________________________________________________________________
class EXP msdlParserGermanWaeHandler : public msdlParserWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParserGermanWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParserGermanWaeHandler ();

    virtual               ~msdlParserGermanWaeHandler ();

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
                            const msdlToken& theToken,
                            const std::string&    context) const override;

    void                  ignoringToken (
                            const msdlToken& theToken,
                            const std::string&    context) const override;

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
typedef SMARTP<msdlParserGermanWaeHandler> S_msdlParserGermanWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlParserGermanWaeHandler& elt);

//________________________________________________________________________
class EXP msdlParserSpanishWaeHandler : public msdlParserWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParserSpanishWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParserSpanishWaeHandler ();

    virtual               ~msdlParserSpanishWaeHandler ();

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
                            const msdlToken& theToken,
                            const std::string&    context) const override;

    void                  ignoringToken (
                            const msdlToken& theToken,
                            const std::string&    context) const override;

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
typedef SMARTP<msdlParserSpanishWaeHandler> S_msdlParserSpanishWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlParserSpanishWaeHandler& elt);

//________________________________________________________________________
class EXP msdlParserDutchWaeHandler : public msdlParserWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParserDutchWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParserDutchWaeHandler ();

    virtual               ~msdlParserDutchWaeHandler ();

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
                            const msdlToken& theToken,
                            const std::string&    context) const override;

    void                  ignoringToken (
                            const msdlToken& theToken,
                            const std::string&    context) const override;

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
typedef SMARTP<msdlParserDutchWaeHandler> S_msdlParserDutchWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlParserDutchWaeHandler& elt);


}


#endif
