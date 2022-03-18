\version "2.22.0"

% Comment or adapt next line as needed (default is 20)
#(set-global-staff-size 25.5)

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
  ragged-last-bottom = ##f
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

\book {
  \score {
    <<
      
      \new Staff = "Part_POne_Staff_One"
      \with {
        instrumentName = "Music"
      }
      <<
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
