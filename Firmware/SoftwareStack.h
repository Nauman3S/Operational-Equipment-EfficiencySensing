//Software Stack

class SoftwareStack
{
public:
#define DebugPrint(x) Serial.println(x)

    String StringSeparator(String data, char separator, int index);
    char *StrToCharArray(String data);
    void addSensorValue(String Timestamp, String oee, String temperatureV, String Humid, String Amps);
    String getSensorsArray();
    String getSensorsJSON();
    String getOEEValue();
    void acquireAndStoreCurrentSensorValues_OEE(String CurrentValue);
    void calculateOEEValue();
    SoftwareStack();

private:
    //  String configs="";
    char buf[100];
    static const int maxDataPoints = 36;
    static const int maxOEEValueDataPoints = 3600;
    String SensorVals[maxDataPoints];
    int OEEArray[maxOEEValueDataPoints];
    int OEEArrayIndexCounter = 0;
    float recentOEEValue = 0;

    int sensorVCounter = 0;
};
SoftwareStack::SoftwareStack()
{

    for (int i = 0; i < maxDataPoints; i++)
    {
        SensorVals[i] = String("0,0,0,0,0");
    }
}
void SoftwareStack::acquireAndStoreCurrentSensorValues_OEE(String CurrentValue)//should run after every 1 second
{
    if (OEEArrayIndexCounter >= maxOEEValueDataPoints)
    {
        OEEArrayIndexCounter = 0; //reset counter
    }
    if (CurrentValue.toFloat() > minActiveValue.toFloat())
    {
        OEEArray[OEEArrayIndexCounter] = 1;
    }
    else
    {
        OEEArray[OEEArrayIndexCounter] = 0;
    }
}

void SoftwareStack::calculateOEEValue()//should run after every 5 mins
{
    int dataPoints = 0;
    float OEEPercent = 0.0;
    for (int i = 0; i < maxOEEValueDataPoints; i++)
    {
        dataPoints += OEEArray[i];
    }

    OEEPercent = (float)dataPoints / (float)maxOEEValueDataPoints;
    OEEPercent = OEEPercent / 100.0;
    recentOEEValue = OEEPercent;
}
String SoftwareStack::getOEEValue()
{
    String valStr = "";
    valStr = String(recentOEEValue);
    return valStr;
}
String SoftwareStack::getSensorsJSON()
{
    String temp = "";
    for (int i = 0; i < sensorVCounter; i++)
    {
        temp += "{\n";
        temp += "{\n\"Timestamp\":";
        temp += StringSeparator(SensorVals[i], ',', 0);
        temp += ",\n\"OEE\":";
        temp += StringSeparator(SensorVals[i], ',', 1);
        temp += ",\n\"Temperature\":";
        temp += StringSeparator(SensorVals[i], ',', 2);
        temp += ",\n\"Humidity\":";
        temp += StringSeparator(SensorVals[i], ',', 3);
        temp += ",\n\"Max Amps Measured\":";
        temp += StringSeparator(SensorVals[i], ',', 4);
        temp += "\n},";
    }
    return temp;
}
String SoftwareStack::getSensorsArray()
{
    String temp = "";
    for (int i = 0; i < sensorVCounter; i++)
    {
        //temp+=SensorVals[i];
        temp += "<tr>";
        temp += String("<td>") + StringSeparator(SensorVals[i], ',', 0) + String("</td>");
        temp += String("<td>") + StringSeparator(SensorVals[i], ',', 1) + String("</td>");
        temp += String("<td>") + StringSeparator(SensorVals[i], ',', 2) + String("</td>");
        temp += String("<td>") + StringSeparator(SensorVals[i], ',', 3) + String("</td>");
        temp += String("<td>") + StringSeparator(SensorVals[i], ',', 4) + String("</td>");
        temp += "</tr>";
        temp += "\n";
    }
    return temp;
}
void SoftwareStack::addSensorValue(String Timestamp, String oee, String temperatureV, String Humid, String Amps)
{
    if (sensorVCounter >= maxDataPoints)
    {
        sensorVCounter = 0; //reset from 0;
    }
    String temp = Timestamp + String(",") + oee + String(",") + temperatureV + String(",") + Humid + String(",") + Amps;
    // String temp="<tr>";
    // temp+=String("<td>")+oee+String("</td>");
    // temp+=String("<td>")+temperatureV+String("</td>");
    // temp+=String("<td>")+Humid+String("</td>");
    // temp+=String("<td>")+Amps+String("</td>");
    // temp+="</tr>";
    SensorVals[sensorVCounter] = temp;
    sensorVCounter++;
    temp = "";
}
char *SoftwareStack::StrToCharArray(String data)
{
    data.toCharArray(this->buf, data.length() + 1);
    return this->buf;
}

String SoftwareStack::StringSeparator(String data, char separator, int index)
{

    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
