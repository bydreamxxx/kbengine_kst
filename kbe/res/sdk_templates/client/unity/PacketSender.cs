﻿namespace KBEngine
{
	using System; 
	using System.Net.Sockets; 
	using System.Net; 
	using System.Collections; 
	using System.Collections.Generic;
	using System.Text;
	using System.Text.RegularExpressions;
	using System.Threading;

	using MessageID = System.UInt16;
	using MessageLength = System.UInt16;
	
	/*
		包发送模块(与服务端网络部分的名称对应)
		处理网络数据的发送
	*/
	public class PacketSender 
	{
		public delegate void AsyncSendMethod();

		private byte[] _buffer;

		int _wpos = 0;				// 写入的数据位置
		int _spos = 0;				// 发送完毕的数据位置
		
		object _sendingObj = new object();
		Boolean _sending = false;
		
		private NetworkInterface _networkInterface = null;
		AsyncCallback _asyncCallback = null;
		AsyncSendMethod _asyncSendMethod;
		
		public PacketSender(NetworkInterface networkInterface)
		{
			_init(networkInterface);
		}

		~PacketSender()
		{
			Dbg.DEBUG_MSG("PacketSender::~PacketSender(), destroyed!");
		}

		void _init(NetworkInterface networkInterface)
		{
			_networkInterface = networkInterface;
			
			_buffer = new byte[KBEngineApp.app.getInitArgs().SEND_BUFFER_MAX];
			_asyncSendMethod = new AsyncSendMethod(this._asyncSend);
			_asyncCallback = new AsyncCallback(_onSent);
			
			_wpos = 0; 
			_spos = 0;
			_sending = false;
		}

		public NetworkInterface networkInterface()
		{
			return _networkInterface;
		}

		public bool send(MemoryStream stream)
		{
			int dataLength = (int)stream.length();
			if (dataLength <= 0)
				return true;

			Monitor.Enter(_sendingObj);
			if (!_sending)
			{
				if (_wpos == _spos)
				{
					_wpos = 0;
					_spos = 0;
				}
			}

			int t_spos = _spos;
			int space = 0;
			int tt_wpos = _wpos % _buffer.Length;
			int tt_spos = t_spos % _buffer.Length;
			
			if(tt_wpos >= tt_spos)
				space = _buffer.Length - tt_wpos + tt_spos - 1;
			else
				space = tt_spos - tt_wpos - 1;

			if (dataLength > space)
			{
				Dbg.ERROR_MSG("PacketSender::send(): no space, Please adjust 'SEND_BUFFER_MAX'! data(" + dataLength 
					+ ") > space(" + space + "), wpos=" + _wpos + ", spos=" + t_spos);
				
				return false;
			}

			int expect_total = tt_wpos + dataLength;
			if(expect_total <= _buffer.Length)
			{
				Array.Copy(stream.data(), stream.rpos, _buffer, tt_wpos, dataLength);
			}
			else
			{
				int remain = _buffer.Length - tt_wpos;
				Array.Copy(stream.data(), stream.rpos, _buffer, tt_wpos, remain);
				Array.Copy(stream.data(), stream.rpos + remain, _buffer, 0, expect_total - _buffer.Length);
			}

			_wpos += dataLength;

			if (!_sending)
			{
				_sending = true;
				Monitor.Exit(_sendingObj);

				_startSend();
			}
			else
			{
				Monitor.Exit(_sendingObj);
			}

			return true;
		}

		void _startSend()
		{
			// 由于socket用的是非阻塞式，因此在这里不能直接使用socket.send()方法
			// 必须放到另一个线程中去做
			_asyncSendMethod.BeginInvoke(_asyncCallback, _asyncSendMethod);
		}

		void _asyncSend()
		{
			if (_networkInterface == null || !_networkInterface.valid())
			{
				Dbg.WARNING_MSG("PacketSender::_asyncSend(): network interface invalid!");
				return;
			}

			var socket = _networkInterface.sock();

			while (true)
			{
				Monitor.Enter(_sendingObj);

				int sendSize = _wpos - _spos;
				int t_spos = _spos % _buffer.Length;
				if (t_spos == 0)
					t_spos = sendSize;

				if (sendSize > _buffer.Length - t_spos)
					sendSize = _buffer.Length - t_spos;

				int bytesSent = 0;
				try
				{
					bytesSent = socket.Send(_buffer, _spos % _buffer.Length, sendSize, 0);
				}
				catch (SocketException se)
				{
					Dbg.ERROR_MSG(string.Format("PacketSender::_asyncSend(): send data error, disconnect from '{0}'! error = '{1}'", socket.RemoteEndPoint, se));
					Event.fireIn("_closeNetwork", new object[] { _networkInterface });

					Monitor.Exit(_sendingObj);
					return;
				}

				_spos += bytesSent;

				// 所有数据发送完毕了
				if (_spos == _wpos)
				{
					_sending = false;
					Monitor.Exit(_sendingObj);
					return;
				}

				Monitor.Exit(_sendingObj);
			}
		}
		
		private static void _onSent(IAsyncResult ar)
		{
			AsyncSendMethod caller = (AsyncSendMethod)ar.AsyncState;
			caller.EndInvoke(ar);
		}
	}
} 
