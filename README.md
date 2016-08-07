# DE-server


###To run server enter:
###gcc serverd.c -o serverd
./serverd
(at DE prompt enter)
(To start server enter start)
##start
(To end enter stop)
##stop

###On another shell ,do the following:
###curl -X GET 'http://localhost:5566/question'
(change the port number to reflect what you have from the server)

####The server will response with a question... simple mathematical expression(this is machine generated , so the gramma may be wrong)
example of math expression:- WHAT IS 196 / 431 ?

####Solve the maths, and do the following:
####Replace all the space in the math expression with "%20" before sending back to server.

######curl -X PUT -d answer=67 -d question=WHAT%20IS%20196%20/%20431%20? localhost:5566

######It was tested with only Clisp, and you may need to open bridge.c and edit path of Clisp to reflect your own path.
######In case you want to use a different Port, go and make such changes in serverd.c . 
 
