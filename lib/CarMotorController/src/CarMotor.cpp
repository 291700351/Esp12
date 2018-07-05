#include "CarMotor.h"

CarMotor::CarMotor(uint8_t left1, uint8_t left2, uint8_t right1, uint8_t right2)
{
    this->_pin_left1 = left1;
    this->_pin_left2 = left2;
    this->_pin_right1 = right1;
    this->_pin_right2 = right2;
    this->initPins();
}

CarMotor::CarMotor(uint8_t left1, uint8_t left2, uint8_t right1, uint8_t right2, uint8_t leftEN, uint8_t rightEN)
{
    this->_pin_left1 = left1;
    this->_pin_left2 = left2;
    this->_pin_right1 = right1;
    this->_pin_right2 = right2;
    this->_pin_left_en = leftEN;
    this->_pin_right_en = rightEN;
    this->initPins();
}

void CarMotor::run(char x, char y)
{
    Serial.print("x = ");
    Serial.print(x);
    Serial.print("----- y = ");
    Serial.print(y);
    //计算斜边长度作为大转弯轮子的点击速度
    int pwmSpeed = map(hypot(x, y), 0, 128 * sqrt(2), 0, 255);

    if (y > 10)
    { //前进，说明在第1、4象限
        if (x > 10)
        { //在第一象限 右转弯
            this->leftMotorPositive(pwmSpeed);
            this->rightMotorPositive(map(y, -128, 127, 0, 255));
        }
        else if (x < -10)
        { //第四象限 左转弯
            this->leftMotorPositive(map(y, -128, 127, 0, 255));
            this->rightMotorPositive(pwmSpeed);
        }
        else
        { //y轴  直线向前
            this->leftMotorPositive(map(y, -128, 127, 0, 255));
            this->rightMotorPositive(map(y, -128, 127, 0, 255));
        }
    }
    else if (y < -10)
    { //后退 说明在2、3象限
        if (x > 10)
        { // 第二象限
            this->leftMotorNegative(pwmSpeed);
            this->rightMotorPositive(map(y, -128, 127, 255, 0));
        }
        else if (x < -10)
        { //第三象限
            this->leftMotorNegative(map(y, -128, 127, 255, 0));
            this->rightMotorPositive(pwmSpeed);
        }
        else
        { //y轴负方向
            this->leftMotorNegative(map(y, -128, 127, 255, 0));
            this->rightMotorNegative(map(y, -128, 127, 255, 0));
        }
    }
    else
    { //单边轮转动
        if (x > 10)
        { // x正方向
            this->leftMotorPositive(pwmSpeed);
            this->rightMotorStop();
        }
        else if (x < -10)
        { //x轴负方向
            this->leftMotorStop();
            this->rightMotorNegative(pwmSpeed);
        }
        else
        { //停止
            this->leftMotorNegative(map(y, -128, 127, 255, 0));
            this->rightMotorNegative(map(y, -128, 127, 255, 0));
        }
    }

    
    Serial.println(digitalRead(this->_pin_left1));
    Serial.println(digitalRead(this->_pin_left2));
}

////////////
//私有方法
////////////
void CarMotor::initPins()
{
    pinMode(_pin_left1, OUTPUT);
    pinMode(_pin_left2, OUTPUT);
    pinMode(_pin_right1, OUTPUT);
    pinMode(_pin_right2, OUTPUT);

    pinMode(_pin_left_en, OUTPUT);
    pinMode(_pin_right_en, OUTPUT);
}
