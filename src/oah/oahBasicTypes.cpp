/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <fstream>

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"
#include "mfTraceOah.h"

#include "oahOah.h"

#include "oahConstants.h"
#include "oahEarlyOptions.h"

#include "waeOah.h"
#include "oahDisplayOah.h"

#include "oahBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// //_______________________________________________________________________________
// EXP void convertArgcArgvToStringPairVector (
//   int                  argc,
//   char*                argv[],
//   mfStringsPairVector& stringPairVector)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       "Converting argc/argv to a string pair vector" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // make sure stringPairVector is empty
//   stringPairVector.erase (
//     stringPairVector.begin (),
//     stringPairVector.end ());
//
//   // populate stringPairVector from the elements in argv
//
//   std::string currentOption;
//
//   for (int i = 1; i < argc; ++i) {
//     std::string currString = argv [i];
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getTraceEarlyOptions ()) {
//       gLog <<
//         "--> currentOption: " << currentOption <<
//         std::endl <<
//         "--> currString 222: " << currString <<
//         std::endl << std::endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     if (currentOption.empty ()) {  // wait for option
//       if (currString [0] == '-') {
//         currentOption = currString;
//       }
//       else {
//         // JMI
//       }
//     }
//
//     else {
//       if (currString [0] == '-') {
//         // option without value
//         stringPairVector.push_back (
//           std::make_pair (
//             currentOption, ""));
//
//         currentOption = currString;
//       }
//       else {
//         // option with value
//         stringPairVector.push_back (
//           std::make_pair (
//             currentOption, currString));
//
//         currentOption = "";
//       }
//     }
//   } // for
//
//   if (currentOption.size())
//     stringPairVector.push_back (
//       std::make_pair (
//         currentOption, ""));
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     displayStringPairVector (
//       stringPairVector,
//       gLog);
//   }
// #endif // MF_TRACE_IS_ENABLED
// }

EXP void displayStringPairVector (
  const mfStringsPairVector& stringPairVector,
  std::ostream&                  os)
{
  size_t stringPairVectorSize =
    stringPairVector.size ();

  os <<
    "The string pair vector contains " <<
    mfSingularOrPlural (
      stringPairVectorSize, "element", "elements");

  if (stringPairVectorSize) {
    os << ":" << std::endl;

    ++gIndenter;

    const size_t fieldWidth = 2;

    int counter = 0;
    for (auto stringPair : stringPairVector) {
      std::string first = stringPair.first;
      std::string second = stringPair.second;

      os <<
        std::right << std::setw (fieldWidth) << counter++ <<
        ": [" <<
        first <<
        "] [" <<
        second <<
        ']' <<
        std::endl;
    } //for

    os << std::endl;

    --gIndenter;
  }
  else {
    os << std::endl;
  }
}

// static void argvElements2stringsVector (
//   int             argc,
//   char*           argv[],
//   std::vector <std::string>& stringsVector)
// {
//   for (int i = 1; i<argc; ++i) {
//     stringsVector.push_back (argv [i]);
//   } // for
// }

// //_______________________________________________________________________________
// EXP Bool arguments2optionsVector (
//   int              argc,
//   char             *argv[],
//   oahOptionsVector& theOptionsVector)
// {
//   // create a strings vector from the elements in argv
//   std::vector <std::string> stringsVector;
//
//   argvElements2stringsVector (argc, argv, stringsVector);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (getTraceEarlyOptions ()) {
//     gLog <<
//       "arguments2optionsVector: stringsVector size: " <<
//       stringsVector.size() <<
//       std::endl;
//
//     gLog << "==> stringsVector:" << std::endl;
//     for (std::string str: stringsVector) {
//       gLog << "   " << str << std::endl;
//     } // for
//
//     gLog << std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // populate the oahOptionsVector
//   // ------------------------------------------------------
//
//   std::string curOption;
//
//   size_t stringsVectorSize =
//     stringsVector.size ();
//
//   if (stringsVectorSize) {
//     for (size_t i = 0; i < stringsVectorSize; ++i) {
//       std::string str = stringsVector.at (i);
//
//   #ifdef MF_TRACE_IS_ENABLED
//       if (getTraceEarlyOptions ()) {
//         gLog << "--> curOption: " << curOption << std::endl;
//         gLog << "--> str     : " << str << std::endl;
//       }
//   #endif // MF_TRACE_IS_ENABLED
//
//       if (curOption.empty ()) {  // wait for option
//         if (str [0] == '-') {
//           curOption = str;
//         }
//         else {
//           return false;
//         }
//       }
//
//       else {
//         if (str [0] == '-') {
//           // option without value
//           theOptionsVector.push_back (std::make_pair (curOption, ""));
//           curOption = str;
//         }
//         else {
//           // option with value
//           theOptionsVector.push_back (std::make_pair(curOption, str));
//           curOption = "";
//         }
//       }
//     } // for
//   }
//
//   if (curOption.size())
//     theOptionsVector.push_back (std::make_pair (curOption, ""));
//
//   return true;
// }

// //______________________________________________________________________________
// void displayOptionsVector (
//   const oahOptionsVector& theOptionsVector,
//   std::ostream&             os)
// {
//   os <<
//     "The options vector contains " <<
//     mfSingularOrPlural (
//       theOptionsVector.size (), "element", "elements");
//
//   if (theOptionsVector.size ()) {
//     os << ":" << std::endl;
//
//     ++gIndenter;
//
//     const size_t fieldWidth = 2;
//
//     for (size_t i = 0; i < theOptionsVector.size (); ++i) {
//       std::string optionName = theOptionsVector.at (i).first;
//       std::string optionValue = theOptionsVector.at (i).second;
//
//       os <<
//         std::right << std::setw (fieldWidth) << i <<
//         ": \"" <<
//         optionName <<
//         "\" \"" <<
//         optionValue <<
//         "\"" <<
//         std::endl;
//     } //for
//
//     os << std::endl;
//
//     --gIndenter;
//   }
//   else {
//     os << std::endl;
//   }
// }

// //_______________________________________________________________________________
// static void argvElements2stringsVector (
//   int             argc,
//   char*           argv[],
//   std::vector <std::string>& stringsVector)
// {
//   for (int i = 1; i<argc; ++i) {
//     stringsVector.push_back (argv [i]);
//   } // for
// }

// //_______________________________________________________________________________
// EXP void convertArgcArgvToOptionsAndArguments (
//   int                    argc,
//   char                   *argv[],
//   oahOptionsAndArguments optionsAndArguments)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       "Converting argc/argv to options and arguments" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//   // create a strings vector from the elements in argv
//   std::vector <std::string> stringsVector;
//
//   argvElements2stringsVector (
//     argc,
//     argv,
//     stringsVector);
//
//   size_t stringsVectorSize =
//     stringsVector.size ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       "Converting argc/argv to options and arguments" <<
//       std::endl <<
//       "==> stringsVector (:" <<
//       stringsVectorSize <<
//       "elements):" <<
//       std::endl;
//
//     displayStringPairVector (
//       stringsVector,
//       gLog);
//
//     gLog << std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // populate optionsAndArguments
//   // ------------------------------------------------------
//
//   std::string currentOption;
//
//   if (stringsVectorSize) {
//     for (std::string currString: stringsVector) {
//
// #ifdef MF_TRACE_IS_ENABLED
//       if (gEarlyOptions.getTraceEarlyOptions ()) {
//         gLog <<
//           "--> currentOption: " << currentOption <<
//           std::endl <<
//           "--> currString  : " << currString <<
//           std::endl << std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       if (currentOption.empty ()) {  // wait for option
//         if (currString [0] == '-') {
//           currentOption = currString;
//         }
//         else {
//           // JMI
//         }
//       }
//
//       else {
//         if (currString [0] == '-') {
//           // option without value
//           optionsAndArguments.appendOption (
//             currentOption, "");
//
//           currentOption = currString;
//         }
//         else {
//           // option with value
//           optionsAndArguments.appendOption (
//             currentOption, currString);
//
//           currentOption = "";
//         }
//       }
//     } // for
//   }
//
//   if (currentOption.size())
//     optionsAndArguments.appendOption (
//       currentOption, "");
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       optionsAndArguments;
//   }
// #endif // MF_TRACE_IS_ENABLED
// }

//______________________________________________________________________________
std::string oahOptionOrArgumentKindAsString (
  oahOptionOrArgumentKind optionOrArgumentKind)
{
  std::string result;

  switch (optionOrArgumentKind) {
    case oahOptionOrArgumentKind::kOption:
      result = "format";
      break;
    case oahOptionOrArgumentKind::kArgument:
      result = "pass";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_oahOptionOrArgument oahOptionOrArgument::create (
  oahOptionOrArgumentKind optionOrArgumentKind,
  const std::string&      first,
  const std::string&      second)
{
  oahOptionOrArgument* obj = new
    oahOptionOrArgument (
      optionOrArgumentKind,
      first,
      second);
  assert (obj != nullptr);
  return obj;
}

oahOptionOrArgument::oahOptionOrArgument (
  oahOptionOrArgumentKind optionOrArgumentKind,
  const std::string&      first,
  const std::string&      second)
{
  fOptionOrArgumentKind = optionOrArgumentKind;

  fFirst = first;
  fSecond = second;
}

oahOptionOrArgument::~oahOptionOrArgument ()
{}

std::string oahOptionOrArgument::asString () const
{
  mfIndentedStringStream iss;

  iss <<
    "oahOptionOrArgument:" <<
    '\n';

  ++gIndenter;

  iss <<
    "fOptionOrArgumentKind" << ": " <<
    oahOptionOrArgumentKindAsString (
      fOptionOrArgumentKind) <<
    '\n' <<

    "fFirst" << ": " << fFirst <<
    '\n' <<
    "fSecond" << ": " << fSecond <<
    '\n';

  --gIndenter;

  return iss.str ();
}

void oahOptionOrArgument::print (std::ostream& os) const
{
  os <<
    "oahOptionOrArgument:" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 22;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOptionOrArgumentKind" << ": " <<
    oahOptionOrArgumentKindAsString (
      fOptionOrArgumentKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fFirst" << ": " << fFirst <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSecond" << ": " << fSecond <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahOptionOrArgument& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const oahOptionOrArgument& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
S_oahOption oahOption::create (
  const std::string& optionName,
  const std::string& optionValue)
{
  oahOption* obj = new
    oahOption (
      optionName,
      optionValue);
  assert (obj != nullptr);
  return obj;
}

oahOption::oahOption (
  const std::string& optionName,
  const std::string& optionValue)
{
  fOptionName = optionName;
  fOptionValue = optionValue;
}

oahOption::~oahOption ()
{}

std::string oahOption::asString () const
{
  std::stringstream ss;

  ss <<
    "oahOption [" << fOptionName << ']';

  if (fOptionValue.size ()) {
    ss << " [";

    if (fOptionValue.find ('\'') != std::string::npos) {
      ss <<
        " \"" + fOptionValue + '"';
    }
    else if (fOptionValue.find ('"') != std::string::npos) {
      ss <<
        " '" + fOptionValue + "\'";
    }
    else if (fOptionValue.find (' ') != std::string::npos) {
      ss <<
        " \"" + fOptionValue + '"';
    }
    else {
      ss <<
        ' ' + fOptionValue;
    }

    ss << ']';
  }

  return ss.str ();
}

std::string oahOption::asStringForCommandLine () const
{
  return
    oahOptionForCommandLine (
      fOptionName,
      fOptionValue);
}

void oahOption::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahOption& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const oahOption& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
std::string oahOptionForCommandLine (
  const std::string& optionName,
  const std::string& optionValue)
{
  std::stringstream ss;

  ss <<
    optionName;

  if (optionValue.size ()) {
    if (optionValue.find ('\'') != std::string::npos) {
      ss <<
        " \"" + optionValue + '"';
    }
    else if (optionValue.find ('"') != std::string::npos) {
      ss <<
        " '" + optionValue + "\'";
    }
    else if (optionValue.find (' ') != std::string::npos) {
      ss <<
        " \"" + optionValue + '"';
    }
    else {
      ss <<
        ' ' + optionValue;
    }
  }

  return ss.str ();
}

void optionsNameAndValueVectorsPlusEquals (
  std::vector <S_oahOption>&       vector1,
  const std::vector <S_oahOption>& vector2)
{
  // insert the elements of vector2 into vector1
  if (vector2.size ()) {
    for (S_oahOption option : vector2) {
      vector1.push_back (option);
    } //for
  }
}

// //______________________________________________________________________________
// S_oahOptionsNamesAndValuesVector oahOptionsNamesAndValuesVector::create ()
// {
//   oahOptionsNamesAndValuesVector* obj = new
//     oahOptionsNamesAndValuesVector ();
//   assert (obj != nullptr);
//   return obj;
// }
//
// oahOptionsNamesAndValuesVector::oahOptionsNamesAndValuesVector ()
// {}
//
// oahOptionsNamesAndValuesVector::~oahOptionsNamesAndValuesVector ()
// {}
//
// S_oahOptionsNamesAndValuesVector oahOptionsNamesAndValuesVector::mergeOptionsNamesAndValues (
//   S_oahOptionsNamesAndValuesVector
//     optionsNamesAndValues1,
//   S_oahOptionsNamesAndValuesVector
//     optionsNamesAndValues2)
// {
//   S_oahOptionsNamesAndValuesVector
//     result =
//       oahOptionsNamesAndValuesVector::create ();
//
//   for (
//     S_oahOption option
//     :
//     optionsNamesAndValues1->getOptionsVector ()
//   ) {
//     result->
//       appendOptionNameAndValue (
//         option);
//   } //for
//
//   for (
//     S_oahOption option
//     :
//     optionsNamesAndValues1->getOptionsVector ()
//   ) {
//     result->
//       appendOptionNameAndValue (
//         option);
//   } //for
//
//   return result;
// }
//
// void oahOptionsNamesAndValuesVector::displayOptionsVector (std::ostream& os) const
// {
//   os <<
//     "The option name and values vector contains " <<
//     mfSingularOrPlural (
//       fOptionsVector.size (), "element", "elements");
//
//   if (fOptionsVector.size ()) {
//     os << ":" << std::endl;
//
//     ++gIndenter;
//
//     constexpr int fieldWidth = 2;
//
//     int counter = 0;
//     for (S_oahOption option : fOptionsVector) {
//       std::string optionName  =
//         option->getOptionName ();
//       std::string optionValue =
//         option->getOptionValue ();
//
//       os <<
//         std::right << std::setw (fieldWidth) << counter++ <<
//         ": [" <<
//         optionName <<
//         "] [" <<
//         optionValue <<
//         ']' <<
//         std::endl;
//     } //for
//
//     os << std::endl;
//
//     --gIndenter;
//   }
//   else {
//     os << std::endl;
//   }
// }
//
// std::string oahOptionsNamesAndValuesVector::asCommandLineOptionsString () const
// {
//   std::stringstream ss;
//
//   for (S_oahOption option : fOptionsVector) {
//     std::string
//       optionName  =
//         option->getOptionName (),
//       optionValue =
//         option->getOptionValue ();
//
//     ss <<
//       optionName;
//
//     if (optionValue.size ()) {
//       ss << ' ';
//
//       if (optionValue.find ('\'') != std::string::npos) {
//         ss <<
//           " \"" + optionValue + '"';
//       }
//       else if (optionValue.find ('"') != std::string::npos) {
//         ss <<
//           " '" + optionValue + "\'";
//       }
//       else if (optionValue.find (' ') != std::string::npos) {
//         ss <<
//           " \"" + optionValue + '"';
//       }
//       else {
//         ss <<
//           ' ' + optionValue;
//       }
//     }
//   } //for
//
//   return ss.str ();
// }
//
// // void oahOptionsNamesAndValuesVector::asString () const
// // {
// //   return ""; // JMI
// // }
//
// void oahOptionsNamesAndValuesVector::print (std::ostream& os) const
// {
//   for (S_oahOption option : fOptionsVector) {
//     std::string optionName  =
//       option->getOptionName ();
//     std::string optionValue =
//       option->getOptionValue ();
//
//     os <<
//       optionName;
//
//     if (optionValue.size ()) {
//       os << ' ';
//
//       if (optionValue.find ('\'') != std::string::npos) {
//         os <<
//           " \"" + optionValue + '"';
//       }
//       else if (optionValue.find ('"') != std::string::npos) {
//         os <<
//           " '" + optionValue + "\'";
//       }
//       else if (optionValue.find (' ') != std::string::npos) {
//         os <<
//           " \"" + optionValue + '"';
//       }
//       else {
//         os <<
//           ' ' + optionValue;
//       }
//     }
//   } //for
// }
//
// std::ostream& operator << (std::ostream& os, const S_oahOptionsNamesAndValuesVector& elt)
// {
//   if (elt) {
//     elt->print (os);
//   }
//   else {
//     os << "[NULL]" << std::endl;
//   }

//   return os;
// }
//
// std::ostream& operator << (std::ostream& os, const oahOptionsNamesAndValuesVector& elt)
// {
//   elt.print (os);
//   return os;
// }

//______________________________________________________________________________
S_oahOptionsAndArguments oahOptionsAndArguments::create ()
{
  oahOptionsAndArguments* obj = new
    oahOptionsAndArguments ();
  assert (obj != nullptr);
  return obj;
}

oahOptionsAndArguments::oahOptionsAndArguments ()
{}

oahOptionsAndArguments::~oahOptionsAndArguments ()
{}

void oahOptionsAndArguments::appendOptionNameAndValue (
  const oahOption&
    option)
{
  fOptionsVector.push_back (
    option);
}

void oahOptionsAndArguments::appendOptionNameAndValue (
  const std::string& optionName,
  const std::string& optionValue)
{
  fOptionsVector.push_back (
    oahOption (
      optionName,
      optionValue));
}

void oahOptionsAndArguments::displayOptionsVector (std::ostream& os) const
{
  os <<
    "The option name and values vector contains " <<
    mfSingularOrPlural (
      fOptionsVector.size (), "element", "elements");

  if (fOptionsVector.size ()) {
    os << ":" << std::endl;

    ++gIndenter;

    constexpr int fieldWidth = 2;

    int counter = 0;
    for (oahOption option : fOptionsVector) {
      std::string optionName  =
        option.getOptionName ();
      std::string optionValue =
        option.getOptionValue ();

      os <<
        std::right << std::setw (fieldWidth) << counter++ <<
        ": [" <<
        optionName <<
        "] [" <<
        optionValue <<
        ']' <<
        std::endl;
    } //for

    os << std::endl;

    --gIndenter;
  }
  else {
    os << std::endl;
  }
}

void oahOptionsAndArguments::displayArgumentsVector (std::ostream& os) const
{
  mfDisplayStringsVector (
    fArgumentsVector,
    "fArgumentsVector",
    os);
}

void oahOptionsAndArguments::print (std::ostream& os) const
{
//   os << JMI
//     "oahOptionsAndArguments:" <<
//     std::endl;
//
//   ++gIndenter;
//
  displayOptionsVector (os);
  displayArgumentsVector (os);

//   --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahOptionsAndArguments& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const oahOptionsAndArguments& elt)
{
  elt.print (os);
  return os;
}

// early option?
//______________________________________________________________________________
std::string oahEarlyOptionKindAsString (
  oahEarlyOptionKind earlyOptionKind)
{
  std::string result;

  switch (earlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo: // default value
      result = "kEarlyOptionNo";
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      result = "kEarlyOptionYes";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const oahEarlyOptionKind& elt)
{
  os << oahEarlyOptionKindAsString (elt);
  return os;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const oahEarlyOptionKind& elt)
{
  iss.getStringstream () <<
    oahEarlyOptionKindAsString (elt);

  return iss;
}

// handler used thru...?
//______________________________________________________________________________
std::string oahHandlerUsedThruKindAsString (
  oahHandlerUsedThruKind handlerUsedThruKind)
{
  std::string result;

  switch (handlerUsedThruKind) {
    case oahHandlerUsedThruKind::kHandlerUsedThruUnknown: // default value
      result = "kHandlerUsedThruUnknown";
      break;
    case oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv:
      result = "kHandlerUsedThruArgcArgv";
      break;
    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      result = "kHandlerUsedThruOptionsAndArguments";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const oahHandlerUsedThruKind& elt)
{
  os << oahHandlerUsedThruKindAsString (elt);
  return os;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const oahHandlerUsedThruKind& elt)
{
  iss.getStringstream () <<
    oahHandlerUsedThruKindAsString (elt);

  return iss;
}

//______________________________________________________________________________
S_oahPrefix oahPrefix::create (
  const std::string& prefixName,
  const std::string& prefixErsatz,
  const std::string& prefixDescription)
{
  oahPrefix* obj = new
    oahPrefix (
      prefixName,
      prefixErsatz,
      prefixDescription);
  assert (obj != nullptr);
  return obj;
}

oahPrefix::oahPrefix (
  const std::string& prefixName,
  const std::string& prefixErsatz,
  const std::string& prefixDescription)
{
  fPrefixName = prefixName;
  fPrefixErsatz = prefixErsatz;
  fPrefixDescription = prefixDescription;
}

oahPrefix::~oahPrefix ()
{}

void oahPrefix::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahPrefix::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahPrefix>*
    p =
      dynamic_cast<visitor<S_oahPrefix>*> (v)) {
        S_oahPrefix elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahPrefix::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahPrefix::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahPrefix::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahPrefix>*
    p =
      dynamic_cast<visitor<S_oahPrefix>*> (v)) {
        S_oahPrefix elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahPrefix::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahPrefix::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahPrefix::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string oahPrefix::fetchPrefixNames () const
{
  std::stringstream ss;

  ss <<
    "\"" <<
    fPrefixName <<
    "\", \"" <<
   fPrefixErsatz <<
   "\"";

  return ss.str ();
}

Bool oahPrefix::findStringInFindableElement (
  const std::string&               lowerCaseString,
  std::list <S_oahFindStringMatch>& foundMatchesList,
  std::ostream&                    os) const
{
  Bool result;

  // does this element's short name match?
  Bool prefixNameMatches =
    mfStringToLowerCase (fPrefixName).find (lowerCaseString) != std::string::npos;

  // does this element's long name match?
  Bool prefixErsatzMatches =
    mfStringToLowerCase (fPrefixErsatz).find (lowerCaseString) != std::string::npos;

  // does this element's description match?
  Bool prefixDescriptionMatches =
    mfStringToLowerCase (fPrefixDescription).find (lowerCaseString) != std::string::npos;

  if (prefixNameMatches || prefixErsatzMatches || prefixDescriptionMatches) {
    // append the match to foundStringsList
    foundMatchesList.push_back (
      oahFindStringMatch::create (
        fetchPrefixNames (),
        fPrefixDescription,
        containingFindableElementAsString ()));

    result = true;
  }

  return result;
}

Bool oahPrefix::fetchElementsMatchingStringInPrefix (
	const std::string&       lowerCaseString,
	std::list <S_oahElement>& foundElementsList,
	std::ostream&            os) const
{
  Bool result;

  // does this element's short name match?
  Bool prefixNameMatches =
    mfStringToLowerCase (fPrefixName).find (lowerCaseString) != std::string::npos;

  // does this element's long name match?
  Bool prefixErsatzMatches =
    mfStringToLowerCase (fPrefixErsatz).find (lowerCaseString) != std::string::npos;

  // does this element's description match?
  Bool prefixDescriptionMatches =
    mfStringToLowerCase (fPrefixDescription).find (lowerCaseString) != std::string::npos;

  if (prefixNameMatches || prefixErsatzMatches || prefixDescriptionMatches) {
    // append the match to foundStringsList
//     foundElementsList.push_back (// JMI 0.9.71 OHA_OAH
//       oahFindStringMatch::create (
//         fetchPrefixNames (),
//         fPrefixDescription,
//         containingFindableElementAsString ()));

    result = true;
  }

  return result;
}

void oahPrefix::printPrefixEssentials (
  std::ostream& os) const
{
  constexpr int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "prefixName" << ": " <<
    fPrefixName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "prefixErsatz" << ": " <<
    fPrefixErsatz <<
    std::endl <<
    std::setw (fieldWidth) <<
    "prefixDescription" << ": " <<
    fPrefixDescription <<
    std::endl;
}

std::string oahPrefix::asString () const
{
  std::stringstream ss;

  ss <<
    "[oahPrefix" <<
    ", prefixName: " <<
    fPrefixName <<
    ", prefixErsatz: " <<
    fPrefixErsatz <<
    ", prefixDescription: " <<
    fPrefixDescription;

  ss << ']';

  return ss.str ();
}

void oahPrefix::print (std::ostream& os) const
{
  os <<
    "[oahPrefix" <<
    std::endl;

  printPrefixEssentials (os);

  os << ']';
}

void oahPrefix::printHelp (std::ostream& os) const
{
  if (fPrefixDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fPrefixDescription,
      os);

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  os << std::endl;
}

// const std::string oahPrefix::containingFindableElementAsString () const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Prefix:"; // JMI 0.9.66
//
//   return ss.str ();
// }

std::ostream& operator << (std::ostream& os, const S_oahPrefix& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtom oahAtom::create (
  const std::string&  longName,
  const std::string&  shortName,
  const std::string&  description,
  oahElementValueKind atomValueExpectedKind)
{
  oahAtom* obj = new
    oahAtom (
      longName,
      shortName,
      description,
      atomValueExpectedKind);
  assert (obj != nullptr);
  return obj;
}
*/

oahAtom::oahAtom (
  const std::string&  longName,
  const std::string&  shortName,
  const std::string&  description,
  oahElementValueKind atomValueExpectedKind)
  : oahElement (
      longName,
      shortName,
      description,
      atomValueExpectedKind,
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  fEarlyOptionKind =
    oahEarlyOptionKind::kEarlyOptionNo; // default value
}

oahAtom::~oahAtom ()
{}

void oahAtom::setUpLinkToSubGroup (
  const S_oahSubGroup& subGroup)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    subGroup != nullptr,
    "subGroup is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set the atom subgroup upLink
  fUpLinkToSubGroup = subGroup;
}

S_oahGroup oahAtom::fetchAtomUpLinkToGroup () const
{
  S_oahGroup result;

  if (fUpLinkToSubGroup) {
    result =
      fUpLinkToSubGroup->
        getUpLinkToGroup ();
  }

  return result;
}

S_oahHandler oahAtom::fetchAtomUpLinkToHandler () const
{
  S_oahHandler result;

  if (fUpLinkToSubGroup) {
    result =
      fUpLinkToSubGroup->
        getUpLinkToGroup ()->
          getUpLinkToHandler ();
  }

  return result;
}

void oahAtom::appendAtomToElementsList (
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    if (false) { // JMI ???
      std::stringstream ss;

      ss <<
        "Registering atom " <<
        fetchNamesBetweenQuotes () <<
        " in handler \"" <<
        handler->getHandlerHeader () <<
        "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // an atom with an optional value
  // should not have the same name as a prefix,
  // since this would create an ambiguity
  switch (fElementValueKind) {
    case oahElementValueKind::kElementValue_UNKNOWN_:
      handler->
        unknownOptionValueKindError (
          fetchNamesBetweenQuotes ());
      break;

    case oahElementValueKind::kElementValueLess:
//     case oahElementValueKind::kElementValueImplicit:
    case oahElementValueKind::kElementValueFitted:
      break;

    case oahElementValueKind::kElementValueDefaulted:
      if (handler->fetchNameInPrefixesMap (fLongName)) {
        std::stringstream ss;

        ss <<
          "the long name of atom with optional value " <<
          fetchNamesBetweenQuotes () <<
          " is already known as a prefix";

        oahError (ss.str ());
      }
      if (handler->fetchNameInPrefixesMap (fShortName)) {
        std::stringstream ss;

        ss <<
          "the short name of atom with optional value " <<
          fetchNamesBetweenQuotes () <<
          " is already known as a prefix";

        oahError (ss.str ());
      }
      break;
  } // switch

  handler->
    appendElementToElementsList (this);

  --gIndenter;
}

void oahAtom::registerAtomAsBeingUsed ()
{
  fUpLinkToSubGroup->
    incrementNumberOfUserChoseAtomsInThisSubGroup ();

  S_oahGroup
    atomGroup =
      fUpLinkToSubGroup-> getUpLinkToGroup ();

  atomGroup->
    incrementNumberOfUserChoseAtomsInThisGroup ();
}

void oahAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahAtom>*
    p =
      dynamic_cast<visitor<S_oahAtom>*> (v)) {
        S_oahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahAtom>*
    p =
      dynamic_cast<visitor<S_oahAtom>*> (v)) {
        S_oahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// std::string oahAtom::asString (std::ostream& os) const // JMI 0.9.67
// {
//   stringstream ss;
//
//   ss <<
//     "[Atom:" <<
//     ']';
//
//   return ss.str ();
// }

void oahAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = 19;

  os <<
    "Atom: ";

  oahElement::printOahElementEssentials (
    os, fieldWidth);
}

void oahAtom::printFull (std::ostream& os) const
{
  constexpr int fieldWidth = 19;

  os <<
    "Atom ???:" <<
      std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) << "fElementHelpOnlyKind" << ": " <<
    oahElementHelpOnlyKindAsString (fElementHelpOnlyKind) <<
    std::endl;

  --gIndenter;
}

void oahAtom::printSummary (std::ostream& os) const
{
  os <<
    "Atom: " <<
    fetchNames () <<
    std::endl;
}

Bool oahAtom::findStringInAtom (
  const std::string&               lowerCaseString,
  std::list <S_oahFindStringMatch>& foundMatchesList,
  std::ostream&                    os) const
{
  return
    findStringInFindableElement (
      lowerCaseString,
      foundMatchesList,
      os);
}

// const std::string oahAtom::containingFindableElementAsString () const
// {
//   std::stringstream ss;
//
//   ss << std::left <<
//     "Atom in subgroup \"" <<
//     fUpLinkToSubGroup->getSubGroupHeader () <<
//     "\" in group \"" <<
//     fUpLinkToSubGroup->getUpLinkToGroup ()->getGroupHeader () <<
//     "\"";
//
//   return ss.str ();
// }

Bool oahAtom::atomMatchesString (
	const std::string&       lowerCaseString,
	std::ostream&            os) const
{
  return true;
//     atomMatchesString (
//       lowerCaseString,
//       os);
}

void oahAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os <<
    "Atom values ???:" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahValueLessAtom oahValueLessAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
{
  oahValueLessAtom* obj = new
    oahValueLessAtom (
      longName,
      shortName,
      description);
  assert (obj != nullptr);
  return obj;
}
*/

oahValueLessAtom::oahValueLessAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueLess)
{}

oahValueLessAtom::~oahValueLessAtom ()
{}

Bool oahValueLessAtom::atomMatchesString (
  const std::string& lowerCaseString,
  std::ostream&      os) const
{
  return true;
}

void oahValueLessAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueLessAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahValueLessAtom>*
    p =
      dynamic_cast<visitor<S_oahValueLessAtom>*> (v)) {
        S_oahValueLessAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahValueLessAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahValueLessAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueLessAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahValueLessAtom>*
    p =
      dynamic_cast<visitor<S_oahValueLessAtom>*> (v)) {
        S_oahValueLessAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahValueLessAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahValueLessAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueLessAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahValueLessAtom::printFull (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahValueLessAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahValueLessAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahValueLessAtom: ";

  printOahElementEssentials (
    os, fieldWidth);

  os <<
    fDescription <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahValueLessAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahValueFittedAtom oahValueFittedAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
{
  oahValueFittedAtom* obj = new
    oahValueFittedAtom (
      longName,
      shortName,
      description);
  assert (obj != nullptr);
  return obj;
}
*/

oahValueFittedAtom::oahValueFittedAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueFitted)
{}

oahValueFittedAtom::~oahValueFittedAtom ()
{}

void oahValueFittedAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueFittedAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahValueFittedAtom>*
    p =
      dynamic_cast<visitor<S_oahValueFittedAtom>*> (v)) {
        S_oahValueFittedAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahValueFittedAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahValueFittedAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueFittedAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahValueFittedAtom>*
    p =
      dynamic_cast<visitor<S_oahValueFittedAtom>*> (v)) {
        S_oahValueFittedAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahValueFittedAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahValueFittedAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahValueFittedAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// void oahValueFittedAtom::applyAtomWithDefaultValue (std::ostream& os) // JMI ??? 0.9.66
// {
//   std::stringstream ss;
//
//   ss <<
//     "Applying value-fitted atom '" <<
//     fetchNames () <<
//     "' with a default value is possible only in oahValueFittedAtom sub-classes";
//
//   oahInternalError (ss.str ());
// }

void oahValueFittedAtom::printFull (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahValueFittedAtom:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahValueFittedAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahValueFittedAtom: ";

  printOahElementEssentials (
    os, fieldWidth);

  os <<
    fDescription <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahValueFittedAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtomImplicitlySettingAVariable oahAtomImplicitlySettingAVariable::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName)
{
  oahAtomImplicitlySettingAVariable* obj = new
    oahAtomImplicitlySettingAVariable (
      longName,
      shortName,
      description,
      variableName);
  assert (obj != nullptr);
  return obj;
}
*/

oahAtomImplicitlySettingAVariable::oahAtomImplicitlySettingAVariable (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{
  fVariableName = variableName;
}

oahAtomImplicitlySettingAVariable::~oahAtomImplicitlySettingAVariable ()
{}

void oahAtomImplicitlySettingAVariable::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAtomImplicitlySettingAVariable::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahAtomImplicitlySettingAVariable>*
    p =
      dynamic_cast<visitor<S_oahAtomImplicitlySettingAVariable>*> (v)) {
        S_oahAtomImplicitlySettingAVariable elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahAtomImplicitlySettingAVariable::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahAtomImplicitlySettingAVariable::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAtomImplicitlySettingAVariable::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahAtomImplicitlySettingAVariable>*
    p =
      dynamic_cast<visitor<S_oahAtomImplicitlySettingAVariable>*> (v)) {
        S_oahAtomImplicitlySettingAVariable elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahAtomImplicitlySettingAVariable::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahAtomImplicitlySettingAVariable::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAtomImplicitlySettingAVariable::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahAtomImplicitlySettingAVariable::printAtomWithVariableEssentials (
  std::ostream& os,
  int fieldWidth) const
{
  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl;
  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "selected" <<
      std::endl;
  }
}

void oahAtomImplicitlySettingAVariable::printAtomWithVariableEssentialsFull (
  std::ostream& os,
  int fieldWidth) const
{
  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl;
  if (fSelected) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "selected" <<
      std::endl;
  }
}

void oahAtomImplicitlySettingAVariable::print (std::ostream& os) const
{
  constexpr int fieldWidth = 19;

  os <<
    "AtomWithVariable";
  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os << ", selected";
  }
  os << std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomImplicitlySettingAVariable::printHelp (std::ostream& os) const
{
  os <<
    fetchNames () <<
    std::endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahAtomImplicitlySettingAVariable::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os <<
    "AtomWithVariable values:" <<
    "???, fSelected: " <<
    fSelected <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahAtomImplicitlySettingAVariable& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahValueDefaultedAtom oahValueDefaultedAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& defaultStringValue)
{
  oahValueDefaultedAtom* obj = new
    oahValueDefaultedAtom (
      longName,
      shortName,
      description);
  assert (obj != nullptr);
  return obj;
}
*/

oahValueDefaultedAtom::oahValueDefaultedAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& defaultStringValue)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueDefaulted)
{
  fDefaultStringValue = defaultStringValue;
}

oahValueDefaultedAtom::~oahValueDefaultedAtom ()
{}

// void oahValueDefaultedAtom::applyAtomWithValue (
//   const std::string& theString,
//   std::ostream&      os)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       "Handling option name '" <<
//       fetchNames () <<
//       "' which is a oahValueDefaultedAtom" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   oahStringAtom::applyAtomWithValue (
//     theString,
//     os);
// }
//
// void oahValueDefaultedAtom::acceptIn (basevisitor* v)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gOahOahGroup->getTraceOahVisitors ()) {
//     gLog <<
//       ".\\\" ==> oahValueDefaultedAtom::acceptIn ()" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (visitor<S_oahValueDefaultedAtom>*
//     p =
//       dynamic_cast<visitor<S_oahValueDefaultedAtom>*> (v)) {
//         S_oahValueDefaultedAtom elem = this;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gOahOahGroup->getTraceOahVisitors ()) {
//           gLog <<
//             ".\\\" ==> Launching oahValueDefaultedAtom::visitStart ()" <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//         p->visitStart (elem);
//   }
// }
//
// void oahValueDefaultedAtom::setStringVariable (const std::string& value)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       "Setting option '" <<
//       fetchNames () <<
//       "' string variable to \"" <<
//      value <<
//       "\"" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   oahStringAtom::setStringVariable (value);
//   fSelected = true;
// }
//
// void oahValueDefaultedAtom::acceptOut (basevisitor* v)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gOahOahGroup->getTraceOahVisitors ()) {
//     gLog <<
//       ".\\\" ==> oahValueDefaultedAtom::acceptOut ()" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (visitor<S_oahValueDefaultedAtom>*
//     p =
//       dynamic_cast<visitor<S_oahValueDefaultedAtom>*> (v)) {
//         S_oahValueDefaultedAtom elem = this;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gOahOahGroup->getTraceOahVisitors ()) {
//           gLog <<
//             ".\\\" ==> Launching oahValueDefaultedAtom::visitEnd ()" <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//         p->visitEnd (elem);
//   }
// }
//
// void oahValueDefaultedAtom::browseData (basevisitor* v)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gOahOahGroup->getTraceOahVisitors ()) {
//     gLog <<
//       ".\\\" ==> oahValueDefaultedAtom::browseData ()" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
// }
//
// std::string oahValueDefaultedAtom::asShortNamedOptionString () const
// {
//   std::stringstream ss;
//
//   ss <<
//     '-' << fShortName << ' ' << fStringVariable;
//
//   return ss.str ();
// }
//
// std::string oahValueDefaultedAtom::asActualLongNamedOptionString () const
// {
//   std::stringstream ss;
//
//   ss <<
//     '-' << fLongName << ' ' << fStringVariable;
//
//   return ss.str ();
// }
//
// void oahValueDefaultedAtom::print (std::ostream& os) const
// {
//   constexpr int fieldWidth = K_OAH_FIELD_WIDTH;
//
//   os <<
//     "StringWithDefaultValueAtom:" <<
//     std::endl;
//
//   ++gIndenter;
//
//   printAtomWithVariableEssentials (
//     os, fieldWidth);
//
//   os << std::left <<
//     std::setw (fieldWidth) <<
//     "fStringVariable" << ": " <<
//     "\"" << fStringVariable << "\"" <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "fDefaultStringValue" << ": " <<
//     "\"" << fDefaultStringValue << "\"" <<
//     std::endl;
//
//   --gIndenter;
// }
//
// void oahValueDefaultedAtom::displayAtomWithVariableOptionsValues (
//   std::ostream& os,
//   int           valueFieldWidth) const
// {
//   os << std::left <<
//     std::setw (valueFieldWidth) <<
//     fVariableName <<
//     ": " <<
//     fStringVariable;
//
//   if (fSelected) {
//     os <<
//       ", selected";
//   }
//
//   os << std::endl;
// }
//
// std::ostream& operator << (std::ostream& os, const S_oahValueDefaultedAtom& elt)
// {
//   os <<
//     "StringWithDefaultValueAtom:" <<
//     std::endl;
//   if (elt) {
//     elt->print (os);
//   }
//   else {
//     os << "[NULL]" << std::endl;
//   }
//
//   return os;
// }

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtomStoringAValue oahAtomStoringAValue::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName)
{
  oahAtomStoringAValue* obj = new
    oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName);
  assert (obj != nullptr);
  return obj;
}
*/

oahAtomStoringAValue::oahAtomStoringAValue (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName)
  : oahValueFittedAtom (
      longName,
      shortName,
      description)
{
  fValueSpecification = valueSpecification;

  fVariableName = variableName;
}

oahAtomStoringAValue::~oahAtomStoringAValue ()
{}

void oahAtomStoringAValue::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAtomStoringAValue::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahAtomStoringAValue>*
    p =
      dynamic_cast<visitor<S_oahAtomStoringAValue>*> (v)) {
        S_oahAtomStoringAValue elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahAtomStoringAValue::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahAtomStoringAValue::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAtomStoringAValue::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahAtomStoringAValue>*
    p =
      dynamic_cast<visitor<S_oahAtomStoringAValue>*> (v)) {
        S_oahAtomStoringAValue elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahAtomStoringAValue::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahAtomStoringAValue::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahAtomStoringAValue::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahAtomStoringAValue::printAtomWithVariableEssentials (
  std::ostream& os,
  int fieldWidth) const
{
  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fValueSpecification" << ": " <<
    fValueSpecification <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSelected" << ": " <<
    fSelected <<
    std::endl;
}

void oahAtomStoringAValue::printAtomWithVariableEssentialsFull (
  std::ostream& os,
  int fieldWidth) const
{
  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fValueSpecification" << ": " <<
    fValueSpecification <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVariableName" << ": " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSelected" << ": " <<
    fSelected <<
    std::endl;
}

void oahAtomStoringAValue::print (std::ostream& os) const
{
  constexpr int fieldWidth = 19;

  os <<
    "AtomWithVariable";
  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os << ", selected";
  }
  os << std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomStoringAValue::printHelp (std::ostream& os) const
{
  os <<
    fetchNames () <<
    ' ' << fValueSpecification <<
    std::endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahAtomStoringAValue::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os <<
    "AtomWithVariable values:" <<
    "???, fSelected: " <<
    fSelected <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahAtomStoringAValue& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahPureHelpValueLessAtom oahPureHelpValueLessAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahPureHelpValueLessAtom* obj = new
    oahPureHelpValueLessAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}
*/

oahPureHelpValueLessAtom::oahPureHelpValueLessAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{
  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  fHelpValueLessAtomServiceName = serviceName;
}

oahPureHelpValueLessAtom::~oahPureHelpValueLessAtom ()
{}

void oahPureHelpValueLessAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahPureHelpValueLessAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahPureHelpValueLessAtom>*
    p =
      dynamic_cast<visitor<S_oahPureHelpValueLessAtom>*> (v)) {
        S_oahPureHelpValueLessAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahPureHelpValueLessAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahPureHelpValueLessAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahPureHelpValueLessAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahPureHelpValueLessAtom>*
    p =
      dynamic_cast<visitor<S_oahPureHelpValueLessAtom>*> (v)) {
        S_oahPureHelpValueLessAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahPureHelpValueLessAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahPureHelpValueLessAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahPureHelpValueLessAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahPureHelpValueLessAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "HelpOnlyAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahPureHelpValueLessAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_oahPureHelpValueLessAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahPureHelpValueFittedAtom oahPureHelpValueFittedAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahPureHelpValueFittedAtom* obj = new
    oahPureHelpValueFittedAtom (
      longName,
      shortName,
      description,
      serviceName);
  assert (obj != nullptr);
  return obj;
}
*/

oahPureHelpValueFittedAtom::oahPureHelpValueFittedAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahValueFittedAtom (
      longName,
      shortName,
      description)
{
  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  fHelpValueFittedAtomServiceName = serviceName;

  fValueSpecification = valueSpecification;
}

oahPureHelpValueFittedAtom::~oahPureHelpValueFittedAtom ()
{}

void oahPureHelpValueFittedAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahPureHelpValueFittedAtom::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahPureHelpValueFittedAtom>*
    p =
      dynamic_cast<visitor<S_oahPureHelpValueFittedAtom>*> (v)) {
        S_oahPureHelpValueFittedAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahPureHelpValueFittedAtom::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahPureHelpValueFittedAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahPureHelpValueFittedAtom::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahPureHelpValueFittedAtom>*
    p =
      dynamic_cast<visitor<S_oahPureHelpValueFittedAtom>*> (v)) {
        S_oahPureHelpValueFittedAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahPureHelpValueFittedAtom::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahPureHelpValueFittedAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahPureHelpValueFittedAtom::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahPureHelpValueFittedAtom::print (std::ostream& os) const
{
  constexpr int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "HelpOnlyAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fValueSpecification" << ": " <<
    fValueSpecification <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fHelpValueFittedAtomServiceName" << ": " <<
    fHelpValueFittedAtomServiceName <<
    std::endl;

  --gIndenter;
}

void oahPureHelpValueFittedAtom::printHelp (std::ostream& os) const
{
  os <<
    fetchNames () <<
    ' ' << fValueSpecification <<
    std::endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

std::ostream& operator << (std::ostream& os, const S_oahPureHelpValueFittedAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahSubGroup oahSubGroup::create (
  const std::string&       subGroupHeader,
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementVisibilityKind optionVisibilityKind,
  const S_oahGroup&        upLinkToGroup)
{
  oahSubGroup* obj = new
    oahSubGroup (
      subGroupHeader,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      upLinkToGroup);
  assert (obj != nullptr);
  return obj;
}

S_oahSubGroup oahSubGroup::createWithNames (
  const std::string&       subGroupHeader,
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementVisibilityKind optionVisibilityKind,
  const S_oahGroup&        upLinkToGroup)
{
  oahSubGroup* obj = new
    oahSubGroup (
      subGroupHeader,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      upLinkToGroup);
  assert (obj != nullptr);
  return obj;
}

oahSubGroup::oahSubGroup (
  const std::string&       subGroupHeader,
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementVisibilityKind optionVisibilityKind,
  const S_oahGroup&        upLinkToGroup)
  : oahElement (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueLess,
      optionVisibilityKind)
{
  fUpLinkToGroup = upLinkToGroup;

  fSubGroupHeader = subGroupHeader;

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  fNumberOfUserChoseAtomsInThisSubGroup = 0;
}

oahSubGroup::~oahSubGroup ()
{}

S_oahHandler oahSubGroup::fetchSubGroupUpLinkToHandler () const
{
  S_oahHandler result;

  if (fUpLinkToGroup) {
    result =
      fUpLinkToGroup->
        getUpLinkToHandler ();
  }

  return result;
}

void oahSubGroup::appendSubGroupToElementsList (
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Appending subgroup \"" <<
      fSubGroupHeader <<
      "\" in the elements list in handler \"" <<
      handler->getHandlerHeader () <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append subgroup to elements list
  handler->
    appendElementToElementsList (this);

  fUpLinkToGroup->
    setUpLinkToHandler (handler);

  // register the OAH atoms in the handler
  for (
    std::list <S_oahAtom>::const_iterator
      i = fSubGroupAtomsList.begin ();
    i != fSubGroupAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);
    // register atom in the handler elements list
    atom->
      appendAtomToElementsList (
        handler);
  } // for

  --gIndenter;
}

void oahSubGroup::registerNamesInSubGroupToTheNamesToElementsMap (
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Registering the names in subgroup \"" <<
      fSubGroupHeader <<
      "\" in the names to elements map in handler \"" <<
      handler->getHandlerHeader () <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // register this subgroup's names in this handler
  handler->
    registerElementNamesInHandler (
      this);

  // register this subgroup's names in handler
  for (
    std::list <S_oahAtom>::const_iterator
      i = fSubGroupAtomsList.begin ();
    i != fSubGroupAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);

    // register atom's names in handler
    handler->
      registerElementNamesInHandler (
        atom);
  } // for

  --gIndenter;
}

void oahSubGroup::appendAtomToSubGroup (
  const S_oahAtom& atom)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    atom != nullptr,
    "atom is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // is atom already present in this subgroup?
  std::string
    atomLongName =
      atom->getLongName ();

  if (fetchOptionByNameInSubGroup (atomLongName)) {
    // prefixName is already known in the map
    std::stringstream ss;

    ss <<
      "atom \"" <<
      atomLongName <<
      "\" is already known in OAH subgroup \"" <<
      fSubGroupHeader <<
      "\"";

    oahError (ss.str ());
  }

  // append atom to atoms list
  fSubGroupAtomsList.push_back (
    atom);

  // set atom subgroup upLink
  atom->
    setUpLinkToSubGroup (this);
}

