//////////////////////////////////////////
/*
program for READING DATA AND SEND DATA THROUGH BLUETOOTH
*/
//////////////////////////////////////////
//#include "io430.h"
#include <msp430x14x.h>
#define WriteDeviceAddress  0x3c     //定义器件在IIC总线中的地址
#define ReadDviceAddress   0x3d

#define uchar unsigned char          //定义一下方便使用
#define uint  unsigned int
#define ulong unsigned long

//P2.1  SCL
//P2.0  SDA

//#define SDA_1       P2OUT |=  0x01              //SDA = 1
#define SDA_1       P2DIR &= ~0x01              //SDA = 1

#define SDA_0       P2DIR |=  0x01; P2OUT &= ~0x01              //SDA = 0

#define SCL_1       P2OUT |=  0x02              //SCL = 1
#define SCL_0       P2OUT &= ~0x02              //SCL = 0

#define DIR_IN      P2DIR &= ~0x01//;  SDA_1      //I/O口为输入
#define DIR_OUT     P2DIR |=  0x01              //I/0口为输出
#define SDA_IN      ((P2IN ) & 0x01)        //Read SDA
char flag;
char num = 0;

int results[7];  
char results_mag[6];
//char ii;
//int i;
char power = 1;
char bt_start =0;
char function_key = 0;
char vcc_alarm = 0;
//char counter=0;


////////////////////////////////////////////
////////HMC5843
////////////////////////////////////////////
/*-------------起始位状态：SCL为高时，SDA线由高到低跳变，之后经延时后SCL由高到低跳变
               停止位状态：SCL为高时，SDA线由低到高跳变
               ------------------------------------------------*/

