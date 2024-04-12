#pragma once

#ifndef _NETWORK_ENGINE_H_
#define _NETWORK_ENGINE_H_

#include "NetworkCommon.h"

class NetworkEngine final
{
	DECLARE_SINGLETON(NetworkEngine)

public:
	enum NetworkState
	{
		DISABLED,
		WAITING_TO_CONNECT,
		WAITING_FOR_FIRST_PACKET,
		NETWORK_ERROR,
		CREATE_SERVER,
		RUNNING,
	};
	void Initialize(bool is_server);
	bool IsInitialized() const { return initialized; }
	bool IsClient() const { return is_client; }
	bool IsServer() const { return is_server; }
	void SendPacket(RakNet::BitStream& bit_stream);
	void PreUpdate();
private:
	void WaitingForFirstPacket();
	void SetupServer();
	void LoadSettings();
	void _ClientUpdate();
	void _UpdateClient();
	void _ServerUpdate();
	void _UpdateServer();
private:
	int port;
	std::string ipAddress;
	NetworkState state;
	bool is_client = false;
	bool is_server = false;
	RakNet::RakPeerInterface* rakInterface;
	RakNet::RakNetGUID serverGUID;
	bool initialized = false;
	std::vector<RakNet::RakNetGUID> connections;

	friend class Engine;
};

#endif