S_oahElement oahSubGroup::fetchOptionByNameInSubGroup (
  const std::string& name)
{
  S_oahElement result;

  for (
    std::list <S_oahAtom>::const_iterator
      i = fSubGroupAtomsList.begin ();
    i != fSubGroupAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);

    // has atom this name?
    result =
      atom->thisElementIfItHasName (name);

    if (result) {
      break;
    }
  } // for

  return result;
}

void oahSubGroup::applySubGroup (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Applying element " <<
      fetchNames () <<
      ", which is a oahSubGroup";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  // print the help
  gLog <<
    "--- Help for subgroup \"" <<
    fSubGroupHeader <<
    "\" in group \"" <<
    fUpLinkToGroup->getGroupHeader () <<
    "\" ---" <<
    std::endl;

  ++gIndenter;

  fUpLinkToGroup->
    printGroupAndSubGroupHelp (
      os,
      this);

  --gIndenter;

  gIndenter.setIndentation (saveIndent);
}

Bool oahSubGroup::fetchElementsMatchingStringInSubGroup (
	const std::string&       lowerCaseString,
	std::list <S_oahElement>& foundElementsList,
	std::ostream&            os) const
{
  switch (fElementVisibilityKind) { // JMI remove???
    case oahElementVisibilityKind::kElementVisibilityNone:
    case oahElementVisibilityKind::kElementVisibilityWhole:
    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      // don't use this groups's data to find the std::string
      return false;
      break;
  } // switch

  Bool subGroupNameMatches = // unused, side effect is to enrich foundElementsList
//     true;
    fetchElementsMatchingString (
      lowerCaseString,
      foundElementsList,
      os);

  // do this groups's subgroups match?
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    for (S_oahAtom atom : fSubGroupAtomsList) {
      Bool atomMatches =
        atom->
          atomMatchesString (
            lowerCaseString,
            os);
    } // for

    --gIndenter;
  }

  return subGroupNameMatches;
}

