/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <fstream>

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "oahOah.h"
#include "waeOah.h"
#include "displayOah.h"

#include "oahEarlyOptions.h"

#include "oahBrowsers.h"


namespace MusicFormats
{

// //_______________________________________________________________________________
// EXP void convertArgcArgvToStringPairVector (
//   int                  argc,
//   char*                argv[],
//   mfStringsPairVector& stringPairVector)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     gLogStream <<
//       "Converting argc/argv to a std::string std::pair std::vector" <<
//       std::endl;
//   }
// #endif
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
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//       gLogStream <<
//         "--> currentOption : " << currentOption <<
//         std::endl <<
//         "--> currString 222   : " << currString <<
//         std::endl << std::endl;
//     }
// #endif
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
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//     displayStringPairVector (
//       stringPairVector,
//       gLogStream);
//   }
// #endif
// }

EXP void displayStringPairVector (
  const mfStringsPairVector& stringPairVector,
  std::ostream&                  os)
{
  size_t stringPairVectorSize =
    stringPairVector.size ();

  os <<
    "The std::string std::pair std::vector contains " <<
    mfSingularOrPlural (
      stringPairVectorSize, "element", "elements");

  if (stringPairVectorSize) {
    os << ":" << std::endl;

    ++gIndenter;

    const size_t fieldWidth = 2;

    int counter = 0;
    for (auto stringPair : stringPairVector) {
      std::string first  = stringPair.first;
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
//   std::vector<std::string>& stringsVector)
// {
//   for (int i=1; i<argc; ++i) {
//     stringsVector.push_back (argv [i]);
//   } // for
// }

// //_______________________________________________________________________________
// EXP Bool arguments2optionsVector (
//   int              argc,
//   char             *argv[],
//   oahOptionsVector& theOptionsVector)
// {
//   // create a strings std::vector from the elements in argv
//   std::vector<std::string> stringsVector;
//
//   argvElements2stringsVector (argc, argv, stringsVector);
//
// #ifdef TRACING_IS_ENABLED
//   if (getEarlyTracingOah ()) {
//     gLogStream <<
//       "arguments2optionsVector: stringsVector size: " <<
//       stringsVector.size() <<
//       std::endl;
//
//     gLogStream << "==> stringsVector:" << std::endl;
//     for (std::string str: stringsVector) {
//       gLogStream << "   " << str << std::endl;
//     } // for
//
//     gLogStream << std::endl;
//   }
// #endif
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
//       std::string str = stringsVector [i];
//
//   #ifdef TRACING_IS_ENABLED
//       if (getEarlyTracingOah ()) {
//         gLogStream << "--> curOption: " << curOption << std::endl;
//         gLogStream << "--> str      : " << str << std::endl;
//       }
//   #endif
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
//     "The options std::vector contains " <<
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
//       std::string optionName  = theOptionsVector [i].first;
//       std::string optionValue = theOptionsVector [i].second;
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
//   std::vector<std::string>& stringsVector)
// {
//   for (int i=1; i<argc; ++i) {
//     stringsVector.push_back (argv [i]);
//   } // for
// }

// //_______________________________________________________________________________
// EXP void convertArgcArgvToOptionsAndArguments (
//   int                    argc,
//   char                   *argv[],
//   oahOptionsAndArguments optionsAndArguments)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     gLogStream <<
//       "Converting argc/argv to options and arguments" <<
//       std::endl;
//   }
// #endif
//   // create a strings std::vector from the elements in argv
//   std::vector<std::string> stringsVector;
//
//   argvElements2stringsVector (
//     argc,
//     argv,
//     stringsVector);
//
//   size_t stringsVectorSize =
//     stringsVector.size ();
//
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     gLogStream <<
//       "Converting argc/argv to options and arguments" <<
//       std::endl <<
//       "==> stringsVector (:" <<
//       stringsVectorSize <<
//       "elements):" <<
//       std::endl;
//
//     displayStringPairVector (
//       stringsVector,
//       gLogStream);
//
//     gLogStream << std::endl;
//   }
// #endif
//
//   // populate optionsAndArguments
//   // ------------------------------------------------------
//
//   std::string currentOption;
//
//   if (stringsVectorSize) {
//     for (std::string currString: stringsVector) {
//
// #ifdef TRACING_IS_ENABLED
//       if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//         gLogStream <<
//           "--> currentOption : " << currentOption <<
//           std::endl <<
//           "--> currString    : " << currString <<
//           std::endl << std::endl;
//       }
// #endif
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
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//     gLogStream <<
//       optionsAndArguments;
//   }
// #endif
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
  const std::string&           first,
  const std::string&           second)
{
  oahOptionOrArgument* o = new
    oahOptionOrArgument (
      optionOrArgumentKind,
      first,
      second);
  assert (o != nullptr);
  return o;
}

oahOptionOrArgument::oahOptionOrArgument (
  oahOptionOrArgumentKind optionOrArgumentKind,
  const std::string&           first,
  const std::string&           second)
{
  fOptionOrArgumentKind = optionOrArgumentKind;

  fFirst  = first;
  fSecond = second;
}

oahOptionOrArgument::~oahOptionOrArgument ()
{}

void oahOptionOrArgument::print (std::ostream& os) const
{
  os <<
    "oahOptionOrArgument:" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOptionOrArgumentKind" << " : " <<
    oahOptionOrArgumentKindAsString (
      fOptionOrArgumentKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fFirst" << " : " << fFirst <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSecond" << " : " << fSecond <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahOptionOrArgument& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
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
  oahOption* o = new
    oahOption (
      optionName,
      optionValue);
  assert (o != nullptr);
  return o;
}

oahOption::oahOption (
  const std::string& optionName,
  const std::string& optionValue)
{
  fOptionName  = optionName;
  fOptionValue = optionValue;
}

oahOption::~oahOption ()
{}

std::string oahOption::asString () const
{
  std::stringstream s;

  s <<
    "oahOption [" << fOptionName << ']';

  if (fOptionValue.size ()) {
    s << " [";

    if (fOptionValue.find ('\'') != std::string::npos) {
      s <<
        " \"" + fOptionValue + '"';
    }
    else if (fOptionValue.find ('"') != std::string::npos) {
      s <<
        " '" + fOptionValue + "\'";
    }
    else if (fOptionValue.find (' ') != std::string::npos) {
      s <<
        " \"" + fOptionValue + '"';
    }
    else {
      s <<
        ' ' + fOptionValue;
    }

    s << ']';
  }

  return s.str ();
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
    os << "[NONE]" << std::endl;
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
  std::stringstream s;

  s <<
    optionName;

  if (optionValue.size ()) {
    if (optionValue.find ('\'') != std::string::npos) {
      s <<
        " \"" + optionValue + '"';
    }
    else if (optionValue.find ('"') != std::string::npos) {
      s <<
        " '" + optionValue + "\'";
    }
    else if (optionValue.find (' ') != std::string::npos) {
      s <<
        " \"" + optionValue + '"';
    }
    else {
      s <<
        ' ' + optionValue;
    }
  }

  return s.str ();
}

void optionsNameAndValueVectorsPlusEquals (
  std::vector<S_oahOption>&       vector1,
  const std::vector<S_oahOption>& vector2)
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
//   oahOptionsNamesAndValuesVector* o = new
//     oahOptionsNamesAndValuesVector ();
//   assert (o != nullptr);
//   return o;
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
//     "The option name and values std::vector contains " <<
//     mfSingularOrPlural (
//       fOptionsVector.size (), "element", "elements");
//
//   if (fOptionsVector.size ()) {
//     os << ":" << std::endl;
//
//     ++gIndenter;
//
//     const int fieldWidth = 2;
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
//   std::stringstream s;
//
//   for (S_oahOption option : fOptionsVector) {
//     std::string
//       optionName  =
//         option->getOptionName (),
//       optionValue =
//         option->getOptionValue ();
//
//     s <<
//       optionName;
//
//     if (optionValue.size ()) {
//       s << ' ';
//
//       if (optionValue.find ('\'') != std::string::npos) {
//         s <<
//           " \"" + optionValue + '"';
//       }
//       else if (optionValue.find ('"') != std::string::npos) {
//         s <<
//           " '" + optionValue + "\'";
//       }
//       else if (optionValue.find (' ') != std::string::npos) {
//         s <<
//           " \"" + optionValue + '"';
//       }
//       else {
//         s <<
//           ' ' + optionValue;
//       }
//     }
//   } //for
//
//   return s.str ();
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
//     os << "[NONE]" << std::endl;
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
  oahOptionsAndArguments* o = new
    oahOptionsAndArguments ();
  assert (o != nullptr);
  return o;
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
    "The option name and values std::vector contains " <<
    mfSingularOrPlural (
      fOptionsVector.size (), "element", "elements");

  if (fOptionsVector.size ()) {
    os << ":" << std::endl;

    ++gIndenter;

    const int fieldWidth = 2;

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
    os << "[NONE]" << std::endl;
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

//______________________________________________________________________________
S_oahPrefix oahPrefix::create (
  const std::string& prefixName,
  const std::string& prefixErsatz,
  const std::string& prefixDescription)
{
  oahPrefix* o = new
    oahPrefix (
      prefixName,
      prefixErsatz,
      prefixDescription);
  assert (o != nullptr);
  return o;
}

oahPrefix::oahPrefix (
  const std::string& prefixName,
  const std::string& prefixErsatz,
  const std::string& prefixDescription)
{
  fPrefixName        = prefixName;
  fPrefixErsatz      = prefixErsatz;
  fPrefixDescription = prefixDescription;
}

oahPrefix::~oahPrefix ()
{}

void oahPrefix::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPrefix::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahPrefix>*
    p =
      dynamic_cast<visitor<S_oahPrefix>*> (v)) {
        S_oahPrefix elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahPrefix::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahPrefix::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPrefix::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahPrefix>*
    p =
      dynamic_cast<visitor<S_oahPrefix>*> (v)) {
        S_oahPrefix elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahPrefix::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahPrefix::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPrefix::browseData ()" <<
      std::endl;
  }
#endif
}

std::string oahPrefix::fetchPrefixNames () const
{
  std::stringstream s;

  s <<
    "\"" <<
    fPrefixName <<
    "\", \"" <<
   fPrefixErsatz <<
   "\"";

  return s.str ();
}

Bool oahPrefix::findStringInFindableElement (
	const std::string&               lowerCaseString,
	std::list<S_oahFindStringMatch>& foundMatchesList,
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

void oahPrefix::printPrefixEssentials (
  std::ostream& os) const
{
  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "prefixName" << " : " <<
    fPrefixName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "prefixErsatz" << " : " <<
    fPrefixErsatz <<
    std::endl <<
    std::setw (fieldWidth) <<
    "prefixDescription" << " : " <<
    fPrefixDescription <<
    std::endl;
}

std::string oahPrefix::asString () const
{
  std::stringstream s;

  s <<
    "[oahPrefix" <<
    ", prefixName: " <<
    fPrefixName <<
    ", prefixErsatz: " <<
    fPrefixErsatz <<
    ", prefixDescription: " <<
    fPrefixDescription;

  s << ']';

  return s.str ();
}

void oahPrefix::print (std::ostream& os) const
{
  os <<
    "[oahPrefix" <<
    std::endl;

  printPrefixEssentials (os);

  os << ']';
}

void oahPrefix::printShort (std::ostream& os) const
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

const std::string oahPrefix::containingFindableElementAsString () const
{
	std::stringstream s;

	s <<
    "Prefix:"; // JMI v0.9.66

	return s.str ();
}

std::ostream& operator << (std::ostream& os, const S_oahPrefix& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class   is purely virtual
S_oahAtom oahAtom::create (
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementValueKind atomValueExpectedKind)
{
  oahAtom* o = new
    oahAtom (
      longName,
      shortName,
      description,
      atomValueExpectedKind);
  assert (o != nullptr);
  return o;
}
*/

oahAtom::oahAtom (
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
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
  S_oahSubGroup subGroup)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    subGroup != nullptr,
    "subGroup is null");

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
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    if (false) // JMI ???
      gLogStream <<
        "Registering atom " <<
        fetchNamesBetweenQuotes () <<
        " in handler \"" <<
        handler->getHandlerHeader () <<
        "\"" <<
        std::endl;
  }
#endif

  ++gIndenter;

  // an atom with an optional value
  // should not have the same name as a prefix,
  // since this would create an ambiguity
  switch (fElementValueKind) {
    case oahElementValueKind::kElementValueUnknown:
    	handler->
				unknownOptionValueKindError (
					fetchNamesBetweenQuotes ());
    	break;

    case oahElementValueKind::kElementValueWithout:
    case oahElementValueKind::kElementValueImplicit:
    case oahElementValueKind::kElementValueMandatory:
      break;

    case oahElementValueKind::kElementValueOptional:
      if (handler->fetchNameInPrefixesMap (fLongName)) {
        std::stringstream s;

        s <<
          "the long name of atom with optional value " <<
          fetchNamesBetweenQuotes () <<
          " is already known as a prefix";

        oahError (s.str ());
      }
      if (handler->fetchNameInPrefixesMap (fShortName)) {
        std::stringstream s;

        s <<
          "the short name of atom with optional value " <<
          fetchNamesBetweenQuotes () <<
          " is already known as a prefix";

        oahError (s.str ());
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtom>*
    p =
      dynamic_cast<visitor<S_oahAtom>*> (v)) {
        S_oahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtom>*
    p =
      dynamic_cast<visitor<S_oahAtom>*> (v)) {
        S_oahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void oahAtom::print (std::ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "Atom ???:" <<
      std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) << "elementHelpOnlyKind" << " : " <<
    oahElementHelpOnlyKindAsString (fElementHelpOnlyKind) <<
    std::endl;

  --gIndenter;
}

void oahAtom::printShort (std::ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "Atom: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);
}

void oahAtom::printSummary (std::ostream& os) const
{
  os <<
    "Atom: " <<
    fetchNames () <<
    std::endl;
}

const std::string oahAtom::containingFindableElementAsString () const
{
	std::stringstream s;

  s << std::left <<
    "Atom in subgroup \"" <<
    fUpLinkToSubGroup->getSubGroupHeader () <<
    "\" in group \"" <<
    fUpLinkToSubGroup->getUpLinkToGroup ()->getGroupHeader () <<
    "\"";

	return s.str ();
}

Bool oahAtom::findStringInAtom (
	const std::string&               lowerCaseString,
	std::list<S_oahFindStringMatch>& foundMatchesList,
	std::ostream&                    os) const
{
  return
    findStringInFindableElement (
      lowerCaseString,
      foundMatchesList,
      os);
}

void oahAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int      valueFieldWidth) const
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class   is purely virtual
S_oahAtomExpectingAValue oahAtomExpectingAValue::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
{
  oahAtomExpectingAValue* o = new
    oahAtomExpectingAValue (
      longName,
      shortName,
      description);
  assert (o != nullptr);
  return o;
}
*/

oahAtomExpectingAValue::oahAtomExpectingAValue (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueMandatory)
{}

oahAtomExpectingAValue::~oahAtomExpectingAValue ()
{}

void oahAtomExpectingAValue::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomExpectingAValue::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtomExpectingAValue>*
    p =
      dynamic_cast<visitor<S_oahAtomExpectingAValue>*> (v)) {
        S_oahAtomExpectingAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomExpectingAValue::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtomExpectingAValue::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomExpectingAValue::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtomExpectingAValue>*
    p =
      dynamic_cast<visitor<S_oahAtomExpectingAValue>*> (v)) {
        S_oahAtomExpectingAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomExpectingAValue::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtomExpectingAValue::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomExpectingAValue::browseData ()" <<
      std::endl;
  }
#endif
}

void oahAtomExpectingAValue::applyElement (std::ostream& os)
{
  std::stringstream s;

  s <<
    "Applying atom expecting a value '" <<
    fetchNames () <<
    "' without a value";

  oahInternalError (s.str ());
}

void oahAtomExpectingAValue::applyAtomWithDefaultValue (std::ostream& os)
{
  std::stringstream s;

  s <<
    "Applying atom expecting a value '" <<
    fetchNames () <<
    "' with a default value is possible only in oahAtomExpectingAValue sub-classes";

  oahInternalError (s.str ());
}

void oahAtomExpectingAValue::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahAtomExpectingAValue:" <<
    std::endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomExpectingAValue::printShort (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahAtomExpectingAValue: ";

  printOahElementEssentialsShort (
    os, fieldWidth);

  os <<
    fDescription <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahAtomExpectingAValue& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class   is purely virtual
S_oahAtomImplicitlyStoringAValue oahAtomImplicitlyStoringAValue::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName)
{
  oahAtomImplicitlyStoringAValue* o = new
    oahAtomImplicitlyStoringAValue (
      longName,
      shortName,
      description,
      variableName);
  assert (o != nullptr);
  return o;
}
*/

oahAtomImplicitlyStoringAValue::oahAtomImplicitlyStoringAValue (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& variableName)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{
  fVariableName = variableName;
}

oahAtomImplicitlyStoringAValue::~oahAtomImplicitlyStoringAValue ()
{}

void oahAtomImplicitlyStoringAValue::applyElement (std::ostream& os)
{
  std::stringstream s;

  s <<
    "Applying atom implicitly storing a value '" <<
    fetchNames () <<
    "'";

  oahInternalError (s.str ());
}

void oahAtomImplicitlyStoringAValue::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomImplicitlyStoringAValue::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtomImplicitlyStoringAValue>*
    p =
      dynamic_cast<visitor<S_oahAtomImplicitlyStoringAValue>*> (v)) {
        S_oahAtomImplicitlyStoringAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomImplicitlyStoringAValue::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtomImplicitlyStoringAValue::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomImplicitlyStoringAValue::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtomImplicitlyStoringAValue>*
    p =
      dynamic_cast<visitor<S_oahAtomImplicitlyStoringAValue>*> (v)) {
        S_oahAtomImplicitlyStoringAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomImplicitlyStoringAValue::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtomImplicitlyStoringAValue::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomImplicitlyStoringAValue::browseData ()" <<
      std::endl;
  }
