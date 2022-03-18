/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...
#include <fstream>

#include <regex>

#include "visitor.h"

#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
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
//       "Converting argc/argv to a string pair vector" <<
//       endl;
//   }
// #endif
//
//   // make sure stringPairVector is empty
// 	stringPairVector.erase (
// 	  stringPairVector.begin (),
// 	  stringPairVector.end ());
//
//   // populate stringPairVector from the elements in argv
//
// 	string currentOption;
//
// 	for (int i = 1; i < argc; ++i) {
// 	  string currString = argv [i];
//
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//       gLogStream <<
//         "--> currentOption : " << currentOption <<
//         endl <<
//         "--> currString 222   : " << currString <<
//         endl << endl;
//     }
// #endif
//
//     if (currentOption.empty ()) {	// wait for option
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
//           make_pair (
//             currentOption, ""));
//
//         currentOption = currString;
//       }
//       else {
//         // option with value
//         stringPairVector.push_back (
//           make_pair (
//             currentOption, currString));
//
//         currentOption = "";
//       }
//     }
//   } // for
//
// 	if (currentOption.size())
//     stringPairVector.push_back (
//       make_pair (
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
  ostream&                  os)
{
  unsigned int stringPairVectorSize =
    stringPairVector.size ();

  os <<
    "The string pair vector contains " <<
    mfSingularOrPlural (
      stringPairVectorSize, "element", "elements");

  if (stringPairVectorSize) {
    os << ":" << endl;

    ++gIndenter;

    const unsigned int fieldWidth = 2;

    int counter = 0;
    for (auto stringPair : stringPairVector) {
      string first  = stringPair.first;
      string second = stringPair.second;

      os <<
        right << setw (fieldWidth) << counter++ <<
        ": [" <<
        first <<
        "] [" <<
        second <<
        ']' <<
        endl;
    } //for

    os << endl;

    --gIndenter;
  }
  else {
    os << endl;
  }
}

// static void argvElements2stringsVector (
//   int             argc,
//   char*           argv[],
//   vector<string>& stringsVector)
// {
// 	for (int i=1; i<argc; ++i) {
// 		stringsVector.push_back (argv [i]);
// 	} // for
// }

// //_______________________________________________________________________________
// EXP Bool arguments2optionsVector (
//   int              argc,
//   char             *argv[],
//   oahOptionsVector& theOptionsVector)
// {
//   // create a strings vector from the elements in argv
// 	vector<string> stringsVector;
//
// 	argvElements2stringsVector (argc, argv, stringsVector);
//
// #ifdef TRACING_IS_ENABLED
//   if (getEarlyTracingOah ()) {
//     gLogStream <<
//       "arguments2optionsVector: stringsVector size: " <<
//       stringsVector.size() <<
//       endl;
//
//     gLogStream << "==> stringsVector:" << endl;
//     for (string str: stringsVector) {
//       gLogStream << "   " << str << endl;
//     } // for
//
//     gLogStream << endl;
//   }
// #endif
//
//   // populate the oahOptionsVector
//   // ------------------------------------------------------
//
// 	string curOption;
//
//   unsigned int stringsVectorSize =
//     stringsVector.size ();
//
//   if (stringsVectorSize) {
//     for (unsigned int i = 0; i < stringsVectorSize; ++i) {
//       string str = stringsVector [i];
//
//   #ifdef TRACING_IS_ENABLED
//       if (getEarlyTracingOah ()) {
//         gLogStream << "--> curOption: " << curOption << endl;
//         gLogStream << "--> str      : " << str << endl;
//       }
//   #endif
//
//       if (curOption.empty ()) {	// wait for option
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
//           theOptionsVector.push_back (make_pair (curOption, ""));
//           curOption = str;
//         }
//         else {
//           // option with value
//           theOptionsVector.push_back (make_pair(curOption, str));
//           curOption = "";
//         }
//       }
//     } // for
//   }
//
// 	if (curOption.size())
// 		theOptionsVector.push_back (make_pair (curOption, ""));
//
// 	return true;
// }

// //______________________________________________________________________________
// void displayOptionsVector (
//   const oahOptionsVector& theOptionsVector,
//   ostream&             os)
// {
//   os <<
//     "The options vector contains " <<
//     mfSingularOrPlural (
//       theOptionsVector.size (), "element", "elements");
//
//   if (theOptionsVector.size ()) {
//     os << ":" << endl;
//
//     ++gIndenter;
//
//     const unsigned int fieldWidth = 2;
//
//     for (unsigned int i = 0; i < theOptionsVector.size (); ++i) {
//       string optionName  = theOptionsVector [i].first;
//       string optionValue = theOptionsVector [i].second;
//
//       os <<
//         right << setw (fieldWidth) << i <<
//         ": \"" <<
//         optionName <<
//         "\" \"" <<
//         optionValue <<
//         "\"" <<
//         endl;
//     } //for
//
//     os << endl;
//
//     --gIndenter;
//   }
//   else {
//     os << endl;
//   }
// }

// //_______________________________________________________________________________
// static void argvElements2stringsVector (
//   int             argc,
//   char*           argv[],
//   vector<string>& stringsVector)
// {
// 	for (int i=1; i<argc; ++i) {
// 		stringsVector.push_back (argv [i]);
// 	} // for
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
//       endl;
//   }
// #endif
//   // create a strings vector from the elements in argv
// 	vector<string> stringsVector;
//
// 	argvElements2stringsVector (
// 	  argc,
// 	  argv,
// 	  stringsVector);
//
//   unsigned int stringsVectorSize =
//     stringsVector.size ();
//
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     gLogStream <<
//       "Converting argc/argv to options and arguments" <<
//       endl <<
//       "==> stringsVector (:" <<
//       stringsVectorSize <<
//       "elements):" <<
//       endl;
//
//     displayStringPairVector (
//       stringsVector,
//       gLogStream);
//
//     gLogStream << endl;
//   }
// #endif
//
//   // populate optionsAndArguments
//   // ------------------------------------------------------
//
// 	string currentOption;
//
//   if (stringsVectorSize) {
//     for (string currString: stringsVector) {
//
// #ifdef TRACING_IS_ENABLED
//       if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//         gLogStream <<
//           "--> currentOption : " << currentOption <<
//           endl <<
//           "--> currString    : " << currString <<
//           endl << endl;
//       }
// #endif
//
//       if (currentOption.empty ()) {	// wait for option
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
// 	if (currentOption.size())
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
string optionOrArgumentKindAsString (
  oahOptionOrArgumentKind optionOrArgumentKind)
{
  string result;

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
  const string&           first,
  const string&           second)
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
  const string&           first,
  const string&           second)
{
  fOptionOrArgumentKind = optionOrArgumentKind;

  fFirst  = first;
  fSecond = second;
}

oahOptionOrArgument::~oahOptionOrArgument ()
{}

void oahOptionOrArgument::print (ostream& os) const
{
  os <<
    "oahOptionOrArgument:" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "fOptionOrArgumentKind" << " : " <<
    optionOrArgumentKindAsString (
      fOptionOrArgumentKind) <<
    endl <<

    setw (fieldWidth) <<
    "fFirst" << " : " << fFirst <<
    endl <<
    setw (fieldWidth) <<
    "fSecond" << " : " << fSecond <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_oahOptionOrArgument& elt)
{
  elt->print (os);
  return os;
}

