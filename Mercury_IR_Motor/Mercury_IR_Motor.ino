
#include <Wire.h>
#include "Seeed_InfraredReceiver.h"
#include "Seeed_MiniI2CMotorDriver.h"


#define GO_STOP    0xc738 // Button '5'
#define GO_FORWARD    0xe718 // Button '2'
#define GO_BACKWARD    0xb54a // Button '8'
#define GO_LEFT    0xef10 // Button '4'
#define GO_RIGHT    0xa55a // Button '6'
#define SPEED_UP    0xcf30 // Button '1'
#define SPEED_DN    0x857a // Button '3'


InfraredReceiver infraredReceiver(A5);
MiniI2CMotorDriver miniI2CMotorDriver(5, 6);

char speed = 10;
char state = 0;


void setup()
{
	Serial.begin(115200);
	Serial.println("IR Motor Demo");
    
    pinMode(13, OUTPUT);
    for(int i=0;i<2; i++)
    {
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
        delay(500);
    }
    digitalWrite(13, HIGH);
    
	infraredReceiver.Init();
    
}

void loop()
{
    unsigned int *command = 0;
	unsigned char *data = infraredReceiver.InfraredReceiverRead(); 
    
	if(data)
	{	
		int length = data[0];

        command = (unsigned int *)(data + 3);
        
        Serial.print("command is 0x");
        Serial.println(*command, HEX);
        
        switch(*command)
        {
            case GO_STOP:
                state = 0;
                Serial.println("GO_STOP");
                robot_go_stop();
            break;
            
            case GO_FORWARD:
                state = 1;
                Serial.println("GO_FORWARD");
                robot_go_forward(speed);
            break;
            
            case GO_BACKWARD:
                state = 2;
                Serial.println("GO_BACKWARD");
                robot_go_backward(speed);
            break;
            
            case GO_LEFT:
                state = 3;
                Serial.println("GO_LEFT");
                robot_go_left(speed);
            break;
            
            case GO_RIGHT:
                state = 4;
                Serial.println("GO_RIGHT");
                robot_go_right(speed);
            break;
            
            case SPEED_UP:
                Serial.println("SPEED_UP");
                if(speed <= 40)speed += 10;
                Serial.print("speed: ");
                Serial.println(speed, DEC);
                Serial.print("state: ");
                Serial.println(state, DEC);
                delay(100);
                switch(state)
                {
                    case 0:
                        robot_go_stop();
                    break;
                    
                    case 1:
                        robot_go_forward(speed);
                    break;
                    
                    case 2:
                        robot_go_backward(speed);
                    break;
                    
                    case 3:
                        robot_go_left(speed);
                    break;
                    
                    case 4:
                        robot_go_right(speed);
                    break;
                    
                    default:
                    break;
                }
            break;
            
            case SPEED_DN:
                Serial.println("SPEED_DN");
                if(speed > 10)speed -= 10;
                Serial.print("speed: ");
                Serial.println(speed, DEC);
                Serial.print("state: ");
                Serial.println(state, DEC);
                delay(100);
                switch(state)
                {
                    case 0:
                        robot_go_stop();
                    break;
                    
                    case 1:
                        robot_go_forward(speed);
                    break;
                    
                    case 2:
                        robot_go_backward(speed);
                    break;
                    
                    case 3:
                        robot_go_left(speed);
                    break;
                    
                    case 4:
                        robot_go_right(speed);
                    break;
                    
                    default:
                    break;
                }
            break;
            
            default:
            break;
        }
	}
}

void robot_go_stop(void)
{
  miniI2CMotorDriver.Driver(0, 0);
  miniI2CMotorDriver.Driver(1, 0);
  delay(50);
}

void robot_go_forward(char data)
{
    miniI2CMotorDriver.Driver(0, data);
    miniI2CMotorDriver.Driver(1, data * (-1));
    delay(50);
}

void robot_go_backward(char data)
{
    miniI2CMotorDriver.Driver(0, data * (-1));
    miniI2CMotorDriver.Driver(1, data);
    delay(50);
}

void robot_go_left(char data)
{
    miniI2CMotorDriver.Driver(0, data * (-1));
    miniI2CMotorDriver.Driver(1, data * (-1));
    delay(50);
}

void robot_go_right(char data)
{
    miniI2CMotorDriver.Driver(0, data);
    miniI2CMotorDriver.Driver(1, data);
    delay(50);
}
