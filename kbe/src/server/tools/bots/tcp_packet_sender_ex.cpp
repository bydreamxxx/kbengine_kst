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


#include "tcp_packet_sender_ex.h"
#include "clientobject.h"
#include "bots.h"

#include "network/address.h"
#include "network/bundle.h"
#include "network/channel.h"
#include "network/endpoint.h"
#include "network/event_dispatcher.h"
#include "network/network_interface.h"
#include "network/event_poller.h"
#include "network/error_reporter.h"
#include "network/tcp_packet.h"
#include "network/udp_packet.h"

namespace KBEngine { 
namespace Network
{


//-------------------------------------------------------------------------------------
TCPPacketSenderEx::TCPPacketSenderEx(EndPoint & endpoint,
	   NetworkInterface & networkInterface, ClientObject* pClientObject) :
	TCPPacketSender(endpoint, networkInterface),
	pClientObject_(pClientObject)
{
}

//-------------------------------------------------------------------------------------
TCPPacketSenderEx::~TCPPacketSenderEx()
{
	//DEBUG_MSG("TCPPacketSenderEx::~TCPPacketSenderEx()\n");
}

//-------------------------------------------------------------------------------------
Channel* TCPPacketSenderEx::getChannel()
{
	return pClientObject_->pServerChannel();
}

//-------------------------------------------------------------------------------------
void TCPPacketSenderEx::onGetError(Channel* pChannel, const std::string& err)
{
	pClientObject_->destroy();
}

//-------------------------------------------------------------------------------------
}
}

