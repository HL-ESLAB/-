#include "My_Packet.h"
#include <string.h>
#include "usart.h"


unsigned char DataBUF[50];

void usart_senddata(USART_TypeDef * USARTx,
	unsigned short ShengLi_ADC,unsigned short NiuLi_ADC,unsigned short Speed)
{
	int i;
	unsigned char sumcheck = 0;
	unsigned char addcheck = 0;
	
	unsigned char cnt=0;
	DataBUF[cnt++]=0xAA;									//֡ͷ
	DataBUF[cnt++]=0xF1;									//Ŀ���ַ
	DataBUF[cnt++]=0x01;									//������
	DataBUF[cnt++]=0x04;									//���ݳ���
	
	DataBUF[cnt++]=ShengLi_ADC;			//��������,С��ģʽ����λ��ǰ
	DataBUF[cnt++]=ShengLi_ADC>>8;			
	DataBUF[cnt++]=NiuLi_ADC;				
	DataBUF[cnt++]=NiuLi_ADC>>8;
	
	//SC��AC��У��ֱ�ӳ�������������ļ���
	for(i=0;i<DataBUF[3]+4;i++) 
	{
		sumcheck+=DataBUF[i];
		addcheck+=sumcheck;
	}
	DataBUF[cnt++]=sumcheck;	
	DataBUF[cnt++]=addcheck;	
	for(i=0;i<cnt;i++)
	{
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);	
			USART_SendData(USARTx,DataBUF[i]);
	}	//���������������
}


///*֡��������*/
//unsigned char UserControl;
//void FrameAnalysis(_RingBuffer *ringbuffer)
//{
//		unsigned char *RxFrameData;/*�洢����֡�Ŀռ�*/
//		unsigned short FrameDataLen = USART1_RxRingBuffer.Lenght;
//		if(xPortGetFreeHeapSize() >= FrameDataLen)
//		{
//			RxFrameData = (unsigned char *)pvPortMalloc(FrameDataLen);	/*Ϊ��ȡ�������е��������������ڴ�*/
//			if(ReadBytes(ringbuffer,RxFrameData,FrameDataLen) == 1)		/*��ȡ�������е���������*/
//			{
//				/*��ȡ��������������*/
//				for(int i=0;i<FrameDataLen;i++)	/*����*/
//				{
//					if(RxFrameData[i] == 0xAA)			/*��֡ͷ*/
//					{
//						if(FrameDataLen - i <= 3)		/*֡ͷ���������ݳ���*/
//						{
//							WriteBytes(ringbuffer,&RxFrameData[i],FrameDataLen - i);/*δ���������д�ػ�����*/
//							vPortFree(RxFrameData);  //�ͷ��ڴ�
//							return ;/*���ؿ�����*/
//						}
//						
//						unsigned short FrameLen = RxFrameData[i+3];			/*�õ�֡�������ݳ���*/
//						
//						if((FrameDataLen - i) >= (4 + FrameLen + 2))	/*ʣ�������Ƿ���һ֡�ĳ���*/
//						{
//							if(FrameDataCheck( &RxFrameData[i] ) == 1)		/*У��֡����*/
//							{
//								unsigned short OneFrameDataLen = 4 + FrameLen + 2;			/*����֡�ܳ�*/
//								if(RxFrameData[i+1] == 0xA5)														/*����λ�����͵�����*/
//								{
//									if(RxFrameData[i+2] == 0x01)							/*����֡*/
//									{
//										UserControl = FrameCMD(&RxFrameData[i]);
//										if(UserControl == 0x01)
//										{
//											LED2=~LED2;
//										}
//									}
//									else if(RxFrameData[i+2] == 0x02)							/*MOS��դ����ѹ����֡*/
//									{
////										unsigned short MOS1_Voltage = 3300 - (RxFrameData[i+4] |  RxFrameData[i+5]<<8);
////										unsigned short MOS2_Voltage = 3300 - (RxFrameData[i+6] |  RxFrameData[i+7]<<8);
////										unsigned short MOS3_Voltage = 3300 - (RxFrameData[i+8] |  RxFrameData[i+9]<<8);
//										unsigned short MOS1_Voltage = (RxFrameData[i+4] |  RxFrameData[i+5]<<8);
//										unsigned short MOS2_Voltage = (RxFrameData[i+6] |  RxFrameData[i+7]<<8);
//										unsigned short MOS3_Voltage = (RxFrameData[i+8] |  RxFrameData[i+9]<<8);
//										TIM_SetCompare1(TIM1,MOS1_Voltage);
//										TIM_SetCompare2(TIM1,MOS2_Voltage);
//										TIM_SetCompare3(TIM1,MOS3_Voltage);
//									}
//									else if(RxFrameData[i+2] == 0x03)						/*PID����֡*/
//									{
//										PID.Kp = (float)(short)(RxFrameData[i+4] |  RxFrameData[i+5]<<8) / 100.0f;
//										PID.Ki = (float)(short)(RxFrameData[i+6] |  RxFrameData[i+7]<<8) / 100.0f;
//										PID.Kd = (float)(short)(RxFrameData[i+8] |  RxFrameData[i+9]<<8) / 100.0f;
//										
//									}
//									else if(RxFrameData[i+2] == 0x04)		
//									{
//										Data.TargetI1_ADC = (RxFrameData[i+4] |  RxFrameData[i+5]<<8);
//										Data.TargetI2_ADC = (RxFrameData[i+6] |  RxFrameData[i+7]<<8);
//										Data.TargetI3_ADC = (RxFrameData[i+8] |  RxFrameData[i+9]<<8);
//									}
//								}
//								i += OneFrameDataLen;										/*ƫ��*/
//							}
//						}
//						else
//						{
//							WriteBytes(ringbuffer,&RxFrameData[i],FrameDataLen - i);/*δ���������д�ػ�����*/
//							vPortFree(RxFrameData);  //�ͷ��ڴ�
//							return ;
//						}
//					}
//				}
//			}
//			vPortFree(RxFrameData);  //�ͷ��ڴ�
//		}
//	
//	

//}
///*֡У��*/
//unsigned char FrameDataCheck(unsigned char* OneFrameData)
//{
//	unsigned char sumcheck = 0;
//	unsigned char addcheck = 0;
//	unsigned char Len = OneFrameData[3];
//	
//	for(unsigned char i=0; i < Len + 4 ; i++)
//	{
//			sumcheck += OneFrameData[i]; //��֡ͷ��ʼ����ÿһ�ֽڽ�����ͣ�ֱ��DATA������
//			addcheck += sumcheck; //ÿһ�ֽڵ���Ͳ���������һ��sumcheck���ۼ�
//	}
//	
//	if(sumcheck == OneFrameData[Len + 4] && addcheck == OneFrameData[Len + 5])
//	{
//			return 1; //У��ͨ��
//	}
//	else
//	{
//			return 0; //У��ʧ��
//	}
//}

///*��������*/
///*
//�����Զ������λ������֡����Ϊ��
//֡ͷ				0xAA
//֡��ַ				0xA6
//֡ID				0x01
//֡���ݳ���		0x01
//֡����				һ�ֽ�CMD
//У���ֽ�1		һ�ֽ�
//У���ֽ�2		һ�ֽ�
//*/
//unsigned char FrameCMD(unsigned char* OneFrameData)
//{
//	return OneFrameData[4];
//}