/********************************************************************/
/********************************************************************/
void DelayMs(unsigned int number)     //定时函数
{

//unsigned char temp;
//for(;number!=0;number--) ;//{for(temp=1;temp!=0;temp--);}
}
void Delay(unsigned int number)     //定时函数
{
	for(;number!=0;number--) ;
}
/*****************************开始总线*******************************/
//SCL为高电平时，SDA由高电平向低电平跳变，开始传送数据。
void Start() 
{ 
  //DIR_OUT; 
  SDA_1;SCL_1; DelayMs(1); SDA_0;DelayMs(1);SCL_0;
}  //
/*****************************结束总线*******************************/
//SCL_1为高电平时，SDA_1由低电平向高电平跳变，结束传送数据。
void Stop()  {SCL_0;SDA_0; DelayMs(1); SCL_1;DelayMs(1);SDA_1;}
/*************************发送数据1或非应答位************************/
void NoAck()  { SDA_1;SCL_1; DelayMs(1); SCL_0;}
//发送数据1或非应答位;在SDA_1为高电平时使SCL_1信号为高;
void SendAck()  {SDA_0;SCL_1; DelayMs(1);SCL_0;}
/*****************************测试应答位*****************************/
unsigned char TestAck()
{
  unsigned char ErrorBit;
  //SDA_1; 
  //P2DIR &= ~0x01;
  
  SDA_1;
  SCL_1;     //启动总线;
  
  DelayMs(1);
  //while (SDA_IN == 0x01);
  DIR_IN;
  ErrorBit=SDA_IN;     //将数据线状态存入临时变量;
  DelayMs(1);
  //DIR_OUT;
  //DelayMs(1);
  SCL_0;            //释放总路线;
  //while(ErrorBit);
  return(ErrorBit); //返回应答信号;
  //DelayMs(1);
}
/*接收数据的IC在接收到8bit数据后，向发送数据的IC发出特定的低电平脉冲;
表示已收到数据。CPU向受控单元发出一个信号后，等待受控单元发出一个应答信号，
CPU接收到应答信号后，根据实际情况作出是否继续传递信号的判断。
若未收到应答信号，由判断为受控单元出现故障。*/
/***************************写入8个bit到24c02*************************/
Write8Bit(unsigned char input)
{
  unsigned char temp;          //定义变量;
  //DIR_OUT;
  for(temp=8;temp!=0;temp--)   //循环执行8次；
  {
    //SCL_0;
    if((uchar)(input&0x80) == 0x80)
    {
      SDA_1;
    }
    else 
    { 
      SDA_0;
    }
   // SDA_1=(bit)(input&0x80);      //取输入数据的最高位;
    DelayMs(1);
    SCL_1; 
   // Delay(20);
    DelayMs(1);
    SCL_0;               //发送一位数据;
    input=input<<1;             //输入数据左移一位数据;
    DelayMs(1);
  }
  //SDA_1;
  //SCL_1;
}
/************************写入一个字节到24c02中*************************/
void Write24c02(uchar ch,uchar address)
{
Start();     
//DelayMs(1);                               //启动总线，开始传输数据;
Write8Bit(WriteDeviceAddress);  
//while(TestAck());  //发送从器件硬件地址;  
TestAck();
Write8Bit(address);             
//while(TestAck());  //发送从器件存储器字节地址;
TestAck();
Write8Bit(ch);                  
//while( TestAck());  //发送数据;
TestAck();
DelayMs(1);Stop();                                     //发送停止位,发送数据结束;
DelayMs(1);                                //操作完成，等待10MS再执行其它操作;
//DelayMs(1);
}
/**************************从24c02中读出8个bit**************************/
uchar Read8Bit()
{
  unsigned char temp,rbyte=0;    //定义变量;
  unsigned char SDT=0;
 // unsigned char SDQ=0;
 // unsigned char sdt_array[8];
  //bit SDA_11=0,
  //DIR_OUT;
  //SDA_1;
  DIR_IN;
  DelayMs(1);
  
  for(temp=8;temp!=0;temp--)     //循环执行8次；
  {
    SCL_1;                        //启动总线;   
   // Delay(20);
    //DelayMs(1);
    //DIR_IN;
  //  SDT=SDA_IN;
  //  DelayMs(1);
  //  SDQ=SDA_IN;
  //  while(SDQ!=SDT)
  //  {
  //  	DelayMs(1);
  //	SDT=SDA_IN;
  //	DelayMs(1);
  //	SDQ=SDA_IN;
  //  }
   //DIR_OUT;
  SDT=SDA_IN;
  
  rbyte=rbyte<<1;               //临时存储读入数据的变量左移一位;
  rbyte=rbyte|((uchar)(SDT));   //将SDA_1状态存入rbyte变量的最低位;(uchar)(SDA_1)强制将SDA_1(bit)数据类型轮换为uchar类型;
  DelayMs(1);
  SCL_0;                        //释放总线;
  //P2DIR &= ~0x01;
 // P2OUT &= ~0x01;
 // sdt_array[temp-1] = P2IN;
  DelayMs(1);
  //DelayMs(1);
  }
return(rbyte);
 
 // return(sdt_array[0]+sdt_array[1]+sdt_array[2]);
}
/**************************从24c02中读出1个字节**************************/
uchar Read24c02(uchar address)        //
{
uchar ch;                                  //定义存储读出数据的临时变量;
Start();                                   //启动总线，开始传输数据;
//DelayMs(1);
Write8Bit(WriteDeviceAddress);   TestAck(); //发送从器件硬件地址;
Write8Bit(address);              TestAck(); //发送从器件内部数据存储器的地址;
Start();                                   //重新启动总线，开始传输数据;
Write8Bit(ReadDviceAddress);     TestAck(); //发送从器件内部数据存储器的地址;
ch=Read8Bit();                   NoAck();   //将读出的一个字节数据存入临时变量,发送非应答位;
Stop();                                    //放送停止信号,释放总路线;
return(ch);                                //返回读取的数据;
}

void  hmc5843_init(void)
{

   Write24c02(0x18,0x00);
  // Read24c02(0x00);
   Write24c02(0x00,0x01);
   Write24c02(0x00,0x02);
}


