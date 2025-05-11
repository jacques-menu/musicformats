\version "2.24.4"

\header {
  workCreditTypeTitle = "Organ example 2"
  encodingDate        = "2016-10-24"
  software            = "MuseScore 2.0.3"
  title               = "Organ example 2"
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
  \numericTimeSignature \time 4/4
  \stemUp aes'!4 ^\markup { "Andante" } \f ( \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      75
    } % concat
  }
  ges'!8 [
  aes' ]
  \stemDown b'4 \stemUp b d'8 [
  e' ]
  b4 d'8 [
  e' ]
  b4 )  | % 3
  \barNumberCheck #3
  aes'!4 ( ges'!8 [
  aes' ]
  b' [
  aes' e' b ]
   | % 4
  \barNumberCheck #4
  d'1 )  | % 5
  \barNumberCheck #5
  f'4 ( g'8 [
  a' ]
  \stemDown e''4 \stemUp a  | % 6
  \barNumberCheck #6
  d'8 [
  e' ]
  a4 d' f' )  | % 7
  \barNumberCheck #7
  e'4 ^\markup { "Philip Hammond" } ( e' b b )  | % 8
  \barNumberCheck #8
  \break | % 1333333 \myLineBreak
  
  R1  | % 9
  \barNumberCheck #9
  \stemDown c''4. ( c''8 b'4 \stemUp g'  | % 10
  \barNumberCheck #10
  \stemDown c''4 c'' c'' d'' )  | % 11
  \barNumberCheck #11
  c''4. ( c''8 b'4 \stemUp g'  | % 12
  \barNumberCheck #12
  a'1 )  | % 13
  \barNumberCheck #13
  \stemDown c''4. ( c''8 b'4 \stemUp g'  | % 14
  \barNumberCheck #14
  \stemDown e''4 e'' d'' b' )  | % 15
  \barNumberCheck #15
  \stemUp a'4 ( a' \stemDown b' d''  | % 16
  \barNumberCheck #16
  c''4 c'' d'' f'' )  | % 17
  \barNumberCheck #17
  \break | % 1333333 \myLineBreak
  
  aes''!4 ( ges''!8 [
  aes'' ]
  b'' [
  aes'' e'' b' ]
   | % 18
  \barNumberCheck #18
  d''8 [
  e'' ]
  b'4 d''8 [
  e'' ]
  b'4 )  | % 19
  \barNumberCheck #19
  \stemUp aes'!4 ( ges'!8 [
  aes' ]
  b' [
  aes' e' b ]
   | % 20
  \barNumberCheck #20
  d'1 )  | % 21
  \barNumberCheck #21
  f'4 ( g'8 [
  a' ]
  e'' [
  a' d' a ]
   | % 22
  \barNumberCheck #22
  d'8 [
  e' ]
  a4 d' f' )  | % 23
  \barNumberCheck #23
  \break | % 1333333 \myLineBreak
  
  e'4 ( e' b b )  | % 24
  \barNumberCheck #24
  R1  | % 25
  \barNumberCheck #25
  \stemDown c''4. ( c''8 b'4 \stemUp g'  | % 26
  \barNumberCheck #26
  \stemDown c''4 c'' c'' d'' )  | % 27
  \barNumberCheck #27
  \stemUp a'4. ( a'8 \stemDown b'4 \stemUp g'  | % 28
  \barNumberCheck #28
  a'1 )  | % 29
  \barNumberCheck #29
  \stemDown c''4. ( c''8 b'4 \stemUp g'  | % 30
  \barNumberCheck #30
  \stemDown e''4 e'' d'' b' )  | % 31
  \barNumberCheck #31
  \stemUp a'4 ( a' \stemDown b' d''  | % 32
  \barNumberCheck #32
  \pageBreak | % 14444444
  
  c''4 c'' d'' f'' )  | % 33
  \barNumberCheck #33
  \stemUp aes'!4 ( ges'!8 [
  aes' ]
  b' [
  aes' e' b ]
   | % 34
  \barNumberCheck #34
  d'8 [
  e' ]
  b4 d'8 ^\markup { "2" } [
  e' ]
  b4 )  | % 35
  \barNumberCheck #35
  aes'!4 ( ges'!8 [
  aes' ]
  b' [
  aes' e' b ]
  )  | % 36
  \barNumberCheck #36
  R1  | % 37
  \barNumberCheck #37
  f'4 ( g'8 [
  a' ]
  e'' [
  a' d' a ]
   | % 38
  \barNumberCheck #38
  d'8 [
  e' ]
  a4 d' f' )  | % 39
  \barNumberCheck #39
  \break | % 1333333 \myLineBreak
  
  e'4 ( e' b b )  | % 40
  \barNumberCheck #40
  R1  | % 41
  \barNumberCheck #41
  \stemDown c''4 ( c'' b' \stemUp g'  | % 42
  \barNumberCheck #42
  \stemDown c''4 c'' c'' d'' )  | % 43
  \barNumberCheck #43
  \stemUp a'4. ( a'8 \stemDown b'4 \stemUp g'  | % 44
  \barNumberCheck #44
  a'1 )  | % 45
  \barNumberCheck #45
  \stemDown c''4. ( c''8 b'4 \stemUp g'  | % 46
  \barNumberCheck #46
  \stemDown e''4 e'' d'' b' )  | % 47
  \barNumberCheck #47
  \stemUp a'4 ( a' \stemDown b' d''  | % 48
  \barNumberCheck #48
  \break | % 1333333 \myLineBreak
  
  c''4 c'' d'' f'' )  | % 49
  \barNumberCheck #49
  e''2 r4 e''  | % 50
  \barNumberCheck #50
  \stemUp gis'!2 \stemDown b'  | % 51
  \barNumberCheck #51
  \stemUp a'4 ( g'8 [
  a' ]
  c'' [
  a' e' c' ]
   | % 52
  \barNumberCheck #52
  \stemDown c''4 \stemUp a' e' \stemDown c''  | % 53
  \barNumberCheck #53
  a'1 _\markup { "(With the help of Crescendo and MuseScore software)" } )
  \bar "|."
   | % 1
  \barNumberCheck #54
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key c \major
  \numericTimeSignature \time 4/4
  \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      75
    } % concat
  }
  < e b > 1 -\f -\f -\f < e a > 1  | % 3
  \barNumberCheck #3
  < e b > 1  | % 4
  \barNumberCheck #4
  < d fis! a > 1  | % 5
  \barNumberCheck #5
  < d f! a > 1  | % 6
  \barNumberCheck #6
  < c f a > 1  | % 7
  \barNumberCheck #7
  < b, e gis! > 1  | % 8
  \barNumberCheck #8
  \break | % 1333333 \myLineBreak
  
  < c e a > 1  | % 9
  \barNumberCheck #9
  < c e a > 1  | % 10
  \barNumberCheck #10
  < c e g > 1  | % 11
  \barNumberCheck #11
  < c f a > 1  | % 12
  \barNumberCheck #12
  < c e a > 1  | % 13
  \barNumberCheck #13
  < c e a > 1  | % 14
  \barNumberCheck #14
  < b, e g > 1  | % 15
  \barNumberCheck #15
  \stemDown  < c f a > 2  < d g b >   | % 16
  \barNumberCheck #16
   < c e a > 2  < d f a >   | % 17
  \barNumberCheck #17
  \break | % 1333333 \myLineBreak
  
  < e b > 1  | % 18
  \barNumberCheck #18
  < e a > 1  | % 19
  \barNumberCheck #19
  < e b > 1  | % 20
  \barNumberCheck #20
  < d fis! a > 1  | % 21
  \barNumberCheck #21
  < d f! a > 1  | % 22
  \barNumberCheck #22
  < c f a > 1  | % 23
  \barNumberCheck #23
  \break | % 1333333 \myLineBreak
  
  < b, e gis! > 1  | % 24
  \barNumberCheck #24
  < c e a > 1  | % 25
  \barNumberCheck #25
  < c e a > 1  | % 26
  \barNumberCheck #26
  < c e g > 1  | % 27
  \barNumberCheck #27
  < c f a > 1  | % 28
  \barNumberCheck #28
  < c e a > 1  | % 29
  \barNumberCheck #29
  < c e a > 1  | % 30
  \barNumberCheck #30
  < b, e g > 1  | % 31
  \barNumberCheck #31
   < c f a > 2  < d g b >   | % 32
  \barNumberCheck #32
  \pageBreak | % 14444444
  
   < c e a > 2  < d f a >   | % 33
  \barNumberCheck #33
  < e b > 1  | % 34
  \barNumberCheck #34
  < e a > 1  | % 35
  \barNumberCheck #35
  < e b > 1  | % 36
  \barNumberCheck #36
  < d fis! a > 1  | % 37
  \barNumberCheck #37
  < d f! a > 1  | % 38
  \barNumberCheck #38
  < c f a > 1  | % 39
  \barNumberCheck #39
  \break | % 1333333 \myLineBreak
  
  < b, e gis! > 1  | % 40
  \barNumberCheck #40
  < c e a > 1  | % 41
  \barNumberCheck #41
  < c e a > 1  | % 42
  \barNumberCheck #42
  < c e g > 1  | % 43
  \barNumberCheck #43
  < c f a > 1  | % 44
  \barNumberCheck #44
  < c e a > 1  | % 45
  \barNumberCheck #45
  < c e a > 1  | % 46
  \barNumberCheck #46
  < b, e g > 1  | % 47
  \barNumberCheck #47
   < c f a > 2  < d g b >   | % 48
  \barNumberCheck #48
  \break | % 1333333 \myLineBreak
  
   < c e a > 2  < d f a >   | % 49
  \barNumberCheck #49
  < e a c' > 1  | % 50
  \barNumberCheck #50
  < b, e gis! > 1  | % 51
  \barNumberCheck #51
  < c ~  e ~  a ~  > 1  | % 52
  \barNumberCheck #52
  < c ~  e ~  a ~  > 1  | % 53
  \barNumberCheck #53
  < c e a > 1
  \bar "|."
   | % 1
  \barNumberCheck #54
}

