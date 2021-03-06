/*
  MusicXML Library
  Copyright (C) Grame 2006-2019

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

//======================================================================
// DON'T MODIFY THIS FILE!
// It is automatically generated from the MusicXML DTDs and from 
// template files (located in the templates folder). 
//======================================================================

#ifndef __elements__
#define __elements__

namespace MusicXML2 
{

enum {	
	kNoElement,
	kComment,
	kProcessingInstruction,
	k_accent,
	k_accidental,
	k_accidental_mark,
	k_accidental_text,
	k_accord,
	k_accordion_high,
	k_accordion_low,
	k_accordion_middle,
	k_accordion_registration,
	k_actual_notes,
	k_alter,
	k_appearance,
	k_arpeggiate,
	k_arrow,
	k_arrow_direction,
	k_arrow_style,
	k_arrowhead,
	k_articulations,
	k_artificial,
	k_assess,
	k_attributes,
	k_backup,
	k_bar_style,
	k_barline,
	k_barre,
	k_base_pitch,
	k_bass,
	k_bass_alter,
	k_bass_separator,
	k_bass_step,
	k_beam,
	k_beat_repeat,
	k_beat_type,
	k_beat_unit,
	k_beat_unit_dot,
	k_beat_unit_tied,
	k_beater,
	k_beats,
	k_bend,
	k_bend_alter,
	k_bookmark,
	k_bottom_margin,
	k_bracket,
	k_brass_bend,
	k_breath_mark,
	k_caesura,
	k_cancel,
	k_capo,
	k_chord,
	k_chromatic,
	k_circular_arrow,
	k_clef,
	k_clef_octave_change,
	k_coda,
	k_concert_score,
	k_creator,
	k_credit,
	k_credit_image,
	k_credit_symbol,
	k_credit_type,
	k_credit_words,
	k_cue,
	k_damp,
	k_damp_all,
	k_dashes,
	k_defaults,
	k_degree,
	k_degree_alter,
	k_degree_type,
	k_degree_value,
	k_delayed_inverted_turn,
	k_delayed_turn,
	k_detached_legato,
	k_diatonic,
	k_direction,
	k_direction_type,
	k_directive,
	k_display_octave,
	k_display_step,
	k_display_text,
	k_distance,
	k_divisions,
	k_doit,
	k_dot,
	k_double,
	k_double_tongue,
	k_down_bow,
	k_duration,
	k_dynamics,
	k_effect,
	k_elevation,
	k_elision,
	k_encoder,
	k_encoding,
	k_encoding_date,
	k_encoding_description,
	k_end_line,
	k_end_paragraph,
	k_ending,
	k_ensemble,
	k_except_voice,
	k_extend,
	k_eyeglasses,
	k_f,
	k_falloff,
	k_feature,
	k_fermata,
	k_ff,
	k_fff,
	k_ffff,
	k_fffff,
	k_ffffff,
	k_fifths,
	k_figure,
	k_figure_number,
	k_figured_bass,
	k_fingering,
	k_fingernails,
	k_first,
	k_first_fret,
	k_flip,
	k_footnote,
	k_for_part,
	k_forward,
	k_fp,
	k_frame,
	k_frame_frets,
	k_frame_note,
	k_frame_strings,
	k_fret,
	k_function,
	k_fz,
	k_glass,
	k_glissando,
	k_glyph,
	k_golpe,
	k_grace,
	k_group,
	k_group_abbreviation,
	k_group_abbreviation_display,
	k_group_barline,
	k_group_link,
	k_group_name,
	k_group_name_display,
	k_group_symbol,
	k_group_time,
	k_grouping,
	k_half_muted,
	k_hammer_on,
	k_handbell,
	k_harmon_closed,
	k_harmon_mute,
	k_harmonic,
	k_harmony,
	k_harp_pedals,
	k_haydn,
	k_heel,
	k_hole,
	k_hole_closed,
	k_hole_shape,
	k_hole_type,
	k_humming,
	k_identification,
	k_image,
	k_instrument,
	k_instrument_abbreviation,
	k_instrument_change,
	k_instrument_link,
	k_instrument_name,
	k_instrument_sound,
	k_instruments,
	k_interchangeable,
	k_inversion,
	k_inverted_mordent,
	k_inverted_turn,
	k_inverted_vertical_turn,
	k_ipa,
	k_key,
	k_key_accidental,
	k_key_alter,
	k_key_octave,
	k_key_step,
	k_kind,
	k_laughing,
	k_left_divider,
	k_left_margin,
	k_level,
	k_line,
	k_line_detail,
	k_line_width,
	k_link,
	k_listen,
	k_listening,
	k_lyric,
	k_lyric_font,
	k_lyric_language,
	k_measure,
	k_measure_distance,
	k_measure_layout,
	k_measure_numbering,
	k_measure_repeat,
	k_measure_style,
	k_membrane,
	k_metal,
	k_metronome,
	k_metronome_arrows,
	k_metronome_beam,
	k_metronome_dot,
	k_metronome_note,
	k_metronome_relation,
	k_metronome_tied,
	k_metronome_tuplet,
	k_metronome_type,
	k_mf,
	k_midi_bank,
	k_midi_channel,
	k_midi_device,
	k_midi_instrument,
	k_midi_name,
	k_midi_program,
	k_midi_unpitched,
	k_millimeters,
	k_miscellaneous,
	k_miscellaneous_field,
	k_mode,
	k_mordent,
	k_movement_number,
	k_movement_title,
	k_mp,
	k_multiple_rest,
	k_music_font,
	k_mute,
	k_n,
	k_natural,
	k_non_arpeggiate,
	k_normal_dot,
	k_normal_notes,
	k_normal_type,
	k_notations,
	k_note,
	k_note_size,
	k_notehead,
	k_notehead_text,
	k_numeral,
	k_numeral_alter,
	k_numeral_fifths,
	k_numeral_key,
	k_numeral_mode,
	k_numeral_root,
	k_octave,
	k_octave_change,
	k_octave_shift,
	k_offset,
	k_open,
	k_open_string,
	k_opus,
	k_ornaments,
	k_other_appearance,
	k_other_articulation,
	k_other_direction,
	k_other_dynamics,
	k_other_listen,
	k_other_listening,
	k_other_notation,
	k_other_ornament,
	k_other_percussion,
	k_other_play,
	k_other_technical,
	k_p,
	k_page_height,
	k_page_layout,
	k_page_margins,
	k_page_width,
	k_pan,
	k_part,
	k_part_abbreviation,
	k_part_abbreviation_display,
	k_part_clef,
	k_part_group,
	k_part_link,
	k_part_list,
	k_part_name,
	k_part_name_display,
	k_part_symbol,
	k_part_transpose,
	k_pedal,
	k_pedal_alter,
	k_pedal_step,
	k_pedal_tuning,
	k_per_minute,
	k_percussion,
	k_pf,
	k_pitch,
	k_pitched,
	k_play,
	k_player,
	k_player_name,
	k_plop,
	k_pluck,
	k_pp,
	k_ppp,
	k_pppp,
	k_ppppp,
	k_pppppp,
	k_pre_bend,
	k_prefix,
	k_principal_voice,
	k_print,
	k_pull_off,
	k_rehearsal,
	k_relation,
	k_release,
	k_repeat,
	k_rest,
	k_rf,
	k_rfz,
	k_right_divider,
	k_right_margin,
	k_rights,
	k_root,
	k_root_alter,
	k_root_step,
	k_scaling,
	k_schleifer,
	k_scoop,
	k_scordatura,
	k_score_instrument,
	k_score_part,
	k_score_partwise,
	k_score_timewise,
	k_second,
	k_segno,
	k_semi_pitched,
	k_senza_misura,
	k_sf,
	k_sffz,
	k_sfp,
	k_sfpp,
	k_sfz,
	k_sfzp,
	k_shake,
	k_sign,
	k_slash,
	k_slash_dot,
	k_slash_type,
	k_slide,
	k_slur,
	k_smear,
	k_snap_pizzicato,
	k_soft_accent,
	k_software,
	k_solo,
	k_sound,
	k_sounding_pitch,
	k_source,
	k_spiccato,
	k_staccatissimo,
	k_staccato,
	k_staff,
	k_staff_details,
	k_staff_distance,
	k_staff_divide,
	k_staff_layout,
	k_staff_lines,
	k_staff_size,
	k_staff_tuning,
	k_staff_type,
	k_staves,
	k_stem,
	k_step,
	k_stick,
	k_stick_location,
	k_stick_material,
	k_stick_type,
	k_stopped,
	k_straight,
	k_stress,
	k_string,
	k_string_mute,
	k_strong_accent,
	k_suffix,
	k_supports,
	k_swing,
	k_swing_style,
	k_swing_type,
	k_syllabic,
	k_symbol,
	k_sync,
	k_system_distance,
	k_system_dividers,
	k_system_layout,
	k_system_margins,
	k_tap,
	k_technical,
	k_tenths,
	k_tenuto,
	k_text,
	k_thumb_position,
	k_tie,
	k_tied,
	k_time,
	k_time_modification,
	k_time_relation,
	k_timpani,
	k_toe,
	k_top_margin,
	k_top_system_distance,
	k_touching_pitch,
	k_transpose,
	k_tremolo,
	k_trill_mark,
	k_triple_tongue,
	k_tuning_alter,
	k_tuning_octave,
	k_tuning_step,
	k_tuplet,
	k_tuplet_actual,
	k_tuplet_dot,
	k_tuplet_normal,
	k_tuplet_number,
	k_tuplet_type,
	k_turn,
	k_type,
	k_unpitched,
	k_unstress,
	k_up_bow,
	k_vertical_turn,
	k_virtual_instrument,
	k_virtual_library,
	k_virtual_name,
	k_voice,
	k_volume,
	k_wait,
	k_wavy_line,
	k_wedge,
	k_with_bar,
	k_wood,
	k_word_font,
	k_words,
	k_work,
	k_work_number,
	k_work_title,
	kEndElement
};

}
#endif

