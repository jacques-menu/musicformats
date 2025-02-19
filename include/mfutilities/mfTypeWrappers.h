/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfTypeWrappers___
#define ___mfTypeWrappers___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"


namespace MusicFormats
{

/*
  Built-in types such a int and std::string
  don't bear any application-specific semantics by themselve.

  The lack of type distinction between an int staff number and an int voice number
  easily lead to potential assignment and parameter passing errors.

  As a self-defensive measure, wrapping bare built-in types such as int and std::string
  in distinct classes without implicit conversions
  helps trapping such errors at compile time.

  Doing so with template classes is also a nice way to:
    -  provide a default bare values for variables of these types,
       such as 0 for an input line number;
    -  print such values with a prefix and postfix,
       to avoid taking care of that everywhere in the code.
       Incidentally, these values specified in the template definition
       help disambiguate these template class types in C++
*/

/*
A non-type template-parameter shall have one of the following (optionally cv-qualified) types:
  integral or enumeration type,
  pointer to object or pointer to function,
  lvalue reference to object or lvalue reference to function,
  pointer to member,
  std::nullptr_t.

template <std::string * temp> //pointer to object
void f()
{
   cout << *temp << endl;
}

template <std::string & temp> //reference to object
void g()
{
     cout << temp << endl;
     temp += "...appended some string";
}

std::string s; //must not be local as it must have external linkage!

int main() {
        s = "can assign values locally";
        f<&s>();
        g<s>();
        cout << s << endl;
        return 0;
}
*/

//______________________________________________________________________________
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
class EXP mfWrappedValue
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfWrappedValue (T bareValue);

    virtual               ~mfWrappedValue ();

  public:

    // set and get
    // ------------------------------------------------------

    T                     getBareValue () const
                              { return fBareValue; }

    std::string           getPrintPrefix () const
                              { return fPrintPrefix; }

    std::string           getPrintPostfix () const
                              { return fPrintPostfix; }

  public:

    // operators
    // ------------------------------------------------------

    // conversion
    explicit operator     T () const;

    bool                  operator == (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherValue) const;

    // comparisons, returning bare bool values for use by the STL
    bool                  operator != (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherValue) const;

    bool                  operator < (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherValue) const;

    bool                  operator <= (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherValue) const;

    bool                  operator >= (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherValue) const;

    bool                  operator > (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherValue) const;

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    T                     fBareValue;

    std::string           fPrintPrefix;
    std::string           fPrintPostfix;
};

// constructor
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
mfWrappedValue <T, printPrefix, printPostfix>::mfWrappedValue (T bareValue)
  : fBareValue (bareValue),
    fPrintPrefix (printPrefix)
{}

// destructor
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
mfWrappedValue <T, printPrefix, printPostfix>::~mfWrappedValue ()
{}

// conversion operator T ()
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
mfWrappedValue <T, printPrefix, printPostfix>::operator T () const
{
  return fBareValue;
}

// comparison operators
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator == (
  const mfWrappedValue& otherInputLineNumber) const
{
  return fBareValue == otherInputLineNumber.fBareValue;
}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator != (
  const mfWrappedValue& otherInputLineNumber) const
{
  return fBareValue != otherInputLineNumber.fBareValue;
}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator < (
  const mfWrappedValue& otherInputLineNumber) const
{
  return fBareValue < otherInputLineNumber.fBareValue;
}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator <= (
  const mfWrappedValue& otherInputLineNumber) const
{
  return fBareValue <= otherInputLineNumber.fBareValue;
}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator >= (
  const mfWrappedValue& otherInputLineNumber) const
{
  return fBareValue >= otherInputLineNumber.fBareValue;
}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator > (
  const mfWrappedValue& otherInputLineNumber) const
{
  return fBareValue < otherInputLineNumber.fBareValue;
}

// print
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
std::string mfWrappedValue <T, printPrefix, printPostfix>::asString () const
{
  std::stringstream ss;

  ss <<
    fPrintPrefix << fBareValue << fPrintPostfix;

  return ss.str ();
}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
void mfWrappedValue <T, printPrefix, printPostfix>::print (std::ostream& os) const
{
  os << asString ();
}

// operator <<
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
std::ostream& operator << (
  std::ostream&                                 os,
  const mfWrappedValue <T, printPrefix, printPostfix>& elt)
{
  elt.print (os);
  return os;
}






/*
https://stackoverflow.com/questions/8810224/inheriting-from-a-template-class-in-c

What you can do is to make Rectangle a template as well. If you write

template<typename T> class Rectangle:
  public Area<T>
{
  // ...
};
You have a template Rectangle from which you can get a class Rectangle<int> which derives from Area<int>, and a different class Rectangle<char> which derives from Area<char>.

It may be that you want to have a single type Rectangle so that you can pass all sorts of Rectangle to the same function (which itself doesn't need to know the Area type). Since the Rectangle<T> classes generated by instantiating the template Rectangle are formally independent of each other, it doesn't work that way. However you can make use of multiple inheritance here:

class Rectangle // not inheriting from any Area type
{
  // Area independent interface
};

template<typename T> class SpecificRectangle:
  public Rectangle,
  public Area<T>
{
  // Area dependent stuff
};

void foo(Rectangle&); // A function which works with generic rectangles

int main()
{
  SpecificRectangle<int> intrect;
  foo(intrect);

  SpecificRectangle<char> charrect;
  foo(charrect);
}
If it is important that your generic Rectangle is derived from a generic Area you can do the same trick with Area too:

class Area
{
  // generic Area interface
};

class Rectangle:
  public virtual Area // virtual because of "diamond inheritance"
{
  // generic rectangle interface
};

template<typename T> class SpecificArea:
  public virtual Area
{
  // specific implementation of Area for type T
};

template<typename T> class SpecificRectangle:
  public Rectangle, // maybe this should be virtual as well, in case the hierarchy is extended later
  public SpecificArea<T> // no virtual inheritance needed here
{
  // specific implementation of Rectangle for type T
};

*/




