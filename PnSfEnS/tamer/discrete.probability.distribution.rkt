#lang scribble/lp2

@(require digimon/tamer)

@(define-bib HtROC
   #:title    "How to Read an OC Curve"
   #:author   (authors "Fred Schenkelberg")
   #:date     2019
   #:url      "https://accendoreliability.com/how-to-read-an-oc-curve/")

@handbook-story{Discrete Random Variables and Probability Distribution}

@tamer-smart-summary[]

@handbook-scenario{The Binomial Probability Distribution}

@margin-note{Exercise 124 on Page 58}
@margin-note{Also see @~cite[HtROC]}

A very large batch of components has arrived at a distributor. The batch can be characterized as acceptable
only if the proportion of defective components is at most @math{10%}. The distributor decides to randomly
select @math{10} components and accept the batch only if the number of defective components in the sample
is at most @math{2}.

@itemlist[#:style 'ordered
          
          @item{What is the probability that the batch will be accepted when the actual proportion of defective is
           @math{1%}? @math{5%}? @math{10%}? @math{20%}? @math{25%}?}

          @item{Let @math{p} denote the actual proportion of defective in the batch. A graph of @math{P}(batch is accepted)
           as a function of @math{p}, with @math{p} on the horizontal axis and @math{P}(batch is accepted) on the
           vertical axis, is called the @italic{operating characteristics curve} for the acceptance sampling plan.
           Use the results of part(1) to sketch this curve for @math{0 <= p <= 1}.}
 
          @item{Repeat parts (1) and (2) with @math{1} replacing @math{2} in the acceptance sampling plan.}
          @item{Repeat parts (1) and (2) with @math{15} replacing @math{10} in the acceptance sampling plan.}
          @item{Which of the three sampling plans, that of part (1), (3), or (4), appears most satisfactory, and why? }
]

@tamer-action[
 (define B (λ [x n p] (cdf (binomial-dist n p) x)))

 (for ([p (in-list (list 0.01 0.05 0.10 0.20 0.25))])
   (printf "B(~a; ~a, ~a) = ~a~n" 2 10 (~r p #:precision '(= 2))
           (B 2 10 p)))
 
 (plot #:title "operating characteristics curve"
       #:legend-anchor 'top-right #:x-label "Lot Percent Defective" #:y-label "Probability of Accept the Lot"
       (list (function (curry B 2 10) 0.0 1.0 #:y-min 0.0 #:y-max 1.0 #:color 1 #:label "B(2; 10, p)")
             (function (curry B 1 10) 0.0 1.0 #:y-min 0.0 #:y-max 1.0 #:color 2 #:label "B(1; 10, p)")
             (function (curry B 2 15) 0.0 1.0 #:y-min 0.0 #:y-max 1.0 #:color 3 #:label "B(2; 15, p)")
             (vrule 0.1 #:style 'long-dash #:color 4)))

 (code:comment "As the plot above shown, both of the three sample plans perform well:")
 (code:comment "Good lots are probably accepted while poor lots probably fail.")
 (code:comment "Procedures may favor the 1st plan since more poor lots may still pass;")
 (code:comment "Consumers may favor the 3rd one since the accepted batch tends to a higher quality.")]

@handbook-reference[]

@; Chunks after `handbook-reference[]` will never be rendered in documents
@; <*> is the main chunk by convention.

@chunk[|<*>|
       (require digimon/tamer)
       (tamer-taming-start!)

       (module+ tamer |<discrete.random.variables.n.probability.distribution:*>|)]

@chunk[|<discrete.random.variables.n.probability.distribution:*>|
       <do-s04e58>]

@chunk[<do-s04e58>
       (require plot/pict)
       (require math/statistics)
       (require math/distributions)]
