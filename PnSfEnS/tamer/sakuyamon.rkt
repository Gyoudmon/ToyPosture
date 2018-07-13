#lang scribble/lp2

@(require "tamer.rkt")

@handbook-story{Hello, Sakuyamon!}

@tamer-smart-summary[]

@chunk[|<sakuyamon taming start>|
       (require "tamer.rkt")
       (tamer-taming-start)

       (module+ tamer |<sakuyamon:*>|)]

@handbook-scenario{Sakuyamon, Realize!}

@tamer-note['realize]
@chunk[|<testcase: realize>|
       (test-spec "sakuyamon realize"
                  (check-eq? 1 1))]

@handbook-scenario{How is everything going?}

Logging plays an important role in the lifecycle of Web Application, but the fact is that
most modern operating systems such as Unixes do not provide a good out of box tool
to monitor the logs just as MacOSX @exec{Console.app} does. Therefore here exists
@exec{foxpipe} and @exec{izuna}.

@tamer-action[(set 1 2 3)]

@handbook-reference[]

@chunk[|<sakuyamon:*>|
       (module+ story
         (define-tamer-suite realize "Sakuyamon, Realize!" |<testcase: realize>|))]