//______________________________________________________________________________
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
class EXP mfWrappedValueWithDefault
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfWrappedValueWithDefault ();

                          mfWrappedValueWithDefault (T bareValue);

    virtual               ~mfWrappedValueWithDefault ();

  public:

    // set and get
    // ------------------------------------------------------

    T                     getBareValue () const
                              { return fBareValue; }

    std::string           getPrintPrefix () const
                              { return fPrintPrefix; }

    std::string           getPrintPostfix () const
                              { return fPrintPostfix; }

  public:

    // operators
    // ------------------------------------------------------

    // conversion
    explicit operator     T () const;

    // comparisons, returning bare bool values for use by the STL
    bool                  operator == (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator != (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator < (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator <= (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator >= (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator > (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    T                     fBareValue;

    std::string           fPrintPrefix;
    std::string           fPrintPostfix;

    T                     fDefaultValue;
    std::string           fDefaultValueString;
};

// constructors
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::mfWrappedValueWithDefault ()
  : fBareValue (neutralValue),
      fPrintPrefix (printPrefix),
      fDefaultValue (neutralValue),
      fDefaultValueString (neutralValueString)
{}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::mfWrappedValueWithDefault (T bareValue)
  : fBareValue (bareValue),
    fPrintPrefix (printPrefix),
    fDefaultValue (neutralValue),
    fDefaultValueString (neutralValueString)
{}

// destructor
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::~mfWrappedValueWithDefault ()
{}

// conversion operator T ()
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::operator T () const
{
  return fBareValue;
}

// comparison operators
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::operator == (
  const mfWrappedValueWithDefault& otherInputLineNumber) const
{
  return fBareValue == otherInputLineNumber.fBareValue;
}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::operator != (
  const mfWrappedValueWithDefault& otherInputLineNumber) const
{
  return fBareValue != otherInputLineNumber.fBareValue;
}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::operator < (
  const mfWrappedValueWithDefault& otherInputLineNumber) const
{
  return fBareValue < otherInputLineNumber.fBareValue;
}

// print
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
std::string mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::asString () const
{
  std::stringstream ss;

  ss <<
    fPrintPrefix;

  if (fBareValue == fDefaultValue) {
    ss << fDefaultValueString;
  }
  else {
    ss << fBareValue;
  }

  return ss.str ();
}

template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
void mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::print (std::ostream& os) const
{
  os << asString ();
}

// operator <<
template <
  typename T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
std::ostream& operator << (
  std::ostream& os,
  const mfWrappedValueWithDefault <
    T,
    printPrefix,
    printPostfix,
    neutralValue,
    neutralValueString
  >&            elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
template <
  typename T,
  const std::string& printSeparator
>
class EXP mfRange
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfRange (
                            T rangeStart,
                            T rangeEnd);

    virtual               ~mfRange ();

  public:

    // set and get
    // ------------------------------------------------------

    T                     getRangeStart () const
                              { return fRangeStart; }

    T                     getRangeEnd () const
                              { return fRangeStart; }

    std::string           getPprintSeparator () const
                              { return fPprintSeparator; }

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    T                     fRangeStart;
    T                     fRangeEnd;

    std::string           fPprintSeparator;
};

// constructor
template <
  typename T,
  const std::string& printSeparator
>
mfRange <T, printSeparator>::mfRange (
  T rangeStart,
  T rangeEnd)
    : fRangeStart (rangeStart),
      fRangeEnd (rangeEnd),
      fPprintSeparator (printSeparator)
{}

// destructor
template <
  typename T,
  const std::string& printSeparator
>
mfRange <T, printSeparator>::~mfRange ()
{}

// print
template <
  typename T,
  const std::string& printSeparator
>
std::string mfRange <T, printSeparator>::asString () const
{
  std::stringstream ss;

  if (fRangeStart == fRangeEnd) {
    ss <<
      fRangeStart;
  }
  else {
    ss <<
      fRangeStart << fPprintSeparator << fRangeEnd;
  }

  return ss.str ();
}

template <
  typename T,
  const std::string& printSeparator
>
void mfRange <T, printSeparator>::print (std::ostream& os) const
{
  os << asString ();
}

// operator <<
template <
  typename T,
  const std::string& printSeparator
>
std::ostream& operator << (
  std::ostream&                      os,
  const mfRange <T, printSeparator>& elt)
{
  elt.print (os);
  return os;
}


}


#endif // ___mfTypeWrappers___
