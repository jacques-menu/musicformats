\version "2.24.4"

\header {
  workCreditTypeTitle = "Nested Tuplets Left"
  movementTitle       = "Bloc 1"
  encodingDate        = "2024-11-14"
  software            = "Dorico 5.1.60.2187"
  title               = "Nested Tuplets Left"
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
  
  %{ begin kMeasureKindOverFlowing, measure 1, % measureCurrentPositionInMeasure: pim 13/10 %}
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 4/4
  \stemUp g'4
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 5/4 {  %{ tupletNumber: 1, tupleFactor: 5/4, line 99 %}  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 2, tupleFactor: 3/2, line 99 %}  \stemDown a'8 [  %{ beam 1, line 113 %}
  b' c'' ]  %{ beam 1, line 167 %}
  } d'' [  %{ beam 1, line 187 %}
  e'' f'' ]  %{ beam 1, line 221 %}
  } g''4
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
  
  %{ end kMeasureKindOverFlowing, measure  %}
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Voice"
          shortInstrumentName = "V."
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
