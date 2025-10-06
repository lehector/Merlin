(set-logic SLIA)


(synth-fun fun ((x String)) String
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

(declare-var x String)
(constraint (= (fun "938-242-504") "504"))
(constraint (= (fun "308-916-545") "545"))
(constraint (= (fun "623-599-749") "749"))
(constraint (= (fun "981-424-843") "843"))
(constraint (= (fun "118-980-214") "214"))
(constraint (= (fun "244-655-094") "094"))

(check-synth)

