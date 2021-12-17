# SOCKET

A single class, just like in Python to easily use Sockets on Windows and Linux.

## Class Socket:
Following methods are avaiable:
```cpp
bool Bind(std::string ip = "0.0.0.0", int port = 52);
bool Listen();
bool Accept();
bool Connect(std::string ip = "127.0.0.1", int port = 52);
bool Send(std::string data);
std::string Recv();
```

### Bind:
This method will bind the provided IP and Port to the socket internally.

### Listen:
This method will listen for incoming connections.
We can change the number of conncurrent connections by modifying the macro `MAX_CONNCURRENT_CONNECTIONS` to an integer in range `( 1 - 10 )`.

### Accept:
This method will accept the incoming connection.

### Connect:
This will be used to connect to a server.

### Send:
This is used to send data.

### Recv:
Receiving data from the server/client. Also, in order to change the size of data to be read, modify the macro `BUFFER_SIZE` to your liking.


## Example Codes:
### Server:

```cpp
#define MAX_CONNCURRENT_CONNECTIONS 1

#include "socket.h"

int main() {
	
	Socket sock;
	
	sock.Bind("0.0.0.0", 9001);
	sock.Listen();
	sock.Accept();

	std::cout << "[*] Sending data to client!" << std::endl;
	sock.Send("This is server!");
	std::cout << "[+] Received data from client: " << sock.Recv() << std::endl;

	return 0;
}
```

### Client:

```cpp
#define BUFFER_SIZE 1024

#include "socket.h"

int main() {
	
	Socket sock;
	sock.Connect("127.0.0.1", 9001);

	std::cout << "[+] Received data from server: " << sock.Recv() << std::endl;
	std::cout << "[*] Sending data to server!" << std::endl;
	sock.Send("This is client!");

	return 0;
}
```
