/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfWaeHandlersFrench.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_mfWaeHandlerFrench mfWaeHandlerFrench::create ()
{
  mfWaeHandlerFrench* o =
    new mfWaeHandlerFrench ();
  assert (o != nullptr);
  return o;
}

mfWaeHandlerFrench::mfWaeHandlerFrench ()
{}

mfWaeHandlerFrench::~mfWaeHandlerFrench ()
{}

std::string mfWaeHandlerFrench::asString () const
{
  std::stringstream s;

  s <<
    "Gestionnaire d'avertissements et d'erreurs de MusicFormats pour le français";

  return s.str ();
}

void mfWaeHandlerFrench::print (std::ostream& os) const
{
  os <<
    "mfWaeHandlerFrench" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerFrench& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
// pass names

std::string mfWaeHandlerFrench::passIDKindAsString (mfPassIDKind passIDKind) const
{
  std::string result;

  switch (passIDKind) {
    case mfPassIDKind::kMfPassID_UNKNOWN:
      result = "*Passe kMfPassID_UNKNOWN*";
      break;

    case mfPassIDKind::kMfPassID_ALL:
      result = "*Passe kMfPassID_ALL*";
      break;

    case mfPassIDKind::kMfPassID_0:
      result = "Passe 0";
      break;

    case mfPassIDKind::kMfPassID_Optional:
      result = "opt";
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

// std::string mfWaeHandlerFrench::passName (mfPassIDKind passIDKind) const
// {
//   return "Passe 1";
// }

std::string mfWaeHandlerFrench::passOptional () const
{
  return "Passe (optionelle)";
}

std::string mfWaeHandlerFrench::fullVersion () const
{
  return "version complète";
}
std::string mfWaeHandlerFrench::summary () const
{
  return "sommaire";
}
std::string mfWaeHandlerFrench::names () const
{
  return "noms";
}
std::string mfWaeHandlerFrench::slices () const
{
  return "tranches";
}

//_______________________________________________________________________________
// quitting after passes
std::string mfWaeHandlerFrench::quittingAfterPass (mfPassIDKind passIDKind) const
{
  std::stringstream s;

  s <<
    "On sort après la passe " <<
    passIDKindAsString (passIDKind) <<
    " comme demandé";

  return s.str ();
}

//_______________________________________________________________________________
// OAH

std::string mfWaeHandlerFrench::handleOptionsAndArgumentsFromArgcArgv () const
{
  return "Traitement des options et arguments de argc/argv";
}

//_______________________________________________________________________________
// passes

std::string mfWaeHandlerFrench::createAnMXSRFromAMusicXMLFile () const
{
  return "Création d'un MXSR à partir d'un fichier MusicXML";
}
std::string mfWaeHandlerFrench::createAnMXSRFromAMusicXMLDescriptor () const
{
  return "Création d'un MXSR à partir d'un descripteor MusicXML";
}
std::string mfWaeHandlerFrench::createAnMXSRFromAMusicXMLBuffer () const
{
  return "Création d'un MXSR à partir d'un tampon MusicXML";
}

std::string mfWaeHandlerFrench::createAnMSRSqueletonFromTheMXSR () const
{
  return "Création d'un squelette de MSR à partir du MXSR";
}

std::string mfWaeHandlerFrench::populateTheMSRSqueletonFromMusicXMLData () const
{
  return "Garnissage du squelette de MSR avec les données MusicXML";
}

std::string mfWaeHandlerFrench::convertTheFirstMSRIntoASecondMSR () const
{
  return "Conversion du premier MSR en un second MSR";
}

std::string mfWaeHandlerFrench::convertTheSecondMSRIntoAnLPSR () const
{
  return "Conversion du second MSR en un LPSR";
}

std::string mfWaeHandlerFrench::convertTheLPSRIntoLilyPondCode () const
{
  return "Conversion du LPSR en du code LilyPond";
}

//_______________________________________________________________________________
// display

std::string mfWaeHandlerFrench::displayTheFirstMSRSkeletonAsText () const
{
  return "Affichage du squelette du premier MSR en texte";
}
std::string mfWaeHandlerFrench::displayTheFirstMSRAsText () const
{
  return "Affichage du premier MSR en texte";
}
std::string mfWaeHandlerFrench::displayTheSecondMSRAsText () const
{
  return "Affichage du second MSR en texte";
}
std::string mfWaeHandlerFrench::displayTheLPSRAsText () const
{
  return "Affichage du LPSR en texte";
}

std::string mfWaeHandlerFrench::displayTheNamesInTheFirstMSR () const
{
  return "Affichage des noms du premier MSR";
}
std::string mfWaeHandlerFrench::displayTheNamesInTheSecondMSR () const
{
  return "Affichage des noms du second MSR";
}

std::string mfWaeHandlerFrench::displayASummaryOfTheFirstMSR () const
{
  return "Affichage du résumé du premier MSR";
}
std::string mfWaeHandlerFrench::displayASummaryOfTheSecondMSR () const
{
  return "Affichage du résumé du second MSR";
}

//_______________________________________________________________________________
// timing

std::string mfWaeHandlerFrench::timingInformation () const
{
  return "Consommation de temps";
}
std::string mfWaeHandlerFrench::activity () const
{
  return "Activité";
}
std::string mfWaeHandlerFrench::description () const
{
  return "Description";
}
std::string mfWaeHandlerFrench::kind () const
{
  return "Genre";
}
std::string mfWaeHandlerFrench::CPUSeconds () const
{
  return "CPU (sec)";
}
std::string mfWaeHandlerFrench::mandatory () const
{
  return "obligatoire";
}
std::string mfWaeHandlerFrench::optional () const
{
  return "optionnel";
}
std::string mfWaeHandlerFrench::totalSeconds () const
{
  return "Total (sec)";
}


} // namespace