///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
void Iint_Port1(void)
{   
    P1DIR&=~BIT0;                         //设置为输入方向
    P1DIR&=~BIT1;  
    P1DIR&=~BIT2;  
    P1DIR&=~0X10;  
    P1SEL&=~0X17;                         //设置为普通I/O口功能
    //P1SEL&=~0X10;  
    P1IES|=0X17;                          //选择下降沿触发
    P1IE|=0X17;                           //打开中断允许
    P1IFG=0;                              //P1IES的切换可能使P1IFG置位，需清除
} 
///////////////////////////////////////////
void TimerA_INIT(void)
{
  CCTL0 &= ~CCIE;                             // CCR0 interrupt disabled
  CCR0 = 40000;
  TACTL = TASSEL_1 + MC_1;                  // ACLK, up/down mode  
}  
///////////////////////////////////////////
///////////////////////////////////////////
void TimerB_INIT(void)
{
  TBCCTL0 = CCIE;                           // CCR0 interrupt enabled
  TBCCR0 = 40000;
  TBCTL = TBSSEL_1 + MC_1;                  // SMCLK, contmode
}  
///////////////////////////////////////////
void AD_Converter_INIT(void)
{
  P6SEL = 0xFF;                             // Enable A/D channel inputs
  ADC12CTL0 = ADC12ON+MSC+SHT0_2;           // Turn on ADC12, set sampling time
  ADC12CTL1 = SHP+CONSEQ_1;                 // Use sampling timer, single sequence
  ADC12MCTL0 = INCH_0;                      // ref+=AVcc, channel = A0
  ADC12MCTL1 = INCH_1;                      // ref+=AVcc, channel = A1
  ADC12MCTL2 = INCH_2;                      // ref+=AVcc, channel = A2
  ADC12MCTL3 = INCH_3;                      // ref+=AVcc, channel = A1
  ADC12MCTL4 = INCH_4;                      // ref+=AVcc, channel = A1
  ADC12MCTL5 = INCH_5;                      // ref+=AVcc, channel = A1
  ADC12MCTL6 = INCH_7+EOS;                  // ref+=AVcc, channel = A3, end seq.
  ADC12IE = 0x40;                           // Enable ADC12IFG.3
  ADC12CTL0 |= ENC;                         // Enable conversions

  ADC12CTL0 |= ADC12SC;
}  
///////////////////////////////////////////
void USART0_INIT(void)
{
  BCSCTL2 |= SELM_3;                        // MCLK = LFXT1 (safe)
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK= ACLK
  UBR00 = 69;                             // 8MHz 115200
  UBR10 = 0x00;                             // 8MHz 115200
  UMCTL0 = 0xaa;                            // 8MHz 115200 modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
 // IE1 |= URXIE0;                             // Enable USART0 RX interrupt
}  
///////////////////////////////////////////
void POWER_ON(void)
{
  P1DIR |= 0x08;                             
  P1OUT |= 0x08;                           // SET THE LED

  P4DIR |= 0x10;
  P4DIR |= 0x20;
  P4DIR |= 0x40;
  P4OUT |= 0x10;                            //SET CLR
  //P4OUT |= 0x20;
  P4OUT &= ~0x20;                           //OPEN BLUETOOTH
  P4OUT &= ~0x40;                           //OPEN SENSOR
}
void POWER_OFF(void)
{
  P1DIR |= 0x08;                             
  P1OUT &= ~0x08;                           // LOW THE LED

  P4DIR |= 0x10;
  P4DIR |= 0x20;
  P4DIR |= 0x40;
  P4OUT |= 0x10;                            //SET CLR
  P4OUT |= 0x20;                           //CLOSE BLUETOOTH
  P4OUT |= 0x40;                           //CLOSE SENSOR
}

void main(void)
{

  char sd;
    /////////////////////////////
  POWER_ON();
  //////////////////////////////
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL1 |= XTS;                           // ACLK= LFXT1= HF XTAL
   
  do
  {
    IFG1 &= ~OFIFG;                           // Clear OSCFault flag
    for (int i = 0xFF; i > 0; i--);               // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  USART0_INIT();
  
  AD_Converter_INIT();  
  
  P2DIR |= 0X01;                        //sda out
  P2DIR |= 0X02;                        //scl out
  P2DIR &= ~0X04;                       //drdy in 
  hmc5843_init();
  
  Iint_Port1();
  

  
  TimerA_INIT();                             //bluetooth transfer
  TimerB_INIT();                            //vcc
  _EINT();                                  //enable global interrupt
  
  
  // Receive = 
  
  
//   _BIC_SR(LPM3_bits+GIE); 
  while(1)
  {
    if(power==1&&bt_start==0)
    {
      CCTL0 &= ~CCIE; 
      bt_start = 1;
      P4OUT &= ~0x02;
      for (int xx=200;xx>0;xx++)
      {
         sd = RXBUF0;
      }
      IFG1 &= ~0X40;
      while (!(IFG1 & URXIFG0)); 
      sd = RXBUF0;
      CCTL0 = CCIE;                       // CCR0 interrupt enabled
    }
   // _BIC_SR(LPM0_bits+GIE);
  }
 
} 

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  char temp;
  if(num==3)
  {
   
    num=0;
    ADC12CTL0 |= ADC12SC;
   //Write24c02(0x18,0x00); 
   results_mag[0] = Read24c02(0x03);
   results_mag[1] = Read24c02(0x04);
   results_mag[2] = Read24c02(0x05);
   results_mag[3] = Read24c02(0x06);
   results_mag[4] = Read24c02(0x07);
   results_mag[5] = Read24c02(0x08);  
   hmc5843_init();
     while (!(P4IN | 0xFE == 0xFE));
     while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
     TXBUF0 = 0xaa;
   for (int ii=0;ii<6;ii++)
   {
     
      while (!(P4IN | 0xFE == 0xFE));
      while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
      TXBUF0 = (uchar)((results[ii]&0xff00)/256);   
        
      while (!(P4IN | 0xFE == 0xFE));
      while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
      TXBUF0 = (uchar)(results[ii]&0x00ff);  
   }       
    for (int ii=0;ii<6;ii++)
   {
     
      while (!(P4IN | 0xFE == 0xFE));
      while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
      TXBUF0 = results_mag[ii];   

   }       
   temp = P1IN&0x07;
   temp = ~temp;
   if ((temp&function_key))
   {
      while (!(P4IN | 0xFE == 0xFE));
      while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
      TXBUF0 = function_key; 
      temp++;
   }
   else
   {
      while (!(P4IN | 0xFE == 0xFE));
      while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
      TXBUF0 = 0x00; 
   }
  }
  else
  {
    num++;
  }
}

