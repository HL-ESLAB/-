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
	DataBUF[cnt++]=0xAA;									//帧头
	DataBUF[cnt++]=0xF1;									//目标地址
	DataBUF[cnt++]=0x01;									//功能码
	DataBUF[cnt++]=0x04;									//数据长度
	
	DataBUF[cnt++]=ShengLi_ADC;			//数据内容,小段模式，低位在前
	DataBUF[cnt++]=ShengLi_ADC>>8;			
	DataBUF[cnt++]=NiuLi_ADC;				
	DataBUF[cnt++]=NiuLi_ADC>>8;
	
	//SC和AC的校验直接抄最上面上面简介的即可
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
	}	//串口逐个发送数据
}


///*帧解析函数*/
//unsigned char UserControl;
//void FrameAnalysis(_RingBuffer *ringbuffer)
//{
//		unsigned char *RxFrameData;/*存储解析帧的空间*/
//		unsigned short FrameDataLen = USART1_RxRingBuffer.Lenght;
//		if(xPortGetFreeHeapSize() >= FrameDataLen)
//		{
//			RxFrameData = (unsigned char *)pvPortMalloc(FrameDataLen);	/*为读取缓冲区中的所有数据申请内存*/
//			if(ReadBytes(ringbuffer,RxFrameData,FrameDataLen) == 1)		/*读取缓冲区中的所有数据*/
//			{
//				/*读取到缓冲区的数据*/
//				for(int i=0;i<FrameDataLen;i++)	/*遍历*/
//				{
//					if(RxFrameData[i] == 0xAA)			/*找帧头*/
//					{
//						if(FrameDataLen - i <= 3)		/*帧头后面无数据长度*/
//						{
//							WriteBytes(ringbuffer,&RxFrameData[i],FrameDataLen - i);/*未处理的数据写回缓冲区*/
//							vPortFree(RxFrameData);  //释放内存
//							return ;/*返回空数据*/
//						}
//						
//						unsigned short FrameLen = RxFrameData[i+3];			/*得到帧数据内容长度*/
//						
//						if((FrameDataLen - i) >= (4 + FrameLen + 2))	/*剩余数据是否还有一帧的长度*/
//						{
//							if(FrameDataCheck( &RxFrameData[i] ) == 1)		/*校验帧数据*/
//							{
//								unsigned short OneFrameDataLen = 4 + FrameLen + 2;			/*返回帧总长*/
//								if(RxFrameData[i+1] == 0xA5)														/*是上位机发送的数据*/
//								{
//									if(RxFrameData[i+2] == 0x01)							/*命令帧*/
//									{
//										UserControl = FrameCMD(&RxFrameData[i]);
//										if(UserControl == 0x01)
//										{
//											LED2=~LED2;
//										}
//									}
//									else if(RxFrameData[i+2] == 0x02)							/*MOS管栅极电压控制帧*/
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
//									else if(RxFrameData[i+2] == 0x03)						/*PID设置帧*/
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
//								i += OneFrameDataLen;										/*偏移*/
//							}
//						}
//						else
//						{
//							WriteBytes(ringbuffer,&RxFrameData[i],FrameDataLen - i);/*未处理的数据写回缓冲区*/
//							vPortFree(RxFrameData);  //释放内存
//							return ;
//						}
//					}
//				}
//			}
//			vPortFree(RxFrameData);  //释放内存
//		}
//	
//	

//}
///*帧校验*/
//unsigned char FrameDataCheck(unsigned char* OneFrameData)
//{
//	unsigned char sumcheck = 0;
//	unsigned char addcheck = 0;
//	unsigned char Len = OneFrameData[3];
//	
//	for(unsigned char i=0; i < Len + 4 ; i++)
//	{
//			sumcheck += OneFrameData[i]; //从帧头开始，对每一字节进行求和，直到DATA区结束
//			addcheck += sumcheck; //每一字节的求和操作，进行一次sumcheck的累加
//	}
//	
//	if(sumcheck == OneFrameData[Len + 4] && addcheck == OneFrameData[Len + 5])
//	{
//			return 1; //校验通过
//	}
//	else
//	{
//			return 0; //校验失败
//	}
//}

///*解析命令*/
///*
//这里自定义的上位机命令帧构成为：
//帧头				0xAA
//帧地址				0xA6
//帧ID				0x01
//帧内容长度		0x01
//帧内容				一字节CMD
//校验字节1		一字节
//校验字节2		一字节
//*/
//unsigned char FrameCMD(unsigned char* OneFrameData)
//{
//	return OneFrameData[4];
//}

