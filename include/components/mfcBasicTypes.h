/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfcBasicTypes___
#define ___mfcBasicTypes___

#include <string>
#include <list>

#include "exports.h"

#include "smartpointer.h"

#include "mfBool.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
void crackVersionNumber ( // JMI ??? v0.9.66
  const std::string& theString,
  int&  generationNumber,
  int&  majorNumber,
  int&  minorNumber);

//______________________________________________________________________________
Bool versionNumberGreaterThanOrEqualTo ( // JMI ???
  const std::string& versionNumber,
  const std::string& otherVersionNumber);

//______________________________________________________________________________
/*
  MusicFormats uses semantic versioning inspired from https://semver.org
*/
class EXP mfcVersionNumber : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcVersionNumber> create (
                            int                majorNumber,
                            int                minorNumber,
                            int                patchNumber,
                            const std::string& preRelease);

    // creation from  a std::string
    // ------------------------------------------------------

    static SMARTP<mfcVersionNumber> createFromString (
                            const std::string& theString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcVersionNumber (
                            int                majorNumber,
                            int                minorNumber,
                            int                patchNumber,
                            const std::string& preRelease);

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

    std::string           getPreRelease () const
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

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    int                   fMajorNumber;
    int                   fMinorNumber;
    int                   fPatchNumber;
    std::string           fPreRelease;
};
typedef SMARTP<mfcVersionNumber> S_mfcVersionNumber;
EXP std::ostream& operator << (std::ostream& os, const S_mfcVersionNumber& elt);

//______________________________________________________________________________
class EXP mfcVersion : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcVersion> create (
                            const S_mfcVersionNumber&     versionNumber,
                            const std::string&            versionDate,
                            const std::list<std::string>& versionDescriptionItems);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcVersion (
                            const S_mfcVersionNumber&     versionNumber,
                            const std::string&            versionDate,
                            const std::list<std::string>& versionDescriptionItems);

    virtual               ~mfcVersion ();

  public:

    // set and get
    // ------------------------------------------------------

    S_mfcVersionNumber    getVersionNumber () const
                              { return fVersionNumber; }

    std::string           getVersionDate () const
                              { return fVersionDate; }

    const std::list<std::string>&

                          getVersionDescriptionItems () const
                              { return fVersionDescriptionItems; }

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    S_mfcVersionNumber    fVersionNumber;
    std::string           fVersionDate;
    std::list<std::string>
                          fVersionDescriptionItems;
};
typedef SMARTP<mfcVersion> S_mfcVersion;
EXP std::ostream& operator << (std::ostream& os, const S_mfcVersion& elt);

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

    const std::list<S_mfcVersion>&
                          getVersionsList () const
                              { return fVersionsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVersionToHistory (
                            const S_mfcVersion& version);

    S_mfcVersion     fetchMostRecentVersion () const;

    S_mfcVersionNumber    fetchMostRecentVersionNumber () const;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::list<S_mfcVersion>
                          fVersionsList;
};
typedef SMARTP<mfcVersionsHistory> S_mfcVersionsHistory;
EXP std::ostream& operator << (std::ostream& os, const S_mfcVersionsHistory& elt);

//______________________________________________________________________________
enum class mfcComponentKind {
  kComponentRepresentation,
  kComponentPass,
  kComponentGenerator,
  kComponentConverter,
  kComponentLibrary
};

std::string mfcComponentKindAsString (
  mfcComponentKind componentKind);

std::ostream& operator << (std::ostream& os, const mfcComponentKind& elt);

