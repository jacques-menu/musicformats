\version "2.24.4"

\header {
  workCreditTypeTitle = "TupletTest2"
  title               = "TupletTest2"
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
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 48 %}  \stemUp fis''8 [  %{ beam 1, line 62 %}
  ( e'' ) c'' ]  %{ beam 1, line 136 %}
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 141 %}  fis''8 [  %{ beam 1, line 155 %}
  ( e'' ) c'' ]  %{ beam 1, line 192 %}
  }
  %{ end kMeasureKindOverFlowing, measure  %}
  
  %{ begin kMeasureKindOverFlowing, measure 2, % measureCurrentPositionInMeasure: pim 1/1 %}
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 200 %}  fis''8 [  %{ beam 1, line 214 %}
  ( e'' ) c'' ]  %{ beam 1, line 287 %}
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 292 %}  fis''8 [  %{ beam 1, line 306 %}
  ( e'' ) c'' ]  %{ beam 1, line 343 %}
  }
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
  
  %{ end kMeasureKindOverFlowing, measure  %}
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key e \minor
  \numericTimeSignature \time 2/4
  \stemDown  < e' ~  ais'! ~  > 2  < e' ais' > 2
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
   | % 3
  \barNumberCheck #3
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
            \voiceOne % out of 2 regular voices
            \Part_POne_Staff_One_Voice_One
          >>
          \context Voice = "Part_POne_Staff_One_Voice_Two" <<
            \voiceTwo % out of 2 regular voices
            \Part_POne_Staff_One_Voice_Two
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
