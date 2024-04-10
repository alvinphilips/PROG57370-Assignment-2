#pragma once
 
#ifndef _INETWORK_RPC_H_
#define _INETWORK_RPC_H

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

void NetworkRPC::RegisterRPC(STRCODE functionName, std::function<void(RakNet::BitStream&)> functionPtr)
{
	rpcFunctions.insert(std::pair<STRCODE, std::function<void(RakNet::BitStream&)>>(functionName, functionPtr));
}

void NetworkRPC::InvokeRPC(RakNet::BitStream& bitStream)
{
	STRCODE function;
	bitStream.Read(function);

	auto iter = rpcFunctions.find(function);;
	if (iter != rpcFunctions.end())
	{
		(*iter).second(bitStream);
	}
}



