#lang racket

(require plot)

(define data
  (list  11  14  20  23  31  36  39  44  47  50
         59  61  65  67  68  71  74  76  78  79
         81  84  85  89  91  93  96  99 101 104
        105 105 112 118 123 136 139 141 148 158
        161 168 184 206 248 263 289 322 388 513))

(define interval-id (λ [n] (add1 (quotient n 50))))

(define intervals (make-hasheq))
(define lntervals (make-hasheq))

(for ([datum (in-list data)])
  (define key (interval-id datum))
  (define lny (exact-ceiling (* (log datum) 10)))
  
  (hash-set! intervals key (add1 (hash-ref intervals key 0)))
  (hash-set! lntervals lny (add1 (hash-ref lntervals lny 0))))

(define holes (map (λ [key] (vector (* key 50) (hash-ref intervals key 0))) (range (interval-id (car data)) (add1 (interval-id (last data))))))
(define lnles (map (λ [key] (vector (* key 0.1) (hash-ref lntervals key))) (sort (hash-keys lntervals) <)))

(plot #:width 800
      (list (discrete-histogram holes #:label "Holes" #:color 1 #:line-color 1)
            (discrete-histogram lnles #:label "lnHoles" #:color 2 #:line-color 2 #:x-min (add1 (length holes)))))
