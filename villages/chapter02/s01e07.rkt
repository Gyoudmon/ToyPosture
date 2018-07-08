#lang racket

(define seed (make-parameter 1024))

(define slips '(A A A A B B B))

(define outcomes
  (for/set ([ls (in-permutations slips)])
    (string->symbol (apply string-append (map symbol->string ls)))))

outcomes
(set-count outcomes) ;;; 7!/(3! * 4!) == C(3, 7) == C(4, 7)
