/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include <cassert>

#include <map>

#include "mfStringsHandling.h"
#include "mfIndentedTextOutput.h"

#include <algorithm>    // for_each, find_if


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
//#define DEBUG_SPLITTING

//______________________________________________________________________________
string mfReplicateString (
  string str,
  int    times)
{
  string result;

  for (int i = 0; i < times; ++i)
    result += str;

  return result;
}

//______________________________________________________________________________
/* JMI
string replaceSubstringInString (
  string str,
  string subString,
  string ersatz)
{
  string result = str;

  size_t found = result.find (subString);

  if (found != string::npos) {
    result.replace (found, subString.size (), ersatz);
  }

  return result;
}
*/

//_______________________________________________________________________________
EXP void mfDisplayArgcAndArgv (
  int      argc,
  char*    argv[],
  ostream& os)
{
  os <<
    "The contents of argv is:" <<
    endl;

  ++gIndenter;

  for (int i = 0; i < argc; ++i) {
    os <<
      "argv [" <<
      right << setw (2) <<
      i <<
      "]: " << argv [i] <<
      endl;
  } // for

  --gIndenter;
}

//______________________________________________________________________________
void mfDisplayStringsVector (
  const mfStringsVector& stringsVector,
  const string&          title,
  ostream&               os)
{
  os <<
    title <<
    " contains " <<
    mfSingularOrPlural (
      stringsVector.size (), "element", "elements");

  if (stringsVector.size ()) {
    os << ":" << endl;

    ++gIndenter;

    int counter = 0;
    for (string theString : stringsVector) {
      os <<
        right << setw (2) << counter++ <<
        ": \"" <<
        theString <<
        "\"" <<
        endl;
    } //for

    --gIndenter;
  }
  else {
    os << endl;
  }
}

//______________________________________________________________________________
string mfInt2EnglishWord (int n)
{
  if (n == -9) {
    assert (false); // JMI
  }

  stringstream s;

  if (n < 0) {
    s << "Minus_";
    n = -n;
  }

  if (n >= 1000) {
    int nDiv1000    = n / 1000;
    int nModulo1000 = n % 1000;

    return
      mfInt2EnglishWord (nDiv1000) +
      "Thousand" +
      mfInt2EnglishWord (nModulo1000);
  }

  else if (n >= 100) {
  //return "LargerThanNinetyNine";
    int nDiv100    = n / 100;
    int nModulo100 = n % 100;

    return
      mfInt2EnglishWord (nDiv100) +
      "HundredAnd" +
      mfInt2EnglishWord (nModulo100);
  }

  else {
    switch (n) {
      case 0:
        s << "Zero";
        break;
      case 1:
        s << "One";
        break;
      case 2:
        s << "Two";
        break;
      case 3:
        s << "Three";
        break;
      case 4:
        s << "Four";
        break;
      case 5:
        s << "Five";
        break;
      case 6:
        s << "Six";
        break;
      case 7:
        s << "Seven";
        break;
      case 8:
        s << "Eight";
        break;
      case 9:
        s << "Nine";
        break;
     case 10:
        s << "Ten";
        break;
      case 11:
        s << "Eleven";
        break;
      case 12:
        s << "Twelve";
        break;
      case 13:
        s << "Thirteen";
        break;
      case 14:
        s << "Fourteen";
        break;
      case 15:
        s << "Fifteen";
        break;
      case 16:
        s << "Sixteen";
        break;
      case 17:
        s << "Seventeen";
        break;
      case 18:
        s << "Eighteen";
        break;
      case 19:
        s << "Nineteen";
        break;

      default: {
        // n >= 20
        int nDiv10    = n / 10;
        int nModulo10 = n % 10;

        switch (nDiv10) {
          case 2:
            s << "Twenty";
            break;
          case 3:
            s << "Thirty";
            break;
          case 4:
            s << "Fourty";
            break;
          case 5:
            s << "Fifty";
            break;
          case 6:
            s << "Sixty";
            break;
          case 7:
            s << "Seventy";
            break;
          case 8:
            s << "Eighty";
            break;
          case 9:
            s << "Ninety";
            break;
        } // switch
        s << mfInt2EnglishWord (nModulo10);
      } // default
    } // switch
  }

  return s.str ();
}

