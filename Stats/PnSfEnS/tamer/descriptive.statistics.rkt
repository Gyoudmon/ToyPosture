#lang scribble/lp2

@(require digimon/tamer)

@(define-bib TLCAoBLFI
   #:title    "Time Lapse Cinematographic Analysis of Beryllium-Lung Fibroblast Interactions"
   #:author   (authors "Marlene Absher" "David Sylwester" "Beth A. Hart")
   #:date     1983
   #:location (journal-location	"Environment Research" #:pages (list 34 43) #:number 1 #:volume 30)
   #:url      "https://doi.org/10.1016/0013-9351(83)90163-9")

@handbook-story{Overview and Discriptive Statistics}

@tamer-smart-summary[]

@handbook-scenario{Pictorial and Tabular Methods}

@margin-note{Exercise 25 on Page 27}

@itemlist[
 @item{A @deftech{transformation} of data values by means of some mathematical function, such as
  @racket[(sqrt x)] or @racketvalfont{1/x}, can often yield a set of numbers that has "nicer" statistical properties
  than the original data. In particular it may be possible to find a function for which the histogram
  of transformed values is more symmetric(or, even better, more like a bell-shaped curve) than the
  original data.}]

As an example, the article @~cite[TLCAoBLFI] reported the results of experiments designed to study the
behavior of certain individual cells that had been exposed to beryllium. An important characteristics of
such an individual cell is its inter division time(IDT). IDTs were determinded for a large number of cells,
both in exposed (treatment) and unexposed (control) conditions. The authors of the article used a logarithmic
transformation, that is, @math{transformed value = log(original value)}. Consider the following representative
IDT data:

@(tabular #:style 'boxed
          #:column-properties '(center center center center center center)
          #:row-properties '(bottom-border ())
          (let ([IDT @bold{IDT}]
                [lIDT @bold{log@subscript{10}(IDT)}]
                [~num (λ [fl] (~r fl #:precision '(= 2)))])
            (list* (list IDT lIDT IDT lIDT IDT lIDT)
                   (let ([metrics (tamer-require 'data)])
                     (append (for/list ([i (in-range 0 12)])
                               (let ([c1 (list-ref metrics (+ i 00))]
                                     [c2 (list-ref metrics (+ i 14))]
                                     [c3 (list-ref metrics (+ i 28))])
                                 (list (~num (car c1)) (~num (cdr c1))
                                       (~num (car c2)) (~num (cdr c2))
                                       (~num (car c3)) (~num (cdr c3)))))
                             (let ([c13.1 (list-ref metrics (+ 12 00))]
                                   [c13.2 (list-ref metrics (+ 12 14))]
                                   [c14.1 (list-ref metrics (+ 13 00))]
                                   [c14.2 (list-ref metrics (+ 13 14))])
                               (list (list (~num (car c13.1)) (~num (cdr c13.1))
                                           (~num (car c13.2)) (~num (cdr c13.2))
                                           "" "")
                                     (list (~num (car c14.1)) (~num (cdr c14.1))
                                           (~num (car c14.2)) (~num (cdr c14.2))
                                           "" ""))))))))

Use class intervals @racketvalfont{10-<20}, @racketvalfont{20-<30},... to construct a histogram of the original data.
Use intervals @racketvalfont{1.1-<1.2}, @racketvalfont{1.2-<1.3},... to do the same for transformed data. What is the effect
of the transformation?

@tamer-answer[
 (plot #:legend-anchor 'top-right
       (list (discrete-histogram IDTv #:label "IDT" #:color 1 #:line-color 1)
             (discrete-histogram lIDTv #:label "log10IDT" #:color 2 #:line-color 2 #:x-min (add1 (length IDTv)))))]

@handbook-reference[#:auto-hide? #false]

@; Chunks after `handbook-reference[]` will never be rendered in documents
@; <*> is the main chunk by convention.

@chunk[|<*>|
       (require digimon/tamer)
       (tamer-taming-start!)

       (module+ tamer |<descriptive.statistics:*>|)]

@chunk[|<descriptive.statistics:*>|
       <do-s02e25>]

@chunk[<do-s02e25>
       (require plot/pict)
       
       (define IDTs (make-hasheq))
       (define lIDTs (make-hasheq))
       
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

       (for ([datum (in-list data)])
         (define kIDT (exact-ceiling (* (car datum) 0.1)))
         (define klIDT (exact-ceiling (* (cdr datum) 10)))
         
         (hash-set! IDTs kIDT (add1 (hash-ref IDTs kIDT 0)))
         (hash-set! lIDTs klIDT (add1 (hash-ref lIDTs klIDT 0))))

       (define IDTv (map (λ [key] (vector key (hash-ref IDTs key))) (sort (hash-keys IDTs) <)))
       (define lIDTv (map (λ [key] (vector (* key 0.1) (hash-ref lIDTs key))) (sort (hash-keys lIDTs) <)))]
