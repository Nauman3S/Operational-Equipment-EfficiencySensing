//Software Stack

class SoftwareStack
{
public:
#define DebugPrint(x) Serial.println(x)

    String StringSeparator(String data, char separator, int index);
    char *StrToCharArray(String data);
    void addSensorValue(String oee, String temperatureV, String Humid, String Amps);
    String getSensorsArray();
    String getSensorsJSON();
    SoftwareStack();

private:
    //  String configs="";
    char buf[100];
    String SensorVals[30];

    int sensorVCounter = 0;
};
SoftwareStack::SoftwareStack()
{
    SensorVals[0] = String("0,0,0,0");
}
String SoftwareStack::getSensorsJSON()
{
    String temp = "";
    for (int i = 0; i < sensorVCounter; i++)
    {
        temp += "{\n";
        temp += "{\n\"OEE\":";
        temp += StringSeparator(SensorVals[i], ',', 0);
        temp += ",\n\"Temperature\":";
        temp += StringSeparator(SensorVals[i], ',', 1);
        temp += ",\n\"Humidity\":";
        temp += StringSeparator(SensorVals[i], ',', 2);
        temp += ",\n\"Max Amps Measured\":";
        temp += StringSeparator(SensorVals[i], ',', 3);
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
        temp += "</tr>";
        temp += "\n";
    }
    return temp;
}
void SoftwareStack::addSensorValue(String oee, String temperatureV, String Humid, String Amps)
{
    if (sensorVCounter >= 30)
    {
        sensorVCounter = 0; //reset from 0;
    }
    String temp = oee + String(",") + temperatureV + String(",") + Humid + String(",") + Amps;
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
