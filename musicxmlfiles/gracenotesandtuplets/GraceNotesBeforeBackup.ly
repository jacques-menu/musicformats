\version "2.24.4"

\header {
  workCreditTypeTitle = "Grace Notes Before Backup"
  encodingDate        = "2021-11-17"
  software            = "soundnotation"
  software            = "Dolet 6.6"
  right               = ""
  title               = "Grace Notes Before Backup"
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
  \key d \minor
  \numericTimeSignature \time 3/4
  \stemUp f'2 g'4 \trill ) (  % noteIsFollowedByGraceNotesGroup
  
  %{ begin kMeasureKindOverFlowing, measure 4, % measureCurrentPositionInMeasure: pim 5/4 %}
  a'4 \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 340 %}  f'8 [  %{ beam 1, line 354 %}
  a' c'' ]  %{ beam 1, line 389 %}
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 394 %}  \stemDown f''8 [  %{ beam 1, line 408 %}
  f'' f'' ]  %{ beam 1, line 443 %}
  }
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
  
  %{ end kMeasureKindOverFlowing, measure  %}
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  %{ begin kMeasureKindOverFlowing, measure 1, % measureCurrentPositionInMeasure: pim 3/2 %}
  
  \clef "bass"
  \key d \minor
  \numericTimeSignature \time 3/4
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 164 %}  \grace { \stemUp f'16  g'16 } \stemDown c8 \stemUp [  %{ beam 1, line 178 %}
  \stemDown f a ]  %{ beam 1, line 213 %}
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 218 %}  c8 [  %{ beam 1, line 232 %}
  f a ]  %{ beam 1, line 267 %}
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 272 %}  c8 [  %{ beam 1, line 286 %}
  e g ]  %{ beam 1, line 321 %}
  }
  %{ end kMeasureKindOverFlowing, measure  %}
  
  %{ begin kMeasureKindOverFlowing, measure 4, % measureCurrentPositionInMeasure: pim 1/1 %}
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 451 %}  f8 [  %{ beam 1, line 465 %}
  a c' ]  %{ beam 1, line 500 %}
  } r4 r
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
  
  %{ end kMeasureKindOverFlowing, measure  %}
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Pno."
        }
        
        <<
        
          \new Staff  = "Part_POne_Staff_One"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \Part_POne_Staff_One_Voice_One
            >>
          >>
          
          \new Staff  = "Part_POne_Staff_Two"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_Two_Voice_Five" <<
              \Part_POne_Staff_Two_Voice_Five
            >>
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
