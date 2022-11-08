/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrIdentification___
#define ___msrIdentification___

#include "msrElements.h"

// #include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrIdentification : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrIdentification> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrIdentification (
                            int inputLineNumber);

    virtual               ~msrIdentification ();

  public:

    // set and get
    // ------------------------------------------------------

    // work number
    void                  setIdentificationWorkNumber (
                            int           inputLineNumber,
                            const string& val);

    string                getIdentificationWorkNumber () const
                              { return fIdentificationWorkNumber; }

    // work title
    void                  setIdentificationWorkCreditTypeTitle (
                            int           inputLineNumber,
                            const string& val);

    string                getIdentificationWorkCreditTypeTitle () const
                              { return fIdentificationWorkCreditTypeTitle; }

    // opus
    void                  setIdentificationOpus (
                            int           inputLineNumber,
                            const string& val);

    string                getIdentificationOpus () const
                              { return fIdentificationOpus; }

    // movement number
    void                  setIdentificationMovementNumber (
                            int           inputLineNumber,
                            const string& val);

    string                getIdentificationMovementNumber () const
                              { return fIdentificationMovementNumber; }

    // movement title
    void                  setIdentificationMovementTitle (
                            int           inputLineNumber,
                            const string& val);

    string                getIdentificationMovementTitle () const
                              { return fIdentificationMovementTitle; }

    // encoding date
    void                  setIdentificationEncodingDate (
                            int           inputLineNumber,
                            const string& val);

    string                getIdentificationEncodingDate () const
                              { return fIdentificationEncodingDate; }

    // miscellaneous field
    void                  setIdentificationMiscellaneousField (
                            int           inputLineNumber,
                            const string& val);

    string                getIdentificationMiscellaneousField () const
                              { return fIdentificationMiscellaneousField; }

    // score instrument
    void                  setIdentificationScoreInstrument (
                            int           inputLineNumber,
                            const string& val);

    string                getIdentificationScoreInstrument () const
                              { return fIdentificationScoreInstrument; }

    // composers
    const list<string>&   getIdentificationComposersList () const
                              { return fIdentificationComposersList; }

    // lyricists
    const list<string>&   getIdentificationLyricistsList () const
                              { return fIdentificationLyricistsList; }

    // arrangers
    const list<string>&   getIdentificationArrangersList () const
                              { return fIdentificationArrangersList; }

    // poets
    const list<string>&   getIdentificationPoetsList () const
                              { return fIdentificationPoetsList; }

    // translators
    const list<string>&   getIdentificationTranslatorsList () const
                              { return fIdentificationTranslatorsList; }

    // artists
    const list<string>&   getIdentificationArtistsList () const
                              { return fIdentificationArtistsList; }

    // softwares
    const list<string>&   getIdentificationSoftwaresList () const
                              { return fIdentificationSoftwaresList; }

    // rights
    const list<string>&   getIdentificationRightsList () const
                              { return fIdentificationRightsList; }

    // sources
    const list<string>&   getIdentificationSourcesList () const
                              { return fIdentificationSourcesList; }

    // relations
    const list<string>&   getIdentificationRelationsList () const
                              { return fIdentificationRelationsList; }

  public:

    // public services
    // ------------------------------------------------------

    // composers
    void                  appendComposer (
                            int           inputLineNumber,
                            const string& value);

    // lyricists
    void                  appendLyricist (
                            int           inputLineNumber,
                            const string& value);

    // arrangers
    void                  appendArranger (
                            int           inputLineNumber,
                            const string& value);

    // poets
    void                  appendPoet (
                            int           inputLineNumber,
                            const string& value);

    // translators
    void                  appendTranslator (
                            int           inputLineNumber,
                            const string& value);

    // artists
    void                  appendArtist (
                            int           inputLineNumber,
                            const string& value);

    // softwares
    void                  appendSoftware (
                            int           inputLineNumber,
                            const string& value);

    // rights
    void                  appendRight (
                            int           inputLineNumber,
                            const string& value);

    // rources
    void                  appendSource (
                            int           inputLineNumber,
                            const string& value);

    // relations
    void                  appendRelation (
                            int           inputLineNumber,
                            const string& value);

    size_t                maxIdentificationNamesLength ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // work
    string                fIdentificationWorkNumber;
    string                fIdentificationWorkCreditTypeTitle;
    string                fIdentificationOpus;

    string                fIdentificationMovementNumber;
    string                fIdentificationMovementTitle;

    // encoding date
    string                fIdentificationEncodingDate;

    // miscellaneous field
    string                fIdentificationMiscellaneousField;

    // score instrument
    string                fIdentificationScoreInstrument;

    // creators
    list<string>          fIdentificationComposersList;
    list<string>          fIdentificationLyricistsList;
    list<string>          fIdentificationArrangersList;

    list<string>          fIdentificationPoetsList;
    list<string>          fIdentificationTranslatorsList;
    list<string>          fIdentificationArtistsList;

    list<string>          fIdentificationSoftwaresList;

    // rights
    list<string>          fIdentificationRightsList;
    // sources
    list<string>          fIdentificationSourcesList;

    // relations
    list<string>          fIdentificationRelationsList;
};
typedef SMARTP<msrIdentification> S_msrIdentification;
EXP ostream& operator << (ostream& os, const S_msrIdentification& elt);


} // namespace MusicFormats


#endif
