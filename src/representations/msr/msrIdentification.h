/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrIdentification___
#define ___msrIdentification___

#include <list>

#include "msrElements.h"


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
                            const std::string& val);

    std::string           getIdentificationWorkNumber () const
                              { return fIdentificationWorkNumber; }

    // work title
    void                  setIdentificationWorkCreditTypeTitle (
                            int           inputLineNumber,
                            const std::string& val);

    std::string           getIdentificationWorkCreditTypeTitle () const
                              { return fIdentificationWorkCreditTypeTitle; }

    // opus
    void                  setIdentificationOpus (
                            int           inputLineNumber,
                            const std::string& val);

    std::string           getIdentificationOpus () const
                              { return fIdentificationOpus; }

    // movement number
    void                  setIdentificationMovementNumber (
                            int           inputLineNumber,
                            const std::string& val);

    std::string           getIdentificationMovementNumber () const
                              { return fIdentificationMovementNumber; }

    // movement title
    void                  setIdentificationMovementTitle (
                            int           inputLineNumber,
                            const std::string& val);

    std::string           getIdentificationMovementTitle () const
                              { return fIdentificationMovementTitle; }

    // encoding date
    void                  setIdentificationEncodingDate (
                            int           inputLineNumber,
                            const std::string& val);

    std::string           getIdentificationEncodingDate () const
                              { return fIdentificationEncodingDate; }

    // miscellaneous field
    void                  setIdentificationMiscellaneousField (
                            int           inputLineNumber,
                            const std::string& val);

    std::string           getIdentificationMiscellaneousField () const
                              { return fIdentificationMiscellaneousField; }

    // score instrument
    void                  setIdentificationScoreInstrument (
                            int           inputLineNumber,
                            const std::string& val);

    std::string           getIdentificationScoreInstrument () const
                              { return fIdentificationScoreInstrument; }

    // composers
    const std::list<std::string>&
                          getIdentificationComposersList () const
                              { return fIdentificationComposersList; }

    // lyricists
    const std::list<std::string>&
                          getIdentificationLyricistsList () const
                              { return fIdentificationLyricistsList; }

    // arrangers
    const std::list<std::string>&
                          getIdentificationArrangersList () const
                              { return fIdentificationArrangersList; }

    // poets
    const std::list<std::string>&
                          getIdentificationPoetsList () const
                              { return fIdentificationPoetsList; }

    // translators
    const std::list<std::string>&
                          getIdentificationTranslatorsList () const
                              { return fIdentificationTranslatorsList; }

    // artists
    const std::list<std::string>&
                          getIdentificationArtistsList () const
                              { return fIdentificationArtistsList; }

    // softwares
    const std::list<std::string>&
                          getIdentificationSoftwaresList () const
                              { return fIdentificationSoftwaresList; }

    // rights
    const std::list<std::string>&
                          getIdentificationRightsList () const
                              { return fIdentificationRightsList; }

    // sources
    const std::list<std::string>&
                          getIdentificationSourcesList () const
                              { return fIdentificationSourcesList; }

    // relations
    const std::list<std::string>&
                          getIdentificationRelationsList () const
                              { return fIdentificationRelationsList; }

  public:

    // public services
    // ------------------------------------------------------

    // composers
    void                  appendComposer (
                            int           inputLineNumber,
                            const std::string& value);

    // lyricists
    void                  appendLyricist (
                            int           inputLineNumber,
                            const std::string& value);

    // arrangers
    void                  appendArranger (
                            int           inputLineNumber,
                            const std::string& value);

    // poets
    void                  appendPoet (
                            int           inputLineNumber,
                            const std::string& value);

    // translators
    void                  appendTranslator (
                            int           inputLineNumber,
                            const std::string& value);

    // artists
    void                  appendArtist (
                            int           inputLineNumber,
                            const std::string& value);

    // softwares
    void                  appendSoftware (
                            int           inputLineNumber,
                            const std::string& value);

    // rights
    void                  appendRight (
                            int           inputLineNumber,
                            const std::string& value);

    // rources
    void                  appendSource (
                            int           inputLineNumber,
                            const std::string& value);

    // relations
    void                  appendRelation (
                            int           inputLineNumber,
                            const std::string& value);

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // work
    std::string           fIdentificationWorkNumber;
    std::string           fIdentificationWorkCreditTypeTitle;
    std::string           fIdentificationOpus;

    std::string           fIdentificationMovementNumber;
    std::string           fIdentificationMovementTitle;

    // encoding date
    std::string           fIdentificationEncodingDate;

    // miscellaneous field
    std::string           fIdentificationMiscellaneousField;

    // score instrument
    std::string           fIdentificationScoreInstrument;

    // creators
    std::list<std::string>
                          fIdentificationComposersList;
    std::list<std::string>
                          fIdentificationLyricistsList;
    std::list<std::string>
                          fIdentificationArrangersList;

    std::list<std::string>
                          fIdentificationPoetsList;
    std::list<std::string>
                          fIdentificationTranslatorsList;
    std::list<std::string>
                          fIdentificationArtistsList;

    std::list<std::string>
                          fIdentificationSoftwaresList;

    // rights
    std::list<std::string>
                          fIdentificationRightsList;
    // sources
    std::list<std::string>
                          fIdentificationSourcesList;

    // relations
    std::list<std::string>
                          fIdentificationRelationsList;
};
typedef SMARTP<msrIdentification> S_msrIdentification;
EXP std::ostream& operator << (std::ostream& os, const S_msrIdentification& elt);


}


#endif // ___msrIdentification___