void oahSubGroup::checkSubGroupOptionsConsistency ()
{}

void oahSubGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahSubGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahSubGroup>*
    p =
      dynamic_cast<visitor<S_oahSubGroup>*> (v)) {
        S_oahSubGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahSubGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahSubGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahSubGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahSubGroup>*
    p =
      dynamic_cast<visitor<S_oahSubGroup>*> (v)) {
        S_oahSubGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahSubGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahSubGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahSubGroup::browseData ()" <<
      ", " << fSubGroupHeader;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // browse the atoms
  if (fSubGroupAtomsList.size ()) {
    for (
      std::list <S_oahAtom>::const_iterator i = fSubGroupAtomsList.begin ();
      i != fSubGroupAtomsList.end ();
      ++i
    ) {
      S_oahAtom atom = (*i);

      // browse the atom
      /* JMI ???
      gLog <<
        ".\\\" ==> Going to browse atom" <<
        " '" << atom->fetchNames () << '\'' <<
        std::endl;
        */

      oahBrowser<oahAtom> browser (v);
      browser.browse (*(atom));
    } // for
  }
}

// std::string oahSubGroup::asString (std::ostream& os) const // JMI 0.9.67
// {
//   stringstream ss;
//
//   ss <<
//     "[SubGroup:" <<
//     ']';
//
//   return ss.str ();
// }

void oahSubGroup::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
   "SubGroup:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fElementVisibilityKind" << ": " <<
    fElementVisibilityKind <<
    std::endl << std::endl;

  os <<
    "fSubGroupAtomsList (" <<
    mfSingularOrPlural (
      fSubGroupAtomsList.size (), "atom",  "atoms") <<
    "):" <<
    std::endl;

  if (fSubGroupAtomsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahSubGroup::printSummary (std::ostream& os) const
{
  os <<
    "SubGroup: " <<
    fetchNames () <<
    std::endl;

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    for (S_oahAtom atom : fSubGroupAtomsList) {
      // print a summary of the atom
      atom->printSummary (os);
    } // for

    --gIndenter;
  }
}

void oahSubGroup::underlineSubGroupHeader (std::ostream& os) const
{
  /* JMI ???
  for (size_t i = 0; i < fSubGroupHeader.size (); ++i) {
    os << "-";
  } // for
  os << std::endl;
  */
  os << "--------------------------" << std::endl;
}

void oahSubGroup::printSubGroupHeader (std::ostream& os) const
{
  // print the header and option names
  os <<
    fSubGroupHeader;

  os <<
    ' ' <<
    fetchNamesBetweenParentheses ();

  switch (fElementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityNone:
      break;

    case oahElementVisibilityKind::kElementVisibilityWhole:
      os <<
        ":";
      break;

    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      os <<
        " (use this option to show this subgroup)";
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      break;
  } // switch

  os << std::endl;
}

void oahSubGroup::printSubGroupHeaderWithHeaderWidth (
  std::ostream& os,
  int           subGroupHeaderWidth) const
{
  // print the header and option names
  os << std::left <<
    std::setw (subGroupHeaderWidth) <<
    fSubGroupHeader <<
    ' ' <<
    fetchNamesBetweenParentheses ();

  switch (fElementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityNone:
      break;

    case oahElementVisibilityKind::kElementVisibilityWhole:
      os <<
        ":";
      break;

    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      os <<
        " (use this option to show this subgroup)";
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      break;
  } // switch

  os << std::endl;

}

void oahSubGroup::printHelp (std::ostream& os) const
{
  // print the header and option names
  printSubGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    --gIndenter;

    os << std::endl;
  }

  // print the options atoms if relevant
  switch (fElementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityNone:
      break;

    case oahElementVisibilityKind::kElementVisibilityWhole:
      if (fSubGroupAtomsList.size ()) {
        ++gIndenter;

        for (S_oahAtom atom : fSubGroupAtomsList) {
          // print the atom help unless it is not visible
          switch (atom->getElementVisibilityKind ()) {
            case oahElementVisibilityKind::kElementVisibilityWhole:
            case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
              atom->printHelp (os);
              break;

            case oahElementVisibilityKind::kElementVisibilityNone:
            case oahElementVisibilityKind::kElementVisibilityHidden:
              break;
          } // switch
        } // for

        --gIndenter;
      }
      break;

    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      break;
  } // switch
}

void oahSubGroup::printHelpWithHeaderWidth (
  std::ostream& os,
  int           subGroupHeaderWidth) const
{
  // print the header and option names
  printSubGroupHeaderWithHeaderWidth (
    os,
    subGroupHeaderWidth);

  // underline the header if the group header is not written
  if (! fUpLinkToGroup->getGroupHeaderIsToBeWritten ()) {
    underlineSubGroupHeader (os);
  }

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    --gIndenter;

    os << std::endl;
  }

  // print the options atoms if relevant
  switch (fElementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityWhole:
      if (fSubGroupAtomsList.size ()) {
        ++gIndenter;

        for (S_oahAtom atom : fSubGroupAtomsList) {
          // print the atom help unless it is not visible
          switch (atom->getElementVisibilityKind ()) {
            case oahElementVisibilityKind::kElementVisibilityWhole:
            case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
              atom->printHelp (os);
              break;

            case oahElementVisibilityKind::kElementVisibilityNone:
            case oahElementVisibilityKind::kElementVisibilityHidden:
              break;
          } // switch
        } // for

        --gIndenter;
      }
      break;

    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      break;

    case oahElementVisibilityKind::kElementVisibilityNone:
    case oahElementVisibilityKind::kElementVisibilityHidden:
      break;
  } // switch
}

void oahSubGroup::printSubGroupHelp (std::ostream& os) const
{
  // print the header and option names
  printSubGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    --gIndenter;
  }

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    for (S_oahAtom atom : fSubGroupAtomsList) {
      // print the atom help
      atom->printHelp (os);
    } // for

    --gIndenter;
  }
}

void oahSubGroup::printOptionsSummary (
  std::ostream& os) const
{
  // fetch maximum subgroups help headers size
  size_t maximumSubGroupsHelpHeadersSize =
    getUpLinkToGroup ()->
      getUpLinkToHandler ()->
        getMaximumSubGroupsHeadersSize ();

    /* JMI
  // fetch maximum short name width
  int maximumShortNameWidth =
    getUpLinkToGroup ()->
      getUpLinkToHandler ()->
        getMaximumShortNameWidth ();
*/

  // print the summary
  os << std::left <<
    std::setw (maximumSubGroupsHelpHeadersSize) <<
    fSubGroupHeader <<
    ' ' <<
    /* JMI
    fetchNamesInColumnsBetweenParentheses (
      maximumShortNameWidth);
      */
    fetchNamesBetweenParentheses ();

  switch (fElementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityNone:
      break;

    case oahElementVisibilityKind::kElementVisibilityWhole:
      break;

    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      os <<
        " (use this option to show this subgroup)";
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      break;
  } // switch

  os << std::endl;

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    --gIndenter;
  }
}

void oahSubGroup::printSubGroupSpecificHelpOrOptionsSummary (
  std::ostream&      os,
  const S_oahSubGroup& subGroup) const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    os <<
      "oahSubGroup::printSubGroupSpecificHelpOrOptionsSummary" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // print only the summary if this is not the desired subgroup,
  // otherwise print the regular help
  if (subGroup == this) {
    os << std::endl;
    printSubGroupSpecificHelpOrOptionsSummary (
      os,
      subGroup);
  }
  else {
    printOptionsSummary (os); // JMI ???
  }
 }

void oahSubGroup::printSubGroupAndAtomHelp (
  std::ostream&  os,
  const S_oahAtom& targetAtom) const
{
  // print the subgroup atoms
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    for (S_oahAtom atom : fSubGroupAtomsList) {
      if (atom == targetAtom) {
        // print the target atom's help
        // target options atom's help
        atom->printHelp (os);
      }
    } // for

    --gIndenter;
  }
}

// const std::string oahSubGroup::containingFindableElementAsString () const
// {
//   std::stringstream ss;
//
//   ss << std::left <<
//     "Subgroup in group \"" <<
//     fUpLinkToGroup->getGroupHeader () <<
//     "\"";
//
//   return ss.str ();
// }

Bool oahSubGroup::findStringInSubGroup (
  const std::string&               lowerCaseString,
  std::list <S_oahFindStringMatch>& foundMatchesList,
  std::ostream&                    os) const
{
  Bool result;

  Bool subGroupNameMatches = // unused, side effect is to enrich foundMatchesList
    findStringInFindableElement (
      lowerCaseString,
      foundMatchesList,
      os);

  // do this subgroups's atoms match?
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    for (S_oahAtom atom : fSubGroupAtomsList) {
      Bool atomMatches =
        atom->
          findStringInAtom (
            lowerCaseString,
            foundMatchesList,
            os);
    } // for

    --gIndenter;
  }

  return result;
}

void oahSubGroup::printSubGroupOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  if (fNumberOfUserChoseAtomsInThisSubGroup > 0) { // JMI 0.9.66
    // print the header
    os <<
      fSubGroupHeader <<
      ' ' <<
      fetchNamesBetweenParentheses () <<
      ", " <<
      mfSingularOrPlural (
        fNumberOfUserChoseAtomsInThisSubGroup, "atom selected", "atoms selected") <<
    ":" <<
    std::endl;

    // underline the options subgroup header
    // JMI  underlineSubGroupHeader (os);
  }

  // print the subgroup atoms values
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    for (S_oahAtom atom : fSubGroupAtomsList) {
      if (
        // boolean atom?

        S_oahBooleanAtom
          booleanAtom =
            dynamic_cast<oahBooleanAtom*>(&(*atom))
      ) {
        // print the atom value if the variable has been selected option by user
        if (booleanAtom->getSelected ()) {
          atom->
            displayAtomWithVariableOptionsValues (
              os,
              valueFieldWidth);
        }
      }

      else if (
        // atomImplicitlyStoringAValue?

        S_oahAtomImplicitlySettingAVariable
          atomImplicitlyStoringAValue =
            dynamic_cast<oahAtomImplicitlySettingAVariable*>(&(*atom))
      ) {
        // print the atom value if the variable has been selected option by user
        if (atomImplicitlyStoringAValue->getSelected ()) {
          atom->
            displayAtomWithVariableOptionsValues (
              os,
              valueFieldWidth);
        }
      }

      else if (
        // oahAtomStoringAValue?

        S_oahAtomStoringAValue
          atomStoringAValue =
            dynamic_cast<oahAtomStoringAValue*>(&(*atom))
      ) {
        // print the atom value if the variable has been selected option by user
        if (atomStoringAValue->getSelected ()) {
          atom->
            displayAtomWithVariableOptionsValues (
              os,
              valueFieldWidth);
        }
      }
    } // for

    --gIndenter;
  }
}

void oahSubGroup::printSubGroupOptionsValuesAll (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // print the header
  os <<
    fSubGroupHeader <<
    ' ' <<
    fetchNamesBetweenParentheses ();
  if (fNumberOfUserChoseAtomsInThisSubGroup > 0) {
    os <<
      ", " <<
      mfSingularOrPlural (
        fNumberOfUserChoseAtomsInThisSubGroup, "atom selected", "atoms selected");
  }
  os <<
    ":" <<
    std::endl;

  // underline the options subgroup header
// JMI  underlineSubGroupHeader (os);

  // print the subgroup atoms values
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    for (S_oahAtom atom : fSubGroupAtomsList) {
      // print the atom values
      atom->
        displayAtomWithVariableOptionsValues (
          os,
          valueFieldWidth);
    } // for

    --gIndenter;
  }
}

std::ostream& operator << (std::ostream& os, const S_oahSubGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahGroup oahGroup::create (
  const std::string&       header,
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementVisibilityKind optionVisibilityKind)
{
  oahGroup* obj = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind);
  assert (obj != nullptr);
  return obj;
}

S_oahGroup oahGroup::create (
  const std::string&       header,
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementVisibilityKind optionVisibilityKind,
  const S_oahHandler&      groupUpLinkToHandler)
{
  oahGroup* obj = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      groupUpLinkToHandler);
  assert (obj != nullptr);
  return obj;
}

S_oahGroup oahGroup::createWithNames (
  const std::string&       header,
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementVisibilityKind optionVisibilityKind)
{
  oahGroup* obj = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind);
  assert (obj != nullptr);
  return obj;
}

S_oahGroup oahGroup::createWithNames (
  const std::string&       header,
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementVisibilityKind optionVisibilityKind,
  const S_oahHandler&      groupUpLinkToHandler)
{
  oahGroup* obj = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      groupUpLinkToHandler);
  assert (obj != nullptr);
  return obj;
}

oahGroup::oahGroup (
  const std::string&       header,
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementVisibilityKind optionVisibilityKind)
  : oahElement (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueLess,
      optionVisibilityKind)
{
  fGroupHeader = header;

  fGroupHeaderIsToBeWritten = true; // default value

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  fNumberOfUserChoseAtomsInThisGroup = 0;
}

oahGroup::oahGroup (
  const std::string&       header,
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementVisibilityKind optionVisibilityKind,
  const S_oahHandler&      groupUpLinkToHandler)
  : oahElement (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueLess,
      optionVisibilityKind)
{
  fUpLinkToHandler = groupUpLinkToHandler;

  fGroupHeader = header;

  fGroupHeaderIsToBeWritten = true; // default value

  fNumberOfUserChoseAtomsInThisGroup = 0;
}

oahGroup::~oahGroup ()
{}

void oahGroup::setUpLinkToHandler (
  const S_oahHandler& upLinkToHandler)
{
  fUpLinkToHandler = upLinkToHandler;
}

void oahGroup::appendGroupToElementsList (
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Appending group \"" <<
      fGroupHeader <<
      "\" to elements list in handler \"" <<
      handler->getHandlerHeader () <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // append this group to the elements list
  handler->
    appendElementToElementsList (this);

  // set group handler upLink
  fUpLinkToHandler = handler;

  // append subgroups to elements list
  for (
    std::list <S_oahSubGroup>::const_iterator
      i = fGroupSubGroupsList.begin ();
    i != fGroupSubGroupsList.end ();
    ++i
  ) {
    S_oahSubGroup subGroup = (*i);

    // append subgroup to elements list
    subGroup->
      appendSubGroupToElementsList (
        handler);
  } // for

  --gIndenter;
}

void oahGroup::registerNamesInGroupToTheNamesToElementsMap (
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Registering the names in group \"" <<
      fGroupHeader <<
      "\" in the names to elements map in handler \"" <<
      handler->getHandlerHeader () <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // register this group' names in handler
  handler->
    registerElementNamesInHandler (
      this);

  // append subgroups to elements list
  for (
    std::list <S_oahSubGroup>::const_iterator
      i = fGroupSubGroupsList.begin ();
    i != fGroupSubGroupsList.end ();
    ++i
  ) {
    S_oahSubGroup subGroup = (*i);

    // append subgroup to elements list
    subGroup->
      registerNamesInSubGroupToTheNamesToElementsMap (
        handler);
  } // for

  --gIndenter;
}

void  oahGroup::appendSubGroupToGroup (
  const S_oahSubGroup& subGroup)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    subGroup != nullptr,
    "subGroup is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // append options subgroup
  fGroupSubGroupsList.push_back (
    subGroup);

  // set options subgroup group upLink
  subGroup->
    setUpLinkToGroup (this);
}

S_oahElement oahGroup::fetchOptionByNameInGroup (
  const std::string& name)
{
  S_oahElement result;

  for (
    std::list <S_oahSubGroup>::const_iterator
      i = fGroupSubGroupsList.begin ();
    i != fGroupSubGroupsList.end ();
    ++i
  ) {
    // search name in the options group
    result =
      (*i)->fetchOptionByNameInSubGroup (name);

    if (result) {
      break;
    }
  } // for

  return result;
}

void oahGroup::applyGroup (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Applying element '" <<
      fetchNames () <<
      "', which is a oahGroup";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  // print the help
  gLog <<
    "--- Help for group \"" <<
    fGroupHeader <<
    "\" ---" <<
    std::endl;

  ++gIndenter;

  printHelp (os);

  --gIndenter;

  gIndenter.setIndentation (saveIndent);
}

void oahGroup::handleAtomValue (
  std::ostream&      os,
  const S_oahAtom&   atom,
  const std::string& theString)
{
  os <<
    "Handling option name atom '" <<
    atom <<
    "' with value \"" <<
    theString <<
    "\" is not handled" <<
    std::endl;
}

S_mfcMultiComponent oahGroup::fetchGroupHandlerMultiComponent () const
{
  return
    fUpLinkToHandler->
      getHandlerMultiComponent ();
}

void oahGroup::enforceGroupQuietness ()
{}

void oahGroup::checkGroupOptionsConsistency ()
{}

void oahGroup::checkGroupSubGroupsOptionsConsistency ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Checking the consistency of OAH group \"" <<
      fGroupHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // check the subgroups options consistency
  if (fGroupSubGroupsList.size ()) {
    for (
      std::list <S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
      i != fGroupSubGroupsList.end ();
      ++i
    ) {
      S_oahSubGroup subgroup = (*i);

      // check the subgroup
      subgroup->
        checkSubGroupOptionsConsistency ();
    } // for
  }

  --gIndenter;

  // check the group's own consistency
  this->checkGroupOptionsConsistency ();
}

void oahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahGroup>*
    p =
      dynamic_cast<visitor<S_oahGroup>*> (v)) {
        S_oahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahGroup>*
    p =
      dynamic_cast<visitor<S_oahGroup>*> (v)) {
        S_oahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahGroup::browseData ()" <<
      ", " << fGroupHeader;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // browse the subGroups
  if (fGroupSubGroupsList.size ()) {
    for (
      std::list <S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
      i != fGroupSubGroupsList.end ();
      ++i
    ) {
      S_oahSubGroup subGroup = (*i);

      // browse the subGroup
      oahBrowser<oahSubGroup> browser (v);
      browser.browse (*(subGroup));
    } // for
  }
}

// std::string oahGroup::asString (std::ostream& os) const // JMI 0.9.67
// {
//   stringstream ss;
//
//   ss <<
//     "[Group:" <<
//     ']';
//
//   return ss.str ();
// }