//______________________________________________________________________________
class EXP mfcComponent : public smartable
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcComponent> create (
                            const std::string& componentName,
                            mfcComponentKind   componentKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcComponent (
                            const std::string& componentName,
                            mfcComponentKind   componentKind);

    virtual               ~mfcComponent ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getComponentName () const
                              { return fComponentName; }

    mfcComponentKind      getComponentKind () const
                              { return fComponentKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_mfcVersion     fetchComponentMostRecentVersion () const
                              {
                                return
                                  fVersionsHistory->
                                     fetchMostRecentVersion ();
                              }

    S_mfcVersionNumber    fetchComponentMostRecentVersionNumber () const
                              {
                                return
                                  fVersionsHistory->
                                     fetchMostRecentVersion ()->
                                        getVersionNumber ();
                              }

    void                  appendVersionToComponent (
                            const S_mfcVersion& version);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    std::string           mostRecentVersionNumberAndDateAsString () const;

    virtual void          print (std::ostream& os) const;

    virtual void          printVersion (std::ostream& os) const;
    virtual void          printHistory (std::ostream& os) const;

    virtual void          printOwnHistory (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fComponentName;

    mfcComponentKind      fComponentKind;

    S_mfcVersionsHistory  fVersionsHistory;
};
typedef SMARTP<mfcComponent> S_mfcComponent;
EXP std::ostream& operator << (std::ostream& os, const S_mfcComponent& elt);

//______________________________________________________________________________
class EXP mfcOahComponent : public mfcComponent
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcOahComponent> create (
                            const std::string& formatName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcOahComponent (
                            const std::string& formatName);

    virtual               ~mfcOahComponent ();
};
typedef SMARTP<mfcOahComponent> S_mfcOahComponent;
EXP std::ostream& operator << (std::ostream& os, const S_mfcOahComponent& elt);

//______________________________________________________________________________
class EXP mfcRepresentationComponent : public mfcComponent
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcRepresentationComponent> create (
                            const std::string& formatName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcRepresentationComponent (
                            const std::string& formatName);

    virtual               ~mfcRepresentationComponent ();
};
typedef SMARTP<mfcRepresentationComponent> S_mfcRepresentationComponent;
EXP std::ostream& operator << (std::ostream& os, const S_mfcRepresentationComponent& elt);

//______________________________________________________________________________
class EXP mfcPassComponent : public mfcComponent
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcPassComponent> create (
                            const std::string& passName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcPassComponent (
                            const std::string& passName);

    virtual               ~mfcPassComponent ();
};
typedef SMARTP<mfcPassComponent> S_mfcPassComponent;
EXP std::ostream& operator << (std::ostream& os, const S_mfcPassComponent& elt);

//______________________________________________________________________________
enum class mfcMultiComponentUsedFromTheCLIKind {
  kComponentUsedFromTheCLIYes,
  kComponentUsedFromTheCLINo
};

std::string mfcComponentUsedFromTheCLIKindAsString (
  mfcMultiComponentUsedFromTheCLIKind componentUsedFromTheCLIKind);

std::ostream& operator << (std::ostream& os, const mfcMultiComponentUsedFromTheCLIKind& elt);

enum class mfcMultiComponentEntropicityKind {
  kComponentEntropicityYes,
  kComponentEntropicityNo
};

std::string mfcComponentEntropicityKindAsString (
  mfcMultiComponentEntropicityKind componentEntropicityKind);

std::ostream& operator << (std::ostream& os, const mfcMultiComponentEntropicityKind& elt);

//______________________________________________________________________________
class EXP mfcMultiComponent : public mfcComponent
{
/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcMultiComponent> create (
                            const std::string& multiComponentName,
                            mfcComponentKind   componentKind,
                            mfcMultiComponentEntropicityKind
                                               componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                               componentUsedFromTheCLIKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcMultiComponent (
                            const std::string& multiComponentName,
                            mfcComponentKind   componentKind,
                            mfcMultiComponentEntropicityKind
                                               componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                               componentUsedFromTheCLIKind);

    virtual               ~mfcMultiComponent ();

  public:

    // set and get
    // ------------------------------------------------------

    S_mfcOahComponent     getOahComponent () const
                              { return fOahComponent; }

    const std::list<S_mfcRepresentationComponent>&
                          getRepresentationComponentsList () const
                              { return fRepresentationComponentsList; }

    const std::list<S_mfcPassComponent>&
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
                            const S_mfcRepresentationComponent& format);

    void                  appendPassToMultiComponent (
                            const S_mfcPassComponent& pass);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (std::ostream& os) const;

    virtual void          printVersionShort (std::ostream& os) const;
    virtual void          printVersionFull (std::ostream& os) const;

    virtual void          printHistory (std::ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

    void                  printOahVersion (std::ostream&  os) const;
    void                  printOahHistory (std::ostream&  os) const;

    void                  printRepresentationsVersions (std::ostream&  os) const;
    void                  printRepresentationsHistory (std::ostream&  os) const;

    void                  printPassesVersions (std::ostream&  os) const;
    void                  printPassesHistory (std::ostream&  os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    S_mfcOahComponent     fOahComponent;

    std::list<S_mfcRepresentationComponent>
                          fRepresentationComponentsList;

    std::list<S_mfcPassComponent>
                          fPassComponentsList;

    // should the version number be at least equal to
    // the ones of the components?
    mfcMultiComponentEntropicityKind
                          fComponentEntropicityKind;

    mfcMultiComponentUsedFromTheCLIKind
                          fComponentUsedFromTheCLIKind;
};
typedef SMARTP<mfcMultiComponent> S_mfcMultiComponent;
EXP std::ostream& operator << (std::ostream& os, const S_mfcMultiComponent& elt);

//______________________________________________________________________________
class EXP mfcGeneratorComponent : public mfcMultiComponent
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcGeneratorComponent> create (
                            const std::string& generatorName,
                            mfcMultiComponentEntropicityKind
                                               componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                               componentUsedFromTheCLIKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcGeneratorComponent (
                            const std::string& generatorName,
                            mfcMultiComponentEntropicityKind
                                               componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                               componentUsedFromTheCLIKind);

    virtual               ~mfcGeneratorComponent ();
};
typedef SMARTP<mfcGeneratorComponent> S_mfcGeneratorComponent;
EXP std::ostream& operator << (std::ostream& os, const S_mfcGeneratorComponent& elt);

//______________________________________________________________________________
class EXP mfcConverterComponent : public mfcMultiComponent
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcConverterComponent> create (
                            const std::string&   converterName,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcConverterComponent (
                            const std::string&   converterName,
                            mfcMultiComponentEntropicityKind
                                            componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                            componentUsedFromTheCLIKind);

    virtual               ~mfcConverterComponent ();
};
typedef SMARTP<mfcConverterComponent> S_mfcConverterComponent;
EXP std::ostream& operator << (std::ostream& os, const S_mfcConverterComponent& elt);

//______________________________________________________________________________
class EXP mfcLibraryComponent : public mfcMultiComponent
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfcLibraryComponent> create (
                            const std::string& libraryVersionsName,
                            mfcMultiComponentEntropicityKind
                                               componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                               componentUsedFromTheCLIKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfcLibraryComponent (
                            const std::string& libraryVersionsName,
                            mfcMultiComponentEntropicityKind
                                               componentEntropicityKind,
                            mfcMultiComponentUsedFromTheCLIKind
                                               componentUsedFromTheCLIKind);

    virtual               ~mfcLibraryComponent ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list<S_mfcConverterComponent>&
                          getConverterComponentsList () const
                              { return fConverterComponentsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendConverterToMultiComponent (
                            const S_mfcConverterComponent& format);

  public:

    // print
    // ------------------------------------------------------

    void                  printVersionShort (std::ostream& os) const override;
    void                  printVersionFull (std::ostream& os) const override;

    void                  printHistory (std::ostream& os) const override;

  protected:

    // protected services
    // ------------------------------------------------------

    void                  printConvertersVersions (std::ostream&  os) const;
    void                  printConvertersHistory (std::ostream&  os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::list<S_mfcConverterComponent>
                          fConverterComponentsList;
};
typedef SMARTP<mfcLibraryComponent> S_mfcLibraryComponent;
EXP std::ostream& operator << (std::ostream& os, const S_mfcLibraryComponent& elt);


}


#endif // ___mfcBasicTypes___
