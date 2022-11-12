\version "2.23.10"

#(define (show-modules id)
   (if (module-local-variable (current-module) id)
       (ly:message "~a is defined in the current module." id)
       (let ((found (find (lambda (mod)
                            (module-variable mod id))
                          (module-uses (current-module)))))
         (if found
             (ly:message "~a is defined in module ~a"
                         id
                         (module-name found))
             (ly:message "~a not defined, or maybe defined dynamically." id)))))

#(show-modules 'std::map)
#(show-modules 'match)
#(show-modules 'music-std::map)
variable = #'value
#(define variable2 'value2)
#(show-modules 'variable)
#(show-modules 'variable2)
#(show-modules 'unbound)

#(define (carre n) (* n n))

#(show-modules 'cons)
#(show-modules 'carre)
