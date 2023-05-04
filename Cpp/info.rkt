#lang info

(define collection 'use-pkg-name)
(define pkg-desc "Cpp Disciplines")

(define version "1.0")
(define pkg-authors '(wargrey))
(define test-omit-paths 'all)

(define native-launcher-names '(["GradeManagementSystem.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "C:\\opt\\vcpkg\\installed\\x64-windows\\include"])
                                  (libpath [windows "C:\\opt\\vcpkg\\installed\\x64-windows\\lib"]))]

                                ["PageReplacementAlgo.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "C:\\opt\\vcpkg\\installed\\x64-windows\\include"])
                                  (libpath [windows "C:\\opt\\vcpkg\\installed\\x64-windows\\lib"]))]))

(define typesettings '(["literacy/GradeManagementSystem.scrbl" xelatex #px#"IMS(/\\w+)?/\\w+[.](png|[hc]pp)$"]))