void oahGroup::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "Group:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    "SubGroupsList (" <<
    mfSingularOrPlural (
      fGroupSubGroupsList.size (), "element",  "elements") <<
    "):" <<
    std::endl;

  if (fGroupSubGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahGroup::printSummary (std::ostream& os) const
{
  os <<
    "Group: " <<
    fetchNames () <<
    std::endl;

  if (fGroupSubGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print a summary of the options subgroup
      subGroup->printSummary (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::underlineGroupHeader (std::ostream& os) const
{
  /* JMI
  for (size_t i = 0; i < fGroupHeader.size (); ++i) {
    os << "-";
  } // for
  os << std::endl;
  */
  os << "--------------------------" << std::endl;
}

void oahGroup::printGroupHeader (std::ostream& os) const
{
  // print the header and option names
  os <<
    fGroupHeader <<
    ' ' <<
    fetchNamesBetweenParentheses ();

  switch (fElementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityNone:
      break;

    case oahElementVisibilityKind::kElementVisibilityWhole:
      os <<
        ":";
      break;

    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      os <<
        " (use this option to show this group)";
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      break;
  } // switch

  os << std::endl;
}

void oahGroup::printHelp (std::ostream& os) const
{
  if (fGroupHeaderIsToBeWritten) {
    // print the header and option names
    printGroupHeader (os);

    // print the description if any
    if (fDescription.size ()) {
      ++gIndenter;

      gIndenter.indentMultiLineString (
        fDescription,
        os);

      --gIndenter;
    }

    // underline the options group header
    underlineGroupHeader (os);
  }

  // print the options subgroups
  if (fGroupSubGroupsList.size ()) {
    // compute the maximun sub group header length
    size_t maximumSubGroupHeaderLength = 0;

    for (
      std::list <S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
      i != fGroupSubGroupsList.end ();
      ++i
    ) {
      std::string subGroupHeader = (*i)->getSubGroupHeader ();
      size_t subGroupHeaderSize = subGroupHeader.size ();

      if (subGroupHeaderSize > maximumSubGroupHeaderLength) {
        maximumSubGroupHeaderLength = subGroupHeaderSize;
      }
    } // for

    maximumSubGroupHeaderLength += 3; // to have some more spaces

    if (fGroupHeaderIsToBeWritten) {
      ++gIndenter;
    }

    for (S_oahSubGroup subGroup : fGroupSubGroupsList) {
      // print the options subgroup help
      subGroup->
        printHelpWithHeaderWidth (
          os,
          maximumSubGroupHeaderLength);
    } // for

    if (fGroupHeaderIsToBeWritten) {
      --gIndenter;
    }
  }
}

void oahGroup::printGroupAndSubGroupHelp (
  std::ostream&      os,
  const S_oahSubGroup& targetSubGroup) const
{
  // print the header and option names
  os <<
    fGroupHeader <<
    ' ' <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    std::endl;

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    --gIndenter;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the target options subgroup
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    for (S_oahSubGroup subGroup : fGroupSubGroupsList) {
      if (subGroup == targetSubGroup) {
        // print the target options subgroup help
        subGroup->printSubGroupHelp (os);
      }
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupAndSubGroupAndAtomHelp (
  std::ostream&        os,
  const S_oahSubGroup& targetSubGroup,
  const S_oahAtom&     targetAtom) const
{
  // print the target options subgroup
  if (fGroupSubGroupsList.size ()) {
    for (S_oahSubGroup subGroup : fGroupSubGroupsList) {
      if (subGroup == targetSubGroup) {
        // print the target options subgroup's
        // target options targetAtom's help
        subGroup->
          printSubGroupAndAtomHelp (
            os,
            targetAtom);
      }
    } // for
  }
}

void oahGroup::printOptionsSummary (std::ostream& os) const
{
  // the description is the header of the information
  os <<
    fGroupHeader <<
    ' ' <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    std::endl;

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    --gIndenter;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    for (S_oahSubGroup subGroup : fGroupSubGroupsList) {
      // print the options subgroup description
      subGroup->
        printOptionsSummary (os);
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupAndSubGroupSpecificHelp (
  std::ostream&        os,
  const S_oahSubGroup& subGroup) const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    os <<
      "oahGroup::printGroupAndSubGroupSpecificHelp" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fElementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityNone:
    case oahElementVisibilityKind::kElementVisibilityWhole:
    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      // don't print specific help for this group
      return;
      break;
  } // switch

  if (fGroupHeaderIsToBeWritten) {
    // the description is the header of the information
    os <<
      fGroupHeader <<
      ' ' <<
      fetchNamesBetweenParentheses () <<
      ":" <<
      std::endl;

    // print the description if any
    if (fDescription.size ()) {
      ++gIndenter;

      gIndenter.indentMultiLineString (
        fDescription,
        os);

      --gIndenter;
    }

    // underline the options group header
    underlineGroupHeader (os);
  }

  // print the options subgroups
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    std::list <S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print the options subgroup specific subgroup help
      subGroup->
        printSubGroupSpecificHelpOrOptionsSummary (
          os,
          subGroup);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  if (fNumberOfUserChoseAtomsInThisGroup > 0) { // JMI 0.9.66
    // print the header
    os <<
      fGroupHeader <<
      ' ' <<
      fetchNamesBetweenParentheses () <<
      ", " <<
      mfSingularOrPlural (
        fNumberOfUserChoseAtomsInThisGroup, "atom selected", "atoms selected") <<
    ":" <<
    std::endl;

    // underline the options group header
    underlineGroupHeader (os);
  }

  // print the options subgroups values
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    Bool doPrintOptionsValues;

    std::list <S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print the options subgroup values
      doPrintOptionsValues =
        subGroup->
          getNumberOfUserChoseAtomsInThisSubGroup () > 0;

      if (doPrintOptionsValues) {
        subGroup->
          printSubGroupOptionsValues (
            os,
            valueFieldWidth);
      }

      if (++i == iEnd) break;

      if (doPrintOptionsValues) {
        os << std::endl;
      }
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupOptionsValuesAll (
  std::ostream& os,
  int           valueFieldWidth) const
{
  // print the header
  os <<
    fGroupHeader <<
    ' ' <<
    fetchNamesBetweenParentheses ();
  if (fNumberOfUserChoseAtomsInThisGroup > 0) {
    os <<
      ", " <<
      mfSingularOrPlural (
        fNumberOfUserChoseAtomsInThisGroup, "atom selected", "atoms selected");
  }
  os <<
    ":" <<
    std::endl;

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups values
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    std::list <S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print the options subgroup values
      subGroup->
        printSubGroupOptionsValuesAll (
          os,
          valueFieldWidth);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
}

// const std::string oahGroup::containingFindableElementAsString () const
// {
//   std::stringstream ss;
//
//   ss << std::left <<
//     "Group in handler \"" <<
//     fUpLinkToHandler->getHandlerHeader () <<
//     "\"";
//
//   return ss.str ();
// }

void oahGroup::findStringInGroup (
  const std::string&               lowerCaseString,
  std::list <S_oahFindStringMatch>& foundMatchesList,
  std::ostream&                    os) const
{
  switch (fElementVisibilityKind) { // JMI remove???
    case oahElementVisibilityKind::kElementVisibilityNone:
    case oahElementVisibilityKind::kElementVisibilityWhole:
    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      // don't use this groups's data to find the std::string
      return;
      break;
  } // switch

  Bool groupNameMatches = // unused, side effect is to enrich foundMatchesList
    findStringInFindableElement (
      lowerCaseString,
      foundMatchesList,
      os);

  // do this groups's subgroups match?
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    for (S_oahSubGroup subGroup : fGroupSubGroupsList) {
      Bool subGroupMatches =
        subGroup->
          findStringInSubGroup (
            lowerCaseString,
            foundMatchesList,
            os);
    } // for

    --gIndenter;
  }
}

Bool oahGroup::fetchElementsMatchingStringInGroup (
	const std::string&       lowerCaseString,
	std::list <S_oahElement>& foundElementsList,
	std::ostream&            os) const
{
  switch (fElementVisibilityKind) { // JMI remove???
    case oahElementVisibilityKind::kElementVisibilityNone:
    case oahElementVisibilityKind::kElementVisibilityWhole:
    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      // don't use this groups's data to find the std::string
      return false;
      break;
  } // switch

  Bool groupNameMatches = // unused, side effect is to enrich foundElementsList
//     true;
    fetchElementsMatchingString (
      lowerCaseString,
      foundElementsList,
      os);

  // do this groups's subgroups match?
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    for (S_oahSubGroup subGroup : fGroupSubGroupsList) {
      Bool subGroupMatches = true; // JMI 0.9.71 OAH_OAH
//         subGroup->
//           findStringInSubGroup (
//             lowerCaseString,
//             foundElementsList,
//             os);
    } // for

    --gIndenter;
  }

  return groupNameMatches;
}

std::ostream& operator << (std::ostream& os, const S_oahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahHandler pPrivateCurrentOahHandler;

S_oahHandler getCurrentOahHandler ()
{
  return pPrivateCurrentOahHandler;
}

S_oahHandler oahHandler::sCurrentOahHandler;

//______________________________________________________________________________
int oahHandler::sIncludeFilesMaximumLevel = 10;

// int            getIncludeFilesMaximumLevel ();

/* this class is purely virtual
S_oahHandler oahHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader,
  const std::string& handlerDescription,
  const std::string& handlerUsage)
{
  oahHandler* obj = new
    oahHandler (
      serviceName,
      handlerHeader,
      handlerDescription,
      handlerUsage);
  assert (obj != nullptr);
  return obj;
}
*/

oahHandler::oahHandler (
  const std::string& serviceName,
  const std::string& handlerHeader,
  const std::string& handlerDescription,
  const std::string& handlerUsage)
{
  fHandlerServiceName = serviceName;

  fHandlerHeader = handlerHeader;

  fHandlerDescription = handlerDescription;

  fHandlerUsage = handlerUsage;

  initializeHandler ();

  // set current OAH handler
  pPrivateCurrentOahHandler = this;

  sCurrentOahHandler = this;
}

void oahHandler::initializeHandler ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing handler \"" <<
      fHandlerHeader <<
      "\" for \"" <<
      fHandlerServiceName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // DON'T create the fHandlerMultiComponent,
  // this will be done in the sub-classes' initializeHandlerMultiComponent ()

  // launch command
  fLaunchCommandAsSupplied =
    fHandlerServiceName;
  fLaunchCommandWithLongOptionsNames =
    fHandlerServiceName;
  fLaunchCommandWithLongOptionsNames =
    fHandlerServiceName;

  // arguments handling
//   fNowEverythingIsAnArgument = false; superflous JMI

  // handler used thru...?
  fHandlerUsedThruKind = oahHandlerUsedThruKind::kHandlerUsedThruUnknown;

  // sizes and widths
  fMaximumSubGroupsHeadersSize = 1;

  fMaximumShortNameWidth = 1;
  fMaximumLongNameWidth = 1;

  fMaximumVariableNameWidth = 0;

  fIncludeFilesCurrentLevel = 0;
}

oahHandler::~oahHandler ()
{}

S_oahPrefix oahHandler::fetchNameInHandlerPrefixesMap (
  const std::string& name) const
{
  S_oahPrefix result;

  // is name known in prefixes map?
  std::map <std::string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        name);

  if (it != fHandlerPrefixesMap.end ()) {
    // yes, name is known in the map
    result = (*it).second;
  }

  return result;
}

S_oahElement oahHandler::fetchNameInNamesToElementsMap (
  const std::string& name) const
{
  S_oahElement result;

  // is name known in the names to elements map?
  std::map <std::string, S_oahElement>::const_iterator
    it =
      fHandlerNamesToElementsMap.find (
        name);

  if (it != fHandlerNamesToElementsMap.end ()) {
    // yes, name is known in the map
    result = (*it).second;
  }

  return result;
}

void oahHandler::registerElementNamesInHandler (
  const S_oahElement& element)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Registering the names of " <<
      element->fetchNamesBetweenQuotes () <<
      " in handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  std::string
    elementShortName =
      element->getShortName (),
    elementLongName =
      element->getLongName ();

  size_t
    elementShortNameSize =
      elementShortName.size (),
    elementLongNameSize =
      elementLongName.size ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (elementLongNameSize == 0) {
    std::stringstream ss;

    ss <<
      "element long name is empty";

    oahError (ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  if (elementShortNameSize == 0 && elementLongNameSize == 0) {
    std::stringstream ss;

    ss <<
      "element long name and short name are both empty";

    oahError (ss.str ());
  }

  if (elementShortName == elementLongName) {
    std::stringstream ss;

    ss <<
      "element short name \"" << elementLongName << "\"" <<
      " is the same as the long name for the same";

    oahError (ss.str ());
  }

  if (elementLongNameSize == 1) {
    std::stringstream ss;

    ss <<
      "element long name \"" << elementLongName << "\"" <<
      " has only one character";

    oahWarning (ss.str ());
  }

  if (elementShortNameSize > elementLongNameSize) {
    std::stringstream ss;

    ss <<
      "element short name \"" << elementShortName << "\"" <<
      " is longer than long name \"" << elementLongName << "\"" <<
      " for the same";

    oahError (ss.str ());
  }

  for (std::pair <std::string, S_oahElement> thePair : fHandlerNamesToElementsMap) {
    std::string  name = thePair.first;
    S_oahElement element = thePair.second;

//     gLog << // JMI 0.9.65
//       "--> name: " << name <<
//       std::endl <<
//       "--> elementLongName: " << elementLongName <<
//       std::endl <<
//       "--> elementShortName: " << elementShortName <<
//       std::endl;

    // is elementLongName already in the elements names map?
    if (name == elementLongName) {
      std::stringstream ss;

      ss <<
        "element long name \"" << elementLongName << "\"" <<
          " for element short name \"" << elementShortName << "\"" <<
          " is already defined for " <<
           element->fetchNamesBetweenQuotes () <<
          " in handler \"" <<
          fHandlerHeader <<
          "\"";

//       gLog << ss.str () << std::endl ; abort (); // JMI TEST 0.9.65

//       oahInternalError (ss.str ()); // JMI TEMP 0.9.65
//       oahInternalWarning (ss.str ());
    }

    // is elementShortName already in the elements names map?
    if (name == elementShortName) {
      if (elementShortName.size ()) {
        std::stringstream ss;

        ss <<
          "element short name \"" << elementShortName << "\"" <<
          " for element long name \"" << elementLongName << "\"" <<
          " is already defined for " <<
           element->fetchNamesBetweenQuotes () <<
          " in handler \"" <<
          fHandlerHeader <<
          "\"";

//       gLog << ss.str () << std::endl ; abort (); // JMI TEST 0.9.65

//         oahInternalError (ss.str ()); // JMI TEMP 0.9.65
//         oahInternalWarning (ss.str ());
      }
    }
  } // for

  if (elementLongNameSize == 1) {
    // register element's long name in single character names set
    fSingleCharacterNamesSet.insert (
      elementLongName);
  }
  if (elementShortNameSize == 1) {
    // register element's short name in single character names set
    fSingleCharacterNamesSet.insert (
      elementShortName);
  }

  // register element's names in names to elements map
  if (elementLongNameSize) {
    fHandlerNamesToElementsMap [elementLongName] = element;

    if (elementLongNameSize > fMaximumLongNameWidth) {
      fMaximumLongNameWidth = elementLongNameSize;
    }
  }

  if (elementShortNameSize) {
    fHandlerNamesToElementsMap [elementShortName] = element;

    if (elementShortNameSize > fMaximumShortNameWidth) {
      fMaximumShortNameWidth = elementShortNameSize;
    }
  }

  // take element's display variable name length into account
  size_t
    elementVariableNameLength =
      element->
        fetchVariableNameLength ();

  if (
    elementVariableNameLength
      >
    fMaximumVariableNameWidth
  ) {
    fMaximumVariableNameWidth =
      elementVariableNameLength;
  }

  --gIndenter;
}

void oahHandler::registerNamesInHandlerToTheNamesToElementsMap ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Registering the names in handler \"" <<
      fHandlerHeader <<
      "\" in its names to elements map";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // check the handler groups options consistency
  if (fHandlerGroupsList.size ()) {
    for (
      std::list <S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
      i != fHandlerGroupsList.end ();
      ++i
    ) {
      S_oahGroup group = (*i);

      // check the group
      group->
        registerNamesInGroupToTheNamesToElementsMap (
          this);
    } // for
  }


#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    if (false){  // JMI
      displayNamesToElementsMapShort (gLog);

      std::stringstream ss;

      ss <<
        "All the names in handler \"" <<
        fHandlerHeader <<
        "\" have been registered in the names to elements map";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void oahHandler::appendElementToElementsList (
  const S_oahElement& element)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Registering element " <<
      element->fetchNamesBetweenQuotes () <<
      " in \"" <<
      fHandlerServiceName <<
      "\" handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // insert element into the registered elements list
  fHandlerElementsList.push_back (element);

  // compute subgroup's header size
  if (
    // subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*element))
    ) {

    std::string
      subHeader=
        subGroup-> getSubGroupHeader ();
    size_t
      subHeaderSize =
        subHeader.size ();

    // account for subgroup's header size
    if (subHeaderSize > fMaximumSubGroupsHeadersSize) {
      fMaximumSubGroupsHeadersSize =
        subHeaderSize;
    }
  }
}

void oahHandler::checkOptionsAndArgumentsConsistency ()
{
  size_t argumentsNumber =
    fOptionsAndArguments.
      getArgumentsVector ().size ();

  if (argumentsNumber > 0) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        fHandlerServiceName <<
        ":" <<
        std::endl;

      ++gIndenter;

//       gLog <<
//         fOptionsAndArguments;

      gLog << std::endl;

      --gIndenter;
    }
#endif // MF_TRACE_IS_ENABLED

    oahInternalWarning (
      "the arguments vector should be empty with options and arguments???");
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "There are no arguments to " <<
        fHandlerServiceName;

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

//______________________________________________________________________________
void oahHandler::createTheCommonPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the common prefixes in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // the 'help' prefixes
  // --------------------------------------

  registerPrefixInHandler (
    oahPrefix::create (
      "help", "help-",
      "'-help=abc,yz' is equivalent to '-help-abc, -help-yz'"));

  registerPrefixInHandler (
    oahPrefix::create (
      "h", "h",
      "'-h=abc,wxyz' is equivalent to '-habc, -hwxyz'"));

  // the 'display' prefixes
  // --------------------------------------

  registerPrefixInHandler (
    oahPrefix::create (
      "display", "display-",
      "'-display=abc,yz' is equivalent to '-display-abc, -display-yz'"));

  registerPrefixInHandler (
    oahPrefix::create (
      "d", "d",
      "'-d=abc,wxyz' is equivalent to '-dabc, -dwxyz'"));

  // the 'ignore' prefix
  // --------------------------------------

  registerPrefixInHandler (
    oahPrefix::create (
      "ignore", "ignore-",
      "'-ignore=abc,yz' is equivalent to '-ignore-abc, -ignore-yz'"));

  --gIndenter;
}

//______________________________________________________________________________
void oahHandler::checkNoInputSourceInArgumentsVector () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Checking that there is no input source in argument vector in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  const std::vector <std::string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  size_t argumentsNumber =
    argumentsVector.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    gEarlyOptions.getTraceEarlyOptions ()
      &&
    ! gEarlyOptions.getEarlyQuietOption ()
  ) {
    if (argumentsNumber > 0) {
      gLog <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        fHandlerServiceName <<
        ":" <<
        std::endl;

      ++gIndenter;

      for (size_t i = 0; i < argumentsNumber; ++i) {
       gLog <<
          i << " : FII " << argumentsVector.at (i) << // JMIJMIJMI
          std::endl;
      } // for

      gLog << std::endl;

      --gIndenter;
    }
    else {
      gLog <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      // fine, do nothing
      break;

    default:
      {
        std::stringstream ss;

        ss <<
          fHandlerServiceName <<
          " doesn't expect arguments, only options can be used";

        std::string message = ss.str ();

        gLog <<
          message <<
          std::endl;

        throw mfOahException (message);
      }
  } //  switch
}

//______________________________________________________________________________
void oahHandler::checkNoOrOneInputSourceInArgumentsVector () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Checking that there is  no or one input source in argument vector in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  const std::vector <std::string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  size_t argumentsNumber =
    argumentsVector.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    gEarlyOptions.getTraceEarlyOptions  ()
      &&
    ! gEarlyOptions.getEarlyQuietOption ()
  ) {
    if (argumentsNumber > 0) {
      gLog <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        fHandlerServiceName <<
        ":" <<
        std::endl;

      ++gIndenter;

      for (size_t i = 0; i < argumentsNumber; ++i) {
        gLog <<
          i << " : FUU " << argumentsVector.at (i) <<
            std::endl;
      } // for

      gLog << std::endl;

      --gIndenter;
    }
    else {
      gLog <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      // fine, use standard input
      gServiceRunData->setInputSourceName (
        "-");
      break;

    case 1:
      // register intput file name
      gServiceRunData->setInputSourceName (
        argumentsVector.at (0));
      break;

    default:
      {
        std::stringstream ss;

        ss <<
          "Several input file names are supplied to " <<
          fHandlerServiceName <<
          ", only the first one, \"" <<
          argumentsVector.at (0) <<
          "\", will be used "; //JMI

        std::string message = ss.str ();

        for (size_t i = 1; i < argumentsNumber; ++i) {
          gLog <<
            argumentsVector.at (i);

          if (i == argumentsNumber - 1) {
            gLog << " and ";
          }
          else {
            gLog << ", ";
          }
        } // for

        gLog <<
          message <<
          " ignored" <<
          std::endl << std::endl;

        // register intput file name
        gServiceRunData->setInputSourceName (
          argumentsVector.at (0));
      }
      break;
  } //  switch
}

//______________________________________________________________________________
void oahHandler::checkSingleInputSourceInArgumentsVector () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Checking that there is  single input source in argument vector in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  const std::vector <std::string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  size_t argumentsNumber =
    argumentsVector.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    gEarlyOptions.getTraceEarlyOptions  ()
      &&
    ! gEarlyOptions.getEarlyQuietOption ()
  ) {
    if (argumentsNumber > 0) {
      gLog <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        fHandlerServiceName <<
        ":" <<
        std::endl;

      ++gIndenter;

//       gLog <<
//         fOptionsAndArguments;

      gLog << std::endl;

      --gIndenter;
    }
    else {
      gLog <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      if (! fOahHandlerFoundAHelpOption) {
        std::string message =
          "Input file name or '-' for standard input expected";

        gLog <<
          message <<
          std::endl;

        throw mfOahException (message);
      }
      break;

    case 1:
      // register intput file name
      gServiceRunData->setInputSourceName (
        argumentsVector.at (0));
      break;

    default:
      if (! fOahHandlerFoundAHelpOption) {
        std::stringstream ss;

        ss <<
          "Several input file names are supplied to " <<
          fHandlerServiceName <<
          ", only one can be used";

        std::string message = ss.str ();

        gLog <<
          message <<
          std::endl;

        mfDisplayStringsVector (
          argumentsVector,
          "The arguments vector",
          gLog);

        throw mfOahException (message);
      }
      break;
  } //  switch
}

void oahHandler::checkOneArgumentInArgumentsVector () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Checking that there is  one argument in argument vector in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI 0.9.70
}