//______________________________________________________________________________
string mfStringNumbersToEnglishWords (string str)
{
  if (! str.size ()) {
    return "NONE";
  }

  enum workState {
    kInitialState, kWorkingOnDigits, kWorkingOnNonDigits };

  vector<string> chunks;
  vector<int>    states;

  workState state = kInitialState;

  string::const_iterator
    iBegin = str.begin (),
    iEnd   = str.end (),
    i      = iBegin;

  for ( ; ; ) {
    char ch = (*i);

    if( isdigit(ch)) {
      // digit
      if (state != kWorkingOnDigits) {
        // create a new chunck for digits
        chunks.push_back ("");
        states.push_back (kWorkingOnDigits);
        state = kWorkingOnDigits;
      }
      chunks.back().push_back(ch);
    }
    else {
      // non digit
      if (state != kWorkingOnNonDigits) {
        // create a new chunck for non digits
        chunks.push_back ("");
        states.push_back (kWorkingOnNonDigits);
        state = kWorkingOnNonDigits;
      }
      chunks.back().push_back(ch);
    }
    if (++i == iEnd) break;
  } // for

  string result = "";

  for (unsigned int i = 0; i < chunks.size (); ++i) {
    if (states[i] == kWorkingOnDigits) {
      int integerValue;

      istringstream inputStream (chunks[i]);

      inputStream >> integerValue;

      result += mfInt2EnglishWord (integerValue);
    }
    else {
      result += chunks[i];
    }
  } // for

  return result;
};

//______________________________________________________________________________
int consumeDecimalNumber (
  string::const_iterator  theStringIterator,
  string::const_iterator& remainingStringIterator,
  Bool                    debugMode)
{
  string::const_iterator cursor = theStringIterator;
  int    result = 0;

  if (! isdigit (*cursor)) {
    gLogStream <<
      "consumeDecimalNumber (" << *cursor <<
      "), " << *cursor << " is no decimal digit!" <<
      endl;
    }

  while (isdigit (*cursor)) {
    if (debugMode) {
      gLogStream <<
        "--> consumeDecimalNumber: cursor = |" <<
        *cursor <<
        "|" <<
        endl;
    }

    result = result*10 + (*cursor-'0');

    ++cursor;
  } // while

  remainingStringIterator = cursor;

  if (debugMode) {
    gLogStream <<
      "--> consumeDecimalNumber: result = " << result <<
      ", *remainingStringIterator = |" << *remainingStringIterator <<
      "|" <<
      endl;
  }

  return result;
}

//______________________________________________________________________________
set<int> mfDecipherNaturalNumbersSetSpecification (
  const string& theString,
  Bool          debugMode)
{
  // A naturalNumbersSetSpecification sample is: "7,15-19,^16-17"

  set<int> result;

  if (debugMode) {
    gLogStream <<
      "--> mfDecipherNaturalNumbersSetSpecification, theString = |" << theString <<
      "|" <<
      endl;
  }

  if (theString.size ()) {
    string::const_iterator
      cursor = theString.begin ();

    while (1) {
      if (debugMode) {
        gLogStream <<
          "--> mfDecipherNaturalNumbersSetSpecification: cursor = |" <<
          *cursor << "|" <<
          endl;
      }

      int negated = 0;

      if (*cursor == '^') {
        ++cursor;
        negated = 1;
      }

      int
        intervalStartNumber =
          consumeDecimalNumber (cursor, cursor, debugMode),
        intervalEndNumber;

      if (*cursor == '-') {
        ++cursor;

        if (debugMode) {
          gLogStream <<
            "--> mfDecipherNaturalNumbersSetSpecification after '-' : cursor = |" <<
            *cursor <<
            "|" <<
            endl << endl;
        }

        intervalEndNumber =
          consumeDecimalNumber (cursor, cursor, debugMode);
      }

      else {
        intervalEndNumber = intervalStartNumber;
      }

      if (debugMode) {
        gLogStream <<
          "--> mfDecipherNaturalNumbersSetSpecification" <<
          ", intervalStartNumber = " << intervalStartNumber <<
          ", intervalEndNumber = " << intervalEndNumber <<
          ": *cursor = |" << *cursor << "|" <<
          endl;
      }

      for (int i = intervalStartNumber; i <= intervalEndNumber; ++i) {
        if (negated) {
          result.erase (i);
        }
        else {
          result.insert (i);
        }
      } // for

      if (*cursor != ',') {
        if (debugMode) {
          gLogStream <<
            "--> mfDecipherNaturalNumbersSetSpecification, after non ',' : cursor = |" <<
            *cursor <<
            "|" <<
            endl << endl;
        }
        break;
      }

      ++cursor;

      if (debugMode) {
        gLogStream <<
          "--> mfDecipherNaturalNumbersSetSpecification after ',' : cursor = |" <<
          *cursor <<
          "|"
          << endl <<
          endl;
      }
    } // while

    if (* cursor != '\0') {
      gLogStream <<
        "--> Extraneous characters |" << *cursor <<
        "| in numbers spec" <<
        endl << endl;
    }
  }

  return result;
}

