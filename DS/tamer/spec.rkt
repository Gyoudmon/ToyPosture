#lang typed/racket/base

(module unsafe racket/base
  (provide (all-defined-out))
  
  (require digimon/ffi)
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-ffi-definer define-spec (digimon-ffi-lib "spec"))

  (define-cstruct _singly_list_node
    ([next _singly_list_node-pointer/null]))

  (define-cstruct _zahlen
    ([datum _llong]
     [node _singly_list_node]))

  (define-cstruct _zahlen_env
    ([head _singly_list_node-pointer/null]
     [tail _singly_list_node-pointer/null]))
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define population 16)

  (define-spec singly_list_split_half (_fun _singly_list_node-pointer/null _singly_list_node-pointer/null -> _singly_list_node-pointer/null))
  (define-spec singly_list_size (_fun _singly_list_node-pointer/null _singly_list_node-pointer/null -> _llong))
  
  (define-spec zahlen_env_destruct (_fun _zahlen_env-pointer -> _void) #:wrap (deallocator))
  (define-spec zahlen_env_construct (_fun -> _zahlen_env-pointer) #:wrap (allocator zahlen_env_destruct))
  (define-spec zahlen_env_push_datum (_fun _zahlen_env-pointer _llong -> _void))
  
  (define-spec zahlen_env_pop_datum
    (_fun _zahlen_env-pointer [datum : (_ptr o _llong)]
          -> [okay? : _bool]
          -> (values datum okay?)))

  (define-spec zahlen_env_sort (_fun _zahlen_env-pointer -> _void))

  (define-spec zahlen_env_sum
    (_fun _zahlen_env-pointer
          [o_sum : (_ptr o _llong)]
          [e_sum : (_ptr o _llong)]
          -> _void
          -> (values o_sum e_sum)))

  (define-spec zahlen_env_ref (_fun _zahlen_env-pointer _int _llong -> _llong))
  (define-spec zahlen_env_size (_fun _zahlen_env-pointer -> _llong))
  (define-spec zahlen_datum_from_node (_fun _singly_list_node-pointer -> _llong))

  (define-spec zahlen_env_to_array
    (_fun _zahlen_env-pointer
          [vs : (_list o _llong population)]
          [size : _int = population]
          -> _void
          -> vs)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (require digimon/spec)
  
  (require racket/list)
  
  (require/typed/provide
   (submod ".." unsafe)
   [#:opaque Zahlen zahlen?]
   [#:opaque Zahlen-Env zahlen_env?]
   [#:opaque Singly-List-Node singly_list_node?]
   [population Byte]

   [singly_list_size (-> (Option Singly-List-Node) (Option Singly-List-Node) Integer)]
   [singly_list_split_half (-> (Option Singly-List-Node) (Option Singly-List-Node) (Option Singly-List-Node))]

   [zahlen_env-head (-> Zahlen-Env (Option Singly-List-Node))]
   [zahlen_env-tail (-> Zahlen-Env (Option Singly-List-Node))]
   [zahlen_env_construct (-> Zahlen-Env)]
   [zahlen_env_push_datum (-> Zahlen-Env Integer Void)]
   [zahlen_env_pop_datum (-> Zahlen-Env (Values Integer Boolean))]
   [zahlen_env_sort (-> Zahlen-Env Void)]
   [zahlen_env_sum (-> Zahlen-Env (Values Integer Integer))]
   
   [zahlen_env_size (-> Zahlen-Env Integer)]
   [zahlen_env_ref (-> Zahlen-Env Integer Integer Integer)]
   [zahlen_env_to_array (-> Zahlen-Env (Listof Integer))]
   
   [zahlen-datum (-> Zahlen Integer)]
   [zahlen-node (-> Zahlen Singly-List-Node)]
   [singly_list_node-next (-> Singly-List-Node (Option Singly-List-Node))]

   [zahlen_datum_from_node (-> Singly-List-Node Integer)])

  (require/typed/provide
   ffi/unsafe
   [ptr-equal? (-> (Option Singly-List-Node) (Option Singly-List-Node) Boolean)])

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-context (list-splitting-context count)
    (let* ([expt-lsize (+ (quotient count 2) (remainder count 2))]
           [expt-rsize (- count expt-lsize)])
      #:desc ["when given a list with ~a elements" count] #:do
      (it ["should be splitted into 2 sublists with sizes ~a and ~a, respectively" expt-lsize expt-rsize] #:do
        (let-values ([(left-size right-size) (singly-list-splitted-sizes count)])
          (expect-= left-size expt-lsize)
          (expect-= right-size expt-rsize)))))

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define samples : (Listof Integer) (build-list population (λ [[i : Index]] (random 0 101))))
  (define sorted-samples : (Listof Integer) (sort samples <))
  (define-values (odd-samples even-samples) (partition odd? samples))

  (define zahlen-pop : (-> Zahlen-Env Integer (Listof Integer))
    (lambda [master N]
      (let pop ([n : Index (assert N index?)]
                [data : (Listof Integer) null])
        (cond [(<= n 0) (reverse data)]
              [else (let-values ([(datum okay?) (zahlen_env_pop_datum master)])
                      (pop (sub1 n) (cons datum data)))]))))

  (define singly-list-splitted-sizes : (-> Index (Values Integer Integer))
    (lambda [count]
      (define master (zahlen_env_construct))

      (for ([i (in-range count)])
        (zahlen_env_push_datum master i))

      (let* ([head (zahlen_env-head master)]
             [subhead (singly_list_split_half head #false)])
        (values (singly_list_size head subhead) (singly_list_size subhead #false)))))
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (default-spec-hide-timing-info #true)
  
  (begin-spec prelude
              (let/spec ([master (zahlen_env_construct)])
                (describe "construction" #:do
                  (it "should return an intance whose head and tail both point to NULL" #:do
                    (expect-false (zahlen_env-head master))
                    (expect-false (zahlen_env-tail master)))
                  (it "should be 0 in size" #:do
                    (expect-zero (zahlen_env_size master)))

                  (context ["when given the first zahlen: ~a" (car samples)] #:do
                    #:before (λ [] (zahlen_env_push_datum master (car samples))) #:do
                    (it "should be 1 in size" #:do
                      (expect-= (zahlen_env_size master) 1))
                    (it ["should contain the datum ~a" (car samples)] #:do
                      (expect-= (zahlen_env_ref master 0 -1) (car samples)))
                    (it "should have both head and tail pointed to the same target" #:do
                      (expect-true (ptr-equal? (zahlen_env-head master)
                                               (zahlen_env-tail master)))))

                  (context ["when given another list of zahlens: ~a" (cdr samples)] #:do
                    #:before (λ [] (for ([n (in-list (cdr samples))]) (zahlen_env_push_datum master n))) #:do
                    (it ["should be ~a in size" (length samples)] #:do
                      (expect-= (zahlen_env_size master) (length samples)))
                    
                    (let/spec ([data (zahlen_env_to_array master)])
                      (it "should be pushed in original order" #:do
                        (expect-equal data samples)))))

                (describe "calculation" #:do
                  (let-values/spec ([(odd_sum even_sum) (zahlen_env_sum master)]
                                    [(odd-sum) (apply + odd-samples)]
                                    [(even-sum) (apply + even-samples)])
                    (it ["should be ~a when summing odd elements" odd-sum] #:do
                      (expect-= odd_sum odd-sum))
                    (it ["should be ~a when summing even elements" even-sum] #:do
                      (expect-= even_sum even-sum))))

                (describe "sorting" #:do
                  (context "splitting list" #:do
                    #:before (λ [] (zahlen_env_sort master)) #:do
                    (list-splitting-context 0)
                    (list-splitting-context 1)
                    (list-splitting-context 8)
                    (list-splitting-context 9))
                  
                  (let/spec ([sdata (zahlen_env_to_array master)])
                    (context "when done in-place sorting" #:do
                      (it ["should be ~a in size" (length samples)] #:do
                        (expect-= (zahlen_env_size master) (length samples)))
                      (it "should be in ascending order" #:do
                        (expect-ordered sdata <=))
                      (it ["should contain the datum '~a' for the head" (car sdata)] #:do
                        (expect-= (zahlen_datum_from_node (assert (zahlen_env-head master))) (car sdata)))
                      (it ["should contain the datum '~a' for the tail" (last sdata)] #:do
                        (expect-= (zahlen_datum_from_node (assert (zahlen_env-tail master))) (last sdata))))))

                (describe "destruction" #:do
                  (let/spec ([rdata (zahlen-pop master (sub1 (length sorted-samples)))])
                    (context ["when done poping datum ~a times" (sub1 (length sorted-samples))] #:do
                      (it "should be left 1 in size" #:do
                        (expect-= (zahlen_env_size master) 1))
                      (it "should be popped in reversed order" #:do
                        (expect-equal rdata (reverse (cdr sorted-samples))))
                      (it "should have both head and tail pointed to the same address" #:do
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
