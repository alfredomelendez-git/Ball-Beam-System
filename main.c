#include <stdint.h>
#include <VL53L0X.h>
#include <Servo.h>
#include <ADC.h>
#include <UART.h>
#include <GPIO.h>
#include <SSD1306_SPI.h>
#include <RealTime.h>
#include <FONT.h>

/****************Variables to use****************/
uint32_t J[3];
float K[3];
float D;
float Distance,Measure;
uint16_t i;
char dataString[10];
char dataString2[10];

float e,derivative;
float integral = 0;
float e_prev = 0;
float u,dutyC;
int SP = 0;


double map(double val,double X0, double Xn, double Y0, double Yn)      //convert data within a range
{
    return Y0 + (val-X0)*(Yn-Y0)/(Xn-X0);
}
/*******************Interrupt Handlers*****************/

void TIMER1A_Handler(void) {

    //Each 5ms send control signal to servo using PID control
    e = (SP/100.0) - (Distance/1000.0);               //error = Setpoint - Sensor_distance  [m]

    integral = integral + ((e+e_prev)/2)*0.005;     //Trapezoidal integration
    derivative = (e-e_prev)/0.005;                  //Derivative
    u = (e*K[0]) + (K[1]*integral) + (K[2]*derivative);           //P+I+D
    dutyC = map(u,-100,100,2,12);
    Servo_write(u + 6);
    e_prev = e;                                     //store current value
    TIMER1_ICR_R = 0x01;
}


void PortF_Handler(void)
{
    if(GPIO_PORTF_MIS_R == 0x04)        //Checks if interrupt was PortF2
    {
        if(SP > -17)
        {
            SP--;
            GPIO_PORTF_ICR_R = 0x0C;    //Clear interrupt flag
            return;
        }
        GPIO_PORTF_ICR_R = 0x0C;
        return;
    }
    else if(GPIO_PORTF_MIS_R == 0x08)   //Checks if interrupt was PortF3
    {
        if(SP < 17)
        {
            SP++;
            GPIO_PORTF_ICR_R = 0x0C;    //Clear interrupt flag
            return;
        }
        GPIO_PORTF_ICR_R = 0x0C;        //Clear interrupt flag
        return;
    }
    GPIO_PORTF_ICR_R  = 0x0C;           //Clear interrupt flag
    return;
}


/**************Functions********************/
void float_To_String(float Data,char *Convert)
{
    char tmpB[5]= {'0','0','0','0','0'};
    uint8_t i;
    int intPart = (int)Data;         //integer part separation
    float decPart = Data - intPart;    //decimal part separation
    for(i=0;i<10;i++)
    {
        Convert[i] = '0';
    }
    //Convert integer part to characters
    if(Data < 0.00)
    {
        Convert[0] = '-';
        intPart = -intPart;
        decPart = -decPart;
    }
    int temp = intPart;
    if (intPart == 0)
    {
        for(i=1;i<6;i++){
        Convert[i]= '0';                        //Converts in ASCII code
        }
    }
    else
    {
        i=0;
        while(temp > 0.00)
            {
                tmpB[i] = (temp % 10) + '0';     //Extract digits
                temp /= 10;
                i++;
            }
        for(i=5;i>0;i--){                          //Copy digits in inverse order
            Convert[6-i] = tmpB[i-1];
        }
    }

    Convert[6] = '.';
    //Decimal part to characters
    for(i=0;i<2;i++)
    {
        decPart *= 10;
        int digit = (int)decPart;
        Convert[7+i] = digit +'0';          //converts in ASCII code
        decPart -= digit;
    }
    Convert[9]= '\0';                       //end of String
}

void OLEDPRINT(void)
{
    OLED_Clear();
    OLED_8x16_P(0,1,sign[0]);                        //First variable
    OLED_8x16_P(9,1,sign[2]);
    if(dataString2[0] == '-'){                        //sign
    OLED_8x16_P(18,1,sign[3]);
    }
    OLED_8x16_P(27,1,number[dataString2[4] - '0']);     // first int digit
    OLED_8x16_P(36,1,number[dataString2[5] - '0']);     // second int digit

    OLED_8x16_P(0,4,sign[1]);                         //Second variable
    OLED_8x16_P(9,4,sign[2]);
    if(dataString[0] == '-'){                         //sign
        OLED_8x16_P(18,4,sign[3]);
        }
    OLED_8x16_P(27,4,number[dataString[3] - '0']);     // first int digit
    OLED_8x16_P(36,4,number[dataString[4] - '0']);     // second int digit
    OLED_8x16_P(45,4,sign[4]);                         // first int digit
    OLED_8x16_P(54,4,number[dataString[5] - '0']);     // second int digit
}

/**************************************************************************************/

int main(void){
    GPTM2A_Init();           // Initialize GPTM2 A
    ADC1_Init();             // Initialize ADC1
    PortF_Init();
    I2C0_Init();
    UART0_Init();
    SysTick_OneShot(320000);
    VL53L0X_Config(0x29);
    SysTick_OneShot(320000);
    VL53L0X_Start(0x29);
    PortH_Init();
    SSI0_Init();
    OLED_reset();
    OLED_init();
    GPTM1_Init();

    while(1)
    {
       Measure = I2C0_Receive(0x29);
       Distance = Measure -230.00;
       ADC_read(J);
       K[0] = map(J[0],0,4095,0,50);                //Kp
       K[1] = map(J[1],0,4095,0,10);                //Ki
       K[2] = map(J[2],0,4095,0,20);                //Kd



       float_To_String(Distance,dataString);
       float_To_String((float)SP,dataString2);
       UART0_DataTransmit(dataString);
       OLEDPRINT();
    }
}

