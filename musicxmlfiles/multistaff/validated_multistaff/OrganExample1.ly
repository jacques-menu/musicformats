\version "2.24.4"

\header {
  workCreditTypeTitle = "Organ Example"
  encodingDate        = "2016-10-24"
  software            = "MuseScore 2.0.3"
  title               = "Organ Example"
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
  \stemDown b'4 \stemUp ais' ais' gis'  | % 3
  \barNumberCheck #3
  fis'4 e' e' dis'  | % 4
  \barNumberCheck #4
  e'4 fis' fis' gis'  | % 5
  \barNumberCheck #5
  ais'4 \stemDown b' b' cis''  | % 6
  \barNumberCheck #6
  b'4 \stemUp ais' gis' gis'  | % 7
  \barNumberCheck #7
  \break | % 1333333 \myLineBreak
  
  fis'4 fis' e' dis'  | % 8
  \barNumberCheck #8
  e'2 e'  | % 9
  \barNumberCheck #9
  e'2 e'  | % 10
  \barNumberCheck #10
  dis'2 cis'
  \bar "|."
   | % 0
  \barNumberCheck #11
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
  e4. r4  | % 3
  \barNumberCheck #3
  dis8 [
  dis dis ]
  \stemUp cis2 r8  | % 4
  \barNumberCheck #4
  b,8 [
  b, b, ]
  ais,2 r8  | % 5
  \barNumberCheck #5
  ais,8 [
  ais, ais, ais, ]
  b,2  | % 6
  \barNumberCheck #6
  cis4 cis cis \stemDown e  | % 7
  \barNumberCheck #7
  \break | % 1333333 \myLineBreak
  
  dis2 e  | % 8
  \barNumberCheck #8
  e2 dis  | % 9
  \barNumberCheck #9
  dis2 \stemUp cis  | % 10
  \barNumberCheck #10
  cis2 ais,
  \bar "|."
   | % 0
  \barNumberCheck #11
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
  R1  | % 4
  \barNumberCheck #4
  \stemDown dis8 [
  dis dis ]
  \stemUp cis4. ais,4  | % 5
  \barNumberCheck #5
  ais,1 ~   | % 6
  \barNumberCheck #6
  ais,1 ~   | % 7
  \barNumberCheck #7
  \break | % 1333333 \myLineBreak
  
  ais,1 ~   | % 8
  \barNumberCheck #8
  ais,1  | % 9
  \barNumberCheck #9
  b,8 [
  cis dis cis ]
  \stemDown dis [
  e fis e ]
   | % 10
  \barNumberCheck #10
  fis8 [
  gis ais gis ]
  ais cis'4.
  \bar "|."
   | % 11
  \barNumberCheck #11
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
