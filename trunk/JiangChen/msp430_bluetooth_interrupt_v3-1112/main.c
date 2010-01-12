//////////////////////////////////////////
/*
program for READING DATA AND SEND DATA THROUGH BLUETOOTH
*/
//////////////////////////////////////////
//#include "io430.h"
#include <msp430x14x.h>
#define WriteDeviceAddress  0x3c     //����������IIC�����еĵ�ַ
#define ReadDviceAddress   0x3d

#define uchar unsigned char          //����һ�·���ʹ��
#define uint  unsigned int
#define ulong unsigned long

//P2.1  SCL
//P2.0  SDA

//#define SDA_1       P2OUT |=  0x01              //SDA = 1
#define SDA_1       P2DIR &= ~0x01              //SDA = 1

#define SDA_0       P2DIR |=  0x01; P2OUT &= ~0x01              //SDA = 0

#define SCL_1       P2OUT |=  0x02              //SCL = 1
#define SCL_0       P2OUT &= ~0x02              //SCL = 0

#define DIR_IN      P2DIR &= ~0x01//;  SDA_1      //I/O��Ϊ����
#define DIR_OUT     P2DIR |=  0x01              //I/0��Ϊ���
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
/*-------------��ʼλ״̬��SCLΪ��ʱ��SDA���ɸߵ������䣬֮����ʱ��SCL�ɸߵ�������
               ֹͣλ״̬��SCLΪ��ʱ��SDA���ɵ͵�������
               ------------------------------------------------*/

