#ifndef _CARMOTOR_H
#define _CARMOTOR_H

#include <Arduino.h>
#include <math.h>

class CarMotor
{
  public:
    /**
     * 构造函数，指定引脚控制
     */
    CarMotor(uint8_t left1, uint8_t left2, uint8_t right1, uint8_t right2);
    CarMotor(uint8_t left1, uint8_t left2, uint8_t right1, uint8_t right2, uint8_t leftEN, uint8_t rightEN);

    /**
     * 根据坐标系同时控制左右轮运行  
     * 根据坐标系象限  控制前进  后退  左右转
     * 
     */
    void run(char x,char y);

    /**左电机正转*/
    void leftMotorPositive(unsigned char speed = 0)
    {
        digitalWrite(this->_pin_left1, HIGH);
        delay(1);
        digitalWrite(this->_pin_left2, LOW);
        delay(1);
        if (speed == 0)
        {
            digitalWrite(this->_pin_left_en, HIGH);
        }
        else
        {
            analogWrite(this->_pin_left_en, speed); //点击使能控制速度
        }
        delay(1);
    }
    /**左电机反转*/
    void leftMotorNegative(unsigned char speed = 0)
    {
        digitalWrite(this->_pin_left1, LOW);
        delay(1);
        digitalWrite(this->_pin_left2, HIGH);
        delay(1);
        if (speed == 0)
        {
            digitalWrite(this->_pin_left_en, HIGH);
        }
        else
        {
            analogWrite(this->_pin_left_en, speed); //点击使能控制速度
        }
        delay(1);
    }
    /**左电机停止*/
    void leftMotorStop()
    {
        digitalWrite(this->_pin_left1, HIGH);
        delay(1);
        digitalWrite(this->_pin_left2, HIGH);
        delay(1);
        digitalWrite(this->_pin_left_en, LOW);
        delay(1);
    }

    void rightMotorPositive(unsigned char speed = 0)
    {
        digitalWrite(this->_pin_right1, HIGH);
        delay(1);
        digitalWrite(this->_pin_right2, LOW);
        delay(1);
        if (speed == 0)
        {
            digitalWrite(this->_pin_right_en, HIGH);
        }
        else
        {
            analogWrite(this->_pin_right_en, speed);
        }
        delay(1);
    }
    void rightMotorNegative(unsigned char speed = 0){
        digitalWrite(this->_pin_right1, LOW);
        delay(1);
        digitalWrite(this->_pin_right2, HIGH);
        delay(1);
        if (speed == 0)
        {
            digitalWrite(this->_pin_right_en, HIGH);
        }
        else
        {
            analogWrite(this->_pin_right_en, speed);
        }
        delay(1);
    }
    void rightMotorStop(){
        digitalWrite(this->_pin_right1, HIGH);
        delay(1);
        digitalWrite(this->_pin_right2, HIGH);
        delay(1);
        digitalWrite(this->_pin_right_en, LOW);
        delay(1);
    }

  private:
    uint8_t _pin_left1;
    uint8_t _pin_left2;
    uint8_t _pin_right1;
    uint8_t _pin_right2;
    uint8_t _pin_left_en;  //左电机使能
    uint8_t _pin_right_en; //右电机使能

    /**初始化引脚相关的函数*/
    void initPins();
};

#endif