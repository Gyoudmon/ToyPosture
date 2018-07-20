#lang scribble/lp2

@(require digimon/tamer)

@;(define-bib TLCAoBLFI
   #:title    "Time Lapse Cinematographic Analysis of Beryllium-Lung Fibroblast Interactions"
   #:author   (authors "Marlene Absher" "David Sylwester" "Beth A. Hart")
   #:date     1983
   #:location (journal-location	"Environment Research" #:pages (list 34 43) #:number 1 #:volume 30)
   #:url      "https://doi.org/10.1016/0013-9351(83)90163-9")

@handbook-story{Modeling Change}

@tamer-smart-summary[]

@handbook-scenario{Modeling Change with Difference Equations}

@margin-note{Project 1.1-1 on Page 9}

@itemlist[
 @item{With the rising price of gas, you wish to buy a new (hybrid) car this year.
  You narrow your choices to the following hybrids: 2007 Toyota Camry hybrid,
  2007 Saturn hybrid, 2007 Honda Civic hybrid, 2007 Nissan Altima hybrid, and
  2007 Mercury Mariner hybrid. Each company has offered you its "best deal" as
  listed below. You are able to allocate approximately $500 for a car payment
  each month for up to 60 months. Use a dynamical system to determine which of
  the new hybrid cars you could buy.}]

@(tabular #:style 'boxed
          #:column-properties '(left center center center)
          #:row-properties '(bottom-border ())
          (list* (list @bold{Hybrid} @bold{Best Deal} @bold{Cash Down} @bold{Interest and Duration})
                 (for/list ([info (in-list (map struct->vector (tamer-story-require 'cars)))])
                   (list (symbol->string (vector-ref info 1))
                         (format "$~a" (vector-ref info 2))
                         (format "$~a" (vector-ref info 3))
                         (format "~a% APR for ~a months"
                           (~r (* (vector-ref info 4) 100.0f0) #:precision '(= 2))
                           (vector-ref info 5))))))

@handbook-reference[]

@; Chunks after `handbook-reference[]` will never be rendered in documents
@; <*> is the main chunk by convention.

@chunk[|<*>|
       (require digimon/tamer)
       (tamer-taming-start!)

       (module+ tamer |<moduling.change:*>|)]

@chunk[|<moduling.change:*>|
       (module+ story
         <do-project1.1-1>)]

@chunk[<do-project1.1-1>
       (require plot)
       
       (struct hybrid (name best down interest duration) #:transparent)
       
       (define cars
         (list (hybrid 'Saturn 22045 1000 0.0595f0 60)
               (hybrid '|Honda Civic| 24350 1500 0.0550f0 60)
               (hybrid '|Toyota Camry| 26200 750 0.0625f0 60)
               (hybrid 'Mariner 27515 1500 0.0600f0 60)
               (hybrid 'Altima 24990 1000 0.0590f0 60)))]
