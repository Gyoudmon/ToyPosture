#lang racket/base

(provide (all-defined-out))
(provide (all-from-out digimon/tamer))

(require digimon/tamer)
(require digimon/collection)

(require scribble/core)
(require scribble/manual)

(require (except-in pict table))
(require racket/draw)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Just in case for README.md
(enter-digimon-zone!)

(tamer-indexed-block-hide-chapter-index #true)

(tamer-block-label-separator #false)
(tamer-block-label-tail ". ")
(tamer-block-label-style 'bold)

(tamer-default-figure-label "图")
(tamer-default-code-label "段")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define filesystem-value->pict
  (let ([substyle '(subscript large-script)]
        [ftext (lambda [t [s null] [c #false]]
                 (define color (send the-color-database find-color (~a c)))
                 (text (~a t) (if color (cons color s) s) 12))])
    (letrec ([value->pict (match-lambda
                            [(or (box filename) (vector filename))
                             (ftext filename '(italic))]
                            [(or (cons filename info) (vector filename info))
                             (hc-append 4 (value->pict filename) (ftext info substyle 'ForestGreen))]
                            [(vector filename info color)
                             (ht-append 4 (value->pict filename) (ftext info substyle color))]
                            [val (ftext val)])])
      value->pict)))

(define filesystem-tree
  (let ([1em (pict-height (text ""))] [phantom (blank 0)])
    (lambda [tree #:value-pict [value->pict filesystem-value->pict]
                  #:padding-space [gapsize 8] #:padding-x [offset 12] #:padding-box [delta 8]
                  #:dir-color [cdir 'LightSkyBlue] #:file-color [cfile 'Ghostwhite] #:line-color [cline 'Gainsboro]]
      (define yoffset (* 0.5 (+ delta 1em)))
      (define xy-find (λ [p f] (let-values ([(x y) (lt-find p f)]) (values (+ x offset) (+ y yoffset)))))
      (define (leaf->pict v color)
        (define content (value->pict v))
        ((curryr cc-superimpose content)
         (filled-rounded-rectangle #:border-color (~a cline) #:color (~a color)
                                   (+ delta delta (pict-width content))
                                   (+ delta (pict-height content)))))
      (define (branch->pict nodes)
        (for/fold ([root (leaf->pict (car nodes) cdir)])
                  ([subtree (in-list (cdr nodes))])
          (define body (filesystem-tree subtree #:value-pict value->pict
                                        #:padding-space gapsize #:padding-x offset #:padding-box delta
                                        #:dir-color cdir #:file-color cfile #:line-color cline))
          (define child (pin-line #:under? #true #:color (~a cline)
                                  (ht-append (* 4 gapsize) phantom body)
                                  phantom xy-find body xy-find))
          (pin-line #:under? #true #:color (~a cline)
                    (vl-append gapsize root child)
                    root xy-find child xy-find)))
      (cond [(null? tree) phantom]
            [(list? tree) (branch->pict tree)]
            [else (leaf->pict tree cfile)]))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define tamer-c++
  (lambda [id caption subpath start [end #px"END"] [exlastline? #true]]
    (tamer-code-here #:language "C++" #:exclude-lastline? exlastline?
                     id caption
                     (build-path "digitama" "IMS" subpath)
                     start end)))

(define tamer-c++-class
  (lambda [id caption subpath]
    (tamer-code-here #:language "C++" #:exclude-lastline? #false
                     (format "cpp:~a" id) caption
                     (build-path "digitama" "IMS" subpath)
                     (pregexp (format "class\\s+~a\\s+(:[^{]+)?[{]" id))
                     #px"[}];")))


(define tamer-c++-function
  (lambda [id caption subpath #:ns [ns 'WarGrey::IMS] #:subpattern [subpattern #false]]
    (tamer-code-here #:language "C++" #:exclude-lastline? #false
                     (format "cpp:~a" id) caption
                     (build-path "digitama" "IMS" subpath)
                     (cons (pregexp (format "~a::~a\\s*[(][^)]*[)]\\s*[{]" ns id))
                           subpattern)
                     #px"^[}]")))

(define pk
  (lambda [attr]
    (elem #:style "underline" attr)))

(define cpp:function
  (lambda body
    (apply racketidfont body)))

(define cpp:variable
  (lambda body
    (apply racketvarfont body)))

(define cpp:type
  (lambda body
    (apply racketvalfont body)))