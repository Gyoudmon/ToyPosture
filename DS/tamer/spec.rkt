#lang typed/racket/base

(module unsafe racket/base
  (provide (all-defined-out))
  
  (require digimon/ffi)
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-ffi-definer define-spec (digimon-ffi-lib "spec"))

  (define-cstruct _linked_list_node
    ([prev _linked_list_node-pointer/null]
     [next _linked_list_node-pointer/null]))

  (define-cstruct _zahlen
    ([value _int]
     [node _linked_list_node]))
  
  (define-spec zahlen_create (_fun _int -> _zahlen-pointer)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (require digimon/spec)
  
  (require/typed/provide
   (submod ".." unsafe)
   [#:opaque Zahlen zahlen?]
   [#:opaque Linked-List-Node linked_list_node?]
   [zahlen_create (-> Integer Zahlen)]
   [zahlen-value (-> Zahlen Integer)]
   [zahlen-node (-> Zahlen Linked-List-Node)]
   [linked_list_node-prev (-> Linked-List-Node Linked-List-Node)]
   [linked_list_node-next (-> Linked-List-Node Linked-List-Node)])

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-context (zahlen-create-context value)
    #:desc ["create zahlen with value ~a" value] #:do
    (it-check-initial-zahlen value))
  
  (define-behavior (it-check-initial-zahlen value)
    (let ([z (zahlen_create value)])
      [#:it ["should associated with value ~a" value] #:do
       (expect-= (zahlen-value z) value)]
      [#:it "should associated the node whose previous and next nodes also point to itself" #:do
       (let* ([cnode (zahlen-node z)]
              [pnode (linked_list_node-prev cnode)]
              [nnode (linked_list_node-next cnode)])
         (expect-equal cnode pnode)
         (expect-equal pnode nnode))]))
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (spec-begin prelude
              (describe "zahlen" #:do
                        (zahlen-create-context 2022)
                        (zahlen-create-context -229))))
