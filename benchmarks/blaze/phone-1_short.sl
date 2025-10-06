(set-logic SLIA)

(synth-fun f ((x String)) String
    (
    (Start String ((Concat ntStringP Start) (Str ntStringP)))
    (ntStringP String ((SubStr argString ntPos ntPos) (ConstStr ntConstString)))
    (ntConstString String (" "))
    (ntPos Int ((Pos argString ntToken ntInt ntDir) (ConstPos ntConstPos)))
    (ntConstPos Int (0 1 2 3 4 5 6 7 8 9 10))
    (argString String (x))
    (ntToken String ("ProperCase" "CAPS" "lowercase" "Digits" "Alphabets" "Alphanumeric" "Whitespace" "StartT" "EndT" "ProperCaseWSpaces" "CapsWSpaces" "lowercaseWSpaces" "AlphabetWSpaces" "Sep"))
    (ntInt Int (1 -1 2 -2 3 -3))
    (ntDir Int (0 1))
    )
    )

(declare-var name String)
(constraint (= (f "938-242-504") "242"))
(constraint (= (f "308-916-545") "916"))
(constraint (= (f "623-599-749") "599"))
(constraint (= (f "981-424-843") "424"))
(constraint (= (f "118-980-214") "980"))
(constraint (= (f "244-655-094") "655"))

(check-synth)

