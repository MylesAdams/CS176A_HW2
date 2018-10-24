This project has 4 programs.

1. server_c_udp
2. client_c_udp
3. server_c_tcp
4. client_c_tcp

For the UDP client/server pair of programs, the user can start the client before the server or
the server before the client. But, the server must be started before entering a string to send in
the client. Once the client has sent a string, the server will check if the string has any non
digit characters. If it does then it will just respond with, cannot compute and then start
waiting for another message. If it does receive all digit characters, it will sum up all the
digits and send that sum back to the client. It will then sum the digits of that new number it
has just sent, and repeat this process until the number it sends is 1 digit long. It will then
wait for another message.

For the TCP client/server pair of programs, it behaves the same way in terms of what is strings
are passed back and forth. One main different is that the server must be started before the
client. Now, for this TCP server, once it has sent the last 1 digit number it will close the
connection with the client and then begin to listen for a new TCP connection request. Once the
client detects that the connection with the server has been closed, it will shut down.

Something different I did with TCP is that for each message back and forth, I actually sent two
messages. I did this because for TCP, you must specify how many bytes you are receiving. So the
first message was always 4 bytes specifying how long the next message would be. That way the next
recv() call could specify exactly how many bytes it should receive, in order to receive the
string. This was done in order to avoid sending the entire buffer for every message.

Whereas, for the UDP client/server, whenever the client or server calls sendto() it will send a
specific amount of bytes. But on the recvfrom() call, I could just specify the max number of
bytes to receive, so this can all be done in a single message.

For running either of the servers the command is as follows:
./{server_program} {port}

For running either of the clients, the command is as follows:
./{client_program} {server_IP} {server_port}
