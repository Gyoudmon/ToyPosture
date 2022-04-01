#lang typed/racket/base

(module unsafe racket/base
  (provide (all-defined-out))
  
  (require digimon/ffi)
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-ffi-definer define-spec (digimon-ffi-lib "spec"))

  (define-cstruct _linked_list_node
    ([next _linked_list_node-pointer/null]))

  (define-cstruct _zahlen
    ([value _int]
     [node _linked_list_node]))

  (define-cstruct _zahlen_env
    ([head _linked_list_node-pointer/null]
     [tail _linked_list_node-pointer/null]))
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-spec zahlen_create (_fun _int -> _zahlen-pointer))

  (define-spec zahlen_env_initialize (_fun -> _zahlen_env-pointer))
  (define-spec zahlen_env_push_value (_fun _zahlen_env-pointer _int -> _void))

  (define-spec zahlen_env_ref (_fun _zahlen_env-pointer _int _int -> _int))
  (define-spec zahlen_env_count (_fun _zahlen_env-pointer -> _int)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (require digimon/spec)
  
  (require racket/list)
  
  (require/typed/provide
   (submod ".." unsafe)
   [#:opaque Zahlen zahlen?]
   [#:opaque Zahlen-Env zahlen_env?]
   [#:opaque Linked-List-Node linked_list_node?]

   [zahlen_env-head (-> Zahlen-Env (Option Linked-List-Node))]
   [zahlen_env-tail (-> Zahlen-Env (Option Linked-List-Node))]
   [zahlen_env_initialize (-> Zahlen-Env)]
   [zahlen_env_push_value (-> Zahlen-Env Integer Void)]
   [zahlen_env_count (-> Zahlen-Env Integer)]
   [zahlen_env_ref (-> Zahlen-Env Integer Integer Integer)]
   
   [zahlen_create (-> Integer Zahlen)]
   [zahlen-value (-> Zahlen Integer)]
   [zahlen-node (-> Zahlen Linked-List-Node)]
   [linked_list_node-next (-> Linked-List-Node (Option Linked-List-Node))])

  (require/typed/provide
   ffi/unsafe
   [ptr-equal? (-> (Option Linked-List-Node) (Option Linked-List-Node) Boolean)])

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-context (zahlen-create-context value)
    #:desc ["create zahlen with value ~a" value] #:do
    (it-check-initial-zahlen value))
  
  (define-behavior (it-check-initial-zahlen value)
    (let ([z (zahlen_create value)])
      [#:it ["should associated with the value ~a" value] #:do
       (expect-= (zahlen-value z) value)]
      [#:it "should associated with a node whose next node is NULL" #:do
       (let* ([cnode (zahlen-node z)])
         (expect-false (linked_list_node-next (zahlen-node z))))]))

  (define samples : (Listof Integer) (build-list 16 (λ [[i : Index]] (random 0 101))))
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (spec-begin prelude
              (describe "zahlen API" #:do
                        (zahlen-create-context 2022)
                        (zahlen-create-context -229))
              (describe "zahlen application" #:do
                        (let/spec ([master (zahlen_env_initialize)])
                                  (context "initialize" #:do
                                           (it "should return an intance whose head and tail both point to NULL" #:do
                                               (expect-false (zahlen_env-head master))
                                               (expect-false (zahlen_env-tail master)))
                                           (it "should be 0 in size" #:do
                                               (expect-zero (zahlen_env_count master))))
                                  (context ["when given the first zahlen: ~a" (car samples)] #:do
                                           #:before (λ [] (zahlen_env_push_value master (car samples))) #:do
                                           (it "should be 1 in size" #:do
                                               (expect-= (zahlen_env_count master) 1))
                                           (it "should be equal for head and tail" #:do
                                               (expect-true (ptr-equal? (zahlen_env-head master)
                                                                        (zahlen_env-tail master)))))
                                  (context ["when given another list of zahlen: ~a" (cdr samples)] #:do
                                           #:before (λ [] (for ([n (in-list (cdr samples))])
                                                            (zahlen_env_push_value master n))) #:do
                                           (it ["should be ~a in size" (length samples)] #:do
                                               (expect-= (zahlen_env_count master) (length samples)))

                                           (for/spec ([idx (in-naturals 0)]
                                                      [val (in-list samples)])
                                             (it ["should be ~a at position ~a" (list-ref samples idx) idx] #:do
                                               (expect-= (zahlen_env_ref master idx -1) (list-ref samples idx)))))))))
