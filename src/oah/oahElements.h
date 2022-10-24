/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahElements___
#define ___oahElements___

#include "smartpointer.h"

#include "basevisitor.h"

#include "oahBasicTypes.h"


using namespace std;

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
	kElementValueUnknown,		// default value
  kElementValueWithout,   // i.e. -cpu, -minimal, -cubase, groups and subgroups elements
  kElementValueImplicit,  // for oahBoolean
  kElementValueMandatory, // i.e. -global-staff-size 30
  kElementValueOptional   // i.e. -name-help, -name-help=cpu
};

string elementValueKindAsString (
  oahElementValueKind elementValueKind);

ostream& operator << (ostream& os, oahElementValueKind& elt);

enum class oahElementVisibilityKind {
	kElementVisibilityNone, // default value
  kElementVisibilityWhole,
  kElementVisibilityHeaderOnly,
  kElementVisibilityHidden
};

string elementVisibilityKindAsString (
  oahElementVisibilityKind elementVisibilityKind);

ostream& operator << (ostream& os, oahElementVisibilityKind& elt);

enum class oahElementHelpOnlyKind {
  kElementHelpOnlyYes,
  kElementHelpOnlyNo
};

string elementHelpOnlyKindAsString (
  oahElementHelpOnlyKind elementHelpOnlyKind);

ostream& operator << (ostream& os, oahElementHelpOnlyKind& elt);

//_______________________________________________________________________________
/*
  A type to hold the matched string and information about
  the oahElement that has been found to contain it
  in a OAH oahFindableElement's hierarchy

  In the pair:
    - first is the string that has been found
    - second is the description of the oahElement in which it has been found
*/
class oahFindStringMatch : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFindStringMatch> create (
    												const string& elementName,
                            const string& foundString,
                            const string& containingFindableElementInfo);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFindStringMatch (
    												const string& elementName,
                            const string& foundString,
                            const string& containingFindableElementInfo);

    virtual               ~oahFindStringMatch ();

  public:

    // set and get
    // ------------------------------------------------------

    const string&         getElementName () const
                              { return fElementName; }

    const string&         getFoundString () const
                              { return fFoundString; }

    const string&			    getContainingFindableElementInfo () const
                              { return fContainingFindableElementInfo; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    string			          fElementName;
    string			          fFoundString;
		string                fContainingFindableElementInfo;
};
typedef SMARTP<oahFindStringMatch> S_oahFindStringMatch;
EXP ostream& operator << (ostream& os, const S_oahFindStringMatch& elt);
EXP ostream& operator << (ostream& os, const oahFindStringMatch& elt);

//______________________________________________________________________________
/*
  A type to hold the the OAH elements that can be found by (sub)string,
  thus implementing OAH introspection
*/
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

    virtual Bool          findStringInFindableElement (
                            const string&               lowerCaseString,
                            list<S_oahFindStringMatch>& foundMatchesList,
                            ostream&                    os) const = 0;

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const = 0;

    virtual void          print (ostream& os) const = 0;

		virtual const string 	containingFindableElementAsString () const = 0;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahFindableElement> S_oahFindableElement;
EXP ostream& operator << (ostream& os, const S_oahFindableElement& elt);
EXP ostream& operator << (ostream& os, const oahFindableElement& elt);

//______________________________________________________________________________
/*
  a common ancestor for all OAH classes,
  i.e. atoms, subgroups and groups
*/

// PRE-declarations for classes mutual dependencies
class oahElement;
typedef SMARTP<oahElement> S_oahElement;

class EXP oahElement : public oahFindableElement
{
  public:

/* this class is purely virtual
    // creation
    // ------------------------------------------------------

    static SMARTP<oahElement> create (
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementValueKind      elementValueKind,
                            oahElementVisibilityKind elementVisibilityKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahElement (
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementValueKind      elementValueKind,
                            oahElementVisibilityKind elementVisibilityKind);

                          oahElement ();

    virtual               ~oahElement ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getLongName () const
                              { return fLongName; }

    string                getShortName () const
                              { return fShortName; }

    string                getShortNameOrLongNameIfEmpty () const;

    string                getDescription () const
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

  public:

    // public services
    // ------------------------------------------------------

    string                fetchNames () const;
    string                fetchNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                fetchNamesBetweenQuotes () const;

    string                fetchNamesBetweenParentheses () const;
    string                fetchNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    Bool                  nameIsANameForElement (const string& name)
                              {
                                return
                                  name == fLongName
                                    ||
                                  name == fShortName;
                              }

    virtual int           fetchVariableNameLength () const
                              { return fLongName.size (); }

    S_oahElement          thisElementIfItHasName (
                            const string& name);

    virtual void          applyElement (ostream& os) = 0;

    Bool                  findStringInFindableElement (
                            const string&               lowerCaseString,
                            list<S_oahFindStringMatch>& foundMatchesList,
                            ostream&                    os) const override;

		Bool									elementMatchesString (
														const string& lowerCaseString) const;
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) = 0;
    virtual void          acceptOut (basevisitor* v) = 0;

    virtual void          browseData (basevisitor* v) = 0;

  public:

    // print
    // ------------------------------------------------------

    virtual string        asActualLongNamedOptionString () const
                              { return '-' + fLongName; }
    virtual string        asShortNamedOptionString () const
                              { return '-' + fShortName; }

    string                asLongNamedOptionString () const;

    virtual string        asString () const override;
    virtual string        asShortString () const;

    virtual void          printOptionHeader (ostream& os) const;

    virtual void          printOahElementEssentials (
                            ostream& os,
                            int fieldWidth) const;
    virtual void          printOahElementEssentialsShort (
                            ostream& os,
                            int fieldWidth) const;

    virtual void          print (ostream& os) const override;
    virtual void          printShort (ostream& os) const;

    virtual void          printHelp (ostream& os) const;

		const string 					containingFindableElementAsString () const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fLongName;
    string                fShortName; // may be empty
    string                fDescription;

    oahElementValueKind   fElementValueKind;

    oahElementHelpOnlyKind
                          fElementHelpOnlyKind;

    oahElementVisibilityKind
                          fElementVisibilityKind;

    Bool                  fMultipleOccurrencesAllowed;
};
EXP ostream& operator << (ostream& os, const S_oahElement& elt);
EXP ostream& operator << (ostream& os, const oahElement& elt);

//______________________________________________________________________________
/*
Because the set needs a comparison functor to work with. If you don't specify one, it will make a default-constructed one. In this case, since you're using a function-pointer type, the default-constructed one will be a null pointer, which can't be called; so instead, you have to provide the correct function pointer at run time.

A better approach might be to use a function class type (a.k.a. functor type); then the function call can be resolved at compile time, and a default-constructed object will do the right thing.
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
                            S_oahElement  elementUsed,
                            const string& nameUsed,
                            const string& valueUsed);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahElementUse (
                            S_oahElement  elementUsed,
                            const string& nameUsed,
                            const string& valueUsed);

    virtual               ~oahElementUse ();

  public:

    // set and get
    // ------------------------------------------------------

    S_oahElement          getElementUsed () const
                              { return fElementUsed; }

    string                getNameUsed () const
                              { return fNameUsed; }

    void                  setValueUsed (const string& value)
                              { fValueUsed = value; }

    string                getValueUsed () const
                              { return fValueUsed; }

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahElement          fElementUsed;

    string                fNameUsed;
    string                fValueUsed;
};
typedef SMARTP<oahElementUse> S_oahElementUse;
EXP ostream& operator << (ostream& os, const S_oahElementUse& elt);


}


#endif