//______________________________________________________________________________
string consumeString (
  string::const_iterator  theStringIterator,
  string::const_iterator& remainingStringIterator,
  Bool                    debugMode)
{
  string result;

/* JMI
  string::const_iterator cursor = theStringIterator;

  while ((*cursor) != ',') {
    if (debugMode) {
      gLogStream <<
        "--> consumeString: cursor = |" <<
        *cursor <<
        "|" <<
        endl;
    }

    result += (*cursor);

    if (++cursor == theString.end ()) break;
  } // while

  remainingStringIterator = cursor;

  if (debugMode) {
    gLogStream <<
      "--> consumeString: result = " << result <<
      ", *remainingStringIterator = |" << *remainingStringIterator <<
      "|" <<
      endl;
  }
*/

  return result;
}

//______________________________________________________________________________
set<string> mfDecipherStringsSetSpecification (
  const string& theString,
  Bool          debugMode)
{
  // A integersSetSpecification sample is: "FOO,159,haLLo"

  set<string> result;

/* JMI
  if (debugMode) {
    gLogStream <<
      "--> mfDecipherStringsSetSpecification, theString = |" << theString <<
      "|" <<
      endl;
  }

  string::const_iterator
    cursor = theString.begin ();

  while (1) {
    if (debugMode) {
      gLogStream <<
        "--> mfDecipherStringsSetSpecification: cursor = |" <<
        *cursor << "|" <<
        endl;
    }

    string
      currentString =
        consumeString (cursor, theString.end (), debugMode);

    if (debugMode) {
      gLogStream <<
        "--> mfDecipherStringsSetSpecification" <<
        ", currentString = " << currentString <<
        ": *cursor = |" << *cursor << "|" <<
        endl;
    }

    result.insert (currentString);

    if (*cursor != ',') {
      if (debugMode) {
        gLogStream <<
          "--> mfDecipherStringsSetSpecification, after non ',' : cursor = |" <<
          *cursor <<
          "|" <<
          endl << endl;
      }
      break;
    }

    if (++cursor == theString.end ()) break;

    if (debugMode) {
      gLogStream <<
        "--> mfDecipherStringsSetSpecification after ',' : cursor = |" <<
        *cursor <<
        "|"
        << endl <<
        endl;
    }
  } // while

  if (* cursor != '\0') {
    gLogStream <<
      "--> Extraneous characters |" << *cursor <<
      "| in numbers spec" <<
      endl << endl;
  }
  */

  return result;
}

//______________________________________________________________________________
list<int> mfExtractNumbersFromString (
  const string& theString, // can contain "1, 2, 17"
  Bool          debugMode)
{
  list<int> foundNumbers;

  if (debugMode) {
    gLogStream <<
      "--> mfExtractNumbersFromString, theString = |" << theString <<
      "|" <<
      endl;
  }

  if (theString.size ()) {
    string::const_iterator
      cursor = theString.begin ();

    while (1) {
      if (cursor == theString.end ())
        break;

      if (debugMode) {
        gLogStream <<
          "--> mfExtractNumbersFromString: cursor = |" <<
          *cursor << "|" <<
          endl;
      }

      if (isdigit (*cursor)) {
        // consume a decimal number
        int n = 0;
        while (isdigit (*cursor)) {
          n = n * 10 + (*cursor - '0');
          ++cursor;
        } // while

        // append the number to the list
        foundNumbers.push_back (n);
      }
      else {
        ++cursor;
      }
    } // while
  }

  return foundNumbers;
}


