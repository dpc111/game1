class UdpChunk {
	public byte size;
	public byte type;
	public int seq;
	public int ack;
	public byte buff[512];
}