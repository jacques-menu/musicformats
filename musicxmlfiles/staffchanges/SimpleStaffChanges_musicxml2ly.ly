\version "2.25.2"
% automatically converted by musicxml2ly from SimpleStaffChanges_musicxml2ly.xml
\pointAndClickOff

\header {
    title =  "Simple Staff Changes"
    encodingsoftware =  "MuseScore 4.0.2"
    encodingdate =  "2023-04-03"
    }

\layout {
    \context { \Score
        skipBars = ##t
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative a {
    \clef "bass" \time 2/4 \key c \major | % 1
    \stemUp a8 [ \change Staff="1" \stemDown e'8 \change Staff="2"
    \stemUp f,8 \change Staff="1" \stemDown g'8 ] | % 2
    \stemDown c8 [ \stemDown e,8 \change Staff="2" \stemUp a,8 \stemUp b8
    ] \bar "|."
    }

PartPOneVoiceFive =  \relative c' {
    \clef "bass" \time 2/4 \key c \major | % 1
    R2*2 \bar "|."
    }


% The score definition
\score {
    <<
        
        \new PianoStaff
        <<
            
            \context Staff = "1" << 
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                >> \context Staff = "2" <<
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \voiceOne \PartPOneVoiceOne }
                \context Voice = "PartPOneVoiceFive" {  \voiceTwo \PartPOneVoiceFive }
                >>
            >>
        
        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 100 }
    }

