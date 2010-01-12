//////////////////////////////////////////
/*
new pab 
*/
//////////////////////////////////////////
//#include "io430.h"
#include <msp430x14x.h>
#define WriteDeviceAddress  0x3c     //定义器件在IIC总线中的地址
#define ReadDviceAddress   0x3d

#define uchar unsigned char          //定义一下方便使用
#define uint  unsigned int
#define ulong unsigned long

char flag = 1;
char num = 0;
char lednum = 0;
int results[7];  
char results_16405[304];
char ii=0;
int i;
char readdata[2];
///////////////////////////////////////////
///////////////////////////////////////////
void SPI_readdata_first(char address)
{
    char temp;
    while (!(IFG2 & UTXIFG1));                // USART0 TX buffer ready?
    TXBUF1 = address;    
    IFG2 &= ~0x20; 
     for (i = 0x30; i > 0; i--); 
    while (!(IFG2 & UTXIFG1));                // USART0 TX buffer ready?
    TXBUF1 = 0x00; 
    IFG2 &= ~0x20;    
    for (i = 0x30; i > 0; i--); 
    while (!(IFG2 & URXIFG1));
    IFG2 &= ~0x10; 
    temp = RXBUF1;  
 }
 
 void SPI_readdata_next(char address)
 {
   //for (i = 0x30; i > 0; i--); 
    while (!(IFG2 & UTXIFG1));                // USART0 TX buffer ready?
    TXBUF1 = address;   
    IFG2 &= ~0x20; 
    for (i = 0x30; i > 0; i--); 
    while (!(IFG2 & URXIFG1));
    IFG2 &= ~0x10; 
    readdata[0] = RXBUF1;  
     while (!(IFG2 & UTXIFG1));                // USART0 TX buffer ready?
    TXBUF1 = 0x00;
    IFG2 &= ~0x20; 
  //  for (i = 0x30; i > 0; i--); 
    while (!(IFG2 & URXIFG1));
    IFG2 &= ~0x10;  
    readdata[1] = RXBUF1;
 }
///////////////////////////////////////////

