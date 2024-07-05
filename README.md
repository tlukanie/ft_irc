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
  <li>Rewrite server to let user set port/password as args</li>
  <li>Find workable ft_irc for test with chosen irssi (Tanya)</li>
</ol>

<h4>Plan for 06.07.24 (13:00-16:00 Prague time)</h4>
<ol>
  <li>Proceed with the article: mode message 4.2.3</li>
  <li>Rewrite server to let user set port/password as args</li>
  <li>Remove getpeername function</li>
  <li>Fix errors with irssi and our server</li>
</ol>

<h4>Plan for an unscheduled future</h4>
<ol>
  <li>Create channels (class channel, assign user, client-channel objs)</li>
  <li>Update client to test prefix, server validation of messages</li>
</ol>
