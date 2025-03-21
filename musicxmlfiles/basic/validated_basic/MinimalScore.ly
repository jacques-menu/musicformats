\version "2.24.4"

\header {
  workCreditTypeTitle = "Minimal score"
  encodingDate        = "2016-08-30"
  software            = "Frescobaldi 2.18.1"
  title               = "Minimal score"
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

_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble_8"
  \key e \major
  \time 4/4
  cis'!2 \p _\< \! \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      90
    } % concat
  }
  bes!4 \! r ees'!2.. f'16 g,
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
   | % 3
  \barNumberCheck #3
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "_Staff_One"
        \with {
          instrumentName = "Part_POne"
        }
        <<
          \context Voice = "_Staff_One_Voice_One" <<
            \_Staff_One_Voice_One
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