/********************************************************************/
/********************************************************************/
void DelayMs(unsigned int number)     //��ʱ����
{

//unsigned char temp;
//for(;number!=0;number--) ;//{for(temp=1;temp!=0;temp--);}
}
void Delay(unsigned int number)     //��ʱ����
{
	for(;number!=0;number--) ;
}
/*****************************��ʼ����*******************************/
//SCLΪ�ߵ�ƽʱ��SDA�ɸߵ�ƽ��͵�ƽ���䣬��ʼ�������ݡ�
void Start() 
{ 
  //DIR_OUT; 
  SDA_1;SCL_1; DelayMs(1); SDA_0;DelayMs(1);SCL_0;
}  //
/*****************************��������*******************************/
//SCL_1Ϊ�ߵ�ƽʱ��SDA_1�ɵ͵�ƽ��ߵ�ƽ���䣬�����������ݡ�
void Stop()  {SCL_0;SDA_0; DelayMs(1); SCL_1;DelayMs(1);SDA_1;}
/*************************��������1���Ӧ��λ************************/
void NoAck()  { SDA_1;SCL_1; DelayMs(1); SCL_0;}
//��������1���Ӧ��λ;��SDA_1Ϊ�ߵ�ƽʱʹSCL_1�ź�Ϊ��;
void SendAck()  {SDA_0;SCL_1; DelayMs(1);SCL_0;}
/*****************************����Ӧ��λ*****************************/
unsigned char TestAck()
{
  unsigned char ErrorBit;
  //SDA_1; 
  //P2DIR &= ~0x01;
  
  SDA_1;
  SCL_1;     //��������;
  
  DelayMs(1);
  //while (SDA_IN == 0x01);
  DIR_IN;
  ErrorBit=SDA_IN;     //��������״̬������ʱ����;
  DelayMs(1);
  //DIR_OUT;
  //DelayMs(1);
  SCL_0;            //�ͷ���·��;
  //while(ErrorBit);
  return(ErrorBit); //����Ӧ���ź�;
  //DelayMs(1);
}
/*�������ݵ�IC�ڽ��յ�8bit���ݺ��������ݵ�IC�����ض��ĵ͵�ƽ����;
��ʾ���յ����ݡ�CPU���ܿص�Ԫ����һ���źź󣬵ȴ��ܿص�Ԫ����һ��Ӧ���źţ�
CPU���յ�Ӧ���źź󣬸���ʵ����������Ƿ���������źŵ��жϡ�
��δ�յ�Ӧ���źţ����ж�Ϊ�ܿص�Ԫ���ֹ��ϡ�*/
/***************************д��8��bit��24c02*************************/
Write8Bit(unsigned char input)
{
  unsigned char temp;          //�������;
  //DIR_OUT;
  for(temp=8;temp!=0;temp--)   //ѭ��ִ��8�Σ�
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
   // SDA_1=(bit)(input&0x80);      //ȡ�������ݵ����λ;
    DelayMs(1);
    SCL_1; 
   // Delay(20);
    DelayMs(1);
    SCL_0;               //����һλ����;
    input=input<<1;             //������������һλ����;
    DelayMs(1);
  }
  //SDA_1;
  //SCL_1;
}
/************************д��һ���ֽڵ�24c02��*************************/
void Write24c02(uchar ch,uchar address)
{
Start();     
//DelayMs(1);                               //�������ߣ���ʼ��������;
Write8Bit(WriteDeviceAddress);  
//while(TestAck());  //���ʹ�����Ӳ����ַ;  
TestAck();
Write8Bit(address);             
//while(TestAck());  //���ʹ������洢���ֽڵ�ַ;
TestAck();
Write8Bit(ch);                  
//while( TestAck());  //��������;
TestAck();
DelayMs(1);Stop();                                     //����ֹͣλ,�������ݽ���;
DelayMs(1);                                //������ɣ��ȴ�10MS��ִ����������;
//DelayMs(1);
}
/**************************��24c02�ж���8��bit**************************/
uchar Read8Bit()
{
  unsigned char temp,rbyte=0;    //�������;
  unsigned char SDT=0;
 // unsigned char SDQ=0;
 // unsigned char sdt_array[8];
  //bit SDA_11=0,
  //DIR_OUT;
  //SDA_1;
  DIR_IN;
  DelayMs(1);
  
  for(temp=8;temp!=0;temp--)     //ѭ��ִ��8�Σ�
  {
    SCL_1;                        //��������;   
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
  
  rbyte=rbyte<<1;               //��ʱ�洢�������ݵı�������һλ;
  rbyte=rbyte|((uchar)(SDT));   //��SDA_1״̬����rbyte���������λ;(uchar)(SDA_1)ǿ�ƽ�SDA_1(bit)���������ֻ�Ϊuchar����;
  DelayMs(1);
  SCL_0;                        //�ͷ�����;
  //P2DIR &= ~0x01;
 // P2OUT &= ~0x01;
 // sdt_array[temp-1] = P2IN;
  DelayMs(1);
  //DelayMs(1);
  }
return(rbyte);
 
 // return(sdt_array[0]+sdt_array[1]+sdt_array[2]);
}
/**************************��24c02�ж���1���ֽ�**************************/
uchar Read24c02(uchar address)        //
{
uchar ch;                                  //����洢�������ݵ���ʱ����;
Start();                                   //�������ߣ���ʼ��������;
//DelayMs(1);
Write8Bit(WriteDeviceAddress);   TestAck(); //���ʹ�����Ӳ����ַ;
Write8Bit(address);              TestAck(); //���ʹ������ڲ����ݴ洢���ĵ�ַ;
Start();                                   //�����������ߣ���ʼ��������;
Write8Bit(ReadDviceAddress);     TestAck(); //���ʹ������ڲ����ݴ洢���ĵ�ַ;
ch=Read8Bit();                   NoAck();   //��������һ���ֽ����ݴ�����ʱ����,���ͷ�Ӧ��λ;
Stop();                                    //����ֹͣ�ź�,�ͷ���·��;
return(ch);                                //���ض�ȡ������;
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
    P1DIR&=~BIT0;                         //����Ϊ���뷽��
    P1DIR&=~BIT1;  
    P1DIR&=~BIT2;  
    P1DIR&=~0X10;  
    P1SEL&=~0X17;                         //����Ϊ��ͨI/O�ڹ���
    //P1SEL&=~0X10;  
    P1IES|=0X17;                          //ѡ���½��ش���
    P1IE|=0X17;                           //���ж�����
    P1IFG=0;                              //P1IES���л�����ʹP1IFG��λ�������
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
  flag = P1IFG;                          //ָʾ�жϹܽ�
  flag &= ~0xE8;
  P1IFG=0;                              //��Դ�жϣ��迿������P1IFG
   // ISR_Symbol|=BIT0;                     //�������־
  //  _BIC_SR_IRQ(LPM3_bits);                    //��CPU��˯��ģʽ����
 
   Delay(0XFF);
   if (!(P1IN&flag))
   {

          Delay(0X1FF); 
          if (!(P1IN&flag))
          {
              Delay(0X1FF); 
             if (!(P1IN&flag))                           //ȷ���а����¼�����
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