void oahHandler::checkOneArgumentAndNoOrOneInputSourceInArgumentsVector () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Checking that there is  no or one input source in argument vector in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  const std::vector <std::string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  size_t argumentsNumber =
    argumentsVector.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    gEarlyOptions.getTraceEarlyOptions  ()
      &&
    ! gEarlyOptions.getEarlyQuietOption ()
  ) {
    if (argumentsNumber > 0) {
      gLog <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        fHandlerServiceName <<
        ":" <<
        std::endl;

      ++gIndenter;

      for (size_t i = 0; i < argumentsNumber; ++i) {
        gLog <<
          i << " : FUU " << argumentsVector.at (i) <<
            std::endl;
      } // for

      gLog << std::endl;

      --gIndenter;
    }
    else {
      gLog <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // get input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 1:
      // fine, use standard input
      gServiceRunData->setInputSourceName (
        "-");
      break;

    case 2:
      // register intput file name
      gServiceRunData->setInputSourceName (
        argumentsVector.at (1));
      break;

    default:
      {
        std::stringstream ss;

        ss <<
          "Several input file names are supplied to " <<
          fHandlerServiceName <<
          ", only the first one, \"" <<
          argumentsVector.at (0) <<
          "\", will be used ";

        std::string message = ss.str ();

        for (size_t i = 1; i < argumentsNumber; ++i) {
          gLog <<
            argumentsVector.at (i);

          if (i == argumentsNumber - 1) {
            gLog << " and ";
          }
          else {
            gLog << ", ";
          }
        } // for

        gLog <<
          message <<
          " ignored" <<
          std::endl << std::endl;

        // register intput file name
        gServiceRunData->setInputSourceName (
          argumentsVector.at (0));
      }
      break;
  } //  switch

  // get stringMatcher expression
  std::string stringMatcherExpression = argumentsVector.at (0);

#ifdef MF_TRACE_IS_ENABLED
  if (
    gEarlyOptions.getTraceEarlyOptions  ()
      &&
    ! gEarlyOptions.getEarlyQuietOption ()
  ) {
    gLog <<
      "The stringMatcherExpression is " <<
      stringMatcherExpression <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahHandler::setOahHandlerFoundAHelpOption (
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Setting oahHandlerFoundAHelpOption in handler \"" <<
      fHandlerHeader <<
      "\", context: " <<
      context;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOahHandlerFoundAHelpOption = true;
}

std::string oahHandler::fetchOutputFileNameFromTheOptions () const
{
  return "*** no output file name ***"; // for MFSL 0.9.62
}

void oahHandler::checkHandlerOwnOptionsConsistency ()
{}

void oahHandler::checkHandlerOptionsConsistency ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gEarlyOptions.getEarlyTracePasses ()
      ||
    gEarlyOptions.getTraceEarlyOptionsDetails ()
  ) {
    std::stringstream ss;

    ss <<
      "oahHandler: checking the consistency of the options in \"" <<
      fHandlerServiceName <<
      "\" handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // check the handler groups options consistency
  if (fHandlerGroupsList.size ()) {
    for (
      std::list <S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
      i != fHandlerGroupsList.end ();
      ++i
    ) {
      S_oahGroup group = (*i);

      // check the group
      group->
        checkGroupSubGroupsOptionsConsistency ();
    } // for
  }

  --gIndenter;

  // the handler's own consistency
  this->checkHandlerOwnOptionsConsistency ();
}

void oahHandler::enforceHandlerQuietness ()
{}

void oahHandler::displayNamesToElementsMap ()
{
  gLog <<
    "The names to elements map contains " <<
    mfSingularOrPlural (
      fHandlerNamesToElementsMap.size (), "element", "elements");

  if (fHandlerNamesToElementsMap.size ()) {
    gLog << ":" << std::endl;
    ++gIndenter;

    for (
      std::map <std::string, S_oahElement>::iterator i =
        fHandlerNamesToElementsMap.begin ();
      i != fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      std::string name = (*i).first;
      S_oahElement element = (*i).second;

      gLog <<
        "\"" <<
        name <<
        "\":" <<
        std::endl;

      ++gIndenter;

      gLog <<
        element <<
        std::endl;

      --gIndenter;
    } // for

    --gIndenter;
  }
  else {
    gLog << std::endl;
  }
}

void oahHandler::displayNamesToElementsMapShort (std::ostream& os) const
{
  gLog <<
    "The names to elements map contains (short version) " <<
    mfSingularOrPlural (
      fHandlerNamesToElementsMap.size (), "element", "elements");

  if (fHandlerNamesToElementsMap.size ()) {
    gLog <<
      ":" <<
      std::endl;
    ++gIndenter;

    constexpr int fieldWidth = 40;

    for (
      std::map <std::string, S_oahElement>::const_iterator i =
        fHandlerNamesToElementsMap.begin ();
      i != fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      std::string name = (*i).first;
      S_oahElement element = (*i).second;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        std::string ("\"") + name + "\"" <<
        ": " <<
        element->fetchNamesBetweenQuotes () <<
        std::endl;
    } // for

    --gIndenter;
  }
  else {
    gLog << std::endl;
  }
}

void oahHandler::displayElementUsesList (std::ostream& os) const
{
  gLog <<
    "The handler elements uses list contains " <<
    mfSingularOrPlural (
      fElementUsesList.size (), "element", "elements");

  if (fElementUsesList.size ()) {
    gLog << ":" << std::endl;
    ++gIndenter;

    int counter = 0;
    for (
      std::list <S_oahElementUse >::const_iterator i =
        fElementUsesList.begin ();
      i != fElementUsesList.end ();
      ++i
    ) {
      S_oahElementUse elementUse = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
      if (false) // JMI 0.9.66
      mfAssert (
        __FILE__, __LINE__,
        elementUse != nullptr,
        "elementUse is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      ++counter;

      gLog <<
        std::right << std::setw (2) << counter <<
        ':' <<
        std::endl;

      ++gIndenter;

      if (elementUse) { // JMI`JMIJMI
        gLog <<
          elementUse <<
          std::endl;
      }
      else {
        gLog <<
          "[NULL]" <<
          std::endl;
      }

      --gIndenter;
    } // for

    gLog << std::endl;

    --gIndenter;
  }
  else {
    gLog << std::endl;
  }
}

void oahHandler::displayOptionsAndArguments (std::ostream& os) const
{
  os <<
    "The handler options and arguments contain:" <<
    std::endl;

  ++gIndenter;
  os <<
    fOptionsAndArguments;
  --gIndenter;
}

void oahHandler::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHandler::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHandler>*
    p =
      dynamic_cast<visitor<S_oahHandler>*> (v)) {
        S_oahHandler elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHandler::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahHandler::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHandler::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahHandler>*
    p =
      dynamic_cast<visitor<S_oahHandler>*> (v)) {
        S_oahHandler elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching oahHandler::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahHandler::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> oahHandler::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // browse the prefixes
  if (fHandlerPrefixesMap.size ()) {
    for (
      std::map <std::string, S_oahPrefix>::const_iterator i = fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      ++i
    ) {
      S_oahPrefix prefix = (*i).second;

      // browse the prefix
      oahBrowser<oahPrefix> browser (v);
      browser.browse (*(prefix));
    } // for
  }

  // browse the groups
  if (fHandlerGroupsList.size ()) {
    for (
      std::list <S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
      i != fHandlerGroupsList.end ();
      ++i
    ) {
      S_oahGroup group = (*i);

      // browse the group
      oahBrowser<oahGroup> browser (v);
      browser.browse (*(group));
    } // for
  }
}

void oahHandler::printHandlerEssentials (
  std::ostream& os,
  int fieldWidth) const
{
  os << std::left <<
    "handlerServiceName:" <<
    std::endl;
  ++gIndenter;
  gIndenter.indentMultiLineString (
    fHandlerServiceName,
    os);
  --gIndenter;

  os << std::left <<
    "handlerHeader:" <<
    std::endl;
  ++gIndenter;
  gIndenter.indentMultiLineString (
    fHandlerHeader,
    os);
  --gIndenter;

  os << std::left <<
    "handlerDescription:" <<
    std::endl;
  ++gIndenter;
  gIndenter.indentMultiLineString (
    fHandlerDescription,
    os);
  --gIndenter;

  os << std::left <<
    "handlerUsage:" <<
    std::endl;
  ++gIndenter;
  gIndenter.indentMultiLineString (
    fHandlerUsage,
    os);
  --gIndenter;
}

void oahHandler::printFull (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "Handler:" <<
    std::endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  os <<
    "fHandlerMultiComponent:";

  if (fHandlerMultiComponent) {
    ++gIndenter;

    os <<
      fHandlerMultiComponent <<
      std::endl;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // launch command
  // --------------------------------------

  os << std::left <<
    std::setw (fieldWidth) << "Launch command:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (fieldWidth) << "fLaunchCommandAsSupplied" << ": " <<
    fLaunchCommandAsSupplied <<
    std::endl <<
    std::setw (fieldWidth) << "fLaunchCommandWithLongOptionsNames" << ": " <<
    fLaunchCommandWithLongOptionsNames <<
    std::endl <<
    std::setw (fieldWidth) << "fLaunchCommandWithShortOptionsNames" << ": " <<
    fLaunchCommandWithShortOptionsNames <<
    std::endl <<
    std::setw (fieldWidth) << "fLaunchOptionsAndOrArgumentsAlreadyMet" << ": " <<
    fLaunchOptionsAndOrArgumentsAlreadyMet <<
    std::endl;


  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOahHandlerFoundAHelpOption" << ": " <<
    fOahHandlerFoundAHelpOption <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fHandlerUsedThruKind" << ": " <<
    fHandlerUsedThruKind <<
    std::endl;

  // print the options prefixes if any
  if (fHandlerPrefixesMap.size ()) {
    printKnownPrefixes (os);
  }

  // print the single-character options if any
  if (fSingleCharacterNamesSet.size ()) {
    printKnownSingleCharacterOptions (os);
  }

  // print the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

std::string oahHandler::asString () const
{
  return "oahHandler::asString()"; // JMI 0.9.71
}

void oahHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "Handler:" <<
    std::endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOahHandlerFoundAHelpOption" << ": " <<
    fOahHandlerFoundAHelpOption <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fHandlerUsedThruKind" << ": " <<
    fHandlerUsedThruKind <<
    std::endl;

/* JMI
  // print the options prefixes if any
  if (fHandlerPrefixesMap.size ()) {
    printKnownPrefixes (os);
  }

  // print the single-character options if any
  if (fSingleCharacterNamesSet.size ()) {
    printKnownSingleCharacterOptions (os);
  }
*/

  // print the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  // print the known options
if (false) { // JMI
  os <<
    "oahHandler known options" <<
    std::endl;
  displayNamesToElementsMap (os);
}

  --gIndenter;
}

void oahHandler::printSummary (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "Handler:" <<
    std::endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  // print a summary of the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahGroup group = (*i);

      // print the options group
      group->printSummary (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahHandler::printHelp (std::ostream& os) const
{
  // print the options handler description
  ++gIndenter;
  gIndenter.indentMultiLineString (
    fHandlerDescription,
    os);
  --gIndenter;

  // print the options handler usage
  os <<
    std::endl <<
    fHandlerUsage <<
    std::endl;

  os << std::endl;

  // print the options groups help
  if (fHandlerGroupsList.size ()) {
// JMI    ++gIndenter;

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahGroup group = (*i);

      // don't print help for empty groups,
      // temporary may 2020 until more is known JMI
      if (group->getGroupSubGroupsList ().size ()) {
        // print the options subgroups if relevant
        switch (group->getElementVisibilityKind ()) {
          case oahElementVisibilityKind::kElementVisibilityNone:
            break;

          case oahElementVisibilityKind::kElementVisibilityWhole:
            group->printHelp (os);
            break;

          case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
            group->printGroupHeader (os);
            group->underlineGroupHeader(os);
            break;

          case oahElementVisibilityKind::kElementVisibilityHidden:
            break;
        } // switch
      }

      if (++i == iEnd) break;
      os << std::endl;
    } // for

 // JMI   --gIndenter;
  }
}

void oahHandler::printOptionsSummary (std::ostream& os) const
{
  // print the options handler description
  gIndenter.indentMultiLineString (
    fHandlerDescription,
    os);

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    ":" <<
    std::endl;

  // print the options handler description
  // print the options groups help summaries
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group summary
      (*i)->printOptionsSummary (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
}

void oahHandler::printHandlerAndGroupAndSubGroupSpecificHelp (
  std::ostream&      os,
  const S_oahSubGroup& subGroup) const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    os <<
      "oahHandler::printHandlerAndGroupAndSubGroupSpecificHelp" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    ":" <<
    std::endl;

  // print the optons group subgroups specific help
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahGroup group = (*i);

      // print the options group specific subgroup help
      group->
        printGroupAndSubGroupSpecificHelp (
          os,
          subGroup);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
}

void oahHandler::printNameIntrospectiveHelp (
  std::ostream&      os,
  const std::string& name)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    os << "Printing name \"" <<
      name <<
      "\" introspective help in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // a given name can be both that of a prefix and that of an option,
  // such as "h" and "help"

  // first, check whether mame is that of a prefix
  Bool suffixHelpHasBeenProvided;

  if (
    // prefix?
    S_oahPrefix
      prefix =
        fetchNameInPrefixesMap (name)
  ) {
    // print the help
    int saveIndent = gIndenter.getIndentation ();

    gIndenter.resetToZero ();

    gLog <<
      "--- Help for prefix \"" <<
      name <<
      "\" ---" <<
      std::endl;

    ++gIndenter;

    prefix->printHelp (os);

    --gIndenter;

    gIndenter.setIndentation (saveIndent);

    suffixHelpHasBeenProvided = true;
  }

  // second, check whether mame is known in the names to elements map
  // is name known in the elements map?
  S_oahElement
    element =
      fetchNameInNamesToElementsMap (name);

  if (element) {
    // name is known by this OAH handler

    if (
      // group?
      S_oahGroup
        group =
          dynamic_cast<oahGroup*>(&(*element))
    ) {
      int saveIndent = gIndenter.getIndentation ();

      gIndenter.resetToZero ();

      // print the help
      gLog <<
        "--- Help for group \"" <<
        name <<
        "\" at help top level ---" <<
        std::endl;

      ++gIndenter;

      group->printHelp (os);

      --gIndenter;

      gIndenter.setIndentation (saveIndent);
    }

    else if (
      // subgroup?
      S_oahSubGroup
        subGroup =
          dynamic_cast<oahSubGroup*>(&(*element))
    ) {
      // get the options group upLink
      S_oahGroup
        group =
          subGroup->
            getUpLinkToGroup ();

      int saveIndent = gIndenter.getIndentation ();

      gIndenter.resetToZero ();

      // print the help
      gLog <<
        "--- Help for subgroup \"" <<
        name <<
        "\" in group \"" <<
        group->getGroupHeader () <<
        "\" ---" <<
        std::endl;

      ++gIndenter;

      group->
        printGroupAndSubGroupHelp (
          gLog,
          subGroup);

      --gIndenter;

      gIndenter.setIndentation (saveIndent);
    }

    else if (
      // atom?
      S_oahAtom
        atom =
          dynamic_cast<oahAtom*>(&(*element))
    ) {
      // get the subgroup upLink
      S_oahSubGroup
        subGroup =
          atom->
            getUpLinkToSubGroup ();

      // get the group upLink
      S_oahGroup
        group =
          subGroup->
            getUpLinkToGroup ();

      int saveIndent = gIndenter.getIndentation ();

      gIndenter.resetToZero ();

      // print the help
      gLog <<
        "--- Help for atom \"" <<
        name <<
        "\" in subgroup \"" <<
        subGroup->
          getSubGroupHeader () <<
        "\"";
      if (group->getGroupHeaderIsToBeWritten ()) {
        gLog <<
          " of group \"" <<
          group->getGroupHeader () <<
          "\" ---";
      }
      gLog << std::endl;

      ++gIndenter;

      group->
        printGroupAndSubGroupAndAtomHelp (
          gLog,
          subGroup,
          atom);

      --gIndenter;

      gIndenter.setIndentation (saveIndent);
    }

    else {
      if (! suffixHelpHasBeenProvided) { // JMI ONH
        std::stringstream ss;

        ss <<
          "cannot provide specific help about name \"" <<
          name <<
          "\"";

        oahInternalError (ss.str ());
      }
    }
  }

  else {
    // element is not known by this OAH handler
    unknownOptionNameError (
      name,
      "cannot deliver specific help");
  }
}

// const std::string oahHandler::containingFindableElementAsString () const
// {
//   std::stringstream ss;
//
//   ss << std::left <<
//     "Handler in service \"" <<
//     fHandlerServiceName <<
//     "\"";
//
//   return ss.str ();
// }

Bool oahHandler::fetchElementsMatchingStringInHandler (
  const std::string&       lowerCaseString,
  std::list <S_oahElement>& foundElementsList,
  std::ostream&            os) const
{
  Bool result;

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    os << "Finding string \"" <<
      lowerCaseString <<
      "\" in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // does this handler's header match?
  Bool headerMatches =
    mfStringToLowerCase (fHandlerHeader).find (lowerCaseString) != std::string::npos;

  // does this handler's description match?
  Bool descriptionMatches =
    mfStringToLowerCase (fHandlerDescription).find (lowerCaseString) != std::string::npos;

  // does this handler's usage match?
  Bool usageMatches =
    mfStringToLowerCase (fHandlerUsage).find (lowerCaseString) != std::string::npos;

  if (headerMatches || descriptionMatches || usageMatches) {
    std::stringstream ss;

    ss <<
      fHandlerHeader <<
      ' ' <<
      fHandlerDescription <<
      ' ' <<
      fHandlerUsage;

    // append the match to foundStringsList
//     foundElementsList.push_back (// JMI 0.9.71 OHA_OAH
//       oahFindStringMatch::create (
//         ss.str (),
//         ss.str (), // JMI 0.9.66
//         containingFindableElementAsString ()));

    result = true;
  }

  // do this handler's prefixes match?
  if (fHandlerPrefixesMap.size ()) {
    ++gIndenter;

    for (
      std::map <std::string, S_oahPrefix>::const_iterator i =
        fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      ++i
    ) {
      S_oahPrefix
        prefix = (*i).second;

      // does the prefix match?
      prefix->
        fetchElementsMatchingString (
          lowerCaseString,
          foundElementsList,
          os);
    } // for

    --gIndenter;
  }

  // do this handler's groups match?
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    for (S_oahGroup group : fHandlerGroupsList) {
      group->
        fetchElementsMatchingString (
          lowerCaseString,
          foundElementsList,
          os);
    } // for

    --gIndenter;
  }

  return result;
}

Bool oahHandler::findStringInFindableElement (
  const std::string&               lowerCaseString,
  std::list <S_oahFindStringMatch>& foundMatchesList,
  std::ostream&                    os) const
{
  Bool result;

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    os << "Finding std::string \"" <<
      lowerCaseString <<
      "\" in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // does this handler's header match?
  Bool headerMatches =
    mfStringToLowerCase (fHandlerHeader).find (lowerCaseString) != std::string::npos;

  // does this handler's description match?
  Bool descriptionMatches =
    mfStringToLowerCase (fHandlerDescription).find (lowerCaseString) != std::string::npos;

  // does this handler's usage match?
  Bool usageMatches =
    mfStringToLowerCase (fHandlerUsage).find (lowerCaseString) != std::string::npos;

  if (headerMatches || descriptionMatches || usageMatches) {
    std::stringstream ss;

    ss <<
      fHandlerHeader <<
      ' ' <<
      fHandlerDescription <<
      ' ' <<
      fHandlerUsage;

    // append the match to foundStringsList
    foundMatchesList.push_back (
      oahFindStringMatch::create (
        ss.str (),
        ss.str (), // JMI 0.9.66
        containingFindableElementAsString ()));

    result = true;
  }

  // do this handler's prefixes match?
  if (fHandlerPrefixesMap.size ()) {
    ++gIndenter;

    for (
      std::map <std::string, S_oahPrefix>::const_iterator i =
        fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      ++i
    ) {
      S_oahPrefix
        prefix = (*i).second;

      // does the prefix match?
      prefix->
        findStringInFindableElement (
          lowerCaseString,
          foundMatchesList,
          os);
    } // for

    --gIndenter;
  }

  // do this handler's groups match?
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    for (S_oahGroup group : fHandlerGroupsList) {
      group->
        findStringInGroup (
          lowerCaseString,
          foundMatchesList,
          os);
    } // for

    --gIndenter;
  }

  return result;
}

void oahHandler::includeOptionsFileInHandler (
  const std::string& optionsFileName,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    os << "Including options file \"" <<
      optionsFileName <<
      "\" in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // open options file
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      gLanguage->openingOptionsFileForReading (optionsFileName));
  }
#endif // MF_TRACE_IS_ENABLED

  std::ifstream
    optionsStream (
      optionsFileName.c_str (),
      std::ifstream::in);

  if (! optionsStream.is_open ()) {
    std::stringstream ss;

    ss <<
      gLanguage->cannotOpenOptionsFileForReading (optionsFileName);

    std::string message = ss.str ();

    gLog <<
      message <<
      std::endl;

    throw mfOahException (message);
  }

  // do the include
  includeOptionsFromFile (
    optionsStream,
    fOptionsAndArguments);

  // close options file
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      gLanguage->closingOptionsFile (optionsFileName));
  }
#endif // MF_TRACE_IS_ENABLED

  optionsStream.close ();

  //  print the options and arguments found
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) { // JMI
//     gLog <<
//       fOptionsAndArguments;
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahHandler::printHandlerOptionsAndArguments (
  std::ostream& os) const
{
  displayOptionsAndArguments (os);
}

void oahHandler::printHandlerOptionsValues (
  std::ostream& os) const
{
#ifdef MF_TRACE_IS_ENABLED
  // print the options handler values header
  size_t
    handlerCommandLineElementsMultisetSize =
      fHandlerCommandLineElementsMultiset.size ();

  if (gEarlyOptions.getTraceEarlyOptions ()) { // JMI
    os <<
      "There are " <<
      fHandlerNamesToElementsMap.size () <<
      " known names for " <<
      fHandlerElementsList.size () <<
      " elements in \"" <<
      /* JMI
      fHandlerServiceName <<
      "\" " <<
      */
      fHandlerHeader <<
      "\", " <<
      handlerCommandLineElementsMultisetSize <<
      " of which " <<
      mfSingularOrPluralWithoutNumber (
        handlerCommandLineElementsMultisetSize, "occurs", "occur") <<
      " in the command line" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  os <<
    "The options values for ";

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    os <<
      "\"" <<
      fHandlerHeader <<
      "\"";
  }
  else {
    os << fHandlerServiceName;
  }
#endif // MF_TRACE_IS_ENABLED

  if (gEarlyOptions.getEarlyInsiderOption ()) {
    os <<
      " (insider mode)";
  }

  os <<
    " are:" <<
    std::endl;

  // print the options groups values
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahGroup group = (*i);

      Bool doPrintOptionsValues;

      // print the options group values if relevant
      switch (group->getElementVisibilityKind ()) {
        case oahElementVisibilityKind::kElementVisibilityNone:
          break;

        case oahElementVisibilityKind::kElementVisibilityWhole:
        case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
          doPrintOptionsValues =
            group->
              getNumberOfUserChoseAtomsInThisGroup () > 0;
          break;

        case oahElementVisibilityKind::kElementVisibilityHidden:
          break;
      } // switch

      if (doPrintOptionsValues) {
        group->
          printGroupOptionsValues (
            os,
            fMaximumVariableNameWidth);
      }

      if (++i == iEnd) break;

      if (doPrintOptionsValues) {
        os << std::endl;
      }
    } // for

    --gIndenter;
  }
}

void oahHandler::printHandlerOptionsValuesAll (
  std::ostream& os) const
{
#ifdef MF_TRACE_IS_ENABLED
  // print the options handler values header
  size_t
    handlerCommandLineElementsMultisetSize =
      fHandlerCommandLineElementsMultiset.size ();

  if (gEarlyOptions.getTraceEarlyOptions ()) { // JMI
    os <<
      "There are " <<
      fHandlerNamesToElementsMap.size () <<
      " known names for " <<
      fHandlerElementsList.size () <<
      " elements in \"" <<
      /* JMI
      fHandlerServiceName <<
      "\" " <<
      */
      fHandlerHeader <<
      "\", " <<
      handlerCommandLineElementsMultisetSize <<
      " of which " <<
      mfSingularOrPluralWithoutNumber (
        handlerCommandLineElementsMultisetSize, "occurs", "occur") <<
      " in the command line" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  os <<
    "All the options values for ";
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    os <<
      "\"" <<
      fHandlerHeader <<
      "\"";
  }
  else {
    os << fHandlerServiceName;
  }
#endif // MF_TRACE_IS_ENABLED
  os <<
    " are:" <<
    std::endl;

  // print the options groups values
  if (fHandlerGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahGroup group = (*i);

      // print the options group values if relevant
      switch (group->getElementVisibilityKind ()) {
        case oahElementVisibilityKind::kElementVisibilityNone:
          break;

        case oahElementVisibilityKind::kElementVisibilityWhole:
        case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
          group->
            printGroupOptionsValuesAll (
              os,
              fMaximumVariableNameWidth);
          break;

        case oahElementVisibilityKind::kElementVisibilityHidden:
          break;
      } // switch

      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
}

void oahHandler::printIncludeFileNamesStack (
  std::ostream& os) const
{
  size_t includeFileNamesStackSize =
    fIncludeFileNamesStack.size ();

  os <<
    "The include file names stack contains " <<
    mfSingularOrPlural (
      includeFileNamesStackSize, "element", "elements");

  if (includeFileNamesStackSize) {
    os << ":" << std::endl;

    ++gIndenter;

    const size_t fieldWidth = 2;

    int counter = 1; // this is to be displayed to the user
    for (std::string fileName : fIncludeFileNamesStack) {
      os <<
        std::right << std::setw (fieldWidth) << counter++ <<
        ": [" <<
        fileName <<
        ']' <<
        std::endl;
    } //for

    --gIndenter;
  }
  else {
    os << std::endl;
  }
}

std::ostream& operator << (std::ostream& os, const S_oahHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

void oahHandler::registerPrefixInHandler (
  const S_oahPrefix& prefix)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    prefix != nullptr,
    "prefix is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Registering prefix " <<
      prefix->fetchPrefixNames () <<
      " in handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string prefixName = prefix->getPrefixName ();

  // is prefixName already known in prefixes map?
  std::map <std::string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        prefixName);

  if (it != fHandlerPrefixesMap.end ()) {
    // prefixName is already known in the map
    std::stringstream ss;

    ss <<
      " prefix name \"" <<
      prefixName <<
      "\" is already known in OAH handler \"" <<
      fHandlerHeader <<
      "\"";

    oahError (ss.str ());
  }

  // register prefix in the options prefixes map
  fHandlerPrefixesMap [prefix->getPrefixName ()] =
    prefix;
}

S_oahPrefix oahHandler::fetchNameInPrefixesMap (
  const std::string& prefixName) const
{
  S_oahPrefix result;

  // is prefixName already known in prefixes map?
  std::map <std::string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        prefixName);

  if (it != fHandlerPrefixesMap.end ()) {
    // prefixName is already known in the map
    result = (*it).second;
  }

  return result;
}

void oahHandler::appendGroupToHandler (
  const S_oahGroup& group)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Appending group \"" <<
      group->getGroupHeader () <<
      "\" to handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append group to elements list
  group->
    appendGroupToElementsList (this);

  // append group to groups list
  fHandlerGroupsList.push_back (group);

  // set the group upLink to this handler
  group->
    setUpLinkToHandler (this);
}

void oahHandler::prependGroupToHandler (
  const S_oahGroup& group)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Prepending group \"" <<
      group->getGroupHeader () <<
      "\" to handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // prepend group  to elements list
  group->
    appendGroupToElementsList (this);

  // prepend group to the groups list
  fHandlerGroupsList.push_front (group);

  // set the group upLink to this handler
  group->
    setUpLinkToHandler (this);
}

void oahHandler::printKnownPrefixes (std::ostream& os) const
{
  size_t oahHandlerPrefixesListSize =
    fHandlerPrefixesMap.size ();

  os <<
    "There are " <<
    oahHandlerPrefixesListSize <<
    " options prefixes known to " <<
    fHandlerServiceName;

  if (gEarlyOptions.getEarlyInsiderOption ()) {
    os <<
    " in handler \"" <<
    fHandlerHeader <<
    "\"";
  }

  os << ":";

  if (oahHandlerPrefixesListSize) {
    os << std::endl;

    ++gIndenter;

    for (
      std::map <std::string, S_oahPrefix>::const_iterator i =
        fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      ++i
    ) {
      S_oahPrefix
        prefix = (*i).second;

      os <<
        prefix->fetchPrefixNames () <<
        ":" <<
        std::endl <<
        gTab <<
          prefix->getPrefixDescription () <<
        std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << std::endl;
  }
}

void oahHandler::printKnownSingleCharacterOptions (std::ostream& os) const
{
  size_t oahHandlerPrefixesListSize =
    fSingleCharacterNamesSet.size ();

  os <<
    "There are " <<
    oahHandlerPrefixesListSize <<
    " options prefixes known to " <<
    fHandlerServiceName;

  if (gEarlyOptions.getEarlyInsiderOption ()) {
    os <<
    " in handler \"" <<
    fHandlerHeader <<
    "\"";
  }

  os <<
    ":" <<
    std::endl;

  // indent a bit more for readability
  ++gIndenter;

  if (oahHandlerPrefixesListSize) {
    std::set <std::string>::const_iterator
      iBegin = fSingleCharacterNamesSet.begin (),
      iEnd   = fSingleCharacterNamesSet.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      std::string theString = (*i);

      cumulatedLength += theString.size ();
      if (cumulatedLength >= K_MF_NAMES_LIST_MAX_LENGTH) {
        os << std::endl;
        cumulatedLength = 0;
        break;
      }

      os << '-' << theString;

      if (++i == iEnd) break;

      if (next (i) == iEnd) {
        os << " and ";
      }
      else {
        os << ", ";
      }
    } // for

    os << std::endl;
  }
  else {
    os <<
      "[EMPTY]" <<
      std::endl;
  }

  --gIndenter;

  os <<
    "They can be clustered, such as:" <<
    std::endl <<
    gTab << "'-vac'" <<
    std::endl <<
    "which is equivalent to:" <<
    std::endl <<
    gTab << "'-v, -a, -c'" <<
    std::endl;
}

