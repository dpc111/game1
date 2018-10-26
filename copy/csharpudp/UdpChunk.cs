public struct UdpChunk {
	public byte size;
	public byte type;
	public int seq;
	public int ack;
	byte buff[512];
}