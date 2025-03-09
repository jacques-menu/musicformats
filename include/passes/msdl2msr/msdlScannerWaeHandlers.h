/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdrScannerHandler___
#define ___msdrScannerHandler___

#include "msdlKeywords.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP msdlScannerWaeHandler : public waeHandler
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerWaeHandler> create ();
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerWaeHandler ();

    virtual               ~msdlScannerWaeHandler ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual void          illegalCharacter (char theChar) const = 0;
    virtual void          illegalCharacters (const std::string& theString) const = 0;

    virtual void          digitExpectedAfterDecimalDot (char theChar) const = 0;
    virtual void          digitExpectedInExponent (char theChar) const = 0;

    virtual void          illegalEscapedCharacterInString (char theChar) const = 0;

    virtual void          inputIsLexicallyCorrect (int numberOfTokens) const = 0;
    virtual void          inputIsLexicallyIncorrect (int numberOfTokens) const = 0;

    virtual void          inputIsAlreadyInMsdlKeywordsLanguage (
                            msdlKeywordsLanguageKind
                              keywordsConversionLanguage) const = 0;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlScannerWaeHandler> S_msdlScannerWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandler& elt);

//________________________________________________________________________
class EXP msdlScannerWaeHandlerEnglish : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerWaeHandlerEnglish> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerWaeHandlerEnglish ();

    virtual               ~msdlScannerWaeHandlerEnglish ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  illegalCharacter (char theChar) const override;
    void                  illegalCharacters (const std::string& theString) const override;

    void                  digitExpectedAfterDecimalDot (char theChar) const override;
    void                  digitExpectedInExponent (char theChar) const override;

    void                  illegalEscapedCharacterInString (char theChar) const override;

    void                  inputIsLexicallyCorrect (int numberOfTokens) const override;
    void                  inputIsLexicallyIncorrect (int numberOfTokens) const override;

    void                  inputIsAlreadyInMsdlKeywordsLanguage (
                            msdlKeywordsLanguageKind
                              keywordsConversionLanguage) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlScannerWaeHandlerEnglish> S_msdlScannerWaeHandlerEnglish;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerEnglish& elt);

//________________________________________________________________________
class EXP msdlScannerWaeHandlerFrench : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerWaeHandlerFrench> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerWaeHandlerFrench ();

    virtual               ~msdlScannerWaeHandlerFrench ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  illegalCharacter (char theChar) const override;
    void                  illegalCharacters (const std::string& theString) const override;

    void                  digitExpectedAfterDecimalDot (char theChar) const override;
    void                  digitExpectedInExponent (char theChar) const override;

    void                  illegalEscapedCharacterInString (char theChar) const override;

    void                  inputIsLexicallyCorrect (int numberOfTokens) const override;
    void                  inputIsLexicallyIncorrect (int numberOfTokens) const override;

    void                  inputIsAlreadyInMsdlKeywordsLanguage (
                            msdlKeywordsLanguageKind
                              keywordsConversionLanguage) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlScannerWaeHandlerFrench> S_msdlScannerWaeHandlerFrench;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerFrench& elt);

//________________________________________________________________________
class EXP msdlScannerWaeHandlerItalian : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerWaeHandlerItalian> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerWaeHandlerItalian ();

    virtual               ~msdlScannerWaeHandlerItalian ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  illegalCharacter (char theChar) const override;
    void                  illegalCharacters (const std::string& theString) const override;

    void                  digitExpectedAfterDecimalDot (char theChar) const override;
    void                  digitExpectedInExponent (char theChar) const override;

    void                  illegalEscapedCharacterInString (char theChar) const override;

    void                  inputIsLexicallyCorrect (int numberOfTokens) const override;
    void                  inputIsLexicallyIncorrect (int numberOfTokens) const override;

    void                  inputIsAlreadyInMsdlKeywordsLanguage (
                            msdlKeywordsLanguageKind
                              keywordsConversionLanguage) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlScannerWaeHandlerItalian> S_msdlScannerWaeHandlerItalian;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerItalian& elt);

//________________________________________________________________________
class EXP msdlScannerWaeHandlerGerman : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerWaeHandlerGerman> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerWaeHandlerGerman ();

    virtual               ~msdlScannerWaeHandlerGerman ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  illegalCharacter (char theChar) const override;
    void                  illegalCharacters (const std::string& theString) const override;

    void                  digitExpectedAfterDecimalDot (char theChar) const override;
    void                  digitExpectedInExponent (char theChar) const override;

    void                  illegalEscapedCharacterInString (char theChar) const override;

    void                  inputIsLexicallyCorrect (int numberOfTokens) const override;
    void                  inputIsLexicallyIncorrect (int numberOfTokens) const override;

    void                  inputIsAlreadyInMsdlKeywordsLanguage (
                            msdlKeywordsLanguageKind
                              keywordsConversionLanguage) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlScannerWaeHandlerGerman> S_msdlScannerWaeHandlerGerman;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerGerman& elt);

//________________________________________________________________________
class EXP msdlScannerWaeHandlerSpanish : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerWaeHandlerSpanish> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerWaeHandlerSpanish ();

    virtual               ~msdlScannerWaeHandlerSpanish ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  illegalCharacter (char theChar) const override;
    void                  illegalCharacters (const std::string& theString) const override;

    void                  digitExpectedAfterDecimalDot (char theChar) const override;
    void                  digitExpectedInExponent (char theChar) const override;

    void                  illegalEscapedCharacterInString (char theChar) const override;

    void                  inputIsLexicallyCorrect (int numberOfTokens) const override;
    void                  inputIsLexicallyIncorrect (int numberOfTokens) const override;

    void                  inputIsAlreadyInMsdlKeywordsLanguage (
                            msdlKeywordsLanguageKind
                              keywordsConversionLanguage) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlScannerWaeHandlerSpanish> S_msdlScannerWaeHandlerSpanish;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerSpanish& elt);

//________________________________________________________________________
class EXP msdlScannerWaeHandlerDutch : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerWaeHandlerDutch> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerWaeHandlerDutch ();

    virtual               ~msdlScannerWaeHandlerDutch ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  illegalCharacter (char theChar) const override;
    void                  illegalCharacters (const std::string& theString) const override;

    void                  digitExpectedAfterDecimalDot (char theChar) const override;
    void                  digitExpectedInExponent (char theChar) const override;

    void                  illegalEscapedCharacterInString (char theChar) const override;

    void                  inputIsLexicallyCorrect (int numberOfTokens) const override;
    void                  inputIsLexicallyIncorrect (int numberOfTokens) const override;

    void                  inputIsAlreadyInMsdlKeywordsLanguage (
                            msdlKeywordsLanguageKind
                              keywordsConversionLanguage) const override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlScannerWaeHandlerDutch> S_msdlScannerWaeHandlerDutch;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerDutch& elt);


}


#endif // ___msdrScannerHandler___
