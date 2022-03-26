\version "2.22.0"

% Comment or adapt next line as needed (default is 20)
#(set-global-staff-size 30.3)

% Pick your choice from the next two lines as needed
%myBreak = { \break }
myBreak = {}

% Pick your choice from the next two lines as needed
%myPageBreak = { \pageBreak }
myPageBreak = {}

\header {
  title                = "Hello World!"
  workTitle            = "Hello World!"
  title                = "Hello World!"
}

\paper {
  % page-count = -1
  % system-count = -1
  ragged-bottom = ##f
  ragged-last-bottom = ##t
}

\layout {
  \context {
    \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context {
    \Voice
  }
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 4/4
  
  \clef "treble"
  c'1 | % 2
  \barNumberCheck #2
  | % 2
  \barNumberCheck #2
}

\book {
  \score {
    <<
      
      \new Staff = "Part_POne_Staff_One"
      \with {
        instrumentName = "Music"
      }
      <<
        \context Voice = "Part_POne_Staff_One_Voice_One" <<
          \Part_POne_Staff_One_Voice_One
        >>
      >>
      
    >>
    
    \layout {
      \context {
        \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context {
        \Voice
      }
    }
    
    \midi {
      \tempo 16 = 360
    }
  }
  
}