ostream& operator<< (ostream& os, const oahOptionOrArgument& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
S_oahOption oahOption::create (
  const string& optionName,
  const string& optionValue)
{
  oahOption* o = new
    oahOption (
      optionName,
      optionValue);
  assert (o != nullptr);
  return o;
}

oahOption::oahOption (
  const string& optionName,
  const string& optionValue)
{
  fOptionName  = optionName;
  fOptionValue = optionValue;
}

oahOption::~oahOption ()
{}

string oahOption::asString () const
{
  stringstream s;

  s <<
    "oahOption [" << fOptionName << "]";

  if (fOptionValue.size ()) {
    s << " [";

    if (fOptionValue.find ('\'') != string::npos) {
      s <<
        " \"" + fOptionValue + '"';
    }
    else if (fOptionValue.find ('"') != string::npos) {
      s <<
        " '" + fOptionValue + "\'";
    }
    else if (fOptionValue.find (' ') != string::npos) {
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

string oahOption::asStringForCommandLine () const
{
  return
    oahOptionForCommandLine (
      fOptionName,
      fOptionValue);
}

void oahOption::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_oahOption& elt)
{
  elt->print (os);
  return os;
}

ostream& operator<< (ostream& os, const oahOption& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
string oahOptionForCommandLine (
  const string& optionName,
  const string& optionValue)
{
  stringstream s;

  s <<
    optionName;

  if (optionValue.size ()) {
    if (optionValue.find ('\'') != string::npos) {
      s <<
        " \"" + optionValue + '"';
    }
    else if (optionValue.find ('"') != string::npos) {
      s <<
        " '" + optionValue + "\'";
    }
    else if (optionValue.find (' ') != string::npos) {
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
  vector<S_oahOption>&       vector1,
  const vector<S_oahOption>& vector2)
{
  // insert the elements of vector2 into vector1
  for (S_oahOption option :vector2) {
    vector1.push_back (option);
  } //for
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
// void oahOptionsNamesAndValuesVector::displayOptionsVector (ostream& os) const
// {
//   os <<
//     "The option name and values vector contains " <<
//     mfSingularOrPlural (
//       fOptionsVector.size (), "element", "elements");
//
//   if (fOptionsVector.size ()) {
//     os << ":" << endl;
//
//     ++gIndenter;
//
//     const int fieldWidth = 2;
//
//     int counter = 0;
//     for (S_oahOption option : fOptionsVector) {
//       string optionName  =
//         option->getOptionName ();
//       string optionValue =
//         option->getOptionValue ();
//
//       os <<
//         right << setw (fieldWidth) << counter++ <<
//         ": [" <<
//         optionName <<
//         "] [" <<
//         optionValue <<
//         ']' <<
//         endl;
//     } //for
//
//     os << endl;
//
//     --gIndenter;
//   }
//   else {
//     os << endl;
//   }
// }
//
// string oahOptionsNamesAndValuesVector::asCommandLineOptionsString () const
// {
//   stringstream s;
//
//   for (S_oahOption option : fOptionsVector) {
//     string
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
//       if (optionValue.find ('\'') != string::npos) {
//         s <<
//           " \"" + optionValue + '"';
//       }
//       else if (optionValue.find ('"') != string::npos) {
//         s <<
//           " '" + optionValue + "\'";
//       }
//       else if (optionValue.find (' ') != string::npos) {
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
// void oahOptionsNamesAndValuesVector::print (ostream& os) const
// {
//   for (S_oahOption option : fOptionsVector) {
//     string optionName  =
//       option->getOptionName ();
//     string optionValue =
//       option->getOptionValue ();
//
//     os <<
//       optionName;
//
//     if (optionValue.size ()) {
//       os << ' ';
//
//       if (optionValue.find ('\'') != string::npos) {
//         os <<
//           " \"" + optionValue + '"';
//       }
//       else if (optionValue.find ('"') != string::npos) {
//         os <<
//           " '" + optionValue + "\'";
//       }
//       else if (optionValue.find (' ') != string::npos) {
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
// ostream& operator<< (ostream& os, const S_oahOptionsNamesAndValuesVector& elt)
// {
//   elt->print (os);
//   return os;
// }
//
// ostream& operator<< (ostream& os, const oahOptionsNamesAndValuesVector& elt)
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
  const string& optionName,
  const string& optionValue)
{
  fOptionsVector.push_back (
    oahOption (
      optionName,
      optionValue));
}

void oahOptionsAndArguments::displayOptionsVector (ostream& os) const
{
  os <<
    "The option name and values vector contains " <<
    mfSingularOrPlural (
      fOptionsVector.size (), "element", "elements");

  if (fOptionsVector.size ()) {
    os << ":" << endl;

    ++gIndenter;

    const int fieldWidth = 2;

    int counter = 0;
    for (oahOption option : fOptionsVector) {
      string optionName  =
        option.getOptionName ();
      string optionValue =
        option.getOptionValue ();

      os <<
        right << setw (fieldWidth) << counter++ <<
        ": [" <<
        optionName <<
        "] [" <<
        optionValue <<
        ']' <<
        endl;
    } //for

    os << endl;

    --gIndenter;
  }
  else {
    os << endl;
  }
}

void oahOptionsAndArguments::displayArgumentsVector (ostream& os) const
{
  mfDisplayStringsVector (
    fArgumentsVector,
    "fArgumentsVector",
    os);
}

void oahOptionsAndArguments::print (ostream& os) const
{
//   os << JMI
//     "oahOptionsAndArguments:" <<
//     endl;
//
//   ++gIndenter;
//
  displayOptionsVector (os);
  displayArgumentsVector (os);

//   --gIndenter;
}

ostream& operator<< (ostream& os, const S_oahOptionsAndArguments& elt)
{
  elt->print (os);
  return os;
}

ostream& operator<< (ostream& os, const oahOptionsAndArguments& elt)
{
  elt.print (os);
  return os;
}

// early option?
//______________________________________________________________________________
string oahEarlyOptionKindAsString (
  oahEarlyOptionKind earlyOptionKind)
{
  string result;

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
string oahHandlerUsedThruKindAsString (
  oahHandlerUsedThruKind handlerUsedThruKind)
{
  string result;

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

ostream& operator<< (ostream& os, const oahHandlerUsedThruKind& elt)
{
  os << oahHandlerUsedThruKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_oahPrefix oahPrefix::create (
  const string& prefixName,
  const string& prefixErsatz,
  const string& prefixDescription)
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
  const string& prefixName,
  const string& prefixErsatz,
  const string& prefixDescription)
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

string oahPrefix::fetchPrefixNames () const
{
  stringstream s;

  s <<
    "\"" <<
    fPrefixName <<
    "\", \"" <<
   fPrefixErsatz <<
   "\"";

  return s.str ();
}

void oahPrefix::findStringInPrefix (
  const string& lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  // does this element's short name match?
  Bool prefixNameMatches =
    mfStringToLowerCase (fPrefixName).find (lowerCaseString) != string::npos;

  // does this element's long name match?
  Bool prefixErsatzMatches =
    mfStringToLowerCase (fPrefixErsatz).find (lowerCaseString) != string::npos;

  // does this element's description match?
  Bool prefixDescriptionMatches =
    mfStringToLowerCase (fPrefixDescription).find (lowerCaseString) != string::npos;

  if (prefixNameMatches || prefixErsatzMatches || prefixDescriptionMatches) {
    stringstream s;

    s <<
      fetchPrefixNames () <<
        endl;

    // indent a bit more for readability
 // JMI    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    s <<
 //     gIndenter.indentMultiLineString ( // JMI
//        fDescription) <<
      fPrefixDescription;

 // JMI    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);

    // append the string
    foundStringsList.push_back (s.str ());
  }
}

void oahPrefix::printPrefixEssentials (
  ostream& os,
  int fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    "prefixName" << " : " <<
    fPrefixName <<
    endl <<
    setw (fieldWidth) <<
    "prefixErsatz" << " : " <<
    fPrefixErsatz <<
    endl <<
    setw (fieldWidth) <<
    "prefixDescription" << " : " <<
    fPrefixDescription <<
    endl;
}

void oahPrefix::print (ostream& os) const
{
  os <<
    "??? oahPrefix ???" <<
    endl;

  printPrefixEssentials (os, 40); // JMI
}

void oahPrefix::printShort (ostream& os) const
{
  os <<
    "??? oahPrefix ???" ;

  printPrefixEssentials (os, 40); // JMI
}

void oahPrefix::printHelp (ostream& os) const
{
  if (fPrefixDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fPrefixDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  os << endl;
}

ostream& operator<< (ostream& os, const S_oahPrefix& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtom oahAtom::create (
  const string&       longName,
  const string&       shortName,
  const string&       description,
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
  const string&       longName,
  const string&       shortName,
  const string&       description,
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

void oahAtom::setSubGroupUpLink (
  S_oahSubGroup subGroup)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    subGroup != nullptr,
    "subGroup is null");

  // set the atom subgroup upLink
  fSubGroupUpLink = subGroup;
}

S_oahGroup oahAtom::fetchAtomGroupUpLink () const
{
  S_oahGroup result;

  if (fSubGroupUpLink) {
    result =
      fSubGroupUpLink->
        getGroupUpLink ();
  }

  return result;
}

S_oahHandler oahAtom::fetchAtomHandlerUpLink () const
{
  S_oahHandler result;

  if (fSubGroupUpLink) {
    result =
      fSubGroupUpLink->
        getGroupUpLink ()->
          getHandlerUpLink ();
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
        endl;
  }
#endif

  ++gIndenter;

  // an atom with an optional value
  // should not have the same name as a prefix,
  // since this would create an ambiguity
  switch (fElementValueKind) {
    case oahElementValueKind::kElementValueWithout:
    case oahElementValueKind::kElementValueImplicit:
    case oahElementValueKind::kElementValueMandatory:
      break;

    case oahElementValueKind::kElementValueOptional:
      if (handler->fetchNameInPrefixesMap (fLongName)) {
        stringstream s;

        s <<
          "the long name of atom with optional value " <<
          fetchNamesBetweenQuotes () <<
          " is already known as a prefix";

        oahError (s.str ());
      }
      if (handler->fetchNameInPrefixesMap (fShortName)) {
        stringstream s;

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
  fSubGroupUpLink->
    incrementNumberOfUserChoseAtomsInThisSubGroup ();

  S_oahGroup
    atomGroup =
      fSubGroupUpLink-> getGroupUpLink ();

  atomGroup->
    incrementNumberOfUserChoseAtomsInThisGroup ();
}

void oahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtom::acceptIn ()" <<
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahAtom::print (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "Atom ???:" <<
      endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) << "elementHelpOnlyKind" << " : " <<
    elementHelpOnlyKindAsString (fElementHelpOnlyKind) <<
    endl;

  --gIndenter;
}

void oahAtom::printShort (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "Atom: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);
}

void oahAtom::printSummary (ostream& os) const
{
  os <<
    "Atom: " <<
    fetchNames () <<
    endl;
}

void oahAtom::findStringInAtom (
  const string& lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  findStringInElement (
    lowerCaseString,
    foundStringsList,
    os);
}

void oahAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "Atom values ???:" <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtomExpectingAValue oahAtomExpectingAValue::create (
  const string& longName,
  const string& shortName,
  const string& description)
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
  const string& longName,
  const string& shortName,
  const string& description)
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahAtomExpectingAValue::applyElement (ostream& os)
{
  stringstream s;

  s <<
    "Applying atom expecting a value '" <<
    fetchNames () <<
    "' without a value";

  oahInternalError (s.str ());
}

void oahAtomExpectingAValue::applyAtomWithDefaultValue (ostream& os)
{
  stringstream s;

  s <<
    "Applying atom expecting a value '" <<
    fetchNames () <<
    "' with a default value is possible only in oahAtomExpectingAValue subclasses";

  oahInternalError (s.str ());
}

void oahAtomExpectingAValue::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahAtomExpectingAValue:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomExpectingAValue::printShort (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahAtomExpectingAValue: ";

  printOahElementEssentialsShort (
    os, fieldWidth);

  os <<
    fDescription <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahAtomExpectingAValue& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtomImplicitlyStoringAValue oahAtomImplicitlyStoringAValue::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& variableName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& variableName)
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

void oahAtomImplicitlyStoringAValue::applyElement (ostream& os)
{
  stringstream s;

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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahAtomImplicitlyStoringAValue::printAtomWithVariableEssentials (
  ostream& os,
  int fieldWidth) const
{
  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl;
  if (fSetByUser) {
    os << left <<
      setw (fieldWidth) <<
      "set by user" <<
      endl;
  }
}

void oahAtomImplicitlyStoringAValue::printAtomWithVariableEssentialsShort (
  ostream& os,
  int fieldWidth) const
{
  printOahElementEssentialsShort (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl;
  if (fSetByUser) {
    os << left <<
      setw (fieldWidth) <<
      "set by user" <<
      endl;
  }
}

void oahAtomImplicitlyStoringAValue::print (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "AtomWithVariable:";
  if (fSetByUser) {
    os <<
      ", set by user";
  }
  os << endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomImplicitlyStoringAValue::printShort (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "AtomWithVariable: ";
  if (fSetByUser) {
    os <<
      ", set by user";
  }

  printAtomWithVariableEssentialsShort (
    os, fieldWidth);
}

void oahAtomImplicitlyStoringAValue::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahAtomImplicitlyStoringAValue::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "AtomWithVariable values:" <<
    "???, fSetByUser: " <<
    fSetByUser <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahAtomImplicitlyStoringAValue& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahAtomStoringAValue oahAtomStoringAValue::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& variableName)
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahAtomStoringAValue::printAtomWithVariableEssentials (
  ostream& os,
  int fieldWidth) const
{
  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fValueSpecification" << " : " <<
    fValueSpecification <<
    endl <<

    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fSetByUser" << " : " <<
    fSetByUser <<
    endl;
}

void oahAtomStoringAValue::printAtomWithVariableEssentialsShort (
  ostream& os,
  int fieldWidth) const
{
  printOahElementEssentialsShort (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fValueSpecification" << " : " <<
    fValueSpecification <<
    endl <<

    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "fSetByUser" << " : " <<
    fSetByUser <<
    endl;
}

void oahAtomStoringAValue::print (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "AtomWithVariable:";
  if (fSetByUser) {
    os <<
      ", set by user";
  }
  os << endl;

  ++gIndenter;

  printAtomWithVariableEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomStoringAValue::printShort (ostream& os) const
{
  const int fieldWidth = 19;

  os <<
    "AtomWithVariable: ";
  if (fSetByUser) {
    os <<
      ", set by user";
  }

  printAtomWithVariableEssentialsShort (
    os, fieldWidth);
}

void oahAtomStoringAValue::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    ' ' << fValueSpecification <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahAtomStoringAValue::findStringInAtom (
  const string& lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  // does this element's long name match?
  Bool longNameMatches =
    mfStringToLowerCase (fLongName).find (lowerCaseString) != string::npos;

  // does this element's short name match?
  Bool shortNameMatches =
    mfStringToLowerCase (fShortName).find (lowerCaseString) != string::npos;

  // does this element's description match?
  Bool descriptionMatches =
    mfStringToLowerCase (fDescription).find (lowerCaseString) != string::npos;

  if (shortNameMatches || longNameMatches || descriptionMatches) {
    stringstream s;

    // add the element's names and value specification
    s <<
      fetchNames () <<
      ' ' <<
      fValueSpecification <<
      endl;

    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    // add the element's description
    s <<
      gIndenter.indentMultiLineString (
        fDescription);

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);

    // append the string
    foundStringsList.push_back (s.str ());
  }
}

void oahAtomStoringAValue::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "AtomWithVariable values:" <<
    "???, fSetByUser: " <<
    fSetByUser <<
    endl;
}

ostream& operator<< (ostream& os, const S_oahAtomStoringAValue& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahPureHelpAtomWithoutAValue oahPureHelpAtomWithoutAValue::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& serviceName)
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahPureHelpAtomWithoutAValue::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "HelpOnlyAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahPureHelpAtomWithoutAValue::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahPureHelpAtomWithoutAValue& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahPureHelpAtomExpectingAValue oahPureHelpAtomExpectingAValue::create (
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
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
  const string& longName,
  const string& shortName,
  const string& description,
  const string& valueSpecification,
  const string& serviceName)
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif
}

void oahPureHelpAtomExpectingAValue::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "HelpOnlyAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fValueSpecification" << " : " <<
    fValueSpecification <<
    endl <<
    setw (fieldWidth) <<
    "fHelpAtomExpectingAValueServiceName" << " : " <<
    fHelpAtomExpectingAValueServiceName <<
    endl;

  --gIndenter;
}

void oahPureHelpAtomExpectingAValue::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    ' ' << fValueSpecification <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

ostream& operator<< (ostream& os, const S_oahPureHelpAtomExpectingAValue& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahSubGroup oahSubGroup::create (
  const string&            subGroupHeader,
  const string&            longName,
  const string&            shortName,
  const string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup               groupUpLink)
{
  oahSubGroup* o = new
    oahSubGroup (
      subGroupHeader,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      groupUpLink);
  assert (o != nullptr);
  return o;
}

S_oahSubGroup oahSubGroup::createWithNames (
  const string&            subGroupHeader,
  const string&            longName,
  const string&            shortName,
  const string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup               groupUpLink)
{
  oahSubGroup* o = new
    oahSubGroup (
      subGroupHeader,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      groupUpLink);
  assert (o != nullptr);
  return o;
}

oahSubGroup::oahSubGroup (
  const string&            subGroupHeader,
  const string&            longName,
  const string&            shortName,
  const string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahGroup               groupUpLink)
  : oahElement (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout,
      optionVisibilityKind)
{
  fGroupUpLink = groupUpLink;

  fSubGroupHeader = subGroupHeader;

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyYes;

  fNumberOfUserChoseAtomsInThisSubGroup = 0;
}

oahSubGroup::~oahSubGroup ()
{}

S_oahHandler oahSubGroup::fetchSubGroupHandlerUpLink () const
{
  S_oahHandler result;

  if (fGroupUpLink) {
    result =
      fGroupUpLink->
        getHandlerUpLink ();
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
      "\" in the elements list in handler \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // append subgroup to elements list
  handler->
    appendElementToElementsList (this);

  fGroupUpLink->
    setHandlerUpLink (handler);

  // register the OAH atoms in the handler
  for (
    list<S_oahAtom>::const_iterator
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
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Registering the names in subgroup \"" <<
      fSubGroupHeader <<
      "\" in the names to elements map in handler \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // register this subgroup's names in this handler
  handler->
    registerElementNamesInHandler (
      this);

  // register this subgroup's names in handler
  for (
    list<S_oahAtom>::const_iterator
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

  // append atom to atoms list
  fSubGroupAtomsList.push_back (
    atom);

  // set atom subgroup upLink
  atom->
    setSubGroupUpLink (this);
}

S_oahElement oahSubGroup::fetchOptionByNameInSubGroup (
  const string& name)
{
  S_oahElement result;

  for (
    list<S_oahAtom>::const_iterator
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

void oahSubGroup::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Applying element " <<
      fetchNames () <<
      ", which is a oahSubGroup" <<
      endl;
  }
#endif

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  // print the help
  gLogStream <<
    "--- Help for subgroup \"" <<
    fSubGroupHeader <<
    "\" in group \"" <<
    fGroupUpLink->getGroupHeader () <<
    "\" ---" <<
    endl;

  ++gIndenter;

  fGroupUpLink->
    printGroupAndSubGroupHelp (
      os,
      this);

  --gIndenter;

  gIndenter.setIndent (saveIndent);
}

void oahSubGroup::checkSubGroupOptionsConsistency ()
{}

void oahSubGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahSubGroup::acceptIn ()" <<
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif

  // browse the atoms
  if (fSubGroupAtomsList.size ()) {
    for (
      list<S_oahAtom>::const_iterator i = fSubGroupAtomsList.begin ();
      i != fSubGroupAtomsList.end ();
      ++i
    ) {
      S_oahAtom atom = (*i);

      // browse the atom
      /* JMI ???
      gLogStream <<
        ".\\\" ==> Going to browse atom" <<
        " '" << atom->fetchNames () << "'" <<
        endl;
        */

      oahBrowser<oahAtom> browser (v);
      browser.browse (*(atom));
    } // for
  }
}

void oahSubGroup::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
   "SubGroup:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "elementVisibilityKind" << " : " <<
      elementVisibilityKindAsString (
        fElementVisibilityKind) <<
    endl << endl;

  os <<
    "AtomsList (" <<
    mfSingularOrPlural (
      fSubGroupAtomsList.size (), "atom",  "atoms") <<
    "):" <<
    endl;

  if (fSubGroupAtomsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahSubGroup::printShort (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
   "SubGroup: " ;

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahSubGroup::printSummary (ostream& os) const
{
  os <<
    "SubGroup: " <<
    fetchNames () <<
    endl;

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahAtom atom = (*i);

      // print a summary of the atom
      atom->printSummary (os);
      if (++i == iEnd) break;
//      os << endl;
    } // for

    --gIndenter;
  }
}

void oahSubGroup::underlineSubGroupHeader (ostream& os) const
{
  /* JMI ???
  for (unsigned int i = 0; i < fSubGroupHeader.size (); ++i) {
    os << "-";
  } // for
  os << endl;
  */
  os << "--------------------------" << endl;
}

void oahSubGroup::printSubGroupHeader (ostream& os) const
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

  os << endl;
}

void oahSubGroup::printSubGroupHeaderWithHeaderWidth (
  ostream& os,
  int      subGroupHeaderWidth) const
{
  // print the header and option names
  os << left <<
    setw (subGroupHeaderWidth) <<
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

  os << endl;

}

void oahSubGroup::printHelp (ostream& os) const
{
  // print the header and option names
  printSubGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription);
    --gIndenter;

    os << endl;
  }

  // print the options atoms if relevant
  switch (fElementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityNone:
      break;

    case oahElementVisibilityKind::kElementVisibilityWhole:
      if (fSubGroupAtomsList.size ()) {
        ++gIndenter;

        list<S_oahAtom>::const_iterator
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
  ostream& os,
  int      subGroupHeaderWidth) const
{
  // print the header and option names
  printSubGroupHeaderWithHeaderWidth (
    os,
    subGroupHeaderWidth);

  // underline the header if the group header is not written
  if (! fGroupUpLink->getGroupHeaderIsToBeWritten ()) {
    underlineSubGroupHeader (os);
  }

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription);
    --gIndenter;

    os << endl;
  }

  // print the options atoms if relevant
  switch (fElementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityWhole:
      if (fSubGroupAtomsList.size ()) {
        ++gIndenter;

        list<S_oahAtom>::const_iterator
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

void oahSubGroup::printSubGroupHelp (ostream& os) const
{
  // print the header and option names
  printSubGroupHeader (os);

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    --gIndenter;
  }

  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
      iBegin = fSubGroupAtomsList.begin (),
      iEnd   = fSubGroupAtomsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the atom help
      (*i)->printHelp (os);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    --gIndenter;
  }
}

void oahSubGroup::printOptionsSummary (
  ostream& os) const
{
  // fetch maximum subgroups help headers size
  unsigned int maximumSubGroupsHelpHeadersSize =
    getGroupUpLink ()->
      getHandlerUpLink ()->
        getMaximumSubGroupsHeadersSize ();

    /* JMI
  // fetch maximum short name width
  int maximumShortNameWidth =
    getGroupUpLink ()->
      getHandlerUpLink ()->
        getMaximumShortNameWidth ();
*/

  // print the summary
  os << left <<
    setw (maximumSubGroupsHelpHeadersSize) <<
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

  os << endl;

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    --gIndenter;
  }
}

void oahSubGroup::printSubGroupSpecificHelpOrOptionsSummary (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    os <<
      "oahSubGroup::printSubGroupSpecificHelpOrOptionsSummary" <<
      endl;
  }
#endif

  // print only the summary if this is not the desired subgroup,
  // otherwise print the regular help
  if (subGroup == this) {
    os << endl;
    printSubGroupSpecificHelpOrOptionsSummary (
      os,
      subGroup);
  }
  else {
    printOptionsSummary (os); // JMI ???
  }
 }

void oahSubGroup::printSubGroupAndAtomHelp (
  ostream&  os,
  S_oahAtom targetAtom) const
{
  // print the subgroup atoms
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
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

void oahSubGroup::findStringInSubGroup (
  const string& lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  findStringInElement (
    lowerCaseString,
    foundStringsList,
    os);

  // do this subgroups's atoms match?
  if (fSubGroupAtomsList.size ()) {
   ++gIndenter;

   for (S_oahAtom atom : fSubGroupAtomsList) {
      atom->
        findStringInAtom (
          lowerCaseString,
          foundStringsList,
          os);
    } // for

    --gIndenter;
  }
}

void oahSubGroup::printSubGroupOptionsValues (
  ostream& os,
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
    endl;

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
        // print the atom value if the variable has been set by user by user
        if (booleanAtom->getSetByUser ()) {
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
        // print the atom value if the variable has been set by user by user
        if (atomImplicitlyStoringAValue->getSetByUser ()) {
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
        // print the atom value if the variable has been set by user by user
        if (atomStoringAValue->getSetByUser ()) {
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
  ostream& os,
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
    endl;

  // underline the options subgroup header
// JMI  underlineSubGroupHeader (os);

  // print the subgroup atoms values
  if (fSubGroupAtomsList.size ()) {
    ++gIndenter;

    list<S_oahAtom>::const_iterator
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
  //    os << endl;
    } // for

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_oahSubGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahGroup oahGroup::create (
  const string&            header,
  const string&            longName,
  const string&            shortName,
  const string&            description,
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
  const string&            header,
  const string&            longName,
  const string&            shortName,
  const string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler             groupHandlerUpLink)
{
  oahGroup* o = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      groupHandlerUpLink);
  assert (o != nullptr);
  return o;
}

S_oahGroup oahGroup::createWithNames (
  const string&            header,
  const string&            longName,
  const string&            shortName,
  const string&            description,
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
  const string&            header,
  const string&            longName,
  const string&            shortName,
  const string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler             groupHandlerUpLink)
{
  oahGroup* o = new
    oahGroup (
      header,
      longName,
      shortName,
      description,
      optionVisibilityKind,
      groupHandlerUpLink);
  assert (o != nullptr);
  return o;
}

oahGroup::oahGroup (
  const string&            header,
  const string&            longName,
  const string&            shortName,
  const string&            description,
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
  const string&            header,
  const string&            longName,
  const string&            shortName,
  const string&            description,
  oahElementVisibilityKind optionVisibilityKind,
  S_oahHandler             groupHandlerUpLink)
  : oahElement (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout,
      optionVisibilityKind)
{
  fHandlerUpLink = groupHandlerUpLink;

  fGroupHeader = header;

  fGroupHeaderIsToBeWritten = true; // default value

  fNumberOfUserChoseAtomsInThisGroup = 0;
}

oahGroup::~oahGroup ()
{}

void oahGroup::setHandlerUpLink (
  S_oahHandler handlerUpLink)
{
  fHandlerUpLink = handlerUpLink;
}

void oahGroup::appendGroupToElementsList (
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Appending group \"" <<
      fGroupHeader <<
      "\" to elements list in handler \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    handler != nullptr,
    "handler is null");

  // append this group to the elements list
  handler->
    appendElementToElementsList (this);

  // set group handler upLink
  fHandlerUpLink = handler;

  // append subgroups to elements list
  for (
    list<S_oahSubGroup>::const_iterator
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
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Registering the names in group \"" <<
      fGroupHeader <<
      "\" in the names to elements map in handler \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      endl;
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

  // append subgroups to elements list
  for (
    list<S_oahSubGroup>::const_iterator
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
    setGroupUpLink (this);
}

S_oahElement oahGroup::fetchOptionByNameInGroup (
  const string& name)
{
  S_oahElement result;

  for (
    list<S_oahSubGroup>::const_iterator
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

void oahGroup::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Applying element '" <<
      fetchNames () <<
      "', which is a oahGroup" <<
      endl;
  }
#endif

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  // print the help
  gLogStream <<
    "--- Help for group \"" <<
    fGroupHeader <<
    "\" ---" <<
    endl;

  ++gIndenter;

  printHelp (os);

  --gIndenter;

  gIndenter.setIndent (saveIndent);
}

void oahGroup::handleAtomValue (
  ostream&      os,
  S_oahAtom     atom,
  const string& theString)
{
  os <<
    "Handling option name atom '" <<
    atom <<
    "' with value \"" <<
    theString <<
    "\" is not handled" <<
    endl;
}

S_mfcMultiComponent oahGroup::fetchGroupHandlerMultiComponent () const
{
  return
    fHandlerUpLink->
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
      endl;
  }
#endif

  ++gIndenter;

  // check the subgroups options consistency
  if (fGroupSubGroupsList.size ()) {
    for (
      list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif

  // browse the subGroups
  if (fGroupSubGroupsList.size ()) {
    for (
      list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
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

void oahGroup::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "Group:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    "SubGroupsList (" <<
    mfSingularOrPlural (
      fGroupSubGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fGroupSubGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahGroup::printShort (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "Group: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  if (fGroupSubGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::printSummary (ostream& os) const
{
  os <<
    "Group: " <<
    fetchNames () <<
    endl;

  if (fGroupSubGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahSubGroup subGroup = (*i);

      // print a summary of the options subgroup
      subGroup->printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::underlineGroupHeader (ostream& os) const
{
  /* JMI
  for (unsigned int i = 0; i < fGroupHeader.size (); ++i) {
    os << "-";
  } // for
  os << endl;
  */
  os << "--------------------------" << endl;
}

void oahGroup::printGroupHeader (ostream& os) const
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

  os << endl;
}

void oahGroup::printHelp (ostream& os) const
{
  if (fGroupHeaderIsToBeWritten) {
    // print the header and option names
    printGroupHeader (os);

    // print the description if any
    if (fDescription.size ()) {
      ++gIndenter;
      os <<
        gIndenter.indentMultiLineString (
          fDescription) <<
        endl;
      --gIndenter;
    }

    // underline the options group header
    underlineGroupHeader (os);
  }

  // print the options subgroups
  if (fGroupSubGroupsList.size ()) {
    // compute the maximun sub group header length
    unsigned int maximumSubGroupHeaderLength = 0;

    for (
      list<S_oahSubGroup>::const_iterator i = fGroupSubGroupsList.begin ();
      i != fGroupSubGroupsList.end ();
      ++i
    ) {
      string subGroupHeader  = (*i)->getSubGroupHeader ();
      unsigned int subGroupHeaderSize = subGroupHeader.size ();

      if (subGroupHeaderSize > maximumSubGroupHeaderLength) {
        maximumSubGroupHeaderLength = subGroupHeaderSize;
      }
    } // for

    maximumSubGroupHeaderLength += 3; // to have some more spaces

    if (fGroupHeaderIsToBeWritten) {
      ++gIndenter;
    }

    list<S_oahSubGroup>::const_iterator
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
  // JMI    os << endl;
    } // for

    if (fGroupHeaderIsToBeWritten) {
      --gIndenter;
    }
  }
}

void oahGroup::printGroupAndSubGroupHelp (
  ostream&      os,
  S_oahSubGroup targetSubGroup) const
{
  // print the header and option names
  os <<
    fGroupHeader <<
    ' ' <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    --gIndenter;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the target options subgroup
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
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
  ostream&      os,
  S_oahSubGroup targetSubGroup,
  S_oahAtom     targetAtom) const
{
  // print the target options subgroup
  if (fGroupSubGroupsList.size ()) {
    list<S_oahSubGroup>::const_iterator
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

void oahGroup::printOptionsSummary (ostream& os) const
{
  // the description is the header of the information
  os <<
    fGroupHeader <<
    ' ' <<
    fetchNamesBetweenParentheses () <<
    ":" <<
    endl;

  // print the description if any
  if (fDescription.size ()) {
    ++gIndenter;
    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
    --gIndenter;
  }

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
      iBegin = fGroupSubGroupsList.begin (),
      iEnd   = fGroupSubGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options subgroup description
      (*i)->
        printOptionsSummary (os);
      if (++i == iEnd) break;
 //     os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupAndSubGroupSpecificHelp (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    os <<
      "oahGroup::printGroupAndSubGroupSpecificHelp" <<
      endl;
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
      endl;

    // print the description if any
    if (fDescription.size ()) {
      ++gIndenter;
      os <<
        gIndenter.indentMultiLineString (
          fDescription) <<
        endl;
      --gIndenter;
    }

    // underline the options group header
    underlineGroupHeader (os);
  }

  // print the options subgroups
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
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
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupOptionsValues (
  ostream& os,
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
    endl;

    // underline the options group header
    underlineGroupHeader (os);
  }

  // print the options subgroups values
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    Bool doPrintOptionsValues;

    list<S_oahSubGroup>::const_iterator
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
        os << endl;
      }
    } // for

    --gIndenter;
  }
}

void oahGroup::printGroupOptionsValuesAll (
  ostream& os,
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
    endl;

  // underline the options group header
  underlineGroupHeader (os);

  // print the options subgroups values
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    list<S_oahSubGroup>::const_iterator
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
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahGroup::findStringInGroup (
  const string& lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  switch (fElementVisibilityKind) { // JMI remove???
    case oahElementVisibilityKind::kElementVisibilityNone:
    case oahElementVisibilityKind::kElementVisibilityWhole:
    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      // don't use this groups's data to find the string
      return;
      break;
  } // switch

  findStringInElement (
    lowerCaseString,
    foundStringsList,
    os);

  // do this groups's subgroups match?
  if (fGroupSubGroupsList.size ()) {
    ++gIndenter;

    for (S_oahSubGroup subGroup : fGroupSubGroupsList) {
      subGroup->
        findStringInSubGroup (
          lowerCaseString,
          foundStringsList,
          os);
    } // for

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_oahGroup& elt)
{
  elt->print (os);
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

/* this class is purely virtual
S_oahHandler oahHandler::create (
  const string& serviceName,
  const string& handlerHeader,
  const string&           handlerDescription,
  const string&           handlerUsage)
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
  const string& serviceName,
  const string& handlerHeader,
  const string&           handlerDescription,
  const string&           handlerUsage)
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
      endl;
  }
#endif

  // DON'T create the fHandlerMultiComponent,
  // this will be done in the subclasses' initializeHandlerMultiComponent ()

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
  const string& name) const
{
  S_oahPrefix result;

  // is name known in prefixes map?
  map<string, S_oahPrefix>::const_iterator
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
  const string& name) const
{
  S_oahElement result;

  // is name known in the names to elements map?
  map<string, S_oahElement>::const_iterator
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
        endl;
  }
#endif

  ++gIndenter;

  string
    elementShortName =
      element->getShortName (),
    elementLongName =
      element->getLongName ();

  unsigned int
    elementShortNameSize =
      elementShortName.size (),
    elementLongNameSize =
      elementLongName.size ();

  // sanity checks
  if (elementLongNameSize == 0) {
    stringstream s;

    s <<
      "element long name is empty";

    oahError (s.str ());
  }

  if (elementShortNameSize == 0 && elementLongNameSize == 0) {
    stringstream s;

    s <<
      "element long name and short name are both empty";

    oahError (s.str ());
  }

  if (elementShortName == elementLongName) {
    stringstream s;

    s <<
      "element short name \"" << elementLongName << "\"" <<
      " is the same as the long name for the same";

    oahError (s.str ());
  }

  if (elementLongNameSize == 1) {
    stringstream s;

    s <<
      "element long name \"" << elementLongName << "\"" <<
      " has only one character";

    oahWarning (s.str ());
  }

  if (elementShortNameSize > elementLongNameSize) {
    stringstream s;

    s <<
      "element short name \"" << elementShortName << "\"" <<
      " is longer than long name \"" << elementLongName << "\"" <<
      " for the same";

    oahError (s.str ());
  }

  for (
    map<string, S_oahElement>::iterator i =
      fHandlerNamesToElementsMap.begin ();
    i != fHandlerNamesToElementsMap.end ();
    ++i
  ) {
    string       name    = (*i).first;
    S_oahElement element = (*i).second;

    // is elementLongName already in the elements names map?
    if (name == elementLongName) {
      stringstream s;

      s <<
        "element long name \"" << elementLongName << "\"" <<
          " for element short name \"" << elementShortName << "\"" <<
          " is already defined for " <<
           element->fetchNamesBetweenQuotes () <<
          " in handler \"" <<
          fHandlerHeader <<
          "\"";

      oahInternalError (s.str ());
    }

    // is elementShortName already in the elements names map?
    if (name == elementShortName) {
      if (elementShortName.size ()) {
        stringstream s;

        s <<
          "element short name \"" << elementShortName << "\"" <<
          " for element long name \"" << elementLongName << "\"" <<
          " is already defined for " <<
           element->fetchNamesBetweenQuotes () <<
          " in handler \"" <<
          fHandlerHeader <<
          "\"";

        oahInternalError (s.str ());
      }
    }
  } // for

  if (elementShortNameSize == 1) {
    // register element's short name in single character short names set
    fSingleCharacterShortNamesSet.insert (
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
  unsigned int
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
      "\" in its names to elements map" <<
      endl;
  }
#endif

  ++gIndenter;

  // check the handler groups options consistency
  if (fHandlerGroupsList.size ()) {
    for (
      list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
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
        "\" have been registered in the names to elements map" <<
        endl;
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
        endl;
  }
#endif

  // insert element into the registered elements list
  fHandlerElementsList.push_back (element);

  // compute subgroup's header size
  if (
    // subgroup?
    S_oahSubGroup
      subGroup =
        dynamic_cast<oahSubGroup*>(&(*element))
    ) {

    string
      subHeader=
        subGroup-> getSubGroupHeader ();
    unsigned int
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
  unsigned int argumentsNumber =
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
        " in handler arguments vector for " <<
        fHandlerServiceName <<
        ":" <<
        endl;

      ++gIndenter;

//       gLogStream <<
//         fOptionsAndArguments;

      gLogStream << endl;

      --gIndenter;
    }
#endif

    oahInternalWarning (
      "the arguments vector should be empty with options and arguments???");
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        endl;
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
      endl;
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
      "Checking no input source in argument vector in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  const vector<string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  unsigned int argumentsNumber =
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
        " in handler arguments vector for " <<
        fHandlerServiceName <<
        ":" <<
        endl;

      ++gIndenter;

      for (unsigned int i = 0; i < argumentsNumber; ++i) {
       gLogStream <<
          i << " : FII " << argumentsVector [i] << // JMIJMIJMI
          endl;
      } // for

      gLogStream << endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        endl;
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
        stringstream s;

        s <<
          fHandlerServiceName <<
          " doesn't expect arguments, only options can be used";

        string message = s.str ();

        gLogStream <<
          message <<
          endl;

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
      "Checking no or one input source in argument vector in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  const vector<string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  unsigned int argumentsNumber =
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
        " in handler arguments vector for " <<
        fHandlerServiceName <<
        ":" <<
        endl;

      ++gIndenter;

      for (unsigned int i = 0; i < argumentsNumber; ++i) {
        gLogStream <<
          i << " : FUU " << argumentsVector [i] <<
            endl;
      } // for

      gLogStream << endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        endl;
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
        stringstream s;

        s <<
          "Several input file names supplied to " <<
          fHandlerServiceName <<
          ", only the first one, \"" <<
          argumentsVector [0] <<
          "\", will be converted "; //JMI

        string message = s.str ();

        for (unsigned int i = 1; i < argumentsNumber; ++i) {
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
          endl << endl;

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
      "Checking single input source in argument vector in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  const vector<string>&
    argumentsVector =
      fOptionsAndArguments.getArgumentsVector ();

  unsigned int argumentsNumber =
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
        " in handler arguments vector for " <<
        fHandlerServiceName <<
        ":" <<
        endl;

      ++gIndenter;

//       gLogStream <<
//         fOptionsAndArguments;

      gLogStream << endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        fHandlerServiceName <<
        endl;
    }
  }
#endif

  // input source name
  // ------------------------------------------------------

  switch (argumentsNumber) {
    case 0:
      if (! fOahHandlerFoundAHelpOption) {
        string message =
          "Input file name or '-' for standard input expected";

        gLogStream <<
          message <<
          endl;

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
        stringstream s;

        s <<
          "Several input file names supplied to " <<
          fHandlerServiceName <<
          ", only one can be used";

        string message = s.str ();

        gLogStream <<
          message <<
          endl;

        mfDisplayStringsVector (
          argumentsVector,
          "The arguments vector",
          gLogStream);

        throw mfOahException (message);
      }
      break;
  } //  switch
}

void oahHandler::setOahHandlerFoundAHelpOption (
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Setting oahHandlerFoundAHelpOption in handler \"" <<
      fHandlerHeader <<
      "\", context: " <<
      context <<
      endl;
  }
#endif

  fOahHandlerFoundAHelpOption = true;
}

string oahHandler::fetchOutputFileNameFromTheOptions () const
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
      endl;
  }
#endif

  ++gIndenter;

  // check the handler groups options consistency
  if (fHandlerGroupsList.size ()) {
    for (
      list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
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
    "The names to elements map contains " <<
    mfSingularOrPlural (
      fHandlerNamesToElementsMap.size (), "element", "elements");

  if (fHandlerNamesToElementsMap.size ()) {
    gLogStream << ":" << endl;
    ++gIndenter;

    for (
      map<string, S_oahElement>::iterator i =
        fHandlerNamesToElementsMap.begin ();
      i != fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      string name          = (*i).first;
      S_oahElement element = (*i).second;

      gLogStream <<
        "\"" <<
        name <<
        "\":" <<
        endl;

      ++gIndenter;

      gLogStream <<
        element <<
        endl;

      --gIndenter;
    } // for

    --gIndenter;
  }
  else {
    gLogStream << endl;
  }
}

void oahHandler::displayNamesToElementsMapShort (ostream& os) const
{
  gLogStream <<
    "The names to elements map contains (short version) " <<
    mfSingularOrPlural (
      fHandlerNamesToElementsMap.size (), "element", "elements");

  if (fHandlerNamesToElementsMap.size ()) {
    gLogStream <<
      ":" <<
      endl;
    ++gIndenter;

    const int fieldWidth = 40;

    for (
      map<string, S_oahElement>::const_iterator i =
        fHandlerNamesToElementsMap.begin ();
      i != fHandlerNamesToElementsMap.end ();
      ++i
    ) {
      string       name    = (*i).first;
      S_oahElement element = (*i).second;

      gLogStream << left <<
        setw (fieldWidth) <<
        string ("\"") + name + "\"" <<
        ": " <<
        element->fetchNamesBetweenQuotes () <<
        endl;
    } // for

    --gIndenter;
  }
  else {
    gLogStream << endl;
  }
}

void oahHandler::displayElementUsesList (ostream& os) const
{
  gLogStream <<
    "The handler elements uses list contains " <<
    mfSingularOrPlural (
      fElementUsesList.size (), "element", "elements");

  if (fElementUsesList.size ()) {
    gLogStream << ":" << endl;
    ++gIndenter;

    int counter = 0;
    for (
      list<S_oahElementUse >::const_iterator i =
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
        right << setw (2) << counter <<
        ": FOO" <<
        endl;

      ++gIndenter;

      if (elementUse) { // JMI`JMIJMI
        gLogStream <<
          elementUse <<
          endl;
      }
      else {
        gLogStream <<
          "null" <<
          endl;
      }

      --gIndenter;
    } // for

    gLogStream << endl;

    --gIndenter;
  }
  else {
    gLogStream << endl;
  }
}

void oahHandler::displayOptionsAndArguments (ostream& os) const
{
  os <<
    "The handler options and arguments contain:" <<
    endl;

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
      endl;
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
            endl;
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
      endl;
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
            endl;
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
      endl;
  }
#endif

  // browse the prefixes
  if (fHandlerPrefixesMap.size ()) {
    for (
      map<string, S_oahPrefix>::const_iterator i = fHandlerPrefixesMap.begin ();
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
      list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
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
  ostream& os,
  int fieldWidth) const
{
  os << left <<
    "handlerServiceName:" <<
    endl;
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (fHandlerServiceName) <<
    endl;
  --gIndenter;

  os << left <<
    "handlerHeader:" <<
    endl;
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (fHandlerHeader) <<
    endl;
  --gIndenter;

  os << left <<
    "handlerDescription:" <<
    endl;
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (fHandlerDescription) <<
    endl;
  --gIndenter;

  os << left <<
    "handlerUsage:" <<
    endl;
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (fHandlerUsage) <<
    endl;
}

void oahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "Handler:" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  os <<
    "fHandlerMultiComponent:";

  if (fHandlerMultiComponent) {
    ++gIndenter;

    os <<
      fHandlerMultiComponent <<
      endl;

    --gIndenter;
  }
  else {
    os << "none" << endl;
  }

  // launch command
  // --------------------------------------

  os << left <<
    setw (fieldWidth) << "Launch command:" <<
    endl;

  ++gIndenter;

  os << left <<
    setw (fieldWidth) << "fLaunchCommandAsSupplied" << " : " <<
    fLaunchCommandAsSupplied <<
    endl <<
    setw (fieldWidth) << "fLaunchCommandWithLongOptionsNames" << " : " <<
    fLaunchCommandWithLongOptionsNames <<
    endl <<
    setw (fieldWidth) << "fLaunchCommandWithShortOptionsNames" << " : " <<
    fLaunchCommandWithShortOptionsNames <<
    endl <<
    setw (fieldWidth) << "fLaunchOptionsAndOrArgumentsAlreadyMet" << " : " <<
    fLaunchOptionsAndOrArgumentsAlreadyMet <<
    endl;


  --gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "oahHandlerFoundAHelpOption" << " : " <<
    fOahHandlerFoundAHelpOption <<
    endl <<

    setw (fieldWidth) <<
    "fHandlerUsedThruKind" << " : " <<
    oahHandlerUsedThruKindAsString (
      fHandlerUsedThruKind) <<
    endl;

  // print the options prefixes if any
  if (fHandlerPrefixesMap.size ()) {
    printKnownPrefixes (os);
  }

  // print the single-character options if any
  if (fSingleCharacterShortNamesSet.size ()) {
    printKnownSingleCharacterOptions (os);
  }

  // print the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahHandler::printShort (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "Handler:" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "oahHandlerFoundAHelpOption" << " : " <<
    fOahHandlerFoundAHelpOption <<
    endl <<

    setw (fieldWidth) <<
    "fHandlerUsedThruKind" << " : " <<
    oahHandlerUsedThruKindAsString (
      fHandlerUsedThruKind) <<
    endl;

/* JMI
  // print the options prefixes if any
  if (fHandlerPrefixesMap.size ()) {
    printKnownPrefixes (os);
  }

  // print the single-character options if any
  if (fSingleCharacterShortNamesSet.size ()) {
    printKnownSingleCharacterOptions (os);
  }
*/

  // print the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  // print the known options
if (false) { // JMI
  os <<
    "oahHandler known options" <<
    endl;
  displayNamesToElementsMap (os);
}

  --gIndenter;
}

void oahHandler::printSummary (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "Handler:" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);

  // print a summary of the options groups if any
  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_oahGroup group = (*i);

      // print the options group
      group->printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void oahHandler::printHelp (ostream& os) const
{
  // print the options handler description
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (
      fHandlerDescription);
  --gIndenter;

  // print the options handler usage
  os <<
    endl <<
    fHandlerUsage <<
    endl;

  os << endl;

  // print the options groups help
  if (fHandlerGroupsList.size ()) {
// JMI    ++gIndenter;

    list<S_oahGroup>::const_iterator
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
      os << endl;
    } // for

 // JMI   --gIndenter;
  }
}

void oahHandler::printOptionsSummary (ostream& os) const
{
  // print the options handler description
  os <<
    gIndenter.indentMultiLineString (
      fHandlerDescription);

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    ":" <<
    endl;

  // print the options handler description
  // print the options groups help summaries
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group summary
      (*i)->printOptionsSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahHandler::printHandlerAndGroupAndSubGroupSpecificHelp (
  ostream&      os,
  S_oahSubGroup subGroup) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    os <<
      "oahHandler::printHandlerAndGroupAndSubGroupSpecificHelp" <<
      endl;
  }
#endif

  // print the options handler help header and option names
  os <<
    fHandlerHeader <<
    ":" <<
    endl;

  // print the optons group subgroups specific help
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    list<S_oahGroup>::const_iterator
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
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahHandler::printNameIntrospectiveHelp (
  ostream&      os,
  const string& name)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    os << "Printing name \"" <<
      name <<
      "\" introspective help in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
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
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    gLogStream <<
      "--- Help for prefix \"" <<
      name <<
      "\" ---" <<
      endl;

    ++gIndenter;

    prefix->printHelp (os);

    --gIndenter;

    gIndenter.setIndent (saveIndent);

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
      int saveIndent = gIndenter.getIndent ();

      gIndenter.resetToZero ();

      // print the help
      gLogStream <<
        "--- Help for group \"" <<
        name <<
        "\" at help top level ---" <<
        endl;

      ++gIndenter;

      group->printHelp (os);

      --gIndenter;

      gIndenter.setIndent (saveIndent);
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
            getGroupUpLink ();

      int saveIndent = gIndenter.getIndent ();

      gIndenter.resetToZero ();

      // print the help
      gLogStream <<
        "--- Help for subgroup \"" <<
        name <<
        "\" in group \"" <<
        group->getGroupHeader () <<
        "\" ---" <<
        endl;

      ++gIndenter;

      group->
        printGroupAndSubGroupHelp (
          gLogStream,
          subGroup);

      --gIndenter;

      gIndenter.setIndent (saveIndent);
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
            getSubGroupUpLink ();

      // get the group upLink
      S_oahGroup
        group =
          subGroup->
            getGroupUpLink ();

      int saveIndent = gIndenter.getIndent ();

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
      gLogStream << endl;

      ++gIndenter;

      group->
        printGroupAndSubGroupAndAtomHelp (
          gLogStream,
          subGroup,
          atom);

      --gIndenter;

      gIndenter.setIndent (saveIndent);
    }

    else {
      if (! suffixHelpHasBeenProvided) { // JMI ONH
        stringstream s;

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

void oahHandler::findStringInHandler (
  const string& lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    os << "Finding string \"" <<
      lowerCaseString <<
      "\" in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  // does this handler's header match?
  Bool headerMatches =
    mfStringToLowerCase (fHandlerHeader).find (lowerCaseString) != string::npos;

  // does this handler's description match?
  Bool descriptionMatches =
    mfStringToLowerCase (fHandlerDescription).find (lowerCaseString) != string::npos;

  // does this handler's usage match?
  Bool usageMatches =
    mfStringToLowerCase (fHandlerUsage).find (lowerCaseString) != string::npos;

  if (headerMatches || descriptionMatches || usageMatches) {
    stringstream s;

    s <<
      fHandlerHeader <<
      ' ' <<
      fHandlerDescription <<
      ' ' <<
      fHandlerUsage;

    // append the string
    foundStringsList.push_back (s.str ());
  }

  // do this handler's prefixes match?
  if (fHandlerPrefixesMap.size ()) {
    ++gIndenter;

    for (
      map<string, S_oahPrefix>::const_iterator i =
        fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      ++i
    ) {
      S_oahPrefix
        prefix = (*i).second;

      // does the prefix match?
      prefix->
        findStringInPrefix (
          lowerCaseString,
          foundStringsList,
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
          foundStringsList,
          os);
    } // for

    --gIndenter;
  }
}

void oahHandler::includeOptionsFileInHandler (
  const string& optionsFileName,
  ostream&      os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    os << "Including options file \"" <<
      optionsFileName <<
      "\" in handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  // open options file
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    gLogStream <<
      "Opening options file \"" << optionsFileName << "\" for reading" <<
      endl;
  }
#endif

  ifstream
    optionsStream (
      optionsFileName.c_str (),
      ifstream::in);

  if (! optionsStream.is_open ()) {
    stringstream s;

    s <<
      "Could not open options file \"" <<
      optionsFileName <<
      "\" for reading, quitting";

    string message = s.str ();

    gLogStream <<
      message <<
      endl;

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
      endl;
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
  ostream& os) const
{
  displayOptionsAndArguments (os);
}

void oahHandler::printHandlerOptionsValues (
  ostream& os) const
{
#ifdef TRACING_IS_ENABLED
  // print the options handler values header
  unsigned int
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
      endl;
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
    endl;

  // print the options groups values
  if (fHandlerGroupsList.size ()) {
    ++gIndenter;

    list<S_oahGroup>::const_iterator
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
        os << endl;
      }
    } // for

    --gIndenter;
  }
}

void oahHandler::printHandlerOptionsValuesAll (
  ostream& os) const
{
#ifdef TRACING_IS_ENABLED
  // print the options handler values header
  unsigned int
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
      endl;
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
    endl;

  // print the options groups values
  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
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
      os << endl;
    } // for

    --gIndenter;
  }
}

void oahHandler::printIncludeFileNamesStack (
  ostream& os) const
{
  unsigned int includeFileNamesStackSize =
    fIncludeFileNamesStack.size ();

  os <<
    "The include file names stack contains " <<
    mfSingularOrPlural (
      includeFileNamesStackSize, "element", "elements");

  if (includeFileNamesStackSize) {
    os << ":" << endl;

    ++gIndenter;

    const unsigned int fieldWidth = 2;

    int counter = 1; // this is to be displayed to the user
    for (string fileName : fIncludeFileNamesStack) {
      os <<
        right << setw (fieldWidth) << counter++ <<
        ": [" <<
        fileName <<
        ']' <<
        endl;
    } //for

    --gIndenter;
  }
  else {
    os << endl;
  }
}

ostream& operator<< (ostream& os, const S_oahHandler& elt)
{
  elt->print (os);
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
      endl;
  }
#endif

  string prefixName = prefix->getPrefixName ();

  // is prefixName already known in prefixes map?
  map<string, S_oahPrefix>::const_iterator
    it =
      fHandlerPrefixesMap.find (
        prefixName);

  if (it != fHandlerPrefixesMap.end ()) {
    // prefixName is already known in the map
    stringstream s;

    s <<
      " prefix name \"" <<
      prefixName <<
      "\" is already known in OAH handler \"" <<
      fHandlerHeader <<
      "\"";

    oahError (s.str ());
  }

  // register prefix in the options prefixes map
  fHandlerPrefixesMap [prefix->getPrefixName ()] =
    prefix;
}

S_oahPrefix oahHandler::fetchNameInPrefixesMap (
  const string& prefixName) const
{
  S_oahPrefix result;

  // is prefixName already known in prefixes map?
  map<string, S_oahPrefix>::const_iterator
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
      endl;
  }
#endif

  // append group to elements list
  group->
    appendGroupToElementsList (this);

  // append group to groups list
  fHandlerGroupsList.push_back (group);

  // set the group upLink to this handler
  group->
    setHandlerUpLink (this);
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
      endl;
  }
#endif

  // prepend group  to elements list
  group->
    appendGroupToElementsList (this);

  // prepend group to the groups list
  fHandlerGroupsList.push_front (group);

  // set the group upLink to this handler
  group->
    setHandlerUpLink (this);
}

void oahHandler::printKnownPrefixes (ostream& os) const
{
  unsigned int oahHandlerPrefixesListSize =
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
    os << endl;

    ++gIndenter;

    for (
      map<string, S_oahPrefix>::const_iterator i =
        fHandlerPrefixesMap.begin ();
      i != fHandlerPrefixesMap.end ();
      ++i
    ) {
      S_oahPrefix
        prefix = (*i).second;

      os <<
        prefix->fetchPrefixNames () <<
        ":" <<
        endl <<
        gTab <<
          prefix->getPrefixDescription () <<
        endl;
    } // for

    --gIndenter;
  }
  else {
    os << endl;
  }
}

void oahHandler::printKnownSingleCharacterOptions (ostream& os) const
{
  unsigned int oahHandlerPrefixesListSize =
    fSingleCharacterShortNamesSet.size ();

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
    endl;

  // indent a bit more for readability
  ++gIndenter;

  if (oahHandlerPrefixesListSize) {
    set<string>::const_iterator
      iBegin = fSingleCharacterShortNamesSet.begin (),
      iEnd   = fSingleCharacterShortNamesSet.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string theString = (*i);

      cumulatedLength += theString.size ();
      if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) {
        os << endl;
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

    os << endl;
  }
  else {
    os <<
      "none" <<
      endl;
  }

  --gIndenter;

  os <<
    "They can be clustered, such as:" <<
    endl <<
    gTab << "'-vac'" <<
    endl <<
    "which is equivalent to:" <<
    endl <<
    gTab << "'-v, -a, -c'" <<
    endl;
}

// void oahHandler::printOptionsDefaultValuesInformation (ostream& os) const
// {
//   os <<
//     endl <<
//     "Some options needing a value can use a default value:" <<
//     endl;
//
//   ++gIndenter;
//
//   os  <<
//     fHandlerServiceName <<
//     gIndenter.indentMultiLineString (
// R"( supports two styles for this, see '-optional-values-style' option, -ovs, .)") <<
//     endl;
//
//   --gIndenter;
// }

void oahHandler::displayNamesToElementsMap (ostream& os) const
{
  unsigned int handlerElementsMapSize =
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
    endl;

  ++gIndenter;

  if (handlerElementsMapSize) {
    map<string, S_oahElement>::const_iterator
      iBegin = fHandlerNamesToElementsMap.begin (),
      iEnd   = fHandlerNamesToElementsMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i).first << "\" ==>" <<
        endl;

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
      "none" <<
      endl;
  }

  --gIndenter;

  // create a list of the options map elements names
  list<string> optionsMapElementsNamesList;

  unsigned int optionsMapElementsNamesListSize =
    optionsMapElementsNamesList.size ();

  if (optionsMapElementsNamesListSize) {
    for (
      map<string, S_oahElement>::const_iterator i = fHandlerNamesToElementsMap.begin ();
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
    endl;

  ++gIndenter;

  if (optionsMapElementsNamesListSize) {
    list<string>::const_iterator
      iBegin = optionsMapElementsNamesList.begin (),
      iEnd   = optionsMapElementsNamesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\" ==>" <<
        endl;

      if (++i == iEnd) break;
    } // for
  }
  else {
    os <<
      "none" <<
      endl;
  }

  --gIndenter;
}

S_oahElement oahHandler::fetchElementByNameInHandler (
  const string& name)
{
  S_oahElement result;

  for (
    list<S_oahGroup>::const_iterator i = fHandlerGroupsList.begin ();
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
  const string& lastOptionNameFound,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    ++gIndenter;

    gLogStream <<
      "Checking missing pending argv atom expecting a value \"" <<
      lastOptionNameFound <<
      "\", the pending atom expecting a value is:";

    if (fPendingArgvAtomExpectingAValue) {
      gLogStream << endl;
      ++gIndenter;

      gLogStream <<
        fPendingArgvAtomExpectingAValue;

      --gIndenter;
    }
    else {
      gLogStream <<
        " null" <<
        endl;
    }

    gLogStream << endl;
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
      elementValueKindAsString (
        fElementValueKind) <<
      endl;
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
        endl;
    }
#endif

    // handle the valued atom using the default value
    switch (fPendingArgvAtomExpectingAValue->getElementValueKind ()) {
      case oahElementValueKind::kElementValueWithout:
      case oahElementValueKind::kElementValueImplicit:
        unknownOptionNameError (
          lastOptionNameFound,
          "does not take an optional value, cannot be used with a '='");
        break;

      case oahElementValueKind::kElementValueMandatory:
        {
          stringstream s;

          s <<
            "Pending last argv atom expecting a value " <<
            fPendingArgvAtomExpectingAValue->fetchNamesBetweenQuotes () <<
            " used under name \"" <<
            fNameUsedForPendingArgvAtomExpectingAValue <<
            "\" expects a value" <<
            endl;

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
            endl;
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
        endl;
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
        endl;
    }
#endif
  }
}

void oahHandler::registerOahElementUse (
  S_oahElement  elementUsed,
  const string& nameUsed,
  const string& valueUsed)
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
      endl;
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
        endl;
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
  const string& nameUsed,
  const string& valueUsed)
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
  const string& nameUsed,
  const string& valueUsed)
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
      endl;
  }
#endif

  size_t valueUsedSize = valueUsed.size ();

  // register elementUsed in fLaunchCommandAsSupplied
  fLaunchCommandAsSupplied +=
    " -" + nameUsed;

  if (valueUsedSize) {
    if (valueUsed.find ('\'') != string::npos) {
      fLaunchCommandAsSupplied +=
        " \"" + valueUsed + '"';
    }
    else if (valueUsed.find ('"') != string::npos) {
      fLaunchCommandAsSupplied +=
        " '" + valueUsed + "\'";
    }
    else if (valueUsed.find (' ') != string::npos) {
      fLaunchCommandAsSupplied +=
        " \"" + valueUsed + '"';
    }
    else {
      fLaunchCommandAsSupplied +=
        ' ' + valueUsed;
    }
  }

  // register elementUsed in fLaunchCommandWithLongOptionsNames
  string longName = elementUsed->getLongName ();

  fLaunchCommandWithLongOptionsNames +=
    " -" + longName;

  if (valueUsedSize) {
    if (valueUsed.find ('\'') != string::npos) {
      fLaunchCommandWithLongOptionsNames +=
        " \"" + valueUsed + '"';
    }
    else if (valueUsed.find ('"') != string::npos) {
      fLaunchCommandWithLongOptionsNames +=
        " '" + valueUsed + "\'";
    }
    else if (valueUsed.find (' ') != string::npos) {
      fLaunchCommandWithLongOptionsNames +=
        " \"" + valueUsed + '"';
    }
    else {
      fLaunchCommandWithLongOptionsNames +=
        ' ' + valueUsed;
    }
  }

  // register elementUsed in fLaunchCommandWithShortOptionsNames
  if (valueUsed.find ('\'') != string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " \"" + valueUsed + '"';
  }
  else if (valueUsed.find ('"') != string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " '" + valueUsed + "\'";
  }
  else if (valueUsed.find (' ') != string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " \"" + valueUsed + '"';
  }
  else {
    fLaunchCommandWithShortOptionsNames +=
      ' ' + valueUsed;
  }
}

void oahHandler::registerArgumentUseInLaunchCommand (
  const string& argumentUsed)
{
  // register argumentUsed in fLaunchCommandAsSupplied
  if (argumentUsed.find ('\'') != string::npos) {
    fLaunchCommandAsSupplied +=
      " \"" + argumentUsed + '"';
  }
  else if (argumentUsed.find ('"') != string::npos) {
    fLaunchCommandAsSupplied +=
      " '" + argumentUsed + "\'";
  }
  else if (argumentUsed.find (' ') != string::npos) {
    fLaunchCommandAsSupplied +=
      " \"" + argumentUsed + '"';
  }
  else {
    fLaunchCommandAsSupplied +=
      ' ' + argumentUsed;
  }

  // register argumentUsed in fLaunchCommandWithLongOptionsNames
  if (argumentUsed.find ('\'') != string::npos) {
    fLaunchCommandWithLongOptionsNames +=
      " \"" + argumentUsed + '"';
  }
  else if (argumentUsed.find ('"') != string::npos) {
    fLaunchCommandWithLongOptionsNames +=
      " '" + argumentUsed + "\'";
  }
  else if (argumentUsed.find (' ') != string::npos) {
    fLaunchCommandWithLongOptionsNames +=
      " \"" + argumentUsed + '"';
  }
  else {
    fLaunchCommandWithLongOptionsNames +=
      ' ' + argumentUsed;
  }

  // register argumentUsed in fLaunchCommandWithShortOptionsNames
  if (argumentUsed.find ('\'') != string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " \"" + argumentUsed + '"';
  }
  else if (argumentUsed.find ('"') != string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " '" + argumentUsed + "\'";
  }
  else if (argumentUsed.find (' ') != string::npos) {
    fLaunchCommandWithShortOptionsNames +=
      " \"" + argumentUsed + '"';
  }
  else {
    fLaunchCommandWithShortOptionsNames +=
      ' ' + argumentUsed;
  }
}

void oahHandler::handleOptionPrefixName (
  const string& prefixName,
  size_t equalsSignPosition,
  const string& stringAfterEqualsSign)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Handling option prefix name \"" <<
      prefixName <<
      ", equalsSignPosition = \"" << equalsSignPosition << "\"" <<
      ", stringAfterEqualsSign = \"" << stringAfterEqualsSign << "\"" <<
      endl;
  }
#endif

  // split stringAfterEqualsSign into a list of strings
  // using the comma as separator
  list<string> chunksList;

  mfSplitStringIntoChunks (
    stringAfterEqualsSign,
    ",",
    chunksList);

  unsigned int chunksListSize = chunksList.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "There are " << chunksListSize << " chunk[s]" <<
      " in '" << stringAfterEqualsSign <<
      "':" <<
      endl;

    ++gIndenter;

    list<string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      gLogStream <<
        '[' << (*i) << "]";
      if (++i == iEnd) break;
      gLogStream <<
        ' ';
    } // for

    gLogStream << endl;

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
        list<string>::const_iterator i =
          chunksList.begin ();
        i != chunksList.end ();
        ++i
      ) {
        string singleOptionName = (*i);

        // build uncontracted option name
        string
          uncontractedOptionName =
            prefix->getPrefixErsatz () + singleOptionName;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
          gLogStream <<
            "Expanding option \"" << singleOptionName <<
            "\" to \"" << uncontractedOptionName <<
            "\"" <<
            endl;
        }
#endif

        // handle the uncontracted option name
        handleOptionNameCommon (
          uncontractedOptionName);
      } // for
    }
  }

  else {
    stringstream s;

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
  const string& optionName)
{
  Bool result (true); // until the contrary is known

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Checking whether optionName \"" <<
      optionName <<
      "\" is a single-character options cluster" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    optionName.size () != 0,
    "optionName.size () == 0");

  ++gIndenter;

  list<S_oahElement> clusterElementsList;

  // collect single-character elements in clusterElementsList
  for (
    string::const_iterator i = optionName.begin ();
    i != optionName.end ();
    ++i
  ) {
    string singleCharacterString (1, (*i));

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
      gLogStream <<
        "Considering single-character '" << singleCharacterString << "'" <<
        endl;
    }
#endif

    set<string>::const_iterator
      it =
        fSingleCharacterShortNamesSet.find (
          singleCharacterString);

    if (it != fSingleCharacterShortNamesSet.end ()) {
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
      gLogStream << endl;
    }
#endif

  unsigned int clusterElementsListSize =
    clusterElementsList.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "clusterElementsListSize = " << clusterElementsListSize <<
      endl;

    gLogStream <<
      "clusterElementsList:" <<
      endl;

    ++gIndenter;

    for (
      list<S_oahElement>::const_iterator i = clusterElementsList.begin ();
      i != clusterElementsList.end ();
      ++i
    ) {
      S_oahElement element = (*i);

      if (element) { // JMI should not be necessary...
        gLogStream <<
          element->
            fetchNamesBetweenQuotes () <<
          endl;
      }
    } // for

    --gIndenter;

    gLogStream <<
      "fSingleCharacterShortNamesSet:" <<
      endl;
    for (
      set<string>::const_iterator i = fSingleCharacterShortNamesSet.begin ();
      i != fSingleCharacterShortNamesSet.end ();
      ++i
    ) {
      string singleCharacterElementName = (*i);

      gLogStream <<
        "\"" <<
        singleCharacterElementName <<
        "\" ";
    } // for
    gLogStream << endl;
  }
