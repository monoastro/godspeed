#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

// Access Point credentials
const char *ssid = "ESP32_Hotspot";
const char *password = "12345678";

// PID parameters
float Kp = 0.0, Ki = 0.0, Kd = 0.0;

// Web server and WebSocket server
AsyncWebServer server(80);
WebSocketsServer webSocket(81);

// WebSocket event handler
void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t len) {
  if (type == WStype_TEXT) {
    String message = String((char *)payload).substring(0, len);

    // Handle slider updates
    if (message.startsWith("Kp=")) {
      Kp = message.substring(3).toFloat();
      Serial.printf("Updated Kp: %.2f, Ki: %.2f, Kd: %.2f\n", Kp, Ki, Kd);
    } else if (message.startsWith("Ki=")) {
      Ki = message.substring(3).toFloat();
      Serial.printf("Updated Kp: %.2f, Ki: %.2f, Kd: %.2f\n", Kp, Ki, Kd);
    } else if (message.startsWith("Kd=")) {
      Kd = message.substring(3).toFloat();
      Serial.printf("Updated Kp: %.2f, Ki: %.2f, Kd: %.2f\n", Kp, Ki, Kd);
    } else if (message == "getValues") {
      // Send the current PID values back to the client
      String response = "{\"Kp\":" + String(Kp) + ",\"Ki\":" + String(Ki) + ",\"Kd\":" + String(Kd) + "}";
      webSocket.sendTXT(num, response);  // Send to the requesting client
    }
  }
}

// Initialize WebSocket
void initWebSocket() {
  webSocket.onEvent(handleWebSocketMessage);
  webSocket.begin();
}

// Serve HTML page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Web Server</title>
    <script>
    var gateway = `ws://${window.location.hostname}:81/`;
    var websocket;
    window.addEventListener('load', onload);

    function onload(event) {
        initWebSocket();
    }

    function getValues() {
        websocket.send("getValues");
    }

    function initWebSocket() {
        console.log('Trying to open a WebSocket connection…');
        websocket = new WebSocket(gateway);
        websocket.onopen = onOpen;
        websocket.onclose = onClose;
        websocket.onmessage = onMessage;
    }

    function onOpen(event) {
        console.log("Connection Opened");
        getValues();
    }

    function onMessage(event) {
        console.log("Received message: " + event.data);
        const data = JSON.parse(event.data);

        // Update PID sliders
        if (data.Kp !== undefined) document.getElementById("slider_value1").innerHTML = data.Kp;
        if (data.Ki !== undefined) document.getElementById("slider_value2").innerHTML = data.Ki;
        if (data.Kd !== undefined) document.getElementById("slider_value3").innerHTML = data.Kd;
    }

    function onClose(event) {
        console.log("Connection Closed");
        setTimeout(initWebSocket, 2000);
    }

    function updateSliderPID(element) {
        let parameter = "";
        switch (element.id) {
            case "slider1":
                parameter = "Kp";
                break;
            case "slider2":
                parameter = "Ki";
                break;
            case "slider3":
                parameter = "Kd";
                break;
            default:
                console.error("Unknown slider ID");
                return;
        }

        let value = parseFloat(element.value);
        document.getElementById("slider_value" + element.id.charAt(element.id.length - 1)).innerHTML = value.toFixed(2);
        websocket.send(`${parameter}=${value}`);
        console.log(`Sent: ${parameter}=${value}`);
    }
    </script>
</head>
<body>
    <div class="topnav">
        <h1>ESP32 PID Web Server</h1>
    </div>
    <div class="content">
        <div class="card-grid">
            <div class="card">
                <p class="card-title state">Kp: <span id="slider_value1"></span></p>
                <p class="switch">
                    <input onchange="updateSliderPID(document.getElementById('slider1'))" type="range" id="slider1" min="0" max="3" step="0.02" value="0" class="slider">
                </p>
            </div>
            <div class="card">
                <p class="card-title state">Ki: <span id="slider_value2"></span></p>
                <p class="switch">
                    <input onchange="updateSliderPID(document.getElementById('slider2'))" type="range" id="slider2" min="0" max="3" step="0.02" value="0" class="slider">
                </p>
            </div>
            <div class="card">
                <p class="card-title state">Kd: <span id="slider_value3"></span></p>
                <p class="switch">
                    <input onchange="updateSliderPID(document.getElementById('slider3'))" type="range" id="slider3" min="0" max="3" step="0.02" value="0" class="slider">
                </p>
            </div>
        </div>
    </div>
</body>
</html>
)rawliteral";

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Set ESP32 as Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Serve the webpage
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Start WebSocket and server
  initWebSocket();
  server.begin();
}

void loop() {
  // Handle WebSocket events
  webSocket.loop();
}
