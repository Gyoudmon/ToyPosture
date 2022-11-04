#lang racket

(require racket/flonum)

(require plot)

(define data
  (list (list 'FC 126 '|failed component|)
        (list 'IC 210 '|incorrect component|)
        (list 'IS  67 '|insufficient solder|)
        (list 'ES  54 '|excess solder|)
        (list 'MC 131 '|missing component|)))

(define y-max
  (+ (cadr (argmin cadr data))
     (cadr (argmax cadr data))))

(define fltotal
  (real->double-flonum
   (for/sum ([datum (in-list data)])
     (cadr datum))))

(define categories
  (map (λ [datum] (vector (car datum) (cadr datum)))
       (sort data > #:key cadr)))

(define cumulative%
  (let acc ([fs null]
            [cs categories])
    (cond [(null? cs) (reverse fs)]
          [else (let ([f (fl/ (real->double-flonum (vector-ref (car cs) 1)) fltotal)])
                  (acc (cons (fl+ f (if (null? fs) 0.0 (car fs))) fs)
                       (cdr cs)))])))

(plot #:y-max y-max
      (list (discrete-histogram categories)
            (lines (for/list ([% (in-list cumulative%)]
                              [i (in-naturals)])
                     (vector (+ i 0.5) (* % y-max))))))
