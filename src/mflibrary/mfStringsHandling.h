/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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


using namespace std;

namespace MusicFormats
{

// //______________________________________________________________________________
// class EXP mfMultiLineString
// /*
//   this class encapsulates a 'bool' value to guarantee it is initialized
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
//                           mfMultiLineString (const string& theString);
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
//     string                asString () const;
//
//     void                  print (ostream& os) const;
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
// EXP ostream& operator<< (ostream& os, const mfMultiLineString& themfMultiLineString);

//______________________________________________________________________________
struct mfStringQuoteEscaper
{
  /* usage:
      string dest = "";
      for_each( source.begin (), source.end (), mfStringQuoteEscaper (dest));
  */

  string&                 target;

  explicit                mfStringQuoteEscaper (string& t)
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
      string dest = "";
      for_each (
        source.begin (),
        source.end (),
        mfStringSpaceRemover (dest));
  */

  string&                 target;

  explicit                mfStringSpaceRemover (string& t)
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
      string dest = "";
      for_each (
        source.begin (),
        source.end (),
        mfStringSpaceReplacer (dest, ersatz));
  */

  string&                 target;
  char                    ersatz;

  explicit                mfStringSpaceReplacer (string& t, char ch)
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
string mfReplicateString (
  string str,
  int    times);

//______________________________________________________________________________
typedef vector<string> mfStringsVector;

void mfDisplayStringsVector (
  const mfStringsVector& stringsVector,
  const string&          title,
  ostream&               os);

//_______________________________________________________________________________
EXP void mfDisplayArgcAndArgv (
  int      argc,
  char*    argv[],
  ostream& os);

//_______________________________________________________________________________
/*
  A type to pass options and their values if any
  as well as arguments to the API functions

  In the pair:
    - first is the name of the option, such as '-title', or the argument
    - second is the value of the option, possibly empty,
      and empty for an argument
*/
typedef pair<string, string> mfStringsPair;

typedef vector<mfStringsPair> mfStringsPairVector;

// EXP void convertArgcArgvToStringPairVector (
//   int                  argc,
//   char*                argv[],
//   mfStringsPairVector& stringPairVector);

EXP void displayStringPairVector (
  const mfStringsPairVector& theOptionAndValueVector,
  ostream&                   os);

//______________________________________________________________________________
char* mfCharStarCat (
  char*        destination,
  const char*  source,
  const size_t destinationSize);

//______________________________________________________________________________
string mfInt2EnglishWord (int n);

//______________________________________________________________________________
string mfStringNumbersToEnglishWords (string str);

//______________________________________________________________________________
set<int> mfDecipherNaturalNumbersSetSpecification (
  const string& theSpecification,
  Bool          debugMode = false);

//______________________________________________________________________________
set<string> mfDecipherStringsSetSpecification (
  string theSpecification,
  Bool   debugMode = false);

//______________________________________________________________________________
list<int> mfExtractNumbersFromString (
  const string& theString, // can contain "1, 2, 17"
  Bool          debugMode = false);

//______________________________________________________________________________
// from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim string from start
EXP string& mfLeftTrim (string& s);
// trim string from end
EXP string& mfRightTrim (string& s);

// trim string from both ends
EXP inline string& mfTrimFromBothEnds (string& s) {
  return mfLeftTrim (mfRightTrim (s));
}

//______________________________________________________________________________
pair<string, string> mfExtractNamesPairFromString (
  const string& theString, // may contain "P1 = Bassoon"
  char          separator,
  Bool          debugMode = false);

//______________________________________________________________________________
string mfDoubleQuoteStringIfNonAlpha (
  const string& theString);

string mfQuoteStringIfNonAlpha (
  const string& theString);

string mfDoubleQuoteString (
  const string& theString);

string mfQuoteString (
  const string& theString);

string mfStringToLowerCase (
  const string& theString);

string mfStringToUpperCase (
  const string& theString);

//______________________________________________________________________________
EXP extern string mfBooleanAsString (Bool value);

//______________________________________________________________________________
void mfTrimString (
  string&     theString,
  const char* toBeRemoved = " \t\n\r\f\v");

//______________________________________________________________________________
string mfSingularOrPlural (
  int number, string singularName, string pluralName);

string mfSingularOrPluralWithoutNumber (
  int number, string singularName, string pluralName);

//______________________________________________________________________________
string mfEscapeDoubleQuotes (string s);

//______________________________________________________________________________
void mfConvertHTMLEntitiesToPlainCharacters (string& s);

//______________________________________________________________________________
void mfSplitStringIntoChunks (
  string        theString,
  string        theSeparator,
  list<string>& chunksList);

void mfSplitRegularStringAtEndOfLines (
  string        theString,
  list<string>& chunksList);

void mfSplitHTMLStringContainingEndOfLines ( // JMI
  string        theString,
  list<string>& chunksList);

//______________________________________________________________________________
string mfBaseName (const string& filename);
  // wait until c++17 for a standard library containing basename()...

//______________________________________________________________________________
string  mfMakeSingleWordFromString (const string& theString);

//______________________________________________________________________________
Bool mfStringIsInStringSet (
  const string&      theString,
  const set<string>& stringSet);

void mfStringSetMinusEquals (
  set<string>&       dest,
  const set<string>& source);

string mfStringSetAsString (
  const set<string>& stringSet);

void mfDisplayStringSet (
  const string&      title,
  const set<string>& stringSet,
  ostream&           os);

//______________________________________________________________________________
Bool mfFetchValueFromStringToStringMap (
  const string&              theKey,
  const map<string, string>& stringToStringMap,
  string&                    theValue);

string mfStringToStringMapAsString (
  const map<string, string>& stringToStringMap);

void mfDisplayStringToStringMap (
  const string&              title,
  const map<string, string>& stringToStringMap,
  ostream&                   os);

//______________________________________________________________________________
Bool mfKeyIsInStringToStringMultiMap (
  const string&                   theKey,
  const multimap<string, string>& stringToStringMultiMap);

Bool mfKeyValuePairIsInStringToStringMultiMap (
  const string&                   theKey,
  const multimap<string, string>& stringToStringMultiMap,
  const string&                   theValue);

string mfStringToStringMultiMapAsString (
  const multimap<string, string>& stringToStringMultiMap);

void mfDisplayStringToStringMultiMap (
  const string&                   title,
  const multimap<string, string>& stringToStringMultiMap,
  ostream&                        os);


}


#endif