// void oahHandler::printOptionsDefaultValuesInformation (std::ostream& os) const
// {
//   os <<
//     std::endl <<
//     "Some value-fitted options can use a default value:" <<
//     std::endl;
//
//   ++gIndenter;
//
//   gIndenter.indentMultiLineString ( JMI 0.9.66
//     foundString,
//     os);
//   os  <<
//     fHandlerServiceName <<
//     gIndenter.indentMultiLineString (
// R"( supports two styles for this, see '-optional-values-style' option, -ovs, .)") <<
//     std::endl;
//
//   --gIndenter;
// }

void oahHandler::displayNamesToElementsMap (std::ostream& os) const
{
  size_t handlerElementsMapSize =
    fHandlerNamesToElementsMap.size ();

  // print the names to elements map
  os <<
    "The " <<
    handlerElementsMapSize <<
    " known names for the " <<
    handlerElementsMapSize << // JMI
    " elements in \"" <<
    fHandlerServiceName <<
    "\" handler \"" <<
    fHandlerHeader <<
    "\" are:" <<
    std::endl;

  ++gIndenter;

  if (handlerElementsMapSize) {
    for (std::pair <std::string, S_oahElement> thePair : fHandlerNamesToElementsMap) {
      os <<
        "\"" << thePair.first << "\" ==>" <<
        std::endl;

      ++gIndenter;

      thePair.second->
        printOptionHeader (
          os);

      --gIndenter;
    } // for
  }
  else {
    os <<
      "[EMPTY]" <<
      std::endl;
  }

  --gIndenter;

  // create a list of the options map elements names
  std::list <std::string> optionsMapElementsNamesList;

  size_t optionsMapElementsNamesListSize =
    optionsMapElementsNamesList.size ();

  if (optionsMapElementsNamesListSize) {
    for (
      std::map <std::string, S_oahElement>::const_iterator i = fHandlerNamesToElementsMap.begin ();
      i !=  fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      optionsMapElementsNamesList.push_back ((*i).first);
    } // for
  }

  // sort it
  optionsMapElementsNamesList.sort ();

  // print it
  os <<
    "The " <<
    optionsMapElementsNamesListSize <<
    " known options names for the " <<
    optionsMapElementsNamesListSize << // JMI
    " registered elements in \"" <<
    fHandlerServiceName <<
    "\" handler " <<
    fHandlerHeader <<
    " are:" <<
    std::endl;

  ++gIndenter;

  if (optionsMapElementsNamesListSize) {
    for (std::string elementName : optionsMapElementsNamesList) {
      os <<
        "\"" << elementName << "\" ==>" <<
        std::endl;
    } // for
  }
  else {
    os <<
      "[EMPTY]" <<
      std::endl;
  }

  --gIndenter;
}

S_oahElement oahHandler::fetchElementByNameInHandler (
  const std::string& name)
{
  S_oahElement result;

  for (
    std::list <S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
    i != fHandlerGroupsList.end ();
    ++i
  ) {
    // search name in the options group
    result =
      (*i)->fetchOptionByNameInGroup (name);

    if (result) {
      break;
    }
  } // for

  return result;
}

void oahHandler::checkMissingPendingArgvValueFittedAtomValue (
  const std::string& lastOptionNameFound,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    ++gIndenter;

    gLog <<
      "Checking missing pending argv value-fitted atom \"" <<
      lastOptionNameFound <<
      "\", the pending value-fitted atom is:";

    if (fPendingArgvValueFittedAtom) {
      gLog << std::endl;
      ++gIndenter;

      gLog <<
        fPendingArgvValueFittedAtom;

      --gIndenter;
    }
    else {
      gLog <<
        " null" <<
        std::endl;
    }

    gLog << std::endl;
    --gIndenter;
  }

/* JMI
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      ", fHandlerOptionalValuesStyleKind: " <<
      oahOptionalValuesStyleKindAsString (
        fHandlerOptionalValuesStyleKind) <<
      ", fElementValueKind: " <<
      oahElementValueKindAsString (
        fElementValueKind);

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
*/
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    gLog <<
      "Checking missing value for pending value-fitted atom \"" <<
      lastOptionNameFound <<
      "\", the name pending value-fitted atom is: " <<
      "\"" << fNameUsedForPendingArgvValueFittedAtom << "\"" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (fPendingArgvValueFittedAtom) {
    std::stringstream ss;

    ss <<
      "Pending last argv value-fitted atom " <<
      fPendingArgvValueFittedAtom->fetchNamesBetweenQuotes () <<
      " used under name \"" <<
      fNameUsedForPendingArgvValueFittedAtom <<
      "\" expects a value" <<
      std::endl;

    oahError (ss.str ());

    // handle the valued atom using the default value
    // JMI 0.9.66 THIS SWITCH IS SUPERFLUOUS...
    // checkMissingPendingArgvValueFittedAtomValue() should be replaced by the direct code
    // for oahElementValueKind::kElementValueFitted below
//     switch (fPendingArgvValueFittedAtom->getElementValueKind ()) {
//       case oahElementValueKind::kElementValue_UNKNOWN_:
// //         unknownOptionValueKindError (
// //           lastOptionNameFound);
//         break;
//
//       case oahElementValueKind::kElementValueLess:
// // //       case oahElementValueKind::kElementValueImplicit:
// //         unknownOptionNameError (
// //           lastOptionNameFound,
// //           "does not expect an optional value, cannot be used with a '='");
//         break;
//
//       case oahElementValueKind::kElementValueFitted:
//         {
//           std::stringstream ss;
//
//           ss <<
//             "Pending last argv value-fitted atom " <<
//             fPendingArgvValueFittedAtom->fetchNamesBetweenQuotes () <<
//             " used under name \"" <<
//             fNameUsedForPendingArgvValueFittedAtom <<
//             "\" expects a value" <<
//             std::endl;
//
//           oahError (ss.str ());
//           }
//         break;
//
//       case oahElementValueKind::kElementValueDefaulted:
// // #ifdef MF_TRACE_IS_ENABLED
// //         if (gEarlyOptions.getTraceEarlyOptions ()) {
// //           gLog <<
// //             "Applying default value to pending value-fitted atom \"" <<
// //             lastOptionNameFound <<
// //             "\", the name pending value-fitted atom is: " <<
// //             "\"" << fNameUsedForPendingArgvValueFittedAtom << "\"" <<
// //             std::endl;
// //         }
// // #endif // MF_TRACE_IS_ENABLED
// //
// //         fPendingArgvValueFittedAtom->
// //           applyAtomWithDefaultValue (
// //             gLog); // oahHandler-specific log stream??? JMI 0.9.66
//         break;
//     } // switch

    // forget about this pending value-fitted atom
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "Forgetting about the pending argv value-fitted atom when handling atom name \"" <<
        lastOptionNameFound <<
        "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPendingArgvValueFittedAtom = nullptr;
    fNameUsedForPendingArgvValueFittedAtom = "";
  }

  else {
    // no value-fitted atom is pending
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "No argv value-fitted atom is pending when handling atom name \"" <<
        lastOptionNameFound <<
        "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

void oahHandler::registerOahElementUse (
  const S_oahElement& elementUsed,
  const std::string&  nameUsed,
  const std::string&  valueUsed)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   if (false) // JMI
  mfAssert (
    __FILE__, __LINE__,
    elementUsed != nullptr,
    "elementUsed is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Registering element use " <<
      elementUsed->fetchNamesBetweenQuotes () <<
      ", nameUsed: \"" << nameUsed << "\"" <<
      ", valueUsed: \"" << valueUsed << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fElementUsesList.push_back (
    oahElementUse::create (
      elementUsed,
      nameUsed,
      valueUsed));

  // register option used in command line
  registerOahElementUseInLaunchCommand (
    elementUsed,
    nameUsed,
    valueUsed);

  // should we apply '-trace-oah' right now? JMIJMIJMI
#ifdef MF_TRACE_IS_ENABLED
  if (
    elementUsed == gTraceOahGroup->getTraceOahBooleanAtom ()
  ) {
    gTraceOahGroup->setTraceOah ();
  }
#endif // MF_TRACE_IS_ENABLED

  // should we apply '-include' right now? JMIJMIJMI
  if (
    elementUsed == gOahOahGroup->getIncludeOptionsAndArgumentsFileAtom ()
  ) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
      std::stringstream ss;

      ss <<
        "Registering element use " <<
        elementUsed->fetchNamesBetweenQuotes () <<
        ", nameUsed: \"" << nameUsed << "\"" <<
        ", valueUsed: \"" << valueUsed << "\"" <<
        " leads to include this options and arguments file now";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

//     displayOptionsAndArguments (gLog); // JMIJMIJMI

    includeOptionsAndArgumentsFile (
      valueUsed,
      gLog);

//     displayOptionsAndArguments (gLog);// JMIJMIJMI
  }
}

void oahHandler::registerAtomUse (
  const S_oahAtom&   atomUsed,
  const std::string& nameUsed,
  const std::string& valueUsed)
{
  registerOahElementUse (
    atomUsed,
    nameUsed,
    valueUsed);

  // register atom as being used
  atomUsed->
    registerAtomAsBeingUsed ();
}

void oahHandler::registerOahElementUseInLaunchCommand (
  const S_oahElement& elementUsed,
  const std::string&  nameUsed,
  const std::string&  valueUsed)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   if (false) // JMI
  mfAssert (
    __FILE__, __LINE__,
    elementUsed != nullptr,
    "elementUsed is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Registering element use " <<
      elementUsed->fetchNamesBetweenQuotes () <<
      ", nameUsed: \"" << nameUsed << "\"" <<
      ", valueUsed: \"" << valueUsed << "\"" <<
      " in command line";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  size_t valueUsedSize = valueUsed.size ();

  // register elementUsed in fLaunchCommandAsSupplied
  fLaunchCommandAsSupplied +=
    " -" + nameUsed;

  if (valueUsedSize) {
    if (valueUsed.find ('\'') != std::string::npos) {
      fLaunchCommandAsSupplied +=
        " \"" + valueUsed + '"';
    }
    else if (valueUsed.find ('"') != std::string::npos) {
      fLaunchCommandAsSupplied +=
        " '" + valueUsed + "\'";
    }
    else if (valueUsed.find (' ') != std::string::npos) {
      fLaunchCommandAsSupplied +=
        " \"" + valueUsed + '"';
    }
    else if (valueUsed.find ('\n') != std::string::npos) {
      fLaunchCommandAsSupplied +=
        " _EOLN_" + valueUsed + '"';
    }
    else {
      fLaunchCommandAsSupplied +=
        ' ' + valueUsed;
    }
  }

  // register elementUsed in fLaunchCommandWithLongOptionsNames
  std::string longName = elementUsed->getLongName ();

  fLaunchCommandWithLongOptionsNames +=
    " -" + longName;

  if (valueUsedSize) {
    if (valueUsed.find ('\'') != std::string::npos) {
      fLaunchCommandWithLongOptionsNames +=
        " \"" + valueUsed + '"';
    }
    else if (valueUsed.find ('"') != std::string::npos) {
      fLaunchCommandWithLongOptionsNames +=
        " '" + valueUsed + "\'";
    }
    else if (valueUsed.find (' ') != std::string::npos) {
      fLaunchCommandWithLongOptionsNames +=
        " \"" + valueUsed + '"';
    }
    else if (valueUsed.find ('\n') != std::string::npos) {
      fLaunchCommandWithLongOptionsNames +=
        " _EOLN_" + valueUsed + '"';
    }
    else {
      fLaunchCommandWithLongOptionsNames +=
        ' ' + valueUsed;
    }
  }

  // register elementUsed in fLaunchCommandWithShortOptionsNames
  if (valueUsed.find ('\'') != std::string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " \"" + valueUsed + '"';
  }
  else if (valueUsed.find ('"') != std::string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " '" + valueUsed + "\'";
  }
  else if (valueUsed.find (' ') != std::string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " \"" + valueUsed + '"';
  }
  else if (valueUsed.find ('\n') != std::string::npos) {
    fLaunchCommandWithShortOptionsNames +=
        " _EOLN_" + valueUsed + '"';
  }
  else {
    fLaunchCommandWithShortOptionsNames +=
      ' ' + valueUsed;
  }
}

void oahHandler::registerArgumentUseInLaunchCommand (
  const std::string& argumentUsed)
{
  // register argumentUsed in fLaunchCommandAsSupplied
  if (argumentUsed.find ('\'') != std::string::npos) {
    fLaunchCommandAsSupplied +=
      " \"" + argumentUsed + '"';
  }
  else if (argumentUsed.find ('"') != std::string::npos) {
    fLaunchCommandAsSupplied +=
      " '" + argumentUsed + "\'";
  }
  else if (argumentUsed.find (' ') != std::string::npos) {
    fLaunchCommandAsSupplied +=
      " \"" + argumentUsed + '"';
  }
  else if (argumentUsed.find ('\n') != std::string::npos) {
    fLaunchCommandAsSupplied +=
      " _EOLN_" + argumentUsed + '"';
  }
  else {
    fLaunchCommandAsSupplied +=
      ' ' + argumentUsed;
  }

  // register argumentUsed in fLaunchCommandWithLongOptionsNames
  if (argumentUsed.find ('\'') != std::string::npos) {
    fLaunchCommandWithLongOptionsNames +=
      " \"" + argumentUsed + '"';
  }
  else if (argumentUsed.find ('"') != std::string::npos) {
    fLaunchCommandWithLongOptionsNames +=
      " '" + argumentUsed + "\'";
  }
  else if (argumentUsed.find (' ') != std::string::npos) {
    fLaunchCommandWithLongOptionsNames +=
      " \"" + argumentUsed + '"';
  }
  else if (argumentUsed.find ('\n') != std::string::npos) {
    fLaunchCommandWithLongOptionsNames +=
      " _EOLN_" + argumentUsed + '"';
  }
  else {
    fLaunchCommandWithLongOptionsNames +=
      ' ' + argumentUsed;
  }

  // register argumentUsed in fLaunchCommandWithShortOptionsNames
  if (argumentUsed.find ('\'') != std::string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " \"" + argumentUsed + '"';
  }
  else if (argumentUsed.find ('"') != std::string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " '" + argumentUsed + "\'";
  }
  else if (argumentUsed.find (' ') != std::string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " \"" + argumentUsed + '"';
  }
  else if (argumentUsed.find ('\n') != std::string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " _EOLN_" + argumentUsed + '"';
  }
  else {
    fLaunchCommandWithShortOptionsNames +=
      ' ' + argumentUsed;
  }
}

void oahHandler::handleOptionPrefixName (
  const std::string& prefixName,
  size_t             equalsSignPosition,
  const std::string& stringAfterEqualsSign)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Handling option prefix name \"" <<
      prefixName <<
      ", equalsSignPosition = \"" << equalsSignPosition << "\"" <<
      ", stringAfterEqualsSign = \"" << stringAfterEqualsSign << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // split stringAfterEqualsSign into a list of strings
  // using the comma as separator
  std::list <std::string> chunksList;

  mfSplitStringIntoChunks (
    stringAfterEqualsSign,
    ",",
    chunksList);

  size_t chunksListSize = chunksList.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "There are " << chunksListSize << " chunk[s]" <<
      " in '" << stringAfterEqualsSign <<
      "':" <<
      std::endl;

    ++gIndenter;

    std::list <std::string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      gLog <<
        '[' << (*i) << ']';
      if (++i == iEnd) break;
      gLog << ' ';
    } // for

    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  S_oahPrefix
    prefix =
      fetchNameInHandlerPrefixesMap (prefixName);

  if (prefix) {
    if (chunksListSize) {
      // expand the option names contained in chunksList
      for (
        std::list <std::string>::const_iterator i =
          chunksList.begin ();
        i != chunksList.end ();
        ++i
      ) {
        std::string singleOptionName = (*i);

        // build uncontracted option name
        std::string
          uncontractedOptionName =
            prefix->getPrefixErsatz () + singleOptionName;

#ifdef MF_TRACE_IS_ENABLED
        if (gEarlyOptions.getTraceEarlyOptions ()) {
          std::stringstream ss;

          ss <<
            "Expanding option \"" << singleOptionName <<
            "\" to \"" << uncontractedOptionName <<
            "\"";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // handle the uncontracted option name
        handleOptionNameCommon (
          uncontractedOptionName);
      } // for
    }
  }

  else {
    std::stringstream ss;

    ss <<
      "option prefix \"" << prefixName <<
      "\" is unknown to \"" <<
        fHandlerHeader <<
        "\", see help summary below";

    printKnownPrefixes (ss);

 // JMI   oahError (ss.str ());
    oahWarning (ss.str ());
  }
}

Bool oahHandler::isNameASingleCharacterOptionsCluster (
  const std::string& optionName)
{
  Bool result (true); // until the contrary is known

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Checking whether optionName \"" <<
      optionName <<
      "\" is a single-character options cluster";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    ! optionName.empty (),
    "optionName.empty ()");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  std::list <S_oahElement> clusterElementsList;

  // collect single-character elements in clusterElementsList
  for (
    std::string::const_iterator i = optionName.begin ();
    i != optionName.end ();
    ++i
  ) {
    std::string singleCharacterString (1, (*i));

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
      std::stringstream ss;

      ss <<
        "Considering single-character '" << singleCharacterString << '\'';

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    std::set <std::string>::const_iterator
      it =
        fSingleCharacterNamesSet.find (
          singleCharacterString);

    if (it != fSingleCharacterNamesSet.end ()) {
      // yes, singleCharacterString is known in the set
      clusterElementsList.push_back (
        fetchElementByNameInHandler (
          singleCharacterString));
    }
    else {
      // no, singleCharacterString is not known in the set,
      // optionName is not a cluster
      result = false; // necessary if others are actually atoms short names JMI
    }
  } // for

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
      gLog << std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  size_t clusterElementsListSize =
    clusterElementsList.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "clusterElementsListSize: " << clusterElementsListSize <<
      std::endl;

    gLog <<
      "clusterElementsList:" <<
      std::endl;

    ++gIndenter;

    for (
      std::list <S_oahElement>::const_iterator i = clusterElementsList.begin ();
      i != clusterElementsList.end ();
      ++i
    ) {
      S_oahElement element = (*i);

      if (element) { // JMI should not be necessary...
        gLog <<
          element->
            fetchNamesBetweenQuotes () <<
          std::endl;
      }
    } // for

    --gIndenter;

    gLog <<
      "fSingleCharacterNamesSet:" <<
      std::endl;
    for (
      std::set <std::string>::const_iterator i = fSingleCharacterNamesSet.begin ();
      i != fSingleCharacterNamesSet.end ();
      ++i
    ) {
      std::string singleCharacterElementName = (*i);

      gLog <<
        "\"" <<
        singleCharacterElementName <<
        "\" ";
    } // for
    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  size_t optionNameSize =
    optionName.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "optionNameSize: " << optionNameSize;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (
    clusterElementsListSize
      &&
    clusterElementsListSize == optionNameSize
  ) {
    // all the elements in clusterElementsList are single character options,
    // handle them
    for (
      std::list <S_oahElement>::const_iterator i = clusterElementsList.begin ();
      i != clusterElementsList.end ();
      ++i
    ) {
      S_oahElement element = (*i);

      // handle element name
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      if (element) { // JMI should not be necessary...
        gLog <<
          "Handling single-character options cluster element " <<
          element->asString () <<
          std::endl;
      }
    }
#endif // MF_TRACE_IS_ENABLED

    handleOptionNameCommon (
      element->getShortName ());
    } // for
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "OptionName \"" <<
        optionName <<
        "\" is no single-character options cluster";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
    result = false;
  }

  --gIndenter;

  return result;
}

void oahHandler::handleNameContainingEqualSign (
  const std::string& optionName,
  size_t equalsSignPosition)
{
  // fetch the strings before and after the equals sign
  std::string
    stringBeforeEqualsSign =
      optionName.substr (0, equalsSignPosition);
  std::string
    stringAfterEqualsSign =
      optionName.substr (equalsSignPosition + 1);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Deciphering option name \"" <<
      optionName <<
      "\" that contains an  equal sign at position " <<
      equalsSignPosition <<
      ", stringBeforeEqualsSign = \"" << stringBeforeEqualsSign <<
      ", stringAfterEqualsSign = \"" << stringAfterEqualsSign << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // prefixes have precedence over options with optional values
  S_oahPrefix
    prefix =
      fetchNameInHandlerPrefixesMap (
        stringBeforeEqualsSign);

  if (prefix) {
    // handle prefix name
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "handleNameContainingEqualSign(): Option name \"" <<
        stringBeforeEqualsSign <<
        "\" is a prefix";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    handleOptionPrefixName (
      stringBeforeEqualsSign,
      equalsSignPosition,
      stringAfterEqualsSign);
  }

  else {
    // stringBeforeEqualsSign is not the name of prefix

    // is it the name of an option?
    S_oahElement
      element =
        fetchElementByNameInHandler (
          stringBeforeEqualsSign);

    if (element) {
      // stringBeforeEqualsSign is the name of an option

      if (
        // oahDefaultedStringAtom?
        S_oahDefaultedStringAtom
          stringWithDefaultValueAtom =
            dynamic_cast<oahDefaultedStringAtom*>(&(*element))
      ) {
#ifdef MF_TRACE_IS_ENABLED
        if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
          std::stringstream ss;

          ss <<
            "Option \"" <<
            stringBeforeEqualsSign <<
            "\" is a stringWithDefaultValueAtom";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // register the use of stringWithDefaultValueAtom
        registerAtomUse (
          stringWithDefaultValueAtom,
          stringBeforeEqualsSign,
          stringAfterEqualsSign);
      }

      else {
        // stringBeforeEqualsSign is not the name an a stringWithDefaultValueAtom
        unknownOptionNameError (
          optionName,
          "doesn't have a default value and thus cannot be used with a '='");
      }
    }

    else {
      // stringBeforeEqualsSign is not the name of an option
      unknownOptionNameError (
        stringBeforeEqualsSign,
        "before '='");
    }
  }
}

void oahHandler::handleOptionsVectorOptionName (
  const std::string& optionName,
  const std::string& valueUsed)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name \"" <<
      optionName <<
      "\" with value \"" <<
      valueUsed <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string
    trailerAfterFirstDash =
      optionName.substr (1, std::string::npos);

  // is optionName known in options map?
  S_oahElement
    element =
      fetchNameInNamesToElementsMap (optionName);

  if (element) {
    // optionName is known, let's handle it
#ifdef MF_TRACE_IS_ENABLED
if (gEarlyOptions.getTraceEarlyOptions ()) {
  gLog <<
    "Handling option name \"" <<
    optionName <<
    "\" which is known to handler \"" <<
    fHandlerHeader <<
    "\"" <<
    std::endl;
}
#endif // MF_TRACE_IS_ENABLED

    handleKnownOptionsVectorElement (
      element,
      optionName,
      valueUsed);
  }

  else {
    // optionName is unknown
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
      if (false) // JMI 0.9.71 OAH_OAH
        this->printHelp (gOutput);

//       unknownOptionNameError (
      unknownOptionNameWarning (
        optionName,
        "unknown to handler 2 ???");
    }
#endif // MF_TRACE_IS_ENABLED

    handleUnknownOptionName (
      optionName);
  }
}

