\version "2.24.4"

\header {
  workCreditTypeTitle = "Grace Note At End Of Mesure"
  miscellaneousField  = "Different kinds of grace notes:
          acciaccatura, appoggiatura; beamed grace notes; grace notes with
          accidentals; different durations of the grace notes."
  title               = "Grace Note At End Of Mesure"
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
  \time 4/4
  c''2. c''8 [  %{ beam 1, line 56 %}
   % noteIsFollowedByGraceNotesGroup
  \afterGrace { \slashedGrace { d''16 } e''8 ]  %{ beam 1, line 75 %}
   % noteIsFollowedByGraceNotesGroup
  } \grace { g'16 } f'4
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "MusicXML Part"
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
