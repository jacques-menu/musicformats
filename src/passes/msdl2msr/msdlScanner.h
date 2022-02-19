/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlScanner___
#define ___msdlScanner___

#include "smartpointer.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"

#include "msdlScannerWaeHandlers.h"


using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
enum class msdlIgnoreSeparatorTokensKind {
  kIgnoreSeparatorTokensNo,
  kIgnoreSeparatorTokensYes
};

string msdlIgnoreSeparatorTokensKindAsString (
  msdlIgnoreSeparatorTokensKind ignoreSeparatorTokensKind);

ostream& operator<< (ostream& os, msdlIgnoreSeparatorTokensKind& elt);

//________________________________________________________________________
class msdlScanner : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlScanner> create (istream& inputStream);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlScanner (istream& inputStream);

    virtual               ~msdlScanner ();

    // set and get
    // ------------------------------------------------------

    int                   getCurrentLineNumber () const
                              { return fCurrentLineNumber; }

    Bool                  getInputIsEmpty () const
                              { return fInputIsEmpty; }

    const msdlToken&      getCurrentToken () const
                              { return fCurrentToken; }

    Bool                  getSourceIsLexicallyCorrect () const
                              { return fSourceIsLexicallyCorrect; }

  public:

    // public services
    // ------------------------------------------------------


    msdlTokenKind         fetchNextToken (
                            msdlIgnoreSeparatorTokensKind
                              ignoreSeparatorTokens);

    void                  scanWholeInputAtOnce (); // for tests

    void                  scanAllTheInputAtOnce (
                            msdlIgnoreSeparatorTokensKind
                              ignoreSeparatorTokens);

    void                  translateAllTheInputToKeywordsLanguage (
                            msdlKeywordsLanguageKind keywordsConversionLanguage,
                            msdlCommentsTypeKind     commentsTypeKind);

  public:

    // print
    // ------------------------------------------------------

    string                currentLocationAsString () const;

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  populateInputString (istream& inputStream);

    char                  fetchNextCharacter ();

     void                 handleEndOfLine (const string& context);

    string                currentCharacterAsString () const;

    void                  handlePercent ();
    void                  handleSlash ();

    void                  acceptAString ();

    void                  acceptAName ();

    void                  acceptAnInteger ();

  private:

    // private fields
    // ------------------------------------------------------

    // trace
#ifdef TRACING_IS_ENABLED
    Bool                  fTraceTokens;
    Bool                  fTraceTokensDetails;
#endif

    // user language
    msdlUserLanguageKind  fUserLanguageKind;

    // input stream
    istream&              fInputStream;
    Bool                  fInputIsEmpty;

    // input string
    string                fInputString;     // modern machines can cope with large data
    int                   fInputStringSize; // denormalization for speed

    /*
      line numbers                  start at 1
      positions in the input string start at 0 (array indexing)
      positions in the current line start at 1
    */

    // lines
    int                   fCurrentLineNumber;
    int                   fCurrentLinePositionInInput;
    int                   fCurrentLineSize;
    int                   fCurrentPositionInLine;
                            // -1 before the first character in the line,
                            // then from 0 to the length of the line minus 1

    // characters
    int                   fCurrentPositionInInput;
    char                  fCurrentCharacter;

    Bool                  fNextCharacterIsAvailable;
                            // initially false;
                            // becomes true when a character has be read ahead
                            // to determine that the current token is complete;
                            // remains then true only till the next call to fetchNextToken

    // tokens
    int                   fCurrentTokenPositionInInput;

    int                   fCurrentTokenLineNumber;
    int                   fCurrentTokenPositionInLine;

    msdlToken             fCurrentToken;
    msdlTokenKind&        fCurrentTokenKind;
    msdlTokenDescription& fCurrentTokenDescription;

    int                   fTokensCounter;

    // keywords
    msdlKeywordsLanguageKind
                          fKeywordsInputLanguageKind;

    // tokens list
    Bool                  fAppendTokensToList;
    msdlTokensList        fTokensList;

    // warnings and errors
    S_msdlScannerWaeHandler
                          fScannerWaeHandler;

    // lexical correctness
    Bool                  fSourceIsLexicallyCorrect;
};
typedef SMARTP<msdlScanner> S_msdlScanner;
EXP ostream& operator<< (ostream& os, const msdlScanner& elt);


}


#endif
