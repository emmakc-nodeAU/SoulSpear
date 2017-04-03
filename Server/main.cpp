#include <iostream>
#include <string>

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

int main()
{
	// CREATE SERVER ----------------------------------
	// RakNet Server: Get Handle, listen to set port, allow set connections
	const unsigned short PORT = 5456;
	RakNet::RakPeerInterface* pPeerInterface = nullptr;

	// STARTUP SERVER: Start listening for clients
	std::cout << "Cranking up the BOSSMODE..the server is active..." << std::endl;

	// INITIALIZE RakNet Peer Interface
	pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	// CREATE: Socket descriptor for this connection
	RakNet::SocketDescriptor sd(PORT, 0);

	// STARTUP: Max 32 Connections, on assigned port
	pPeerInterface->Startup(32, &sd, 1);
	pPeerInterface->SetMaximumIncomingConnections(32);

	// RECEIVE PACKETS ---------------------------------
	RakNet::Packet* packet = nullptr;

	while (true)
	{
		for (packet = pPeerInterface->Receive(); packet;
			pPeerInterface->DeallocatePacket(packet),
			packet = pPeerInterface->Receive())
		{
			switch (packet->data[0])
			{
			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "A connection is incoming.\n";
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "A client has disconnected.\n";
				break;
			case ID_CONNECTION_LOST:
				std::cout << "A client lost the connection. \n";
				break;
			default:
				std::cout << "Receive a message with unknown id: " << packet->data[0];
				break;
			}
		}
	}

	return 0;
}