//______________________________________________________________________________
string& mfLeftTrim (string& s)
{
  function <int (int)>
    checkSpace =
      [] (int x) { return isspace (x); };

  s.erase (
    s.begin (),
    find_if (
      s.begin (),
      s.end (),
      not1 (checkSpace) // 'not1<std::function<int (int)>>' is deprecated
      )
    );

  return s;
}

string& mfRightTrim (string& s)
{
  function <int (int)>
    checkSpace =
      [] (int x) { return isspace (x); };

  s.erase (
    find_if (
      s.rbegin (),
      s.rend (),
      not1 (checkSpace) // 'not1<std::function<int (int)>>' is deprecated
      ).base(),
    s.end ()
    );

  return s;
}

//______________________________________________________________________________
pair<string, string> mfExtractNamesPairFromString (
  const string& theString, // can contain "P1 = Bassoon"
  char          separator,
  Bool          debugMode)
{
  string name1;
  string name2;

  if (debugMode) {
    gLogStream <<
      "--> mfExtractNamesPairFromString, theString = |" << theString <<
      "|" <<
      endl;
  }

  if (theString.size ()) {
    string::const_iterator
      cursor = theString.begin ();

    // fetch name1
    while (1) {
      if (cursor == theString.end ())
        break;

      if (debugMode) {
        gLogStream <<
          "--> mfExtractNamesPairFromString: cursor = |" <<
          *cursor << "|" <<
          endl;
      }

      if ((*cursor) == separator) {
        // found the separator
        break;
      }

      // append the character to name1
      name1 += *cursor;
      ++cursor;
    } // while

    name1 = mfTrimFromBothEnds (name1);
    if (! name1.size ()) {
      // found an empty name1
      gLogStream <<
        "### ERROR: the first name before the " << separator <<
        " separator is empty in '" << theString << "'" <<
        endl;
    }

    if (cursor == theString.end ())
      gLogStream <<
        "### ERROR: the " << separator <<
        " separator is missing in string '" <<
        theString << "'" <<
        endl;
    else
      // overtake the separator
      ++cursor;

    // fetch name2
    while (1) {
      if (cursor == theString.end ())
        break;

      if (debugMode) {
        gLogStream <<
          "--> mfExtractNamesPairFromString: cursor = |" <<
          *cursor << "|" <<
          endl;
      }

      if ((*cursor) == '=') {
        // found the separator
        gLogStream <<
          "### ERROR: the " << separator <<
          " separator occurs more than once in string '" <<
          theString << "'" <<
          endl;
        break;
      }

      // append the character to name2
      name2 += *cursor;
      ++cursor;
    } // while

    name2 = mfTrimFromBothEnds (name2);
    if (! name2.size ()) {
      // found an empty name2
      gLogStream <<
        "### ERROR: the second name after the " << separator <<
        " separator is empty in '" << theString << "'" <<
        endl;
    }
  }

  return make_pair (name1, name2);
}

//______________________________________________________________________________
string mfDoubleQuoteStringIfNonAlpha (
  const string& theString)
{
  string result;

  Bool   stringShouldBeDoubleQuoted (false);

  if (theString.size ()) {
    for (
      string::const_iterator i = theString.begin ();
      i != theString.end ();
      ++i
    ) {

      if (
        ((*i) >= 'a' && (*i) <= 'z')
          ||
        ((*i) >= 'A' && (*i) <= 'Z')) {
        // (*i) is a letter
        result += (*i);
      }

      else {
        // (*i) is not a letter
        if ((*i) == ' ')
          result += ' '; // TEMP JMI
        else
          result += (*i);

        stringShouldBeDoubleQuoted = true;
      }
    } // for
  }

  if (stringShouldBeDoubleQuoted) {
    return "\"" + result + "\"";
  }
  else {
    return result;
  }
}

//______________________________________________________________________________
string mfQuoteStringIfNonAlpha (
  const string& theString)
{
  string result;

  Bool   stringShouldBeQuoted (false);

  if (theString.size ()) {
    for (
      string::const_iterator i = theString.begin ();
      i != theString.end ();
      ++i
    ) {

      if (
        ((*i) >= 'a' && (*i) <= 'z')
          ||
        ((*i) >= 'A' && (*i) <= 'Z')) {
        // (*i) is a letter
        result += (*i);
      }

      else {
        // (*i) is not a letter
        if ((*i) == ' ')
          result += ' '; // TEMP JMI
        else
          result += (*i);

        stringShouldBeQuoted = true;
      }
    } // for
  }

  if (stringShouldBeQuoted) {
    return "'" + result + "'";
  }
  else {
    return result;
  }
}

