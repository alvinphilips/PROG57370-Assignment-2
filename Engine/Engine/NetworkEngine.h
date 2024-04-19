#pragma once

#ifndef _NETWORK_ENGINE_H_
#define _NETWORK_ENGINE_H_

#include "NetworkCommon.h"

class NetworkEngine final
{
	DECLARE_SINGLETON(NetworkEngine)

public:
	enum class NetworkState
	{
		DISABLED,
		INITIALIZE_NETWORK,
		RUNNING,
	};
	bool IsInitialized() const { return initialized; }
	bool IsClient() const { return is_client; }
	bool IsServer() const { return is_server; }
	void SendPacket(const RakNet::BitStream& bs) const;
private:
	void Initialize(bool is_server);
	void PreUpdate();
	void PostUpdate() const;
	void InitializeNetwork();
	void ReceivePackets();
	void LoadSettings();
private:
	int port;
	std::string ipAddress;
	NetworkState state;
	bool is_client = false;
	bool is_server = false;
	RakNet::RakPeerInterface* rakInterface;
	bool initialized = false;
	std::vector<RakNet::RakNetGUID> connections;
	const unsigned int server_max_connections = 8;

	friend class Engine;
};

#endif
