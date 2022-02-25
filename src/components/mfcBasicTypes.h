/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfcBasicTypes___
#define ___mfcBasicTypes___

#include <string>
#include <list>

#include "exports.h"

#include "smartpointer.h"

#include "mfBool.h"


using namespace std;
using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
void crackVersionNumber ( // JMI ???
  const string& theString,
  int&          generationNumber,
  int&          majorNumber,
  int&          minorNumber);

//______________________________________________________________________________
Bool versionNumberGreaterThanOrEqualTo ( // JMI ???
  const string& versionNumber,
  const string& otherVersionNumber);

//______________________________________________________________________________
/*
  musicformats uses semantic versioning inspired from https://semver.org
*/
class EXP mfcVersionNumber : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcVersionNumber> create (
                            int           majorNumber,
                            int           minorNumber,
                            int           patchNumber,
                            const string& preRelease);

    // creation from  a string
    // ------------------------------------------------------

    static SMARTP<mfcVersionNumber> createFromString (
                            const string& theString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcVersionNumber (
                            int           majorNumber,
                            int           minorNumber,
                            int           patchNumber,
                            const string& preRelease);

    virtual               ~mfcVersionNumber ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getMajorNumber () const
                              { return fMajorNumber; }

    int                   getMinorNumber () const
                              { return fMinorNumber; }

    int                   getPatchNumber () const
                              { return fPatchNumber; }

    string                getPreRelease () const
                              { return fPreRelease; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  operator== (const mfcVersionNumber& other) const;

    Bool                  operator!= (const mfcVersionNumber& other) const;

    Bool                  operator<  (const mfcVersionNumber& other) const;

    Bool                  operator>= (const mfcVersionNumber& other) const;

    Bool                  operator>  (const mfcVersionNumber& other) const;

    Bool                  operator<= (const mfcVersionNumber& other) const;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    int                   fMajorNumber;
    int                   fMinorNumber;
    int                   fPatchNumber;
    string                fPreRelease;
};
typedef SMARTP<mfcVersionNumber> S_mfcVersionNumber;
EXP ostream& operator<< (ostream& os, const S_mfcVersionNumber& elt);

//______________________________________________________________________________
class EXP mfcVersionDescr : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcVersionDescr> create (
                            S_mfcVersionNumber  versionNumber,
                            const string&       versionDate,
                            const list<string>& versionDescriptionItems);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcVersionDescr (
                            S_mfcVersionNumber  versionNumber,
                            const string&       versionDate,
                            const list<string>& versionDescriptionItems);

    virtual               ~mfcVersionDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    S_mfcVersionNumber    getVersionNumber () const
                              { return fVersionNumber; }

    string                getVersionDate () const
                              { return fVersionDate; }

    const list<string>&   getVersionDescriptionItems () const
                              { return fVersionDescriptionItems; }

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    S_mfcVersionNumber    fVersionNumber;
    string                fVersionDate;
    list<string>          fVersionDescriptionItems;
};
typedef SMARTP<mfcVersionDescr> S_mfcVersionDescr;
EXP ostream& operator<< (ostream& os, const S_mfcVersionDescr& elt);

//______________________________________________________________________________
class EXP mfcVersionsHistory : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcVersionsHistory> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcVersionsHistory ();

    virtual               ~mfcVersionsHistory ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_mfcVersionDescr>&
                          getVersionsList () const
                              { return fVersionsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVersionDescrToHistory (
                            S_mfcVersionDescr versionDescr);

    S_mfcVersionDescr     fetchCurrentVersion () const;

    S_mfcVersionNumber    fetchCurrentVersionNumber () const;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    list<S_mfcVersionDescr>
                          fVersionsList;
};
typedef SMARTP<mfcVersionsHistory> S_mfcVersionsHistory;
EXP ostream& operator<< (ostream& os, const S_mfcVersionsHistory& elt);

//______________________________________________________________________________
enum class mfcComponenKind {
  kComponentRepresentation,
  kComponentPass,
  kComponentGenerator,
  kComponentConverter,
  kComponentLibrary
};

string componenKindAsString (
  mfcComponenKind componenKind);

ostream& operator<< (ostream& os, mfcComponenKind& elt);

