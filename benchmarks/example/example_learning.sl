(set-logic SLIA)

(synth-fun f ((arg String)) String
    ((Start String (ntString))
    (ntString String (arg "" " Conference" " City" (str.++ ntString ntString) (str.replace ntString ntString ntString)))
    )
    )

(declare-var name String)
(constraint (= (f "POPL Conference") "POPL"))
(constraint (= (f "Rennes City") "Renne"))
(constraint (= (f "PLDI Conference") "PLDI"))
(constraint (= (f "Seoul City") "Seou"))

(check-synth)