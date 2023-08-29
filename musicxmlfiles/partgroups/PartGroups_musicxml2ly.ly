\version "2.25.2"
% automatically converted by musicxml2ly from PartGroups_musicxml2ly.xml
\pointAndClickOff

\header {
    title =  "Prelude to a Tragedy"
    copyright =  "© 2004 Polygames.     All Rights Reserved."
    composer =  "Lee Actor (2003)"
    encodingsoftware =  "Finale 2005 for Windows"
    encodingdate =  "2007-06-19"
    }

#(set-global-staff-size 11.416000000000002)
\paper {
    
    paper-width = 27.94\cm
    paper-height = 35.56\cm
    top-margin = 1.27\cm
    bottom-margin = 0.45\cm
    left-margin = 1.77\cm
    right-margin = 0.79\cm
    between-system-space = 0.39\cm
    page-top-space = 0.39\cm
    indent = 2.1492307692307695\cm
    short-indent = 0.6787044534412956\cm
    }
\layout {
    \context { \Score
        skipBars = ##t
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative b'' {
    \clef "treble" \time 3/4 \key c \major \transposition c'' | % 1
    \tempo "" 4=85 R2. | % 2
    \numericTimeSignature\time 4/4  R1*2 | % 4
    \time 3/4  R2. | % 5
    \time 2/4  R2 | % 6
    \numericTimeSignature\time 4/4  R1 | % 7
    \time 3/4  \stemDown b2. ~ -\p | % 8
    \time 2/4  \stemDown b2 ~ }

PartPTwoVoiceOne =  \relative b'' {
    \clef "treble" \time 3/4 \key c \major | % 1
    R2. | % 2
    \numericTimeSignature\time 4/4  R1*2 | % 4
    \time 3/4  R2. | % 5
    \time 2/4  R2 | % 6
    \numericTimeSignature\time 4/4  R1 | % 7
    \time 3/4  \stemDown <b fis'>2. ~ -\p ~ | % 8
    \time 2/4  \stemDown <b fis'>2 _\< -\! }

PartPThreeVoiceOne =  \relative b'' {
    \clef "treble" \time 3/4 \key c \major | % 1
    R2. | % 2
    \numericTimeSignature\time 4/4  R1*2 | % 4
    \time 3/4  R2. | % 5
    \time 2/4  R2 | % 6
    \numericTimeSignature\time 4/4  R1 | % 7
    \time 3/4  \stemDown <b d>2. ~ -\p ~ | % 8
    \time 2/4  \stemDown <b d>2 _\< -\! }

PartPFourVoiceOne =  \relative c' {
    \clef "treble" \time 3/4 \key c \major \transposition f | % 1
    R2. | % 2
    \numericTimeSignature\time 4/4  R1*2 | % 4
    \time 3/4  R2. | % 5
    \time 2/4  R2 | % 6
    \numericTimeSignature\time 4/4  R1 | % 7
    \time 3/4  R2. | % 8
    \time 2/4  R2 }

PartPFiveVoiceOne =  \relative cis''' {
    \clef "treble" \time 3/4 \key c \major \transposition bes | % 1
    R2. | % 2
    \numericTimeSignature\time 4/4  R1*2 | % 4
    \time 3/4  R2. | % 5
    \time 2/4  R2 | % 6
    \numericTimeSignature\time 4/4  R1 | % 7
    \time 3/4  \stemDown <cis e>2. ~ -\p ~ | % 8
    \time 2/4  \stemDown <cis e>2 _\< -\! }

PartPSixVoiceOne =  \relative c' {
    \clef "treble" \time 3/4 \key c \major \transposition bes, | % 1
    R2. | % 2
    \numericTimeSignature\time 4/4  R1*2 | % 4
    \time 3/4  R2. | % 5
    \time 2/4  R2 | % 6
    \numericTimeSignature\time 4/4  R1 | % 7
    \time 3/4  R2. | % 8
    \time 2/4  R2 }

PartPSevenVoiceOne =  \relative c' {
    \clef "bass" \time 3/4 \key c \major | % 1
    R2. | % 2
    \numericTimeSignature\time 4/4  R1*2 | % 4
    \time 3/4  R2. | % 5
    \time 2/4  R2 | % 6
    \numericTimeSignature\time 4/4  R1 | % 7
    \time 3/4  R2. | % 8
    \time 2/4  R2 }


% The score definition
\score {
    <<
        
        \new StaffGroup
        <<
            \new Staff
            <<
                \set Staff.instrumentName = "Piccolo"
                \set Staff.shortInstrumentName = "Picc."
                
                \context Staff << 
                    \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                    \context Voice = "PartPOneVoiceOne" {  \PartPOneVoiceOne }
                    >>
                >>
            \new StaffGroup \with { \consists "Instrument_name_engraver"
                }
            
            <<
                \set StaffGroup.instrumentName = \markup { \center-column { \line {"1"} \line {"2"} } }
                \new Staff
                <<
                    \set Staff.instrumentName = "Flutes"
                    \set Staff.shortInstrumentName = "Fl."
                    
                    \context Staff << 
                        \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                        \context Voice = "PartPTwoVoiceOne" {  \PartPTwoVoiceOne }
                        >>
                    >>
                
                >>
            \new StaffGroup \with { \consists "Instrument_name_engraver"
                }
            
            <<
                \set StaffGroup.instrumentName = \markup { \center-column { \line {"1"} \line {"2"} } }
                \new Staff
                <<
                    \set Staff.instrumentName = "Oboes"
                    \set Staff.shortInstrumentName = "Ob."
                    
                    \context Staff << 
                        \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                        \context Voice = "PartPThreeVoiceOne" {  \PartPThreeVoiceOne }
                        >>
                    >>
                
                >>
            \new Staff
            <<
                \set Staff.instrumentName = "English Horn"
                \set Staff.shortInstrumentName = "E. H."
                
                \context Staff << 
                    \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                    \context Voice = "PartPFourVoiceOne" {  \PartPFourVoiceOne }
                    >>
                >>
            \new StaffGroup \with { \consists "Instrument_name_engraver"
                }
            
            <<
                \set StaffGroup.instrumentName = \markup { \center-column { \line {"1"} \line {"2"} } }
                \new Staff
                <<
                    \set Staff.instrumentName = "Clarinets in Bb"
                    \set Staff.shortInstrumentName = "Cl."
                    
                    \context Staff << 
                        \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                        \context Voice = "PartPFiveVoiceOne" {  \PartPFiveVoiceOne }
                        >>
                    >>
                
                >>
            \new Staff
            <<
                \set Staff.instrumentName = "Bass Clarinet in Bb"
                \set Staff.shortInstrumentName = "B. Cl."
                
                \context Staff << 
                    \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                    \context Voice = "PartPSixVoiceOne" {  \PartPSixVoiceOne }
                    >>
                >>
            \new StaffGroup \with { \consists "Instrument_name_engraver"
                }
            
            <<
                \set StaffGroup.instrumentName = \markup { \center-column { \line {"1"} \line {"2"} } }
                \new Staff
                <<
                    \set Staff.instrumentName = "Bassoons"
                    \set Staff.shortInstrumentName = "Bsn."
                    
                    \context Staff << 
                        \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                        \context Voice = "PartPSevenVoiceOne" {  \PartPSevenVoiceOne }
                        >>
                    >>
                
                >>
            
            >>
        
        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 85 }
    }

