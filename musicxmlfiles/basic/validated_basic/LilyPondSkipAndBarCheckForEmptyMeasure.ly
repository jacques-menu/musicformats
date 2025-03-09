\version "2.24.4"

\header {
  workCreditTypeTitle = "LilyPondSkipAndBarCheckForEmptyMeasure"
  encodingDate        = "2024-10-14"
  software            = "Sibelius 20240.6"
  software            = "Dolet 6.6 for Sibelius"
  title               = "LilyPondSkipAndBarCheckForEmptyMeasure"
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
  \numericTimeSignature \time 2/4
  \stemUp g'2 ~  g'2  | % 3
  \barNumberCheck #3
  \key d \major
  \bar "||"  %{ b333 visitStart (S_msrBarLine& elt) %}
  a'2 ~   | % 4
  \barNumberCheck #4
  a'2
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
   | % 1
  \barNumberCheck #5
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 2/4
  s %{ s222 %}  2 \stemDown b4 c'  | % 3
  \barNumberCheck #3
  \key d \major
  \bar "||"  %{ b333 visitStart (S_msrBarLine& elt) %}
  s %{ s222 %}  2  | % 4
  \barNumberCheck #4
  cis'4 d'
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
   | % 5
  \barNumberCheck #5
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Violine"
          shortInstrumentName = "Vl."
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