//______________________________________________________________________________
string mfDoubleQuoteString (
  const string& theString)
{
  string result;

  if (theString.size ()) {
    for (char const &c : theString) {
      if (
        ((c >= 'a') && (c <= 'z'))
          ||
        ((c >= 'A') && (c <= 'Z'))
      ) {
        // c is a letter
        result += c;
      }

      else {
        // c is not a letter
        if (c == ' ') {
          result += ' '; // TEMP JMI
        }
        else if (c == '"') {
          result += "\\\"";
        }
        else {
          result += c;
        }
      }
    } // for
  }

  return "\"" + result + "\"";
}

//______________________________________________________________________________
string mfQuoteString (
  const string& theString)
{
  string result;

  if (theString.size ()) {
    for (char const &c : theString) {
      if (
        ((c >= 'a') && (c <= 'z'))
          ||
        ((c >= 'A') && (c <= 'Z'))
      ) {
        // c is a letter
        result += c;
      }

      else {
        // c is not a letter
        if (c == ' ') {
          result += ' '; // TEMP JMI
        }
        else if (c == '\'') {
          result += "\\\'";
        }
        else {
          result += c;
        }
      }
    } // for
  }

  return "'" + result + "'";
}

//______________________________________________________________________________
string mfStringToLowerCase (
  const string& theString)
{
  string result = theString;

  for_each (
    result.begin(),
    result.end(),
    [] (char & c) {
      c = ::tolower (c);
      }
    );

  return result;
}

string mfStringToUpperCase (
  const string& theString)
{
  string result = theString;

  for_each (
    result.begin(),
    result.end(),
    [] (char & c) {
      c = ::toupper (c);
      }
    );

  return result;
}

//______________________________________________________________________________
void mfTrimString (
  string&     theString,
  const char* toBeRemoved)
{
  theString.erase (
    0,
    theString.find_first_not_of (toBeRemoved));

  theString.erase (
    theString.find_last_not_of (toBeRemoved) + 1);
}

string trimSpaces (const string& line) // KEEP ??? JMI
{
  const char* WhiteSpace = " \t\v\r\n";
  size_t start = line.find_first_not_of(WhiteSpace);
  size_t end = line.find_last_not_of(WhiteSpace);
  return start == end ? string() : line.substr(start, end - start + 1);
}

//______________________________________________________________________________
string mfBooleanAsString (Bool value)
{
  return
    string (
      value
        ? "true"
        : "false");
}

//______________________________________________________________________________
string mfSingularOrPlural (
  int number, string singularName, string pluralName)
{
  stringstream s;

  s <<
    number << ' ';

  if (number <= 1) {
    s <<
      singularName;
  }
  else {
    s <<
      pluralName;
  }

  return s.str ();
}

string mfSingularOrPluralWithoutNumber (
  int number, string singularName, string pluralName)
{
  stringstream s;

  if (number <= 1) {
    s <<
      singularName;
  }
  else {
    s <<
      pluralName;
  }

  return s.str ();
}

//______________________________________________________________________________
string mfEscapeDoubleQuotes (string s)
{
  string result;

  for_each (
    s.begin (),
    s.end (),
    mfStringQuoteEscaper (result));

  // replace occurrences of '\\"' by '\"',
  // in case there were already double quotes in string
  string
    lookedFor = "\\\\\"",
    ersatz    = "\\\"";

  for ( ; ; ) {
    size_t found = result.find (lookedFor);

    if (found == string::npos)
      break;

    result.replace (found, lookedFor.size (), ersatz);
  } // for

  return result;
}

//______________________________________________________________________________
void mfConvertHTMLEntitiesToPlainCharacters (string& s)
{
  map<string, string> conversionMap;

  conversionMap ["&"] = "&amp;";
  conversionMap ["\""] = "&quot;";
  conversionMap ["'"] = "&apos;";
  conversionMap ["<"] = "&lt;";
  conversionMap [">"] = "&gt;";

  map<string, string>::const_iterator i;

  for (i = conversionMap.begin (); i != conversionMap.end (); ++i) {
    string
      lookedFor = i->second,
      ersatz    = i->first;

    // replace all occurrences of lookedFor by ersatz
    for ( ; ; ) {
      size_t found = s.find (lookedFor);

      if (found == string::npos)
        break;

      s.replace (found, lookedFor.size (), ersatz);
    } // for

  } // for
}

