(set-logic SLIA)

(synth-fun f ((x String)) String
    (
    (Start String ((Concat ntStringP Start) (Str ntStringP)))
    (ntStringP String ((SubStr argString ntPos ntPos) (ConstStr ntConstString)))
    (ntConstString String (" " "-" "+" "."))
    (ntPos Int ((Pos argString ntToken ntInt ntDir) (ConstPos ntConstPos)))
    (ntConstPos Int (0 1 2 3 4 5 6 7 8 9 10))
    (argString String (x))
    (ntToken String ("ProperCase" "CAPS" "lowercase" "Digits" "Alphabets" "Alphanumeric" "Whitespace" "StartT" "EndT" "ProperCaseWSpaces" "CapsWSpaces" "lowercaseWSpaces" "AlphabetWSpaces" "Sep"))
    (ntInt Int (1 -1 2 -2 3 -3))
    (ntDir Int (0 1))
    )
    )

(declare-var name String)
(constraint (= (f "+106 769-858-438") "106.769.858.438"))
(constraint (= (f "+83 973-757-831") "83.973.757.831"))
(constraint (= (f "+62 647-787-775") "62.647.787.775"))
(constraint (= (f "+172 027-507-632") "172.027.507.632"))
(constraint (= (f "+72 001-050-856") "72.001.050.856"))
(constraint (= (f "+95 310-537-401") "95.310.537.401"))
(constraint (= (f "+6 775-969-238") "6.775.969.238"))

(check-synth)