#endif
}

void oahAtomImplicitlyStoringAValue::printAtomWithVariableEssentials (
  std::ostream& os,
  int fieldWidth) const
{
  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    std::endl;
  if (fSetByAnOption) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "set by an option" <<
      std::endl;
  }
}

void oahAtomImplicitlyStoringAValue::printAtomWithVariableEssentialsShort (
  std::ostream& os,
  int fieldWidth) const
{
  printOahElementEssentialsShort (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    std::endl;
  if (fSetByAnOption) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "set by an option" <<
      std::endl;
  }
}

void oahAtomImplicitlyStoringAValue::print (std::ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "AtomWithVariable:";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomImplicitlyStoringAValue::printShort (std::ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "AtomWithVariable: ";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }

  printAtomWithVariableEssentialsShort (
    os, fieldWidth);
}

void oahAtomImplicitlyStoringAValue::printHelp (std::ostream& os) const
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

void oahAtomImplicitlyStoringAValue::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "AtomWithVariable values:" <<
    "???, fSetByAnOption: " <<
    fSetByAnOption <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahAtomImplicitlyStoringAValue& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class   is purely virtual
S_oahAtomStoringAValue oahAtomStoringAValue::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName)
{
  oahAtomStoringAValue* o = new
    oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName);
  assert (o != nullptr);
  return o;
}
*/

oahAtomStoringAValue::oahAtomStoringAValue (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName)
  : oahAtomExpectingAValue (
      longName,
      shortName,
      description)
{
  fValueSpecification = valueSpecification;

  fVariableName = variableName;

// superflous JMI  this->setElementValueKind (oahElementValueKind::kElementValueMandatory);
}

oahAtomStoringAValue::~oahAtomStoringAValue ()
{}

void oahAtomStoringAValue::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomStoringAValue::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtomStoringAValue>*
    p =
      dynamic_cast<visitor<S_oahAtomStoringAValue>*> (v)) {
        S_oahAtomStoringAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomStoringAValue::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtomStoringAValue::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomStoringAValue::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahAtomStoringAValue>*
    p =
      dynamic_cast<visitor<S_oahAtomStoringAValue>*> (v)) {
        S_oahAtomStoringAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomStoringAValue::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtomStoringAValue::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomStoringAValue::browseData ()" <<
      std::endl;
  }
#endif
}

void oahAtomStoringAValue::printAtomWithVariableEssentials (
  std::ostream& os,
  int fieldWidth) const
{
  printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fValueSpecification" << " : " <<
    fValueSpecification <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSetByAnOption" << " : " <<
    fSetByAnOption <<
    std::endl;
}

void oahAtomStoringAValue::printAtomWithVariableEssentialsShort (
  std::ostream& os,
  int fieldWidth) const
{
  printOahElementEssentialsShort (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fValueSpecification" << " : " <<
    fValueSpecification <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSetByAnOption" << " : " <<
    fSetByAnOption <<
    std::endl;
}

void oahAtomStoringAValue::print (std::ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "AtomWithVariable:";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }
  os << std::endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomStoringAValue::printShort (std::ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "AtomWithVariable: ";
  if (fSetByAnOption) {
    os <<
      ", set by an option";
  }

  printAtomWithVariableEssentialsShort (
    os, fieldWidth);
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

void oahAtomStoringAValue::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "AtomWithVariable values:" <<
    "???, fSetByAnOption: " <<
    fSetByAnOption <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahAtomStoringAValue& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class   is purely virtual
S_oahPureHelpAtomWithoutAValue oahPureHelpAtomWithoutAValue::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
{
  oahPureHelpAtomWithoutAValue* o = new
    oahPureHelpAtomWithoutAValue (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}
*/

oahPureHelpAtomWithoutAValue::oahPureHelpAtomWithoutAValue (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& serviceName)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{
  fHelpAtomWithoutAValueServiceName = serviceName;

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;
}

oahPureHelpAtomWithoutAValue::~oahPureHelpAtomWithoutAValue ()
{}

void oahPureHelpAtomWithoutAValue::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPureHelpAtomWithoutAValue::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahPureHelpAtomWithoutAValue>*
    p =
      dynamic_cast<visitor<S_oahPureHelpAtomWithoutAValue>*> (v)) {
        S_oahPureHelpAtomWithoutAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahPureHelpAtomWithoutAValue::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahPureHelpAtomWithoutAValue::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPureHelpAtomWithoutAValue::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahPureHelpAtomWithoutAValue>*
    p =
      dynamic_cast<visitor<S_oahPureHelpAtomWithoutAValue>*> (v)) {
        S_oahPureHelpAtomWithoutAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahPureHelpAtomWithoutAValue::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahPureHelpAtomWithoutAValue::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPureHelpAtomWithoutAValue::browseData ()" <<
      std::endl;
  }
#endif
}

void oahPureHelpAtomWithoutAValue::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "HelpOnlyAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahPureHelpAtomWithoutAValue::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

std::ostream& operator << (std::ostream& os, const S_oahPureHelpAtomWithoutAValue& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
/* this class   is purely virtual
S_oahPureHelpAtomExpectingAValue oahPureHelpAtomExpectingAValue::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
{
  oahPureHelpAtomExpectingAValue* o = new
    oahPureHelpAtomExpectingAValue (
      longName,
      shortName,
      description,
      serviceName);
  assert (o != nullptr);
  return o;
}
*/

oahPureHelpAtomExpectingAValue::oahPureHelpAtomExpectingAValue (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& serviceName)
  : oahAtomExpectingAValue (
      longName,
      shortName,
      description)
{
  fHelpAtomExpectingAValueServiceName = serviceName;

  fValueSpecification = valueSpecification;
}

oahPureHelpAtomExpectingAValue::~oahPureHelpAtomExpectingAValue ()
{}

void oahPureHelpAtomExpectingAValue::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPureHelpAtomExpectingAValue::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahPureHelpAtomExpectingAValue>*
    p =
      dynamic_cast<visitor<S_oahPureHelpAtomExpectingAValue>*> (v)) {
        S_oahPureHelpAtomExpectingAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahPureHelpAtomExpectingAValue::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahPureHelpAtomExpectingAValue::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPureHelpAtomExpectingAValue::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahPureHelpAtomExpectingAValue>*
    p =
      dynamic_cast<visitor<S_oahPureHelpAtomExpectingAValue>*> (v)) {
        S_oahPureHelpAtomExpectingAValue elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahPureHelpAtomExpectingAValue::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahPureHelpAtomExpectingAValue::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahPureHelpAtomExpectingAValue::browseData ()" <<
      std::endl;
  }
#endif
}

void oahPureHelpAtomExpectingAValue::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "HelpOnlyAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "fValueSpecification" << " : " <<
    fValueSpecification <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fHelpAtomExpectingAValueServiceName" << " : " <<
    fHelpAtomExpectingAValueServiceName <<
    std::endl;

  --gIndenter;
}

void oahPureHelpAtomExpectingAValue::printHelp (std::ostream& os) const
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

std::ostream& operator << (std::ostream& os, const S_oahPureHelpAtomExpectingAValue& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahSubGroup oahSubGroup::create (
  const std::string&            subGroupHeader,
  const std::string&            longName,
  const std::string&            shortName,
  const std::string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup               upLinkToGroup)
{
  oahSubGroup* o = new
    oahSubGroup (
      subGroupHeader,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      upLinkToGroup);
  assert (o != nullptr);
  return o;
}

S_oahSubGroup oahSubGroup::createWithNames (
  const std::string&            subGroupHeader,
  const std::string&            longName,
  const std::string&            shortName,
  const std::string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup               upLinkToGroup)
{
  oahSubGroup* o = new
    oahSubGroup (
      subGroupHeader,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      upLinkToGroup);
  assert (o != nullptr);
  return o;
}

oahSubGroup::oahSubGroup (
  const std::string&            subGroupHeader,
  const std::string&            longName,
  const std::string&            shortName,
  const std::string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup               upLinkToGroup)
  : oahElement (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout,
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
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Appending subgroup \"" <<
      fSubGroupHeader <<
      "\" in the elements std::list in handler \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append subgroup to elements std::list
  handler->
    appendElementToElementsList (this);

  fUpLinkToGroup->
    setUpLinkToHandler (handler);

  // register the OAH atoms in the handler
  for (
    std::list<S_oahAtom>::const_iterator
      i = fSubGroupAtomsList.begin ();
    i != fSubGroupAtomsList.end ();
    ++i
  ) {
    S_oahAtom atom = (*i);
    // register atom in the handler elements std::list
    atom->
      appendAtomToElementsList (
        handler);
  } // for

  --gIndenter;
}

void oahSubGroup::registerNamesInSubGroupToTheNamesToElementsMap (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Registering the names in subgroup \"" <<
      fSubGroupHeader <<
      "\" in the names to elements std::map in handler \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // register this subgroup's names in this handler
  handler->
    registerElementNamesInHandler (
      this);

  // register this subgroup's names in handler
  for (
    std::list<S_oahAtom>::const_iterator
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
  S_oahAtom atom)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    atom != nullptr,
    "atom is null");

  // append atom to atoms std::list
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
    std::list<S_oahAtom>::const_iterator
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

void oahSubGroup::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Applying element " <<
      fetchNames () <<
      ", which is a oahSubGroup" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  // print the help
  gLogStream <<
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

void oahSubGroup::checkSubGroupOptionsConsistency ()
{}

void oahSubGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahSubGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahSubGroup>*
    p =
      dynamic_cast<visitor<S_oahSubGroup>*> (v)) {
        S_oahSubGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahSubGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahSubGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahSubGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahSubGroup>*
    p =
      dynamic_cast<visitor<S_oahSubGroup>*> (v)) {
        S_oahSubGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahSubGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahSubGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahSubGroup::browseData ()" <<
      ", " << fSubGroupHeader <<
      std::endl;
  }
