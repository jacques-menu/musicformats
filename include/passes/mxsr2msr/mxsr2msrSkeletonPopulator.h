/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrTranslator___
#define ___mxsr2msrTranslator___

#include <map>

#include "typedefs.h"

#include "mxsrEventsCollections.h"

#include "mxsr2msrVoices.h"

#include "msrBarLines.h"
#include "msrDivisions.h"
#include "msrDoubleTremolos.h"
#include "msrTempos.h"
#include "msrTuplets.h"
#include "msrUseSymbols.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP mxsr2msrSkeletonPopulator :

  // MXSR score partwise
  // ------------------------------------------------------

  public                      visitor<S_score_partwise>,

  // scaling
  // ------------------------------------------------------

  public                      visitor<S_scaling>,
  public                      visitor<S_millimeters>,
  public                      visitor<S_tenths>,

  // layout
  // ------------------------------------------------------

  public                      visitor<S_page_layout>,
  public                      visitor<S_page_height>,
  public                      visitor<S_page_width>,
  public                      visitor<S_page_margins>,

  public                      visitor<S_left_margin>,
  public                      visitor<S_right_margin>,
  public                      visitor<S_top_margin>,
  public                      visitor<S_bottom_margin>,

  public                      visitor<S_system_layout>,
  public                      visitor<S_system_margins>,
  public                      visitor<S_system_distance>,
  public                      visitor<S_top_system_distance>,

  public                      visitor<S_staff_layout>,
  public                      visitor<S_staff_distance>,

  public                      visitor<S_measure_layout>,
  public                      visitor<S_measure_distance>,

  // appearance
  // ------------------------------------------------------

  public                      visitor<S_appearance>,
  public                      visitor<S_line_width>,
  public                      visitor<S_note_size>,
  public                      visitor<S_distance>,
  public                      visitor<S_glyph>,
  public                      visitor<S_other_appearance>,

  // system dividers
  // ------------------------------------------------------

  public                      visitor<S_system_dividers>,
  public                      visitor<S_left_divider>,
  public                      visitor<S_right_divider>,

  // notations
  // ------------------------------------------------------

  public                      visitor<S_notations>,
  public                      visitor<S_other_notation>,

  // groups
  // ------------------------------------------------------

  public                      visitor<S_group>,
  public                      visitor<S_group_name>,
  public                      visitor<S_group_abbreviation>,
  public                      visitor<S_group_abbreviation_display>,

  // parts
  // ------------------------------------------------------

  public                      visitor<S_part>,
  public                      visitor<S_part_abbreviation>,
  public                      visitor<S_part_abbreviation_display>,
  public                      visitor<S_part_symbol>,

  // JMI ??? 0.9.67
  // ------------------------------------------------------

  public                      visitor<S_attributes>,

  public                      visitor<S_divisions>,

  // staves
  // ------------------------------------------------------

  public                      visitor<S_staves>,
  public                      visitor<S_staff>,

  // staff details
  // ------------------------------------------------------

  public                      visitor<S_staff_details>,
  public                      visitor<S_staff_type>,
  public                      visitor<S_staff_lines>,
  public                      visitor<S_staff_tuning>,
  public                      visitor<S_tuning_step>,
  public                      visitor<S_tuning_octave>,
  public                      visitor<S_tuning_alter>,
  public                      visitor<S_capo>,
  public                      visitor<S_staff_size>,

  // voices
  // ------------------------------------------------------

  public                      visitor<S_voice>,

  // backup & forward
  // ------------------------------------------------------

  public                      visitor<S_backup>,
  public                      visitor<S_forward>,

  // clefs
  // ------------------------------------------------------

  public                      visitor<S_clef>,
  public                      visitor<S_sign>,
  public                      visitor<S_line>,
  public                      visitor<S_clef_octave_change>,

  // keys
  // ------------------------------------------------------

  public                      visitor<S_key>,
  public                      visitor<S_cancel>,
  public                      visitor<S_fifths>,
  public                      visitor<S_mode>,
  public                      visitor<S_key_step>,
  public                      visitor<S_key_alter>,
  public                      visitor<S_key_octave>,

  // time signatures
  // ------------------------------------------------------

  public                      visitor<S_time>,
  public                      visitor<S_beats>,
  public                      visitor<S_beat_type>,
  public                      visitor<S_senza_misura>,

  public                      visitor<S_interchangeable>,
  public                      visitor<S_time_relation>,

  // instruments
  // ------------------------------------------------------

  public                      visitor<S_score_instrument>,
  public                      visitor<S_instrument_name>,
  public                      visitor<S_solo>,
  public                      visitor<S_instruments>,

  // transposition
  // ------------------------------------------------------

  public                      visitor<S_transpose>,
  public                      visitor<S_diatonic>,
  public                      visitor<S_chromatic>,
  public                      visitor<S_octave_change>,
  public                      visitor<S_double>,

  // directions
  // ------------------------------------------------------

  public                      visitor<S_direction>,
  public                      visitor<S_direction_type>,
  public                      visitor<S_offset>,

  public                      visitor<S_words>,

  public                      visitor<S_octave_shift>,

  public                      visitor<S_scordatura>,
  public                      visitor<S_accord>,

  // tempo
  // ------------------------------------------------------

  public                      visitor<S_metronome>,
  public                      visitor<S_beat_unit>,
  public                      visitor<S_beat_unit_dot>,
  public                      visitor<S_per_minute>,
  public                      visitor<S_metronome_note>,
  public                      visitor<S_metronome_relation>,
  public                      visitor<S_metronome_type>,
  public                      visitor<S_metronome_beam>,
  public                      visitor<S_metronome_dot>,
  public                      visitor<S_metronome_tuplet>,
  public                      visitor<S_normal_dot>,

  // ties, slurs, brackets & beams
  // ------------------------------------------------------

  public                      visitor<S_tied>,
  public                      visitor<S_slur>,
  public                      visitor<S_bracket>,
  public                      visitor<S_beam>,

  // lyrics
  // ------------------------------------------------------

  public                      visitor<S_lyric>,
  public                      visitor<S_syllabic>,
  public                      visitor<S_text>,
  public                      visitor<S_elision>,
  public                      visitor<S_extend>,

  // harmonies
  // ------------------------------------------------------

  public                      visitor<S_degree>,
  public                      visitor<S_degree_value>,
  public                      visitor<S_degree_alter>,
  public                      visitor<S_degree_type>,

  public                      visitor<S_harmony>,
  public                      visitor<S_root>,
  public                      visitor<S_root_step>,
  public                      visitor<S_root_alter>,
  public                      visitor<S_function>,
  public                      visitor<S_kind>,
  public                      visitor<S_inversion>,

  public                      visitor<S_bass>,
  public                      visitor<S_bass_step>,
  public                      visitor<S_bass_alter>,

  public                      visitor<S_numeral>,
  public                      visitor<S_numeral_root>,
  public                      visitor<S_numeral_alter>,
  public                      visitor<S_numeral_key>,

  // frames
  // ------------------------------------------------------

  public                      visitor<S_frame>,
  public                      visitor<S_frame_strings>,
  public                      visitor<S_frame_frets>,
  public                      visitor<S_first_fret>,
  public                      visitor<S_frame_note>,
  public                      visitor<S_barre>,

  // figured bass
  // ------------------------------------------------------

  public                      visitor<S_figured_bass>,
  public                      visitor<S_figure>,
  public                      visitor<S_prefix>,
  public                      visitor<S_figure_number>,
  public                      visitor<S_suffix>,

  // measures
  // ------------------------------------------------------

  public                      visitor<S_measure>,
  public                      visitor<S_measure_numbering>,

  // printing
  // ------------------------------------------------------

  public                      visitor<S_print>,

  // miscellaneous??? JMI 0.9.71
  // ------------------------------------------------------

  public                      visitor<S_segno>,
  public                      visitor<S_coda>,
  public                      visitor<S_eyeglasses>,
  public                      visitor<S_pedal>,

  // barlines
  // ------------------------------------------------------

  public                      visitor<S_barline>,
  public                      visitor<S_bar_style>,
  public                      visitor<S_repeat>,
  public                      visitor<S_ending>,

  // notes
  // ------------------------------------------------------

  public                      visitor<S_note>,
  public                      visitor<S_step>,
  public                      visitor<S_alter>,
  public                      visitor<S_octave>,
  public                      visitor<S_duration>,
  public                      visitor<S_instrument>,
  public                      visitor<S_dot>,

  public                      visitor<S_rest>,

  public                      visitor<S_unpitched>,
  public                      visitor<S_display_step>,
  public                      visitor<S_display_octave>,

  public                      visitor<S_stem>,

  // note heads
  // ------------------------------------------------------

  public                      visitor<S_type>,
  public                      visitor<S_notehead>,
  public                      visitor<S_accidental>,

  // repeats
  // ------------------------------------------------------

  public                      visitor<S_measure_style>,

  public                      visitor<S_beat_repeat>,

  public                      visitor<S_measure_repeat>,

  public                      visitor<S_multiple_rest>,

  public                      visitor<S_slash>,
  public                      visitor<S_slash_type>,
  public                      visitor<S_slash_dot>,

  // articulations
  // ------------------------------------------------------

  public                      visitor<S_articulations>,
  public                      visitor<S_accent>,
  public                      visitor<S_breath_mark>,
  public                      visitor<S_caesura>,
  public                      visitor<S_spiccato>,
  public                      visitor<S_staccato>,
  public                      visitor<S_staccatissimo>,
  public                      visitor<S_stress>,
  public                      visitor<S_unstress>,
  public                      visitor<S_detached_legato>,
  public                      visitor<S_strong_accent>,
  public                      visitor<S_tenuto>,
  public                      visitor<S_fermata>,
  public                      visitor<S_arpeggiate>,
  public                      visitor<S_non_arpeggiate>,
  public                      visitor<S_doit>,
  public                      visitor<S_falloff>,
  public                      visitor<S_plop>,
  public                      visitor<S_scoop>,

  // technicals
  // ------------------------------------------------------

  public                      visitor<S_technical>,
  public                      visitor<S_arrow>,
  public                      visitor<S_bend>,
  public                      visitor<S_bend_alter>,
  public                      visitor<S_double_tongue>,
  public                      visitor<S_down_bow>,
  public                      visitor<S_fingering>,
  public                      visitor<S_fingernails>,
  public                      visitor<S_fret>,
  public                      visitor<S_hammer_on>,
  public                      visitor<S_handbell>,
  public                      visitor<S_harmonic>,
  public                      visitor<S_heel>,
  public                      visitor<S_hole>,
  public                      visitor<S_open_string>,
  public                      visitor<S_other_technical>,
  public                      visitor<S_pluck>,
  public                      visitor<S_pull_off>,
  public                      visitor<S_snap_pizzicato>,
  public                      visitor<S_stopped>,
  public                      visitor<S_string>,
  public                      visitor<S_tap>,
  public                      visitor<S_thumb_position>,
  public                      visitor<S_toe>,
  public                      visitor<S_triple_tongue>,
  public                      visitor<S_up_bow>,

  // ornaments
  // ------------------------------------------------------

  public                      visitor<S_ornaments>,
  public                      visitor<S_trill_mark>,
  public                      visitor<S_dashes>,
  public                      visitor<S_wavy_line>,
  public                      visitor<S_turn>,
  public                      visitor<S_inverted_turn>,
  public                      visitor<S_delayed_turn>,
  public                      visitor<S_delayed_inverted_turn>,
  public                      visitor<S_vertical_turn>,
  public                      visitor<S_mordent>,
  public                      visitor<S_inverted_mordent>,
  public                      visitor<S_schleifer>,
  public                      visitor<S_shake>,
  public                      visitor<S_accidental_mark>,

  // tremolos
  // ------------------------------------------------------

  public                      visitor<S_tremolo>,

  // dynamics
  // ------------------------------------------------------

  public                      visitor<S_f>,
  public                      visitor<S_ff>,
  public                      visitor<S_fff>,
  public                      visitor<S_ffff>,
  public                      visitor<S_fffff>,
  public                      visitor<S_ffffff>,

  public                      visitor<S_p>,
  public                      visitor<S_pp>,
  public                      visitor<S_ppp>,
  public                      visitor<S_pppp>,
  public                      visitor<S_ppppp>,
  public                      visitor<S_pppppp>,

  public                      visitor<S_mf>,
  public                      visitor<S_mp>,
  public                      visitor<S_fp>,
  public                      visitor<S_fz>,
  public                      visitor<S_pf>, // 3.1

  public                      visitor<S_rf>,
  public                      visitor<S_sf>,
  public                      visitor<S_rfz>,
  public                      visitor<S_sfz>,

  public                      visitor<S_sfp>,
  public                      visitor<S_sfpp>,
  public                      visitor<S_sffz>,
  public                      visitor<S_sfzp>, // 3.1

  public                      visitor<S_n>, // niente, 3.1

  public                      visitor<S_other_dynamics>,

  public                      visitor<S_wedge>,

  // cue notes
  // ------------------------------------------------------

  public                      visitor<S_cue>,

  // grace notes
  // ------------------------------------------------------

  public                      visitor<S_grace>,

  // chords
  // ------------------------------------------------------

  public                      visitor<S_chord>,

  // time modification in notes
  // ------------------------------------------------------

  public                      visitor<S_time_modification>,
  public                      visitor<S_actual_notes>,
  public                      visitor<S_normal_notes>,
  public                      visitor<S_normal_type>,

  // tuplets
  // ------------------------------------------------------

  public                      visitor<S_tuplet>,

  public                      visitor<S_tuplet_actual>,
  public                      visitor<S_tuplet_normal>,
  public                      visitor<S_tuplet_number>,
  public                      visitor<S_tuplet_type>,
  public                      visitor<S_tuplet_dot>,

  // glissandos
  // ------------------------------------------------------

  public                      visitor<S_glissando>,

  // slides
  // ------------------------------------------------------

  public                      visitor<S_slide>,

  // rehearsals
  // ------------------------------------------------------

  public                      visitor<S_rehearsal>,

  // accordion registration
  // ------------------------------------------------------

  public                      visitor<S_accordion_registration>,
  public                      visitor<S_accordion_high>,
  public                      visitor<S_accordion_middle>,
  public                      visitor<S_accordion_low>,

  // harp pedals tuning
  // ------------------------------------------------------

  public                      visitor<S_harp_pedals>,
  public                      visitor<S_pedal_tuning>,
  public                      visitor<S_pedal_step>,
  public                      visitor<S_pedal_alter>,

  public                      visitor<S_damp>,
  public                      visitor<S_damp_all>,

  // sound
  // ------------------------------------------------------

  public                      visitor<S_sound>,

  // midi
  // ------------------------------------------------------

  public                      visitor<S_instrument_sound>,
  public                      visitor<S_virtual_instrument>,
  public                      visitor<S_midi_device>,
  public                      visitor<S_midi_instrument>

