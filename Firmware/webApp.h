void cmotsValues();
void handleRoot();
void sendRedirect(String uri);
void handleGPIO();

void cmotsValues(){

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

    page += String(F("</body></html>"));

    page += String(F("{\n\""));
    page += String((hostName));
    page += String(F("\":{\n"));
    page += String(F("{\n\"OEE\":"));
    page += String((OEEValue));
    page += String(F(",\n\"Temperature\":"));
    page += String((getTemp()));
    page += String(F(",\n\"Humidity\":"));
    page += String((getHumid()));
    page += String(F(",\n\"AmpsMAX,Watts\":"));
    page += String((getCurrentWatts()));
    page += String(F("\n}\n}"));
   
    page += String(F("<p><br><a class=\"button\" href=\"/api-now\">Refresh</a></p>"));

    page += String(F("</body></html>"));
    server.send(200, "text/html", page);


}
void handleRoot() {
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
  "Smart T Controller<br>"
  "GPIO(");
  page += String(F("</body></html>"));
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
    page += String((getTemp()));
    page += String(F("</h3>"));
    page += String(F("<h3>"));
    page += String(F("Humidity: "));
    page += String((getHumid()));
    page += String(F("</h3>"));
    page += String(F("<h3>"));
    page += String(F("AmpsMAX and Watts"));
    page += String((getCurrentWatts()));
    page += String(F("</h3>"));
   
    page += String(F("<p><br><a class=\"button\" href=\"/\">Refresh</a></p>"));

    page += String(F("</body></html>"));

  
  server.send(200, "text/html", page);
}
void redirectToHome() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
  //server.client().stop();
}
void sendRedirect(String uri) {
  server.sendHeader("Location", uri, true);
  server.send(302, "text/plain", "");
  server.client().stop();
}
void handleCON() {
  String argV=String(server.arg("v"));
  if(argV=="wifi"){
    connectionMode="WiFi";
  }
  else if(argV=="gprs"){
    connectionMode="GPRS";
  }
  
  redirectToHome();
  
}
void handleDEV() {
  String argV=String(server.arg("v"));
  Serial.println(argV);
  selectedDeviceIndex= argV.toInt();
  redirectToHome();
  
}
// void handleGPIO() {
//   OLDemailAddress=emailAddress;
//   String em=String(server.arg("email"));
//   emailAddress=em;
//   String pas=String(server.arg("pass"));
//   //String freq=String(server.arg("freq"));
//   Serial.println(em);
//   Serial.println(pas);
//   String dataV=String(em);
//   dataV+=String(";");
//   dataV+=String(pas);
//   mqttPublish("SmartTControl/creds/data",dataV);
//   //mqttPublish("SmartTControl/device/freq",freq);
//   //mqttPublish("SmartTControl/device/run","3");//to run
//   //reconnect();//for new email address subscription
//   MQTTUnSubscribe();
//   delay(0.5);
//   MQTTSubscriptions();
//   if (server.arg("v") == "low"){
//     digitalWrite(BUILTIN_LED, LOW);
//     mqttPublish("SmartTControl/device/run","0");//to off
//   }
//   else if (server.arg("v") == "high"){
//     digitalWrite(BUILTIN_LED, HIGH);
//     mqttPublish("SmartTControl/device/run","1");//to off
//     }
//    else{
//      Serial.println(String(server.arg("v")));
    
//    }
   
//   sendRedirect("/");
// }