void main(void)
{

    /////////////////////////////
  P3DIR |= 0x04;                             
  P3OUT |= 0x04;                           // OPEN  THE BLUETOOTH

  P5DIR |= 0x01;
  P5OUT |= 0x01;                           //set the led
  //////////////////////////////
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  BCSCTL1 |= XTS;                           //High frequency mode
                                            //ACLK= LFXT1= HF XTAL
   
  do
  {
    IFG1 &= ~OFIFG;                           // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);               // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?
  
  BCSCTL2 |= SELM_3;                        // MCLK = LFXT1 (safe) smclk=LFXT1
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK= ACLK
  UBR00 = 5;                             // 8MHz 115200
  UBR10 = 0x00;                             // 8MHz 115200
  UMCTL0 = 0x7b;                            // 8MHz 115200 modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
 // IE1 |= URXIE0;                             // Enable USART0 RX interrupt
  
  
  
   P1DIR |= 0x20;
  P3DIR |= 0x02;                            //RTS--OUT   
  P3DIR &= ~0x01;                            //CTS--IN
 // P4OUT &= 0xFE;
  P1OUT &= ~0x20;
  P3OUT |= 0X02;
  
 // P5DIR |= 0x70;                            // P5.6,5,4 outputs
 // P5SEL |= 0x70;                            // P5.6,5,5 options
  
//  P5SEL |= 0x0E;                            // P5.1,2,3 SPI option select
  P5DIR |= 0x02;                            // P5.1 OUTPUT
  P5DIR &= ~0x04;                            // P5.2 OUTPUT
  P5DIR |= 0x08;                            // P5.3 OUTPUT
  P5OUT |= 0X02;
  P5OUT |= 0X08;
  
  U1CTL = CHAR + SYNC + MM + SWRST;         // 8-bit, SPI, Master
  U1TCTL = CKPL + SSEL0 + STC;              // Polarity, ACLK, 3-wire
  U1BR0 = 0x08;                             // SPICLK = ACLK/16
  U1BR1 = 0x00;
  U1MCTL = 0x00;
  ME2 |= USPIE1;                            // USART1 SPI Module enable
  U1CTL &= ~SWRST;                          // SPI enable
 // IE2 |= URXIE1 + UTXIE1;                   // RX and TX interrupt enable
 
  
  P4DIR |= 0X7F;
//  P4OUT |= 0X10;
//  P4OUT &= ~0X20;
//  P4OUT &= ~0X40;
  
  P3OUT &= ~0X02;
  while (!(IFG1 & URXIFG0)); 
  
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TACTL = TASSEL_1 + MC_1 + ID_2;                  // ACLK, up mode ,1/8 CLK 
  CCR0 = 40000;                             //8M/8/10000 = 100HZ
  _EINT();
  
   while(1)
   {
      while(flag);
      flag = 1;
      lednum++;
      if(lednum==100)
      {
        lednum = 0;
        P5OUT = P5OUT ^ 0x01;
        P5OUT = P5OUT ^ 0X02;
        P5OUT = P5OUT ^ 0X08;
      }
      for(int j=0;j<304;j++)
      {
        P3OUT &= ~0X02;
        while (!(P3IN | 0xFE == 0xFE));
        while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
        TXBUF0 = results_16405[j];    
      }
      
   }
  
  
                                     //enable global interrupt
  
  
 
} 

//// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  int x=0;
  flag = 0;
  for(num=1;num<1;num++)
  {
    switch (num)
    {
      case 0: P4OUT = 0X10;
              break;
      case 1: P4OUT = 0X50;
              break;
      case 2: P4OUT = 0X30;
              break;
      case 3: P4OUT = 0X08;
              break;
      case 4: P4OUT = 0X48;
              break;
      case 5: P4OUT = 0X28;
              break;
      case 6: P4OUT = 0X68;
              break;
      case 7: P4OUT = 0X04;
              break;
      case 8: P4OUT = 0X44;
              break;
      case 9: P4OUT = 0X24;
              break;
      case 10: P4OUT = 0X02;
              break;
      case 11: P4OUT = 0X42;
              break;
      case 12: P4OUT = 0X22;
              break;
      case 13: P4OUT = 0X01;
              break;
      case 14: P4OUT = 0X41;
              break;
      case 15: P4OUT = 0X21;
              break;                        
    }
      results_16405[x] = num+1;
      SPI_readdata_first(0x0a);
        x++;
      SPI_readdata_next(0x0c);
      results_16405[x] = readdata[0];
        x++;
      results_16405[x] = readdata[1];
        x++;
        
      SPI_readdata_next(0x0e);
      results_16405[x] = readdata[0];
        x++;
      results_16405[x] = readdata[1];
        x++;
        
      SPI_readdata_next(0x04);
      results_16405[x] = readdata[0];
        x++;
      results_16405[x] = readdata[1];
        x++;
        
      SPI_readdata_next(0x06);
      results_16405[x] = readdata[0];
        x++;
      results_16405[x] = readdata[1];
        x++;
      
      SPI_readdata_next(0x08);
      results_16405[x] = readdata[0];
        x++;
      results_16405[x] = readdata[1];
        x++;
        
      SPI_readdata_next(0x10);
      results_16405[x] = readdata[0];
        x++;
      results_16405[x] = readdata[1];
        x++;
        
      SPI_readdata_next(0x12);
      results_16405[x] = readdata[0];
        x++;
      results_16405[x] = readdata[1];
        x++;
        
       SPI_readdata_next(0x14);
      results_16405[x] = readdata[0];
        x++;
      results_16405[x] = readdata[1];
        x++;
        
        SPI_readdata_next(0x00);
      results_16405[x] = readdata[0];
        x++;
      results_16405[x] = readdata[1];
        x++;
  }     
 // CCTL0 &= ~0X01;  
}

 