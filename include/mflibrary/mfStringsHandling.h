/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfStringsHandling___
#define ___mfStringsHandling___

#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>

#include <functional>

#include "exports.h"

#include "mfBool.h"


namespace MusicFormats
{

// //______________________________________________________________________________
// class EXP mfMultiLineString
// /*
//   this class   encapsulates a 'bool' value to guarantee it is initialized
// */
// {
//   public:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           mfMultiLineString ();
//
//                           mfMultiLineString (bool value);
//
//                           mfMultiLineString (const std::string& theString);
//
//     virtual               ~mfMultiLineString ();
//
//     // copy constructor
//     // ------------------------------------------------------
//
//                           mfMultiLineString (const mfMultiLineString& obj);
//
// public:
//
//     // set and get
//     // ------------------------------------------------------
//
//     // set the value
//     void                  setValue (bool value)
//                               { fValue = value; }
//
//     // get the value
//     bool                  getValue () const
//                               { return fValue; }
//
//   public:
//
//     // public services
//     // ------------------------------------------------------
//
//     // explicit conversion
//     explicit              operator bool () const { return fValue; }
//
//     // copy assignment operator
//     // ------------------------------------------------------
//
//     mfMultiLineString&                 operator= (const mfMultiLineString& themfMultiLineString);
//
// /*
//     // Declare prefix and postfix decrement operators.
//     Point& operator--();       // Prefix decrement operator.
//     Point operator--(int);     // Postfix decrement operator.
// */
//
//     // negation, prefix operator
//     mfMultiLineString                  operator! () const;
//
//     // dyadic operators
//     mfMultiLineString                  operator&& (const mfMultiLineString& othermfMultiLineString) const;
//     mfMultiLineString                  operator|| (const mfMultiLineString& othermfMultiLineString) const;
//
//     // compare value JMI ???
//     mfMultiLineString                  operator == (const mfMultiLineString &othermfMultiLineString) const
//                               { return fValue == othermfMultiLineString.fValue; }
//     mfMultiLineString                  operator != (const mfMultiLineString &othermfMultiLineString) const
//                               { return fValue != othermfMultiLineString.fValue; }
//
//   public:
//
//     // print
//     // ------------------------------------------------------
//
//     std::string           asString () const;
//
//     void                  print (std::ostream& os) const;
//
//
//   private:
//
//     // private fields
//     // ------------------------------------------------------
//
//     bool                  fValue;
// };
//
// //EXP mfMultiLineString operator&& (const mfMultiLineString& leftmfMultiLineString, const mfMultiLineString& rightmfMultiLineString);
// EXP mfMultiLineString operator&& (const mfMultiLineString& leftmfMultiLineString, const bool& themfMultiLineString);
// EXP mfMultiLineString operator&& (const bool& themfMultiLineString, const mfMultiLineString& rightmfMultiLineString);
//
// //EXP mfMultiLineString operator|| (const mfMultiLineString& leftmfMultiLineString, const mfMultiLineString& rightmfMultiLineString);
// EXP mfMultiLineString operator|| (const mfMultiLineString& leftmfMultiLineString, const bool& themfMultiLineString);
// EXP mfMultiLineString operator|| (const bool& themfMultiLineString, const mfMultiLineString& rightmfMultiLineString);
//
// EXP std::ostream& operator << (std::ostream& os, const mfMultiLineString& themfMultiLineString);

//______________________________________________________________________________
struct mfStringQuoteEscaper
{
  /* usage:
      std::string dest = "";
      for_each( source.begin (), source.end (), mfStringQuoteEscaper (dest));
  */

  std::string&                 target;

  explicit                mfStringQuoteEscaper (std::string& t)
                            : target (t)
                              {}

  void                    operator() (char ch) const
                              {
                                 if( ch == '"') {
                                   // or switch on any character that
                                   // needs escaping like '\' itself
                                    target.push_back ('\\');
                                 }
                                 target.push_back (ch);
                              }
};

//______________________________________________________________________________
struct mfStringSpaceRemover
{
  /* usage:
      std::string dest = "";
      for_each (
        source.begin (),
        source.end (),
        mfStringSpaceRemover (dest));
  */

  std::string&                 target;

  explicit                mfStringSpaceRemover (std::string& t)
                            : target (t)
                              {}

  void                    operator() (char ch) const
                              {
                                if (ch != ' ') {
                                  target.push_back (ch);
                                }
                              }
};

//______________________________________________________________________________
struct mfStringSpaceReplacer
{
  /* usage:
      std::string dest = "";
      for_each (
        source.begin (),
        source.end (),
        mfStringSpaceReplacer (dest, ersatz));
  */

  std::string&                 target;
  char                    ersatz;

  explicit                mfStringSpaceReplacer (std::string& t, char ch)
                            : target (t), ersatz (ch)
                              {}

