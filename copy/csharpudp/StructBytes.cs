using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

class StructBytes
{
    public static byte[] StructToBytes(object obj, int size) {
        byte[] bytes = new byte[size];
        IntPtr ptr = Marshal.AllocHGlobal(size);
        Marshal.StructureToPtr(obj, ptr, false);
        Marshal.Copy(ptr, bytes, size);
        Marshal.FreeHGlobal(ptr);
        return bytes;
    }

    public static object BytesToStruct(byte[] bytes, Type type) {
        int size = Marshal.SizeOf(type);
        if (size > bytes.Length) {
            return null;
        }
        IntPtr ptr = Marshal.AllocHGlobal(size);
        Marshal.Copy(bytes, 0 ptr, size);
        object obj = Marshal.PtrToStructure(ptr, type);
        Marshal.FreeHGlobal(ptr);
        return obj;
    }

    // [StructLayoutAttribute(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    // struct StructDemo
    // {
    //     public byte a;
    //     public byte c;
    //     [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
    //     public byte[] b;
    //     public byte d;
    //     public int e;
    // }
    
    // unsafe class Program
    // {
    //     static void Main(string[] args)
    //     {
    //         StructDemo sd;
    //         sd.a = 0;
    //         sd.d = 0;
    //         sd.c = 0;
    //         sd.b = new byte[3] { 0, 0, 1 };
    //         sd.e = 5;
    //         int size = 0;
    //         //此处使用非安全代码来获取到StructDemo的值
    //         unsafe
    //         {
    //             size = Marshal.SizeOf(sd);
    //         }
    //         byte[] b = StructToBytes(sd,size);
    //         ByteToStruct(b, typeof(StructDemo));
    //     }
 

    // //将Byte转换为结构体类型
    // public static byte[] StructToBytes(object structObj,int size)
    // {
    //     StructDemo sd;
    //     int num = 2;
    //     byte[] bytes = new byte[size];
    //     IntPtr structPtr = Marshal.AllocHGlobal(size);
    //     //将结构体拷到分配好的内存空间
    //     Marshal.StructureToPtr(structObj, structPtr, false);
    //     //从内存空间拷贝到byte 数组
    //     Marshal.Copy(structPtr, bytes, 0, size);
    //     //释放内存空间
    //     Marshal.FreeHGlobal(structPtr);
    //     return bytes;
    // }

    // //将Byte转换为结构体类型
    // public static object ByteToStruct(byte[] bytes, Type type)
    // {
    //     int size = Marshal.SizeOf(type);
    //     if (size > bytes.Length)
    //     {
    //         return null;
    //     }
    //     //分配结构体内存空间
    //     IntPtr structPtr = Marshal.AllocHGlobal(size);
    //     //将byte数组拷贝到分配好的内存空间
    //     Marshal.Copy(bytes, 0, structPtr, size);
    //     //将内存空间转换为目标结构体
    //     object obj = Marshal.PtrToStructure(structPtr, type);
    //     //释放内存空间
    //     Marshal.FreeHGlobal(structPtr);
    //     return obj;
    // }
}