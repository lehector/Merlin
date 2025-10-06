(set-logic SLIA)

(synth-fun fun ((x String)) String
    (
    (Start String ((Concat ntStringP Start) (Str ntStringP)))
    (ntStringP String ((SubStr argString ntPos ntPos) (ConstStr ntConstString)))
    (ntConstString String (" " "," "USA" "PA" "CT" "CA" "MD" "NY"))
    (ntPos Int ((Pos argString ntToken ntInt ntDir) (ConstPos ntConstPos)))
    (ntConstPos Int (0 1 2 3 4 5 6 7 8 9 10))
    (argString String (x))
    (ntToken String ("ProperCase" "CAPS" "lowercase" "Digits" "Alphabets" "Alphanumeric" "Whitespace" "StartT" "EndT" "ProperCaseWSpaces" "CapsWSpaces" "lowercaseWSpaces" "AlphabetWSpaces" "Sep"))
    (ntInt Int (1 -1 2 -2 3 -3))
    (ntDir Int (0 1))
    )
    )

(declare-var x String)
(constraint (= (fun "University of Pennsylvania|Phialdelphia, PA, USA") "Phialdelphia, PA, USA"))
(constraint (= (fun "UCLA|Los Angeles, CA") "Los Angeles, CA, USA"))
(constraint (= (fun "Cornell University|Ithaca, New York, USA") "Ithaca, New York, USA"))
(constraint (= (fun "Penn|Philadelphia, PA, USA") "Philadelphia, PA, USA"))
(constraint (= (fun "University of Maryland College Park|College Park, MD") "College Park, MD, USA"))
(constraint (= (fun "University of Michigan|Ann Arbor, MI, USA") "Ann Arbor, MI, USA"))

(check-synth)

