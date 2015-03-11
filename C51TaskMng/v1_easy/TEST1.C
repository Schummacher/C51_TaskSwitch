/*
1)  ����������ֲ�ԣ���CPU�޹أ������κ�֧��C���Ա�̵�CPU�������ã������Ľ�����51��Ƭ��Ϊ�����ѣ���ʵ���Ͽ���������ֲ��
2)  С֮��С�� ԭ���ܼ򵥣�һ���Ͷ���
3)  ʡ֮��ʡ�� ����˵��RAM��ROMʡ�����¡�
4)  ȡprotothread֮����������ʱ����״̬����α�߳��﷨�ںϵ�һ����ܣ�����������������д����
5)  ���ڶ�ʱ������������Ч�ʸߣ���󻯼�����Ч�Ĵ�������ʱ�䡣
***********************************************************/  
#include <STC12C2052AD.H>
#include <stdio.h>


/*****************СС���������ֿ�ʼ********************************************/ 
#define  _SS   static char lc=0; switch(lc){   case 0: lc=0;
#define  _EE   }; lc=0; 
#define  WaitX(a,b)  settimer(&lc,__LINE__,a,b); return ; case __LINE__:
struct TASK {
  char td;
  void (*fp)();
};
#define MAXTASKS 5
struct TASK tasks[MAXTASKS];

//���ö�ʱ��
void settimer(char *lc,char  line,char  tmrid,int d)
{
  *lc=line;
  tasks[tmrid].td=d;
}

//�߼���ʱ���������ڶ�ʱ���ж������
void dectimers() 
{ 
	unsigned char i;   
	for (i=0;i<MAXTASKS;i++){ 
	   if (tasks[i].td>0)  tasks[i].td--;  
	}
}
//������Ⱥ�������main��������
void runtasks() 
{
	unsigned char i;    
	for(i=0;i<MAXTASKS;i++)        
	{   
		if (tasks[i].fp!=0)
		{    
			if (tasks[i].td==0)
			{
				tasks[i].td=-1;  
				tasks[i].fp();
			}  
		}         
	}
}
/****************СС���������ֽ���*******************************************************/


sbit KEY = P3^2;
unsigned char code numtab[16]={0x24,0x6F,0xE0,0x62,0x2B,0x32,0x30,0x67,0x20,0x22,0x21,0x38,0xB4,0x68,0xB0,0xB1};


//��ʼ����ʱ��
void InitT0()
{
        TMOD = 0x21;
        IE |= 0x82;  // 12t
        TL0=0Xff;
        TH0=0Xb7;
        TR0 = 1;
}
//��ʱ���ж�
void INTT0(void) interrupt 1 using 1
{
        TL0=0Xff;    //10ms ��װ
        TH0=0Xb7;
        dectimers();
}



//����һ��״̬��д��
sbit LED1= P2^4;  
void ontimer0()
{ 
  LED1=!LED1;  // LED1���Ž��ڷ���ܸ�����LED1=0 Ϊ����LED1=1Ϊ��

  //��װ��ʱ��
  if (LED1) tasks[0].td=45;  //450mS ��
  else tasks[0].td=5;  //50ms  ��
}

//�������״̬��д��
char keycount=0;
void task1()
{
	if(KEY==0) 
	{
	   keycount++;
	}
	else
	{
	    keycount=0;
	}
	//��װ��ʱ��
	tasks[1].td=5;
}


//��������α�߳�д��
void  task2()
{
	static char i;
	_SS
	
	while(1){
	
	for(i=0;i<=9;i++){   //��0--9������ʾ�����200mS
	   WaitX(2,20);         //    �ȴ�200mS,ʵ�������ö�ʱ��2Ϊ200mS
	   P1=numtab[i];
	}
	for(i=0;i<=9;i++){ //��0--9������ʾ�����500mS
	   WaitX(2,50);       //    �ȴ�500mS,ʵ�������ö�ʱ��2Ϊ500mS
	   P1=numtab[i];
	}
	}
	
	_EE
}


const char * pte=23;
char * const pdt=23;


void main()
{
        unsigned char         KeyNum;

        P1 = 0xff;         //����ʾ

        InitT0();

        KEY =1;                                //����IO��
        KeyNum=0;                        //���´���

    //װ������:
        tasks[0].fp=ontimer0; 
        tasks[1].fp=task1; 
        tasks[2].fp=task2; 

    //ѭ������
        while(1)
		{
         runtasks();
        // clr_wdt();
        }
}