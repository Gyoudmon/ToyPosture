#lang racket

(require plot)

(define data
  (list (cons 28.10 1.45) (cons 31.20 1.49) (cons 13.70 1.14)
        (cons 46.00 1.66) (cons 25.80 1.41) (cons 16.80 1.23)
        (cons 34.80 1.54) (cons 62.30 1.79) (cons 28.00 1.45)
        (cons 17.90 1.25) (cons 19.50 1.29) (cons 21.10 1.32)
        (cons 31.90 1.50) (cons 28.90 1.46) (cons 60.10 1.78)
        (cons 23.70 1.37) (cons 18.60 1.27) (cons 21.40 1.33)
        (cons 26.60 1.42) (cons 26.20 1.42) (cons 32.00 1.51)
        (cons 43.50 1.64) (cons 17.40 1.24) (cons 38.80 1.59)
        (cons 30.60 1.49) (cons 55.60 1.75) (cons 25.50 1.41)
        (cons 52.10 1.72) (cons 21.00 1.32) (cons 22.30 1.35)
        (cons 15.50 1.19) (cons 36.30 1.56) (cons 19.10 1.28)
        (cons 38.40 1.58) (cons 72.80 1.86) (cons 48.90 1.69)
        (cons 21.40 1.33) (cons 20.70 1.32) (cons 57.30 1.76)
        (cons 40.90 1.61)))

(define IDTs (make-hasheq))
(define lIDTs (make-hasheq))

(for ([datum (in-list data)])
  (define kIDT (exact-ceiling (* (car datum) 0.1)))
  (define klIDT (exact-ceiling (* (cdr datum) 10)))

  (hash-set! IDTs kIDT (add1 (hash-ref IDTs kIDT 0)))
  (hash-set! lIDTs klIDT (add1 (hash-ref lIDTs klIDT 0))))

(define IDTv (map (λ [key] (vector key (hash-ref IDTs key))) (sort (hash-keys IDTs) <)))
(define lIDTv (map (λ [key] (vector (* key 0.1) (hash-ref lIDTs key))) (sort (hash-keys lIDTs) <)))

(plot (list (discrete-histogram IDTv #:label "IDT" #:color 1 #:line-color 1)
            (discrete-histogram lIDTv #:label "log10IDT" #:color 2 #:line-color 2 #:x-min (add1 (length IDTv)))))
