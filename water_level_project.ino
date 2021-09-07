#include "WiFi.h"
#include "ESPAsyncWebServer.h"
const char* ssid = "DIR-615-0D98";
const char* password = "123456789";
AsyncWebServer server(80);
#define GPIO34  34
#define GPIO35  35


float percentage() {
  digitalWrite(GPIO34, LOW);
  delayMicroseconds(2);
  digitalWrite(GPIO34, HIGH);
  delayMicroseconds(2);
  digitalWrite(GPIO34, LOW);
  long timereq = pulseIn(GPIO35, HIGH);
  long dist = timereq / 29 / 2;
  
  float percent = ( dist * 100 / 138);
  return percent;
}
String Percent() {
  return String(percentage());

}
///////////////////

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Times New Roman;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>WATER LEVEL IN TANK </h2>
  <p> 
    
    <span class="dht-labels">Waterlevel</span> 
    <span id="percentage">%PERCENTAGE%</span>
   
  </p>
  <p>%</p>

</body>
<script>
setInterval(
  function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("percentage").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/percentage", true);
  xhttp.send();
}, 1000) ;


</script>
</html>)rawliteral";


/////////////////

String processor(const String& var) {
  //Serial.println(var);
  if (var == "PERCENTAGE") {
    return String(Percent());
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(GPIO35, INPUT);
  pinMode(GPIO34, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());







  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/percentage", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", Percent().c_str());
  });


  // Start server
  server.begin();
}

void loop() {

}
