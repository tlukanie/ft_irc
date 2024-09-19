<h3>Socket programming, basic client/server implementation : </h3> 
<p>https://www.geeksforgeeks.org/socket-programming-in-cpp/</p>
<h3>Socket Programming in C/C++: Handling multiple clients on server without multi threading :</h3>
<p>https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/</p>
<h3>Official documentation for IRC</h3>
<p>https://www.rfc-editor.org/rfc/rfc1459</p>
<h3>An IRC quickstart guide</h3>
<p>https://opensource.com/life/16/6/irc-quickstart-guide</p>
<h3>Modern IRC</h3>
<p>https://modern.ircdocs.horse/</p>
<h3>Could be needed for file transfers (bonus part)</h3>
<p>https://en.wikipedia.org/wiki/Direct_Client-to-Client</p>

<p>/connect localhost 5555 aaa</p>
<h4>Plan for 30.06.24 (DONE)</h4> 
<ol>
  <li><s>Rewrite server in cpp oop style, get rid off read, printf, puts, perror</s></li>
  <li>Improve the client we already have <b>(still need to do)</b></li>
  <li><s>Write a Makefile</s></li>
</ol>

<h4>Plan for 03.07.24</h4>
<ol>
  <li><s>Select reference client (real irc client)</s> (Done)</li>
  <li>Undertsand the communication between the client and the server : format of the messages</li>
  <li><s>Install irc server/client to learn about the behaviour that we need to recreate</s></li>
</ol>

<h4>Plan for 05.07.24 (9:00-11:00, 14:00-18:00 Prague time)</h4>
<ol>
  <li>Find workable ft_irc for test with chosen irssi (Tanya)</li>
</ol>

<h4>Plan for 06.07.24 (13:00-16:00 Prague time)</h4>
<ol>
  <li><s>Proceed with the article: mode message 4.2.3</s></li>
  <li><s>Check for port and password as program parameters</s></li>
</ol>

<h4>Plan for 07.07.24 (14:00-16:00 Prague time)</h4>
<ol>
  <li><s>Remove getpeername function</s></li>
  <li><s>Add colours to messages</s></li>
  <li><s>Write pseudocode</s></li>
  <li><s>Handle Ctrl+C and exit cleanly</s></li>
</ol>

<h4>Plan for 09.07.24 (17:00-19:00 Prague time) and the rest of the week</h4>
<ol>
  <li><s>buffer for every connection?</s></li>
  <li><s>class? for every connection?</s></li>
  <li><s>map of class users (sd-connection class)</s></li>
</ol>

<h4>Plan for 10.07.24 (14:15-15:45 16:30-19:00 Prague time) and the rest of the week</h4>
<ol>
  <li><s>Use users in the main server loop</s></li>
  <li><s>Use connection buffer (vector _data)</s></li>
  <li><s>Create message class</s></li>
</ol>

<h4>Plan for 11.07.24 (10:30-12:00 && 15:30-17:00 Prague time) and the rest of the week</h4>
<ol>
  <li><s>Extract message from the data buffer 2.3.1</s></li>
  <li><s>Free data buffer and discard invalid commands</s></li>
  <li><s>Process message (class? structure? maybe split it in the constructor getters for command and params)</s></li>
  <li><s>map container "(string command, pointer to function to process message)"</s></li>
</ol>

<h4>Plan for 12.07.24 (09:30 - 11:30 && 13:30 - 19:00 Prague time)</h4>
<ol>
  <li><s>Set the map of commands and functions (PING-PONG, PASS,...)</s></li>
  <li><s>Make a container of commands, check if they exist</s></li>
  <li><s>Go through messages and pass them to functions</s></li>
  <li><s>Send something back to client for ping and user message</s></li>
  <li><s>Clear the multimap</s></li>
</ol>

<h4>Plan for 14.07.24 (14:30 - 15:30 Prague time)</h4>
<ol>
  <li><s>Optimise/rewrite the loop</s></li>
  <li><s>Read through the modern irc specification (especially commands)</s></li>
</ol>

<h4>Plan for 21.07.24 (11:15 - 13:15 Prague time)</h4>
<ol>
  <li><s>Password command check</s></li>
  <li><s>Updated colours</s></li>
</ol>

<h4>Plan for 31.08.24 (13:30-15:30 Prague time)</h4>
<ol>
  <li><s>Logger/debugger improvements</s></li>
</ol>

<h4>Plan for 01.09.24 (13:30-15:30 Prague time)</h4>
<ol>
  <li><s>More logger/debugger improvements</s></li>
  <li><s>Understand the code</s></li>
  <li><s>Utils functions</s></li>
  <li><s>Template functions strtoi and itostr</s></li>
</ol>

<h4>Plan for 04.09.24 (10:00 - 12:00)</h4>
<ol>
  <li><s>More logger/debugger improvements (time stamps)</s></li>
</ol>

<h4>Plan for 05.09.24 (13:30-15:30 Prague time)</h4>
<ol>
  <li><s>Logger/debugger used in the code</s></li>
  <li><s>Understand the code</s></li>
  <li><s>Utils functions</s></li>
</ol>

<h4>Plan for 07.09.24 (13:00-15:00 Prague time)</h4>
<ol>
  <li><s>Logger/debugger used in the code</s></li>
  <li><s>Read the new documentation</s></li>
  <li><s>User class</s></li>
</ol>

