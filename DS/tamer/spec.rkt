#lang typed/racket/base

(module unsafe racket/base
  (provide (all-defined-out))
  
  (require digimon/ffi)
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-ffi-definer define-spec (digimon-ffi-lib "spec"))

  (define-cstruct _linked_list_node
    ([next _linked_list_node-pointer/null]))

  (define-cstruct _zahlen
    ([datum _llong]
     [node _linked_list_node]))

  (define-cstruct _zahlen_env
    ([head _linked_list_node-pointer/null]
     [tail _linked_list_node-pointer/null]))
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-spec zahlen_env_construct (_fun -> _zahlen_env-pointer))
  (define-spec zahlen_env_push_datum (_fun _zahlen_env-pointer _llong -> _void))
  
  (define-spec zahlen_env_pop_datum
    (_fun _zahlen_env-pointer [datum : (_ptr o _llong)]
          -> [okay? : _bool]
          -> (values datum okay?)))

  (define-spec zahlen_env_sum
    (_fun _zahlen_env-pointer
          [o_sum : (_ptr o _llong)]
          [e_sum : (_ptr o _llong)]
          -> _void
          -> (values o_sum e_sum)))

  (define-spec zahlen_env_ref (_fun _zahlen_env-pointer _int _llong -> _llong))
  (define-spec zahlen_env_size (_fun _zahlen_env-pointer -> _llong)))

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
   [zahlen_env_construct (-> Zahlen-Env)]
   [zahlen_env_push_datum (-> Zahlen-Env Integer Void)]
   [zahlen_env_pop_datum (-> Zahlen-Env (Values Integer Boolean))]
   [zahlen_env_sum (-> Zahlen-Env (Values Integer Integer))]
   
   [zahlen_env_size (-> Zahlen-Env Integer)]
   [zahlen_env_ref (-> Zahlen-Env Integer Integer Integer)]
   
   [zahlen-datum (-> Zahlen Integer)]
   [zahlen-node (-> Zahlen Linked-List-Node)]
   [linked_list_node-next (-> Linked-List-Node (Option Linked-List-Node))])

  (require/typed/provide
   ffi/unsafe
   [ptr-equal? (-> (Option Linked-List-Node) (Option Linked-List-Node) Boolean)])

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define samples : (Listof Integer) (build-list 16 (λ [[i : Index]] (random 0 101))))
  (define-values (odd-samples even-samples) (partition odd? samples))

  (define zahlen-pop : (-> Zahlen-Env Integer (Listof Integer))
    (lambda [master N]
      (let pop ([n : Index (assert N index?)]
                [data : (Listof Integer) null])
        (cond [(<= n 0) (reverse data)]
              [else (let-values ([(datum okay?) (zahlen_env_pop_datum master)])
                      (pop (sub1 n) (cons datum data)))]))))
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (begin-spec prelude
              (let/spec ([master (zahlen_env_construct)])
                (context "construction" #:do
                  (it "should return an intance whose head and tail both point to NULL" #:do
                    (expect-false (zahlen_env-head master))
                    (expect-false (zahlen_env-tail master)))
                  (it "should be 0 in size" #:do
                    (expect-zero (zahlen_env_size master)))
                  (context ["when given the first zahlen: ~a" (car samples)] #:do
                    #:before (λ [] (zahlen_env_push_datum master (car samples))) #:do
                    (it "should be 1 in size" #:do
                      (expect-= (zahlen_env_size master) 1))
                    (it ["should contain datum ~a" (car samples)] #:do
                      (expect-= (zahlen_env_ref master 0 -1) (car samples)))
                    (it "should have both head and tail pointed to the same target" #:do
                      (expect-true (ptr-equal? (zahlen_env-head master)
                                               (zahlen_env-tail master)))))
                  (context ["when given another list of zahlen: ~a" (cdr samples)] #:do
                    #:before (λ [] (for ([n (in-list (cdr samples))]) (zahlen_env_push_datum master n))) #:do
                    (it ["should be ~a in size" (length samples)] #:do
                      (expect-= (zahlen_env_size master) (length samples)))
                    
                    (for/spec ([idx (in-naturals 1)]
                               [val (in-list (cdr samples))])
                      (it ["should be ~a at position ~a" (list-ref samples idx) idx] #:do
                        (expect-= (zahlen_env_ref master idx -1) (list-ref samples idx))))))
                (context "calculation" #:do
                  (let-values/spec ([(odd_sum even_sum) (zahlen_env_sum master)]
                                    [(odd-sum) (apply + odd-samples)]
                                    [(even-sum) (apply + even-samples)])
                    (it ["should be ~a when summing odd elements" odd-sum] #:do
                      (expect-= odd_sum odd-sum))
                    (it ["should be ~a when summing even elements" even-sum] #:do
                      (expect-= even_sum even-sum))))
                (context "destruction" #:do
                  (let/spec ([rdata (zahlen-pop master (sub1 (length samples)))])
                    (context ["when done poping datum ~a times" (sub1 (length samples))] #:do
                      (it "should be left 1 in size" #:do
                        (expect-= (zahlen_env_size master) 1))
                      (it "should be poped in reversed order" #:do
                        (expect-equal rdata (reverse (cdr samples))))
                      (it "should have both head and tail pointed to the same target" #:do
                        (expect-true (ptr-equal? (zahlen_env-head master)
                                                 (zahlen_env-tail master))))))
                  (context "when continue with the last datum" #:do
                    #:before (λ [] (let-values ([(who cares) (zahlen_env_pop_datum master)]) (void))) #:do
                    (it "should be 0 in size" #:do
                      (expect-zero (zahlen_env_size master)))
                    (it "should have both head and tail pointed to NULL" #:do
                      (expect-false (zahlen_env-head master))
                      (expect-false (zahlen_env-tail master))))
                  (context "when continue with no datum" #:do
                    (let-values ([(dirty okay?) (zahlen_env_pop_datum master)])
                      (it "should be 0 in size" #:do
                        (expect-false okay?))))))))
