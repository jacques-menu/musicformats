\version "2.24.4"

\header {
  workCreditTypeTitle = "Multi-Staff Clef Change"
  encodingDate        = "2011-08-08"
  composer            = "Ludwig van Beethoven"
  lyricist            = "Aloys Jeitteles"
  software            = "Finale 2011 for Windows"
  software            = "Dolet 6.0 for Finale"
  right               = "Copyright © 2002 Recordare LLC"
  title               = "Multi-Staff Clef Change"
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
  \key ees \major
  \numericTimeSignature \time 3/4
  r4 ^\markup { \bold "No. 1" } \once\override Score.MetronomeMark.direction = #DOWN
  \tempo \markup {
    "Ziemlich langsam und mit Ausdruck"\concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      60
    } % concat
  }
\stemDown bes' bes' }

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key ees \major
  \numericTimeSignature \time 3/4
\stemUp  < bes ees' g' bes' > 4 -\p -\p -\p \stemDown bes' ( \stemUp aes' ) }

Part_PTwo_Staff_Two_Voice_Three = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key ees \major
  \numericTimeSignature \time 3/4
    \stemUp  < ees,
    \sustainOn
  ees > 4
  \clef "treble"
    \slurDown g'
    \sustainOff
( f' ) }

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
        \new PianoStaff
        \with {
          instrumentName = "Part_PTwo"
        }
        
        <<
        
          \new Staff  = "Part_PTwo_Staff_One"
          \with {
          }
          <<
            \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
              \Part_PTwo_Staff_One_Voice_One
            >>
          >>
          \new Staff  = "Part_PTwo_Staff_Two"
          \with {
          }
          <<
            \context Voice = "Part_PTwo_Staff_Two_Voice_Three" <<
              \Part_PTwo_Staff_Two_Voice_Three
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
