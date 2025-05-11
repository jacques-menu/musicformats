\version "2.24.4"

\header {
  workCreditTypeTitle = "Humdrum/Scot Keys"
  miscellaneousField  = "Non-traditional key signatures,
          where each alteration is separately given. Here we have (f sharp,
          a flat, b flat) and (c flatflat, g sharp sharp, d flat, b sharp, f
          natural), where in the second case an explicit octave is given for
          each alteration."
  title               = "Humdrum/Scot Keys"
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
  
  \set Staff.keyAlterations = #`((4 . ,SHARP) (6 . ,FLAT) (7 . ,FLAT))\numericTimeSignature \time 2/4
  c'2
  \set Staff.keyAlterations = #`(((0 . 1) . ,DOUBLE-FLAT) ((1 . 5) . ,DOUBLE-SHARP) ((2 . 2) . ,FLAT) ((3 . 7) . ,SHARP) ((4 . 4) . ,NATURAL))c'2
  \bar "|."
   | % 3
  \barNumberCheck #3
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "MusicXML"
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
