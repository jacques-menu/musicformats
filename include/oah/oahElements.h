/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahElements___
#define ___oahElements___

#include "smartpointer.h"

#include "basevisitor.h"

#include "oahBasicTypes.h"


namespace MusicFormats
{

// layout settings
//______________________________________________________________________________
EXP extern const int K_OAH_ELEMENTS_INDENTER_OFFSET;
  // indent a bit more for readability

EXP extern const int K_OAH_FIELD_WIDTH;

// data types
// ------------------------------------------------------

enum class oahElementValueKind {
  kElementValue_UNKNOWN_, // default value
  kElementValueLess,     // i.e. -cpu, -minimal, -cubase, groups and subgroups elements
//   kElementValueImplicit,  // for oahBoolean
  kElementValueFitted,   // i.e. -global-staff-size 30
  kElementValueDefaulted // i.e. -name-help for 'name' itself by default, or -name-help=cpu
};

std::string oahElementValueKindAsString (
  oahElementValueKind elementValueKind);

std::ostream& operator << (std::ostream& os, const oahElementValueKind& elt);

enum class oahElementVisibilityKind {
  kElementVisibilityNone, // default value
  kElementVisibilityWhole,
  kElementVisibilityHeaderOnly,
  kElementVisibilityHidden
};

std::string oahElementVisibilityKindAsString (
  oahElementVisibilityKind elementVisibilityKind);

std::ostream& operator << (std::ostream& os, const oahElementVisibilityKind& elt);

enum class oahElementHelpOnlyKind {
  kElementHelpOnlyYes,
  kElementHelpOnlyNo
};

std::string oahElementHelpOnlyKindAsString (
  oahElementHelpOnlyKind elementHelpOnlyKind);

std::ostream& operator << (std::ostream& os, const oahElementHelpOnlyKind& elt);

//_______________________________________________________________________________
/*
  A type to hold the matched std::string and information about
  the oahElement that has been found to contain it
  in a OAH oahFindableElement's hierarchy

  In the pair:
    - first is the std::string that has been found
    - second is the description of the oahElement in which it has been found
*/
class oahFindStringMatch : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFindStringMatch> create (
                            const std::string& elementName,
                            const std::string& foundString,
                            const std::string& containingFindableElementInfo);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFindStringMatch (
                            const std::string& elementName,
                            const std::string& foundString,
                            const std::string& containingFindableElementInfo);

    virtual               ~oahFindStringMatch ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::string&    getElementName () const
                              { return fElementName; }

    const std::string&    getFoundString () const
                              { return fFoundString; }

    const std::string&    getContainingFindableElementInfo () const
                              { return fContainingFindableElementInfo; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

  protected:

    // protected work fields
    // ------------------------------------------------------

    std::string           fElementName;

    std::string           fFoundString;
    std::string           fContainingFindableElementInfo;
};
typedef SMARTP<oahFindStringMatch> S_oahFindStringMatch;
EXP std::ostream& operator << (std::ostream& os, const S_oahFindStringMatch& elt);
EXP std::ostream& operator << (std::ostream& os, const oahFindStringMatch& elt);

//______________________________________________________________________________
/*
  A type to hold the the OAH elements that can be found by (sub)std::string,
  thus implementing OAH introspection
*/

// PRE-declarations
class   oahElement;
typedef SMARTP<oahElement> S_oahElement;

class EXP oahFindableElement : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahFindableElement> create ();
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFindableElement ();

    virtual               ~oahFindableElement ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual Bool          findStringInFindableElement ( // JMI v0.9.71 OAH_OAH
                            const std::string&               lowerCaseString,
                            std::list <S_oahFindStringMatch>& foundMatchesList,
                            std::ostream&                    os) const = 0;

    virtual Bool          fetchElementsMatchingString (
                            const std::string&       lowerCaseString,
                            std::list <S_oahElement>& foundElementsList,
//                             std::ostream&            os) const = 0;
                            std::ostream&            os) const { return true; }; // JMI v0.9.71 OAH_OAH

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const = 0;

    virtual void          print (std::ostream& os) const = 0;
    virtual void          printFull (std::ostream& os) const
                              { print (os); }