#endif

  // browse the atoms
  if (fSubGroupAtomsList.size ()) {
    for (
      std::list<S_oahAtom>::const_iterator i = fSubGroupAtomsList.begin ();
      i != fSubGroupAtomsList.end ();
      ++i
    ) {
      S_oahAtom atom = (*i);

      // browse the atom
      /* JMI ???
      gLogStream <<
        ".\\\" ==> Going to browse atom" <<
        " '" << atom->fetchNames () << "'" <<
        std::endl;
        */

      oahBrowser<oahAtom> browser (v);
      browser.browse (*(atom));
    } // for
  }
}

void oahSubGroup::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
   "SubGroup:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "elementVisibilityKind" << " : " <<
      oahElementVisibilityKindAsString (
        fElementVisibilityKind) <<
    std::endl << std::endl;

  os <<
    "AtomsList (" <<
    mfSingularOrPlural (
      fSubGroupAtomsList.size (), "atom",  "atoms") <<
    "):" <<
    std::endl;

  if (fSubGroupAtomsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_oahAtom>::const_iterator
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

void oahSubGroup::printShort (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
   "SubGroup: " ;

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    std::list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
}

void oahSubGroup::printSummary (std::ostream& os) const
{
  os <<
    "SubGroup: " <<
    fetchNames () <<
    std::endl;

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    std::list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahAtom atom = (*i);

      // print a summary of the atom
      atom->printSummary (os);
      if (++i == iEnd) break;
//      os << std::endl;
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
  int      subGroupHeaderWidth) const
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

        std::list<S_oahAtom>::const_iterator
          iBegin = fSubGroupAtomsList.begin (),
          iEnd   = fSubGroupAtomsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahAtom atom = (*i);

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

          if (++i == iEnd) break;
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
  int      subGroupHeaderWidth) const
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

        std::list<S_oahAtom>::const_iterator
          iBegin = fSubGroupAtomsList.begin (),
          iEnd   = fSubGroupAtomsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahAtom atom = (*i);

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

          if (++i == iEnd) break;
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

    std::list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
  // JMI    os << std::endl;
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
  S_oahSubGroup subGroup) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    os <<
      "oahSubGroup::printSubGroupSpecificHelpOrOptionsSummary" <<
      std::endl;
  }
#endif

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
  S_oahAtom targetAtom) const
{
  // print the subgroup atoms
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    std::list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahAtom atom = (*i);

      if (atom == targetAtom) {
        // print the target atom's help
        // target options atom's help
        (*i)->printHelp (os);
      }
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

const std::string oahSubGroup::containingFindableElementAsString () const
{
	std::stringstream s;

  s << std::left <<
    "Subgroup in group \"" <<
    fUpLinkToGroup->getGroupHeader () <<
    "\"";

	return s.str ();
}

Bool oahSubGroup::findStringInSubGroup (
	const std::string&               lowerCaseString,
	std::list<S_oahFindStringMatch>& foundMatchesList,
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
  int      valueFieldWidth) const
{
  if (true || fNumberOfUserChoseAtomsInThisSubGroup > 0) {
    // print the header
    os <<
      fSubGroupHeader <<
      ' ' <<
      fetchNamesBetweenParentheses () <<
      ", " <<
      mfSingularOrPlural (
        fNumberOfUserChoseAtomsInThisSubGroup, "atom chosen", "atoms chosen") <<
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
        // print the atom value if the variable has been set by an option by user
        if (booleanAtom->getSetByAnOption ()) {
          atom->
            printAtomWithVariableOptionsValues (
              os,
              valueFieldWidth);
        }
      }

      else if (
        // atomImplicitlyStoringAValue?

        S_oahAtomImplicitlyStoringAValue
          atomImplicitlyStoringAValue =
            dynamic_cast<oahAtomImplicitlyStoringAValue*>(&(*atom))
      ) {
        // print the atom value if the variable has been set by an option by user
        if (atomImplicitlyStoringAValue->getSetByAnOption ()) {
          atom->
            printAtomWithVariableOptionsValues (
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
        // print the atom value if the variable has been set by an option by user
        if (atomStoringAValue->getSetByAnOption ()) {
          atom->
            printAtomWithVariableOptionsValues (
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
  int      valueFieldWidth) const
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
        fNumberOfUserChoseAtomsInThisSubGroup, "atom chosen", "atoms chosen");
  }
  os <<
    ":" <<
    std::endl;

  // underline the options subgroup header
// JMI  underlineSubGroupHeader (os);

  // print the subgroup atoms values
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    std::list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom values
      (*i)->
        printAtomWithVariableOptionsValues (
          os,
          valueFieldWidth);
      if (++i == iEnd) break;
  //    os << std::endl;
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahGroup oahGroup::create (
  const std::string&            header,
  const std::string&            longName,
  const std::string&            shortName,
  const std::string&            description,
  oahElementVisibilityKind optionVisibilityKind)
{
  oahGroup* o = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind);
  assert (o != nullptr);
  return o;
}

S_oahGroup oahGroup::create (
  const std::string&            header,
  const std::string&            longName,
  const std::string&            shortName,
  const std::string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler             groupUpLinkToHandler)
{
  oahGroup* o = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      groupUpLinkToHandler);
  assert (o != nullptr);
  return o;
}

S_oahGroup oahGroup::createWithNames (
  const std::string&            header,
  const std::string&            longName,
  const std::string&            shortName,
  const std::string&            description,
  oahElementVisibilityKind optionVisibilityKind)
{
  oahGroup* o = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind);
  assert (o != nullptr);
  return o;
}

S_oahGroup oahGroup::createWithNames (
  const std::string&            header,
  const std::string&            longName,
  const std::string&            shortName,
  const std::string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler             groupUpLinkToHandler)
{
  oahGroup* o = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      groupUpLinkToHandler);
  assert (o != nullptr);
  return o;
}

oahGroup::oahGroup (
  const std::string&            header,
  const std::string&            longName,
  const std::string&            shortName,
  const std::string&            description,
  oahElementVisibilityKind optionVisibilityKind)
  : oahElement (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout,
      optionVisibilityKind)
{
  fGroupHeader = header;

  fGroupHeaderIsToBeWritten = true; // default value

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  fNumberOfUserChoseAtomsInThisGroup = 0;
}

oahGroup::oahGroup (
  const std::string&            header,
  const std::string&            longName,
  const std::string&            shortName,
  const std::string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler             groupUpLinkToHandler)
  : oahElement (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout,
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
  S_oahHandler upLinkToHandler)
{
  fUpLinkToHandler = upLinkToHandler;
}

void oahGroup::appendGroupToElementsList (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Appending group \"" <<
      fGroupHeader <<
      "\" to elements std::list in handler \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");

  // append this group to the elements std::list
  handler->
    appendElementToElementsList (this);

  // set group handler upLink
  fUpLinkToHandler = handler;

  // append subgroups to elements std::list
  for (
    std::list<S_oahSubGroup>::const_iterator
      i = fGroupSubGroupsList.begin ();
    i != fGroupSubGroupsList.end ();
    ++i
  ) {
    S_oahSubGroup subGroup = (*i);

    // append subgroup to elements std::list
    subGroup->
      appendSubGroupToElementsList (
        handler);
  } // for

  --gIndenter;
}

void oahGroup::registerNamesInGroupToTheNamesToElementsMap (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Registering the names in group \"" <<
      fGroupHeader <<
      "\" in the names to elements std::map in handler \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");

  // register this group' names in handler
  handler->
    registerElementNamesInHandler (
      this);

  // append subgroups to elements std::list
  for (
    std::list<S_oahSubGroup>::const_iterator
      i = fGroupSubGroupsList.begin ();
    i != fGroupSubGroupsList.end ();
    ++i
  ) {
    S_oahSubGroup subGroup = (*i);

    // append subgroup to elements std::list
    subGroup->
      registerNamesInSubGroupToTheNamesToElementsMap (
        handler);
  } // for

  --gIndenter;
}

void  oahGroup::appendSubGroupToGroup (
  S_oahSubGroup subGroup)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    subGroup != nullptr,
    "subGroup is null");

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
    std::list<S_oahSubGroup>::const_iterator
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

void oahGroup::applyElement (std::ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Applying element '" <<
      fetchNames () <<
      "', which is a oahGroup" <<
      std::endl;
  }
#endif

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  // print the help
  gLogStream <<
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
  S_oahAtom     atom,
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Checking the consistency of OAH group \"" <<
      fGroupHeader <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // check the subgroups options consistency
  if (fGroupSubGroupsList.size ()) {
    for (
      std::list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahGroup>*
    p =
      dynamic_cast<visitor<S_oahGroup>*> (v)) {
        S_oahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahGroup>*
    p =
      dynamic_cast<visitor<S_oahGroup>*> (v)) {
        S_oahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahGroup::browseData ()" <<
      ", " << fGroupHeader <<
      std::endl;
  }
#endif

  // browse the subGroups
  if (fGroupSubGroupsList.size ()) {
    for (
      std::list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
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

void oahGroup::print (std::ostream& os) const
{
  const int fieldWidth = 27;

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

    std::list<S_oahSubGroup>::const_iterator
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

void oahGroup::printShort (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "Group: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  if (fGroupSubGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
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

    std::list<S_oahSubGroup>::const_iterator
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
      std::list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
      i != fGroupSubGroupsList.end ();
      ++i
    ) {
      std::string subGroupHeader  = (*i)->getSubGroupHeader ();
      size_t subGroupHeaderSize = subGroupHeader.size ();

      if (subGroupHeaderSize > maximumSubGroupHeaderLength) {
        maximumSubGroupHeaderLength = subGroupHeaderSize;
      }
    } // for

    maximumSubGroupHeaderLength += 3; // to have some more spaces

    if (fGroupHeaderIsToBeWritten) {
      ++gIndenter;
    }

    std::list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print the options subgroup help
      subGroup->
        printHelpWithHeaderWidth (
          os,
          maximumSubGroupHeaderLength);

      if (++i == iEnd) break;
  // JMI    os << std::endl;
    } // for

    if (fGroupHeaderIsToBeWritten) {
      --gIndenter;
    }
  }
}

void oahGroup::printGroupAndSubGroupHelp (
  std::ostream&      os,
  S_oahSubGroup targetSubGroup) const
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

    std::list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup
        subGroup = (*i);

      if (subGroup == targetSubGroup) {
        // print the target options subgroup help
        subGroup->printSubGroupHelp (os);
      }
      if (++i == iEnd) break;
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupAndSubGroupAndAtomHelp (
  std::ostream&      os,
  S_oahSubGroup targetSubGroup,
  S_oahAtom     targetAtom) const
{
  // print the target options subgroup
  if (fGroupSubGroupsList.size ()) {
    std::list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      if (subGroup == targetSubGroup) {
        // print the target options subgroup's
        // target options targetAtom's help
        subGroup->
          printSubGroupAndAtomHelp (
            os,
            targetAtom);
      }

      if (++i == iEnd) break;
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

    std::list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup description
      (*i)->
        printOptionsSummary (os);
      if (++i == iEnd) break;
 //     os << std::endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupAndSubGroupSpecificHelp (
  std::ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    os <<
      "oahGroup::printGroupAndSubGroupSpecificHelp" <<
      std::endl;
  }
#endif

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

    std::list<S_oahSubGroup>::const_iterator
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
  int      valueFieldWidth) const
{
  if (true || fNumberOfUserChoseAtomsInThisGroup > 0) {
    // print the header
    os <<
      fGroupHeader <<
      ' ' <<
      fetchNamesBetweenParentheses () <<
      ", " <<
      mfSingularOrPlural (
        fNumberOfUserChoseAtomsInThisGroup, "atom chosen", "atoms chosen") <<
    ":" <<
    std::endl;

    // underline the options group header
    underlineGroupHeader (os);
  }

  // print the options subgroups values
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    Bool doPrintOptionsValues;

    std::list<S_oahSubGroup>::const_iterator
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
  int      valueFieldWidth) const
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
        fNumberOfUserChoseAtomsInThisGroup, "atom chosen", "atoms chosen");
  }
  os <<
    ":" <<
    std::endl;

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups values
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    std::list<S_oahSubGroup>::const_iterator
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

const std::string oahGroup::containingFindableElementAsString () const
{
	std::stringstream s;

  s << std::left <<
    "Group in handler \"" <<
    fUpLinkToHandler->getHandlerHeader () <<
    "\"";

	return s.str ();
}