std::string oahHandler::handleArgvOptionName (
  const std::string& optionName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling argv option name \"" <<
      optionName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string currentOptionName;

  std::string
    trailerAfterFirstDash =
      optionName.substr (1, std::string::npos);

  /* JMI
  gLog <<
    "trailerAfterFirstDash \"" << trailerAfterFirstDash << "\" is preceded by a dash" <<
    std::endl;
  */

  // here, trailerAfterFirstDash.size () >= 1

  if (trailerAfterFirstDash [0] == '-') {
    // '--' has been found, optionName is a double-dashed option

    if (trailerAfterFirstDash.size () == 1) {
      // optionName is '--' alone, that marks the end of the options
//       fNowEverythingIsAnArgument = true; // JMI ???

      return "";
    }

    else {
      // optionName is a double-dashed option
      currentOptionName =
        trailerAfterFirstDash.substr (1, std::string::npos);

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
        std::stringstream ss;

        ss <<
          "\"" << currentOptionName << "\" is a double-dashed option" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  else {
    // optionName is a single-dashed option
    currentOptionName = trailerAfterFirstDash;

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
      std::stringstream ss;

      ss <<
        "\"" << currentOptionName << "\" is a single-dashed option";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // handle the current option name
  handleOptionNameCommon (
    currentOptionName);

  return currentOptionName;
}

void oahHandler::handleOptionNameAndValueAtTopOahLevel (
  const std::string& optionName,
  const std::string& optionValue)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "----------------------------------------------------------" <<
      std::endl <<
      "Handling options and arguments element [" <<
      optionName <<
      "] [" <<
      optionValue << ']';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    optionName.size () > 0,
    "optionName is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // does optionName start with a dash?
  if (optionName.front () != '-') {
    unknownOptionNameError (
      optionName,
      "does not start with a dash");
  }

  // extract the non-dash part of optionName
  size_t
    firstNonDashCharacterPosition =
      optionName.find_first_not_of ("-");

  if (firstNonDashCharacterPosition == std::string::npos) {
    unknownOptionNameError (
      optionName,
      "contains only dashes");
  }

  std::string
    optionNameNonDashPart =
      optionName.substr (
        firstNonDashCharacterPosition);

  // does optionNameNonDashPart contain an equal sign?
  size_t equalsSignPosition =
    optionNameNonDashPart.find ("=");

  if (equalsSignPosition != std::string::npos) {
    // yes, there's an equal sign
    handleNameContainingEqualSign ( // JMI NOT HERE BECAUSE IT'S DONE AFTERWARDS IN handleOptionNameCommon() ???
      optionNameNonDashPart,
      equalsSignPosition);
  }

  else {
    // no, there's no equal sign
    // handle the current option name
      handleOptionsVectorOptionName (
        optionNameNonDashPart,
        optionValue);
  }
}

void oahHandler::handleArgvOptionValueOrArgument (
  const std::string& theString)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling argv option value or argument \"" <<
      theString <<
      "\"" <<
      std::endl;

    ++gIndenter;

    gLog <<
      "The pending value-fitted atom is:";

    if (fPendingArgvValueFittedAtom) {
      gLog << std::endl;
      ++gIndenter;

      gLog <<
        fPendingArgvValueFittedAtom;

      --gIndenter;
    }
    else {
      gLog << " null";
    }

    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // options are registered at once, unless they are valued,
  // in which case the handling of the option and its value
  // are postponed until the latter is available

  if (fPendingArgvValueFittedAtom) {
    // theString is the value for the pending value-fitted atom

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "The value associated to atom \"" <<
        fNameUsedForPendingArgvValueFittedAtom <<
        "\" under the name \"" <<
        fNameUsedForPendingArgvValueFittedAtom <<
        "\" in argv is \"" << theString << "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    if (
      fPendingArgvValueFittedAtom
        ==
      gOahOahGroup->getIncludeOptionsAndArgumentsFileAtom ()
    ) {
      // don't register this atom's use,
      // include the file contents instead
      includeOptionsFileInHandler (
        theString,
        gLog);

      // forget about '-include' as the value-fitted atom
      fPendingArgvValueFittedAtom = nullptr; // JMIJMIJMI
    }
    else {
      // register this atom's use
      registerAtomUse (
        fPendingArgvValueFittedAtom,
        fNameUsedForPendingArgvValueFittedAtom,
        theString);
    }

    // forget about this pending value-fitted atom
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
      std::stringstream ss;

      ss <<
        "handleArgvOptionValueOrArgument() Forgetting about the pending value-fitted atom";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPendingArgvValueFittedAtom = nullptr;
    fNameUsedForPendingArgvValueFittedAtom = "";
  }

  else {
    // theString is an argument

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        std::stringstream ss;

        ss <<
          "\"" << theString << "\" is an argument" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

    fOptionsAndArguments.appendArgument (
      theString);

    // register argument in command line
    registerArgumentUseInLaunchCommand (
      theString);
  }
}

oahElementHelpOnlyKind oahHandler::handleOptionsFromOptionsAndArguments (
  const std::string& serviceName)
{
  // start the clock
  clock_t startClock = clock ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling the options from options and arguments in " <<
      "\"" << fHandlerHeader << "\"" <<
      " for \"" << fHandlerServiceName << "\"" <<
      std::endl;

    gTraceOahGroup->setTraceOah ();
  }
#endif // MF_TRACE_IS_ENABLED

  std::string optionsPassDescription =
    "Handle the options from the options and arguments";

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLog <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      optionsPassDescription <<
      std::endl <<
      separator <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  oahElementHelpOnlyKind result;

  // register all this handler's names in the names to elements map
  registerNamesInHandlerToTheNamesToElementsMap ();

  // initialize the history
  // only now because the library must initialized beforehand
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Initializing handler component" <<
      ", serviceName: " << serviceName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  initializeHandlerMultiComponent ();

  // create the elements uses list from the options and arguments
  createElementUsesListFromOptionsAndArguments ( // (pass 2)
    serviceName);

// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     // display the options and arguments
//     gLog <<
//       fOptionsAndArguments;
//   }
// #endif // MF_TRACE_IS_ENABLED

  // apply the options in the elements uses list
  result =
    applyOptionsFromElementUsesList (); // (pass 3)

  // check the options and arguments
  checkOptionsAndArgumentsConsistency ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Result oahElementHelpOnlyKind: " <<
      oahElementHelpOnlyKindAsString (result);

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling,
    optionsPassDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  return result;
}

oahElementHelpOnlyKind oahHandler::handleOptionsAndArgumentsFromArgcArgv (
  int   argc,
  char* argv[])
{
  oahElementHelpOnlyKind
    result =
      oahElementHelpOnlyKind::kElementHelpOnlyNo;

  std::string
    optionsAndArgumentsPassDescription =
      gLanguage->handleOptionsAndArgumentsFromArgcArgv ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      optionsAndArgumentsPassDescription << " in \"" <<
      fHandlerHeader <<
      "\" for \"" <<
      fHandlerServiceName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // start the clock
  clock_t startClock = clock ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceOah ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLog <<
      optionsAndArgumentsPassDescription <<
        std::endl <<
        separator <<
        std::endl <<
        gTab <<
        optionsAndArgumentsPassDescription <<
        std::endl <<
        separator <<
        std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    mfDisplayArgcAndArgv (
      argc,
      argv,
      gLog);
  }
#endif // MF_TRACE_IS_ENABLED

  // register service name
  fHandlerServiceName = std::string (argv [0]);

  // register all this handler's names in the names to elements map
  registerNamesInHandlerToTheNamesToElementsMap ();

  // initialize the history
  // only now because the library must initialized beforehand
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "Initializing handler component" <<
      ", fHandlerServiceName: " << fHandlerServiceName;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  initializeHandlerMultiComponent ();

  // create the elements uses list from argc/argv (pass 2)
  createElementUsesListFromArgcArgv (
    argc, argv);

  // apply the options in the elements uses list
  result =
    applyOptionsFromElementUsesList (); // (pass 3)

  // check the options and arguments
  checkOptionsAndArguments ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "result oahElementHelpOnlyKind: " <<
      oahElementHelpOnlyKindAsString (result);

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling,
    optionsAndArgumentsPassDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  return result;
}

void oahHandler::createElementUsesListFromOptionsAndArguments (
  const std::string& serviceName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating atom uses list from options and arguments in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  switch (fHandlerUsedThruKind) {
    case oahHandlerUsedThruKind::kHandlerUsedThruUnknown: // default value
        fHandlerUsedThruKind =
          oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments;
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv:
      {
        std::stringstream ss;

        ss <<
          "handler \"" <<
          fHandlerHeader <<
          "\" has been handled from argc/argv, cannot handle it from oahOptionsVector";

        oahError (ss.str ());
      }
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      {
        std::stringstream ss;

        ss <<
          "handler \"" <<
          fHandlerHeader <<
          "\" cannot be handled twice from argc/argv";

        oahError (ss.str ());
      }
      break;
  } // switch
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       fOptionsAndArguments;
//   }
// #endif // MF_TRACE_IS_ENABLED

  // register service name
  fHandlerServiceName = serviceName;

//   // analyze the options
//   const std::vector <oahOption>&
//     optionsVector =
//       gServiceRunData->
//         getOptionsAndArguments ().getOptionsVector ();
//
//   if (optionsVector.size ()) {
//     for (size_t i = 0; i < optionsVector.size (); ++i) {
//         std::string optionName =
//           optionsVector.at (i).getOptionName ();
//         std::string optionValue =
//           optionsVector.at (i).getOptionValue ();
//
//       handleOptionNameAndValueAtTopOahLevel (
//         optionName,
//         optionValue);
//     } // for
//   }
//
//   else {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getTraceEarlyOptions ()) {
//       gLog <<
//         "The options and arguments is empty in \"" <<
//         fHandlerHeader <<
//         "\"" <<
//         std::endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
//
//   gLog << std::endl;
//
//   --gIndenter;
//
//   // check the options and arguments
//   checkOptionsAndArgumentsConsistency ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     displayOptionsAndArguments (gLog);
//
//     displayElementUsesList ();
//   }
// #endif // MF_TRACE_IS_ENABLED
}

void oahHandler::createElementUsesListFromArgcArgv (
  int   argc,
  char* argv[])
{
  // this is OAH handling pass 2 for command line use

  // force early options trace // TEMP JMIJMIJMI
//   gEarlyOptions.setTraceEarlyOptions ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    if (false) { // JMI
      std::stringstream ss;

      this->printHelp (ss);

      ss <<
        "Creating atom uses list from argc/argv in \"" <<
        fHandlerHeader <<
        "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  switch (fHandlerUsedThruKind) {
    case oahHandlerUsedThruKind::kHandlerUsedThruUnknown: // default value
        fHandlerUsedThruKind =
          oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv;
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv:
      {
        std::stringstream ss;

        ss <<
          "handler \"" <<
          fHandlerHeader <<
          "\" cannot be handled twice from argc/argv";

        oahError (ss.str ());
      }
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      {
        std::stringstream ss;

        ss <<
          "handler \"" <<
          fHandlerHeader <<
          "\" has been handled from oahOptionsVector, cannot handle it from argc/argv";

        oahError (ss.str ());
      }
      break;
  } // switch
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // sort out the command options and the arguments
  int n = 1;

  std::string lastOptionNameFound;

  // loop over the contents of argv:
  while (true) {
    if (argv [n] == nullptr)
      break;

    std::string currentString = std::string (argv [n]);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    // print current option
    std::stringstream ss;

    ss <<
      std::endl <<
      "----------------------------------------------------------" <<
      std::endl <<
      "Handling argv element " <<
      n <<
      ": [" <<
      currentString <<
      ']';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    ++gIndenter;

    // handle currentString
    if (currentString [0] == '-') {
      // currentString start with a dash: stdin or option?

      if (currentString.size () == 1) {
        // currentString is the stdin indicator, thus an argument
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "\"" << currentString << "\" is the '-' stdin indicator";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

        // currentString is an argument
#ifdef MF_TRACE_IS_ENABLED
        if (gEarlyOptions.getTraceEarlyOptions ()) {
          std::stringstream ss;

          ss <<
              currentString << " is an argument" <<
              std::endl;

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // append it to the arguments vector
        fOptionsAndArguments.appendArgument (
          currentString);
      }

      else {
        // currentString is an option name
#ifdef MF_TRACE_IS_ENABLED
        if (gEarlyOptions.getTraceEarlyOptions ()) {
          std::stringstream ss;

          ss <<
            currentString << " is an option name";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        // first '-' has been found and currentString.size () >= 2

        lastOptionNameFound =
          handleArgvOptionName (
            currentString);
      }
    }

    else {
      // currentString is no option name:
      // it is an atom value or an argument

      handleArgvOptionValueOrArgument (
        currentString);
    }

    --gIndenter;

    // next please
    ++n;
  } // while

  // is a pending value-fitted atom' value missing?
  checkMissingPendingArgvValueFittedAtomValue (
    lastOptionNameFound,
    "last option in command line");

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    displayOptionsAndArguments (gLog);

    displayElementUsesList (gLog);
  }
#endif // MF_TRACE_IS_ENABLED
}

oahElementHelpOnlyKind oahHandler::applyOptionsFromElementUsesList ()
{
  // this is OAH handling pass 3

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Applying options from elements uses list in \"" <<
      fHandlerHeader << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    displayOptionsAndArguments (gLog); // JMIJMIJMI 0.9.66

    displayElementUsesList (gLog);
  }
#endif // MF_TRACE_IS_ENABLED

  if (fElementUsesList.size ()) {
    ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
    int counter = 0;
#endif // MF_TRACE_IS_ENABLED

    for (
      std::list <S_oahElementUse >::const_iterator i =
        fElementUsesList.begin ();
      i != fElementUsesList.end ();
      ++i
    ) {
      S_oahElementUse elementUse = (*i);

      S_oahElement
        elementUsed =
          elementUse->getElementUsed ();

      std::string
        nameUsed =
          elementUse->getNameUsed (),
        valueUsed =
          elementUse->getValueUsed ();

#ifdef MF_TRACE_IS_ENABLED
      ++counter;
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
      gLog <<
        std::endl <<
        "----------------------------------------------------------" <<
        std::endl <<
        "Applying option " <<
        counter <<
        ": " <<
        elementUsed->fetchNamesBetweenQuotes () <<
        " used under name \"" <<
        nameUsed  <<
        "\" with value [" <<
        valueUsed << "]:" <<
        std::endl;

      ++gIndenter;

      gLog <<
        elementUsed->asShortString () <<
        std::endl;

      --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

      if (elementUsed) {
#ifdef MF_TRACE_IS_ENABLED
        if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
          oahElementValueKind
            atomValueExpectedKind =
              elementUsed->getElementValueKind ();

          std::stringstream ss;

          ss <<
            "Its atomValueExpectedKind is '" <<
            oahElementValueKindAsString (atomValueExpectedKind) <<
            '\'';

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // the heart of it
        if (
          // group?
          S_oahGroup
            group =
              dynamic_cast<oahGroup*>(&(*elementUsed))
        ) {
          group->
            applyGroup (
              gOutput);
        }

        else if (
          // subgroup?
          S_oahSubGroup
            subGroup =
              dynamic_cast<oahSubGroup*>(&(*elementUsed))
        ) {
          subGroup->
            applySubGroup (
              gOutput);
        }

        else {
          // this is an atom
          S_oahAtom
            atom =
              dynamic_cast<oahAtom*>(&(*elementUsed));

//           oahElementValueKind
//             atomValueKind =
//               atom->
//                 getElementValueKind ();

          if (
            // value-fitted atom?
            S_oahValueFittedAtom
              valueFittedAtom =
                dynamic_cast<oahValueFittedAtom*>(&(*elementUsed))
          ) {
#ifdef MF_TRACE_IS_ENABLED
            if (gEarlyOptions.getTraceEarlyOptions ()) {
              gLog <<
                "**** Handling value-fitted atom: \"" <<
                nameUsed  <<
                "\" with value [" <<
                valueUsed << "]:" <<
              std::endl;

              ++gIndenter;

              gLog <<
                atom->asShortString () <<
                std::endl;

              --gIndenter;
            }
#endif // MF_TRACE_IS_ENABLED

            if (valueUsed.size ()) {
              valueFittedAtom->
                applyAtomWithValue (
                  valueUsed,
                  gOutput);
            }
            else {
              std::stringstream ss;

              ss <<
                "Value-fitted atom " <<
                valueFittedAtom->fetchNamesBetweenQuotes () <<
                " expects a non-empty value";

              oahInternalError (ss.str ());
            }

//             switch (atomValueKind) { // JMI 0.9.71 OAH_OAH
//               case oahElementValueKind::kElementValue_UNKNOWN_:
//                 unknownOptionValueKindError (
//                   valueFittedAtom->fetchNamesBetweenQuotes ());
//                 break;
//
//               case oahElementValueKind::kElementValueLess:
//                 {
//                   std::stringstream ss;
//
//                   ss <<
//                     "Atom " <<
//                     valueFittedAtom->fetchNamesBetweenQuotes () <<
//                     " is value-less";
//
//                   oahInternalError (ss.str ());
//                 }
//                 break;
//
// //               case oahElementValueKind::kElementValueImplicit:
// //                 valueFittedAtom->
// //                   applyAtomWithDefaultValue (
// //                     gOutput);
// //                 break;
//
//               case oahElementValueKind::kElementValueFitted:
//                 if (valueUsed.size ()) {
//                   valueFittedAtom->
//                     applyAtomWithValue (
//                       valueUsed,
//                       gOutput);
//                 }
//                 else {
//                   std::stringstream ss;
//
//                   ss <<
//                     "Value-fitted atom " <<
//                     valueFittedAtom->fetchNamesBetweenQuotes () <<
//                     " expects a non-empty value";
//
//                   oahInternalError (ss.str ());
//                 }
//                 break;
//
//               case oahElementValueKind::kElementValueDefaulted:
//                 if (valueUsed.size ()) {
//                   valueFittedAtom->
//                     applyAtomWithValue (
//                       valueUsed,
//                       gOutput);
//                 }
//                 else {
//                   valueFittedAtom->
//                     applyAtomWithDefaultValue (
//                       gOutput);
//                 }
//                 break;
//             } // switch
          }

          else {
            // value-less atom
            S_oahValueLessAtom
              valueLessAtom =
                dynamic_cast<oahValueLessAtom*>(&(*elementUsed));

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
            // sanity check // JMI 0.9.71 OAH_OAH
            mfAssert (
              __FILE__, __LINE__,
              valueLessAtom != nullptr,
              "valueLessAtom is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
            if (gEarlyOptions.getTraceEarlyOptions ()) {
              gLog <<
                "**** Handling value-less atom:" <<
                std::endl;

              ++gIndenter;

              gLog <<
                valueLessAtom->asShortString () <<
                std::endl;

              --gIndenter;
            }
#endif // MF_TRACE_IS_ENABLED

            valueLessAtom->
              applyValueLessAtom (
                gOutput);
          }
        }

        // has a help-only been applied?
        switch (elementUsed->getElementHelpOnlyKind ()) {
          case oahElementHelpOnlyKind::kElementHelpOnlyYes:
            // a help option has been applied
            this->
              setOahHandlerFoundAHelpOption (
                elementUsed->
                  fetchNamesBetweenQuotes ());
            break;
          case oahElementHelpOnlyKind::kElementHelpOnlyNo:
            break;
        } // switch
      }

      else {
        std::stringstream ss;

        ss <<
          "Element from the from the atom uses list for \"" <<
          nameUsed <<
          "\" is NULL";

        oahInternalError (ss.str ());
      }
    } // for

    --gIndenter;
  }

  // print the options and arguments if so selected
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayOptionsAndArguments ()) {
    printHandlerOptionsAndArguments (
      gLog);

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // print the early options if so selected
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayEarlyOptionsValues ()) {
    gEarlyOptions.print (
      gLog);

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // print the options values if so selected
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayOptionsValues ()) {
    printHandlerOptionsValues (
      gLog);
    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayOptionsValuesFull ()) {
    printHandlerOptionsValuesAll (
      gLog);
    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // check the consistency of the options
  // ------------------------------------------------------

//   gLog <<
//     "==> checkHandlerOptionsConsistency();" << // JMI
//     std::endl;

  checkHandlerOptionsConsistency ();

  --gIndenter;

  // was this run a 'help-only' one?
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    if (false && fOahHandlerFoundAHelpOption) { // JMIJMIJMI 0.9.66
      gLog <<
        "This is a help-only run " <<
        std::endl;

      return oahElementHelpOnlyKind::kElementHelpOnlyYes;
    }
    else {
      gLog <<
        "This is an actual conversion run " <<
        std::endl;

      // let's go ahead
    }
  }
#endif // MF_TRACE_IS_ENABLED

  return
    fOahHandlerFoundAHelpOption
      ? oahElementHelpOnlyKind::kElementHelpOnlyYes
      : oahElementHelpOnlyKind::kElementHelpOnlyNo;
}

void oahHandler::handleKnownOptionsVectorElement (
  const S_oahElement& element,
  const std::string&  optionNameUsed,
  const std::string&  valueUsed)
{

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling known options and arguments element " <<
      element->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Element \"" <<
      element->fetchNamesBetweenQuotes () <<
      "\" is described by: " <<
      std::endl <<
      gTab <<
      element->getDescription ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this element already present in the commande line?
  std::multiset <S_oahElement>::const_iterator
    it =
      fHandlerCommandLineElementsMultiset.find (
        element);

  if (it != fHandlerCommandLineElementsMultiset.end ()) {
    // yes, element is known in the list
    if (! element->getMultipleOccurrencesAllowed ()) {
      std::stringstream ss;

      ss <<
        "options and arguments element \"" <<
        element->fetchNames () <<
        "\" is already present in the options vector";

      oahWarning (ss.str ());
      //abort (); // JMIJMIJMI 0.9.68
    }
  }

  // remember this element as occurring in the command line
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Inserting \"" <<
      element->fetchNamesBetweenQuotes () <<
      "\" in fHandlerCommandLineElementsMultiset";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fHandlerCommandLineElementsMultiset.insert (element);

  // determine element short and long names to be used,
  // in case one of them (short or long) is empty
  std::string
    shortName =
      element->getShortName (),
    longName =
      element->getLongName ();

  std::string
    shortNameToBeUsed =
      shortName,
    longNameToBeUsed =
      longName;

  // replace empty element name if any by the other one,
  // since they can't both be empty
  if (! shortNameToBeUsed.size ()) {
    shortNameToBeUsed = longNameToBeUsed;
  }
  if (! longNameToBeUsed.size ()) {
    longNameToBeUsed = shortNameToBeUsed;
  }

  // handle the option
  if (
    // options group?
    S_oahGroup
      group =
        dynamic_cast<oahGroup*>(&(*element))
  ) {
    registerOahElementUse (
      group, optionNameUsed, ""); // "===group==="); // JMI to debug
  }

  else if (
    // options subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*element))
  ) {
    registerOahElementUse (
      subGroup, optionNameUsed, ""); // "===subGroup==="); // JMI to debug
  }

  else if (
    // value-less atom alias?
    S_oahValueLessAtomAlias
      valueLessAtomAlias =
        dynamic_cast<oahValueLessAtomAlias*>(&(*element))
  ) {
    // use the original value-less atom instead
    S_oahValueLessAtom
      originalValueLessAtom =
        valueLessAtomAlias->getOriginalValueLessAtom ();

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "Atom name \"" << optionNameUsed << "\" is a alias for \"" <<
        originalValueLessAtom->asString () <<
        "\", handling the latter";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    handleKnownOptionsVectorAtom (
      originalValueLessAtom,
      optionNameUsed,
      valueUsed);
  }

  else if (
    // atom?
    S_oahAtom
      atom =
        dynamic_cast<oahAtom*>(&(*element))
  ) {
    handleKnownOptionsVectorAtom (
      atom,
      optionNameUsed,
      valueUsed);
  }

  else {
    std::stringstream ss;

    ss <<
      "Known option name \"" <<
      optionNameUsed <<
      "\" with value \"" <<
      valueUsed <<
      "\" cannot be handled";

    oahInternalError (ss.str ());
  }
}

