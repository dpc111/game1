public class Rudp {
	public LinkedList<UdpChunk> recvQueue = new LinkedList<UdpChunk>();
	public int recvMaxSeq;
	public int recvMinSeq;
	public int recvMaxAck;

	public LinkedList<UdpChunk> sendQueue = new LinkedList<UdpChunk>();
	public LinkedList<UdpChunk> sendHistory = new LinkedList<UdpChunk>();
	public int sendSeq;

	public long curTick;
	public long recvTick;
	public long reqTick;
	public int reqTimes;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PushQueueFront
	// AddFirst
	
	// PushQueueBack
	// AddLast
	
	// PushQueueAfter
	// AddAfter
	
	public void PushQueueBackSort(LinkedList<UdpChunk> q, UdpChunk c) {
		LinkedListNode<UdpChunk> n = q.Last;
		while (n != null) {
			if (n.Value.seq <= c.seq) {
				q.AddAfter(n, c);
				return;
			}
			n = n.Previous;
		}
		q.AddLast(c);
	}

	public bool PushQueueBackSortCheckSame(LinkedList<UdpChunk> q, UdpChunk c) {
		LinkedListNode<UdpChunk> n = q.Last;
		while (n != null) {
			if (n.Value.seq == c.seq) {
				return false;
			}
			if (n.Value.seq < c.seq) {
				q.AddAfter(n, c);
				return true;
			}
			n = n.Previous;
		}
		q.AddLast(c);
		return true;
	}

	// PopQueueFront
	// First RemoveFirst
	
	public UdpChunk PopQueueBySeq(LinkedList<UdpChunk> q, int seq) {
		LinkedListNode<UdpChunk> n = q.Last;
		while (n != null) {
			if (n.Value.seq == seq) {
				q.Remove(n);
				return n.Value;
			}
			n = n.Previous;
		}
		return null;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	public void RecvMinSeqUpdate() {
		LinkedListNode<UdpChunk> n = recvQueue.First;
		while (n != null) {
			if (n.Value.seq > recvMinSeq) {
				if (n.Value.seq == recvMinSeq + 1) {
					recvMinSeq = n.Value.seq;
					h.reqTimes = 0;
				} else {
					return;
				}
			}
			n = n.Next;
		}
	}

	// UdpRecvBuffIn
	
	public UdpRecvBuffInProcess(int sz) {

	}

	public UdpRecvBuffWrite(UdpChunk c) {
		switch (c.type) {
			case UdpConst.udpTypeReqSeq:
				{
					UdpChunk cAgain = PopQueueBySeq(sendHistory, c.ack);
					if (cAgain == null) {
						break;
					}
					sendQueue.AddFirst(cAgain);
				}
			case UdpConst.udpTypeSynAck:
				break;
			case UdpConst.udpTypeData:
			case UdpConst.udpTypeDataAgain:
				if (recvMinSeq <= c.seq) {
					break;
				}
				if (PushQueueBackSortCheckSame(recvQueue, c)) {
					// recv cur in = null
					if (recvMinSeq < c.seq) {
						RecvMinSeqUpdate();
					}
					if (recvMaxSeq < c.seq) {
						recvMaxSeq = c.seq;
					}
					if (recvMaxAck < c.ack) {
						recvMaxAck = c.ack;
						SendHistoryClear();
					}
					recvTick = curTick;
				}
			default :
				break;
		}
	}

	// UdpRecvBuffOut
	// UdpRecvBuffOutProcess
	
	UdpChunk UdpRecvBuffOut() {
		
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	public void SendHistoryClear() {
		LinkedListNode<UdpChunk> n = sendHistory.First;
		while (n != null && n.Value.seq <= recvMaxAck) {
			sendHistory.Remove(n);
			n = sendHistory.First;
		}
	}
}