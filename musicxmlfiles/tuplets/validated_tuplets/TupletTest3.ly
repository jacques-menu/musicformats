\version "2.24.4"

\header {
  workCreditTypeTitle = "TupletTest3"
  title               = "TupletTest3"
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
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 48 %}  \stemDown fis''8 [  %{ beam 1, line 62 %}
  ( e'' ) c'' ]  %{ beam 1, line 105 %}
  \< } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 110 %}  fis''8 [  %{ beam 1, line 124 %}
  ( e'' ) c'' ]  %{ beam 1, line 161 %}
  }
  %{ end kMeasureKindOverFlowing, measure  %}
  
  %{ begin kMeasureKindOverFlowing, measure 2, % measureCurrentPositionInMeasure: pim 1/1 %}
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 169 %}  fis''8 [  %{ beam 1, line 183 %}
  ( e'' ) c'' ]  %{ beam 1, line 220 %}
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 225 %}  fis''8 [  %{ beam 1, line 239 %}
  ( e'' ) c'' ]  %{ beam 1, line 276 %}
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
