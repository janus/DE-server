;; seed randomness
(setf *random-state* (make-random-state t))

;;simple sentence generator
(defun one-of (nl)
 (cons (nth (random ( - (list-length nl) 1)) nl) '()))

(defun Question() (one-of  '(what result which)))

(defun figure(n) (list (random n)))
(defun Verb() (one-of  '(- + / % *)))  
(defun direction () (one-of '( is of should would)))

(defun noun-phrase() (append (Question) (direction)))
(defun verb-phrase() (append (figure 489) (verb) (figure 489)))

(defun sentence() (append (noun-phrase) (verb-phrase)))

(defun result ()
   (format t "~{~a~2t~}?" (sentence)))



(result)


