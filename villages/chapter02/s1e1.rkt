#lang racket

(define seed (make-parameter 1024))

(define match
  (lambda [p1 p2]
    (if (even? (random (seed))) (values p1 p2) (values p2 p1))))

(define tournament
  (lambda [S]
    (define-values (r11:w r11:l)  (match     1     2))
    (define-values (r12:w r12:l)  (match     3     4))
    (define-values (r21:w r21:l)  (match r11:w r12:w))
    (define-values (r22:w r22:l)  (match r11:l r12:l))
    
    (set-add S (+ (* r21:w 1000) (* r21:l 100)
                  (* r22:w 10) (* r22:l 1)))))


(define S
  (for/fold ([S (seteq)])
            ([i (in-range 1000)])
    (tournament S)))

(set-count S) ;;; 2 * (2*2 + 2*2)
(define A (list->seteq (filter (λ [s] (< s 2000)) (set->list S))))
(define B (list->seteq (filter (λ [s] (or (< 2000 s 3000) (< 200 (remainder s 1000) 300))) (set->list S))))
A
B
(set-union A B)
(set-intersect A B)
(set-subtract S A)