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

@tamer-action[
 (define B (λ [x n p] (cdf (binomial-dist n p) x)))
 (define (oc-curve x n #:color c)
   (function #:y-min 0.0 #:y-max 1.0 #:color c #:label (format "B(~a; ~a, p)" x n)
             (curry B x n) 0.0 1.0 ))]

@margin-note{Exercise 58 on Page 124}
@margin-note{Also see @~cite[HtROC]}

@itemlist[
 @item{A very large batch of components has arrived at a distributor. The batch can be characterized as acceptable
  only if the proportion of defective components is at most @math{10%}. The distributor decides to randomly
  select @math{10} components and accept the batch only if the number of defective components in the sample
  is at most @math{2}.

  @itemlist[#:style 'ordered
            
            @item{What is the probability that the batch will be accepted when the actual proportion of defective is
               @math{1%}? @math{5%}? @math{10%}? @math{20%}? @math{25%}?}
            
            @item{Let @math{p} denote the actual proportion of defective in the batch. A graph of @math{P}(batch is accepted)
               as a function of @math{p}, with @math{p} on the horizontal axis and @math{P}(batch is accepted) on the
               vertical axis, is called the @italic{operating characteristics curve} for the acceptance sampling plan.
               Use the results of part(1) to sketch this curve for @math{0 ≤ p ≤ 1}.}
            
            @item{Repeat parts (1) and (2) with @math{1} replacing @math{2} in the acceptance sampling plan.}
            @item{Repeat parts (1) and (2) with @math{15} replacing @math{10} in the acceptance sampling plan.}
            @item{Which of the three sampling plans, that of part (1), (3), or (4), appears most satisfactory, and why?}]
 }]

@tamer-action[
 (for ([p (in-list (list 0.01 0.05 0.10 0.20 0.25))])
   (printf "B(~a; ~a, ~a) = ~a~n" 2 10 (~r p #:precision '(= 2))
           (B 2 10 p)))
 
 (plot #:title "Operating Characteristics Curve"
       #:legend-anchor 'top-right #:x-label "Lot Percentage Defective" #:y-label "Probability of Accept the Lot"
       (list (oc-curve 2 10 #:color 1)
             (oc-curve 1 10 #:color 2)
             (oc-curve 2 15 #:color 3)
             (vrule 0.1 #:style 'long-dash #:color 4)))

 (code:comment "As the plot above shown, both of the three sample plans perform well:")
 (code:comment "Good lots are probably accepted while poor lots probably fail.")
 (code:comment "Procedures may favor the 1st plan since more poor lots may still pass;")
 (code:comment "Consumers may favor the 3rd one since the accepted batch tends to a higher quality.")]

@margin-note{Exercise 59 on Page 125}

@itemlist[
 @item{An ordinance requiring that a smoke detector be installed in all previously constructed houses
  has been in effect in a particular city for 1 year. The fire department is concerned that many house
  remains without smoke detectors. Let @math{p} = the true proportion of such houses having detectors,
  and suppose that a random sample of @math{25} homes is inspected. If the sample strongly indicates
  that fewer than @math{80%} of all houses have a detector, the fire department will campaign for a
  mandatory inspection program. Because of the costliness of the program, the department prefers not
  to call for such inspections unless sample evidence strongly argues for their necessity. Let @math{X}
  denote the number of homes with detectors among the @math{25} sampled. Consider rejecting the claim
  that @math{p ≥ 80%} if @math{x ≤ 15}.

  @itemlist[#:style 'ordered
            @item{What is the probability that the claim is rejected when the actual value of @math{p} is @math{80%}?}
            @item{What is the probability of not rejecting the claim when @math{p = 70%}? When @math{p = 60%}?}
            @item{How do the "error probability" of parts (1) and (2) change if the value @math{15} in the decision
               rule is replaced by @math{14}?}]}
 ]

@tamer-action[
 (B 15 25 0.80)
 (for ([x (in-list (list 15 14))])
   (for ([p (in-list (list 0.60 0.70))])
     (printf "~~B(~a; ~a, ~a) = ~a~n" x 25 (~r p #:precision '(= 2))
             (- 1.0 (B x 25 p)))))
 
 (plot #:title "Error Probability"
       #:x-label "Proportion of Houses Having Detectors"
       #:y-label "Probability of Rejecting the Claim"
       (list (oc-curve 15 25 #:color 5)
             (oc-curve 14 25 #:color 6)
             (vrule 0.8 #:style 'long-dash #:color 4)))]


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