#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR (void)
{
  results[0] = ADC12MEM0;                   // Move results, IFG is cleared
  results[1] = ADC12MEM1;                   // Move results, IFG is cleared
  results[2] = ADC12MEM2;                   // Move results, IFG is cleared
  results[3] = ADC12MEM3;                   // Move results, IFG is cleared
  results[4] = ADC12MEM4;                   // Move results, IFG is cleared
  results[5] = ADC12MEM5;                   // Move results, IFG is cleared
  results[6] = ADC12MEM6;                   // Move results, IFG is cleared

  _BIC_SR_IRQ(LPM0_bits);                   // Clear LPM0, SET BREAKPOINT HERE
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void)
{   
//  int j;
//  char flag=0;
 // P1IE&=~BIT0;      
  flag = P1IFG;                          //指示中断管脚
  flag &= ~0xE8;
  P1IFG=0;                              //多源中断，需靠软件清除P1IFG
   // ISR_Symbol|=BIT0;                     //置任务标志
  //  _BIC_SR_IRQ(LPM3_bits);                    //将CPU从睡眠模式唤醒
 
   Delay(0XFF);
   if (!(P1IN&flag))
   {

          Delay(0X1FF); 
          if (!(P1IN&flag))
          {
              Delay(0X1FF); 
             if (!(P1IN&flag))                           //确认有按键事件触发
             {

               if(flag&0x10)                                //POWER KEY
               {
                  if (power==0)                             //on
                  {
                    power = 1;
                    bt_start = 0;
                    POWER_ON();
                     ADC12CTL0 |= ENC;                         // Enable conversions                    
                     CCTL0 &= ~CCIE; 
                     TBCCTL0 = CCIE;                         //TIMERB  interrupt ENabled  
                     //counter++;
                   // Delay(0XFF); 
                     
                //    P4OUT &= ~0x02;
                //    while (!(IFG1 & URXIFG0)); 
                    
                  }
                  else if(power==1)                         //off
                  {
                    power = 0;     
                    bt_start = 0;
                    POWER_OFF();
                    CCTL0 &= ~CCIE;                             // CCR0 interrupt disabled
                    TBCCTL0 &= ~CCIE;                           //TIMERB  interrupt disabled
                    ADC12CTL0 &= ~ENC;                         // ADC DISable conversions
                               
                    //counter++
                  //  _BIC_SR_IRQ(LPM3_bits+GIE); 
                  }
                }
                else if(flag&0x01)                               //function key 1
                {
                   function_key = 0x01;
                }
                else if(flag&0x02)                               //function key 2
                {
                   function_key = 0x02;
                }
                else if(flag&0x04)                               //function key 3
                {
                   function_key = 0x04;;
                }
                
              }
             
          }

    }
  
   // P1IE|=BIT0;      
}
// Timer B0 interrupt service routine
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
  
   if(vcc_alarm==200)
   {
     vcc_alarm=0;
     if(results[6]<400)
     {
      P1OUT = P1OUT^0x08;                           // change THE LED
     }
     else
     {
       P1OUT |=0x08;                               // SET THE LED
     }
     
   }
   else
   {
     vcc_alarm++;
   }
}