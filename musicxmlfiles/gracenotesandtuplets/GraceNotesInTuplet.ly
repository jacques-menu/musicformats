\version "2.24.4"

\header {
  workCreditTypeTitle = "Grace Notes In Tuplet"
  encodingDate        = "2011-08-08"
  composer            = "Franz Schubert"
  poet                = "Walter Scott"
  translator          = "D. Adam Storck"
  software            = "Finale 2011 for Windows"
  software            = "Dolet 6.0 for Finale"
  right               = "Copyright © 2002 Recordare LLC"
  title               = "Grace Notes In Tuplet"
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
  
  %{ begin kMeasureKindOverFlowing, measure 1, % measureCurrentPositionInMeasure: pim 5/4 %}
  
  \clef "treble"
  \key bes \major
  \time 4/4
  
  \stopStaff
  \override Staff.StaffSymbol.line-count = 5
  \startStaff
  \stemDown c''4 ~  (  % noteIsFollowedByGraceNotesGroup
  \once \omit TupletBracket
  
  \once\override TupletBracket.direction = -1 % DOWN
  \tuplet 6/4 {  %{ tupletNumber: 1, tupleFactor: 6/4, line 139 %}  \stemUp c''16 [  %{ beam 1, line 153 %}
  g' a' bes'! \grace { \stemDown c''16 [  %{ line 210, elementNumber: 1 %}
   bes'16 ]  %{ line 222, elementNumber: 2 %}
  } \stemUp a'16 \stemDown \stemUp g' ]  %{ beam 1, line 264 %}
  ) } f'4 r8 f'
  %{ end kMeasureKindOverFlowing, measure  %}
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Voice"
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
