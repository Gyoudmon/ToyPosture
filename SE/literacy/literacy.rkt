#lang racket/base

(provide (all-defined-out))
(provide (all-from-out digimon/tamer))

(require digimon/tamer)
(require digimon/collection)

(require scribble/core)
(require scribble/manual)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Just in case for README.md
(enter-digimon-zone!)

(tamer-indexed-block-hide-chapter-index #true)

(tamer-default-figure-label "图")
(tamer-default-figure-label-separator #false)
(tamer-default-figure-label-tail ". ")
(tamer-default-figure-label-style 'bold)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define uc-exmaple
  (let ([sep (hspace 4)])
    (lambda content
      (tabular #:sep #false
               #:style 'block
               #:column-properties '(left vcenter)
               (for/list ([row (in-list content)])
                 (list* (list (tt (car row)) sep)
                        (for/list ([c (in-list (cdr row))])
                          (cond [(block? c) c]
                                [else (make-compound-paragraph placeholder-style
                                                               (list (para c)))]))))))))

(define ac-table
  (let ([sep (hspace 1)])
    (lambda content
      (tabular #:sep #false
               #:style 'block
               #:column-properties '(center)
               #:row-properties '(bottom-border ())
               (cons (list (bold "潜在的分析类") (bold "类型") (bold "满足的特征") (bold "分析类名"))
                     (for/list ([row (in-list content)])
                       (for/list ([c (in-list row)])
                         (list sep c sep))))))))

(define pk
  (lambda [attr]
    (elem #:style "underline" attr)))
