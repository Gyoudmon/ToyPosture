#lang racket

(define sample-space (make-hasheq))

(define seed (make-parameter 1024))

(define match
  (lambda [p1 p2]
    (if (even? (random (seed))) (values p1 p2) (values p2 p1))))

(define tournament
  (lambda []
    (define-values (r11:w r11:l)  (match     1     2))
    (define-values (r12:w r12:l)  (match     3     4))
    (define-values (r21:w r21:l)  (match r11:w r12:w))
    (define-values (r22:w r22:l)  (match r11:l r12:l))
    (define-values (winner loser) (match r21:w r22:w))
    
    (define outcome (+ (* r21:w 1000) (* r21:l 100) (* r22:w 10) (* r22:l 1)))

    (hash-set! sample-space outcome (add1 (hash-ref sample-space outcome 0)))))


(for ([i (in-range 1000)]) (tournament))

(define S (hash-keys sample-space))
(list (length S) S) ;;; 2 * (2*2 + 2*2)
