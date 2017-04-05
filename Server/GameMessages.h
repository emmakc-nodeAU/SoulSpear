#pragma once
#include "MessageIdentifiers.h"

#include <RakPeerInterface.h>
#include <thread>
#include <chrono>
#include <BitStream.h>

enum GameMessages
{
	// CREATE: Packet ID's derived from the server
	ID_SERVER_TEXT_MESSAGE = ID_USER_PACKET_ENUM + 1
};

// GAME MESSAGES
void sendClientPing(RakNet::RakPeerInterface* pPeerInterface)
{
	while (true)
	{
		RakNet::BitStream bs;
		bs.Write((RakNet::MessageID)GameMessages::ID_SERVER_TEXT_MESSAGE);
		bs.Write("Ping!");

		pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
			RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}