{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                              mxsr2msrSkeletonPopulator (
                                const S_msrScore&     scoreSkeleton,
                                mxsrEventsCollection& theKnownEventsCollection);

    virtual                   ~mxsr2msrSkeletonPopulator ();

    // set and get
    // ------------------------------------------------------

    S_msrScore                getMsrScore () const
                                  { return fMsrScore; }

  public:

    // public services
    // ------------------------------------------------------

    void                      populateMsrSkeletonFromMxsr (
                                const Sxmlelement theMxsr);

    // visitors
    // ------------------------------------------------------

    void                      browseMxsr (
                                const Sxmlelement& theMxsr);

  protected:

    // scaling
    // ------------------------------------------------------

    virtual void              visitEnd   (S_scaling& elt);
    virtual void              visitStart (S_millimeters& elt);
    virtual void              visitStart (S_tenths& elt);

    // layout
    // ------------------------------------------------------

    virtual void              visitStart (S_page_layout& elt);
    virtual void              visitEnd   (S_page_layout& elt);

    virtual void              visitStart (S_page_height& elt);
    virtual void              visitStart (S_page_width& elt);

    virtual void              visitStart (S_page_margins& elt);
    virtual void              visitEnd   (S_page_margins& elt);

    virtual void              visitStart (S_left_margin& elt);
    virtual void              visitStart (S_right_margin& elt);
    virtual void              visitStart (S_top_margin& elt);
    virtual void              visitStart (S_bottom_margin& elt);

    virtual void              visitStart (S_system_layout& elt);
    virtual void              visitEnd   (S_system_layout& elt);

    virtual void              visitStart (S_system_margins& elt);
    virtual void              visitEnd   (S_system_margins& elt);
    virtual void              visitStart (S_system_distance& elt);
    virtual void              visitStart (S_top_system_distance& elt);

    virtual void              visitStart (S_staff_layout& elt);
    virtual void              visitEnd   (S_staff_layout& elt);
    virtual void              visitStart (S_staff_distance& elt);

    virtual void              visitStart (S_measure_layout& elt);
    virtual void              visitEnd   (S_measure_layout& elt);
    virtual void              visitStart (S_measure_distance& elt);

    // appearance
    // ------------------------------------------------------

    virtual void              visitStart (S_appearance& elt);
    virtual void              visitEnd   (S_appearance& elt);
    virtual void              visitStart (S_line_width& elt);
    virtual void              visitStart (S_note_size& elt);
    virtual void              visitStart (S_distance& elt);
    virtual void              visitStart (S_glyph& elt);
    virtual void              visitStart (S_other_appearance& elt);

    // system dividers
    // ------------------------------------------------------

    virtual void              visitStart (S_system_dividers& elt);
    virtual void              visitStart (S_left_divider& elt);
    virtual void              visitStart (S_right_divider& elt);

    // notations
    // ------------------------------------------------------

    virtual void              visitStart (S_notations& elt);
    virtual void              visitStart (S_other_notation& elt);

    // parts
    // ------------------------------------------------------

    virtual void              visitStart (S_part& elt);
    virtual void              visitEnd   (S_part& elt);

    // ???
    // ------------------------------------------------------

    virtual void              visitStart (S_attributes& elt);
    virtual void              visitEnd   (S_attributes& elt);

    virtual void              visitStart (S_divisions& elt);

    // staves
    // ------------------------------------------------------

    virtual void              visitStart (S_staves& elt);

    virtual void              visitStart (S_staff& elt);

    // staff details
    // ------------------------------------------------------

    virtual void              visitStart (S_staff_details& elt);
    virtual void              visitEnd   (S_staff_details& elt);
    virtual void              visitStart (S_staff_type& elt);
    virtual void              visitStart (S_staff_lines& elt);
    virtual void              visitStart (S_staff_tuning& elt);
    virtual void              visitEnd   (S_staff_tuning& elt);
    virtual void              visitStart (S_tuning_step& elt);
    virtual void              visitStart (S_tuning_octave& elt);
    virtual void              visitStart (S_tuning_alter& elt);
    virtual void              visitStart (S_capo& elt);
    virtual void              visitStart (S_staff_size& elt);

    // voices
    // ------------------------------------------------------

    virtual void              visitStart (S_voice& elt);

    // notes
    // ------------------------------------------------------

    virtual void              visitStart (S_note& elt);
    virtual void              visitEnd   (S_note& elt);

    virtual void              visitStart (S_rest& elt);

    virtual void              visitStart (S_step& elt);
    virtual void              visitStart (S_alter& elt);
    virtual void              visitStart (S_octave& elt);
    virtual void              visitStart (S_duration& elt);
    virtual void              visitStart (S_dot& elt);

    virtual void              visitStart (S_stem& elt);

    virtual void              visitEnd   (S_unpitched& elt);
    virtual void              visitStart (S_display_step& elt);
    virtual void              visitStart (S_display_octave& elt);

    virtual void              visitStart (S_accidental& elt);

    // backup & forward
    // ------------------------------------------------------

    virtual void              visitStart (S_backup& elt);
    virtual void              visitEnd   (S_backup& elt);
    virtual void              visitStart (S_forward& elt);
    virtual void              visitEnd   (S_forward& elt);

    // clefs
    // ------------------------------------------------------

    virtual void              visitStart (S_clef& elt);
    virtual void              visitEnd   (S_clef& elt);
    virtual void              visitStart (S_sign& elt);
    virtual void              visitStart (S_line& elt);
    virtual void              visitStart (S_clef_octave_change& elt);

    // keys
    // ------------------------------------------------------

    virtual void              visitStart (S_key& elt);
    virtual void              visitEnd   (S_key& elt);
    virtual void              visitStart (S_cancel& elt);
    virtual void              visitStart (S_fifths& elt);
    virtual void              visitStart (S_mode& elt);
    virtual void              visitStart (S_key_step& elt);
    virtual void              visitStart (S_key_alter& elt);
    virtual void              visitStart (S_key_octave& elt);

    // time signatures
    // ------------------------------------------------------

    virtual void              visitStart (S_time& elt);
    virtual void              visitEnd   (S_time& elt);
    virtual void              visitStart (S_beats& elt);
    virtual void              visitStart (S_beat_type& elt);
    virtual void              visitStart (S_senza_misura& elt);

    virtual void              visitStart (S_interchangeable& elt);
    virtual void              visitStart (S_time_relation& elt);

    // instruments
    // ------------------------------------------------------

    virtual void              visitStart (S_score_instrument& elt);
    virtual void              visitStart (S_instrument_name& elt);
    virtual void              visitStart (S_solo& elt);
    virtual void              visitStart (S_instrument& elt);
    virtual void              visitStart (S_instruments& elt);

    // transposition
    // ------------------------------------------------------

    virtual void              visitStart (S_transpose& elt);
    virtual void              visitEnd   (S_transpose& elt);
    virtual void              visitStart (S_diatonic& elt);
    virtual void              visitStart (S_chromatic& elt);
    virtual void              visitStart (S_octave_change& elt);
    virtual void              visitStart (S_double& elt);

    // directions
    // ------------------------------------------------------

    virtual void              visitStart (S_direction& elt);
    virtual void              visitEnd   (S_direction& elt);
    virtual void              visitStart (S_direction_type& elt);
    virtual void              visitEnd   (S_direction_type& elt);
    virtual void              visitStart (S_offset& elt);

    virtual void              visitStart (S_words& elt);

    virtual void              visitStart (S_octave_shift& elt);

    virtual void              visitStart (S_scordatura& elt);
    virtual void              visitEnd   (S_scordatura& elt);
    virtual void              visitStart (S_accord& elt);
    virtual void              visitEnd   (S_accord& elt);

    virtual void              visitStart (S_other_direction& elt);

    // accordion registration
    // ------------------------------------------------------

    virtual void              visitStart (S_accordion_registration& elt);
    virtual void              visitEnd   (S_accordion_registration& elt);
    virtual void              visitStart (S_accordion_high& elt);
    virtual void              visitStart (S_accordion_middle& elt);
    virtual void              visitStart (S_accordion_low& elt);

    // tempo
    // ------------------------------------------------------

    virtual void              visitStart (S_metronome& elt);
    virtual void              visitEnd   (S_metronome& elt);
    virtual void              visitStart (S_beat_unit& elt);
    virtual void              visitStart (S_beat_unit_dot& elt);
    virtual void              visitStart (S_per_minute& elt);
    virtual void              visitStart (S_metronome_note& elt);
    virtual void              visitEnd   (S_metronome_note& elt);
    virtual void              visitStart (S_metronome_relation& elt);
    virtual void              visitStart (S_metronome_type& elt);
    virtual void              visitStart (S_metronome_beam& elt);
    virtual void              visitStart (S_metronome_dot& elt);
    virtual void              visitStart (S_metronome_tuplet& elt);
    virtual void              visitEnd   (S_metronome_tuplet& elt);
    virtual void              visitStart (S_normal_dot& elt);

    // ties, slurs, brackets & beams
    // ------------------------------------------------------

    virtual void              visitStart (S_tied& elt);
    virtual void              visitStart (S_slur& elt);
    virtual void              visitStart (S_bracket& elt);
    virtual void              visitStart (S_beam& elt);

    // lyrics
    // ------------------------------------------------------

    virtual void              visitStart (S_lyric& elt);
    virtual void              visitEnd   (S_lyric& elt);
    virtual void              visitStart (S_syllabic& elt);
    virtual void              visitStart (S_text& elt);
    virtual void              visitStart (S_elision& elt);
    virtual void              visitStart (S_extend& elt);

    // degrees
    // ------------------------------------------------------

    virtual void              visitStart (S_degree& elt);
    virtual void              visitEnd   (S_degree& elt);
    virtual void              visitStart (S_degree_value& elt);
    virtual void              visitStart (S_degree_alter& elt);
    virtual void              visitStart (S_degree_type& elt);

    // harmonies
    // ------------------------------------------------------

    virtual void              visitStart (S_harmony& elt);
    virtual void              visitEnd   (S_harmony& elt);
    virtual void              visitStart (S_root& elt);
    virtual void              visitStart (S_root_step& elt);
    virtual void              visitStart (S_root_alter& elt);
    virtual void              visitStart (S_function& elt);
    virtual void              visitStart (S_kind& elt);

    virtual void              visitStart (S_inversion& elt);

    virtual void              visitStart (S_bass& elt);
    virtual void              visitStart (S_bass_step& elt);
    virtual void              visitStart (S_bass_alter& elt);

    // frames
    // ------------------------------------------------------

    virtual void              visitStart (S_frame& elt);
    virtual void              visitEnd   (S_frame& elt);
    virtual void              visitStart (S_frame_strings& elt);
    virtual void              visitStart (S_frame_frets& elt);
    virtual void              visitStart (S_first_fret& elt);
    virtual void              visitStart (S_frame_note& elt);
    virtual void              visitEnd   (S_frame_note& elt);
    virtual void              visitStart (S_barre& elt);

    // figured bass
    // ------------------------------------------------------

    virtual void              visitStart (S_figured_bass& elt);
    virtual void              visitEnd   (S_figured_bass& elt);
    virtual void              visitStart (S_figure& elt);
    virtual void              visitEnd   (S_figure& elt);
    virtual void              visitStart (S_prefix& elt);
    virtual void              visitStart (S_figure_number& elt);
    virtual void              visitStart (S_suffix& elt);

    // measures
    // ------------------------------------------------------

    virtual void              visitStart (S_measure& elt);
    virtual void              visitEnd   (S_measure& elt);

    // print
    // ------------------------------------------------------

    virtual void              visitStart (S_print& elt);
    virtual void              visitEnd   (S_print& elt);

    // measure numbering
    // ------------------------------------------------------

    virtual void              visitStart (S_measure_numbering& elt);

    // barlines
    // ------------------------------------------------------

    virtual void              visitStart (S_barline& elt);
    virtual void              visitStart (S_segno& elt);
    virtual void              visitStart (S_coda& elt);
    virtual void              visitStart (S_eyeglasses& elt);
    virtual void              visitStart (S_pedal& elt);
    virtual void              visitStart (S_bar_style& elt);
    virtual void              visitStart (S_repeat& elt);
    virtual void              visitStart (S_ending& elt);
    virtual void              visitEnd   (S_barline& elt);

    // repeats
    // ------------------------------------------------------

    virtual void              visitStart (S_measure_style& elt);
    virtual void              visitStart (S_beat_repeat& elt);
    virtual void              visitStart (S_measure_repeat& elt);
    virtual void              visitStart (S_multiple_rest& elt);
    virtual void              visitEnd   (S_multiple_rest& elt);
    virtual void              visitStart (S_slash& elt);
    virtual void              visitEnd   (S_slash& elt);
    virtual void              visitStart (S_slash_type& elt);
    virtual void              visitStart (S_slash_dot& elt);

    // atticulations
    // ------------------------------------------------------

    virtual void              visitStart (S_articulations& elt);
    virtual void              visitEnd   (S_articulations& elt);
    virtual void              visitStart (S_accent& elt);
    virtual void              visitStart (S_breath_mark& elt);
    virtual void              visitStart (S_caesura& elt);
    virtual void              visitStart (S_spiccato& elt);
    virtual void              visitStart (S_staccato& elt);
    virtual void              visitStart (S_staccatissimo& elt);
    virtual void              visitStart (S_stress& elt);
    virtual void              visitStart (S_unstress& elt);
    virtual void              visitStart (S_detached_legato& elt);
    virtual void              visitStart (S_strong_accent& elt);
    virtual void              visitStart (S_tenuto& elt);
    virtual void              visitStart (S_fermata& elt);
    virtual void              visitStart (S_arpeggiate& elt);
    virtual void              visitStart (S_non_arpeggiate& elt);
    virtual void              visitStart (S_doit& elt);
    virtual void              visitStart (S_falloff& elt);
    virtual void              visitStart (S_plop& elt);
    virtual void              visitStart (S_scoop& elt);

    // technicals
    // ------------------------------------------------------

    virtual void              visitStart (S_technical& elt);
    virtual void              visitEnd   (S_technical& elt);
    virtual void              visitStart (S_arrow& elt);
    virtual void              visitStart (S_bend& elt);
    virtual void              visitEnd   (S_bend& elt);
    virtual void              visitStart (S_bend_alter& elt);
    virtual void              visitStart (S_double_tongue& elt);
    virtual void              visitStart (S_down_bow& elt);
    virtual void              visitStart (S_fingering& elt);
    virtual void              visitStart (S_fingernails& elt);
    virtual void              visitStart (S_fret& elt);
    virtual void              visitStart (S_hammer_on& elt);
    virtual void              visitStart (S_handbell& elt);
    virtual void              visitStart (S_harmonic& elt);
    virtual void              visitStart (S_heel& elt);
    virtual void              visitStart (S_hole& elt);
    virtual void              visitStart (S_open_string& elt);
    virtual void              visitStart (S_other_technical& elt);
    virtual void              visitStart (S_pluck& elt);
    virtual void              visitStart (S_pull_off& elt);
    virtual void              visitStart (S_snap_pizzicato& elt);
    virtual void              visitStart (S_stopped& elt);
    virtual void              visitStart (S_string& elt);
    virtual void              visitStart (S_tap& elt);
    virtual void              visitStart (S_thumb_position& elt);
    virtual void              visitStart (S_toe& elt);
    virtual void              visitStart (S_triple_tongue& elt);
    virtual void              visitStart (S_up_bow& elt);

    // ornaments
    // ------------------------------------------------------

    virtual void              visitStart (S_ornaments& elt);
    virtual void              visitEnd   (S_ornaments& elt);
    virtual void              visitStart (S_trill_mark& elt);
    virtual void              visitStart (S_dashes& elt);
    virtual void              visitStart (S_wavy_line& elt);
    virtual void              visitStart (S_turn& elt);
    virtual void              visitStart (S_inverted_turn& elt);
    virtual void              visitStart (S_delayed_turn& elt);
    virtual void              visitStart (S_delayed_inverted_turn& elt);
    virtual void              visitStart (S_vertical_turn& elt);
    virtual void              visitStart (S_mordent& elt);
    virtual void              visitStart (S_inverted_mordent& elt);
    virtual void              visitStart (S_schleifer& elt);
    virtual void              visitStart (S_shake& elt);
    virtual void              visitStart (S_accidental_mark& elt);

    // tremolos
    // ------------------------------------------------------

    virtual void              visitStart (S_tremolo& elt);

    // dynamics
    // ------------------------------------------------------

    virtual void              visitStart (S_f& elt);
    virtual void              visitStart (S_ff& elt);
    virtual void              visitStart (S_fff& elt);
    virtual void              visitStart (S_ffff& elt);
    virtual void              visitStart (S_fffff& elt);
    virtual void              visitStart (S_ffffff& elt);

    virtual void              visitStart (S_p& elt);
    virtual void              visitStart (S_pp& elt);
    virtual void              visitStart (S_ppp& elt);
    virtual void              visitStart (S_pppp& elt);
    virtual void              visitStart (S_ppppp& elt);
    virtual void              visitStart (S_pppppp& elt);

    virtual void              visitStart (S_mf& elt);
    virtual void              visitStart (S_mp& elt);
    virtual void              visitStart (S_fp& elt);
    virtual void              visitStart (S_fz& elt);
    virtual void              visitStart (S_pf& elt);

    virtual void              visitStart (S_rf& elt);
    virtual void              visitStart (S_sf& elt);
    virtual void              visitStart (S_rfz& elt);
    virtual void              visitStart (S_sfz& elt);

    virtual void              visitStart (S_sfp& elt);
    virtual void              visitStart (S_sfpp& elt);
    virtual void              visitStart (S_sffz& elt);
    virtual void              visitStart (S_sfzp& elt);

    virtual void              visitStart (S_n& elt);

    virtual void              visitStart (S_other_dynamics& elt);

    virtual void              visitStart (S_wedge& elt);

    // harp pedals tuning
    // ------------------------------------------------------

    virtual void              visitStart (S_harp_pedals& elt);
    virtual void              visitStart (S_pedal_tuning& elt);
    virtual void              visitEnd   (S_pedal_tuning& elt);
    virtual void              visitStart (S_pedal_step& elt);
    virtual void              visitStart (S_pedal_alter& elt);

    virtual void              visitStart (S_damp& elt);
    virtual void              visitStart (S_damp_all& elt);

    // cue notes
    // ------------------------------------------------------

    virtual void              visitStart (S_cue& elt);

    // grace notes
    // ------------------------------------------------------

    virtual void              visitStart (S_grace& elt);

    // type
    // ------------------------------------------------------

    virtual void              visitStart (S_type& elt);

    // note heads
    // ------------------------------------------------------

    virtual void              visitStart (S_notehead& elt);


    // chords
    // ------------------------------------------------------

    virtual void              visitStart (S_chord& elt);

    // time modifications in notes
    // ------------------------------------------------------

    virtual void              visitStart (S_time_modification& elt);
    virtual void              visitStart (S_actual_notes& elt);
    virtual void              visitStart (S_normal_notes& elt);
    virtual void              visitStart (S_normal_type& elt);

    // tuplets
    // ------------------------------------------------------

    virtual void              visitStart (S_tuplet& elt);
    virtual void              visitEnd   (S_tuplet& elt);

    virtual void              visitStart (S_tuplet_actual& elt);
    virtual void              visitEnd   (S_tuplet_actual& elt);

    virtual void              visitStart (S_tuplet_normal& elt);
    virtual void              visitEnd   (S_tuplet_normal& elt);

    virtual void              visitStart (S_tuplet_number& elt);
    virtual void              visitStart (S_tuplet_type& elt);
    virtual void              visitStart (S_tuplet_dot& elt);

    // glissandos
    // ------------------------------------------------------

    virtual void              visitStart (S_glissando& elt);

    // slides
    // ------------------------------------------------------

    virtual void              visitStart (S_slide& elt);

    // rehearsals
    // ------------------------------------------------------

    virtual void              visitStart (S_rehearsal& elt);

    // sound & MIDI
    // ------------------------------------------------------

    virtual void              visitStart (S_sound& elt);
    virtual void              visitEnd   (S_sound& elt);

    virtual void              visitStart (S_instrument_sound& elt);
    virtual void              visitStart (S_virtual_instrument& elt);
    virtual void              visitStart (S_midi_device& elt);
    virtual void              visitStart (S_midi_instrument& elt);

  private:

    // the MSR score we are populating
    // ------------------------------------------------------

    S_msrScore                fMsrScore;


    // the score notes events we know from  mxsr2msrSkeletonBuilder
    // ------------------------------------------------------

    mxsrEventsCollection&     fKnownEventsCollection;
//     int                       fEventSequentialNumber;


    // part handling
    // ------------------------------------------------------

    S_msrPart                 fCurrentPart;


    // divisions per quarter note
    // ------------------------------------------------------

    int                       fCurrentDivisionsPerQuarterNote;
    S_msrDivisions            fCurrentDivisions;


    // scaling handling
    // ------------------------------------------------------

    float                     fCurrentMillimeters;
    float                     fCurrentTenths;


    // page layout handling
    // ------------------------------------------------------

    Bool                      fOnGoingPageMargins;

    msrMarginTypeKind         fCurrentPageMarginsTypeKind;

    S_msrPageLayout           fCurrentPageLayout;

    Bool                      fOnGoingPageLayout;
    S_msrMarginsGroup         fCurrentPageLayoutMarginsGroup;


    // system layout handling
    // ------------------------------------------------------

    Bool                      fOnGoingSystemLayout;

    S_msrSystemLayout         fCurrentSystemLayout;

    Bool                      fOnGoingSystemMargins;


    // staff layout handling
    // ------------------------------------------------------

    Bool                      fOnGoingStaffLayout;

    S_msrStaffLayout          fCurrentStaffLayout;


    // appearance handling
    // ------------------------------------------------------

    Bool                      fOnGoingAppearance;

    S_msrAppearance           fCurrentAppearance;


    // measure layout handling
    // ------------------------------------------------------

    Bool                      fOnGoingMeasureLayout;

    S_msrMeasureLayout        fCurrentMeasureLayout;


    // print
    // ------------------------------------------------------

    msrPrintObjectKind        fCurrentPrintObjectKind;

    std::string               fCurrentDisplayText;

    S_msrMusicXMLPrintLayout  fCurrentMusicXMLPrintLayout;

    Bool                      fOnGoingPrint;


    // clef, key and time signature handling
    // ------------------------------------------------------

//     S_msrClefKeyTimeSignatureGroup
//                               fCurrentClefKeyTimeSignatureGroup;
    int                       fCurrentAttributesInputLineNumber;

//     Bool                      fOnGoingClefKeyTimeSignatureGroup;

    // clef handling
    // ------------------------------------------------------

    int                       fCurrentClefStaffNumber;
    std::string               fCurrentClefSign;
    int                       fCurrentClefLine;
    int                       fCurrentClefOctaveChange;


    // key handling
    // ------------------------------------------------------

    msrKeyKind                fCurrentKeyKind;

    // traditional
    int                       fCurrentKeyStaffNumber;
    int                       fCurrentKeyFifths;
    int                       fCurrentKeyCancelFifths;
    msrModeKind               fCurrentModeKind;

    S_msrKey                  handleTraditionalKey (
                                const mfInputLineNumber& inputLineNumber);

    // Humdrum-Scot
    S_msrHumdrumScotKeyItem   fCurrentHumdrumScotKeyItem;
    std::vector <S_msrHumdrumScotKeyItem>
                              fCurrentHumdrumScotKeyItemsVector;

    S_msrKey                  handleHumdrumScotKey (
                                const mfInputLineNumber& inputLineNumber);


    // time signatures handling
    // ------------------------------------------------------

    msrTimeSignatureSymbolKind
                              fCurrentTimeSignatureSymbolKind;

    int                       fCurrentTimeStaffNumber;
    std::string               fCurrentTimeSignatureBeats;
    Bool                      fCurrentTimeSenzaMisura;

    std::vector <S_msrTimeSignatureItem>
                              fCurrentTimeSignatureItemsVector;

    S_msrTimeSignature        fCurrentTimeSignature;

    msrTimeSignatureSymbolKind
                              fCurrentInterchangeableSymbolKind;
    msrTimeSignatureSeparatorKind
                              fCurrentInterchangeableSeparatorKind;
    msrTimeSignatureRelationKind
                              fCurrentInterchangeableRelationKind;

    Bool                      fOnGoingInterchangeable;


    // part handling
    // ------------------------------------------------------

    std::map <std::string, S_msrPart>
                              fPartsMap;

    void                      populatePartsMapFromScore ();

		void											displayPartsMap () const;

    void                      handlePartMusicXMLID (
                                const mfInputLineNumber& inputLineNumber,
                                const std::string idString);

    // staff handling
    // ------------------------------------------------------

    // we use a pointer to the current part staves map
    std::map <mfStaffNumber, S_msrStaff>*
                              fCurrentPartStavesMapPtr;

    std::map <std::pair <int, int>, S_msrVoice>
                              fVoicesLastMetNoteMap;

//     void                      populateCurrentPartStavesMapFromPart (
//                                   const S_msrPart& part);

		void											displayCurrentPartStavesMap () const;


    // voices handling
    // ------------------------------------------------------

    S_msrMeasureElement       fPreviousMeasureElement;

    S_msrVoice                fetchFirstVoiceFromCurrentPart (
                                const mfInputLineNumber& inputLineNumber);

		void											displayStaffAndVoiceInformation (
                                const mfInputLineNumber& inputLineNumber,
																const std::string& context) const;

    /*
      the order in which a tuplet'members are present in a MusicXML file
      is up to the exporter that created it,
      and there may backups to other voices before all of them have been seen

      we therefore need to store tuplets internally until they are complete.

      this is done with a vector, whose indices are the voices numbers
    */

    // we need a fast access to the staves and voices
    // indexes are staff number and voice number
    std::map <mfStaffNumber, std::map <mfVoiceNumber, S_msrVoice>>
                              fCurrentPartStaffMsrVoicesMap;

		void											displayCurrentPartStaffMsrVoicesMap (
                                const mfInputLineNumber& inputLineNumber,
																const std::string& context) const;

    // we need a fast access to the voices and their handlers
    // indexes are staff number and voice number
    std::map <mfStaffNumber, std::map <mfVoiceNumber, S_mxsrVoice>>
                              fCurrentPartStaffMxsrVoicesMapMap;

    // denormalization for speed
    S_mxsrVoice               fCurrentNoteMxsrVoice;
    S_mxsrVoice               fCurrentRecipientMxsrVoice;

    S_msrVoice                fCurrentNoteMsrVoice;
    S_msrVoice                fCurrentRecipientMsrVoice;

    void                      populateCurrentPartStaffVoicesMapsFromPart (
                                  const S_msrPart& part);

		void											displayCurrentPartStaffMxsrVoicesMap () const;

    // staff details handling
    // ------------------------------------------------------

    msrStaffTypeKind          fCurrentStaffTypeKind;
    msrShowFretsKind          fCurrentShowFretsKind;
    msrPrintSpacingKind       fCurrentPrintSpacingKind;

    int                       fCurrentStaffDetailsCapo;

    int                       fCurrentStaffDetailsStaffSize; // JMI

    int                       fCurrentStaffLinesNumber;

    int                       fStaffDetailsStaffNumber;

    int                       fCurrentStaffTuningLine;
    msrDiatonicPitchKind      fCurrentStaffTuningDiatonicPitchKind;
    msrAlterationKind         fCurrentStaffTuningAlterationKind;
    msrOctaveKind             fCurrentStaffTuningOctaveKind;

    S_msrStaffDetails         fCurrentStaffDetails;

    Bool                      fOnGoingStaffTuning;


    // measures handling
    // ------------------------------------------------------

    // we need to count the measures for option fSeparatorLineEveryNMeasures,
    // since measure numbers are actually strings
    int                       fPartMeasuresCounter;

    mfMeasureNumber           fCurrentMeasureNumber;

    // measure end line number
    int                       fPreviousMeasureInputLineNumber;


    // multiple measure rests handling
    // ------------------------------------------------------

    int                       fCurrentMultipleMeasureRestMeasuresNumber;
    int                       fRemainingMultipleMeasureRestMeasuresNumber;

    int                       fCurrentMultipleMeasureRestSlashesNumber;

    msrUseSymbolsKind         fCurrentUseSymbolsKind;

    Bool                      fCurrentRestIsAMeasureRest;

//     void                      handleOnGoingMultipleMeasureRestsAtTheEndOfMeasure (
//                                 const mfInputLineNumber& inputLineNumber);

   S_mxsrMultipleMeasureRestEvent
                              fCurrentMultipleMeasureRestBegin;
    S_mxsrMultipleMeasureRestEvent
                              fCurrentMultipleMeasureRestEnd;

    S_msrMultipleMeasureRest  fCurrentMultipleMeasureRest;
    Bool                      fOnGoingMultipleMeasureRest;

    Bool                      fCurrentMeasureBelongsToAMultipleMeasureRest;

    void                      handleMultipleMeasureRestBeginEventIfAny ();

    void                      handleMultipleMeasureRestEndEventIfAny ();

    void                      handleMultipleMeasureRestBegin ();

    void                      handleMultipleMeasureRestEnd ();

		void											displayGatheredMultipleMeasureRestInformations (
																const std::string& context) const;

    void                      printCurrentMultipleMeasureRest ();


    // measure repeats handling
    // ------------------------------------------------------

    msrMeasureRepeatKind      fCurrentMeasureRepeatKind;
    int                       fCurrentMeasureRepeatMeasuresNumber;
    int                       fCurrentMeasureRepeatSlashesNumber;

    S_mxsrMeasureRepeatEvent  fCurrentMeasureRepeatBegin;
    S_mxsrMeasureRepeatEvent  fCurrentMeasureRepeatEnd;

    S_msrMeasureRepeat        fCurrentMeasureRepeat;

    Bool                      fCurrentNoteBelongsToAMeasureRepeat;



//     S_msrMeasureRepeat        fCurrentNoteMeasureRepeatBegin;
//     S_msrMeasureRepeat        fCurrentNoteMeasureRepeatEnd;
    Bool                      fOnGoingMeasureRepeat;

    void                      handleMeasureRepeatBeginEventIfAny ();

    void                      handleMeasureRepeatEndEventIfAny ();

    void                      handleMeasureRepeatBegin ();

    void                      handleMeasureRepeatEnd ();

//     void                      finalizeCurrentMeasureRepeat (
//                                 const mfInputLineNumber& inputLineNumber);

		void											displayGatheredMeasureRepeatInformations (
																const std::string& context) const;

    void                      printCurrentMeasureRepeats ();


    // measure style handling
    // ------------------------------------------------------

    msrSlashTypeKind          fCurrentSlashTypeKind;
    msrUseDotsKind            fCurrentUseDotsKind;
    msrSlashUseStemsKind      fCurrentSlashUseStemsKind;

    mfDurationKind      fCurrentSlashGraphicNotesDurationKind;
    int                       fCurrentSlashDotsNumber;


    // beats repeats
    // ------------------------------------------------------

    int                       fCurrentBeatRepeatSlashes;


    // notes handling
    // ------------------------------------------------------

    int                       fCurrentNoteMusicXMLStaffNumber;
    int                       fPreviousNoteMusicXMLStaffNumber;

    int                       fCurrentNoteMusicXMLVoiceNumber; // used throughout

    S_msrNote                 fCurrentNote;

    // sounding notes
    // ------------------------------------------------------

//     Bool                      fCurrentNoteIsSounding;

    // rests
    // ------------------------------------------------------

    Bool                      fCurrentNoteIsARest;

    // staff changes handling
    // ------------------------------------------------------

    // MusicXMl contains sequences of elements on one and the same staff,
    // until a <backup/> or <forward/> markup may change the latter
    // or some note in not in the same staff

    // such a staff change occurs when the staff number changes
    // whilst the voice number remains the same
    // hence the use of fCurrentRecipientStaffNumber,
    // which doesnt change in case of a staff change

    // fCurrentRecipientStaffNumber is that of the staff that contains the note,
    // which may be different that tne current note's staff number
    // in case of staff change
    int                       fCurrentRecipientStaffNumber;

    // fCurrentDisplayStaffNumber is that where the notes are displayed
    // for the duration of the staff change
    int                       fCurrentDisplayStaffNumber;

    // fCurrentNoteStaffChangeTakeOff contains the staff change event
    // that occurs on a take off note, if any
    S_mxsrStaffChangeEvent    fCurrentNoteStaffChangeTakeOff; // EVENTS

    void                      handleStaffChangeTakeOffEventIfAny ();

    void                      createStaffChange (
                                const mfInputLineNumber& inputLineNumber,
                                S_mxsrStaffChangeEvent staffChangeTakeOffEvent);

    // grace notes handling
    // ------------------------------------------------------

    Bool                      fCurrentNoteIsAGraceNote;

    std::string               fCurrentStealTimeFollowing;
    std::string               fCurrentStealTimePrevious;
    std::string               fCurrentMakeTimeSignature;


    // grace notes groups handling
    // ------------------------------------------------------

    S_mxsrGraceEvent          fCurrentNoteGraceBeginEvent;
    S_mxsrGraceEvent          fCurrentNoteGraceEndEvent;

    Bool                      fCurrentGraceNotesGroupIsSlashed;
    Bool                      fCurrentGraceNotesGroupIsBeamed;

    Bool                      fCurrentGraceNotesGroupIsTied;
    Bool                      fCurrentGraceNotesGroupIsSlurred;

    S_msrGraceNotesGroup      fPendingGraceNotesGroup;

    void                      handleGraceBeginEventIfAny ();

    void                      handleGraceEndEventIfAny ();


    // chords handling
    // ------------------------------------------------------

    S_mxsrChordEvent          fCurrentNoteChordBegin;
    S_mxsrChordEvent          fCurrentNoteChordEnd;

    Bool                      fCurrentNoteBelongsToAChord;

    Bool                      fOnGoingChord;
    S_msrChord                fCurrentChord;

    Bool                      fCurrentChordHasBeenPopulatedFromItsFirstNote;

    void                      handleChordBeginEventIfAny ();

    void                      handleChordEndEventIfAny ();

    void                      handleChordBegin ();

    void                      handleChordEnd ();

//     void                      finalizeCurrentChord (
//                                 const mfInputLineNumber& inputLineNumber);

    void                      printCurrentChord ();


    // tuplets handling
    // ------------------------------------------------------

    S_mxsrTupletEvent         fCurrentNoteTupletEvent; // EVENTS

    Bool                      fCurrentNoteHasATimeModification;

    int                       fCurrentNoteActualNotes;
    int                       fCurrentNoteNormalNotes;
    mfDurationKind            fCurrentNoteNormalTypeNotesDuration;

    // nested tuplets are numbered 1, 2, ...
    mfTupletNumber            fPreviousTupletNumber;

    Bool                      fOnGoingTupletActual;
    int                       fCurrentTupletActualNumber;
    std::string               fCurrentTupletActualType;
    int                       fCurrentTupletActualDotsNumber;

    Bool                      fOnGoingTupletNormal;
    int                       fCurrentTupletNormalNumber;
    std::string               fCurrentTupletNormalType;
    int                       fCurrentTupletNormalDotsNumber;

    msrTupletTypeKind         fCurrentTupletTypeKind;
    mfTupletNumber            fCurrentTempoTupletNumber;
    msrTupletBracketKind      fCurrentTupletBracketKind;
    msrTupletShowNumberKind   fCurrentTupletShowNumberKind;
    msrTupletShowTypeKind     fCurrentTupletShowTypeKind;
    msrTupletLineShapeKind    fCurrentTupletLineShapeKind;
    msrPlacementKind          fCurrentTupletPlacementKind;


    Bool                      fCurrentNoteBelongsToATuplet;

    // there is no fOnGoingTuplet, this is indicated
    // by fCurrentTupletStackTop being null
//     S_msrTuplet               fCurrentTupletStackTop;

    /*
      the measure position of a harmony is that of the next note
      after it in the MusicXML data,
      which can be the first note of a outermost tuplet

      we should memoize it because the latter note measure position
      will be known when the tuplet is appended to the current part,
      which occurs only after the whole tuplet contents has be analyzed

      the elements in a tuplet, including the nested ones,
      form a tree built along the way
      each note therein however has an offset relative to
      the first note of the outermost tuplet, computed linearly long the way
    */
    S_msrNote                 fCurrentOuterMostTupletFirstNote;
    S_msrTuplet               fCurrentOuterMostTuplet;

    mfWholeNotes              fCurrentOuterMostTupletRelativeOffset;

		void											displayGatheredTupletInformations (
																const std::string& context) const;

    S_msrTuplet               createATuplet (
                                const mfInputLineNumber& inputLineNumber,
                                const mfTupletNumber&  tupletNumber,
                                const msrTupletFactor&   tupletFactor);

    void                      handleTupletBeginEventsIfAny ();

    void                      handleTupletEndEventsIfAny ();

    void                      handleTupletBegin (
                                S_mxsrTupletEvent tupletBeginEvent);

    void                      handleTupletEnd (
                                S_mxsrTupletEvent tupletEndEvent);

    // cue notes
    // ------------------------------------------------------

    Bool                      fCurrentNoteIsACueNote;
    msrNoteIsACueNoteKind     fCurrentNoteIsACueNoteKind;

    // detailed notes handling
    // ------------------------------------------------------

		void											displayGatheredNoteInformations (
																const std::string& context) const;

    void                      handleCurrentNote (
                                const mfInputLineNumber& inputLineNumber);

    S_msrNote                 createNote (
                                const mfInputLineNumber& inputLineNumber);

    void                      handleARegularNoteInAMeasure (
																const S_msrNote& note);

    void                      handleARestInAMeasure (
																const S_msrNote& rest);

    void                      handleARegularNoteInAChord (
                                const S_msrNote& regularNote);

    void                      handleARegularNoteInATuplet (
																const S_msrNote& regularNote);

    void                      handleARestInATuplet (
																const S_msrNote& rest);

    void                      handleAGraceNoteAttachedToANote (
																const S_msrNote& graceNote);

    void                      handleARegularNoteInAChordInATuplet (
                                const S_msrNote& graceNote);

    void                      handleAGraceNoteInAChord (
                                const S_msrNote& graceNote);


    // harmonies and figured bass elements are pending
    // until the note itself is found in the MusicXML data
    // ------------------------------------------------------
//     void                      attachPendingHarmoniesToCurrentNote ( // JMI 0.9.67
//                                 const mfInputLineNumber& inputLineNumber);

//     void                      attachPendingFiguredBassesToCurrentNote ( // JMI 0.9.67
//                                 const mfInputLineNumber& inputLineNumber);

    // populate current note
    // ------------------------------------------------------

    void                      populateCurrentNoteWithCurrentInformations (
                                const mfInputLineNumber& inputLineNumber);

    // harmonies and figured bass elements need
    // the position of the note  in its measure to be known
    // when they are inserted in their own measure
    void                      populateCurrentNoteWithPendingInformations (
                                const mfInputLineNumber& inputLineNumber);


    // transpose handling
    // ------------------------------------------------------

    int                       fCurrentTransposeStaffNumber;
    int                       fCurrentTransposeDiatonic;
    int                       fCurrentTransposeChromatic;
    int                       fCurrentTransposeOctaveChange;
    Bool                      fCurrentTransposeDouble;


    // direction handling
    // ------------------------------------------------------

    int                       fCurrentDirectionStaffNumber;
    int                       fCurrentDirectionVoiceNumber;
    msrPlacementKind          fCurrentDirectionPlacementKind;

    Bool                      fOnGoingDirection;


    // direction-type handling
    // ------------------------------------------------------

    Bool                      fOnGoingDirectionType;

//     void                      attachPendingPartLevelElementsIfAnyToPart (
//                                 const S_msrPart& part);


    // rehearsal marks handling
    // ------------------------------------------------------

    // rehearsal marks remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    std::list <S_msrRehearsalMark>
                              fPendingRehearsalMarksList;

    void                      attachPendingRehearsalMarksToPart (
                                const S_msrPart& part);

//     void                  convertWordsToRehearsalMark (
//                             const mfInputLineNumber& inputLineNumber,
//                             const std::string& wordsValuee);


    // segnos handling
    // ------------------------------------------------------

    // segnos remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    std::list <S_msrSegno>    fPendingSegnosList;

    void                      attachPendingSegnosToCurrentNote ();

    void                      attachThePendingDalSegnosIfAny ();

//     void                      convertWordsToSegno (
//                                 const mfInputLineNumber& inputLineNumber,
//                                 const std::string& wordsValuee);


    // dal segnos handling
    // ------------------------------------------------------
    // dal segnos are not represented as such in MusicXML,
    // but they are in MSR, hence an option to convert words
    // with specific contents to dal segnos
    std::list <S_msrDalSegno> fPendingDalSegnosList;

    void                      attachPendingDalSegnosToCurrentNote ();
    void                      attachPendingDalSegnosToChord (
                                const S_msrChord& chord);

//     void                      convertWordsToDalSegno (
//                                 const mfInputLineNumber& inputLineNumber,
//                                 const std::string& wordsValuee);


    // codas handling
    // ------------------------------------------------------
    // codas remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    std::list <S_msrCoda>     fPendingCodasList;

    int                       fCodasCounter;

    void                      attachPendingCodasToCurrentNote ();

//     void                      convertWordsToDalSegnoAlFine (
//                                 const mfInputLineNumber& inputLineNumber,
//                                 const std::string& wordsValuee);
//
//     void                      convertWordsToDalSegnoAlCoda (
//                                 const mfInputLineNumber& inputLineNumber,
//                                 const std::string& wordsValuee);
//
//     void                      convertWordsToCoda (
//                                 const mfInputLineNumber& inputLineNumber,
//                                 const std::string& wordsValuee);


    // cresc/decresc handling
    // ------------------------------------------------------
    // cresc/decresc are not represented as such in MusicXML,
    // but they are in MSR, hence an option to convert words
    // with specific contents to dal segnos

    std::list <S_msrCrescDecresc>
                              fPendinCrescDecrescsList;

    void                      attachPendingCrescDecrescsToCurrentNote ();

//     void                      convertWordsToCresc (
//                                 const mfInputLineNumber& inputLineNumber,
//                                 const std::string& wordsValuee);

//     void                      convertWordsToDecresc (
//                                 const mfInputLineNumber& inputLineNumber,
//                                 const std::string& wordsValuee);


    // eyeglasses handling
    // ------------------------------------------------------

    // eyeglasses remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    std::list <S_msrEyeGlasses>
                              fPendingEyeGlassesList;

    void                      attachPendingEyeGlassesToCurrentNote ();


    // damp handling
    // ------------------------------------------------------

    // damps remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    std::list <S_msrDamp>     fPendingDampsList;

    void                      attachPendingDampsToCurrentNote ();


    // damp all handling
    // ------------------------------------------------------

    // damp alls remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    std::list <S_msrDampAll>  fPendingDampAllsList;

    void                      attachPendingDampAllsToCurrentNote ();


    // accordion-registration handling
    // ------------------------------------------------------

    int                       fCurrentAccordionHigh;
    int                       fCurrentAccordionMiddle;
    int                       fCurrentAccordionLow;
    int                       fCurrentAccordionNumbersCounter;


    // metronome handling
    // ------------------------------------------------------

    mfDurationKind      fCurrentMetronomeBeatUnitNotesDurationKind;

    std::vector <mfDottedNotesDuration>
                              fCurrentMetronomeBeatUnitsVector;
    std::string               fCurrentMetrenomePerMinute;
    msrTempoParenthesizedKind fCurrentMetronomeParenthesedKind;

    int                       fCurrentMetrenomeDotsNumber;
    msrTempoNotesRelationshipKind
                              fCurrentMetrenomeRelationKind;
    mfDurationKind      fCurrentMetronomeNotesDurationKind;
    std::string               fCurrentMetronomeBeamValue;

    Bool                      fOnGoingMetronomeNote;

    std::list <S_msrBeam>     fPendingMetronomeBeamsList;
    void                      attachCurrentMetronomeBeamsToMetronomeNote (
                                const S_msrTempoNote& tempoNote);

    msrTempoTupletTypeKind    fCurrentTempoTupletTypeKind;
    msrTempoTupletBracketKind fCurrentTempoTupletBracketKind;
    msrTempoTupletShowNumberKind
                              fCurrentTempoTupletShowNumberKind;

    int                       fCurrentMetrenomeNormalDotsNumber;
    int                       fCurrentMetronomeNoteActualNotes;
    int                       fCurrentMetronomeNoteNormalNotes;
    std::string               fCurrentMetronomeNoteNormalType; // JMI ???

    mfWholeNotes              fCurrentMetronomeNoteWholeNotesFromMetronomeType;

    mfTupletNumber            fCurrentMetronomeTupletNumber;
    S_msrTempoTuplet          fCurrentMetronomeTuplet;
    Bool                      fOnGoingMetronomeTuplet;

    S_msrTempoNotesRelationshipElements
                              fCurrentMetronomeRelationLeftElements;
    S_msrTempoNotesRelationshipElements
                              fCurrentMetronomeRelationRightElements;

    S_msrTempo                fCurrentMetronomeTempo;

    // tempos remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    std::list <S_msrTempo>    fPendingTemposList;

    void                      attachPendingTemposToVoice (
                                const S_msrVoice& voice);

    void                      attachPendingTemposToPart (
                                const S_msrPart& part);

//     void                      convertWordsToTempo (
//                                 const mfInputLineNumber& inputLineNumber,
//                                 const std::string& wordsValue);


    // line breaks handling
    // ------------------------------------------------------

    std::list <S_msrLineBreak>
                              fPendingLineBreaksList;

//     void                      attachLineBreaksToVoice ( //JMI UNUSED??? 0.9.68
//                                 const S_msrVoice& voice);

    void                      attachPendingLineBreaksToPart (
                                const S_msrPart& part);


    // page and line breaks handling
    // ------------------------------------------------------

    std::list <S_msrPageBreak>
                              fPendingPageBreaksList;

//     void                      attachPageBreaksToVoice ( //JMI UNUSED??? 0.9.68
//                                 const S_msrVoice& voice);

    void                      attachPendingPageBreaksToPart (
                                const S_msrPart& part);


    // octave shift handling
    // ------------------------------------------------------

    // octave shifts remain pending until the next note
    // (they precede the note in MusicXML and
    // may occur when no current voice exists)
    std::list <S_msrOctaveShift>
                              fPendingOctaveShiftsList;
    void                      attachPendingOctaveShiftsToCurrentNote ();


    // scordatura handling
    // ------------------------------------------------------

    S_msrScordatura           fCurrentScordatura;

    // scordaturas remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    std::list <S_msrScordatura>
                              fPendingScordaturasList;

    void                      attachPendingScordaturasToCurrentNote ();

    int                       fCurrentAccordNumber;

    msrDiatonicPitchKind      fCurrentAccordDiatonicPitchKind;
    msrAlterationKind         fCurrentAccordAlterationKind;
    msrOctaveKind             fCurrentAccordOctaveKind;

    Bool                      fOnGoingAccord;



    // lyrics handling
    // ------------------------------------------------------

    Bool                      fOnGoingLyric;

    std::string               fCurrentSyllabic;
    msrSyllableKind           fCurrentSyllableKind;
    msrSyllableKind           fFirstSyllableInSlurKind;
    msrSyllableKind           fFirstSyllableInLigatureKind;

    std::list <msrSyllableElement>
                              fCurrentSyllableElementsList;

    msrSyllableExtendKind     fCurrentSyllableExtendKind;
    std::string               fCurrentSyllableElision;

    mfStanzaNumber            fCurrentStanzaNumber;
    std::string               fCurrentStanzaName;

    std::list <S_msrSyllable> fCurrentNoteSyllablesList;

    Bool                      fLastHandledNoteInVoiceHasLyrics;

    void                      handleLyricsAfterCurrentNoteHasBeenHandled ();

		void											displayGatheredLyricInformations (
																const std::string& context) const;


    // harmonies handling
    // ------------------------------------------------------

    int                       fHarmoniesVoicesCounter;

    std::list <S_msrHarmony>  fPendingHarmoniesList;

    void                      handlePendingHarmonies ();

    void                      handlePendingSingleHarmony (
                                const S_msrHarmony& harmony);

    void                      handlePendingMultipleHarmonies ();

    void                      displayPendingHarmoniesList (
                                std::string context) const;

    int                       fCurrentHarmonyInputLineNumber;

    msrDiatonicPitchKind      fCurrentHarmonyRootDiatonicPitchKind;
    msrAlterationKind         fCurrentHarmonyRootAlterationKind;

    std::string               fCurrentHarmonyFunctionText;

    msrHarmonyKind            fCurrentHarmonyKind;
    std::string               fCurrentHarmonyKindText;

    int                       fCurrentHarmonyInversion;

    msrDiatonicPitchKind      fCurrentHarmonyBassDiatonicPitchKind;
    msrAlterationKind         fCurrentHarmonyBassAlterationKind;
    msrHarmonyDegreeTypeKind
                              fCurrentHarmonyDegreeTypeKind;

    std::list <S_msrHarmonyDegree>
                              fCurrentHarmonyDegreesList;


    int                       fCurrentHarmonyDegreeValue;
    msrAlterationKind         fCurrentHarmonyDegreeAlterationKind;

    msrQuarterTonesPitchKind  fCurrentHarmonyRootQuarterTonesPitchKind;
    msrQuarterTonesPitchKind  fCurrentHarmonyBassQuarterTonesPitchKind;

    int                       fCurrentHarmoniesStaffNumber;

    mfWholeNotes              fCurrentHarmonyWholeNotesOffset;

    Bool                      fOnGoingHarmony;


    // figured bass handling
    // ------------------------------------------------------

    int                       fFiguredBassVoicesCounter;

    std::list <S_msrFiguredBass>
                              fPendingFiguredBassesList;

    void                      handlePendingFiguredBasses ();

    void                      handlePendingSingleFiguredBass (
                                const S_msrFiguredBass& figuredBass);

    void                      handlePendingMultipleFiguredBasses ();

    void                      displayPendingFiguredBassesList (
                                std::string context) const;

    Bool                      fOnGoingFiguredBass;

    int                       fCurrentFiguredBassInputLineNumber;

    msrBassFigurePrefixKind   fCurrentFigurePrefixKind;
    int                       fCurrentFigureNumber;
    msrBassFigureSuffixKind   fCurrentFigureSuffixKind;

    mfWholeNotes              fCurrentFiguredBassSoundingWholeNotes;
    mfWholeNotes              fCurrentFiguredBassDisplayWholeNotes;

    msrFiguredBassParenthesesKind
                              fCurrentFiguredBassParenthesesKind;

    std::list <S_msrBassFigure>
                              fPendingFiguredBassFiguresList;


    // frames handling
    // ------------------------------------------------------

    int                       fCurrentFrameStrings;
    int                       fCurrentFrameFrets;
    int                       fCurrentFrameFirstFret;
    Bool                      fOnGoingFrame;

    S_msrFrame                fCurrentFrame;

    int                       fCurrentFrameNoteStringNumber;
    int                       fCurrentFrameNoteFretNumber;
    int                       fCurrentFrameNoteFingering;

    msrBarreTypeKind          fCurrentFrameNoteBarreTypeKind;

    Bool                      fOnGoingFrameNote;

    std::list <S_msrFrameNote>
                              fPendingFramesNotesList;


    // bar lines handling
    // ------------------------------------------------------

    Bool                      fOnGoingBarLine;
    int                       fRepeatEndCounter;

    msrBarLineHasSegnoKind    fCurrentBarLineHasSegnoKind;
    msrBarLineHasCodaKind     fCurrentBarLineHasCodaKind;
    std::string               fCurrentBarLineEndingNumber; // std::vector <std::string> ??? JMI
                                // may be "1, 2"

    msrBarLineLocationKind    fCurrentBarLineLocationKind;
    msrBarLineStyleKind       fCurrentBarLineStyleKind;
    msrBarLineEndingTypeKind  fCurrentBarLineEndingTypeKind;
    msrBarLineRepeatDirectionKind
                              fCurrentBarLineRepeatDirectionKind;
    msrBarLineRepeatWingedKind
                              fCurrentBarLineRepeatWingedKind;

    int                       fCurrentBarLineTimes;

    // bar lines remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    std::list <S_msrBarLine>  fPendingBarLinesList;

//     void                      attachPendingBarLinesToVoice ( // JMI UNUSED??? 0.9.68
//                                 const S_msrVoice& voice);

    void                      attachPendingBarLinesToPart (
                                const S_msrPart& part);


    // repeats handling
    // ------------------------------------------------------

    mfMeasureNumber           fCurrentRepeatStartMeasureNumber;
    S_msrBarLine              fCurrentRepeatEndingStartBarLine;

    void                      handleImplicitInitialForwardRepeat (
                                const mfInputLineNumber& inputLineNumber);

    void                      handleRepeatStart (
                                const S_msrBarLine& barLine);

    void                      handleRepeatEnd (
                                const S_msrBarLine& barLine);

    void                      handleRepeatEndingStart (
                                const S_msrBarLine& barLine);

/* JMI 0.9.66
    void                      handleRepeatHookedEndingStart (
                                S_barline     elt,
                                const S_msrBarLine& barLine);
*/
    void                      handleRepeatHookedEndingEnd (
                                const S_msrBarLine& barLine);
/* JMI 0.9.66
    void                      handleRepeatHooklessEndingStart (
                                S_barline     elt,
                                const S_msrBarLine& barLine);
*/
    void                      handleRepeatHooklessEndingEnd (
                                const S_msrBarLine& barLine);


    // notes/rests handling
    // ------------------------------------------------------

    int                       fCurrentNoteSequentialNumber; // EVENTS

    int                       fCurrentNoteInputStartLineNumber;

    // indexes are staff number and voice number

    std::map <int, std::map <int, S_msrNote>>
                              fStaffVoicesLastMetNoteMap;

    void                      printVoicesLastMetNoteMap (
                                const mfInputLineNumber& inputLineNumber,
                                const std::string& context);

    void                      checkStep (
                                const mfInputLineNumber& inputLineNumber,
                                const std::string& stepValue,
                                const std::string& markup);

    // note pitch

    Bool                      fCurrentNoteIsUnpitched;

    msrDiatonicPitchKind      fCurrentNoteDiatonicPitchKind;

    msrQuarterTonesPitchKind  fCurrentNoteQuarterTonesPitchKind;

    msrAlterationKind         fCurrentNoteAlterationKind;

    msrDiatonicPitchKind      fCurrentDisplayDiatonicPitchKind;

    // note octave

    msrOctaveKind             fCurrentNoteOctave;
    msrOctaveKind             fCurrentDisplayOctave;

    // MusicXML note duration
    // not mandatory, has to be computed from note type if missing
    int                       fCurrentNoteDuration;

    // MusicXML note augmentation dots

    int                       fCurrentNoteDotsNumber;

    // MusicXML graphic note type

    std::string               fCurrentGraphicNoteType;
    mfDurationKind            fCurrentNoteDurationKindFromGraphicNoteType;

    // note quarter tones pitch

    msrQuarterTonesPitchKind  fCurrentNoteQuarterTonesDisplayPitchKind;

    // note sounding duration

    mfWholeNotes              fCurrentNoteSoundingWholeNotesFromNotesDuration;
    mfWholeNotes              fCurrentNoteSoundingWholeNotes;
    mfDurationKind            fCurrentNoteSoundingDurationKind;

    // note display duration

    mfWholeNotes              fCurrentNoteDisplayWholeNotesFromType;
    mfWholeNotes              fCurrentNoteDisplayWholeNotes;

    // note print object kind // JMI for others too 0.9.65
    msrPrintObjectKind        fCurrentNotePrintObjectKind;

    // note heads

    msrNoteHeadKind           fCurrentNoteHeadKind;
    msrNoteHeadFilledKind     fCurrentNoteHeadFilledKind;
    msrNoteHeadParenthesesKind
                              fCurrentNoteHeadParenthesesKind;
    // note color

    std::string               fCurrentNoteRGB;
    std::string               fCurrentNoteAlpha;

    // accidentals

    msrAccidentalKind         fCurrentAccidentalKind;
    msrEditorialAccidentalKind
                              fCurrentEditorialAccidentalKind;
    msrCautionaryAccidentalKind
                              fCurrentCautionaryAccidentalKind;

    // glissandos

    // slides

    // elements attached to the note
    S_msrStem                 fCurrentStem;

    std::string               fCurrentBeamValue;
    int                       fCurrentBeamNumber;

    // ongoing note

    Bool                      fOnGoingNote;

    // initialization

    void                      initializeNoteData ();


    // articulations handling
    // ------------------------------------------------------

    std::list <S_msrArticulation>
                              fCurrentArticulations;

    void                      attachCurrentArticulationsToCurrentNote ();

                                /* JMI
    void                      attachCurrentArticulationsToChord ( // JMI
                                const S_msrChord& chord);
                                */

    // dynamics handling
    // ------------------------------------------------------

    // dynamics, words and wedges remain pending until the next note
    // (they precede the note in MusicXML but follow it in LilyPond)
//     msrPlacementKind          fCurrentDynamicsPlacementKind;

    std::list <S_msrDynamic>  fPendingDynamicsList;
    std::list <S_msrOtherDynamic>
                              fPendingOtherDynamicsList;

    // words handling
    // ------------------------------------------------------

//     msrPlacementKind          fCurrentWordsPlacementKind;
    std::list <S_msrWords>    fPendingWordsList;

    // slurs handling
    // ------------------------------------------------------

    std::list <S_msrSlur>     fPendingSlursList;

    // ligatures handling
    // ------------------------------------------------------

    std::list <S_msrLigature> fPendingLigaturesList;

    // slashes handling
    // ------------------------------------------------------

    std::list <S_msrSlash>    fPendingSlashesList;

    // wedges are 'floating' in MusicXML,
    // but LilyPond needs wedges stops on the note to the right
    std::list <S_msrWedge>    fPendingWedgesList;
    std::list <std::pair <int, S_msrWedge>>
                              fPendingVoiceWedgesList;

    // attaching elements to notes
    // ------------------------------------------------------

    void                      attachPendingNoteLevelElementsIfAnyToCurrentNote ();

    void                      attachPendingStaffLevelElementsIfAnyToCurrentNote (
                                const mfInputLineNumber& inputLineNumber);

    void                      attachPendingDynamicsToCurrentNote ();
    void                      attachPendingOtherDynamicsToCurrentNote ();
    void                      attachPendingWordsToCurrentNote ();
    void                      attachPendingSlursToCurrentNote ();
    void                      attachPendingLigaturesToCurrentNote ();
    void                      attachPendingPedalsToCurrentNote ();
    void                      attachPendingSlashesToCurrentNote ();

    void                      attachPendingWedgesToCurrentNote ();

//     void                      attachPendingVoicesWedgesToCurrentNoteIfRelevant ( // superflous??? JMI 0.9.71
//                                 int theVoiceNumber);

    void                      attachPendingGlissandosToCurrentNote ();
    void                      attachPendingSlidesToCurrentNote ();


//    void                      copyNoteGraceNotesGroupsToChord (
//                                const S_msrNote& note, S_msrChord chord);


    // technicals handling
    // ------------------------------------------------------

    std::list <S_msrTechnical> fCurrentTechnicalsList;
    std::list <S_msrTechnicalWithInteger>
                              fCurrentTechnicalWithIntegersList;
    std::list <S_msrTechnicalWithFloat>
                              fCurrentTechnicalWithFloatsList;
    std::list <S_msrTechnicalWithString>
                              fCurrentTechnicalWithStringsList;

    Bool                      fOnGoingTechnical;

    float                     fBendAlterValue;

    void                      attachCurrentTechnicalsToCurrentNote ();
    void                      attachCurrentTechnicalWithIntegersToCurrentNote ();
    void                      attachCurrentTechnicalWithFloatsToCurrentNote ();
    void                      attachCurrentTechnicalWithStringsToCurrentNote ();

//    void                      attachCurrentTechnicalsToChord ( // JMI
 //                               const S_msrChord& chord);


    // ornaments handling
    // ------------------------------------------------------

    std::list <S_msrOrnament> fCurrentOrnamentsList;

    void                      attachCurrentOrnamentsToCurrentNote ();

//    void                      attachCurrentOrnamentsToChord ( // JMI
 //                               const S_msrChord& chord);


    // spanners handling
    // ------------------------------------------------------

    S_msrSpanner              fCurrentWavyLineSpannerStart;

    std::list <S_msrSpanner>  fCurrentSpannersList;

    void                      attachCurrentSpannersToNote (
																const S_msrNote&   note,
                                const std::string& context);


    // stems handling
    // ------------------------------------------------------

    std::vector <S_msrStem>   fPendingStemsVector; // std::list JMI ???


    // beams handling
    // ------------------------------------------------------

    std::list <S_msrBeam>     fPendingBeamsList;

    void                      attachPendingBeamsToCurrentNote ();


    // glissandos handling
    // ------------------------------------------------------

    std::list <S_msrGlissando>
                              fPendingGlissandosList;


    // slides handling
    // ------------------------------------------------------

    std::list <S_msrSlide>    fPendingSlidesList;


    // tremolos handling
    // ------------------------------------------------------

    Bool                      fCurrentNoteBelongsToADoubleTremolo; // JMI ???

    msrDoubleTremoloTypeKind
                              fCurrentDoubleTremoloTypeKind;

    S_msrSingleTremolo        fCurrentSingleTremolo;

    S_msrDoubleTremolo        fCurrentDoubleTremolo;

    void                      attachCurrentSingleTremoloToCurrentNote ();


    // ties handling
    // ------------------------------------------------------

// JMI    std::string               fCurrentTiedType;
    msrTieKind                fCurrentTieKind;
    std::string               fCurrentTiedOrientation; // JMI 0.9.70
//     S_msrTie                  fCurrentTie;
    std::list <S_msrTie>      fPendingTiesList;

    void                      attachPendingTiesToCurrentNote ();


    // slurs handling
    // ------------------------------------------------------

    std::string               fCurrentSlurType;
    msrSlurTypeKind           fCurrentSlurTypeKind;
    Bool                      fOnGoingSlur;
    Bool                      fOnGoingSlurHasStanza;

    std::list <S_msrSlur>     fSlurStartsStack;
    void                      displaySlurStartsStack (
                                const std::string& context) const;

    // ligatures handling
    // ------------------------------------------------------

    S_msrLigature             fCurrentLigatureStartAbove;
    S_msrLigature             fCurrentLigatureStartBelow;

    std::string               fCurrentLigaturePlacement;
    msrLigatureKind           fCurrentLigatureKind;
    Bool                      fOnGoingLigature;
    Bool                      fOnGoingLigatureHasStanza;


    // piano pedals handling
    // ------------------------------------------------------

    std::list <S_msrPedal>    fPendingPedalsList;

/* JMI
    damperPedalKind           fCurrentDamperPedalKind;
    int                       fCurrentDamperPedalIntegerValue;

    damperPedalKind           fCurrentDamperPedalKind;
    int                       fCurrentDamperPedalIntegerValue;

    damperPedalKind           fCurrentDamperPedalKind;
    int                       fCurrentDamperPedalIntegerValue;
*/


    // harp pedals handling
    // ------------------------------------------------------

    msrDiatonicPitchKind      fCurrentHarpPedalDiatonicPitchKind;
    msrAlterationKind         fCurrentHarpPedalAlterationKind;
    S_msrHarpPedalsTuning     fCurrentHarpPedalsTuning;


    // backup handling
    // ------------------------------------------------------

    int                       fCurrentBackupDuration;
    Bool                      fOnGoingBackup;

//     void                      handleBackup (
//                                 const mfInputLineNumber& inputLineNumber);


    // forward handling
    // ------------------------------------------------------

    int                       fCurrentForwardDuration;
    Bool                      fOnGoingForward;

    int                       fCurrentMusicXMLForwardStaffNumber;
    int                       fCurrentMusicXMLForwardVoiceNumber;

//     Bool                      fAForwardHasJustBeenHandled;
    std::list <S_msrVoice>    fForwardedToVoicesList;

    // current ongoing values display
    // ------------------------------------------------------
    void                      displayCurrentOnGoingValues () const;
};


}


#endif // ___mxsr2msrTranslator___