void oahGroup::findStringInGroup (
	const std::string&               lowerCaseString,
	std::list<S_oahFindStringMatch>& foundMatchesList,
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

std::ostream& operator << (std::ostream& os, const S_oahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_oahHandler pPrivateCurrentOahHandler;

S_oahHandler getCurrentOahHandler ()
{
  return pPrivateCurrentOahHandler;
}

S_oahHandler oahHandler::pCurrentOahHandler;

//______________________________________________________________________________
int oahHandler::pIncludeFilesMaximumLevel = 10;

// int            getIncludeFilesMaximumLevel ();

/* this class   is purely virtual
S_oahHandler oahHandler::create (
	const std::string& serviceName,
	const std::string& handlerHeader,
	const std::string& handlerDescription,
	const std::string& handlerUsage)
{
  oahHandler* o = new
    oahHandler (
      serviceName,
      handlerHeader,
      handlerDescription,
      handlerUsage);
  assert (o != nullptr);
  return o;
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

  pCurrentOahHandler = this;
}

void oahHandler::initializeHandler ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing handler \"" <<
      fHandlerHeader <<
      "\" for \"" <<
      fHandlerServiceName <<
      "\"" <<
      std::endl;
  }
#endif

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

  fMaximumShortNameWidth   = 1;
  fMaximumLongNameWidth    = 1;

  fMaximumVariableNameWidth = 0;

  fIncludeFilesCurrentLevel =  0;
}

oahHandler::~oahHandler ()
{}

S_oahPrefix oahHandler::fetchNameInHandlerPrefixesMap (
  const std::string& name) const
{
  S_oahPrefix result;

  // is name known in prefixes std::map?
  std::map<std::string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        name);

  if (it != fHandlerPrefixesMap.end ()) {
    // yes, name is known in the std::map
    result = (*it).second;
  }

  return result;
}

S_oahElement oahHandler::fetchNameInNamesToElementsMap (
  const std::string& name) const
{
  S_oahElement result;

  // is name known in the names to elements std::map?
  std::map<std::string, S_oahElement>::const_iterator
    it =
      fHandlerNamesToElementsMap.find (
        name);

  if (it != fHandlerNamesToElementsMap.end ()) {
    // yes, name is known in the std::map
    result = (*it).second;
  }

  return result;
}

void oahHandler::registerElementNamesInHandler (
  S_oahElement element)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    if (true) // JMI
      gLogStream <<
        "Registering the names of " <<
        element->fetchNamesBetweenQuotes () <<
        " in handler \"" <<
        fHandlerHeader <<
        "\"" <<
        std::endl;
  }
#endif

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

  // sanity checks
  if (elementLongNameSize == 0) {
    std::stringstream s;

    s <<
      "element long name is empty";

    oahError (s.str ());
  }

  if (elementShortNameSize == 0 && elementLongNameSize == 0) {
    std::stringstream s;

    s <<
      "element long name and short name are both empty";

    oahError (s.str ());
  }

  if (elementShortName == elementLongName) {
    std::stringstream s;

    s <<
      "element short name \"" << elementLongName << "\"" <<
      " is the same as the long name for the same";

    oahError (s.str ());
  }

  if (elementLongNameSize == 1) {
    std::stringstream s;

    s <<
      "element long name \"" << elementLongName << "\"" <<
      " has only one character";

    oahWarning (s.str ());
  }

  if (elementShortNameSize > elementLongNameSize) {
    std::stringstream s;

    s <<
      "element short name \"" << elementShortName << "\"" <<
      " is longer than long name \"" << elementLongName << "\"" <<
      " for the same";

    oahError (s.str ());
  }

  for (std::pair<std::string, S_oahElement> thePair : fHandlerNamesToElementsMap) {
    std::string       name    = thePair.first;
    S_oahElement element = thePair.second;

// 		gLogStream << // JMI v0.9.65
// 			"--> name: " << name <<
// 			std::endl <<
// 			"--> elementLongName: " << elementLongName <<
// 			std::endl <<
// 			"--> elementShortName: " << elementShortName <<
// 			std::endl;

    // is elementLongName already in the elements names std::map?
    if (name == elementLongName) {
      std::stringstream s;

      s <<
        "element long name \"" << elementLongName << "\"" <<
          " for element short name \"" << elementShortName << "\"" <<
          " is already defined for " <<
           element->fetchNamesBetweenQuotes () <<
          " in handler \"" <<
          fHandlerHeader <<
          "\"";

// 			gLogStream << s.str () << std::endl ; abort (); // JMI TEST v0.9.65

//       oahInternalError (s.str ()); // JMI TEMP v0.9.65
//       oahInternalWarning (s.str ());
    }

    // is elementShortName already in the elements names std::map?
    if (name == elementShortName) {
      if (elementShortName.size ()) {
        std::stringstream s;

        s <<
          "element short name \"" << elementShortName << "\"" <<
          " for element long name \"" << elementLongName << "\"" <<
          " is already defined for " <<
           element->fetchNamesBetweenQuotes () <<
          " in handler \"" <<
          fHandlerHeader <<
          "\"";

// 			gLogStream << s.str () << std::endl ; abort (); // JMI TEST v0.9.65

//         oahInternalError (s.str ()); // JMI TEMP v0.9.65
//         oahInternalWarning (s.str ());
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

  // register element's names in names to elements std::map
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Registering the names in handler \"" <<
      fHandlerHeader <<
      "\" in its names to elements std::map" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // check the handler groups options consistency
  if (fHandlerGroupsList.size ()) {
    for (
      std::list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
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


#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    if (false) // JMI
      displayNamesToElementsMapShort (gLogStream);

      gLogStream <<
        "All the names in handler \"" <<
        fHandlerHeader <<
        "\" have been registered in the names to elements std::map" <<
        std::endl;
  }
#endif

  --gIndenter;
}

void oahHandler::appendElementToElementsList (
  S_oahElement element)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    if (true) // JMI
      gLogStream <<
        "Registering element " <<
        element->fetchNamesBetweenQuotes () <<
        " in \"" <<
        fHandlerServiceName <<
        "\" handler \"" <<
        fHandlerHeader <<
        "\"" <<
        std::endl;
  }
#endif

  // insert element into the registered elements std::list
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments std::vector for " <<
        fHandlerServiceName <<
        ":" <<
        std::endl;

      ++gIndenter;

//       gLogStream <<
//         fOptionsAndArguments;

      gLogStream << std::endl;

      --gIndenter;
    }
#endif

    oahInternalWarning (
      "the arguments std::vector should be empty with options and arguments???");
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        std::endl;
    }
#endif
  }
}

//______________________________________________________________________________
void oahHandler::createTheCommonPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the common prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Checking no input source in argument std::vector in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  const std::vector<std::string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  size_t argumentsNumber =
    argumentsVector.size ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalOahEarlyOptions.getTraceEarlyOptions ()
      &&
    ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
  ) {
    if (argumentsNumber > 0) {
      gLogStream <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments std::vector for " <<
        fHandlerServiceName <<
        ":" <<
        std::endl;

      ++gIndenter;

      for (size_t i = 0; i < argumentsNumber; ++i) {
       gLogStream <<
          i << " : FII " << argumentsVector [i] << // JMIJMIJMI
          std::endl;
      } // for

      gLogStream << std::endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        std::endl;
    }
  }
#endif

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      // fine, do nothing
      break;

    default:
      {
        std::stringstream s;

        s <<
          fHandlerServiceName <<
          " doesn't expect arguments, only options can be used";

        std::string message = s.str ();

        gLogStream <<
          message <<
          std::endl;

        throw mfOahException (message);
      }
  } //  switch
}

//______________________________________________________________________________
void oahHandler::checkNoOrOneInputSourceInArgumentsVector () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Checking no or one input source in argument std::vector in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  const std::vector<std::string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  size_t argumentsNumber =
    argumentsVector.size ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalOahEarlyOptions.getTraceEarlyOptions  ()
      &&
    ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
  ) {
    if (argumentsNumber > 0) {
      gLogStream <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments std::vector for " <<
        fHandlerServiceName <<
        ":" <<
        std::endl;

      ++gIndenter;

      for (size_t i = 0; i < argumentsNumber; ++i) {
        gLogStream <<
          i << " : FUU " << argumentsVector [i] <<
            std::endl;
      } // for

      gLogStream << std::endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        std::endl;
    }
  }
#endif

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      // fine, use standard input
      gGlobalServiceRunData->setInputSourceName (
        "-");
      break;

    case 1:
      // register intput file name
      gGlobalServiceRunData->setInputSourceName (
        argumentsVector [0]);
      break;

    default:
      {
        std::stringstream s;

        s <<
          "Several input file names supplied to " <<
          fHandlerServiceName <<
          ", only the first one, \"" <<
          argumentsVector [0] <<
          "\", will be converted "; //JMI

        std::string message = s.str ();

        for (size_t i = 1; i < argumentsNumber; ++i) {
          gLogStream <<
            argumentsVector [i];

          if (i == argumentsNumber - 1) {
            gLogStream << " and ";
          }
          else {
            gLogStream << ", ";
          }
        } // for

        gLogStream <<
          message <<
          " ignored" <<
          std::endl << std::endl;

        // register intput file name
        gGlobalServiceRunData->setInputSourceName (
          argumentsVector [0]);
      }
      break;
  } //  switch
}

//______________________________________________________________________________
void oahHandler::checkSingleInputSourceInArgumentsVector () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Checking single input source in argument std::vector in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  const std::vector<std::string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  size_t argumentsNumber =
    argumentsVector.size ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalOahEarlyOptions.getTraceEarlyOptions  ()
      &&
    ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
  ) {
    if (argumentsNumber > 0) {
      gLogStream <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments std::vector for " <<
        fHandlerServiceName <<
        ":" <<
        std::endl;

      ++gIndenter;

//       gLogStream <<
//         fOptionsAndArguments;

      gLogStream << std::endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        std::endl;
    }
  }
#endif

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      if (! fOahHandlerFoundAHelpOption) {
        std::string message =
          "Input file name or '-' for standard input expected";

        gLogStream <<
          message <<
          std::endl;

        throw mfOahException (message);
      }
      break;

    case 1:
      // register intput file name
      gGlobalServiceRunData->setInputSourceName (
        argumentsVector [0]);
      break;

    default:
      if (! fOahHandlerFoundAHelpOption) {
        std::stringstream s;

        s <<
          "Several input file names supplied to " <<
          fHandlerServiceName <<
          ", only one can be used";

        std::string message = s.str ();

        gLogStream <<
          message <<
          std::endl;

        mfDisplayStringsVector (
          argumentsVector,
          "The arguments std::vector",
          gLogStream);

        throw mfOahException (message);
      }
      break;
  } //  switch
}

void oahHandler::setOahHandlerFoundAHelpOption (
  const std::string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Setting oahHandlerFoundAHelpOption in handler \"" <<
      fHandlerHeader <<
      "\", context: " <<
      context <<
      std::endl;
  }
#endif

  fOahHandlerFoundAHelpOption = true;
}

std::string oahHandler::fetchOutputFileNameFromTheOptions () const
{
  return "*** no output file name ***"; // for MFSL v0.9.62
}

void oahHandler::checkHandlerOwnOptionsConsistency ()
{}

void oahHandler::checkHandlerOptionsConsistency ()
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalOahEarlyOptions.getEarlyTracePasses ()
      ||
    gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()
  ) {
    gLogStream <<
      "oahHandler: checking the consistency of the options in \"" <<
      fHandlerServiceName <<
      "\" handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // check the handler groups options consistency
  if (fHandlerGroupsList.size ()) {
    for (
      std::list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
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
  gLogStream <<
    "The names to elements std::map contains " <<
    mfSingularOrPlural (
      fHandlerNamesToElementsMap.size (), "element", "elements");

  if (fHandlerNamesToElementsMap.size ()) {
    gLogStream << ":" << std::endl;
    ++gIndenter;

    for (
      std::map<std::string, S_oahElement>::iterator i =
        fHandlerNamesToElementsMap.begin ();
      i != fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      std::string name          = (*i).first;
      S_oahElement element = (*i).second;

      gLogStream <<
        "\"" <<
        name <<
        "\":" <<
        std::endl;

      ++gIndenter;

      gLogStream <<
        element <<
        std::endl;

      --gIndenter;
    } // for

    --gIndenter;
  }
  else {
    gLogStream << std::endl;
  }
}

void oahHandler::displayNamesToElementsMapShort (std::ostream& os) const
{
  gLogStream <<
    "The names to elements std::map contains (short version) " <<
    mfSingularOrPlural (
      fHandlerNamesToElementsMap.size (), "element", "elements");

  if (fHandlerNamesToElementsMap.size ()) {
    gLogStream <<
      ":" <<
      std::endl;
    ++gIndenter;

    const int fieldWidth = 40;

    for (
      std::map<std::string, S_oahElement>::const_iterator i =
        fHandlerNamesToElementsMap.begin ();
      i != fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      std::string       name    = (*i).first;
      S_oahElement element = (*i).second;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        std::string ("\"") + name + "\"" <<
        ": " <<
        element->fetchNamesBetweenQuotes () <<
        std::endl;
    } // for

    --gIndenter;
  }
  else {
    gLogStream << std::endl;
  }
}

void oahHandler::displayElementUsesList (std::ostream& os) const
{
  gLogStream <<
    "The handler elements uses std::list contains " <<
    mfSingularOrPlural (
      fElementUsesList.size (), "element", "elements");

  if (fElementUsesList.size ()) {
    gLogStream << ":" << std::endl;
    ++gIndenter;

    int counter = 0;
    for (
      std::list<S_oahElementUse >::const_iterator i =
        fElementUsesList.begin ();
      i != fElementUsesList.end ();
      ++i
    ) {
      S_oahElementUse elementUse = (*i);

      // sanity check
      if (false) // JMI
      mfAssert (
        __FILE__, __LINE__,
        elementUse != nullptr,
        "elementUse is null");

      ++counter;

      gLogStream <<
        std::right << std::setw (2) << counter <<
        ": FOO" <<
        std::endl;

      ++gIndenter;

      if (elementUse) { // JMI`JMIJMI
        gLogStream <<
          elementUse <<
          std::endl;
      }
      else {
        gLogStream <<
          "null" <<
          std::endl;
      }

      --gIndenter;
    } // for

    gLogStream << std::endl;

    --gIndenter;
  }
  else {
    gLogStream << std::endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHandler::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahHandler>*
    p =
      dynamic_cast<visitor<S_oahHandler>*> (v)) {
        S_oahHandler elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHandler::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahHandler::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHandler::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahHandler>*
    p =
      dynamic_cast<visitor<S_oahHandler>*> (v)) {
        S_oahHandler elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHandler::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahHandler::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHandler::browseData ()" <<
      std::endl;
  }
#endif

  // browse the prefixes
  if (fHandlerPrefixesMap.size ()) {
    for (
      std::map<std::string, S_oahPrefix>::const_iterator i = fHandlerPrefixesMap.begin ();
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
      std::list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
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

void oahHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

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
    os << "[NONE]" << std::endl;
  }

  // launch command
  // --------------------------------------

  os << std::left <<
    std::setw (fieldWidth) << "Launch command:" <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    std::setw (fieldWidth) << "fLaunchCommandAsSupplied" << " : " <<
    fLaunchCommandAsSupplied <<
    std::endl <<
    std::setw (fieldWidth) << "fLaunchCommandWithLongOptionsNames" << " : " <<
    fLaunchCommandWithLongOptionsNames <<
    std::endl <<
    std::setw (fieldWidth) << "fLaunchCommandWithShortOptionsNames" << " : " <<
    fLaunchCommandWithShortOptionsNames <<
    std::endl <<
    std::setw (fieldWidth) << "fLaunchOptionsAndOrArgumentsAlreadyMet" << " : " <<
    fLaunchOptionsAndOrArgumentsAlreadyMet <<
    std::endl;


  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "oahHandlerFoundAHelpOption" << " : " <<
    fOahHandlerFoundAHelpOption <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fHandlerUsedThruKind" << " : " <<
    oahHandlerUsedThruKindAsString (
      fHandlerUsedThruKind) <<
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

    std::list<S_oahGroup>::const_iterator
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

void oahHandler::printShort (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "Handler:" <<
    std::endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  os << std::left <<
    std::setw (fieldWidth) <<
    "oahHandlerFoundAHelpOption" << " : " <<
    fOahHandlerFoundAHelpOption <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fHandlerUsedThruKind" << " : " <<
    oahHandlerUsedThruKindAsString (
      fHandlerUsedThruKind) <<
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

    std::list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      (*i)->printShort (os);
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
  const int fieldWidth = 27;

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

    std::list<S_oahGroup>::const_iterator
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

    std::list<S_oahGroup>::const_iterator
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

    std::list<S_oahGroup>::const_iterator
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
  S_oahSubGroup subGroup) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    os <<
      "oahHandler::printHandlerAndGroupAndSubGroupSpecificHelp" <<
      std::endl;
  }
#endif

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    ":" <<
    std::endl;

  // print the optons group subgroups specific help
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    std::list<S_oahGroup>::const_iterator
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    os << "Printing name \"" <<
      name <<
      "\" introspective help in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

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

    gLogStream <<
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

  // second, check whether mame is known in the names to elements std::map
  // is name known in the elements std::map?
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
      gLogStream <<
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
      gLogStream <<
        "--- Help for subgroup \"" <<
        name <<
        "\" in group \"" <<
        group->getGroupHeader () <<
        "\" ---" <<
        std::endl;

      ++gIndenter;

      group->
        printGroupAndSubGroupHelp (
          gLogStream,
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
      gLogStream <<
        "--- Help for atom \"" <<
        name <<
        "\" in subgroup \"" <<
        subGroup->
          getSubGroupHeader () <<
        "\"";
      if (group->getGroupHeaderIsToBeWritten ()) {
        gLogStream <<
          " of group \"" <<
          group->getGroupHeader () <<
          "\" ---";
      }
      gLogStream << std::endl;

      ++gIndenter;

      group->
        printGroupAndSubGroupAndAtomHelp (
          gLogStream,
          subGroup,
          atom);

      --gIndenter;

      gIndenter.setIndentation (saveIndent);
    }

    else {
      if (! suffixHelpHasBeenProvided) { // JMI ONH
        std::stringstream s;

        s <<
          "cannot provide specific help about name \"" <<
          name <<
          "\"";

        oahInternalError (s.str ());
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

const std::string oahHandler::containingFindableElementAsString () const
{
	std::stringstream s;

  s << std::left <<
    "Handler in service \"" <<
    fHandlerServiceName <<
    "\"";

	return s.str ();
}

Bool oahHandler::findStringInFindableElement (
	const std::string&               lowerCaseString,
	std::list<S_oahFindStringMatch>& foundMatchesList,
	std::ostream&                    os) const
{
	Bool result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    os << "Finding std::string \"" <<
      lowerCaseString <<
      "\" in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

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
    std::stringstream s;

    s <<
      fHandlerHeader <<
      ' ' <<
      fHandlerDescription <<
      ' ' <<
      fHandlerUsage;

    // append the match to foundStringsList
    foundMatchesList.push_back (
      oahFindStringMatch::create (
        s.str (),
        s.str (), // JMI v0.9.66
        containingFindableElementAsString ()));

    result = true;
  }

  // do this handler's prefixes match?
  if (fHandlerPrefixesMap.size ()) {
    ++gIndenter;

    for (
      std::map<std::string, S_oahPrefix>::const_iterator i =
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    os << "Including options file \"" <<
      optionsFileName <<
      "\" in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  // open options file
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    gLogStream <<
      "Opening options file \"" << optionsFileName << "\" for reading" <<
      std::endl;
  }
#endif

  std::ifstream
    optionsStream (
      optionsFileName.c_str (),
      std::ifstream::in);

  if (! optionsStream.is_open ()) {
    std::stringstream s;

    s <<
      "Could not open options file \"" <<
      optionsFileName <<
      "\" for reading - quitting";

    std::string message = s.str ();

    gLogStream <<
      message <<
      std::endl;

    throw mfOahException (message);
  }

  // do the include
  includeOptionsFromFile (
    optionsStream,
    fOptionsAndArguments);

  // close options file
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    gLogStream <<
      "Closing options file \"" << optionsFileName << "\"" <<
      std::endl;
  }
#endif

  optionsStream.close ();

  //  print the options and arguments found
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) { // JMI
//     gLogStream <<
//       fOptionsAndArguments;
  }
#endif
}

void oahHandler::printHandlerOptionsAndArguments (
  std::ostream& os) const
{
  displayOptionsAndArguments (os);
}

void oahHandler::printHandlerOptionsValues (
  std::ostream& os) const
{
#ifdef TRACING_IS_ENABLED
  // print the options handler values header
  size_t
    handlerCommandLineElementsMultisetSize =
      fHandlerCommandLineElementsMultiset.size ();

  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) { // JMI
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
  }
#endif

  os <<
    "The options values for ";

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    os <<
      "\"" <<
      fHandlerHeader <<
      "\"";
  }
  else {
    os << fHandlerServiceName;
  }
#endif

  if (gGlobalOahEarlyOptions.getEarlyInsiderOption ()) {
    os <<
      " (insider mode)";
  }

  os <<
    " are:" <<
    std::endl;

  // print the options groups values
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    std::list<S_oahGroup>::const_iterator
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
#ifdef TRACING_IS_ENABLED
  // print the options handler values header
  size_t
    handlerCommandLineElementsMultisetSize =
      fHandlerCommandLineElementsMultiset.size ();

  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) { // JMI
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
  }
#endif

  os <<
    "All the options values for ";
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    os <<
      "\"" <<
      fHandlerHeader <<
      "\"";
  }
  else {
    os << fHandlerServiceName;
  }
#endif
  os <<
    " are:" <<
    std::endl;

  // print the options groups values
  if (fHandlerGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_oahGroup>::const_iterator
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
    "The include file names std::stack contains " <<
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

void oahHandler::registerPrefixInHandler (
  S_oahPrefix prefix)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    prefix != nullptr,
    "prefix is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Registering prefix " <<
      prefix->fetchPrefixNames () <<
      " in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  std::string prefixName = prefix->getPrefixName ();

  // is prefixName already known in prefixes std::map?
  std::map<std::string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        prefixName);

  if (it != fHandlerPrefixesMap.end ()) {
    // prefixName is already known in the std::map
    std::stringstream s;

    s <<
      " prefix name \"" <<
      prefixName <<
      "\" is already known in OAH handler \"" <<
      fHandlerHeader <<
      "\"";

    oahError (s.str ());
  }

  // register prefix in the options prefixes std::map
  fHandlerPrefixesMap [prefix->getPrefixName ()] =
    prefix;
}

S_oahPrefix oahHandler::fetchNameInPrefixesMap (
  const std::string& prefixName) const
{
  S_oahPrefix result;

  // is prefixName already known in prefixes std::map?
  std::map<std::string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        prefixName);

  if (it != fHandlerPrefixesMap.end ()) {
    // prefixName is already known in the std::map
    result = (*it).second;
  }

  return result;
}

void oahHandler::appendGroupToHandler (
  S_oahGroup group)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Appending group \"" <<
      group->getGroupHeader () <<
      "\" to handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  // append group to elements std::list
  group->
    appendGroupToElementsList (this);

  // append group to groups std::list
  fHandlerGroupsList.push_back (group);

  // set the group upLink to this handler
  group->
    setUpLinkToHandler (this);
}

void oahHandler::prependGroupToHandler (
  S_oahGroup group)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Prepending group \"" <<
      group->getGroupHeader () <<
      "\" to handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  // prepend group  to elements std::list
  group->
    appendGroupToElementsList (this);

  // prepend group to the groups std::list
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

  if (gGlobalOahEarlyOptions.getEarlyInsiderOption ()) {
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
      std::map<std::string, S_oahPrefix>::const_iterator i =
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

  if (gGlobalOahEarlyOptions.getEarlyInsiderOption ()) {
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
    std::set<std::string>::const_iterator
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
      "[NONE]" <<
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
//     "Some options expecting a value can use a default value:" <<
//     std::endl;
//
//   ++gIndenter;
//
// 	gIndenter.indentMultiLineString ( JMI v0.9.66
// 		foundString,
// 		os);
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

  // print the names to elements std::map
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
    std::map<std::string, S_oahElement>::const_iterator
      iBegin = fHandlerNamesToElementsMap.begin (),
      iEnd   = fHandlerNamesToElementsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i).first << "\" ==>" <<
        std::endl;

      ++gIndenter;

      (*i).second->
        printOptionHeader (
          os);

      --gIndenter;

      if (++i == iEnd) break;
    } // for
  }
  else {
    os <<
      "[NONE]" <<
      std::endl;
  }

  --gIndenter;

  // create a std::list of the options std::map elements names
  std::list<std::string> optionsMapElementsNamesList;

  size_t optionsMapElementsNamesListSize =
    optionsMapElementsNamesList.size ();

  if (optionsMapElementsNamesListSize) {
    for (
      std::map<std::string, S_oahElement>::const_iterator i = fHandlerNamesToElementsMap.begin ();
      i!= fHandlerNamesToElementsMap.end ();
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
    std::list<std::string>::const_iterator
      iBegin = optionsMapElementsNamesList.begin (),
      iEnd   = optionsMapElementsNamesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\" ==>" <<
        std::endl;

      if (++i == iEnd) break;
    } // for
  }
  else {
    os <<
      "[NONE]" <<
      std::endl;
  }

  --gIndenter;
}

S_oahElement oahHandler::fetchElementByNameInHandler (
  const std::string& name)
{
  S_oahElement result;

  for (
    std::list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
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

void oahHandler::checkMissingPendingArgvAtomExpectingAValueValue ( // JMIJMIJMI
  const std::string& lastOptionNameFound,
  const std::string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    ++gIndenter;

    gLogStream <<
      "Checking missing pending argv atom expecting a value \"" <<
      lastOptionNameFound <<
      "\", the pending atom expecting a value is:";

    if (fPendingArgvAtomExpectingAValue) {
      gLogStream << std::endl;
      ++gIndenter;

      gLogStream <<
        fPendingArgvAtomExpectingAValue;

      --gIndenter;
    }
    else {
      gLogStream <<
        " null" <<
        std::endl;
    }

    gLogStream << std::endl;
    --gIndenter;
  }

/* JMI
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      ", fHandlerOptionalValuesStyleKind: " <<
      oahOptionalValuesStyleKindAsString (
        fHandlerOptionalValuesStyleKind) <<
      ", fElementValueKind: " <<
      oahElementValueKindAsString (
        fElementValueKind) <<
      std::endl;
  }
#endif
*/
#endif

  if (fPendingArgvAtomExpectingAValue) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "Checking missing value for pending atom expecting a value \"" <<
        lastOptionNameFound <<
        "\", the name pending atom expecting a value is: " <<
        "\"" << fNameUsedForPendingArgvAtomExpectingAValue << "\"" <<
        std::endl;
    }
#endif

    // handle the valued atom using the default value
    switch (fPendingArgvAtomExpectingAValue->getElementValueKind ()) {
    case oahElementValueKind::kElementValueUnknown:
			unknownOptionValueKindError (
				lastOptionNameFound);
    	break;

      case oahElementValueKind::kElementValueWithout:
      case oahElementValueKind::kElementValueImplicit:
        unknownOptionNameError (
          lastOptionNameFound,
          "does not expect an optional value, cannot be used with a '='");
        break;

      case oahElementValueKind::kElementValueMandatory:
        {
          std::stringstream s;

          s <<
            "Pending last argv atom expecting a value " <<
            fPendingArgvAtomExpectingAValue->fetchNamesBetweenQuotes () <<
            " used under name \"" <<
            fNameUsedForPendingArgvAtomExpectingAValue <<
            "\" expects a value" <<
            std::endl;

          oahError (s.str ());
          }
        break;

      case oahElementValueKind::kElementValueOptional:
#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
          gLogStream <<
            "Applying default value to pending atom expecting a value \"" <<
            lastOptionNameFound <<
            "\", the name pending atom expecting a value is: " <<
            "\"" << fNameUsedForPendingArgvAtomExpectingAValue << "\"" <<
            std::endl;
        }
#endif

        fPendingArgvAtomExpectingAValue->
          applyAtomWithDefaultValue (
            gLogStream); // oahHandler-specific log stream??? JMI
        break;
    } // switch

    // forget about this pending atom expecting a value
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "Forgetting about the pending argv atom expecting a value when handling atom name \"" <<
        lastOptionNameFound <<
        "\"" <<
        std::endl;
    }