void oahHandler::handleKnownArgvElement (
  const S_oahElement& element,
  const std::string&  optionNameUsed)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling argv known element " <<
      element->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Element " <<
      element->fetchNamesBetweenQuotes () <<
      " is described by: " <<
      std::endl <<
      gTab <<
      element->getDescription ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this element already present in the commande line?
  std::multiset <S_oahElement, compareOahElements>::const_iterator
    it =
      fHandlerCommandLineElementsMultiset.find (
        element);

  if (it != fHandlerCommandLineElementsMultiset.end ()) {
    // yes, element is known in the list
    if (! element->getMultipleOccurrencesAllowed ()) {
      std::stringstream ss;

      ss <<
        "argv element \"" <<
        element->fetchNames () <<
        "\" is already present in the command line";

      oahWarning (ss.str ());
    }
  }

  // remember this element as occurring in the command line
//   fHandlerCommandLineElementsList.push_back (element); JMI
  fHandlerCommandLineElementsMultiset.insert (element);

  // determine element short and long names to be used,
  // in case one of them (short or long) is empty
  std::string
    longName =
      element->getLongName (),
    shortName =
      element->getShortName ();

  std::string
    longNameToBeUsed =
      longName,
    shortNameToBeUsed =
      shortName;

  // replace empty element name if any by the other one, JMIJMIJMI
  // since they can't both be empty
  if (! shortNameToBeUsed.size ()) {
    shortNameToBeUsed = longNameToBeUsed;
  }
  if (! longNameToBeUsed.size ()) {
    longNameToBeUsed = shortNameToBeUsed;
  }

  // handle the option
  if (
    // options group?
    S_oahGroup
      group =
        dynamic_cast<oahGroup*>(&(*element))
  ) {
    registerGroupUse (
      group, optionNameUsed, "group FYY");
  }

  else if (
    // options subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*element))
  ) {
    registerSubGroupUse (
      subGroup, optionNameUsed, "subGroup FII");
  }

  else if (
    // value-less atom alias?
    S_oahValueLessAtomAlias
      valueLessAtomAlias =
        dynamic_cast<oahValueLessAtomAlias*>(&(*element))
  ) {
    // use the original value-less atom instead
    S_oahValueLessAtom
      originalValueLessAtom =
        valueLessAtomAlias->getOriginalValueLessAtom ();

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      std::stringstream ss;

      ss <<
        "Atom name \"" << optionNameUsed << "\" is a alias for \"" <<
        originalValueLessAtom->asString () <<
        "\", handling the latter";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    handleKnownArgvAtom (
      originalValueLessAtom,
      optionNameUsed);
  }

  else if (
    // atom?
    S_oahAtom
      atom =
        dynamic_cast<oahAtom*>(&(*element))
  ) {
    handleKnownArgvAtom (
      atom, optionNameUsed);
  }

  else {
    std::stringstream ss;

    ss <<
      "Known option name \"" << optionNameUsed << "\" cannot be handled";

    oahInternalError (ss.str ());
  }
}

void oahHandler::handleKnownOptionsVectorAtom (
  const S_oahAtom&   atom,
  const std::string& optionNameUsed,
  const std::string& valueUsed)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling known options and arguments atom " <<
      atom->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (atom->getElementValueKind ()) {
    case oahElementValueKind::kElementValue_UNKNOWN_:
      unknownOptionValueKindError (
        optionNameUsed);
      break;

    case oahElementValueKind::kElementValueLess:
//     case oahElementValueKind::kElementValueImplicit:
      registerAtomUse (
        atom, optionNameUsed, ""); // "===options and arguments atom without value==="); // JMI to debug
      break;

    case oahElementValueKind::kElementValueFitted:
      if (
        // options and arguments value-fitted atom?
        S_oahValueFittedAtom
          valueFittedAtom =
            dynamic_cast<oahValueFittedAtom*>(&(*atom))
      ) {
        registerAtomUse (
          atom, optionNameUsed, valueUsed);
      }

      else {
        std::stringstream ss;

        ss <<
          "options and arguments atom " <<
          atom->fetchNamesBetweenQuotes () <<
          " is value-less even though its kind is oahElementValueKind::kElementValueFitted";

        oahInternalError (ss.str ());
      }
      break;

    case oahElementValueKind::kElementValueDefaulted:
      registerAtomUse (
        atom, optionNameUsed, ""); // "===atom with optional value===" JMI to debug
      break;
  } // switch
}

void oahHandler::handleKnownArgvAtom (
  const S_oahAtom&   atom,
  const std::string& optionNameUsed)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling known argv atom " <<
      atom->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fPendingArgvValueFittedAtom) {
    std::stringstream ss;

    ss <<
      "Pending argv value-fitted atom " <<
      fPendingArgvValueFittedAtom->fetchNamesBetweenQuotes () <<
      " used under name \"" <<
      fNameUsedForPendingArgvValueFittedAtom <<
      "\" expects a value" <<
      std::endl;

    oahWarning (ss.str ()); // JMI
//     oahError (ss.str ());
  }

  switch (atom->getElementValueKind ()) {
    case oahElementValueKind::kElementValue_UNKNOWN_:
      unknownOptionValueKindError (
        optionNameUsed);
      break;

    case oahElementValueKind::kElementValueLess:
//     case oahElementValueKind::kElementValueImplicit:
      registerAtomUse (
        atom, optionNameUsed, ""); // "===argv atom without value==="); // JMI to debug

      fPendingArgvValueFittedAtom = nullptr;
      fNameUsedForPendingArgvValueFittedAtom = "";
      break;

    case oahElementValueKind::kElementValueFitted:
      if (
        // argv value-fitted atom?
        S_oahValueFittedAtom
          valueFittedAtom =
            dynamic_cast<oahValueFittedAtom*>(&(*atom))
      ) {
        // delay this argv atom's handling until the value is handled
        fPendingArgvValueFittedAtom =
          valueFittedAtom;
        fNameUsedForPendingArgvValueFittedAtom =
          optionNameUsed;
      }

      else {
        std::stringstream ss;

        ss <<
          "argv atom " <<
          atom->fetchNamesBetweenQuotes () <<
          " is value-less even though its kind is"
          " oahElementValueKind::kElementValueFitted";

        oahInternalError (ss.str ());
      }
      break;

    case oahElementValueKind::kElementValueDefaulted:
      registerAtomUse (
        atom, optionNameUsed, ""); // "===use default value===" // JMI to debug

      fPendingArgvValueFittedAtom = nullptr;
      fNameUsedForPendingArgvValueFittedAtom = "";
      break;
  } // switch
}

void oahHandler::handleUnknownOptionName (
  const std::string& optionName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Handling unknown option name \"" <<
      optionName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is optionName an options cluster?
  Bool nameIsAnOptionsCluster;

  if (optionName.size () > 1) {
    // is optionName a single-character options cluster?
    // if so, the options contained in it should be handled
    nameIsAnOptionsCluster =
      isNameASingleCharacterOptionsCluster (
        optionName);
  }

  if (nameIsAnOptionsCluster) {
    // the cluster has just been handled, nothing more here
  }
  else {
    // optionName is unknown to this handler
    unknownOptionNameError (
      optionName,
      "oahHandler::handleUnknownOptionName()"); // JMI do better ??? 0.9.71 OAH_OAH
  }
}

void oahHandler::unknownOptionNameWarning (
  const std::string& name,
  const std::string& context)
{
  std::stringstream ss;

  ss <<
    "unknownOptionNameWarning(): option name \"" <<
    name <<
    "\" is unknown";

  if (gEarlyOptions.getEarlyInsiderOption ()) {
    ss <<
      " in handler \"" <<
    fHandlerHeader <<
    "\"";
  }

  ss <<
    ", " << context;

  oahWarning (ss.str ());
}

void oahHandler::unknownOptionNameError (
  const std::string& name,
  const std::string& context)
{
  std::stringstream ss;

  ss <<
    "unknownOptionNameError(): option name \"" <<
    name <<
    "\" is unknown";

  if (gEarlyOptions.getEarlyInsiderOption ()) {
    ss <<
      " in handler \"" <<
    fHandlerHeader <<
    "\"";
  }

  ss <<
    ", " << context;

  oahError (ss.str ());
}

void oahHandler::unknownOptionValueKindError (const std::string& name)
{
  std::stringstream ss;

  ss <<
    "the value kind of option \"" <<
    name <<
    "\" is unknown";

  if (gEarlyOptions.getEarlyInsiderOption ()) {
    ss <<
      " in handler \"" <<
    fHandlerHeader <<
    "\"";
  }

  oahError (ss.str ());
}


void oahHandler::handleOptionNameCommon (
  const std::string& optionName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Common handling of option name \"" <<
      optionName <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // does optionName contain an equal sign?
  size_t equalsSignPosition =
    optionName.find ("=");

  if (equalsSignPosition != std::string::npos) {
    // yes, there's an equal sign
    handleNameContainingEqualSign (
      optionName,
      equalsSignPosition);
  }

  else {
    // is optionName known in options map?
    S_oahElement
      element =
        fetchNameInNamesToElementsMap (optionName);

    if (element) {
      // optionName is known, let's handle it
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Handling option name \"" <<
      optionName <<
      "\" which is known to handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

      handleKnownArgvElement (
        element,
        optionName);
    }

    else {
      // optionName is unknown
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
        if (false) // JMI
          this->printHelp (gOutput);

//        unknownOptionNameError (
        unknownOptionNameWarning (
          optionName,
          "unknown to handler 3 ???");
      }
#endif // MF_TRACE_IS_ENABLED

      handleUnknownOptionName (
        optionName);
    }
  }
}




// void oahHandler::analyzeOptionsAndArguments (
//    std::string              serviceName,
//    const oahOptionsAndArguments& optionsAndArguments)
// {
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       optionsAndArguments;
//   }
//
//   // analyze the options
//   const std::vector <oahOption>&
//     optionsVector =
//       fOptionsAndArguments.getOptionsVector ();
//
//   if (optionsVector.size ()) {
//     for (size_t i = 0; i < optionsVector.size (); ++i) {
//       std::string optionName =
//         optionsVector.at (i).getOptionName ();
//       std::string optionValue =
//         optionsVector.at (i).getOptionValue ();
//
//       Bool   argumentIsAnOption;
//       std::string argumentWithoutDash;
//
//       // is optionName a double of single dash?
//       if (optionName.substr (0, 2) == "--") {
//         argumentIsAnOption = true;
//
//         argumentWithoutDash = optionName.substr (2);
//       }
//       else if (optionName [0] == '-') {
//         argumentIsAnOption = true;
//
//         argumentWithoutDash = optionName.substr (1);
//       }
//
//       if (argumentIsAnOption) {
//         if (gEarlyOptions.getTraceEarlyOptions ()) {
//           gLog <<
//             "argumentIsAnOption, " <<
//             serviceName << " main()" <<
//             ", argumentWithoutDash: '" << argumentWithoutDash <<
//             '\'' <<
//             std::endl;
//         }
//
//         // is argumentWithoutDash starting with a prefix?
//         Bool   argumentWithoutDashStartsWithAPrefix;
//         std::string argumentWithoutDashToBeUsed = argumentWithoutDash;
//
//         if (argumentWithoutDash.substr (0, 2) == "t=") {
//           argumentWithoutDashStartsWithAPrefix = true;
//
//           argumentWithoutDashToBeUsed =
//            std::string ("t") + argumentWithoutDash.substr (2);
//         }
//         else if (argumentWithoutDash.substr (0, 6) == "trace=") {
//           argumentWithoutDashStartsWithAPrefix = true;
//
//           argumentWithoutDashToBeUsed =
//             std::string ("trace-") + argumentWithoutDash.substr (6);
//         }
//
//         if (gEarlyOptions.getTraceEarlyOptions ()) {
//           gLog <<
//             serviceName <<
//             ", analyzeOptionsAndArguments()" <<
//             ", argumentWithoutDash: '" <<
//             argumentWithoutDash <<
//             ", argumentWithoutDashStartsWithAPrefix: '" <<
//             argumentWithoutDashStartsWithAPrefix <<
//             ", argumentWithoutDashToBeUsed: '" <<
//             argumentWithoutDashToBeUsed <<
//             '\'' <<
//             std::endl;
//         }
//
//         // apply argumentWithoutDashToBeUsed early if relevant
//         gEarlyOptions.applyEarlyOptionIfRelevant (
//           argumentWithoutDashToBeUsed,
//           optionValue);
//       }
//     } //for
//   }
//
//   // analyze the arguments
//   const std::vector <std::string>&
//     optionArgumentsVector =
//       optionsAndArguments.getArgumentsVector ();
//
//   size_t optionArgumentsVectorSize =
//     optionArgumentsVector.size ();
//
//   switch (optionArgumentsVectorSize) {
//     case 0:
//       if (gEarlyOptions.getTraceEarlyOptions ()) {
//         gLog <<
//           serviceName <<
//           " is being launched without any argument, \"-\" will be used" <<
//           std::endl;
//       }
//
//       fInputSourceName = "-";
//       break;
//
//     case 1:
//       {
//         std::string argument =
//           optionArgumentsVector.at (0);
//         if (gEarlyOptions.getTraceEarlyOptions ()) {
//           gLog <<
//             serviceName <<
//             " is being launched with single argument " <<
//             argument <<
//             std::endl;
//         }
//
//         fInputSourceName = argument;
//       }
//       break;
//
//     default:
//       if (gEarlyOptions.getTraceEarlyOptions ()) {
//         gLog <<
//           serviceName <<
//           " is being launched with " <<
//           optionArgumentsVectorSize <<
//           " arguments" <<
//           std::endl;
//       }
//   } // switch
// }
//

void oahHandler::includeOptionsAndArgumentsFile (
  const std::string& includeFileName,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    os << "Including options and arguments file \"" <<
      includeFileName <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fIncludeFileNamesStack.push_front (includeFileName);

  // are there too many include levels?
  if (++fIncludeFilesCurrentLevel == sIncludeFilesMaximumLevel) {
    std::stringstream ss;

    ss <<
      "Including file [" <<
      includeFileName <<
      "]: more than " <<
      sIncludeFilesMaximumLevel <<
      " include levels - quitting";

    std::string message = ss.str ();

    os <<
      message <<
      std::endl << std::endl;

    printIncludeFileNamesStack (os);

    throw mfOahException (message);
  }

  // open options and arguments file
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      gLanguage->openingIncludeFileForReading (includeFileName));
  }
#endif // MF_TRACE_IS_ENABLED

  std::ifstream
    optionsStream (
      includeFileName.c_str (),
      std::ifstream::in);

  if (! optionsStream.is_open ()) {
    std::stringstream ss;

    ss <<
      gLanguage->cannotOpenIncludeFileForReading (includeFileName);

    std::string message = ss.str ();

    os <<
      message <<
      std::endl;

    throw mfOahException (message);
  }

  // do the include
  includeOptionsFromFile (
    optionsStream,
    fOptionsAndArguments);

#ifdef MF_TRACE_IS_ENABLED
  //  print the options and arguments found
  if (gEarlyOptions.getTraceEarlyOptions ()) { // JMI
    os <<
      "After the inclusion of [" + includeFileName + ']' <<
      ", fOptionsAndArguments contains:" <<
      std::endl <<
      fOptionsAndArguments;
  }
#endif // MF_TRACE_IS_ENABLED

  fIncludeFileNamesStack.pop_front ();
  --fIncludeFilesCurrentLevel;
}

//______________________________________________________________________________
void oahHandler::includeOptionsFromFile (
  std::istream&           inputStream,
  oahOptionsAndArguments& optionsAndArguments)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> includeOptionsFromFile()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // decipher inputStream line by line
  while (true) {
    std::string currentLine;

    getline (inputStream, currentLine);

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
      std::stringstream ss;

      ss <<
        "==> currentLine: [" << currentLine << ']';

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    ++gIndenter;

    // remove comment from currentLine if relevant
    size_t commentStart = currentLine.find_first_of ('#');

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
        std::stringstream ss;

        ss <<
          "--> currentLine with comment: [" << currentLine << ']' <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

    if (commentStart != std::string::npos) {
      currentLine.erase (
        commentStart);

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
        std::stringstream ss;

        ss <<
          "--> currentLine without comment: [" << currentLine << ']' <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }

    if (currentLine.size ()) {
      // trim the currentLine
      mfTrimString (currentLine);

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
        std::stringstream ss;

        ss <<
          "--> trimmed currentLine: [" << currentLine << ']' <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }

    if (currentLine.size ()) {
      // option or argument

      if (currentLine [0] == '-') {
        // option

#ifdef MF_TRACE_IS_ENABLED
        if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
          std::stringstream ss;

          ss <<
            "--> option found: [" << currentLine << ']';

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // fetch the option name part and value if any
        std::string optionNamePart (currentLine);
        std::string valuePart (currentLine);

        size_t afterOptionName =
          optionNamePart.find_first_not_of (
            "-"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789"
            "_"
            "=");

        if (afterOptionName == std::string::npos) {
          // there isn't any value after the option name
#ifdef MF_TRACE_IS_ENABLED
          if (gEarlyOptions.getTraceEarlyOptions ()) {
            gLog <<
              "Found option [" << optionNamePart << ']' <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          valuePart.erase (0);
        }

        else {
          // there is something after the option name
          // fetch the option name
          optionNamePart.erase (
            afterOptionName);

          // fetch the value part
          valuePart.erase (
            0,
            afterOptionName);

          // there is at least a space after the option name
          mfTrimString (valuePart);

#ifdef MF_TRACE_IS_ENABLED
          if (gEarlyOptions.getTraceEarlyOptions ()) {
            gLog <<
              "Found option [" << optionNamePart << ' ' << valuePart << ']' <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED
        }

        // the valuePart may be surrounded by quotes or double quotes,
        // as in a shell
        std::string
          valuePartToBeUsed (valuePart);

        // trim optionNameToBeUsed
        mfTrimString (valuePartToBeUsed);

        size_t optionNameToBeUsedSize =
          valuePartToBeUsed.size ();

        if (optionNameToBeUsedSize) {
          if (
            valuePartToBeUsed [0] == '\''
              &&
            valuePartToBeUsed [optionNameToBeUsedSize - 1] == '\''
          ) {
            // quoted option value
            valuePartToBeUsed.erase (0, 1);
            valuePartToBeUsed.erase (optionNameToBeUsedSize - 2, optionNameToBeUsedSize - 1);
          }

          else if (
            valuePartToBeUsed [0] == '"'
              &&
            valuePartToBeUsed [optionNameToBeUsedSize - 1] == '"'
          ) {
            // double-quoted option value
            valuePartToBeUsed.erase (0, 1);
            valuePartToBeUsed.erase (optionNameToBeUsedSize - 2, optionNameToBeUsedSize - 1);
          }
        }

#ifdef MF_TRACE_IS_ENABLED
          if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
            gLog <<
              "valuePart: [" << valuePart << ']' <<
              std::endl <<
              "valuePartToBeUsed: [" << valuePartToBeUsed << ']' <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//         size_t
//           firstNonDashCharacterPosition =
//             optionNameToBeUsed.find_first_of ("-");
//
//         if (firstNonDashCharacterPosition == std::string::npos) {
//           unknownOptionNameError (
//             optionName,
//             "contains only dashes");
//         }
//
//         std::string
//           optionNameNonDashPart =
//             optionName.substr (
//               firstNonDashCharacterPosition);

//         // fetch the option name
//
//
//         while (optionName [0] == '-') {
//           optionName.erase (0, 1);
//         } // while
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
//           gLog <<
//             "optionName: " << optionName <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED

        // store the option name and value
        optionsAndArguments.appendOptionNameAndValue (
          optionNamePart,
          valuePartToBeUsed);

        // are there early options in the included file?
        gEarlyOptions.applyEarlyOptionsIfPresentInOptionsAndArguments (
          fOptionsAndArguments);

        // handle the option name and value
        handleOptionNameAndValueAtTopOahLevel (
          optionNamePart,
          valuePartToBeUsed);

//           // register its use
//           registerAtomUse ( // JMIJMIJMI ???
//             usedOption,
//             optionName,
//             valuePart);

//         // fetch the option by its name
//         S_oahElement
//           usedOption =
//             fetchElementByNameInHandler (
//               optionName);
//
//         if (usedOption) {
// //           handleKnownArgvElement (
// //             usedOption,
// //             optionName);
//         }
//         else {
//           // unknown option name in included filen
//           unknownOptionNameError (
//             optionName,
//             "in included file");
//         }
      }

      else {
        // argument

#ifdef MF_TRACE_IS_ENABLED
        if (gEarlyOptions.getTraceEarlyOptions ()) {
          std::stringstream ss;

          ss <<
            "Found argument [" << currentLine << ']';

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        optionsAndArguments.appendArgument (
          currentLine);
      }
    }

    --gIndenter;

    if (inputStream.eof ()) break;
  } // while

  //  print the options and arguments found
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      optionsAndArguments;
  }
#endif // MF_TRACE_IS_ENABLED
}

//_______________________________________________________________________________
// converting argc/argv to options and arguments
EXP void convertArgcArgvToOptionsAndArguments ( // JMIJMIJMI
  int                     argc,
  char                    *argv[],
  oahOptionsAndArguments& optionsAndArguments);


}

/*
  // decipher theString with a three-number regular expression
  std::string regularExpression (
    "([[:digit:]]+)" // generation number
    "."
    "([[:digit:]]+)" // major number
    "."
    "([[:digit:]]+)" // minor number
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceComponents ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for version string '" << theString <<
      "' with std::regex '" << regularExpression <<
      '\'' <<
      std::endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLog <<
          '[' << sm [i] << "] ";
      } // for

      gLog << std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  if (smSize == 4) {
    // found an n.x.y specification
    std::string
      generationNumberValue = sm [1],
      majorNumberValue      = sm [2],
      minorNumberValue      = sm [3];

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
        "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
        "--> minorNumberValue = \"" << minorNumberValue << "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    generationNumber = stoi (generationNumberValue);
    majorNumber      = stoi (majorNumberValue);
    minorNumber      = stoi (minorNumberValue);
  }

  else {
    // decipher theString with a two-number regular expression
    std::string regularExpression (
      "([[:digit:]]+)" // generation number
      "."
      "([[:digit:]]+)" // major number
      );

    std::regex  e (regularExpression);
    std::smatch sm;

    regex_match (theString, sm, e);

    size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceComponents ()) {
      std::stringstream ss;

      ss <<
        "There are " << smSize << " matches" <<
        " for chord details string '" << theString <<
        "' with std::regex '" << regularExpression <<
        '\'' <<
        std::endl <<
        smSize << " elements: ";

        for (unsigned i = 0; i < smSize; ++i) {
          ss <<
            '[' << sm [i] << "] ";
        } // for

        ss << std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

    if (smSize == 3) {
      // found an n.x specification
      // assume implicit 0 minor number
      std::string
        generationNumberValue = sm [1],
        majorNumberValue      = sm [2];

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getEarlyTraceComponents ()) {
        std::stringstream ss;

        ss <<
          "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
          "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      generationNumber = stoi (generationNumberValue);
      majorNumber      = stoi (majorNumberValue);
      minorNumber      = 0;
    }

    else {
      std::stringstream ss;

      ss <<
        "version number argument '" << theString <<
        "' is ill-formed";

      oahError (ss.str ());
    }
  }
*/


// //   const std::vector <oahOption>& // JMI
// //     optionsVector =
// //       fOptionsAndArguments.getOptionsVector ();
// //
// //   if (optionsVector.size ()) {
// //     ss << ' ';
// //
// //     std::vector <oahOption>::const_iterator
// //       iBegin = optionsVector.begin (),
// //       iEnd   = optionsVector.end (),
// //       i      = iBegin;
// //     for ( ; ; ) {
// //       ss << (*i);
// //       if (++i == iEnd) break;
// //       ss << ' ';
// //     } // for
// //   }
// //
// //   if (fHandlerCommandLineElementsList.size ()) {
// //     ss << ' ';
// //
// //     std::list <S_oahElement>::const_iterator
// //       iBegin = fHandlerCommandLineElementsList.begin (),
// //       iEnd   = fHandlerCommandLineElementsList.end (),
// //       i      = iBegin;
// //     for ( ; ; ) {
// //       S_oahElement element = (*i);
// //
// //       ss << element-> asShortNamedOptionString ();
// //       if (++i == iEnd) break;
// //       ss << ' ';
// //     } // for
// //   }

