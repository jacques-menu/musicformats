\version "2.24.4"

\header {
  workCreditTypeTitle = "Several Tuplets In A Row"
  movementTitle       = "Several Tuplets In A Row"
  encodingDate        = "2024-10-06"
  composer            = "Lars P. Opfermann"
  arranger            = "2016"
  software            = "Finale v27.4 for Mac"
  right               = "©"
  title               = "Several Tuplets In A Row"
}

\paper {
  % horizontal-shift = 0.0\mm
  % indent = 0.0\mm
  % short-indent = 0.0\mm
  
  % markup-system-spacing-padding = 0.0\mm
  % between-system-space = 0.0\mm
  % page-top-space = 0.0\mm
  
  % page-count = -1
  % system-count = -1
  
  % oddHeaderMarkup = ""
  % evenHeaderMarkup = ""
  % oddFooterMarkup = ""
  % evenFooterMarkup = ""
}

\layout {
  \context { \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context { \Voice
  }
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  %{ begin kMeasureKindOverFlowing, measure 1, % measureAccumulatedWholeNotesDuration: 1/1 whn %}
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 3/4
  \stemUp c'4 d' \mp _-
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 157 %}  e'4 \mf _- f'8 _- }
  %{ end kMeasureKindOverFlowing, measure  %}
  
  %{ begin kMeasureKindOverFlowing, measure 2, % measureAccumulatedWholeNotesDuration: 37/24 whn %}
  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 204 %}  f'8 [  %{ beam 1, line 218 %}
  g'8. _\markup { "possibile" } \f _- a'16 ]  %{ beam 1, line 273 %}
  _- }
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 283 %}  a'8 [  %{ beam 1, line 297 %}
  bis'!16 _. b'! _. cis''! _. d'' ]  %{ beam 1, line 384 %}
  _. }
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 393 %}  e''16 [  %{ beam 1, line 407 %}
  _. f''!8. \mf _- g''8 ]  %{ beam 1, line 461 %}
  _- }
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
  
  %{ end kMeasureKindOverFlowing, measure  %}
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Flöte"
          shortInstrumentName = "Fl."
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
        >>
      
      
      >>
    
    >>
    
    \layout {
      \context { \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context { \Voice
      }
    }
    
    \midi {
      \tempo 16 = 360
    }
  }
  
}