#endif

    fPendingArgvAtomExpectingAValue = nullptr;
    fNameUsedForPendingArgvAtomExpectingAValue = "";
  }

  else {
    // no atom expecting a value is pending
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "No argv atom expecting a value is pending when handling atom name \"" <<
        lastOptionNameFound <<
        "\"" <<
        std::endl;
    }
#endif
  }
}

void oahHandler::registerOahElementUse (
  S_oahElement  elementUsed,
  const std::string& nameUsed,
  const std::string& valueUsed)
{
  // sanity check
//   if (false) // JMI
  mfAssert (
    __FILE__, __LINE__,
    elementUsed != nullptr,
    "elementUsed is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Registering element use " <<
      elementUsed->fetchNamesBetweenQuotes () <<
      ", nameUsed: \"" << nameUsed << "\"" <<
      ", valueUsed: \"" << valueUsed << "\"" <<
      std::endl;
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  if (
    elementUsed == gGlobalTracingOahGroup->getTracingOahBooleanAtom ()
  ) {
    gGlobalTracingOahGroup->setTracingOah ();
  }
#endif

  // should we apply '-include' right now? JMIJMIJMI
  if (
    elementUsed == gGlobalOahOahGroup->getIncludeOptionsAndArgumentsFileAtom ()
  ) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
      gLogStream <<
        "Registering element use " <<
        elementUsed->fetchNamesBetweenQuotes () <<
        ", nameUsed: \"" << nameUsed << "\"" <<
        ", valueUsed: \"" << valueUsed << "\"" <<
        " leads to include this options and arguments file now" <<
        std::endl;
    }
#endif

//     displayOptionsAndArguments (gLogStream); // JMIJMIJMI

    includeOptionsAndArgumentsFile (
      valueUsed,
      gLogStream);

//     displayOptionsAndArguments (gLogStream);// JMIJMIJMI
  }
}

