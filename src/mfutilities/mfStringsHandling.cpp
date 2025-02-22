/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <string.h>     // strncat
#include <sstream>

#include "mfPreprocessorSettings.h"

#include <map>

#include "mfAssert.h"
#include "mfStringsHandling.h"
#include "mfIndentedTextOutput.h"

#include <algorithm>    // for_each, find_if


namespace MusicFormats
{

//______________________________________________________________________________
//#define DEBUG_SPLITTING

// //______________________________________________________________________________
// mfMultiLineString::mfMultiLineString ()
// {
//   fValue = false;
// }
//
// mfMultiLineString::mfMultiLineString (bool value)
// {
//   fValue = value;
// }
//
// mfMultiLineString::~mfMultiLineString ()
// {}
//
// mfMultiLineString::mfMultiLineString(const mfMultiLineString& themfMultiLineString) {
// //   std::cout << "mfMultiLineString copy constructor " << std::endl; // JMI CLAR
//
//    // copy the value
//    fValue = themfMultiLineString.getValue ();
// }
//
// /*
//   // Declare prefix and postfix decrement operators.
//   Point& operator--();       // Prefix decrement operator.
//   Point operator--(int);     // Postfix decrement operator.
// */
//
// mfMultiLineString::mfMultiLineString (const std::string& theString)
// {
//   if (theString == "false") {
//     fValue = false;
//   }
//   else if (theString == "true") {
//     fValue = true;
//   }
//   else {
//   }
// }
//
// mfMultiLineString& mfMultiLineString::operator= (const mfMultiLineString& themfMultiLineString)
// {
//   if (this != &themfMultiLineString) { // JMI costly?
//     fValue = themfMultiLineString.getValue ();
//   }
//
//   return *this;
// }
//
// mfMultiLineString mfMultiLineString::operator! () const
// {
//   mfMultiLineString result (! fValue);
//
//   return result;
// }
//
// mfMultiLineString mfMultiLineString::operator&&(const mfMultiLineString& othermfMultiLineString) const
// {
//   mfMultiLineString result (fValue && othermfMultiLineString.fValue);
//
//   return result;
// }
//
// mfMultiLineString mfMultiLineString::operator||(const mfMultiLineString& othermfMultiLineString) const
// {
//   mfMultiLineString result (fValue || othermfMultiLineString.fValue);
//
//   return result;
// }
//
// EXP mfMultiLineString operator&& (const mfMultiLineString& leftmfMultiLineString, const bool& themfMultiLineString)
// {
//   mfMultiLineString result (leftmfMultiLineString.getValue () && themfMultiLineString);
//
//   return result;
// }
//
// EXP mfMultiLineString operator&& (const bool& themfMultiLineString, const mfMultiLineString& rightmfMultiLineString)
// {
//   mfMultiLineString result (themfMultiLineString && rightmfMultiLineString.getValue ());
//
//   return result;
// }
//
// EXP mfMultiLineString operator|| (const mfMultiLineString& leftmfMultiLineString, const bool& themfMultiLineString)
// {
//
//   mfMultiLineString result (leftmfMultiLineString.getValue () || themfMultiLineString);
//
//   return result;
// }
//
// EXP mfMultiLineString operator|| (const bool& themfMultiLineString, const mfMultiLineString& rightmfMultiLineString)
// {
//   mfMultiLineString result (themfMultiLineString || rightmfMultiLineString.getValue ());
//
//   return result;
// }
//
// std::string mfMultiLineString::asString () const
// {
//   return
//     fValue
//       ? "true"
//       : "false";
// }
//
// void mfMultiLineString::print (std::ostream& os) const
// {
//   os << asString ();
// }
//
// std::ostream& operator << (std::ostream& os, const mfMultiLineString& theBool) {
//   theBool.print (os);
//   return os;
// }

//______________________________________________________________________________
std::string mfReplicateChar (
  char theChar,
  int  times)
{
  std::string result;

  for (int i = 0; i < times; ++i)
    result += theChar;

  return result;
}

std::string mfReplicateString (
  std::string theString,
  int         times)
{
  std::string result;

  for (int i = 0; i < times; ++i)
    result += theString;

  return result;
}

//______________________________________________________________________________
/* JMI
std::string replaceSubstringInString (
  std::string theString,
  std::string subString,
  std::string ersatz)
{
  std::string result = theString;

  size_t found = result.find (subString);

  if (found != std::string::npos) {
    result.replace (found, subString.size (), ersatz);
  }

  return result;
}
*/

//_______________________________________________________________________________
EXP void mfDisplayArgcAndArgv (
  int           argc,
  char*         argv[],
  std::ostream& os)
{
  os <<
    "The contents of argv is:" <<
    std::endl;

  ++gIndenter;

  for (int i = 0; i < argc; ++i) {
    os <<
      "argv [" <<
      std::right << std::setw (2) <<
      i <<
      "]: " << argv [i] <<
      std::endl;
  } // for

  --gIndenter;
}

//______________________________________________________________________________
void mfDisplayStringsVector (
  const mfStringsVector& stringsVector,
  const std::string&     title,
  std::ostream&          os)
{
  os <<
    title <<
    " contains " <<
    mfSingularOrPlural (
      stringsVector.size (), "element", "elements");

  if (stringsVector.size ()) {
    os << ":" << std::endl;

    ++gIndenter;

    int counter = 0;
    for (std::string theString : stringsVector) {
      os <<
        std::right << std::setw (2) << counter++ <<
        ": \"" <<
        theString <<
        "\"" <<
        std::endl;
    } //for

    --gIndenter;
  }
  else {
    os << std::endl;
  }
}

//______________________________________________________________________________
char* mfCharStarCat (
  char*        destination,
  const char*  source,
  const size_t destinationSize)
{
  Bool
    destinationOverflow =
      strlen (source) >= destinationSize - strlen (destination) - 1;

  if (destinationOverflow) {
    std::stringstream ss;

    ss <<
      "mfCharStarCat(): attempt at destination overflow" <<
      ", destinationSize: [" << destinationSize <<
      "], destination: [" << destination <<
      "], strlen (source): [" << strlen (source) <<
      "], source: [" << source <<
      "], destinationSize - strlen (destination) - 1: [" << destinationSize - strlen (destination) - 1 <<
      ']';

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
    mfAssert (
      __FILE__, __LINE__,
      false,
      ss.str ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED
  }

  return
    strncat (
      destination,
      source,
      destinationSize - strlen (destination) - 1);
}

//______________________________________________________________________________
std::string mfIntToEnglishWord (int n)
{
  if (n == -9) {
    assert (false); // JMI
  }

  std::stringstream ss;

  if (n < 0) {
    ss << "Minus_";
    n = -n;
  }

  if (n >= 1000) {
    int nDiv1000    = n / 1000;
    int nModulo1000 = n % 1000;

    return
      mfIntToEnglishWord (nDiv1000) +
      "Thousand" +
      mfIntToEnglishWord (nModulo1000);
  }

  else if (n >= 100) {
  //return "LargerThanNinetyNine";
    int nDiv100    = n / 100;
    int nModulo100 = n % 100;

    return
      mfIntToEnglishWord (nDiv100) +
      "HundredAnd" +
      mfIntToEnglishWord (nModulo100);
  }

  else {
    switch (n) {
      case 0:
        ss << "Zero";
        break;
      case 1:
        ss << "One";
        break;
      case 2:
        ss << "Two";
        break;
      case 3:
        ss << "Three";
        break;
      case 4:
        ss << "Four";
        break;
      case 5:
        ss << "Five";
        break;
      case 6:
        ss << "Six";
        break;
      case 7:
        ss << "Seven";
        break;
      case 8:
        ss << "Eight";
        break;
      case 9:
        ss << "Nine";
        break;
     case 10:
        ss << "Ten";
        break;
      case 11:
        ss << "Eleven";
        break;
      case 12:
        ss << "Twelve";
        break;
      case 13:
        ss << "Thirteen";
        break;
      case 14:
        ss << "Fourteen";
        break;
      case 15:
        ss << "Fifteen";
        break;
      case 16:
        ss << "Sixteen";
        break;
      case 17:
        ss << "Seventeen";
        break;
      case 18:
        ss << "Eighteen";
        break;
      case 19:
        ss << "Nineteen";
        break;

      default: {
        // n >= 20
        int nDiv10    = n / 10;
        int nModulo10 = n % 10;

        switch (nDiv10) {
          case 2:
            ss << "Twenty";
            break;
          case 3:
            ss << "Thirty";
            break;
          case 4:
            ss << "Fourty";
            break;
          case 5:
            ss << "Fifty";
            break;
          case 6:
            ss << "Sixty";
            break;
          case 7:
            ss << "Seventy";
            break;
          case 8:
            ss << "Eighty";
            break;
          case 9:
            ss << "Ninety";
            break;
        } // switch
        ss << mfIntToEnglishWord (nModulo10);
      } // default
    } // switch
  }

  return ss.str ();
}

//______________________________________________________________________________
std::string mfStringNumbersToEnglishWords (std::string theString)
{
  if (! theString.size ()) {
    return "NONE";
  }

  enum workState {
    kInitialState, kWorkingOnDigits, kWorkingOnNonDigits };

  std::vector <std::string> chunks;
  std::vector <int>         states;

  workState state = kInitialState;

  for (char theChar : theString) {
    if( isdigit (theChar)) {
      // digit
      if (state != kWorkingOnDigits) {
        // create a new chunck for digits
        chunks.push_back ("");
        states.push_back (kWorkingOnDigits);
        state = kWorkingOnDigits;
      }
      chunks.back().push_back (theChar);
    }
    else {
      // non digit
      if (state != kWorkingOnNonDigits) {
        // create a new chunck for non digits
        chunks.push_back ("");
        states.push_back (kWorkingOnNonDigits);
        state = kWorkingOnNonDigits;
      }
      chunks.back().push_back (theChar);
    }
  } // for

  std::string result = "";

  for (size_t i = 0; i < chunks.size (); ++i) {
    if (states[i] == kWorkingOnDigits) {
      int integerValue;

      std::istringstream inputStream (chunks[i]);

      inputStream >> integerValue;

      result += mfIntToEnglishWord (integerValue);
    }
    else {
      result += chunks[i];
    }
  } // for

//   gLog << JMI v0.9.68
//     "mfStringNumbersToEnglishWords(): result = " << result <<
//     std::endl;

  return result;
};

//______________________________________________________________________________
// std::string mfInputLineNumberAsString (
//   mfInputLineNumber inputLineNumber)
// {
//   std::stringstream ss;
//
//   ss <<
//     "input line L" << inputLineNumber;
//
//   return ss.str ();
// }

std::string mfInputLineNumberAsString (
  int inputLineNumber)
{
  std::stringstream ss;

  ss <<
    "input line L" << inputLineNumber;

  return ss.str ();
}

// std::string mfInputLineNumbersAsString (
//   int inputStartLineNumber,
//   int inputLineNumber)
// {
//   std::stringstream ss;
//
//   if (inputLineNumber == inputStartLineNumber) {
//     ss <<
//       "input line L" << inputStartLineNumber;
//   }
//
//   else {
//     if (inputLineNumber == 0) {
//       ss <<
//         "input line " << inputStartLineNumber;
//     }
//     else {
//       ss <<
//         "input lines L" << inputStartLineNumber << '-' << inputLineNumber;
//     }
//   }
//
//   return ss.str ();
// }

//______________________________________________________________________________
int consumeDecimalNumber (
  std::string::const_iterator  theStringIterator,
  std::string::const_iterator& remainingStringIterator,
  Bool                         debugMode)
{
  std::string::const_iterator cursor = theStringIterator;
  int    result = 0;

  if (! isdigit (*cursor)) {
    gLog <<
      "consumeDecimalNumber (" << *cursor <<
      "), " << *cursor << " is no decimal digit!" <<
      std::endl;
    }

  while (isdigit (*cursor)) {
    if (debugMode) {
      gLog <<
        "--> consumeDecimalNumber: cursor = |" <<
        *cursor <<
        "|" <<
        std::endl;
    }

    result = result*10 + (*cursor-'0');

    ++cursor;
  } // while

  remainingStringIterator = cursor;

  if (debugMode) {
    gLog <<
      "--> consumeDecimalNumber: result: " << result <<
      ", *remainingStringIterator = |" << *remainingStringIterator <<
      "|" <<
      std::endl;
  }

  return result;
}

//______________________________________________________________________________
std::set <int> mfDecipherNaturalNumbersSetSpecification (
  const std::string& theString,
  Bool               debugMode)
{
  // A naturalNumbersSetSpecification sample is: "7,15-19,^16-17"

  std::set <int> result;

  if (debugMode) {
    gLog <<
      "--> mfDecipherNaturalNumbersSetSpecification, theString = |" << theString <<
      "|" <<
      std::endl;
  }

  if (theString.size ()) {
    std::string::const_iterator
      cursor = theString.begin ();

    while (1) {
      if (debugMode) {
        gLog <<
          "--> mfDecipherNaturalNumbersSetSpecification: cursor = |" <<
          *cursor << "|" <<
          std::endl;
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
          gLog <<
            "--> mfDecipherNaturalNumbersSetSpecification after '-' : cursor = |" <<
            *cursor <<
            "|" <<
            std::endl << std::endl;
        }

        intervalEndNumber =
          consumeDecimalNumber (cursor, cursor, debugMode);
      }

      else {
        intervalEndNumber = intervalStartNumber;
      }

      if (debugMode) {
        gLog <<
          "--> mfDecipherNaturalNumbersSetSpecification" <<
          ", intervalStartNumber: " << intervalStartNumber <<
          ", intervalEndNumber: " << intervalEndNumber <<
          ": *cursor = |" << *cursor << "|" <<
          std::endl;
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
          gLog <<
            "--> mfDecipherNaturalNumbersSetSpecification, after non ',' : cursor = |" <<
            *cursor <<
            "|" <<
            std::endl << std::endl;
        }
        break;
      }

      ++cursor;

      if (debugMode) {
        gLog <<
          "--> mfDecipherNaturalNumbersSetSpecification after ',' : cursor = |" <<
          *cursor <<
          "|"
          << std::endl <<
          std::endl;
      }
    } // while

    if (* cursor != '\0') {
      gLog <<
        "--> Extraneous characters |" << *cursor <<
        "| in numbers spec" <<
        std::endl << std::endl;
    }
  }

