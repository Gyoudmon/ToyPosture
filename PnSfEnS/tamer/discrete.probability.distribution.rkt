#lang scribble/lp2

@(require digimon/tamer)

@(require "../digitama/style.rkt")

@(define-bib HtROC
   #:title    "How to Read an OC Curve"
   #:author   (authors "Fred Schenkelberg")
   #:date     2019
   #:url      "https://accendoreliability.com/how-to-read-an-oc-curve/")

@(define-bib AEIoINaTR
   #:title    "An Exploratory Investigation of Identity Negotiation and Tattoo Removal"
   #:author   (authors "Jeremy A. Shelton" "Cara Peters")
   #:date     2008
   #:location (journal-location	"Academy of Marketing Science Review" #:number 6 #:volume 12))

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
  only if the proportion of defective components is at most @racket%[10]. The distributor decides to randomly
  select @racket[10] components and accept the batch only if the number of defective components in the sample
  is at most @racket[2].

  @itemlist[#:style 'ordered
            
            @item{What is the probability that the batch will be accepted when the actual proportion of defective is
               @racket%[1]? @racket%[5]? @racket%[10]? @racket%[20]? @racket%[25]?}
            
            @item{Let @math{p} denote the actual proportion of defective in the batch. A graph of @math{P}(batch is accepted)
               as a function of @math{p}, with @math{p} on the horizontal axis and @math{P}(batch is accepted) on the
               vertical axis, is called the @italic{operating characteristics curve} for the acceptance sampling plan.
               Use the results of part(1) to sketch this curve for @racketvalfont{0 ≤ p ≤ 1}.}
            
            @item{Repeat parts (1) and (2) with @racket[1] replacing @racket[2] in the acceptance sampling plan.}
            @item{Repeat parts (1) and (2) with @racket[15] replacing @racket[10] in the acceptance sampling plan.}
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
  remains without smoke detectors. Let @math{p = the true proportion of such houses having detectors},
  and suppose that a random sample of @racket[25] homes is inspected. If the sample strongly indicates
  that fewer than @racket%[80] of all houses have a detector, the fire department will campaign for a
  mandatory inspection program. Because of the costliness of the program, the department prefers not
  to call for such inspections unless sample evidence strongly argues for their necessity. Let @math{X}
  denote the number of homes with detectors among the @racket[25] sampled. Consider rejecting the claim
  that @math{p ≥ @racket%[80]} if @math{x ≤ @racket[15]}.

  @itemlist[#:style 'ordered
            @item{What is the probability that the claim is rejected when the actual value of @math{p} is @racket%[80]?}
            @item{What is the probability of not rejecting the claim when @math{p = @racket%[70]}? When @math{p = @racket%[60]}?}
            @item{How do the "error probability" of parts (1) and (2) change if the value @racket[15] in the decision
               rule is replaced by @racket[14]?}]}
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

@handbook-scenario{The Poisson Probability Distribution}

@margin-note{Exercise 98 on Page 137}

@margin-note{There is a typo in the formula in original book.}

@itemlist[
 @item{In some applications the distribution of a discrete rv @math{X} resembles the Poisson distrubution except
  that zero is not a possible value of @math{X}. For example, let @math{X = the number of tattoos that an individual wants
   removed when she or he arrives at a tattoo-removal facility}. Suppose the pmf of @math{X} is
                                                               
  @centered{@math{p(x) = ke@math-prime{-θ}θ@math-prime{x}/x!, x = 1, 2, 3...}}
  
  @itemlist[#:style 'ordered
            @item{Determine the value of @math{k}. @italic{Hint:} The sum of all probabilities in the Poisson pmf is @racket[1],
               and this pmf must also sum to @racket[1].}
            
            @item{If the mean value of @math{X} is @racket[2.313035], what is the probability that an individual wants at most
               @math{5} tattoos removed?}
            
            @item{Determine the standard deviation of @math{X} when the mean value is as given in (2).}]
  
  [@italic{NOTE:} The article @~cite[AEIoINaTR] gave a sample of @racket[22] observations on the number of tattoos
  people wanted removed; @italic{estimates} of @math{μ} and @math{σ} calculated from the data were @racket[2.318182] and
  @racket[1.249242], respectively.]}
 ]

@bold{Solution}
@itemlist[@item{@math{K(θ) = 1/(1-e@math-prime{-θ})}}]

@chunk[<s04e98-K>
       (define (K θ)
         (/ 1.0
            (- 1.0 (exp (- θ)))))]

@itemlist[@item{@math{p(x) = ke@math-prime{-θ}θ@math-prime{x}/x!}}]

@chunk[<s04e98-pmf>
       (define (p x θ)
         (let ([k (K θ)])
           (/ (* k (exp (- θ)) (expt θ x))
              (factorial x))))]

Since this unnamed distribution is just the @bold{Poisson Distribution} with a coefficient @math{k} for @math{x > 0}.
For the @bold{Poisson Distribution} we have @math{V@math-index{Po}(X) = E@math-index{Po}(X) = E(X@math-prime{2}) - [E(X)]@math-prime{2} = θ},
also @math{x@math-prime{2}p(x) = 0 for x = 0}, hence

@itemlist[@item{V = @math{σ@math-prime{2} = (k - 1)θ@math-prime{2} + kθ}}]

@chunk[<s04e98-V>
       (define (V θ)
         (let ([k (K θ)])
           (+ (* (- k 1.0) (expt θ 2))
              (* k θ))))]

@bold{Answers}
@tamer-action[
 (define θ 2.313035)
 (K θ)
 (for/sum ([x (in-range 1 6)])
   (p x θ))
 (sqrt (V θ))]

@handbook-reference[]

@; Chunks after `handbook-reference[]` will never be rendered in documents
@; <*> is the main chunk by convention.

@chunk[|<*>|
       (require digimon/tamer)
       (tamer-taming-start!)

       (module+ tamer |<discrete.random.variables.n.probability.distribution:*>|)]

@chunk[|<discrete.random.variables.n.probability.distribution:*>|
       (require plot/pict)
       (require math/statistics)
       (require math/distributions)
       (require (only-in math/number-theory factorial))
       
       <s04e98-K>
       <s04e98-pmf>
       <s04e98-V>]
