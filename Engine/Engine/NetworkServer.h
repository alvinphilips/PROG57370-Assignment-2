#pragma once

#ifndef _NETWORK_SERVER_H_
#define _NETWORK_SERVER_H_

#include "NetworkCommon.h"

class NetworkServer final
{
	DECLARE_SINGLETON(NetworkServer)

public:
	enum NetworkServerState
	{
		DISABLED,
		CREATE_SERVER,
		RUNNING
	};

	bool IsInitialized() { return initialized; }
	void SendPacket(RakNet::BitStream& bs);

protected:
	void Initialize();
	void Update();

private:
	void LoadSettings();
	void SetupServer();
	void _Update();

private:
	int port;
	bool initialized = false;
	NetworkServerState state;
	RakNet::RakPeerInterface* rakInterface;
	std::vector<RakNet::RakNetGUID> clientConnections;

	friend class Engine;
};

#endif