#endif

  unsigned int optionNameSize =
    optionName.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "optionNameSize = " << optionNameSize <<
      endl;
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
      list<S_oahElement>::const_iterator i = clusterElementsList.begin ();
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
          endl;
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
        endl;
    }
#endif
    result = false;
  }

  --gIndenter;

  return result;
}

void oahHandler::handleNameContainingEqualSign (
  const string& optionName,
  size_t equalsSignPosition)
{
  // fetch the strings before and after the equals sign
  string
    stringBeforeEqualsSign =
      optionName.substr (0, equalsSignPosition);
  string
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
      endl;
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
        endl;
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
            endl;
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
  const string& optionName,
  const string& valueUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling option name \"" <<
      optionName <<
      "\" with value \"" <<
      valueUsed <<
      "\"" <<
      endl;
  }
#endif

  string
    trailerAfterFirstDash =
      optionName.substr (1, string::npos);

  // is optionName known in options map?
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
    endl;
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

string oahHandler::handleArgvOptionName (
  const string& optionName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling argv option name \"" <<
      optionName <<
      "\"" <<
      endl;
  }
#endif

  string currentOptionName;

  string
    trailerAfterFirstDash =
      optionName.substr (1, string::npos);

  /* JMI
  gLogStream <<
    "trailerAfterFirstDash \"" << trailerAfterFirstDash << "\" is preceded by a dash" <<
    endl;
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
        trailerAfterFirstDash.substr (1, string::npos);

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "\"" << currentOptionName << "\" is a double-dashed option" <<
          endl;
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
        endl;
    }
#endif
  }

  // handle the current option name
  handleOptionNameCommon (
    currentOptionName);

  return currentOptionName;
}

void oahHandler::handleOptionNameAndValueAtTopOahLevel (
  const string& optionName,
  const string& optionValue)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      endl <<
      "----------------------------------------------------------" <<
      endl <<
      "Handling options and arguments element [" <<
      optionName <<
      "] [" <<
      optionValue << ']' <<
      endl;
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

  if (firstNonDashCharacterPosition == string::npos) {
    unknownOptionNameError (
      optionName,
      "contains only dashes");
  }

  string
    optionNameNonDashPart =
      optionName.substr (
        firstNonDashCharacterPosition);

  // does optionNameNonDashPart contain an equal sign?
  size_t equalsSignPosition =
    optionNameNonDashPart.find ("=");

  if (equalsSignPosition != string::npos) {
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
  const string& theString)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling argv option value or argument \"" <<
      theString <<
      "\"" <<
      endl;

    ++gIndenter;

    gLogStream <<
      "The pending atom expecting a value is:";

    if (fPendingArgvAtomExpectingAValue) {
      gLogStream << endl;
      ++gIndenter;

      gLogStream <<
        fPendingArgvAtomExpectingAValue;

      --gIndenter;
    }
    else {
      gLogStream << " null";
    }

    gLogStream << endl;

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
        endl;
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
        endl;
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
          endl;
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
  const string& serviceName)
{
  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling the options from options and arguments in " <<
      "\"" << fHandlerHeader << "\"" <<
      " for \"" << fHandlerServiceName << "\"" <<
      endl;

    gGlobalTracingOahGroup->setTracingOah ();
  }
#endif

  string optionsPassDescription =
    "Handle the options from the options and arguments";

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      optionsPassDescription <<
      endl;

    gLogStream <<
      separator <<
      endl;
  }
#endif

  oahElementHelpOnlyKind result;

  // register all this handler's names in the names to elements map
  registerNamesInHandlerToTheNamesToElementsMap ();

  // initialize the history
  // only now because the library must initialized beforehand
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Initializing handler component" <<
      ", serviceName: " << serviceName <<
      endl;
  }
#endif

  initializeHandlerMultiComponent ();

  // create the elements uses list from the options and arguments
  createElementUsesListFromOptionsAndArguments ( // (pass 2)
    serviceName);

// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     // display the options and arguments
//     gLogStream <<
//       fOptionsAndArguments;
//   }
// #endif

  // apply the options in the elements uses list
  result =
    applyOptionsFromElementUsesList (); // (pass 3)

  // check the options and arguments
  checkOptionsAndArgumentsConsistency ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Result oahElementHelpOnlyKind: " <<
      elementHelpOnlyKindAsString (result) <<
      endl;
  }
#endif

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    optionsPassDescription,
    mfTimingItem::kMandatory,
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

  string optionsAndArgumentsPassDescription =
    "Handle the options and arguments from argc/argv";

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      optionsAndArgumentsPassDescription << " in \"" <<
      fHandlerHeader <<
      "\" for \"" <<
      fHandlerServiceName <<
      "\"" <<
      endl;
  }
#endif

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      optionsAndArgumentsPassDescription <<
      endl;

    gLogStream <<
      separator <<
      endl;
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
  fHandlerServiceName = string (argv [0]);

  // register all this handler's names in the names to elements map
  registerNamesInHandlerToTheNamesToElementsMap ();

  // initialize the history
  // only now because the library must initialized beforehand
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "Initializing handler component" <<
      ", fHandlerServiceName: " << fHandlerServiceName <<
      endl;
  }
#endif

  initializeHandlerMultiComponent ();

  // create the elements uses list from argc/argv (pass 2)
  createElementUsesListFromArgcArgv (
    argc, argv);

  // apply the options in the elements uses list
  result =
    applyOptionsFromElementUsesList (); // (pass 3)

  // check the options and arguments
  checkOptionsAndArguments ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "result oahElementHelpOnlyKind: " <<
      elementHelpOnlyKindAsString (result) <<
      endl;
  }
#endif

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    optionsAndArgumentsPassDescription,
    mfTimingItem::kMandatory,
    startClock,
    endClock);

  return result;
}

void oahHandler::createElementUsesListFromOptionsAndArguments (
  const string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Creating atom uses list from options and arguments in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
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
        stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" has been handled from argc/argv, cannot handle it from oahOptionsVector";

        oahError (s.str ());
      }
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      {
        stringstream s;

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
//   const vector<oahOption>&
//     optionsVector =
//       gGlobalServiceRunData->
//         getOptionsAndArguments ().getOptionsVector ();
//
//   if (optionsVector.size ()) {
//     for (unsigned int i = 0; i < optionsVector.size (); ++i) {
//         string optionName =
//           optionsVector [i].getOptionName ();
//         string optionValue =
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
//         endl;
//     }
// #endif
//   }
//
//   gLogStream << endl;
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
        "Creating atom uses list from argc/argv in \"" <<
        fHandlerHeader <<
        "\"" <<
        endl;
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
        stringstream s;

        s <<
          "handler \"" <<
          fHandlerHeader <<
          "\" cannot be handled twice from argc/argv";

        oahError (s.str ());
      }
      break;

    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      {
        stringstream s;

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

  string lastOptionNameFound;

  // loop over the contents of argv:
  while (true) {
    if (argv [n] == nullptr)
      break;

    string currentString = string (argv [n]);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    // print current option
    gLogStream <<
      endl <<
      "----------------------------------------------------------" <<
      endl <<
      "Handling argv element " <<
      n <<
      ": [" <<
      currentString <<
      ']' <<
      endl;
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
          endl;
  }
#endif

        // currentString is an argument
#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
            gLogStream <<
              currentString << " is an argument" <<
              endl;
        }
#endif

        // append it to the arguments vector
        fOptionsAndArguments.appendArgument (
          currentString);
      }

      else {
        // currentString is an option name
#ifdef TRACING_IS_ENABLED
        if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
            gLogStream <<
              currentString << " is an option name" <<
              endl;
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
      "Applying options from elements uses list in \"" <<
      fHandlerHeader << "\"" <<
      endl;
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
      list<S_oahElementUse >::const_iterator i =
        fElementUsesList.begin ();
      i != fElementUsesList.end ();
      ++i
    ) {
      S_oahElementUse elementUse = (*i);

      S_oahElement
        elementUsed =
          elementUse->getElementUsed ();

      string
        nameUsed =
          elementUse->getNameUsed (),
        valueUsed =
          elementUse->getValueUsed ();

      ++counter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
      gLogStream <<
        endl <<
        "----------------------------------------------------------" <<
        endl <<
        "Applying option " <<
        counter <<
        ": " <<
        elementUsed->fetchNamesBetweenQuotes () <<
        " used under name \"" <<
        nameUsed  <<
        "\" with value [" <<
        valueUsed << "]:" <<
        endl;

      ++gIndenter;

      gLogStream <<
        elementUsed->asShortString () <<
        endl;

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
          elementValueKindAsString (atomValueExpectedKind) <<
          "'" <<
          endl;
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
              endl;

              ++gIndenter;

              gLogStream <<
                atom->asShortString () <<
                endl;

              --gIndenter;
            }
#endif
            switch (atomValueKind) {
              case oahElementValueKind::kElementValueWithout:
                {
                  stringstream s;

                  s <<
                    "Atom with value " <<
                    atomExpectingAValue->fetchNamesBetweenQuotes () <<
                    " has been registered as without value";

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
                  stringstream s;

                  s <<
                    "Atom expecting a value " <<
                    atomExpectingAValue->fetchNamesBetweenQuotes () <<
                    " needs a non-empty value";

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
                endl;

              ++gIndenter;

              gLogStream <<
                atom->asShortString () <<
                endl;

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
        stringstream s;

        s <<
          "Element from the from the atom uses list for \"" <<
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

    gLogStream << endl;
  }
#endif

  // print the early options if so chosen
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayEarlyOptionsValues ()) {
    gGlobalOahEarlyOptions.print (
      gLogStream);

    gLogStream << endl;
  }
#endif

  // print the options values if so chosen
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayOptionsValues ()) {
    printHandlerOptionsValues (
      gLogStream);
    gLogStream << endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalDisplayOahGroup->getDisplayOptionsValuesAll ()) {
    printHandlerOptionsValuesAll (
      gLogStream);
    gLogStream << endl;
  }
#endif

  // check the consistency of the options
  // ------------------------------------------------------

//   gLogStream <<
//     "==> checkHandlerOptionsConsistency();" << // JMI
//     endl;

  checkHandlerOptionsConsistency ();

  --gIndenter;

  // was this run a 'help-only' one?
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    if (false && fOahHandlerFoundAHelpOption) { // JMIJMIJMI
      gLogStream <<
        "This is a help-only run " <<
        endl;

      return oahElementHelpOnlyKind::kElementHelpOnlyYes;
    }
    else {
      gLogStream <<
        "This is an actual conversion run " <<
        endl;

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
  const string& optionNameUsed,
  const string& valueUsed)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling known options and arguments element " <<
      element->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Element \"" <<
      element->fetchNamesBetweenQuotes () <<
      "\" is described by: " <<
      endl <<
      gTab <<
      element->getDescription () <<
      endl;
  }
#endif

  // is this element already present in the commande line?
  multiset<S_oahElement>::const_iterator
    it =
      fHandlerCommandLineElementsMultiset.find (
        element);

  if (it != fHandlerCommandLineElementsMultiset.end ()) {
    // yes, element is known in the list
    if (! element->getMultipleOccurrencesAllowed ()) {
      stringstream s;

      s <<
        "options and arguments element \"" <<
        element->fetchNames () <<
        "\" is already present in the options vector";

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
      endl;
  }
#endif

  fHandlerCommandLineElementsMultiset.insert (element);

  // determine element short and long names to be used,
  // in case one of them (short or long) is empty
  string
    shortName =
      element->getShortName (),
    longName =
      element->getLongName ();

  string
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
        endl;
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
    stringstream s;

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
  const string& optionNameUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling argv known element " <<
      element->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Element " <<
      element->fetchNamesBetweenQuotes () <<
      " is described by: " <<
      endl <<
      gTab <<
      element->getDescription () <<
      endl;
  }
#endif

  // is this element already present in the commande line?
  multiset<S_oahElement, compareOahElements>::const_iterator
    it =
      fHandlerCommandLineElementsMultiset.find (
        element);

  if (it != fHandlerCommandLineElementsMultiset.end ()) {
    // yes, element is known in the list
    if (! element->getMultipleOccurrencesAllowed ()) {
      stringstream s;

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
  string
    longName =
      element->getLongName (),
    shortName =
      element->getShortName ();

  string
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
        endl;
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
    stringstream s;

    s <<
      "Known option name \"" << optionNameUsed << "\" cannot be handled";

    oahInternalError (s.str ());
  }
}

void oahHandler::handleKnownOptionsVectorAtom (
  S_oahAtom     atom,
  const string& optionNameUsed,
  const string& valueUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling known options and arguments atom " <<
      atom->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      endl;
  }
#endif

  switch (atom->getElementValueKind ()) {
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
        stringstream s;

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
  const string& optionNameUsed)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Handling known argv atom " <<
      atom->fetchNamesBetweenQuotes () <<
      " under name \"" <<
      optionNameUsed <<
      "\"" <<
      endl;
  }
#endif

  if (fPendingArgvAtomExpectingAValue) {
    stringstream s;

    s <<
      "Pending argv atom expecting a value " <<
      fPendingArgvAtomExpectingAValue->fetchNamesBetweenQuotes () <<
      " used under name \"" <<
      fNameUsedForPendingArgvAtomExpectingAValue <<
      "\" expects a value" <<
      endl;

    oahWarning (s.str ()); // JMI
//     oahError (s.str ());
  }

  switch (atom->getElementValueKind ()) {
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
        stringstream s;

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
  const string& optionName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Handling unknown option name \"" <<
      optionName <<
      "\"" <<
      endl;
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
  string name,
  string context)
{
  stringstream s;

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
  string name,
  string context)
{
  stringstream s;

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

void oahHandler::handleOptionNameCommon (
  const string& optionName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    gLogStream <<
      "Common handling of option name \"" <<
      optionName <<
      "\"" <<
      endl;
  }
#endif

  // does optionName contain an equal sign?
  size_t equalsSignPosition =
    optionName.find ("=");

  if (equalsSignPosition != string::npos) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Handling option name \"" <<
      optionName <<
      "\" which is known to handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
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
//   string                        serviceName,
//   const oahOptionsAndArguments& optionsAndArguments)
// {
//   if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//     gLogStream <<
//       optionsAndArguments;
//   }
//
//   // analyze the options
//   const vector<oahOption>&
//     optionsVector =
//       fOptionsAndArguments.getOptionsVector ();
//
//   if (optionsVector.size ()) {
//     for (unsigned int i = 0; i < optionsVector.size (); ++i) {
//       string optionName =
//         optionsVector [i].getOptionName ();
//       string optionValue =
//         optionsVector [i].getOptionValue ();
//
//       Bool   argumentIsAnOption;
//       string argumentWithoutDash;
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
//             endl;
//         }
//
//         // is argumentWithoutDash starting with a prefix?
//         Bool   argumentWithoutDashStartsWithAPrefix;
//         string argumentWithoutDashToBeUsed = argumentWithoutDash;
//
//         if (argumentWithoutDash.substr (0, 2) == "t=") {
//           argumentWithoutDashStartsWithAPrefix = true;
//
//           argumentWithoutDashToBeUsed =
//            string ("t") + argumentWithoutDash.substr (2);
//         }
//         else if (argumentWithoutDash.substr (0, 6) == "trace=") {
//           argumentWithoutDashStartsWithAPrefix = true;
//
//           argumentWithoutDashToBeUsed =
//             string ("trace-") + argumentWithoutDash.substr (6);
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
//             endl;
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
//   const vector<string>&
//     optionArgumentsVector =
//       optionsAndArguments.getArgumentsVector ();
//
//   unsigned int optionArgumentsVectorSize =
//     optionArgumentsVector.size ();
//
//   switch (optionArgumentsVectorSize) {
//     case 0:
//       if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//         gLogStream <<
//           serviceName <<
//           " is being launched without any argument, \"-\" will be used" <<
//           endl;
//       }
//
//       fInputSourceName = "-";
//       break;
//
//     case 1:
//       {
//         string argument =
//           optionArgumentsVector [0];
//         if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
//           gLogStream <<
//             serviceName <<
//             " is being launched with single argument " <<
//             argument <<
//             endl;
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
//           endl;
//       }
//   } // switch
// }
//

void oahHandler::includeOptionsAndArgumentsFile (
  const string&           includeFileName,
  ostream&                os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
    os << "Including options and arguments file \"" <<
      includeFileName <<
      "\"" <<
      endl;
  }
#endif

  fIncludeFileNamesStack.push_back (includeFileName);

  // are there too many include levels?
  if (++fIncludeFilesCurrentLevel == pIncludeFilesMaximumLevel) {
    stringstream s;

    s <<
      "Including file [" <<
      includeFileName <<
      "]: more than " <<
      pIncludeFilesMaximumLevel <<
      " include levels, quitting";

    string message = s.str ();

    os <<
      message <<
      endl;

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
      endl;
  }
#endif

  ifstream
    optionsStream (
      includeFileName.c_str (),
      ifstream::in);

  if (! optionsStream.is_open ()) {
    stringstream s;

    s <<
      "Could not open options and arguments file \"" <<
      includeFileName <<
      "\" for reading, quitting";

    string message = s.str ();

    os <<
      message <<
      endl;

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
      endl <<
      fOptionsAndArguments;
  }
#endif

  fIncludeFileNamesStack.pop_back ();
  --fIncludeFilesCurrentLevel;
}

//______________________________________________________________________________
void oahHandler::includeOptionsFromFile (
  istream&                inputStream,
  oahOptionsAndArguments& optionsAndArguments)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> includeOptionsFromFile()" <<
      endl;
  }
#endif

  // decipher inputStream line by line
  while (true) {
    string currentLine;

    getline (inputStream, currentLine);

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
      gLogStream <<
        "==> currentLine: [" << currentLine << ']' <<
        endl;
    }
#endif

    ++gIndenter;

    // remove comment from currentLine if relevant
    size_t commentStart = currentLine.find_first_of ('#');

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "--> currentLine with comment: [" << currentLine << ']' <<
          endl;
      }
#endif

    if (commentStart != string::npos) {
      currentLine.erase (
        commentStart);

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
        gLogStream <<
          "--> currentLine without comment: [" << currentLine << ']' <<
          endl;
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
          endl;
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
            endl;
        }
#endif

        // fetch the option name part and value if any
        string optionNamePart (currentLine);
        string valuePart (currentLine);

        size_t afterOptionName =
          optionNamePart.find_first_not_of (
            "-"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789"
            "_"
            "=");

        if (afterOptionName == string::npos) {
          // there isn't any value after the option name
#ifdef TRACING_IS_ENABLED
          if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
            gLogStream <<
              "Found option [" << optionNamePart << ']' <<
              endl;
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
              endl;
          }
#endif
        }

        // the valuePart may be surrounded by quotes or double quotes,
        // as in a shell
        string
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
              endl <<
              "valuePartToBeUsed: [" << valuePartToBeUsed << ']' <<
              endl;
          }
#endif

//         size_t
//           firstNonDashCharacterPosition =
//             optionNameToBeUsed.find_first_of ("-");
//
//         if (firstNonDashCharacterPosition == string::npos) {
//           unknownOptionNameError (
//             optionName,
//             "contains only dashes");
//         }
//
//         string
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
//             endl;
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
            endl;
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
  string regularExpression (
    "([[:digit:]]+)" // generation number
    "."
    "([[:digit:]]+)" // major number
    "."
    "([[:digit:]]+)" // minor number
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for version string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          '[' << sm [i] << "] ";
      } // for

      gLogStream << endl;
    }
#endif

  if (smSize == 4) {
    // found an n.x.y specification
    string
      generationNumberValue = sm [1],
      majorNumberValue      = sm [2],
      minorNumberValue      = sm [3];

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
        "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
        "--> minorNumberValue = \"" << minorNumberValue << "\"" <<
        endl;
    }
#endif

    generationNumber = stoi (generationNumberValue);
    majorNumber      = stoi (majorNumberValue);
    minorNumber      = stoi (minorNumberValue);
  }

  else {
    // decipher theString with a two-number regular expression
    string regularExpression (
      "([[:digit:]]+)" // generation number
      "."
      "([[:digit:]]+)" // major number
      );

    regex  e (regularExpression);
    smatch sm;

    regex_match (theString, sm, e);

    unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
      gLogStream <<
        "There are " << smSize << " matches" <<
        " for chord details string '" << theString <<
        "' with regex '" << regularExpression <<
        "'" <<
        endl <<
        smSize << " elements: ";

        for (unsigned i = 0; i < smSize; ++i) {
          gLogStream <<
            '[' << sm [i] << "] ";
        } // for

        gLogStream << endl;
      }
#endif

    if (smSize == 3) {
      // found an n.x specification
      // assume implicit 0 minor number
      string
        generationNumberValue = sm [1],
        majorNumberValue      = sm [2];

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTraceComponents ()) {
        gLogStream <<
          "--> generationNumberValue = \"" << generationNumberValue << "\", " <<
          "--> majorNumberValue = \"" << majorNumberValue << "\", " <<
          endl;
      }
#endif

      generationNumber = stoi (generationNumberValue);
      majorNumber      = stoi (majorNumberValue);
      minorNumber      = 0;
    }

    else {
      stringstream s;

      s <<
        "version number argument '" << theString <<
        "' is ill-formed";

      oahError (s.str ());
    }
  }
*/


// //   const vector<oahOption>& // JMI
// //     optionsVector =
// //       fOptionsAndArguments.getOptionsVector ();
// //
// //   if (optionsVector.size ()) {
// //     s << ' ';
// //
// //     vector<oahOption>::const_iterator
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
// //     list<S_oahElement>::const_iterator
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

