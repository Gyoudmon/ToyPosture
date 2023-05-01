#lang typed/racket

(require schema/csv)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-type Scores (Listof (Listof String)))
(define-type Score-Records (Immutable-HashTable String Scores))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define list-students : (-> Path-String (Listof String))
  (lambda [/dev/gmsin]
    (for/list : (Listof String)
      ([line (in-list (file->lines /dev/gmsin))]
       #:when (regexp-match? #px"^s:" line))
      (define maybe-name (regexp-match #px"^s:\\d+,(\\d+).*" line))
      (assert (cadr (assert maybe-name))))))

(define list-disciplines : (-> Path-String (Listof String))
  (lambda [/dev/gmsin]
    (for/list : (Listof String)
      ([line (in-list (file->lines /dev/gmsin))]
       #:when (regexp-match? #px"^d:" line))
      (define maybe-code (regexp-match #px"^d:(\\d+).*" line))
      (assert (cadr (assert maybe-code))))))

(define list-scores : (->* (Path-String) (Score-Records) Score-Records)
  (lambda [/dev/gmsin [scores ((inst make-immutable-hash String Scores))]]
    (for/fold ([ss : Score-Records scores])
              ([line (in-csv* /dev/gmsin #false)])
      (hash-set ss (car line)
                (cons (cdr line)
                      (hash-ref ss (car line)
                                (λ [] null)))))))

(define list-all-scores : (->* (String) (Score-Records) Score-Records)
  (lambda [rootdir [scores ((inst make-immutable-hash String Scores))]]
    (cond [(directory-exists? rootdir)
           (for/fold ([ss : Score-Records scores])
                     ([sub (in-list (directory-list rootdir #:build? #true))]
                      #:when (equal? (path-get-extension sub) #".csv"))
             (list-scores sub ss))]
          [(file-exists? rootdir)
           (list-scores rootdir)]
          [else scores])))

(define list-all-scores/flatten : (-> String Scores)
  (lambda [rootdir]
    (apply append (hash-values (list-all-scores rootdir)))))

(define bind-scores : (-> (Listof String) (Listof String) String (Listof (Listof String)) Void)
  (lambda [students disciplines timestamp scores]
    (define d-count (length disciplines))
    (define s-count (length scores))
    (for ([sNo (in-list students)])
      (for ([disCode (in-list disciplines)])
        (printf "sdt:~a,~a,~a,~a~n" sNo disCode timestamp
                (string-join (append (list-ref scores (random 0 s-count))
                                     (list "3"))
                             " "))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (define argv (current-command-line-arguments))
  (define argc (vector-length argv))

  (when (>= argc 3)
    (bind-scores
     (list-students (vector-ref argv 0))
     (list-disciplines (vector-ref argv 0))
     (vector-ref argv 1)
     (list-all-scores/flatten (vector-ref argv 2)))))
