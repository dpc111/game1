namespace Net
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Net;
    using System.Net.Sockets;
    using System.Threading;

    public class UdpMsg {
  		public static Dictionary<int, Type> msgMap = new Dictionary<int, Type>();

  		public static void Register(int msgid, Type msgType) {
  			msgMap[msgid] = msgType;
  		}

  		public static void Init() {

  		}

  		public static Type Proto(int msgid) {
  			Type type = null;
            if (!msgMap.TryGetValue(msgid, out type))
            {
                Debug.Log(msgid);
                return null;
            }
            return type
  		}
    }
}