#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <CarMotor.h>
#include <ArduinoJson.h>

// const char *SSID = "ShaFei";
// const char *SSID_PASSWORD = "99233123";

#define MQTT_SERVER "192.168.1.106" //mqtt服务器地址
#define MQTT_PORT 61613             //mqtt服务器端口
#define MQTT_USERNAME "admin"       //mqtt服务器用户名
#define MQTT_PASSWORD "password"    //mqtt服务器登录密码

// 添加订阅消息
char *TOPIC_IN_MOVE = "car/move"; //接受订阅小车移动

#define TOPIC_OUT_ULTRASONIC "car/ultrasonic" //输出订阅超声波距离

int oldOutTime = 0;           //上次发送数据时间
const int DEFAULT_TIME = 500; //发送间隔时长

WiFiClient wifiClient;           //wifi客户端驱动
PubSubClient client(wifiClient); //MQTT协议驱动

CarMotor carMotor(D0, D1, D2, D3);
/**
 * Esp8266-12F智能组网
 */
void configWifi()
{
    Serial.println("Start config wifi with smart");

    WiFi.mode(WIFI_AP_STA);  //设置模式全双工模式
    WiFi.beginSmartConfig(); //等待智能组网设置wifi
    Serial.print("Wait config");

    Serial.println("");
    Serial.print("Congratulations..... Smart config wifi Success!");
    Serial.println(WiFi.smartConfigDone());
    //打印ip
    Serial.println("");
    Serial.println("**********WiFi connected**********");
    Serial.print("\tIP address: ");
    Serial.println(WiFi.localIP());
    delay(10);
}

/**
 * MQTT服务订阅消息的回调，收到订阅的广播将会回调该方法
 */
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    String json = "";
    for (unsigned int i = 0; i < length; i++)
    {
        json += (char)payload[i];
    }

    //判断topic，根据类型统统分别做不同的操作
    if (strcmp(topic, TOPIC_IN_MOVE) == 0)
    {
        Serial.println(json);
        Serial.println("============");
        //获取xy坐标
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(json);
        const char x = root["x"];
        const char y = root["y"];

        carMotor.run(x, y);
    }
}
/**
 * 重新连接MQTT服务器
 */
void reconnect()
{
    char retryCount = 5;
    while (!client.connected())
    {
        Serial.print("Start attempting connection MQTT service ");
        Serial.print(MQTT_SERVER);
        Serial.println(" Please wait...");
        if (client.connect("ESP8266-12FCarClient", MQTT_USERNAME, MQTT_PASSWORD))
        {
            //连接成功  订阅消息
            Serial.println("connected");
            //订阅输入主题
            client.subscribe(TOPIC_IN_MOVE);
        }
        else
        {
            // 连接失败，等待5秒重试
            Serial.print("Connect to MQTT service ");
            Serial.print(MQTT_SERVER);
            Serial.print(" failed, status is : ");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
            retryCount--;
            if (retryCount < 0)
            {
                //停止重试
                Serial.println("");
                Serial.print(" It has tried 5 times again, But can not connect to MQTT, It will stop...");
                break;
            }
        }
    }
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    delay(10);
    //配置wifi
    configWifi();
    //配置mqtt服务器
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(mqttCallback);
}

void loop()
{
    // put your main code here, to run repeatedly:
    //Check The MQTT service is conected
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    //MQTT发布输出,每间隔固定时长发布一次数据到服务器
    if (micros() - oldOutTime > DEFAULT_TIME)
    {
        client.publish(TOPIC_OUT_ULTRASONIC, "Hello"); //发送超声波测距数据
        oldOutTime = micros();
    }

    // client.subscribe(TOPIC_OUT_ULTRASONIC);
}