(set-logic SLIA)

(synth-fun f ((x String)) String
    (
    (Start String ((Concat ntStringP Start) (Str ntStringP)))
    (ntStringP String ((SubStr argString ntPos ntPos) (ConstStr ntConstString)))
    (ntConstString String (" " ","))
    (ntPos Int ((Pos argString ntToken ntInt ntDir) (ConstPos ntConstPos)))
    (ntConstPos Int (0 1 2 3 4 5 6 7 8 9 10))
    (argString String (x))
    (ntToken String ("ProperCase" "CAPS" "lowercase" "Digits" "Alphabets" "Alphanumeric" "Whitespace" "StartT" "EndT" "ProperCaseWSpaces" "CapsWSpaces" "lowercaseWSpaces" "AlphabetWSpaces" "Sep"))
    (ntInt Int (1 -1 2 -2 3 -3))
    (ntDir Int (0 1))
    )
    )

(declare-var col1 String)
(declare-var col2 String)
(constraint (= (f "University of Pennsylvania|Phialdelphia, PA, USA") "University of Pennsylvania, Phialdelphia, PA, USA"))
(constraint (= (f "Cornell University|Ithaca, New York, USA") "Cornell University, Ithaca, New York, USA"))
(constraint (= (f "University of Maryland College Park|College Park, MD") "University of Maryland College Park, College Park, MD"))
(constraint (= (f "University of Michigan|Ann Arbor, MI, USA") "University of Michigan, Ann Arbor, MI, USA"))
(constraint (= (f "Yale University|New Haven, CT, USA") "Yale University, New Haven, CT, USA"))
(constraint (= (f "Columbia University|New York, NY, USA") "Columbia University, New York, NY, USA"))

(check-synth)

