#lang typed/racket/base

(module unsafe racket/base
  (provide (all-defined-out))
  
  (require digimon/ffi)
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-ffi-definer define-spec (digimon-ffi-lib "spec"))

  (define-spec test_offsetof (_fun _int _symbol -> _int))
  (define-spec test_containerof (_fun _int _symbol -> _int64))

  (define-spec pseudo_address _int64))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (require digimon/spec)
  (require digimon/foreign)

  (require racket/format)
  
  (require/typed/provide
   (submod ".." unsafe)
   [pseudo_address Natural]
   [test_offsetof (Integer Symbol -> Integer)]
   [test_containerof (Integer Symbol -> Integer)])
  
  (spec-begin prelude
              (describe "macro" #:do
                        (describe "offsetof" #:do
                                  (context "given `struct { char id; char name[20]; linked_list_node_t* list; char unused[16]; }`" #:do
                                           (let*/spec ([ptr-sizeof (compiler-sizeof '*)]
                                                       [3rd-offset (compiler-aligned-offset ptr-sizeof 21)]
                                                       [4th-offset (+ 3rd-offset ptr-sizeof)])
                                                      (it "should return 0 for the 1st member" #:do
                                                          (expect-= (test_offsetof 0 'id) 0))
                                                      (it "should return 1 for the 2nd member" #:do
                                                          (expect-= (test_offsetof 1 'name) 1))
                                                      (it ["should return ~a for the 3rd member" 3rd-offset] #:do
                                                          (expect-= (test_offsetof 2 'list) 3rd-offset))
                                                      (it ["should return ~a for the 4th member" 4th-offset] #:do
                                                          (expect-= (test_offsetof 3 'unused) 4th-offset)))))
                        (describe "containerof" #:do
                                  (it ["should return 0x~a for the instance" (~r pseudo_address #:base 16)] #:do
                                      (expect-0x= (test_containerof 2 'list) pseudo_address))))))
