/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <ctime>
#include <iostream>
#include <string>
#include <stdlib.h>

#include "elements.h"
#include "factory.h"
#include "xml.h"
#include "xmlfile.h"


using namespace MusicXML2;


//------------------------------------------------------------------------
#define kDivision  4

//------------------------------------------------------------------------
// a function that return random numbers in the given range
//------------------------------------------------------------------------
static int getrandom(int range) {
#ifdef WIN32
  float f = (float)rand() / RAND_MAX;
#else
  float f = (float)random() / RAND_MAX;
#endif // WIN32

  return (int)(f * range);
}

static std::string randomNote() {
  int n = getrandom(7);
  std::string note;
  note += (char('A' + n));
  return note;
}

//------------------------------------------------------------------------
static Sxmlattribute newAttribute(const std::string& name, const std::string& value)
{
  Sxmlattribute attribute = xmlattribute::create();
  attribute->setName(name);
  attribute->setValue(value);
  return attribute;
}

//------------------------------------------------------------------------
static Sxmlattribute newAttributeI(const std::string& name, int value)
{
  Sxmlattribute attribute = xmlattribute::create();
  attribute->setName(name);
  attribute->setValue(value);
  return attribute;
}

//------------------------------------------------------------------------
static Sxmlelement newElement(int type, const std::string& value)
{
  Sxmlelement elt = factory::instance().create(type);
  elt->setValue (value);
  return elt;
}

//------------------------------------------------------------------------
static Sxmlelement newElementI(int type, int value)
{
  Sxmlelement elt = factory::instance().create(type);
  elt->setValue (value);
  return elt;
}

//------------------------------------------------------------------------
static Sxmlelement makeAttributes() {
  Sxmlelement attributes = factory::instance().create(k_attributes);
  attributes->push (newElementI(k_divisions, kDivision));

  Sxmlelement timeSignature = factory::instance().create(k_time);
  timeSignature->push (newElement(k_beats, "4"));
  timeSignature->push (newElement(k_beat_type, "4"));
  attributes->push (timeSignature);

  Sxmlelement clef = factory::instance().create(k_clef);
  clef->push (newElement(k_sign, "G"));
  clef->push (newElement(k_line, "2"));
  attributes->push (clef);

  return attributes;
}

//------------------------------------------------------------------------
// creates a measure containing random notes
// the function takes the measure number as an argument
//------------------------------------------------------------------------
static Sxmlelement makemeasure(unsigned long num) {
  Sxmlelement measure = factory::instance().create(k_measure);
  measure->add (newAttributeI("number", num));
  if (num==1) {          //  creates specific elements of the first measure
    measure->push(makeAttributes());    // division, time, clef...
  }


  for (int i = 0; i < 4; ++i) {    // next adds 4 quarter notes

    if (i == 0 || i == 2) { // add a harmony
        /*
          <harmony print-frame="no" default-y="31" relative-x="8">
            <root>
              <root-step>C</root-step>
            </root>
            <kind text="">major</kind>
            <staff>1</staff>
          </harmony>
        */
      Sxmlelement harmony = factory::instance().create(k_harmony);    // creates the harmony

      Sxmlelement root = factory::instance().create(k_root);    // creates the root
      root->push (newElement(k_root_step, "C"));
      harmony->push (root);

      Sxmlelement kind = newElement(k_kind, "major");    // creates the kind
      kind->add (newAttribute("text", "FOO"));
      harmony->push (kind);

      harmony->push (newElementI(k_staff, 1)); // add the staff

      measure->push (harmony);    // and finally adds the harmony to the measure
    }

    Sxmlelement note = factory::instance().create(k_note);    // creates the note
    Sxmlelement pitch = factory::instance().create(k_pitch);  // creates a pitch
    pitch->push (newElement(k_step, randomNote()));        // sets the pitch to a random value
    pitch->push (newElementI(k_octave, 4 + getrandom(2)));    // sets the octave to a random value
    note->push (pitch);                      // adds the pitch to the note
    note->push (newElementI(k_duration, kDivision));        // sets the note duration to a quarter note
    note->push (newElement(k_type, "quarter"));          // creates the graphic elements of the note
    measure->push (note);    // and finally adds the note to the measure
  } // for

  return measure;
}