  void                    operator() (char ch) const
                              {
                                if (ch == ' ')
                                  target.push_back (ersatz);
                                else
                                  target.push_back (ch);
                              }
};

//______________________________________________________________________________
std::string mfReplicateChar (
  char theChar,
  int  times);

std::string mfReplicateString (
  std::string theString,
  int         times);

//______________________________________________________________________________
typedef std::vector<std::string> mfStringsVector;

void mfDisplayStringsVector (
  const mfStringsVector& stringsVector,
  const std::string&     title,
  std::ostream&          os);

//_______________________________________________________________________________
EXP void mfDisplayArgcAndArgv (
  int      argc,
  char*    argv[],
  std::ostream& os);

//_______________________________________________________________________________
/*
  A type to pass options and their values if any
  as well as arguments to the API functions

  In the std::pair:
    - first is the name of the option, such as '-title', or the argument
    - second is the value of the option, possibly empty,
      and empty for an argument
*/
typedef std::pair<std::string, std::string> mfStringsPair;

typedef std::vector<mfStringsPair> mfStringsPairVector;

// EXP void convertArgcArgvToStringPairVector (
//   int                  argc,
//   char*                argv[],
//   mfStringsPairVector& stringPairVector);

EXP void displayStringPairVector (
  const mfStringsPairVector& theOptionAndValueVector,
  std::ostream&              os);

//______________________________________________________________________________
char* mfCharStarCat (
  char*        destination,
  const char*  source,
  const size_t destinationSize);

//______________________________________________________________________________
std::string mfInt2EnglishWord (int n);

//______________________________________________________________________________
std::string mfStringNumbersToEnglishWords (std::string str);

//______________________________________________________________________________
std::set<int> mfDecipherNaturalNumbersSetSpecification (
  const std::string& theSpecification,
  Bool               debugMode = false);

//______________________________________________________________________________
std::set<std::string> mfDecipherStringsSetSpecification (
  std::string theSpecification,
  Bool   debugMode = false);

//______________________________________________________________________________
std::list<int> mfExtractNumbersFromString (
  const std::string& theString, // can contain "1, 2, 17"
  Bool               debugMode = false);

//______________________________________________________________________________
// from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim std::string from start
EXP std::string& mfLeftTrim (std::string& s);
// trim std::string from end
EXP std::string& mfRightTrim (std::string& s);

// trim std::string from both ends
EXP inline std::string& mfTrimFromBothEnds (std::string& s) {
  return mfLeftTrim (mfRightTrim (s));
}

//______________________________________________________________________________
std::pair<std::string, std::string> mfExtractNamesPairFromString (
  const std::string& theString, // may contain "P1 = Bassoon"
  char               separator,
  Bool               debugMode = false);

//______________________________________________________________________________
std::string mfDoubleQuoteStringIfNonAlpha (
  const std::string& theString);

std::string mfQuoteStringIfNonAlpha (
  const std::string& theString);

std::string mfDoubleQuoteString (
  const std::string& theString);

std::string mfQuoteString (
  const std::string& theString);

std::string mfStringToLowerCase (
  const std::string& theString);

std::string mfStringToUpperCase (
  const std::string& theString);

//______________________________________________________________________________
EXP extern std::string mfBooleanAsString (Bool value);

//______________________________________________________________________________
void mfTrimString (
  std::string& theString,
  const char*  toBeRemoved = " \t\n\r\f\v");

//______________________________________________________________________________
std::string mfSingularOrPlural (
  int         number,
  std::string singularName,
  std::string pluralName);

std::string mfSingularOrPluralWithoutNumber (
  int         number,
  std::string singularName,
  std::string pluralName);

//______________________________________________________________________________
std::string mfEscapeDoubleQuotes (std::string s);

//______________________________________________________________________________
void mfConvertHTMLEntitiesToPlainCharacters (std::string& s);

//______________________________________________________________________________
void mfSplitStringIntoChunks (
  std::string             theString,
  std::string             theSeparator,
  std::list<std::string>& chunksList);

void mfSplitRegularStringAtEndOfLines (
  std::string             theString,
  std::list<std::string>& chunksList);

void mfSplitHTMLStringContainingEndOfLines ( // JMI
  std::string             theString,
  std::list<std::string>& chunksList);

//______________________________________________________________________________
std::string mfBaseName (const std::string& filename);
  // wait until C++17 for a standard library containing basename()... JMI v0.9.66

//______________________________________________________________________________
std::string  mfMakeSingleWordFromString (const std::string& theString);

//______________________________________________________________________________
Bool mfStringIsInStringSet (
  const std::string&           theString,
  const std::set<std::string>& stringSet);

void mfStringSetMinusEquals (
  std::set<std::string>&       dest,
  const std::set<std::string>& source);

std::string mfStringSetAsString (
  const std::set<std::string>& stringSet);

std::string mfStringSetAsBracketedString (
  const std::set<std::string>& stringSet);

void mfDisplayStringSet (
  const std::string&           title,
  const std::set<std::string>& stringSet,
  std::ostream&                os);

//______________________________________________________________________________
Bool mfFetchValueFromStringToStringMap (
  const std::string&                        theKey,
  const std::map<std::string, std::string>& stringToStringMap,
  std::string&                              theValue);

std::string mfStringToStringMapAsString (
  const std::map<std::string, std::string>& stringToStringMap);

void mfDisplayStringToStringMap (
  const std::string&                        title,
  const std::map<std::string, std::string>& stringToStringMap,
  std::ostream&                             os);

//______________________________________________________________________________
Bool mfKeyIsInStringToStringMultiMap (
  const std::string&                             theKey,
  const std::multimap<std::string, std::string>& stringToStringMultiMap);

Bool mfKeyValuePairIsInStringToStringMultiMap (
  const std::string&                             theKey,
  const std::multimap<std::string, std::string>& stringToStringMultiMap,
  const std::string&                             theValue);

std::string mfStringToStringMultiMapAsString (
  const std::multimap<std::string, std::string>& stringToStringMultiMap);

void mfDisplayStringToStringMultiMap (
  const std::string&                             title,
  const std::multimap<std::string, std::string>& stringToStringMultiMap,
  std::ostream&                                  os);

//______________________________________________________________________________
int countTwoBytesWideCharactersInString (const std::string theString);


}


#endif // ___mfStringsHandling___