Part_POne_Staff_Three_Voice_Nine = \absolute {
  \language "nederlands"
  
  \clef "bass_8"
  \key c \major
  \numericTimeSignature \time 4/4
  \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      75
    } % concat
  }
  R1 R1  | % 3
  \barNumberCheck #3
  R1  | % 4
  \barNumberCheck #4
  r2 \stemUp  < d,, d, >  -\f -\f -\f  | % 5
  \barNumberCheck #5
  R1  | % 6
  \barNumberCheck #6
  R1  | % 7
  \barNumberCheck #7
  R1  | % 8
  \barNumberCheck #8
  \break | % 1333333 \myLineBreak
  
  r2 a,,  | % 9
  \barNumberCheck #9
  R1  | % 10
  \barNumberCheck #10
  R1  | % 11
  \barNumberCheck #11
  R1  | % 12
  \barNumberCheck #12
  r2 a,,  | % 13
  \barNumberCheck #13
  R1  | % 14
  \barNumberCheck #14
  r2 e,,  | % 15
  \barNumberCheck #15
  \stemDown  < a,, a, > 2 \stemUp  < g,, g, >   | % 16
  \barNumberCheck #16
   < e,, e, > 2  < d,, d, >   | % 17
  \barNumberCheck #17
  \break | % 1333333 \myLineBreak
  
  R1  | % 18
  \barNumberCheck #18
  R1  | % 19
  \barNumberCheck #19
  R1  | % 20
  \barNumberCheck #20
  r2  < d,, d, >   | % 21
  \barNumberCheck #21
  R1  | % 22
  \barNumberCheck #22
  R1  | % 23
  \barNumberCheck #23
  \break | % 1333333 \myLineBreak
  
  r2  < e,, e, >   | % 24
  \barNumberCheck #24
  r2 a,,  | % 25
  \barNumberCheck #25
  R1  | % 26
  \barNumberCheck #26
  R1  | % 27
  \barNumberCheck #27
  R1  | % 28
  \barNumberCheck #28
  r2 a,, _\markup { "1" }  | % 29
  \barNumberCheck #29
  R1  | % 30
  \barNumberCheck #30
  r2 e,,  | % 31
  \barNumberCheck #31
  R1  | % 32
  \barNumberCheck #32
  \pageBreak | % 14444444
  
  R1  | % 33
  \barNumberCheck #33
  r2 e,,  | % 34
  \barNumberCheck #34
  r2 a,,  | % 35
  \barNumberCheck #35
  r2 e,,  | % 36
  \barNumberCheck #36
  r2 d,,  | % 37
  \barNumberCheck #37
  r2 d,,  | % 38
  \barNumberCheck #38
  r2 c,,  | % 39
  \barNumberCheck #39
  \break | % 1333333 \myLineBreak
  
  r2 e,,  | % 40
  \barNumberCheck #40
  r2 a,,  | % 41
  \barNumberCheck #41
  R1  | % 42
  \barNumberCheck #42
  R1  | % 43
  \barNumberCheck #43
  R1  | % 44
  \barNumberCheck #44
  r2 a,,  | % 45
  \barNumberCheck #45
  R1  | % 46
  \barNumberCheck #46
  r2 e,,  | % 47
  \barNumberCheck #47
  R1  | % 48
  \barNumberCheck #48
  \break | % 1333333 \myLineBreak
  
  R1  | % 49
  \barNumberCheck #49
  r2 e,,  | % 50
  \barNumberCheck #50
  r2 e,,  | % 51
  \barNumberCheck #51
  R1  | % 52
  \barNumberCheck #52
  a,,1 ~  _\markup { "Philip Hammond" }  | % 53
  \barNumberCheck #53
  a,,1
  \bar "|."
   | % 54
  \barNumberCheck #54
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
