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
  (lambda content
    (tabular #:sep (hspace 4)
             #:column-properties '(left vcenter)
             (for/list ([row (in-list content)])
               (list* (tt (car row))
                      (for/list ([c (in-list (cdr row))])
                        (cond [(block? c) c]
                              [else (make-compound-paragraph placeholder-style
                                                             (list (para c)))])))))))