<h4>Plan for 08.09.24 (9:30-11:30 Prague time)</h4>
<ol>
  <li><s>Logger/debugger used in the code</s></li>
  <li><s>User class username, realname, auth getters and setters</s></li>
  <li><s>Nick message prefix (need to process USER first)</s></li>
  <li><s>Config file</s></li>
</ol>

<h4>Plan for 09.09.24 (14:30-16:30 Prague time)</h4>
<ol>
  <li><s>Channel class</s></li>
  <li><s>Logger/debugger (function to display messages outgoing)</s></li>
</ol>

<h4>Plan for 10.09.24 (13:30-15:30 Prague time)</h4>
<ol>
  <li><s>JOIN message</s></li>
  <li><s>PART message</s></li>
  <li><s>2 (multi)maps of users and channels one with key on user another with key on channel</s></li>
</ol>

<h4>Plan for 11.09.24 (10:30-12:30 Prague time)</h4>
<ol>
  <li><s>PRIVMSG</s></li>
  <li><s>Logger/debugger (function to display buffers)</s></li>
  <li><s>Logger/debugger (function to display messages incoming)</s></li>
  <li><s>Logger/debugger (function to display messages processing)</s></li>
  <li><s>send_reply function</s></li>
</ol>

<h4>Plan for 12.09.24 (10:30-12:30 Prague time)</h4>
<ol>
  <li><s>TOPIC</s></li>
  <li><s>INVITE</s></li>
  <li><s>send_reply_channel function</s></li>
  <li><s>send_reply_others function</s></li>
</ol>

<h4>Plan for 13.09.24 (09:30-11:30 Prague time)</h4>
<ol>
  <li><s>KICK</s></li>
  <li><s>AWAY</s></li>
</ol>

<h4>Plan for 14.09.24 (11:30-13:30 Prague time)</h4>
<ol>
  <li><s>MODE +itkl</s></li>
</ol>

<h4>Plan for 15.09.24 (09:30-11:30 Prague time)</h4>
<ol>
  <li><s>MODE +o</s></li>
  <li><s>Operator priviledges and user status in channel or user class? identify users by sd?</s></li>
  <li><s>Check if the server can run on other IP than localhost</s></li>
  <li><s>plan for next week</s></li>
</ol>

<h4>Plan for 16.09.24 (14:30-16:30 Prague time)</h4>
<ol>
  <li><s>remove user from the channel</s></li>
  <li><s>add user to the channel</s></li>
  <li><s>remove user from the server (send error message)</s></li>
  <li><s>QUIT</s></li>
  <li><s>ERROR</s></li>
</ol>

<h4>Plan for 17.09.24 (9:30-11:30 Prague time)</h4>
<ol>
  <li><s>DEBUG - command (print info about Channel or User)</s></li>
  <li><s>Get rid of exit and other? functions that are not allowed</s></li>
  <li><s>Try-catch blocks for server breaking errors</s></li>
</ol>

<h4>Plan for 18.09.24 (16:30-18:30 Prague time)</h4>
<ol>
  <li><s>Update logic of replies so they always go through select</s></li>
  <li><s>send_reply function used for other messages</s></li>
</ol>


<h4>Plan for 19.09.24 (9:30-11:30 Prague time)</h4>
<ol>
  <li>Proper command handling</li>
  <li>Error codes</li>
  <li>Logger/debugger used everywhere in the code</li>
  
</ol>
-   [ ] Task 1
-   [x] Completed Task

<h4>Plan for 20.09.24 (10:30-12:30 Prague time)</h4>
<ol>
  <li>Proper command handling</li>
  <li>Error codes</li>
  <li>Logger/debugger used everywhere in the code</li>
</ol>


<h4>Plan for 21.09.24 (14:30-16:30 Prague time)</h4>
<ol>
  <li>Logger/debugger goes through select()</li>
  <li></li>
</ol>

<h4>Plan for 22.09.24 (10:30-12:30 Prague time)</h4>
<ol>
  <li></li>
  <li></li>
</ol>


<h4>Plan for 23.09.24 (??? Prague time)</h4>
<ol>
  <li>Authorise users</li>
  <li>Make sure nicks are unique</li>
</ol>


<h4>Plan for 24.09.24 (??? Prague time)</h4>
<ol>
  <li>Make buffers in user private</li>
  <li>Check if all functions are allowed</li>
</ol>

<h4>Plan for the next week</h4>
<ol>
  <li>Add User class - User verified by password and with user details and nick maybe a child class of User</li>
  <li>Proper command handling</li>
  <li>Error codes</li>
  <li>Logger/debugger used everywhere in the code</li>
  <li>Logger/debugger goes through select()</li>
</ol>

<h4>Plan for an unscheduled future</h4>
<ol>
  <li>Update client to test prefix, server validation of messages</li>
  <li>Create a second client. One will be automated another will take messages manually like telnet</li>
  <li>Read through the modern irc specification (especially commands)</li>
  <li>Proper password command check</li>
  <li>Process more commands make them work in all scenarios</li>
  <li>Fix/check memory leaks and free everything that needs freeing</li>
  <li>WHO</li>
  <li>PONG</li>
  <li>server pinging users?</li>
  <li>NOTICE ?</li>
  <li>Transfer files</li>
  <li>Bot (magic 8ball "magic8bot") from our client</li>
  <li>new</li>
  <li>new</li>
  <li>new</li>
  <li>new</li>
  <li>new</li>
  <li>new</li>
  <li>new</li>
</ol>