//______________________________________________________________________________
void mfSplitStringIntoChunks (
  string        theString,
  string        theSeparator,
  list<string>& chunksList)
{
#ifdef DEBUG_SPLITTING
  gLogStream <<
    "---> splitting |" << theString << "|" <<
    endl << endl;
#endif

  unsigned int theStringSize = theString.size ();

  unsigned int currentPosition = 0;

#ifdef DEBUG_SPLITTING
  string remainder = theString;
#endif

  unsigned int theSeparatorSize = theSeparator.size ();

  map<string, string>::const_iterator i;

  while (1) {
    size_t found =
      theString.find (theSeparator, currentPosition);

    if (found == string::npos) {
      // fetch the last chunk
      // we have a last chunk
      // from currentPosition to theStringSize
      int chunkLength = theStringSize - currentPosition;

      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      chunksList.push_back (
        chunk);

#ifdef DEBUG_SPLITTING
      gLogStream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl << endl;
#endif

      break;
    }

    else {
      // we have a chunk from currentPosition to found
      int chunkLength = found - currentPosition;

      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      // append it to the chunks list
      chunksList.push_back (
        chunk);

      // advance the cursor
      currentPosition +=
        chunkLength + theSeparatorSize;

      // there can be an end of line JMI
      if (theString [currentPosition] == '\n')
        ++currentPosition;

#ifdef DEBUG_SPLITTING
      // set remainder
      remainder =
        theString.substr (
          currentPosition);

      gLogStream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "found = " << found <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl << endl;
#endif
    }
  } // while
}

//______________________________________________________________________________
void mfSplitRegularStringAtEndOfLines (
  string        theString,
  list<string>& chunksList)
{
#ifdef DEBUG_SPLITTING
  gLogStream <<
    "---> splitting |" << theString << "|" <<
    endl << endl;
#endif

  mfSplitStringIntoChunks (
    theString,
    "\n",
    chunksList);

    /* JMI
  unsigned int theStringSize = theString.size ();

  unsigned int currentPosition = 0;

#ifdef DEBUG_SPLITTING
  string remainder = theString;
#endif

  string lookedFor     = "\n";
  unsigned int    lookedForSize = lookedFor.size ();

  map<string, string>::const_iterator i;

  while (1) {
    size_t found =
      theString.find (lookedFor, currentPosition);

    if (found == string::npos) {
      // fetch the last chunk
      // we have a last chunk
      // from currentPosition to theStringSize
      int chunkLength = theStringSize - currentPosition;

      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      chunksList.push_back (
        chunk);

#ifdef DEBUG_SPLITTING
      gLogStream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl << endl;
#endif

      break;
    }

    else {
      // we have a chunk from currentPosition to found
      int chunkLength = found - currentPosition;

      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      // append it to the chunks list
      chunksList.push_back (
        chunk);

      // advance the cursor
      currentPosition +=
        chunkLength + lookedForSize;

      // there can be an end of line JMI
      if (theString [currentPosition] == '\n')
        ++currentPosition;

#ifdef DEBUG_SPLITTING
      // set remainder
      remainder =
        theString.substr (
          currentPosition);

      gLogStream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "found = " << found <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl << endl;
#endif
    }
  } // while
  */
}

