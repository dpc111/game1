using System.Runtime.InteropServices; 

/// 将结构转换为字节数组
/// 结构对象
/// 字节数组
public byte[] StructToBytes(object obj)
{
	//得到结构体的大小
	int size = Marshal.SizeOf(obj);
	//创建byte数组
	byte[] bytes = new byte[size];
	//分配结构体大小的内存空间
	IntPtr structPtr = Marshal.AllocHGlobal(size);
	//将结构体拷到分配好的内存空间
	Marshal.StructureToPtr(obj, structPtr, false);
	//从内存空间拷到byte数组
	Marshal.Copy(structPtr, bytes, 0, size);
	//释放内存空间
	Marshal.FreeHGlobal(structPtr);
	//返回byte数组
	return bytes;
}

// 接收的时候需要把字节数组转换成结构 
/// byte数组转结构
/// 
/// byte数组
/// 结构类型
/// 转换后的结构
public object BytesToStruct(byte[] bytes, Type type)
{
	//得到结构的大小
	int size = Marshal.SizeOf(type);
	Log(size.ToString(), 1);
	//byte数组长度小于结构的大小
	if (size > bytes.Length)
	{
	//返回空
	return null;
	}
	//分配结构大小的内存空间
	IntPtr structPtr = Marshal.AllocHGlobal(size);
	//将byte数组拷到分配好的内存空间
	Marshal.Copy(bytes, 0, structPtr, size);
	//将内存空间转换为目标结构
	object obj = Marshal.PtrToStructure(structPtr, type);
	//释放内存空间
	Marshal.FreeHGlobal(structPtr);
	//返回结构
	return obj;
}

// 4、实际操作： 

using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;

byte[] Message = StructToBytes(new Operator("user","pass")); // 将结构转换成字节数组

TcpClient socket = new TcpClient();

socket.Connect(ip,port);

NetworkStream ns = Socket.GetStream();

ns.Write(Message,0,Message.Length); // 发送

byte[] Recv = new byte[1024]; // 缓冲

int NumberOfRecv = 0;

IList newRecv = new List();
ns.ReadTimeout = 3000;
try
{
	do
	{
		// 接收响应
		NumberOfRecv = ns.Read(Recv, 0, Recv.Length);
		for (int i = 0; i < NumberOfRecv; i++)
			newRecv.Add(Recv);
	}
while (ns.DataAvailable);
byte[] resultRecv = new byte[newRecv.Count];
newRecv.CopyTo(resultRecv, 0);

Operator MyOper = new Operator();

MyOper = (Operator)BytesToStruct(resultRecv, MyOper.GetType()); // 将字节数组转换成结构

