\version "2.24.4"

\header {
  workCreditTypeTitle = "Non-contiguous Voice Numbers"
  encodingDate        = "2016-10-24"
  software            = "MuseScore 2.0.3"
  title               = "Non-contiguous Voice Numbers"
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
  \partial 2.
  
  \clef "treble"
  \key b \major
  \numericTimeSignature \time 4/4
  \stemUp gis'2.  | % 1
  \barNumberCheck #1
  fis'4 gis' gis' ais'  | % 2
  \barNumberCheck #2
  \stemDown b'4 \stemUp ais' ais' gis'  | % 0
  \barNumberCheck #3
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  \partial 2.
  
  \clef "bass"
  \key b \major
  \numericTimeSignature \time 4/4
  \stemUp b,2.  | % 1
  \barNumberCheck #1
  b,8 [
  b, b, ]
  cis4. r4  | % 2
  \barNumberCheck #2
  \stemDown dis8 [
  dis dis ]
  e4. r4  | % 0
  \barNumberCheck #3
}

Part_POne_Staff_Three_Voice_Nine = \absolute {
  \language "nederlands"
  \partial 2.
  
  \clef "bass"
  \key b \major
  \numericTimeSignature \time 4/4
  r2.  | % 1
  \barNumberCheck #1
  R1  | % 2
  \barNumberCheck #2
  R1  | % 3
  \barNumberCheck #3
}

\book {

  \score {
    <<
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Org."
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
          
          \new Staff  = "Part_POne_Staff_Three"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_Three_Voice_Nine" <<
              \Part_POne_Staff_Three_Voice_Nine
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
