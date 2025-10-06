(set-logic SLIA)

(synth-fun f ((x String)) String
    (
    (Start String ((Concat ntStringP Start) (Str ntStringP)))
    (ntStringP String ((SubStr argString ntPos ntPos) (ConstStr ntConstString)))
    (ntConstString String ("," " " "."))
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
(constraint (= (f "Launa|Withers") "Withers, L."))
(constraint (= (f "Lakenya|Edison") "Edison, L."))
(constraint (= (f "Brendan|Hage") "Hage, B."))
(constraint (= (f "Bradford|Lango") "Lango, B."))
(constraint (= (f "Rudolf|Akiyama") "Akiyama, R."))

(check-synth)