void oahHandler::registerAtomUse (
  S_oahAtom     atomUsed,
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
  S_oahElement  elementUsed,
  const std::string& nameUsed,
  const std::string& valueUsed)
{
  // sanity check
//   if (false) // JMI
  mfAssert (
    __FILE__, __LINE__,
    elementUsed != nullptr,
    "elementUsed is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Registering element use " <<
      elementUsed->fetchNamesBetweenQuotes () <<
      ", nameUsed: \"" << nameUsed << "\"" <<
      ", valueUsed: \"" << valueUsed << "\"" <<
      " in command line" <<
      std::endl;
  }
#endif

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
  size_t equalsSignPosition,
  const std::string& stringAfterEqualsSign)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Handling option prefix name \"" <<
      prefixName <<
      ", equalsSignPosition = \"" << equalsSignPosition << "\"" <<
      ", stringAfterEqualsSign = \"" << stringAfterEqualsSign << "\"" <<
      std::endl;
  }
#endif

  // split stringAfterEqualsSign into a std::list of strings
  // using the comma as separator
  std::list<std::string> chunksList;

  mfSplitStringIntoChunks (
    stringAfterEqualsSign,
    ",",
    chunksList);

  size_t chunksListSize = chunksList.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "There are " << chunksListSize << " chunk[s]" <<
      " in '" << stringAfterEqualsSign <<
      "':" <<
      std::endl;

    ++gIndenter;

    std::list<std::string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      gLogStream <<
        '[' << (*i) << ']';
      if (++i == iEnd) break;
      gLogStream <<
        ' ';
    } // for

    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  S_oahPrefix
    prefix =
      fetchNameInHandlerPrefixesMap (prefixName);

  if (prefix) {
    if (chunksListSize) {
      // expand the option names contained in chunksList
      for (
        std::list<std::string>::const_iterator i =
          chunksList.begin ();
        i != chunksList.end ();
        ++i
      ) {
        std::string singleOptionName = (*i);

        // build uncontracted option name
        std::string
          uncontractedOptionName =
            prefix->getPrefixErsatz () + singleOptionName;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
          gLogStream <<
            "Expanding option \"" << singleOptionName <<
            "\" to \"" << uncontractedOptionName <<
            "\"" <<
            std::endl;
        }
#endif

        // handle the uncontracted option name
        handleOptionNameCommon (
          uncontractedOptionName);
      } // for
    }
  }

  else {
    std::stringstream s;

    s <<
      "option prefix \"" << prefixName <<
      "\" is unknown to \"" <<
        fHandlerHeader <<
        "\", see help summary below";

    printKnownPrefixes (s);

 // JMI   oahError (s.str ());
    oahWarning (s.str ());
  }
}

Bool oahHandler::isNameASingleCharacterOptionsCluster (
  const std::string& optionName)
{
  Bool result (true); // until the contrary is known

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Checking whether optionName \"" <<
      optionName <<
      "\" is a single-character options cluster" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    optionName.size () != 0,
    "optionName.size () == 0");

  ++gIndenter;

  std::list<S_oahElement> clusterElementsList;

  // collect single-character elements in clusterElementsList
  for (
    std::string::const_iterator i = optionName.begin ();
    i != optionName.end ();
    ++i
  ) {
    std::string singleCharacterString (1, (*i));

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
      gLogStream <<
        "Considering single-character '" << singleCharacterString << "'" <<
        std::endl;
    }
#endif

    std::set<std::string>::const_iterator
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

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
      gLogStream << std::endl;
    }
#endif

  size_t clusterElementsListSize =
    clusterElementsList.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "clusterElementsListSize = " << clusterElementsListSize <<
      std::endl;

    gLogStream <<
      "clusterElementsList:" <<
      std::endl;

    ++gIndenter;

    for (
      std::list<S_oahElement>::const_iterator i = clusterElementsList.begin ();
      i != clusterElementsList.end ();
      ++i
    ) {
      S_oahElement element = (*i);

      if (element) { // JMI should not be necessary...
        gLogStream <<
          element->
            fetchNamesBetweenQuotes () <<
          std::endl;
      }
    } // for

    --gIndenter;

    gLogStream <<
      "fSingleCharacterNamesSet:" <<
      std::endl;
    for (
      std::set<std::string>::const_iterator i = fSingleCharacterNamesSet.begin ();
      i != fSingleCharacterNamesSet.end ();
      ++i
    ) {
      std::string singleCharacterElementName = (*i);

      gLogStream <<
        "\"" <<
        singleCharacterElementName <<
        "\" ";
    } // for
    gLogStream << std::endl;
  }
#endif

  size_t optionNameSize =
    optionName.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "optionNameSize = " << optionNameSize <<
      std::endl;
  }
#endif

  if (
    clusterElementsListSize
      &&
    clusterElementsListSize == optionNameSize
  ) {
    // all the elements in clusterElementsList are single character options,
    // handle them
    for (
      std::list<S_oahElement>::const_iterator i = clusterElementsList.begin ();
      i != clusterElementsList.end ();
      ++i
    ) {
      S_oahElement element = (*i);

      // handle element name
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      if (element) { // JMI should not be necessary...
        gLogStream <<
          "Handling single-character options cluster element " <<
          element->asString () <<
          std::endl;
      }
    }
#endif

    handleOptionNameCommon (
      element->getShortName ());
    } // for
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "OptionName \"" <<
        optionName <<
        "\" is no single-character options cluster" <<
        std::endl;
    }
#endif
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Deciphering option name \"" <<
      optionName <<
      "\" that contains an  equal sign at position " <<
      equalsSignPosition <<
      ", stringBeforeEqualsSign = \"" << stringBeforeEqualsSign <<
      ", stringAfterEqualsSign = \"" << stringAfterEqualsSign << "\"" <<
      std::endl;
  }
#endif

  // prefixes have precedence over options with optional values
  S_oahPrefix
    prefix =
      fetchNameInHandlerPrefixesMap (
        stringBeforeEqualsSign);

  if (prefix) {
    // handle prefix name
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "Option name \"" <<
        stringBeforeEqualsSign <<
        "\" is a prefix" <<
        std::endl;
    }
#endif

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
        // oahStringWithDefaultValueAtom?
        S_oahStringWithDefaultValueAtom
          stringWithDefaultValueAtom =
            dynamic_cast<oahStringWithDefaultValueAtom*>(&(*element))
      ) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
          gLogStream <<
            "Option \"" <<
            stringBeforeEqualsSign <<
            "\" is a stringWithDefaultValueAtom" <<
            std::endl;
        }
#endif

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling option name \"" <<
      optionName <<
      "\" with value \"" <<
      valueUsed <<
      "\"" <<
      std::endl;
  }
#endif

  std::string
    trailerAfterFirstDash =
      optionName.substr (1, std::string::npos);

  // is optionName known in options std::map?
  S_oahElement
    element =
      fetchNameInNamesToElementsMap (optionName);

  if (element) {
    // optionName is known, let's handle it
#ifdef TRACING_IS_ENABLED
if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  gLogStream <<
    "Handling option name \"" <<
    optionName <<
    "\" which is known to handler \"" <<
    fHandlerHeader <<
    "\"" <<
    std::endl;
}
#endif

    handleKnownOptionsVectorElement (
      element,
      optionName,
      valueUsed);
  }

  else {
    // optionName is unknown
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
      if (false) // JMI
        this->printHelp (gOutputStream);

//       unknownOptionNameError (
      unknownOptionNameWarning (
        optionName,
        "unknown to handler 2 ???");
    }
#endif

    handleUnknownOptionName (
      optionName);
  }
}

std::string oahHandler::handleArgvOptionName (
  const std::string& optionName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling argv option name \"" <<
      optionName <<
      "\"" <<
      std::endl;
  }
