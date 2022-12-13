/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
class EXP msdlScannerEnglishWaeHandler : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerEnglishWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerEnglishWaeHandler ();

    virtual               ~msdlScannerEnglishWaeHandler ();

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
typedef SMARTP<msdlScannerEnglishWaeHandler> S_msdlScannerEnglishWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerEnglishWaeHandler& elt);

//________________________________________________________________________
class EXP msdlScannerFrenchWaeHandler : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerFrenchWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerFrenchWaeHandler ();

    virtual               ~msdlScannerFrenchWaeHandler ();

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
typedef SMARTP<msdlScannerFrenchWaeHandler> S_msdlScannerFrenchWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerFrenchWaeHandler& elt);

//________________________________________________________________________
class EXP msdlScannerItalianWaeHandler : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerItalianWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerItalianWaeHandler ();

    virtual               ~msdlScannerItalianWaeHandler ();

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
typedef SMARTP<msdlScannerItalianWaeHandler> S_msdlScannerItalianWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerItalianWaeHandler& elt);

//________________________________________________________________________
class EXP msdlScannerGermanWaeHandler : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerGermanWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerGermanWaeHandler ();

    virtual               ~msdlScannerGermanWaeHandler ();

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
typedef SMARTP<msdlScannerGermanWaeHandler> S_msdlScannerGermanWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerGermanWaeHandler& elt);

//________________________________________________________________________
class EXP msdlScannerSpanishWaeHandler : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerSpanishWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerSpanishWaeHandler ();

    virtual               ~msdlScannerSpanishWaeHandler ();

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
typedef SMARTP<msdlScannerSpanishWaeHandler> S_msdlScannerSpanishWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerSpanishWaeHandler& elt);

//________________________________________________________________________
class EXP msdlScannerDutchWaeHandler : public msdlScannerWaeHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScannerDutchWaeHandler> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScannerDutchWaeHandler ();

    virtual               ~msdlScannerDutchWaeHandler ();

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
typedef SMARTP<msdlScannerDutchWaeHandler> S_msdlScannerDutchWaeHandler;
EXP std::ostream& operator << (std::ostream& os, const S_msdlScannerDutchWaeHandler& elt);


}


#endif
