\version "2.24.4"

\header {
  movementTitle       = "TupletTest1"
  title               = "TupletTest1"
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
  
  %{ begin kMeasureKindOverFlowing, measure 1, % measureCurrentPositionInMeasure: pim 1/1 %}
  
  \clef "treble"
  \key e \minor
  \numericTimeSignature \time 2/4
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 46 %}  \stemDown fis''8 [  %{ beam 1, line 60 %}
  ( e'' ) c'' ]  %{ beam 1, line 97 %}
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 102 %}  fis''8 [  %{ beam 1, line 116 %}
  ( e'' ) c'' ]  %{ beam 1, line 153 %}
  }
  %{ end kMeasureKindOverFlowing, measure  %}
  
  %{ begin kMeasureKindOverFlowing, measure 2, % measureCurrentPositionInMeasure: pim 1/1 %}
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 161 %}  fis''8 [  %{ beam 1, line 175 %}
  ( e'' ) c'' ]  %{ beam 1, line 212 %}
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 217 %}  fis''8 [  %{ beam 1, line 231 %}
  ( e'' ) c'' ]  %{ beam 1, line 268 %}
  }
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
  
  %{ end kMeasureKindOverFlowing, measure  %}
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Piano"
          shortInstrumentName = "Pno."
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
