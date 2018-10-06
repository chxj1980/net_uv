#pragma once

NS_NET_UV_BEGIN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �׽�����С���ջ����С
#define KCP_UV_SOCKET_RECV_BUF_LEN (1024 * 4)
// �׽�����С���ͻ����С
#define KCP_UV_SOCKET_SEND_BUF_LEN (1024 * 4)

// ����Ϣ����ʹ�С
// �����Ϣͷ�ĳ����ֶδ��ڸ�ֵ
// ��ֱ���϶�Ϊ�ÿͻ��˷��͵���ϢΪ�Ƿ���Ϣ
// (4MB)
#define KCP_BIG_MSG_MAX_LEN (1024 * 1024 * 4)

// ������Ϣ��������ֽ�
// �������ó��ȣ�����з�Ƭ����
// (1K)
#define KCP_WRITE_MAX_LEN (1024 * 1)

// IP��ַ����
#define KCP_IP_ADDR_LEN (32)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��ϢУ�� 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ��Ϣ����md5У��
#define KCP_UV_OPEN_MD5_CHECK 1
// У������
#define KCP_UV_ENCODE_KEY "net_uv_KCP_md5_key"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �Ƿ�����UV�߳̽�������У��
// ����رո�ѡ�����Ҫ��Ӧ�ò��Լ�������У��
#define KCP_OPEN_UV_THREAD_HEARTBEAT 0

#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
#define KCP_HEARTBEAT_TIMER_DELAY (400)		// ������ⶨʱ�����
#define KCP_HEARTBEAT_CHECK_DELAY (1200)	// �������ʱ��
#define KCP_HEARTBEAT_MAX_COUNT_SERVER 3	// �������ظ�������(�����)
#define KCP_HEARTBEAT_MAX_COUNT_CLIENT 3	// �������ظ�������(�ͻ���)

// ����������������ֵ(�����) С��0 
// ������˸�ֵ�ȿͻ���Сʱ����������һ���ɿͻ��˷��ͣ�����˽��лظ�
#define KCP_HEARTBEAT_COUNT_RESET_VALUE_SERVER (-2) 
// ����������������ֵ(�ͻ���) С��0
#define KCP_HEARTBEAT_COUNT_RESET_VALUE_CLIENT (-1)	
#endif

#define KCP_HEARTBEAT_MSG_C2S ((unsigned int)0)		// �ͻ���->����������̽����Ϣ
#define KCP_HEARTBEAT_MSG_S2C ((unsigned int)1)		// ������->�ͻ�������̽����Ϣ
#define KCP_HEARTBEAT_RET_MSG_C2S ((unsigned int)2) // �ͻ���->�����������ظ���Ϣ
#define KCP_HEARTBEAT_RET_MSG_S2C ((unsigned int)3) // ������->�ͻ��������ظ���Ϣ
#define KCP_HEARTBEAT_MSG_SIZE sizeof(unsigned int)	// ������Ϣ��С


NS_NET_UV_END