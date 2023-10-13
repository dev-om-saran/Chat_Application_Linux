# Chat_Application_Linux
Terminal Based Chat Application in Linux using Socket Programming with client to client encryption using RC4 and Diffie Hellman key Exchange Algorithm.

To simulate the chat application, we will run server and each client in a
separate terminal, where each terminal can be treated as a separate computer.

• Steps :
1. compile server.cpp as g++ server.cpp -o server
2. compile client.cpp as g++ client.cpp -o client
3. In a new terminal, run server application as ./server
4. In new separate terminals, run multiple client applications as ./client
5. For each client, enter your username to get connected to server, and then
each client has following set of valid commands to proceed:
➢ status : List all the users and their status (free or busy)
➢ connect <username> : Connect to username and start chatting
➢ goodbye : Ends current chatting session
➢ close : Disconnects you from the server
➢ Ctrl+C (client side) : Ends current chatting session (if present) and
disconnects and terminates the client.
➢ Ctrl+C (server side) : Ends all the chatting sessions and disconnects
all the clients and terminates the server.

Once a client gets connected to the other client, all the messages typed in the
terminal by either of the clients (except commands) are sent to the other
client.

• The private key is taken as input from each client along with the
username as soon as a client application starts. The key is generated and
and sent to server along with the username. The server stores the key in
an unordered map of strings to strings (client_username : client_key).

• When two clients are successfully connected to each other, the server
sends the key of one client to the other. Each client then on its end
generate the secret key using the key of other client and its private key.
This shared secret key is used to encrypt and decrypt the messages by
the two clients.

Only the messages sent from one client to other (i.e. after the connection is
established) are encrypted and sent. The messages between client-server are
not encrypted.
