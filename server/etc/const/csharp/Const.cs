/////////////////////////////////////////////////////
// do not modify the file, gen by const/gen.bat
/////////////////////////////////////////////////////
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

public class Const {
	//��Ϸ����
	public static ENTITY_STATE_NONE           = 1;              // 
	public static ENTITY_STATE_BORN           = 2;              // 
	public static ENTITY_STATE_IDLE           = 3;              // 
	public static ENTITY_STATE_FIRE           = 4;              // 
	public static ENTITY_STATE_DEATH          = 5;              // 
	public static ENTITY_STATE_DEL            = 6;              // 
	//�ӵ�
	public static BULLET_STATE_NONE           = 7;              // 
	public static BULLET_STATE_BORN           = 8;              // 
	public static BULLET_STATE_FLY            = 9;              // 
	public static BULLET_STATE_HIT            = 10;             // 
	public static BULLET_STATE_DEL            = 11;             // 
	//����
	public static ROOM_STATE_WAIT             = 1;              // 
	public static ROOM_STATE_ING              = 2;              // 
	public static ROOM_STATE_END              = 3;              // 
	//�ӵ��켣
	public static BULLET_PATCH_HORIZONTAL     = 1;              // 
	public static BULLET_PATCH_PARABOLA       = 2;              // 
	//����
	public static BULLET_MAX_LIFE_TM          = 20;             // ������ʱ��
	public static ROOM_WAIT_TIME              = 2;              // ��ʼʱ��
	public static ROOM_ING_TIME               = 300;            // ����ʱ��
	public static ROOM_END_TIME               = 5;              // ����ʱ��
}