//______________________________________________________________________________
void mfSplitHTMLStringContainingEndOfLines (
  string        theString,
  list<string>& chunksList)
{
#ifdef DEBUG_SPLITTING
  gLogStream <<
    "---> splitting |" << theString << "|" <<
    endl << endl;
#endif

  unsigned int theStringSize = theString.size ();

  map<string, string> conversionMap; // JMI

  conversionMap ["&"] = "&amp;";
  conversionMap ["\""] = "&quot;";
  conversionMap ["'"] = "&apos;";
  conversionMap ["<"] = "&lt;";
  conversionMap [">"] = "&gt;";

  size_t currentPosition = 0;

#ifdef DEBUG_SPLITTING
  string remainder = theString;
#endif

// JMI  string lookedFor     = "&#xd;";
  string       lookedFor     = "\n";
  unsigned int lookedForSize = lookedFor.size ();

  map<string, string>::const_iterator i;

/*
  for (i = conversionMap.begin (); i != conversionMap.end (); ++i) {
    string
      lookedFor = i->second,
      ersatz    = i->first;
    // replace all occurrences of lookedFor by ersatz
  } // for
*/

  while (1) {
    size_t found =
      theString.find (lookedFor, currentPosition);

    if (found == string::npos) {
      // fetch the last chunk
      // we have a last chunk
      // from currentPosition to theStringSize
      int chunkLength = theStringSize - currentPosition;

      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      chunksList.push_back (
        chunk);

#ifdef DEBUG_SPLITTING
      gLogStream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl << endl;
#endif

      break;
    }

    else {
      // we have a chunk from currentPosition to found
      int chunkLength = found - currentPosition;

      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      // append it to the chunks list
      chunksList.push_back (
        chunk);

      // advance the cursor
      currentPosition +=
        chunkLength + lookedForSize;

      // there can be an end of line JMI
      if (theString [currentPosition] == '\n')
        ++currentPosition;

#ifdef DEBUG_SPLITTING
      // set remainder
      remainder =
        theString.substr (
          currentPosition);

      gLogStream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "found = " << found <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl << endl;
#endif
    }
  } // while
}

//______________________________________________________________________________
string mfBaseName (const string& filename)
{
  if (! filename.size ()) {
      return {};
  }

  auto len   = filename.length ();
  auto index = filename.find_last_of ("/\\");

  if (index == string::npos) {
      return filename;
  }

  if (index + 1 >= len) {
    --len;
    index = filename.substr (0, len).find_last_of ("/\\");

    if (len == 0) {
      return filename;
    }

    if (index == 0) {
      return filename.substr (1, len - 1);
    }

    if (index == string::npos) {
      return filename.substr (0, len);
    }

    return filename.substr (index + 1, len - index - 1);
  }

  return filename.substr (index + 1, len - index);
}

//______________________________________________________________________________
string mfMakeSingleWordFromString (const string& theString)
{
  string result;

  if (theString.size ()) {
    for (
      string::const_iterator i = theString.begin ();
      i != theString.end ();
      ++i
    ) {
      if (isalnum (*i)) {
        result.push_back ((*i));
      }
    } // for
  }

  return result;
}

//______________________________________________________________________________
Bool mfStringIsInStringSet (
  const string&     theString,
  const set<string> stringSet)
{
  Bool result (false);

  if (stringSet.size ()) {
    set<string>::iterator
      it =
        stringSet.find (
          theString);

    if (it != stringSet.end ()) {
      result = true;
    }
  }

  return result;
}

string mfStringSetAsString (
  const set<string> stringSet)
{
  stringstream s;

  s << "[";

  // append the set elements if any
  unsigned int stringSetSize =
    stringSet.size ();

  if (stringSetSize) {
    set<string>::const_iterator
      iBegin = stringSet.begin (),
      iEnd   = stringSet.end (),
      i      = iBegin;

    unsigned int nextToLast =
      stringSetSize - 1;

    unsigned int count = 0;

    for ( ; ; ) {
      ++count;

      s << "\"" << (*i) << "\"";
      if (++i == iEnd) break;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != stringSetSize) {
        s << ", ";
      }
    } // for
  }

  s << "]";

  return s.str ();
}

void mfDisplayStringSet (
  const string&     title,
  const set<string> stringSet,
  ostream&          os)
{
  // print the title
  os << title << endl;

  // print the set elements if any
  os << title << ":";

  unsigned int stringSetSize =
    stringSet.size ();

  if (stringSetSize) {
    os << endl;

    ++gIndenter;

    set<string>::const_iterator
      iBegin = stringSet.begin (),
      iEnd   = stringSet.end (),
      i      = iBegin;

    unsigned int nextToLast =
      stringSetSize - 1;

    unsigned int count = 0;

    for ( ; ; ) {
      ++count;

      os << "\"" << (*i) << "\"";
      if (++i == iEnd) break;

      if (count == nextToLast) {
        os << " and ";
      }
      else if (count != stringSetSize) {
        os << ", ";
      }
    } // for

    --gIndenter;
  }
  else {
    os << " empty";
  }

  os << endl;
}


}