#endif

  std::string currentOptionName;

  std::string
    trailerAfterFirstDash =
      optionName.substr (1, std::string::npos);

  /* JMI
  gLogStream <<
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

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "\"" << currentOptionName << "\" is a double-dashed option" <<
          std::endl;
      }
#endif
    }
  }

  else {
    // optionName is a single-dashed option
    currentOptionName = trailerAfterFirstDash;

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
      gLogStream <<
        "\"" << currentOptionName << "\" is a single-dashed option" <<
        std::endl;
    }
#endif
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      std::endl <<
      "----------------------------------------------------------" <<
      std::endl <<
      "Handling options and arguments element [" <<
      optionName <<
      "] [" <<
      optionValue << ']' <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    optionName.size () > 0,
    "optionName is empty");

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling argv option value or argument \"" <<
      theString <<
      "\"" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      "The pending atom expecting a value is:";

    if (fPendingArgvAtomExpectingAValue) {
      gLogStream << std::endl;
      ++gIndenter;

      gLogStream <<
        fPendingArgvAtomExpectingAValue;

      --gIndenter;
    }
    else {
      gLogStream << " null";
    }

    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  // options are registered at once, unless they are valued,
  // in which case the handling of the option and its value
  // are postponed until the latter is available

  if (fPendingArgvAtomExpectingAValue) {
    // theString is the value for the pending atom expecting a value

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "The value associated to atom \"" <<
        fNameUsedForPendingArgvAtomExpectingAValue <<
        "\" under the name \"" <<
        fNameUsedForPendingArgvAtomExpectingAValue <<
        "\" in argv is \"" << theString << "\"" <<
        std::endl;
    }
#endif

    if (
      fPendingArgvAtomExpectingAValue
        ==
      gGlobalOahOahGroup->getIncludeOptionsAndArgumentsFileAtom ()
    ) {
      // don't register this atom's use,
      // include the file contents instead
      includeOptionsFileInHandler (
        theString,
        gLogStream);

      // forget about '-include' as the atom expecting a value
      fPendingArgvAtomExpectingAValue = nullptr; // JMIJMIJMI
    }
    else {
      // register this atom's use
      registerAtomUse (
        fPendingArgvAtomExpectingAValue,
        fNameUsedForPendingArgvAtomExpectingAValue,
        theString);
    }

    // forget about this pending atom expecting a value
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
      gLogStream <<
        "handleArgvOptionValueOrArgument() Forgetting about the pending atom expecting a value" <<
        std::endl;
    }
#endif

    fPendingArgvAtomExpectingAValue = nullptr;
    fNameUsedForPendingArgvAtomExpectingAValue = "";
  }

  else {
    // theString is an argument

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream <<
          "\"" << theString << "\" is an argument" <<
          std::endl;
      }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling the options from options and arguments in " <<
      "\"" << fHandlerHeader << "\"" <<
      " for \"" << fHandlerServiceName << "\"" <<
      std::endl;

    gGlobalTracingOahGroup->setTracingOah ();
  }
#endif

  std::string optionsPassDescription =
    "Handle the options from the options and arguments";

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      optionsPassDescription <<
      std::endl;

    gLogStream <<
      separator <<
      std::endl;
  }
#endif

  oahElementHelpOnlyKind result;

  // register all this handler's names in the names to elements std::map
  registerNamesInHandlerToTheNamesToElementsMap ();

  // initialize the history
  // only now because the library must initialized beforehand
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Initializing handler component" <<
      ", serviceName: " << serviceName <<
      std::endl;
  }
#endif

  initializeHandlerMultiComponent ();

  // create the elements uses std::list from the options and arguments
  createElementUsesListFromOptionsAndArguments ( // (pass 2)
    serviceName);

// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     // display the options and arguments
//     gLogStream <<
//       fOptionsAndArguments;
//   }
// #endif

  // apply the options in the elements uses std::list
  result =
    applyOptionsFromElementUsesList (); // (pass 3)

  // check the options and arguments
  checkOptionsAndArgumentsConsistency ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Result oahElementHelpOnlyKind: " <<
      oahElementHelpOnlyKindAsString (result) <<
      std::endl;
  }
#endif

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
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

  std::string optionsAndArgumentsPassDescription =
    "Handle the options and arguments from argc/argv";

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      optionsAndArgumentsPassDescription << " in \"" <<
      fHandlerHeader <<
      "\" for \"" <<
      fHandlerServiceName <<
      "\"" <<
      std::endl;
  }
#endif

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      optionsAndArgumentsPassDescription <<
      std::endl;

    gLogStream <<
      separator <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    mfDisplayArgcAndArgv (
      argc,
      argv,
      gLogStream);
  }
#endif

  // register service name
  fHandlerServiceName = std::string (argv [0]);

  // register all this handler's names in the names to elements std::map
  registerNamesInHandlerToTheNamesToElementsMap ();

  // initialize the history
  // only now because the library must initialized beforehand
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Initializing handler component" <<
      ", fHandlerServiceName: " << fHandlerServiceName <<
      std::endl;
  }
#endif

  initializeHandlerMultiComponent ();

  // create the elements uses std::list from argc/argv (pass 2)
  createElementUsesListFromArgcArgv (
    argc, argv);

  // apply the options in the elements uses std::list
  result =
    applyOptionsFromElementUsesList (); // (pass 3)

  // check the options and arguments
  checkOptionsAndArguments ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "result oahElementHelpOnlyKind: " <<
      oahElementHelpOnlyKindAsString (result) <<
      std::endl;
  }
#endif

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    optionsAndArgumentsPassDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  return result;
}

void oahHandler::createElementUsesListFromOptionsAndArguments (
  const std::string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Creating atom uses std::list from options and arguments in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  switch (fHandlerUsedThruKind) {
    case oahHandlerUsedThruKind::kHandlerUsedThruUnknown: // default value
        fHandlerUsedThruKind =
          oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments;
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv:
      {
        std::stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" has been handled from argc/argv, cannot handle it from oahOptionsVector";

        oahError (s.str ());
      }
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      {
        std::stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" cannot be handled twice from argc/argv";

        oahError (s.str ());
      }
      break;
  } // switch

  ++gIndenter;

// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     gLogStream <<
//       fOptionsAndArguments;
//   }
// #endif

  // register service name
  fHandlerServiceName = serviceName;

//   // analyze the options
//   const std::vector<oahOption>&
//     optionsVector =
//       gGlobalServiceRunData->
//         getOptionsAndArguments ().getOptionsVector ();
//
//   if (optionsVector.size ()) {
//     for (size_t i = 0; i < optionsVector.size (); ++i) {
//         std::string optionName =
//           optionsVector [i].getOptionName ();
//         std::string optionValue =
//           optionsVector [i].getOptionValue ();
//
//       handleOptionNameAndValueAtTopOahLevel (
//         optionName,
//         optionValue);
//     } // for
//   }
//
//   else {
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//       gLogStream <<
//         "The options and arguments is empty in \"" <<
//         fHandlerHeader <<
//         "\"" <<
//         std::endl;
//     }
// #endif
//   }
//
//   gLogStream << std::endl;
//
//   --gIndenter;
//
//   // check the options and arguments
//   checkOptionsAndArgumentsConsistency ();
//
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     displayOptionsAndArguments (gLogStream);
//
//     displayElementUsesList ();
//   }
// #endif
}

void oahHandler::createElementUsesListFromArgcArgv (
  int   argc,
  char* argv[])
{
  // this is OAH handling pass 2 for command line use

  // force early options tracing // TEMP JMIJMIJMI
//   gGlobalOahEarlyOptions.setTraceEarlyOptions ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    if (false) // JMI
      this->printHelp (gOutputStream);

      gLogStream <<
        "Creating atom uses std::list from argc/argv in \"" <<
        fHandlerHeader <<
        "\"" <<
        std::endl;
  }
#endif

  // sanity check
  switch (fHandlerUsedThruKind) {
    case oahHandlerUsedThruKind::kHandlerUsedThruUnknown: // default value
        fHandlerUsedThruKind =
          oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv;
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv:
      {
        std::stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" cannot be handled twice from argc/argv";

        oahError (s.str ());
      }
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      {
        std::stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" has been handled from oahOptionsVector, cannot handle it from argc/argv";

        oahError (s.str ());
      }
      break;
  } // switch

  ++gIndenter;

  // sort out the command options and the arguments
  int n = 1;

  std::string lastOptionNameFound;

  // loop over the contents of argv:
  while (true) {
    if (argv [n] == nullptr)
      break;

    std::string currentString = std::string (argv [n]);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    // print current option
    gLogStream <<
      std::endl <<
      "----------------------------------------------------------" <<
      std::endl <<
      "Handling argv element " <<
      n <<
      ": [" <<
      currentString <<
      ']' <<
      std::endl;
  }
#endif

    ++gIndenter;

    // handle currentString
    if (currentString [0] == '-') {
      // currentString start with a dash: stdin or option?

      if (currentString.size () == 1) {
        // currentString is the stdin indicator, thus an argument
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
        gLogStream <<
          "\"" << currentString << "\" is the '-' stdin indicator" <<
          std::endl;
  }
#endif

        // currentString is an argument
#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
            gLogStream <<
              currentString << " is an argument" <<
              std::endl;
        }
#endif

        // append it to the arguments std::vector
        fOptionsAndArguments.appendArgument (
          currentString);
      }

      else {
        // currentString is an option name
#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
            gLogStream <<
              currentString << " is an option name" <<
              std::endl;
        }
#endif
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

  // is a pending atom expecting a value value missing?
  checkMissingPendingArgvAtomExpectingAValueValue (
    lastOptionNameFound,
    "last option in command line");

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    displayOptionsAndArguments (gLogStream);

    displayElementUsesList (gLogStream);
  }
#endif
}

oahElementHelpOnlyKind oahHandler::applyOptionsFromElementUsesList ()
{
  // this is OAH handling pass 3

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Applying options from elements uses std::list in \"" <<
      fHandlerHeader << "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    displayOptionsAndArguments (gLogStream); // JMIJMIJMI

    displayElementUsesList (gLogStream);
  }
#endif

  if (fElementUsesList.size ()) {
    ++gIndenter;

    int counter = 0;
    for (
      std::list<S_oahElementUse >::const_iterator i =
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

      ++counter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
      gLogStream <<
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

      gLogStream <<
        elementUsed->asShortString () <<
        std::endl;

      --gIndenter;
  }
#endif

      if (elementUsed) {
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        oahElementValueKind
          atomValueExpectedKind =
            elementUsed->getElementValueKind ();

        gLogStream <<
          "Its atomValueExpectedKind is '" <<
          oahElementValueKindAsString (atomValueExpectedKind) <<
          "'" <<
          std::endl;
  }
#endif

        // the heart of it
        if (
          // group?
          S_oahGroup
            group =
              dynamic_cast<oahGroup*>(&(*elementUsed))
        ) {
          group->
            applyElement (
              gOutputStream);
        }

        else if (
          // subgroup?
          S_oahSubGroup
            subGroup =
              dynamic_cast<oahSubGroup*>(&(*elementUsed))
        ) {
          subGroup->
            applyElement (
              gOutputStream);
        }

        else {
          // this is an atom

          S_oahAtom
            atom =
              dynamic_cast<oahAtom*>(&(*elementUsed));

          oahElementValueKind
            atomValueKind =
              atom->
                getElementValueKind ();

          if (
            // atom expecting a value?
            S_oahAtomExpectingAValue
              atomExpectingAValue =
                dynamic_cast<oahAtomExpectingAValue*>(&(*elementUsed))
          ) {
#ifdef TRACING_IS_ENABLED
            if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
              gLogStream <<
                "**** Handling atom expecting a value:" <<
              std::endl;

              ++gIndenter;

              gLogStream <<
                atom->asShortString () <<
                std::endl;

              --gIndenter;
            }
#endif
            switch (atomValueKind) {
							case oahElementValueKind::kElementValueUnknown:
								unknownOptionValueKindError (
									atomExpectingAValue->fetchNamesBetweenQuotes ());
								break;

              case oahElementValueKind::kElementValueWithout:
                {
                  std::stringstream s;

                  s <<
                    "Atom " <<
                    atomExpectingAValue->fetchNamesBetweenQuotes () <<
                    " is not expecting a value";

                  oahInternalError (s.str ());
                }
                break;

              case oahElementValueKind::kElementValueImplicit:
                atomExpectingAValue->
                  applyAtomWithDefaultValue (
                    gOutputStream);
                break;

              case oahElementValueKind::kElementValueMandatory:
                if (valueUsed.size ()) {
                  atomExpectingAValue->
                    applyAtomWithValue (
                      valueUsed,
                      gOutputStream);
                }
                else {
                  std::stringstream s;

                  s <<
                    "Atom expecting a value " <<
                    atomExpectingAValue->fetchNamesBetweenQuotes () <<
                    " expects a non-empty value";

                  oahInternalError (s.str ());
                }
                break;

              case oahElementValueKind::kElementValueOptional:
                if (valueUsed.size ()) {
                  atomExpectingAValue->
                    applyAtomWithValue (
                      valueUsed,
                      gOutputStream);
                }
                else {
                  atomExpectingAValue->
                    applyAtomWithDefaultValue (
                      gOutputStream);
                }
                break;
            } // switch
          }

          else {
  #ifdef TRACING_IS_ENABLED
            if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
              gLogStream <<
                "**** Handling atom not expecting a value:" <<
                std::endl;

              ++gIndenter;

              gLogStream <<
                atom->asShortString () <<
                std::endl;

              --gIndenter;
            }
  #endif

            atom->
              applyElement (
                gOutputStream);
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
        std::stringstream s;

        s <<
          "Element from the from the atom uses std::list for \"" <<
          nameUsed <<
          "\" is null";

        oahInternalError (s.str ());
      }
    } // for

    --gIndenter;
  }

  // print the options and arguments if so chosen
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayOptionsAndArguments ()) {
    printHandlerOptionsAndArguments (
      gLogStream);

    gLogStream << std::endl;
  }
#endif

  // print the early options if so chosen
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayEarlyOptionsValues ()) {
    gGlobalOahEarlyOptions.print (
      gLogStream);

    gLogStream << std::endl;
  }
#endif

  // print the options values if so chosen
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayOptionsValues ()) {
    printHandlerOptionsValues (
      gLogStream);
    gLogStream << std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayOptionsValuesAll ()) {
    printHandlerOptionsValuesAll (
      gLogStream);
    gLogStream << std::endl;
  }
#endif

  // check the consistency of the options
  // ------------------------------------------------------

//   gLogStream <<
//     "==> checkHandlerOptionsConsistency();" << // JMI
//     std::endl;

  checkHandlerOptionsConsistency ();

  --gIndenter;

  // was this run a 'help-only' one?
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    if (false && fOahHandlerFoundAHelpOption) { // JMIJMIJMI
      gLogStream <<
        "This is a help-only run " <<
        std::endl;

      return oahElementHelpOnlyKind::kElementHelpOnlyYes;
    }
    else {
      gLogStream <<
        "This is an actual conversion run " <<
        std::endl;

      // let's go ahead
    }
  }
#endif

  return
    fOahHandlerFoundAHelpOption
      ? oahElementHelpOnlyKind::kElementHelpOnlyYes
      : oahElementHelpOnlyKind::kElementHelpOnlyNo;
}

void oahHandler::handleKnownOptionsVectorElement (
  S_oahElement  element,
  const std::string& optionNameUsed,
  const std::string& valueUsed)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling known options and arguments element " <<
      element->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Element \"" <<
      element->fetchNamesBetweenQuotes () <<
      "\" is described by: " <<
      std::endl <<
      gTab <<
      element->getDescription () <<
      std::endl;
  }
#endif

  // is this element already present in the commande line?
  std::multiset<S_oahElement>::const_iterator
    it =
      fHandlerCommandLineElementsMultiset.find (
        element);

  if (it != fHandlerCommandLineElementsMultiset.end ()) {
    // yes, element is known in the std::list
    if (! element->getMultipleOccurrencesAllowed ()) {
      std::stringstream s;

      s <<
        "options and arguments element \"" <<
        element->fetchNames () <<
        "\" is already present in the options std::vector";

      oahWarning (s.str ());
      // JMIJMIJMI
      //abort();
    }
  }

  // remember this element as occurring in the command line
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Inserting \"" <<
      element->fetchNamesBetweenQuotes () <<
      "\" in fHandlerCommandLineElementsMultiset" <<
      std::endl;
  }
#endif

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
    // atom alias?
    S_oahAtomAlias
      atomAlias =
        dynamic_cast<oahAtomAlias*>(&(*element))
  ) {
    // use the original atom instead
    S_oahAtom
      originalOahAtom =
        atomAlias->getOriginalOahAtom ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
      gLogStream <<
        "Atom name \"" << optionNameUsed << "\" is a alias for \"" <<
        originalOahAtom->asString () <<
        "\", handling the latter" <<
        std::endl;
    }
#endif

    handleKnownOptionsVectorAtom (
      originalOahAtom,
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
    std::stringstream s;

    s <<
      "Known option name \"" <<
      optionNameUsed <<
      "\" with value \"" <<
      valueUsed <<
      "\" cannot be handled";

    oahInternalError (s.str ());
  }
}

void oahHandler::handleKnownArgvElement (
  S_oahElement  element,
  const std::string& optionNameUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling argv known element " <<
      element->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Element " <<
      element->fetchNamesBetweenQuotes () <<
      " is described by: " <<
      std::endl <<
      gTab <<
      element->getDescription () <<
      std::endl;
  }
#endif

  // is this element already present in the commande line?
  std::multiset<S_oahElement, compareOahElements>::const_iterator
    it =
      fHandlerCommandLineElementsMultiset.find (
        element);

  if (it != fHandlerCommandLineElementsMultiset.end ()) {
    // yes, element is known in the std::list
    if (! element->getMultipleOccurrencesAllowed ()) {
      std::stringstream s;

      s <<
        "argv element \"" <<
        element->fetchNames () <<
        "\" is already present in the command line";

      oahWarning (s.str ());
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
    // atom alias?
    S_oahAtomAlias
      atomAlias =
        dynamic_cast<oahAtomAlias*>(&(*element))
  ) {
    // use the original atom instead
    S_oahAtom
      originalOahAtom =
        atomAlias->getOriginalOahAtom ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
      gLogStream <<
        "Atom name \"" << optionNameUsed << "\" is a alias for \"" <<
        originalOahAtom->asString () <<
        "\", handling the latter" <<
        std::endl;
    }
#endif

    handleKnownArgvAtom (
      originalOahAtom,
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
    std::stringstream s;

    s <<
      "Known option name \"" << optionNameUsed << "\" cannot be handled";

    oahInternalError (s.str ());
  }
}

void oahHandler::handleKnownOptionsVectorAtom (
  S_oahAtom     atom,
  const std::string& optionNameUsed,
  const std::string& valueUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling known options and arguments atom " <<
      atom->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      std::endl;
  }
#endif

  switch (atom->getElementValueKind ()) {
    case oahElementValueKind::kElementValueUnknown:
			unknownOptionValueKindError (
				optionNameUsed);
    	break;

    case oahElementValueKind::kElementValueWithout:
    case oahElementValueKind::kElementValueImplicit:
      registerAtomUse (
        atom, optionNameUsed, ""); // "===options and arguments atom without value==="); // JMI to debug
      break;

    case oahElementValueKind::kElementValueMandatory:
      if (
        // options and arguments atom expecting a value?
        S_oahAtomExpectingAValue
          atomExpectingAValue =
            dynamic_cast<oahAtomExpectingAValue*>(&(*atom))
      ) {
        registerAtomUse (
          atom, optionNameUsed, valueUsed);
      }

      else {
        std::stringstream s;

        s <<
          "options and arguments atom " <<
          atom->fetchNamesBetweenQuotes () <<
          " is not expecting a value even though its kind is oahElementValueKind::kElementValueMandatory";

        oahInternalError (s.str ());
      }
      break;

    case oahElementValueKind::kElementValueOptional:
      registerAtomUse (
        atom, optionNameUsed, ""); // "===atom with optional value===" JMI to debug
      break;
  } // switch
}

void oahHandler::handleKnownArgvAtom (
  S_oahAtom     atom,
  const std::string& optionNameUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling known argv atom " <<
      atom->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      std::endl;
  }
#endif

  if (fPendingArgvAtomExpectingAValue) {
    std::stringstream s;

    s <<
      "Pending argv atom expecting a value " <<
      fPendingArgvAtomExpectingAValue->fetchNamesBetweenQuotes () <<
      " used under name \"" <<
      fNameUsedForPendingArgvAtomExpectingAValue <<
      "\" expects a value" <<
      std::endl;

    oahWarning (s.str ()); // JMI
//     oahError (s.str ());
  }

  switch (atom->getElementValueKind ()) {
    case oahElementValueKind::kElementValueUnknown:
			unknownOptionValueKindError (
				optionNameUsed);
    	break;

    case oahElementValueKind::kElementValueWithout:
    case oahElementValueKind::kElementValueImplicit:
      registerAtomUse (
        atom, optionNameUsed, ""); // "===argv atom without value==="); // JMI to debug

      fPendingArgvAtomExpectingAValue = nullptr;
      fNameUsedForPendingArgvAtomExpectingAValue = "";
      break;

    case oahElementValueKind::kElementValueMandatory:
      if (
        // argv atom expecting a value?
        S_oahAtomExpectingAValue
          atomExpectingAValue =
            dynamic_cast<oahAtomExpectingAValue*>(&(*atom))
      ) {
        // delay this argv atom's handling until the value is handled
        fPendingArgvAtomExpectingAValue =
          atomExpectingAValue;
        fNameUsedForPendingArgvAtomExpectingAValue =
          optionNameUsed;
      }

      else {
        std::stringstream s;

        s <<
          "argv atom " <<
          atom->fetchNamesBetweenQuotes () <<
          " is not expecting a value even though its kind is"
          " oahElementValueKind::kElementValueMandatory";

        oahInternalError (s.str ());
      }
      break;

    case oahElementValueKind::kElementValueOptional:
      registerAtomUse (
        atom, optionNameUsed, ""); // "===use default value===" // JMI to debug

      fPendingArgvAtomExpectingAValue = nullptr;
      fNameUsedForPendingArgvAtomExpectingAValue = "";
      break;
  } // switch
}

void oahHandler::handleUnknownOptionName (
  const std::string& optionName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Handling unknown option name \"" <<
      optionName <<
      "\"" <<
      std::endl;
  }
#endif

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
      ""); // JMI do better ???
  }
}

void oahHandler::unknownOptionNameWarning (
  const std::string& name,
  const std::string& context)
{
  std::stringstream s;

  s <<
    "option name \"" <<
    name <<
    "\" is unknown";

  if (gGlobalOahEarlyOptions.getEarlyInsiderOption ()) {
    s <<
      " in handler \"" <<
    fHandlerHeader <<
    "\"";
  }

  s <<
    ", " << context;

  oahWarning (s.str ());
}

void oahHandler::unknownOptionNameError (
  const std::string& name,
  const std::string& context)
{
  std::stringstream s;

  s <<
    "option name \"" <<
    name <<
    "\" is unknown";

  if (gGlobalOahEarlyOptions.getEarlyInsiderOption ()) {
    s <<
      " in handler \"" <<
    fHandlerHeader <<
    "\"";
  }

  s <<
    ", " << context;

  oahError (s.str ());
}

void oahHandler::unknownOptionValueKindError (const std::string& name)
{
  std::stringstream s;

  s <<
    "the value kind of option \"" <<
    name <<
    "\" is unknown";

  if (gGlobalOahEarlyOptions.getEarlyInsiderOption ()) {
    s <<
      " in handler \"" <<
    fHandlerHeader <<
    "\"";
  }

  oahError (s.str ());
}


void oahHandler::handleOptionNameCommon (
  const std::string& optionName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Common handling of option name \"" <<
      optionName <<
      "\"" <<
      std::endl;
  }
#endif

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
    // is optionName known in options std::map?
    S_oahElement
      element =
        fetchNameInNamesToElementsMap (optionName);

    if (element) {
      // optionName is known, let's handle it
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Handling option name \"" <<
      optionName <<
      "\" which is known to handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

      handleKnownArgvElement (
        element,
        optionName);
    }

    else {
      // optionName is unknown
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        if (false) // JMI
          this->printHelp (gOutputStream);

//        unknownOptionNameError (
        unknownOptionNameWarning (
          optionName,
          "unknown to handler 3 ???");
      }
#endif

      handleUnknownOptionName (
        optionName);
    }
  }
}




// void oahHandler::analyzeOptionsAndArguments (
//   std::string                   serviceName,
//   const oahOptionsAndArguments& optionsAndArguments)
// {
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     gLogStream <<
//       optionsAndArguments;
//   }
//
//   // analyze the options
//   const std::vector<oahOption>&
//     optionsVector =
//       fOptionsAndArguments.getOptionsVector ();
//
//   if (optionsVector.size ()) {
//     for (size_t i = 0; i < optionsVector.size (); ++i) {
//       std::string optionName =
//         optionsVector [i].getOptionName ();
//       std::string optionValue =
//         optionsVector [i].getOptionValue ();
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
//         if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//           gLogStream <<
//             "argumentIsAnOption, " <<
//             serviceName << " main()" <<
//             ", argumentWithoutDash: '" << argumentWithoutDash <<
//             "'" <<
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
//         if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//           gLogStream <<
//             serviceName <<
//             ", analyzeOptionsAndArguments()" <<
//             ", argumentWithoutDash: '" <<
//             argumentWithoutDash <<
//             ", argumentWithoutDashStartsWithAPrefix: '" <<
//             argumentWithoutDashStartsWithAPrefix <<
//             ", argumentWithoutDashToBeUsed: '" <<
//             argumentWithoutDashToBeUsed <<
//             "'" <<
//             std::endl;
//         }
//
//         // apply argumentWithoutDashToBeUsed early if relevant
//         gGlobalOahEarlyOptions.applyEarlyOptionIfRelevant (
//           argumentWithoutDashToBeUsed,
//           optionValue);
//       }
//     } //for
//   }
//
//   // analyze the arguments
//   const std::vector<std::string>&
//     optionArgumentsVector =
//       optionsAndArguments.getArgumentsVector ();
//
//   size_t optionArgumentsVectorSize =
//     optionArgumentsVector.size ();
//
//   switch (optionArgumentsVectorSize) {
//     case 0:
//       if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//         gLogStream <<
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
//           optionArgumentsVector [0];
//         if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//           gLogStream <<
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
//       if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//         gLogStream <<
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
  const std::string&           includeFileName,
  std::ostream&                os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    os << "Including options and arguments file \"" <<
      includeFileName <<
      "\"" <<
      std::endl;
  }
#endif

  fIncludeFileNamesStack.push_back (includeFileName);

  // are there too many include levels?
  if (++fIncludeFilesCurrentLevel == pIncludeFilesMaximumLevel) {
    std::stringstream s;

    s <<
      "Including file [" <<
      includeFileName <<
      "]: more than " <<
      pIncludeFilesMaximumLevel <<
      " include levels - quitting";

    std::string message = s.str ();

    os <<
      message <<
      std::endl;

    printIncludeFileNamesStack (os);

    throw mfOahException (message);
  }

  // open options and arguments file
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    os <<
      "Opening options and arguments file \"" <<
      includeFileName <<
      "\" for reading" <<
      std::endl;
  }
#endif

  std::ifstream
    optionsStream (
      includeFileName.c_str (),
      std::ifstream::in);

  if (! optionsStream.is_open ()) {
    std::stringstream s;

    s <<
      "Could not open options and arguments file \"" <<
      includeFileName <<
      "\" for reading - quitting";

    std::string message = s.str ();

    os <<
      message <<
      std::endl;

    throw mfOahException (message);
  }

  // do the include
  includeOptionsFromFile (
    optionsStream,
    fOptionsAndArguments);

#ifdef TRACING_IS_ENABLED
  //  print the options and arguments found
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) { // JMI
    os <<
      "After the inclusion of [" + includeFileName + ']' <<
      ", fOptionsAndArguments contains:" <<
      std::endl <<
      fOptionsAndArguments;
  }
#endif

  fIncludeFileNamesStack.pop_back ();
  --fIncludeFilesCurrentLevel;
}

//______________________________________________________________________________
void oahHandler::includeOptionsFromFile (
  std::istream&                inputStream,
  oahOptionsAndArguments& optionsAndArguments)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> includeOptionsFromFile()" <<
      std::endl;
  }
#endif

  // decipher inputStream line by line
  while (true) {
    std::string currentLine;

    getline (inputStream, currentLine);

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
      gLogStream <<
        "==> currentLine: [" << currentLine << ']' <<
        std::endl;
    }
#endif

    ++gIndenter;

    // remove comment from currentLine if relevant
    size_t commentStart = currentLine.find_first_of ('#');

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "--> currentLine with comment: [" << currentLine << ']' <<
          std::endl;
      }
#endif

    if (commentStart != std::string::npos) {
      currentLine.erase (
        commentStart);

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "--> currentLine without comment: [" << currentLine << ']' <<
          std::endl;
      }
#endif
    }

    if (currentLine.size ()) {
      // trim the currentLine
      mfTrimString (currentLine);

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "--> trimmed currentLine: [" << currentLine << ']' <<
          std::endl;
      }
#endif
    }

    if (currentLine.size ()) {
      // option or argument

      if (currentLine [0] == '-') {
        // option

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
          gLogStream <<
            "--> option found: [" << currentLine << ']' <<
            std::endl;
        }
#endif

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
#ifdef TRACING_IS_ENABLED
          if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
            gLogStream <<
              "Found option [" << optionNamePart << ']' <<
              std::endl;
          }
#endif

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

#ifdef TRACING_IS_ENABLED
          if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
            gLogStream <<
              "Found option [" << optionNamePart << ' ' << valuePart << ']' <<
              std::endl;
          }
#endif
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

#ifdef TRACING_IS_ENABLED
          if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
            gLogStream <<
              "valuePart: [" << valuePart << ']' <<
              std::endl <<
              "valuePartToBeUsed: [" << valuePartToBeUsed << ']' <<
              std::endl;
          }
#endif

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
// #ifdef TRACING_IS_ENABLED
//         if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
//           gLogStream <<
//             "optionName: " << optionName <<
//             std::endl;
//         }
// #endif

        // store the option name and value
        optionsAndArguments.appendOptionNameAndValue (
          optionNamePart,
          valuePartToBeUsed);

        // are there early options in the included file?
        gGlobalOahEarlyOptions.applyEarlyOptionsIfPresentInOptionsAndArguments (
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

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
          gLogStream <<
            "Found argument [" << currentLine << ']' <<
            std::endl;
        }
#endif

        optionsAndArguments.appendArgument (
          currentLine);
      }
    }

    --gIndenter;

     if (inputStream.eof ()) break;
  } // while

  //  print the options and arguments found
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      optionsAndArguments;
  }
#endif
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for version std::string '" << theString <<
      "' with std::regex '" << regularExpression <<
      "'" <<
      std::endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          '[' << sm [i] << "] ";
      } // for

      gLogStream << std::endl;
    }
#endif

  if (smSize == 4) {
    // found an n.x.y specification
    std::string
      generationNumberValue = sm [1],
      majorNumberValue      = sm [2],
      minorNumberValue      = sm [3];

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
        "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
        "--> minorNumberValue = \"" << minorNumberValue << "\"" <<
        std::endl;
    }
#endif

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

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "There are " << smSize << " matches" <<
        " for chord details std::string '" << theString <<
        "' with std::regex '" << regularExpression <<
        "'" <<
        std::endl <<
        smSize << " elements: ";

        for (unsigned i = 0; i < smSize; ++i) {
          gLogStream <<
            '[' << sm [i] << "] ";
        } // for

        gLogStream << std::endl;
      }
#endif

    if (smSize == 3) {
      // found an n.x specification
      // assume implicit 0 minor number
      std::string
        generationNumberValue = sm [1],
        majorNumberValue      = sm [2];

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
        gLogStream <<
          "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
          "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
          std::endl;
      }
#endif

      generationNumber = stoi (generationNumberValue);
      majorNumber      = stoi (majorNumberValue);
      minorNumber      = 0;
    }

    else {
      std::stringstream s;

      s <<
        "version number argument '" << theString <<
        "' is ill-formed";

      oahError (s.str ());
    }
  }
*/


// //   const std::vector<oahOption>& // JMI
// //     optionsVector =
// //       fOptionsAndArguments.getOptionsVector ();
// //
// //   if (optionsVector.size ()) {
// //     s << ' ';
// //
// //     std::vector<oahOption>::const_iterator
// //       iBegin = optionsVector.begin (),
// //       iEnd   = optionsVector.end (),
// //       i      = iBegin;
// //     for ( ; ; ) {
// //       s << (*i);
// //       if (++i == iEnd) break;
// //       s << ' ';
// //     } // for
// //   }
// //
// //   if (fHandlerCommandLineElementsList.size ()) {
// //     s << ' ';
// //
// //     std::list<S_oahElement>::const_iterator
// //       iBegin = fHandlerCommandLineElementsList.begin (),
// //       iEnd   = fHandlerCommandLineElementsList.end (),
// //       i      = iBegin;
// //     for ( ; ; ) {
// //       S_oahElement element = (*i);
// //
// //       s << element-> asShortNamedOptionString ();
// //       if (++i == iEnd) break;
// //       s << ' ';
// //     } // for
// //   }

