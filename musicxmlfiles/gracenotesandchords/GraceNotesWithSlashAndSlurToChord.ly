\version "2.24.4"

\header {
  workCreditTypeTitle = "Grace Notes With Slash And Slur To Chord"
  encodingDate        = "2020-07-13"
  software            = "Sibelius 20200.6"
  software            = "Dolet 6.6 for Sibelius"
  right               = "Copyright © "
  title               = "Grace Notes With Slash And Slur To Chord"
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
  \numericTimeSignature \time 1/4
  \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      100
    } % concat
  }
  r8
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
   | % 1
  \barNumberCheck #1
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Klavier"
          shortInstrumentName = "Klav."
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
