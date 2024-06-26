#pragma once
 
#ifndef _INETWORK_RPC_H_
#define _INETWORK_RPC_H_

class NetworkRPC
{
private: 
	std::map<STRCODE, std::function<void(RakNet::BitStream&)>> rpcFunctions;

public:
	void InvokeRPC(RakNet::BitStream& bitStream);

protected: 
	virtual void RegisterRPC(STRCODE functionName, std::function<void(RakNet::BitStream&)>functionPtr);
};

#endif



