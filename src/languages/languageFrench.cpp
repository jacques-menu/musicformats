/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "languageFrench.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_languageFrench languageFrench::create ()
{
  languageFrench* obj =
    new languageFrench ();
  assert (obj != nullptr);
  return obj;
}

languageFrench::languageFrench ()
{}

languageFrench::~languageFrench ()
{}

std::string languageFrench::asString () const
{
  std::stringstream ss;

  ss <<
    "Gestionnaire d'avertissements et d'erreurs de MusicFormats pour le français";

  return ss.str ();
}

void languageFrench::print (std::ostream& os) const
{
  os <<
    "languageFrench" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_languageFrench& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
// pass names

std::string languageFrench::passIDKindAsString (mfPassIDKind passIDKind) const
{
  std::string result;

  switch (passIDKind) {
    case mfPassIDKind::kMfPassID_UNKNOWN_:
      result = "*Passe kMfPassID_UNKNOWN_*";
      break;

    case mfPassIDKind::kMfPassID_ALL:
      result = "*Passe kMfPassID_ALL*";
      break;

    case mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling:
      result = "Opts & Args";
      break;

    case mfPassIDKind::kMfPassID_DisplayPass:
      result = "Affichage";
      break;

    case mfPassIDKind::kMfPassID_1:
     result = "Passe 1";
      break;

    case mfPassIDKind::kMfPassID_2:
     result = "Passe 2";
      break;
    case mfPassIDKind::kMfPassID_2a:
     result = "Passe 2a";
      break;
    case mfPassIDKind::kMfPassID_2b:
     result = "Passe 2b";
      break;

    case mfPassIDKind::kMfPassID_3:
     result = "Passe 3";
      break;
    case mfPassIDKind::kMfPassID_3a:
     result = "Passe 3a";
      break;
    case mfPassIDKind::kMfPassID_3b:
     result = "Passe 3b";
      break;

    case mfPassIDKind::kMfPassID_4:
     result = "Passe 4";
      break;
    case mfPassIDKind::kMfPassID_4a:
     result = "Passe 4a";
      break;
    case mfPassIDKind::kMfPassID_4b:
     result = "Passe 4b";
      break;

    case mfPassIDKind::kMfPassID_5:
     result = "Passe 5";
      break;
  } // switch

  return result;
}

std::string languageFrench::passOptional () const
{
  return "Passe (optionelle)";
}

std::string languageFrench::fullVersion () const
{
  return "version complète";
}
std::string languageFrench::summary () const
{
  return "sommaire";
}
std::string languageFrench::names () const
{
  return "noms";
}
std::string languageFrench::slices () const
{
  return "tranches";
}

//_______________________________________________________________________________
// quitting after passes
std::string languageFrench::quittingAfterPass (mfPassIDKind passIDKind) const
{
  std::stringstream ss;

  ss <<
    "On sort après la passe " <<
    passIDKindAsString (passIDKind) <<
    " comme demandé";

  return ss.str ();
}

//_______________________________________________________________________________
// OAH

std::string languageFrench::handleOptionsAndArgumentsFromArgcArgv () const
{
  return "Traitement des options et arguments de argc/argv";
}

//_______________________________________________________________________________
// passes

std::string languageFrench::convertAMusicXMLStreamIntoAnMXSR () const
{
  return "Conversion d'un flot de texte MusicXML en un MXSR (arbre MusicXML)";
}
std::string languageFrench::convertAMusicXMLDescriptorIntoAnMXSR () const
{
  return "Conversion d'un descripteor MusicXML en un MXSR (arbre MusicXML)";
}
std::string languageFrench::convertAMusicXMLBufferIntoAnMXSR () const
{
  return "Conversion d'un tampon MusicXML en un MXSR (arbre MusicXML)";
}

std::string languageFrench::convertTheMXSRIntoAnMSRSkeleton () const
{
  return "Conversion du MXSR en un squelette de MSR";
}

std::string languageFrench::populateTheMSRSkeletonFromMusicXMLData () const
{
  return "Garnissage du squelette de MSR avec les données MusicXML";
}

std::string languageFrench::convertTheFirstMSRIntoASecondMSR () const
{
  return "Conversion du premier MSR en un second MSR";
}

std::string languageFrench::convertTheSecondMSRIntoAnLPSR () const
{
  return "Conversion du second MSR en un LPSR";
}

std::string languageFrench::convertTheLPSRIntoLilyPondCode () const
{
  return "Conversion du LPSR en du code LilyPond";
}

//_______________________________________________________________________________
// display

std::string languageFrench::displayTheFirstMSRSkeletonAsText () const
{
  return "Affichage du squelette du premier MSR en texte";
}
std::string languageFrench::displayTheFirstMSRAsText () const
{
  return "Affichage du premier MSR en texte";
}
std::string languageFrench::displayTheSecondMSRAsText () const
{
  return "Affichage du second MSR en texte";
}
std::string languageFrench::displayTheLPSRAsText () const
{
  return "Affichage du LPSR en texte";
}

std::string languageFrench::displayTheNamesInTheFirstMSR () const
{
  return "Affichage des noms du premier MSR";
}
std::string languageFrench::displayTheNamesInTheSecondMSR () const
{
  return "Affichage des noms du second MSR";
}

std::string languageFrench::displayASummaryOfTheFirstMSR () const
{
  return "Affichage du résumé du premier MSR";
}
std::string languageFrench::displayASummaryOfTheSecondMSR () const
{
  return "Affichage du résumé du second MSR";
}

//_______________________________________________________________________________
// timing

std::string languageFrench::timingInformation () const
{
  return "Consommation de temps";
}
std::string languageFrench::activity () const
{
  return "Activité";
}
std::string languageFrench::description () const
{
  return "Description";
}
std::string languageFrench::kind () const
{
  return "Genre";
}
std::string languageFrench::CPUSeconds () const
{
  return "CPU (sec)";
}
std::string languageFrench::mandatory () const
{
  return "obligatoire";
}
std::string languageFrench::optional () const
{
  return "optionnel";
}
std::string languageFrench::totalSeconds () const
{
  return "Total (sec)";
}


} // namespace
