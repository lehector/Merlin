(set-logic SLIA)

(synth-fun fun ((x String)) String
    (
    (Start String ((Concat ntStringP Start) (Str ntStringP)))
    (ntStringP String ((SubStr argString ntPos ntPos) (ConstStr ntConstString)))
    (ntConstString String (" " "+" "-" "." "(" ")"))
    (ntPos Int ((Pos argString ntToken ntInt ntDir) (ConstPos ntConstPos)))
    (ntConstPos Int (0 1 2 3 4 5 6 7 8 9 10))
    (argString String (x))
    (ntToken String ("ProperCase" "CAPS" "lowercase" "Digits" "Alphabets" "Alphanumeric" "Whitespace" "StartT" "EndT" "ProperCaseWSpaces" "CapsWSpaces" "lowercaseWSpaces" "AlphabetWSpaces" "Sep"))
    (ntInt Int (1 -1 2 -2 3 -3))
    (ntDir Int (0 1))
    )
    )

(declare-var x String)
(constraint (= (fun "+106 769-858-438") "+106 (769) 858-438"))
(constraint (= (fun "+83 973-757-831") "+83 (973) 757-831"))
(constraint (= (fun "+62 647-787-775") "+62 (647) 787-775"))
(constraint (= (fun "+172 027-507-632") "+172 (027) 507-632"))
(constraint (= (fun "+72 001-050-856") "+72 (001) 050-856"))
(constraint (= (fun "+95 310-537-401") "+95 (310) 537-401"))
(constraint (= (fun "+6 775-969-238") "+6 (775) 969-238"))

(check-synth)