//______________________________________________________________________________
class EXP mfcComponentDescr : public smartable
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcComponentDescr> create (
                            const string&   componentName,
                            mfcComponenKind componenKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcComponentDescr (
                            const string&   componentName,
                            mfcComponenKind componenKind);

    virtual               ~mfcComponentDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getComponentName () const
                              { return fComponentName; }

    mfcComponenKind       getComponenKind () const
                              { return fComponenKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_mfcVersionDescr     fetchComponentCurrentVersion () const
                              {
                                return
                                  fVersionsHistory->
                                     fetchCurrentVersion();
                              }

    S_mfcVersionNumber    fetchComponentCurrentVersionNumber () const
                              {
                                return
                                  fVersionsHistory->
                                     fetchCurrentVersionNumber();
                              }

    void                  appendVersionDescrToComponent (
                            S_mfcVersionDescr versionDescr);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    string                currentVersionNumberAndDateAsString () const;

    virtual void          print (ostream& os) const;

    virtual void          printVersion (ostream& os) const;
    virtual void          printHistory (ostream& os) const;

    virtual void          printOwnHistory (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fComponentName;

    mfcComponenKind       fComponenKind;

    S_mfcVersionsHistory  fVersionsHistory;
};
typedef SMARTP<mfcComponentDescr> S_mfcComponentDescr;
EXP ostream& operator<< (ostream& os, const S_mfcComponentDescr& elt);

//______________________________________________________________________________
class EXP mfcRepresentationComponent : public mfcComponentDescr
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcRepresentationComponent> create (
                            const string& formatName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcRepresentationComponent (
                            const string& formatName);

    virtual               ~mfcRepresentationComponent ();
};
typedef SMARTP<mfcRepresentationComponent> S_mfcRepresentationComponent;
EXP ostream& operator<< (ostream& os, const S_mfcRepresentationComponent& elt);

//______________________________________________________________________________
class EXP mfcPassComponent : public mfcComponentDescr
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcPassComponent> create (
                            const string& passName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcPassComponent (
                            const string& passName);

    virtual               ~mfcPassComponent ();
};
typedef SMARTP<mfcPassComponent> S_mfcPassComponent;
EXP ostream& operator<< (ostream& os, const S_mfcPassComponent& elt);

//______________________________________________________________________________
enum class mfcMultiComponentUsedFromTheCLIKind {
  kComponentUsedFromTheCLIYes,
  kComponentUsedFromTheCLINo
};

string componentUsedFromTheCLIKindAsString (
  mfcMultiComponentUsedFromTheCLIKind componentUsedFromTheCLIKind);

ostream& operator<< (ostream& os, mfcMultiComponentUsedFromTheCLIKind& elt);

enum class mfcMultiComponentEntropicityKind {
  kComponentEntropicityYes,
  kComponentEntropicityNo
};

string componentEntropicityKindAsString (
  mfcMultiComponentEntropicityKind componentEntropicityKind);

ostream& operator<< (ostream& os, mfcMultiComponentEntropicityKind& elt);

//______________________________________________________________________________
class EXP mfcMultiComponent : public mfcComponentDescr
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcMultiComponent> create (
                            const string&   multiComponentName,
                            mfcComponenKind componenKind,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcMultiComponent (
                            const string&   multiComponentName,
                            mfcComponenKind componenKind,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);

    virtual               ~mfcMultiComponent ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_mfcRepresentationComponent>&
                          getRepresentationComponentsList () const
                              { return fRepresentationComponentsList; }

    const list<S_mfcPassComponent>&
                          getPassComponentsList () const
                              { return fPassComponentsList; }

    mfcMultiComponentEntropicityKind
                          getComponentEntropicityKind () const
                              { return fComponentEntropicityKind; }

    mfcMultiComponentUsedFromTheCLIKind
                          getComponentUsedFromTheCLIKind () const
                              { return fComponentUsedFromTheCLIKind; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendRepresentationToMultiComponent (
                            S_mfcRepresentationComponent format);

    void                  appendPassToMultiComponent (
                            S_mfcPassComponent pass);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

    virtual void          printVersionShort (ostream& os) const;
    virtual void          printVersionFull (ostream& os) const;

    virtual void          printHistory (ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

    void                  printRepresentationsVersions (ostream&  os) const;
    void                  printRepresentationsHistory (ostream&  os) const;

    void                  printPassesVersions (ostream&  os) const;
    void                  printPassesHistory (ostream&  os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    list<S_mfcRepresentationComponent>
                          fRepresentationComponentsList;
    list<S_mfcPassComponent>
                          fPassComponentsList;

    // should the version number be at least equal to
    // the ones of the components?
    mfcMultiComponentEntropicityKind
                          fComponentEntropicityKind;

    mfcMultiComponentUsedFromTheCLIKind
                          fComponentUsedFromTheCLIKind;
};
typedef SMARTP<mfcMultiComponent> S_mfcMultiComponent;
EXP ostream& operator<< (ostream& os, const S_mfcMultiComponent& elt);

//______________________________________________________________________________
class EXP mfcGeneratorComponent : public mfcMultiComponent
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcGeneratorComponent> create (
                            const string&   generatorName,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcGeneratorComponent (
                            const string&   generatorName,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);

    virtual               ~mfcGeneratorComponent ();
};
typedef SMARTP<mfcGeneratorComponent> S_mfcGeneratorComponent;
EXP ostream& operator<< (ostream& os, const S_mfcGeneratorComponent& elt);

//______________________________________________________________________________
class EXP mfcConverterComponent : public mfcMultiComponent
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcConverterComponent> create (
                            const string&   converterName,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcConverterComponent (
                            const string&   converterName,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);

    virtual               ~mfcConverterComponent ();
};
typedef SMARTP<mfcConverterComponent> S_mfcConverterComponent;
EXP ostream& operator<< (ostream& os, const S_mfcConverterComponent& elt);

//______________________________________________________________________________
class EXP mfcLibraryComponent : public mfcMultiComponent
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcLibraryComponent> create (
                            const string&   libraryVersionsName,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcLibraryComponent (
                            const string&   libraryVersionsName,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);

    virtual               ~mfcLibraryComponent ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_mfcConverterComponent>&
                          getConverterComponentsList () const
                              { return fConverterComponentsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendConverterToMultiComponent (
                            S_mfcConverterComponent format);

  public:

    // print
    // ------------------------------------------------------

    void                  printVersionShort (ostream& os) const override;
    void                  printVersionFull (ostream& os) const override;

    void                  printHistory (ostream& os) const override;

  protected:

    // protected services
    // ------------------------------------------------------

    void                  printConvertersVersions (ostream&  os) const;
    void                  printConvertersHistory (ostream&  os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    list<S_mfcConverterComponent>
                          fConverterComponentsList;
};
typedef SMARTP<mfcLibraryComponent> S_mfcLibraryComponent;
EXP ostream& operator<< (ostream& os, const S_mfcLibraryComponent& elt);

}


#endif