  return result;
}

//______________________________________________________________________________
std::string consumeString (
  std::string::const_iterator  theStringIterator,
  std::string::const_iterator& remainingStringIterator,
  Bool                         debugMode)
{
  std::string result;

/* JMI v0.9.66 ???
  std::string::const_iterator cursor = theStringIterator;

  while ((*cursor) != ',') {
    if (debugMode) {
      gLog <<
        "--> consumeString: cursor = |" <<
        *cursor <<
        "|" <<
        std::endl;
    }

    result += (*cursor);

    if (++cursor == theString.end ()) break;
  } // while

  remainingStringIterator = cursor;

  if (debugMode) {
    gLog <<
      "--> consumeString: result: " << result <<
      ", *remainingStringIterator = |" << *remainingStringIterator <<
      "|" <<
      std::endl;
  }
*/

  return result;
}

//______________________________________________________________________________
std::set <std::string> mfDecipherStringsSetSpecification (
  const std::string& theString,
  Bool               debugMode)
{
  // A integersSetSpecification sample is: "FOO,159,haLLo"

  std::set <std::string> result;

/* JMI v0.9.66 ???
  if (debugMode) {
    gLog <<
      "--> mfDecipherStringsSetSpecification, theString = |" << theString <<
      "|" <<
      std::endl;
  }

  std::string::const_iterator
    cursor = theString.begin ();

  while (1) {
    if (debugMode) {
      gLog <<
        "--> mfDecipherStringsSetSpecification: cursor = |" <<
        *cursor << "|" <<
        std::endl;
    }

    std::string
      currentString =
        consumeString (cursor, theString.end (), debugMode);

    if (debugMode) {
      gLog <<
        "--> mfDecipherStringsSetSpecification" <<
        ", currentString: " << currentString <<
        ": *cursor = |" << *cursor << "|" <<
        std::endl;
    }

    result.insert (currentString);

    if (*cursor != ',') {
      if (debugMode) {
        gLog <<
          "--> mfDecipherStringsSetSpecification, after non ',' : cursor = |" <<
          *cursor <<
          "|" <<
          std::endl << std::endl;
      }
      break;
    }

    if (++cursor == theString.end ()) break;

    if (debugMode) {
      gLog <<
        "--> mfDecipherStringsSetSpecification after ',' : cursor = |" <<
        *cursor <<
        "|"
        << std::endl <<
        std::endl;
    }
  } // while

  if (* cursor != '\0') {
    gLog <<
      "--> Extraneous characters |" << *cursor <<
      "| in numbers spec" <<
      std::endl << std::endl;
  }
  */

  return result;
}

