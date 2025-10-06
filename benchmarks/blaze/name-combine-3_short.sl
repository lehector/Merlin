(set-logic SLIA)

(synth-fun f ((x String)) String
    (
    (Start String ((Concat ntStringP Start) (Str ntStringP)))
    (ntStringP String ((SubStr argString ntPos ntPos) (ConstStr ntConstString)))
    (ntConstString String (" " "."))
    (ntPos Int ((Pos argString ntToken ntInt ntDir) (ConstPos ntConstPos)))
    (ntConstPos Int (0 1 2 3 4 5 6 7 8 9 10))
    (argString String (x))
    (ntToken String ("ProperCase" "CAPS" "lowercase" "Digits" "Alphabets" "Alphanumeric" "Whitespace" "StartT" "EndT" "ProperCaseWSpaces" "CapsWSpaces" "lowercaseWSpaces" "AlphabetWSpaces" "Sep"))
    (ntInt Int (1 -1 2 -2 3 -3))
    (ntDir Int (0 1))
    )
    )

(declare-var firstname String)
(declare-var lastname String)
(constraint (= (f "Launa|Withers") "L. Withers"))
(constraint (= (f "Lakenya|Edison") "L. Edison"))
(constraint (= (f "Brendan|Hage") "B. Hage"))
(constraint (= (f "Bradford|Lango") "B. Lango"))
(constraint (= (f "Rudolf|Akiyama") "R. Akiyama"))
(constraint (= (f "Lara|Constable") "L. Constable"))

(check-synth)

