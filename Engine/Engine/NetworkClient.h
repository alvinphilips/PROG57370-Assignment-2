#pragma once

#ifndef _NETWORK_CLIENT_H_
#define _NETWORK_CLIENT_H_

#include "NetworkCommon.h"

class NetworkClient final
{
	DECLARE_SINGLETON(NetworkClient)

public:
	enum NetworkClientState
	{
		DISABLED,
		WAITING_TO_CONNECT,
		WAITING_FOR_FIRST_PACKET,
		CONNECTED,
		NETWORK_ERROR
	};

	bool IsInitialized() { return initialized; }
	void SendPacket(RakNet::BitStream& bs);

protected:
	void Initialize();
	void Update();


private:
	void LoadSettings();
	void WaitingForFirstPacket();
	void _Update();

private:
	int port;
	bool initialized = false;
	std::string ipAddress;
	NetworkClientState state;

	RakNet::RakPeerInterface* rakInterface;
	RakNet::RakNetGUID serverGUID;


	friend class Engine;
};

#endif
