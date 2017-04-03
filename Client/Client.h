#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

class Client : public aie::Application {
public:

	Client();
	virtual ~Client();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	// NETWORK
	// SERVER CONNECTION
	void handleNetworkConnection();
	void initaliseClientConnection();
	// LISTEN FOR MESSAGES
	void handleNetworkMessages();
	// CREATE MESSAGE
	void chatMessages();
	// Client: User requests Client to send chat message packet
	// Server: Receives chat message, forward message to all connected users
	// Client system address:
	// systemAddress: User[Set username]
	// systemAddress: Iterate through users: [Query No. users in server]

protected:

	// NETWORK
	RakNet::RakPeerInterface* m_pPeerInterface;

	const char* IP = "127.0.0.1";
	const unsigned short PORT = 5456;

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
};