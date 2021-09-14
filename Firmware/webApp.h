void cmotsValues();
void handleRoot();
void sendRedirect(String uri);
void handleGPIO();

void cmotsValues()
{

    String page = PSTR(
        "<html>"
        "</head>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<style type=\"text/css\">"
        "body {"
        "-webkit-appearance:none;"
        "-moz-appearance:none;"
        "font-family:'Arial',sans-serif;"
        "text-align:center;"
        "}"
        ".menu > a:link {"
        "position: absolute;"
        "display: inline-block;"
        "right: 12px;"
        "padding: 0 6px;"
        "text-decoration: none;"
        "}"
        ".button {"
        "display:inline-block;"
        "border-radius:7px;"
        "background:#73ad21;"
        "margin:0 10px 0 10px;"
        "padding:10px 20px 10px 20px;"
        "text-decoration:none;"
        "color:#000000;"
        "}"
        "</style>"
        "</head>"
        "<body>"
        "<div class=\"menu\">" AUTOCONNECT_LINK(BAR_24) "</div>"
                                                        "Live API<br>");

    

    page += String(F("{\n\""));
    page += String((hostName));
    page += String(F("\":{\n"));
    page += String(F("{\n\"OEE\":"));
    page += String((OEEValue));
    page += String(F(",\n\"Temperature\":"));
    page += String((getTemp(tempUnits)));
    page += String(F(",\n\"Humidity\":"));
    page += String((getHumid()));
    page += String(F(",\n\"Max Amps Measured\":"));
    page += String((getCurrentWatts()));
    page += String(F("\n}\n}"));

    page += String(F("<p><br><a class=\"button\" href=\"/api-now\">Refresh</a></p>"));

    page += String(F("</body></html>"));
    server.send(200, "text/html", page);
}
void api()
{

    String page = PSTR(
        "<html>"
        "</head>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<style type=\"text/css\">"
        "body {"
        "-webkit-appearance:none;"
        "-moz-appearance:none;"
        "font-family:'Arial',sans-serif;"
        "text-align:center;"
        "}"
        ".menu > a:link {"
        "position: absolute;"
        "display: inline-block;"
        "right: 12px;"
        "padding: 0 6px;"
        "text-decoration: none;"
        "}"
        ".button {"
        "display:inline-block;"
        "border-radius:7px;"
        "background:#73ad21;"
        "margin:0 10px 0 10px;"
        "padding:10px 20px 10px 20px;"
        "text-decoration:none;"
        "color:#000000;"
        "}"
        "</style>"
        "</head>"
        "<body>"
        "<div class=\"menu\">" AUTOCONNECT_LINK(BAR_24) "</div>"
                                                        "Historical API<br>");

    

    page += String(F("{\n\""));
    page += String((hostName));
    page += String(F("\":"));
    
    page += String((ss.getSensorsJSON()));
    
    page += String(F("\n}"));

    page += String(F("<p><br><a class=\"button\" href=\"/api\">Refresh</a></p>"));

    page += String(F("</body></html>"));
    server.send(200, "text/html", page);
}

void handleRoot()
{
    String page = PSTR(
        "<html>"
        "</head>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<style type=\"text/css\">"
        "table, th, td {"
        "  border: 1px solid black;"
        "}"
        "table.center {"
        "margin-left: auto; "
        "margin-right: auto;"
        "}"
        "body {"
        "-webkit-appearance:none;"
        "-moz-appearance:none;"
        "font-family:'Arial',sans-serif;"
        "text-align:center;"
        "}"
        ".menu > a:link {"
        "position: absolute;"
        "display: inline-block;"
        "right: 12px;"
        "padding: 0 6px;"
        "text-decoration: none;"
        "}"
        ".button {"
        "display:inline-block;"
        "border-radius:7px;"
        "background:#73ad21;"
        "margin:0 10px 0 10px;"
        "padding:10px 20px 10px 20px;"
        "text-decoration:none;"
        "color:#000000;"
        "}"
        "</style>"
        "</head>"
        "<body>"
        "<div class=\"menu\">" AUTOCONNECT_LINK(BAR_24) "</div>"
                                                        "OEE Sensing<br>");

    page += String(F("<h1>Historical Sensors Data</h1>"));
    page += String(F("<table class=\"center\">"));
    page += String(F("<tr>"));
    page += String(F("<th>OEE</th>"));
    page += String(F("<th>Temperature</th>"));
    page += String(F("<th>Humidity</th>"));
    page += String(F("<th>Amps Max</th>"));
    page += String(F("</tr>"));
    page += String(ss.getSensorsArray());

    page += String(F("</table>"));
    page += String(F("<p><br><a class=\"button\" href=\"/\">Refresh</a></p>"));
    page += String(F("</body></html>"));

    server.send(200, "text/html", page);
}

void redirectToHome()
{
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
    //server.client().stop();
}
void dataTable(){
    redirectToHome();
}
void live()
{
    String page = PSTR(
        "<html>"
        "</head>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<style type=\"text/css\">"
        "body {"
        "-webkit-appearance:none;"
        "-moz-appearance:none;"
        "font-family:'Arial',sans-serif;"
        "text-align:center;"
        "}"
        ".menu > a:link {"
        "position: absolute;"
        "display: inline-block;"
        "right: 12px;"
        "padding: 0 6px;"
        "text-decoration: none;"
        "}"
        ".button {"
        "display:inline-block;"
        "border-radius:7px;"
        "background:#73ad21;"
        "margin:0 10px 0 10px;"
        "padding:10px 20px 10px 20px;"
        "text-decoration:none;"
        "color:#000000;"
        "}"
        "</style>"
        "</head>"
        "<body>"
        "<div class=\"menu\">" AUTOCONNECT_LINK(BAR_24) "</div>"
                                                        "OEE Sensing<br>");

    page += String(F("<h1>Live Device Values</h1>"));
    page += String(F("<h2>DeviceName: "));
    page += String((hostName));
    page += String(F("</h2>"));
    page += String(F("<h2>Device Data</h2>"));
    page += String(F("<h3>"));
    page += String(F("OEE: "));
    page += String((OEEValue));
    page += String(F("</h3>"));
    page += String(F("<h3>"));
    page += String(F("Temperature: "));
    page += String((getTemp(tempUnits)));
    page += String(F("</h3>"));
    page += String(F("<h3>"));
    page += String(F("Humidity: "));
    page += String((getHumid()));
    page += String(F("</h3>"));
    page += String(F("<h3>"));
    page += String(F("Max Amps Measured"));
    page += String((getCurrentWatts()));
    page += String(F("</h3>"));
    page += String(F("<br><br><h3>Data is being published to MQTT Topic: "));
    String dTopic = "OEE/" + hostName;
    page += String((dTopic));
    page += String(F("</h3>"));
    page += String(F("<p><br><a class=\"button\" href=\"/LiveSensors\">Refresh</a></p>"));

    page += String(F("</body></html>"));

    server.send(200, "text/html", page);
}
void sendRedirect(String uri)
{
    server.sendHeader("Location", uri, true);
    server.send(302, "text/plain", "");
    server.client().stop();
}
void handleCON()
{
    String argV = String(server.arg("v"));
    if (argV == "wifi")
    {
        connectionMode = "WiFi";
    }
    else if (argV == "gprs")
    {
        connectionMode = "GPRS";
    }

    redirectToHome();
}
void handleDEV()
{
    String argV = String(server.arg("v"));
    Serial.println(argV);
    selectedDeviceIndex = argV.toInt();
    redirectToHome();
}