    virtual const std::string
//                           containingFindableElementAsString () const = 0;
                          containingFindableElementAsString () const { return ""; }; // JMI v0.9.71 OAH_OAH

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahFindableElement> S_oahFindableElement;
EXP std::ostream& operator << (std::ostream& os, const S_oahFindableElement& elt);
EXP std::ostream& operator << (std::ostream& os, const oahFindableElement& elt);

//______________________________________________________________________________
/*
  a common ancestor for all OAH classes,
  i.e. atoms, subgroups and groups
*/

// PRE-declarations
class   oahElement;
typedef SMARTP<oahElement> S_oahElement;

class EXP oahElement : public oahFindableElement //  smartable
{
  public:

/* this class is purely virtual
    // creation
    // ------------------------------------------------------

    static SMARTP<oahElement> create (
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementValueKind      elementValueKind,
                            oahElementVisibilityKind elementVisibilityKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahElement (
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementValueKind      elementValueKind,
                            oahElementVisibilityKind elementVisibilityKind);

                          oahElement ();

    virtual               ~oahElement ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getLongName () const
                              { return fLongName; }

    std::string           getShortName () const
                              { return fShortName; }

    std::string           getShortNameOrLongNameIfEmpty () const;

    std::string           getDescription () const
                              { return fDescription; }

    void                  setElementValueKind (
                            oahElementValueKind elementValueKind)
                              { fElementValueKind = elementValueKind; }

    oahElementValueKind   getElementValueKind () const
                              { return fElementValueKind; }

    oahElementHelpOnlyKind
                          getElementHelpOnlyKind () const
                              { return fElementHelpOnlyKind; }

    void                  setElementVisibilityKind (
                            oahElementVisibilityKind elementVisibilityKind)
                              { fElementVisibilityKind = elementVisibilityKind; }

    oahElementVisibilityKind
                          getElementVisibilityKind () const
                              { return fElementVisibilityKind; }

    void                  setMultipleOccurrencesAllowed ()
                              { fMultipleOccurrencesAllowed = true; }

    Bool                  getMultipleOccurrencesAllowed () const
                              { return fMultipleOccurrencesAllowed; }

    Bool                  getSelected () const
                              { return fSelected; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           fetchNames () const;
    std::string           fetchNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    std::string           fetchNamesBetweenQuotes () const;

    std::string           fetchNamesBetweenParentheses () const;
    std::string           fetchNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    Bool                  nameIsANameForElement (const std::string& name)
                              {
                                return
                                  name == fLongName
                                    ||
                                  name == fShortName;
                              }

    virtual int           fetchVariableNameLength () const
                              { return fLongName.size (); }

    S_oahElement          thisElementIfItHasName (
                            const std::string& name);

    Bool                  findStringInFindableElement (
                            const std::string&               lowerCaseString,
                            std::list <S_oahFindStringMatch>& foundMatchesList,
                            std::ostream&                    os) const override;

    Bool                  fetchElementsMatchingString (
                            const std::string&       lowerCaseString,
                            std::list <S_oahElement>& foundElementsList,
                            std::ostream&            os) const override;

    virtual Bool          elementMatchesString (
                            const std::string& lowerCaseString) const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) = 0;
    virtual void          acceptOut (basevisitor* v) = 0;

    virtual void          browseData (basevisitor* v) = 0;

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asActualLongNamedOptionString () const
                              { return '-' + fLongName; }
    virtual std::string   asShortNamedOptionString () const
                              { return '-' + fShortName; }

    std::string           asLongNamedOptionString () const;

    virtual std::string   asString () const override;
    virtual std::string   asShortString () const;

    virtual void          printOptionHeader (std::ostream& os) const;

    virtual void          printOahElementEssentials (
                            std::ostream& os,
                            int fieldWidth) const;
    virtual void          printOahElementEssentialsFull (
                            std::ostream& os,
                            int fieldWidth) const;

    virtual void          print (std::ostream& os) const override;
    virtual void          printFull (std::ostream& os) const override;

    virtual void          printHelp (std::ostream& os) const;

//     const std::string     containingFindableElementAsString () const;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fLongName;
    std::string           fShortName; // may be empty
    std::string           fDescription;

    oahElementValueKind   fElementValueKind;

    oahElementHelpOnlyKind
                          fElementHelpOnlyKind;

    oahElementVisibilityKind
                          fElementVisibilityKind;

    Bool                  fMultipleOccurrencesAllowed;

    Bool                  fSelected;
};
EXP std::ostream& operator << (std::ostream& os, const S_oahElement& elt);
EXP std::ostream& operator << (std::ostream& os, const oahElement& elt);

//______________________________________________________________________________
/*
  Because the set needs a comparison functor to work with. JMI v0.9.66

  If you don't specify one, it will make a default-constructed one.
  In this case, since you're using a function-pointer type,
  the default-constructed one will be a null pointer, which can't be called;
  so instead, you have to provide the correct function pointer at run time.

  A better approach might be to use a function class type (a.k.a. functor type);
  then the function call can be resolved at compile time,
  and a default-constructed object will do the right thing.
*/

struct compareOahElements {
                          bool operator() (
                            const S_oahElement firstElement,
                            const S_oahElement secondElement) const;
};

//______________________________________________________________________________
class EXP oahElementUse : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<oahElementUse> create (
                            const S_oahElement&  elementUsed,
                            const std::string& nameUsed,
                            const std::string& valueUsed);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahElementUse (
                            const S_oahElement&  elementUsed,
                            const std::string& nameUsed,
                            const std::string& valueUsed);

    virtual               ~oahElementUse ();

  public:

    // set and get
    // ------------------------------------------------------

    S_oahElement          getElementUsed () const
                              { return fElementUsed; }

    std::string           getNameUsed () const
                              { return fNameUsed; }

    void                  setValueUsed (const std::string& value)
                              { fValueUsed = value; }

    std::string           getValueUsed () const
                              { return fValueUsed; }

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahElement          fElementUsed;

    std::string           fNameUsed;
    std::string           fValueUsed;
};
typedef SMARTP<oahElementUse> S_oahElementUse;
EXP std::ostream& operator << (std::ostream& os, const S_oahElementUse& elt);


}


#endif // ___oahElements___
