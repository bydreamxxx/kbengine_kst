/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2018 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KBE_NETWORKUDPPACKET_RECEIVER_H
#define KBE_NETWORKUDPPACKET_RECEIVER_H

#include "common/common.h"
#include "common/timer.h"
#include "common/objectpool.h"
#include "helper/debug_helper.h"
#include "network/common.h"
#include "network/interfaces.h"
#include "network/udp_packet.h"
#include "network/packet_receiver.h"

namespace KBEngine { 
namespace Network
{
class Socket;
class Channel;
class Address;
class NetworkInterface;
class EventDispatcher;

class UDPPacketReceiver : public PacketReceiver
{
public:
	typedef KBEShared_ptr< SmartPoolObject< UDPPacketReceiver > > SmartPoolObjectPtr;
	static SmartPoolObjectPtr createSmartPoolObj(const std::string& logPoint);
	static ObjectPool<UDPPacketReceiver>& ObjPool();
	static UDPPacketReceiver* createPoolObject(const std::string& logPoint);
	static void reclaimPoolObject(UDPPacketReceiver* obj);
	static void destroyObjPool();

	UDPPacketReceiver() noexcept: PacketReceiver(){}
	UDPPacketReceiver(EndPoint & endpoint, NetworkInterface & networkInterface);
	virtual ~UDPPacketReceiver();

	Reason processFilteredPacket(Channel* pChannel, Packet * pPacket);
	
	virtual PacketReceiver::PACKET_RECEIVER_TYPE type() const
	{
		return UDP_PACKET_RECEIVER;
	}

	virtual ProtocolSubType protocolSubType() const {
		return SUB_PROTOCOL_UDP;
	}

	virtual bool processRecv(bool expectingPacket);
	virtual bool processRecv(UDPPacket* pReceiveWindow);

	virtual Channel* findChannel(const Address& addr);

protected:
	PacketReceiver::RecvState checkSocketErrors(int len, bool expectingPacket);

protected:

};

}
}

#ifdef CODE_INLINE
#include "udp_packet_receiver.inl"
#endif
#endif // KBE_NETWORKUDPPACKET_RECEIVER_H
