(set-logic SLIA)

(synth-fun f ((arg String)) String
    ((Start String (ntString))
    (ntString String (arg "" " Conference" " City" (str.++ ntString ntString) (str.replace ntString ntString ntString)))
    )
    )

;; must stay substrings but not reachable for analysis
(constraint (= (f "POPL Conference") "POP"))
(constraint (= (f "Rennes City") "Renne"))
(constraint (= (f "PLDI Conference") "PLD"))
(constraint (= (f "Seoul City") "Seou"))

(check-synth)