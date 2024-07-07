<h3>Socket programming, basic client/server implementation : </h3> 
<p>https://www.geeksforgeeks.org/socket-programming-in-cpp/</p>
<h3>Socket Programming in C/C++: Handling multiple clients on server without multi threading :</h3>
<p>https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/</p>
<h3>Official documentation for IRC</h3>
<p>https://www.rfc-editor.org/rfc/rfc1459</p>
<h3>Could be needed for file transfers (bonus part)</h3>
<p>https://en.wikipedia.org/wiki/Direct_Client-to-Client</p>
<h4>Plan for 30.06.24 (DONE)</h4> 
<ol>
  <li>Rewrite server in cpp oop style, get rid off read, printf, puts, perror</li>
  <li>Improve the client we already have <b>(still need to do)</b></li>
  <li>Write a Makefile</li>
</ol>

<h4>Plan for 03.07.24</h4>
<ol>
  <li>Select reference client (real irc client) (Done)</li>
  <li>Undertsand the communication between the client and the server : format of the messages</li>
  <li>Install irc server/client to learn about the behaviour that we need to recreate</li>
</ol>

<h4>Plan for 05.07.24 (9:00-11:00, 14:00-18:00 Prague time)</h4>
<ol>
  <li>Find workable ft_irc for test with chosen irssi (Tanya)</li>
</ol>

<h4>Plan for 06.07.24 (13:00-16:00 Prague time)</h4>
<ol>
  <li>Proceed with the article: mode message 4.2.3</li>
  <li>Check for port and password as program parameters</li>
</ol>

<h4>Plan for 07.07.24 (14:00-16:00 Prague time)</h4>
<ol>
  <li><s>Remove getpeername function</s></li>
  <li>buffer for every connection?</li>
  <li>class? for every connection?</li>
  <li>map of class connections (sd-connection class)</li>
  <li>Extract message from the buffer 2.3.1</li>
  <li>Process message (class? structure? maybe split it in the constructor getters for command and params)</li>
  <li>Free buffer and discard invalid commands</li>
</ol>

<h4>Plan for 09.07.24 (10:00-16:00 Prague time)</h4>
<ol>
  <li>Add colours to messages</li>
  <li>Handle Ctrl+C and exit cleanly</li>
  <li>Fix memory leaks and free everything that needs freeing</li>
</ol>

<h4>Plan for an unscheduled future</h4>
<ol>
  <li>Create channels (class channel, assign user, client-channel objs)</li>
  <li>Update client to test prefix, server validation of messages</li>
  <li>Make a container of commands, check if they exist</li>
</ol>