/*
static Sxmlelement makemeasure_TRY(unsigned long num) {
  Sxmlelement measure = factory::instance().create(k_measure);
  measure->add (newAttributeI("number", num));
  if (num==1) {          //  creates specific elements of the first measure
    measure->push(makeAttributes());    // division, time, clef...
  }


  for (int i = 0; i < 4; ++i) {    // next adds 4 quarter notes

    if (i == 0 || i == 2) { // add a harmony
      Sxmlelement harmony = factory::instance().create(k_harmony);  // creates the harmony

      Sxmlelement root = factory::instance().create(k_root);  // creates the root
      root->push (newElement(k_root_step, "C"));
      root->push (newElement(k_root_alter,"1"));              // <-------------select the alteration
      harmony->push (root);

      Sxmlelement kind = newElement(k_kind, "[NONE]");  // creates the kind   <------------- change from "major" to "[NONE]"
      kind->add (newAttribute("halign", "center"));
      kind->add (newAttribute("text", "13b5"));                                                 // // <-------------- write any arbitrary chord name without being hassled
      harmony->push (kind);

      harmony->push (newElementI(k_staff, 1)); // add the staff

      measure->push (harmony);  // and finally adds the harmony to the measure
    }

  / *
    -use "[NONE]" for the kind allows the text to print and for me to select any arbitrary chord type
    -use k_root_alter to create sharps or flats of any basic note
    -the example above prints the chord C#13b5 onto beats 1 & 3 of each measure
  * /

    Sxmlelement note = factory::instance().create(k_note);    // creates the note
    Sxmlelement pitch = factory::instance().create(k_pitch);  // creates a pitch
    pitch->push (newElement(k_step, randomNote()));        // sets the pitch to a random value
    pitch->push (newElementI(k_octave, 4 + getrandom(2)));    // sets the octave to a random value
    note->push (pitch);                      // adds the pitch to the note
    note->push (newElementI(k_duration, kDivision));        // sets the note duration to a quarter note
    note->push (newElement(k_type, "quarter"));          // creates the graphic elements of the note
    measure->push (note);    // and finally adds the note to the measure
  } // for

  return measure;
}
*/

#define kPartID  "P1"
//------------------------------------------------------------------------
// creates a part containing 'count' measures
//------------------------------------------------------------------------
static Sxmlelement makePart (int count) {
  Sxmlelement part = factory::instance().create(k_part);
  part->add (newAttribute("id", kPartID));
  for (int i = 1; i<=count; ++i)      // and 'count' times
    part->push (makemeasure(i));      // adds a new measure to the part
  return part;
}

//------------------------------------------------------------------------
// creates the part list element
//------------------------------------------------------------------------
static Sxmlelement makePartList() {
  Sxmlelement partlist = factory::instance().create(k_part_list);
  Sxmlelement scorepart = factory::instance().create(k_score_part);
  scorepart->add (newAttribute("id", kPartID));
  scorepart->push (newElement(k_part_name, "Part name"));
  Sxmlelement scoreinstr = factory::instance().create(k_score_instrument);
  scoreinstr->add (newAttribute("id", "I1"));
  scoreinstr->push (newElement(k_instrument_name, "Any instr."));
  scorepart->push (scoreinstr);
  partlist->push(scorepart);
  return partlist;
}

//------------------------------------------------------------------------
// creates the identification element
//------------------------------------------------------------------------
static Sxmlelement makeIdentification() {
  Sxmlelement id = factory::instance().create(k_identification);
  Sxmlelement encoding = factory::instance().create(k_encoding);

  Sxmlelement creator = newElement(k_creator, "Georg Chance");
  creator->add(newAttribute("type", "Composer"));
  id->push (creator);

  encoding->push (newElement(k_software, "MusicFormats Library's MusicAndHarmonies generator"));
  id->push (encoding);
  return id;
}

//------------------------------------------------------------------------
// the function that creates and writes the score
//------------------------------------------------------------------------
static Sxmlelement randomMusic(int measuresCount) {
  Sxmlelement score = factory::instance().create(k_score_partwise);
  score->push (newElement(k_movement_title, "Random Music"));
  score->push (makeIdentification());
  score->push (makePartList());
  score->push(makePart(measuresCount));      // adds a part to the score
  return score;
}

//------------------------------------------------------------------------
int main (int argc, char*  argv[]) {
  // if present, reads the measures count from the command line
  int count = (argc == 2) ? atoi(argv[1]) : 20;

  // sets the random numbers seed
#ifdef WIN32
  srand((unsigned)time(0));
#else
  srandom((unsigned)time(0));
#endif // WIN32

  SXMLFile f = TXMLFile::create();
  f->set( new TXMLDecl("1.0", "UTF-8", TXMLDecl::kNo));
  f->set( new TDocType("score-partwise"));
  f->set( randomMusic(count) );
  f->print(std::cout);
  std::cout << std::endl;
    return 0;
}

