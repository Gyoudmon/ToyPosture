#lang typed/racket/base

(require math/distributions)

'ex3.4.46
(define ex3.4.46 (binomial-dist 9 0.1))
(cdf ex3.4.46 1)

(newline)
'ex3.4.47
(define ex3.4.47.m (binomial-dist 15 0.3))
(- (cdf ex3.4.47.m 6) (cdf ex3.4.47.m 5))

(define ex3.4.47 : (-> Natural Natural Flonum)
  (lambda [n x]
    (pdf (binomial-dist n 0.1) x)))

(ex3.4.47 6 1)
(- 1.0 (+ (ex3.4.47 6 0) 0.354))
(+ (ex3.4.47 4 0)
   (* (ex3.4.47 4 1)
      0.9))

(newline)
'ex3.4.52
(define ex3.4.52.c : (-> Fixnum Flonum Fixnum (Pairof Fixnum Flonum))
  (lambda [n p x]
    (cons x
          (* (pdf (binomial-dist n p) x)
             (* (real->double-flonum (- n x))
                (- 1.0 p))))))

(ex3.4.52.c 25 0.3 10)
(ex3.4.52.c 25 0.3 11)
(ex3.4.52.c 25 0.3 12)
(ex3.4.52.c 25 0.3 13)
(ex3.4.52.c 25 0.3 14)
(ex3.4.52.c 25 0.3 15)
