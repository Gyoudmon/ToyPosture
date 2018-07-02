#lang racket

(require plot)

(define make-smoothing
  (lambda [alpha]
    (lambda [x:t xbar:t-1]
      (+ (* alpha x:t)
         (* (- 1 alpha)
            xbar:t-1)))))

(define make-smoothed-metrics
  (lambda [alpha xts]
    (define smoothing (make-smoothing alpha))
    (define metrics
      (let calculate ([xbars (list (car xts))]
                      [xts (cdr xts)])
        (cond [(null? xts) (reverse xbars)]
              [else (calculate (cons (smoothing (car xts) (car xbars)) xbars)
                               (cdr xts))])))

    (for/list ([xt (in-list metrics)]
               [t (in-naturals)])
      (vector t xt))))

(define xts (list 47 54 53 50 46 46 47 50 51 50 46 52 50 50))

(define raw-metrics (make-smoothed-metrics 1.0 xts))
(define metrics:1 (make-smoothed-metrics 0.1 xts))
(define metrics:5 (make-smoothed-metrics 0.5 xts))

(plot #:y-min (- (apply min xts) 2)
      #:y-max (+ (apply max xts) 2)
      (list (lines raw-metrics #:color 0 #:label "Temperature(°F)")
            (lines metrics:1 #:color 1 #:label "α = 0.1")
            (lines metrics:5 #:color 5 #:label "α = 0.5")))


;;; Xst = αΧt + [(1-α)^k]αX_t-k + [(1-α)^(t-k-1)]Xst_(t-k-1)
;;; lim(Xs1) = 0