//______________________________________________________________________________
std::list <int> mfExtractNumbersFromString (
  const std::string& theString, // can contain "1, 2, 17"
  Bool               debugMode)
{
  std::list <int> foundNumbers;

  if (debugMode) {
    gLog <<
      "--> mfExtractNumbersFromString, theString = |" << theString <<
      "|" <<
      std::endl;
  }

  if (theString.size ()) {
    std::string::const_iterator
      cursor = theString.begin ();

    while (1) {
      if (cursor == theString.end ())
        break;

      if (debugMode) {
        gLog <<
          "--> mfExtractNumbersFromString: cursor = |" <<
          *cursor << "|" <<
          std::endl;
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
std::string& mfLeftTrim (std::string& s)
{
  std::function <int (int)>
    checkSpace = // lambda expression
      [] (int x) { return isspace (x); };

  s.erase (
    s.begin (),
    find_if (
      s.begin (),
      s.end (),
      std::not_fn (checkSpace) // since C++17
      )
    );

  return s;
}

std::string& mfRightTrim (std::string& s)
{
  std::function <int (int)>
    checkSpace = // lambda expression
      [] (int x) { return isspace (x); };

  s.erase (
    find_if (
      s.rbegin (),
      s.rend (),
      std::not_fn (checkSpace) // since C++17
      ).base(),
    s.end ()
    );

  return s;
}

//______________________________________________________________________________
std::pair <std::string, std::string> mfExtractNamesPairFromString (
  const std::string& theString, // can contain "P1 = Bassoon"
  char               separator,
  Bool               debugMode)
{
  std::string name1;
  std::string name2;

  if (debugMode) {
    gLog <<
      "--> mfExtractNamesPairFromString, theString = |" << theString <<
      "|" <<
      std::endl;
  }

  if (theString.size ()) {
    std::string::const_iterator
      cursor = theString.begin ();

    // fetch name1
    while (1) {
      if (cursor == theString.end ())
        break;

      if (debugMode) {
        gLog <<
          "--> mfExtractNamesPairFromString: cursor = |" <<
          *cursor << "|" <<
          std::endl;
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
      gLog <<
        "### ERROR: the first name before the " << separator <<
        " separator is empty in '" << theString << '\'' <<
        std::endl;
    }

    if (cursor == theString.end ())
      gLog <<
        "### ERROR: the " << separator <<
        " separator is missing in std::string '" <<
        theString << '\'' <<
        std::endl;
    else
      // overtake the separator
      ++cursor;

    // fetch name2
    while (1) {
      if (cursor == theString.end ())
        break;

      if (debugMode) {
        gLog <<
          "--> mfExtractNamesPairFromString: cursor = |" <<
          *cursor << "|" <<
          std::endl;
      }

      if ((*cursor) == '=') {
        // found the separator
        gLog <<
          "### ERROR: the " << separator <<
          " separator occurs more than once in std::string '" <<
          theString << '\'' <<
          std::endl;
        break;
      }

      // append the character to name2
      name2 += *cursor;
      ++cursor;
    } // while

    name2 = mfTrimFromBothEnds (name2);
    if (! name2.size ()) {
      // found an empty name2
      gLog <<
        "### ERROR: the second name after the " << separator <<
        " separator is empty in '" << theString << '\'' <<
        std::endl;
    }
  }

  return std::make_pair (name1, name2);
}

//______________________________________________________________________________
std::string mfDoubleQuoteStringIfNonAlpha (
  const std::string& theString)
{
  std::string result;

  Bool stringShouldBeDoubleQuoted (false);

  if (theString.size ()) {
    Bool stringContainsNotPrintableCharacters (false);

    for (char theChar : theString) {
      // is theChar printable?
      if (! isprint (theChar)) {
        stringShouldBeDoubleQuoted = true;
      }

      // append theChar to result // JMI v0.9.70
      result += theChar;

//       if (
//         (theChar >= 'a' && theChar <= 'z')
//           ||
//         (theChar >= 'A' && theChar <= 'Z')
//       ) {
//         // theChar is a letter
//         result += theChar;
//       }
//
//       else {
//         // theChar is not a letter
//         if (theChar == ' ')
//           result += ' '; // TEMP JMI
//         else if (theChar == '\t')
//           result += ' '; // TEMP JMI
//         else
//           result += theChar;
//
//         stringShouldBeDoubleQuoted = true;
//       }
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
std::string mfQuoteStringIfNonAlpha (
  const std::string& theString)
{
  std::string result;

  Bool stringShouldBeQuoted (false);

  if (theString.size ()) {
    for (char theChar : theString) {
      if (
        (theChar >= 'a' && theChar <= 'z')
          ||
        (theChar >= 'A' && theChar <= 'Z')
      ) {
        // theChar is a letter
        result += theChar;
      }

      else {
        // theChar is not a letter
        if (theChar == ' ')
          result += ' '; // TEMP JMI
        else
          result += theChar;

        stringShouldBeQuoted = true;
      }
    } // for
  }

  if (stringShouldBeQuoted) {
    return "'" + result + '\'';
  }
  else {
    return result;
  }
}

//______________________________________________________________________________
std::string mfDoubleQuoteStringContentsOnly (
  const std::string& theString)
{
  std::string result;

  if (theString.size ()) {
    for (char theChar : theString) {
      if (theChar == '"') {
        result += "\\\"";
      }
      else {
        result += theChar;
      }
    } // for
  }

  return result;

//   std::ostringstream ss;
//
//   ss << std::quoted (theString);
//
//   return ss.str();
}

//______________________________________________________________________________
std::string mfDoubleQuoteString (
  const std::string& theString)
{
  return "\"" + mfDoubleQuoteStringContentsOnly (theString) + "\"";
}

//______________________________________________________________________________
std::string mfDoubleQuoteStringIfNonWord (
  const std::string& theString)
{
  std::string result;

  Bool stringShouldBeDoubleQuoted (false);

  if (theString.size ()) {
    for (char theChar : theString) {
      if (! std::isalpha (theChar)) {
        stringShouldBeDoubleQuoted = true;
      }

      if (theChar == '"') {
        result += "\\\"";
      }
      else {
        result += theChar;
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
std::string mfSingleQuoteStringContentsOnly (
  const std::string& theString)
{
  std::string result;

  if (theString.size ()) {
    for (char theChar : theString) {
      if (theChar == '\'') {
        result += "\\\'";
      }
      else {
        result += theChar;
      }
    } // for
  }

  return result;
}

std::string mfSingleQuoteString (
  const std::string& theString)
{
  return "'" + mfSingleQuoteStringContentsOnly (theString) + '\'';
}

//______________________________________________________________________________
std::string mfStringToLowerCase (
  const std::string& theString)
{
  std::string result = theString;

  for_each (
    result.begin(),
    result.end(),
    [] (char& theChar)
      {
        theChar = ::tolower (theChar);
      }
  ); // for_each

  return result;
}

std::string mfStringToUpperCase (
  const std::string& theString)
{
  std::string result = theString;

  for_each (
    result.begin(),
    result.end(),
    [] (char& theChar)
      {
        theChar = ::toupper (theChar);
      }
  ); // for_each

  return result;
}

//______________________________________________________________________________
void mfTrimString (
  std::string& theString,
  const char*  toBeRemoved)
{
  theString.erase (
    0,
    theString.find_first_not_of (toBeRemoved));

  theString.erase (
    theString.find_last_not_of (toBeRemoved) + 1);
}

std::string trimSpaces (const std::string& line) // KEEP ??? JMI
{
  const char* WhiteSpace = " \t\v\r\n";

  size_t start = line.find_first_not_of(WhiteSpace);
  size_t end   = line.find_last_not_of(WhiteSpace);

  return
    start == end ? std::string() : line.substr(start, end - start + 1);
}

//______________________________________________________________________________
std::string mfBooleanAsString (Bool value)
{
  return
    std::string (
      value
        ? "true"
        : "false");
}

//______________________________________________________________________________
std::string mfSingularOrPlural (
  int         number,
  std::string singularName,
  std::string pluralName)
{
  std::stringstream ss;

  ss <<
    number << ' ';

  if (number <= 1) {
    ss <<
      singularName;
  }
  else {
    ss <<
      pluralName;
  }

  return ss.str ();
}

std::string mfSingularOrPluralWithoutNumber (
  int         number,
  std::string singularName,
  std::string pluralName)
{
  std::stringstream ss;

  if (number <= 1) {
    ss <<
      singularName;
  }
  else {
    ss <<
      pluralName;
  }

  return ss.str ();
}

//______________________________________________________________________________
std::string mfEscapeDoubleQuotes (std::string s)
{
  std::string result;

  for_each (
    s.begin (),
    s.end (),
    mfStringQuoteEscaper (result));

  // replace occurrences of '\\"' by '\"',
  // in case there were already double quotes in std::string
  std::string
    lookedFor = "\\\\\"",
    ersatz    = "\\\"";

  for ( ; ; ) {
    size_t found = result.find (lookedFor);

    if (found == std::string::npos)
      break;

    result.replace (found, lookedFor.size (), ersatz);
  } // for

  return result;
}

//______________________________________________________________________________
void mfConvertHTMLEntitiesToPlainCharacters (std::string& s)
{
  std::map <std::string, std::string> conversionMap;

  conversionMap ["&"] = "&amp;";
  conversionMap ["\""] = "&quot;";
  conversionMap ["'"] = "&apos;";
  conversionMap ["<"] = "&lt;";
  conversionMap [">"] = "&gt;";

  std::map <std::string, std::string>::const_iterator i;

  for (i = conversionMap.begin (); i != conversionMap.end (); ++i) {
    std::string
      lookedFor = i->second,
      ersatz    = i->first;

    // replace all occurrences of lookedFor by ersatz
    for ( ; ; ) {
      size_t found = s.find (lookedFor);

      if (found == std::string::npos)
        break;

      s.replace (found, lookedFor.size (), ersatz);
    } // for

  } // for
}

//______________________________________________________________________________
void mfSplitStringIntoChunks (
  std::string             theString,
  std::string             theSeparator,
  std::list <std::string>& chunksList)
{
#ifdef DEBUG_SPLITTING
  gLog <<
    "---> splitting |" << theString << "|" <<
    std::endl << std::endl;
#endif // DEBUG_SPLITTING

  size_t theStringSize = theString.size ();

  size_t currentPosition = 0;

#ifdef DEBUG_SPLITTING
  std::string remainder = theString;
#endif // DEBUG_SPLITTING

  size_t theSeparatorSize = theSeparator.size ();

  std::map <std::string, std::string>::const_iterator i;

  while (1) {
    size_t found =
      theString.find (theSeparator, currentPosition);

    if (found == std::string::npos) {
      // fetch the last chunk
      // we have a last chunk
      // from currentPosition to theStringSize
      int chunkLength = theStringSize - currentPosition;

      std::string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      chunksList.push_back (
        chunk);

#ifdef DEBUG_SPLITTING
      gLog <<
        "theStringSize: " << theStringSize <<
        std::endl <<
        "currentPosition: " << currentPosition <<
        std::endl <<
        "remainder = |" << remainder << "|" <<
        std::endl <<
        "chunkLength: " << chunkLength <<
        std::endl <<
        "chunk = \"" << chunk << "\"" <<
        std::endl << std::endl;
#endif // DEBUG_SPLITTING

      break;
    }

    else {
      // we have a chunk from currentPosition to found
      int chunkLength = found - currentPosition;

      std::string
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

      gLog <<
        "theStringSize: " << theStringSize <<
        std::endl <<
        "currentPosition: " << currentPosition <<
        std::endl <<
        "remainder = |" << remainder << "|" <<
        std::endl <<
        "found: " << found <<
        std::endl <<
        "chunkLength: " << chunkLength <<
        std::endl <<
        "chunk = \"" << chunk << "\"" <<
        std::endl << std::endl;
#endif // DEBUG_SPLITTING
    }
  } // while
}

//______________________________________________________________________________
void mfSplitRegularStringAtEndOfLines (
  std::string             theString,
  std::list <std::string>& chunksList)
{
#ifdef DEBUG_SPLITTING
  gLog <<
    "---> splitting |" << theString << "|" <<
    std::endl << std::endl;
#endif // DEBUG_SPLITTING

  mfSplitStringIntoChunks (
    theString,
    "\n",
    chunksList);

    /* JMI
  size_t theStringSize = theString.size ();

  size_t currentPosition = 0;

#ifdef DEBUG_SPLITTING
  std::string remainder = theString;
#endif // DEBUG_SPLITTING

  std::string lookedFor     = "\n";
  size_t    lookedForSize = lookedFor.size ();

  std::map <std::string, std::string>::const_iterator i;

  while (1) {
    size_t found =
      theString.find (lookedFor, currentPosition);

    if (found == std::string::npos) {
      // fetch the last chunk
      // we have a last chunk
      // from currentPosition to theStringSize
      int chunkLength = theStringSize - currentPosition;

      std::string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      chunksList.push_back (
        chunk);

#ifdef DEBUG_SPLITTING
      gLog <<
        "theStringSize: " << theStringSize <<
        std::endl <<
        "currentPosition: " << currentPosition <<
        std::endl <<
        "remainder = |" << remainder << "|" <<
        std::endl <<
        "chunkLength: " << chunkLength <<
        std::endl <<
        "chunk = \"" << chunk << "\"" <<
        std::endl << std::endl;
#endif // DEBUG_SPLITTING

      break;
    }

    else {
      // we have a chunk from currentPosition to found
      int chunkLength = found - currentPosition;

      std::string
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

      gLog <<
        "theStringSize: " << theStringSize <<
        std::endl <<
        "currentPosition: " << currentPosition <<
        std::endl <<
        "remainder = |" << remainder << "|" <<
        std::endl <<
        "found: " << found <<
        std::endl <<
        "chunkLength: " << chunkLength <<
        std::endl <<
        "chunk = \"" << chunk << "\"" <<
        std::endl << std::endl;
#endif // DEBUG_SPLITTING
    }
  } // while
  */
}

//______________________________________________________________________________
void mfSplitHTMLStringContainingEndOfLines (
  std::string             theString,
  std::list <std::string>& chunksList)
{
#ifdef DEBUG_SPLITTING
  gLog <<
    "---> splitting |" << theString << "|" <<
    std::endl << std::endl;
#endif // DEBUG_SPLITTING

  size_t theStringSize = theString.size ();

  std::map <std::string, std::string> conversionMap; // JMI

  conversionMap ["&"] = "&amp;";
  conversionMap ["\""] = "&quot;";
  conversionMap ["'"] = "&apos;";
  conversionMap ["<"] = "&lt;";
  conversionMap [">"] = "&gt;";

  size_t currentPosition = 0;

#ifdef DEBUG_SPLITTING
  std::string remainder = theString;
#endif // DEBUG_SPLITTING

// JMI  std::string lookedFor     = "&#xd;";
  std::string       lookedFor     = "\n";
  size_t            lookedForSize = lookedFor.size ();

  std::map <std::string, std::string>::const_iterator i;

/*
  for (i = conversionMap.begin (); i != conversionMap.end (); ++i) {
    std::string
      lookedFor = i->second,
      ersatz    = i->first;
    // replace all occurrences of lookedFor by ersatz
  } // for
*/

  while (1) {
    size_t found =
      theString.find (lookedFor, currentPosition);

    if (found == std::string::npos) {
      // fetch the last chunk
      // we have a last chunk
      // from currentPosition to theStringSize
      int chunkLength = theStringSize - currentPosition;

      std::string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      chunksList.push_back (
        chunk);

#ifdef DEBUG_SPLITTING
      gLog <<
        "theStringSize: " << theStringSize <<
        std::endl <<
        "currentPosition: " << currentPosition <<
        std::endl <<
        "remainder = |" << remainder << "|" <<
        std::endl <<
        "chunkLength: " << chunkLength <<
        std::endl <<
        "chunk = \"" << chunk << "\"" <<
        std::endl << std::endl;
#endif // DEBUG_SPLITTING

      break;
    }

    else {
      // we have a chunk from currentPosition to found
      int chunkLength = found - currentPosition;

      std::string
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

      gLog <<
        "theStringSize: " << theStringSize <<
        std::endl <<
        "currentPosition: " << currentPosition <<
        std::endl <<
        "remainder = |" << remainder << "|" <<
        std::endl <<
        "found: " << found <<
        std::endl <<
        "chunkLength: " << chunkLength <<
        std::endl <<
        "chunk = \"" << chunk << "\"" <<
        std::endl << std::endl;
#endif // DEBUG_SPLITTING
    }
  } // while
}

//______________________________________________________________________________
std::string mfBaseName (const std::string& filename)
{
  std::string result;

  size_t filenameSize = filename.size ();

  if (filename.size ()) {
    size_t lastSeparatorPosition = filename.find_last_of ("/\\");

    if (lastSeparatorPosition == std::string::npos) {
      result = filename;
    }
    else {
      result = filename.substr (
        lastSeparatorPosition + 1,
        filenameSize - lastSeparatorPosition - 1);
    }
  }

//   if (index + 1 >= len) {
//     --len;
//     index = filename.substr (0, len).find_last_of ("/\\");
//
//     if (len == 0) {
//       return filename;
//     }
//
//     if (index == 0) {
//       return filename.substr (1, len - 1);
//     }
//
//     if (index == std::string::npos) {
//       return filename.substr (0, len);
//     }
//
//     return filename.substr (index + 1, len - index - 1);
//   }
//
//   return filename.substr (index + 1, len - index);

  return result;
}

//______________________________________________________________________________
std::string mfMakeSingleWordFromString (const std::string& theString)
{
  std::string result;

  if (theString.size ()) {
    for (char theChar : theString) {
      if (isalnum (theChar)) {
        result.push_back (theChar);
      }
    } // for
  }

  return result;
}

//______________________________________________________________________________
Bool mfStringIsInStringSet (
  const std::string&           theString,
  const std::set <std::string>& stringSet)
{
  Bool result (false);

  if (stringSet.size ()) {
    std::set <std::string>::const_iterator
      it =
        stringSet.find (
          theString);

    if (it != stringSet.end ()) {
      result = true;
    }
  }

  return result;
}

void mfStringSetMinusEquals (
  std::set <std::string>&       dest,
  const std::set <std::string>& source)
{
  for (std::string theString : source) {
    size_t
      numberOfEraseElements =
        dest.erase (theString);

    gLog <<
      "numberOfEraseElements: " << numberOfEraseElements <<
      std::endl;
  } // for
}

std::string mfStringSetAsString (
  const std::set <std::string>& stringSet)
{
  std::stringstream ss;

  // append the set elements if any to ss
  size_t stringSetSize =
    stringSet.size ();

  if (stringSetSize) {
    size_t nextToLast = stringSetSize - 1;

    size_t count = 0;

    for (std::string theString : stringSet) {
      ++count;

      ss << "\"" << theString << "\"";

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != stringSetSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

std::string mfStringSetAsBracketedString (
  const std::set <std::string>& stringSet)
{
  std::stringstream ss;

  ss << '[' << mfStringSetAsString (stringSet) << ']';

  return ss.str ();
}

void mfDisplayStringSet (
  const std::string&           title,
  const std::set <std::string>& stringSet,
  std::ostream&                os)
{
  // print the title
  os << title << ":";

  // print the set elements if any
  size_t stringSetSize =
    stringSet.size ();

  if (stringSetSize) {
    os << std::endl;

    ++gIndenter;

    size_t nextToLast =
      stringSetSize - 1;

    size_t count = 0;

    for (
      std::set <std::string>::const_iterator
        i    = stringSet.begin (),
        iEnd = stringSet.end ();
      i != iEnd;
      ++i
    ) {
      std::string theString = (*i);

      ++count;

      os << "\"" << theString << "\"";
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

  os << std::endl;
}

//______________________________________________________________________________
Bool mfFetchValueFromStringToStringMap (
  const std::string&                        theKey,
  const std::map <std::string, std::string>& stringToStringMap,
  std::string&                              theValue)
{
  Bool result (false);

  if (stringToStringMap.size ()) {
    std::map <std::string, std::string>::const_iterator
      it =
        stringToStringMap.find (
          theKey);

    if (it != stringToStringMap.end ()) {
      theValue = (*it).second;
      result = true;
    }
  }

  return result;
}

std::string mfStringToStringMapAsString (
  const std::map <std::string, std::string>& stringToStringMap)
{
  std::stringstream ss;

  ss << '[';

  // append the set elements if any
  size_t stringToStringMapSize =
    stringToStringMap.size ();

  if (stringToStringMapSize) {
    size_t nextToLast =
      stringToStringMapSize - 1;

    size_t count = 0;

    for (
     std::map <std::string, std::string>::const_iterator
        i    = stringToStringMap.begin (),
        iEnd = stringToStringMap.end ();
      i != iEnd;
      ++i
    ) {
      std::pair thePair = (*i);

      std::string
        key   = thePair.first,
        value = thePair.second;

      ++count;

      ss << "\"" << key << "\": \"" << value << "\"";

      if (++i == iEnd) break;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != stringToStringMapSize) {
        ss << ", ";
      }
    } // for
  }

  ss << ']';

  return ss.str ();
}

void mfDisplayStringToStringMap (
  const std::string&                        title,
  const std::map <std::string, std::string>& stringToStringMap,
  std::ostream&                             os)
{
  // print the title
  os << title << ":";

  // print the set elements if any
  size_t stringToStringMapSize =
    stringToStringMap.size ();

  if (stringToStringMapSize) {
    os << std::endl;

    ++gIndenter;

    size_t nextToLast =
      stringToStringMapSize - 1;

    size_t count = 0;

    for (
     std::map <std::string, std::string>::const_iterator
        i    = stringToStringMap.begin (),
        iEnd = stringToStringMap.end ();
      i != iEnd;
      ++i
    ) {
      std::pair thePair = (*i);

      std::string
        key   = thePair.first,
        value = thePair.second;

      ++count;

      os << "\"" << key << "\": \"" << value << "\"";

      if (++i == iEnd) break;

      if (count == nextToLast) {
        os << " and ";
      }
      else if (count != stringToStringMapSize) {
        os << ", ";
      }
    } // for

    --gIndenter;
  }
  else {
    os << " empty";
  }

  os << std::endl;
}

//______________________________________________________________________________
Bool mfKeyIsInStringToStringMultiMap (
  const std::string&                             theKey,
  const std::multimap <std::string, std::string>& stringToStringMultiMap)
{
  const std::multimap <std::string, std::string>::const_iterator it =
    stringToStringMultiMap.find (theKey);

  return
    it != stringToStringMultiMap.end ();
}

Bool mfKeyValuePairIsInStringToStringMultiMap (
  const std::string&                             theKey,
  const std::multimap <std::string, std::string>& stringToStringMultiMap,
  const std::string&                             theValue)
{
  Bool result (false);

  if (stringToStringMultiMap.size ()) {
    // find all the  elements with key theKey
    std::pair <
      std::multimap <std::string, std::string>::const_iterator,
      std::multimap <std::string, std::string>::const_iterator
      >
      iteratorsRange =
        stringToStringMultiMap.equal_range (
          theKey);

    if (iteratorsRange.first != iteratorsRange.second) {
      // the range is non-empty, there are elements with key theKey
      for (
        std::multimap <std::string,std::string>::const_iterator it = iteratorsRange.first;
        it != iteratorsRange.second;
        ++it
      ) {
        if ((*it).second == theValue) {
          result = true;
          break;
        }
      }
    }
  }

  return result;
}

std::string mfStringToStringMultiMapAsString (
  const std::multimap <std::string, std::string>& stringToStringMultiMap)
{
  std::stringstream ss;

  ss << '[';

  // append the set elements if any
  size_t stringToStringMultiMapSize =
    stringToStringMultiMap.size ();

  if (stringToStringMultiMapSize) {
    size_t nextToLast =
      stringToStringMultiMapSize - 1;

    size_t count = 0;

    for (
     std::multimap <std::string, std::string>::const_iterator
        i    = stringToStringMultiMap.begin (),
        iEnd = stringToStringMultiMap.end ();
      i != iEnd;
      ++i
    ) {
      std::pair thePair = (*i);

      std::string
        key   = thePair.first,
        value = thePair.second;

      ++count;

      ss << "\"" << key << "\": \"" << value << "\"";

      if (++i == iEnd) break;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != stringToStringMultiMapSize) {
        ss << ", ";
      }
    } // for
  }

  ss << ']';

  return ss.str ();
}

void mfDisplayStringToStringMultiMap (
  const std::string&                             title,
  const std::multimap <std::string, std::string>& stringToStringMultiMap,
  std::ostream&                                  os)
{
  // print the title
  os << title << ":";

  // print the set elements if any
  size_t stringToStringMultiMapSize =
    stringToStringMultiMap.size ();

  if (stringToStringMultiMapSize) {
    os << std::endl;

    ++gIndenter;

    size_t nextToLast =
      stringToStringMultiMapSize - 1;

    size_t count = 0;

    for (
     std::multimap <std::string, std::string>::const_iterator
        i    = stringToStringMultiMap.begin (),
        iEnd = stringToStringMultiMap.end ();
      i != iEnd;
      ++i
    ) {
      std::pair thePair = (*i);

      std::string
        key   = thePair.first,
        value = thePair.second;

      ++count;

      os << "\"" << key << "\": \"" << value << "\"";

      if (++i == iEnd) break;

      if (count == nextToLast) {
        os << " and ";
      }
      else if (count != stringToStringMultiMapSize) {
        os << ", ";
      }
    } // for

    --gIndenter;
  }
  else {
    os << " empty";
  }

  os << std::endl;
}

//______________________________________________________________________________
int countTwoBytesWideCharactersInString (const std::string theString)
{
/*
  If theString contains non-diacritic characters (UTF-8),
  that fouls std::setw() which accounts two characters for them

  This is horrible custom code until C++20 std::format is more portable JMI v0.9.66
*/

  int result = 0;

  for_each (
    theString.begin(),
    theString.end(),
    [&result] (const char theChar) // capture result by reference in lambda body
      {
        if (
          // to be augmented when other 1-byte characters appear later in the code...
//           (theChar >= 'a' && theChar <= 'z')
//             ||
//           (theChar >= 'A' && theChar <= 'Z')
//             ||
//           (theChar == ' ')
//             ||
//           (theChar == ',')
//             ||
//           (theChar == ':')
//             ||
//           (theChar == '.')
//             ||
//           (theChar == '-')
//             ||
//           (theChar == '(')
//             ||
//           (theChar == ')')
//             ||
//           (theChar == '[')
//             ||
//           (theChar == ']')
//             ||
//           (theChar == '/')
//             ||
//           (theChar == '\\')
//             ||
//           (theChar == '\'')
//             ||
//           (theChar == '"')
          theChar <= 127 // JMI v0.9.67
        ) {
          // theChar is 1-byte large
        }

        else {
          // theChar is 2-bytes large
          result += 1;
        }
      }
  ); // for_each

  return result / 2;
    // because a 2-byte character's second byte is also seen in the loop above...
}


}

//     switch (wchar (theChar)) { JMI v0.9.66
//       case 'à':
//       case 'â':
//       case 'ä':
//
//       case 'é':
//       case 'è':
//       case 'ê':
//       case 'ë':
//
//       case 'ì':
//       case 'î':
//       case 'ï':
//
//       case 'ò':
//       case 'ô':
//       case 'ö':
//
//       case 'ù':
//       case 'û':
//       case 'ü':
//         result += 1;
//         break;
//
//       default:
//         ;
//     } // switch

