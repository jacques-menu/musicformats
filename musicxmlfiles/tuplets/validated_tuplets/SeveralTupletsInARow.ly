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
  
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 3/4
  \stemUp c'4 d' \mp _-
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  e'4 \mf _- f'8 _- }
  
  
  \once\override TupletBracket.bracket-visibility = ##t
  
  \once\override TupletBracket.direction = -1 % DOWN
  \tuplet 3/2 {  f'8 [
  g'8. _\markup { "possibile" } \f _- a'16 ]
  _- }
  \once\override TupletBracket.bracket-visibility = ##t
  
  \once\override TupletBracket.direction = -1 % DOWN
  \tuplet 3/2 {  a'8 [
  bis'!16 _. b'! _. cis''! _. d'' ]
  _. }
  \once\override TupletBracket.bracket-visibility = ##t
  
  \once\override TupletBracket.direction = -1 % DOWN
  \tuplet 3/2 {  e''16 [
  _. f''!8. \mf _- g''8 ]
  _- }
  \bar "|."
  
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
