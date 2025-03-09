\version "2.24.4"

\header {
  workCreditTypeTitle = "Grace Notes With Trill"
  encodingDate        = "2017-04-03"
  software            = "Finale 2014.5 for Mac"
  software            = "Dolet Light for Finale 2014.5"
  title               = "Grace Notes With Trill"
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
  \stemDown c''4 r8 \grace { \stemUp g'16 [  %{ line 132, elementNumber: 1 %}
   a'16 ]  %{ line 144, elementNumber: 2 %}
  } \stemDown b'8 \stemUp \trill  % noteIsFollowedByGraceNotesGroup
  \grace { a'16. [  %{ line 171, elementNumber: 1 %}
   b'16. ]  %{ line 184, elementNumber: 2 %}
} \stemDown c''4 \stemUp r }

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
