#include <ESP8266WiFi.h>

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Robot Arm Controller</title>
<style>
*{
    margin:0;
    padding:0;
    box-sizing:border-box;
}
body{
    background:#121212;
    color:white;
    font-family:Arial,Helvetica,sans-serif;
    display:flex;
    justify-content:center;
    align-items:center;
    min-height:100vh;
}
.container{
    width:360px;
    background:#1e1e1e;
    border-radius:16px;
    padding:20px;
    box-shadow:0 0 20px rgba(0,0,0,0.4);
}
h1{
    text-align:center;
    margin-bottom:20px;
    font-size:28px;
}
.grid{
    display:grid;
    grid-template-columns:1fr 1fr;
    gap:12px;
}
button{
    width:100%;
    height:50px;
    border:none;
    border-radius:10px;
    background:#2196F3;
    color:white;
    font-size:17px;
    cursor:pointer;
}
button:hover{
    background:#1976D2;
}
input{
    width:100%;
    height:45px;
    margin-top:20px;
    border:none;
    border-radius:10px;
    padding-left:12px;
    font-size:17px;
    outline:none;
}
.send{
    margin-top:15px;
}
.footer{
    margin-top:18px;
    text-align:center;
    color:#888;
    font-size:13px;
}
</style>
<script>
function send(path){
    fetch(path);
}
function sendCommand(){
    let cmd=document.getElementById("cmd").value.trim();
    if(cmd=="")
        return;
    fetch("/command?cmd="+encodeURIComponent(cmd));
    document.getElementById("cmd").value="";
}
document.addEventListener("DOMContentLoaded",function(){
    document.getElementById("cmd").addEventListener("keypress",function(e){
        if(e.key==="Enter"){
            e.preventDefault();
            sendCommand();
        }
    });
});
</script>
</head>
<body>
<div class="container">
<h1>Robot Arm</h1>
<div class="grid">
<button type="button" onclick="send('/home')">HOME</button>
<button type="button" onclick="send('/stop')">STOP</button>
<button type="button" onclick="send('/save')">SAVE</button>
<button type="button" onclick="send('/play')">PLAY</button>
<button type="button" onclick="send('/undo')">UNDO</button>
<button type="button" onclick="send('/clear')">CLEAR</button>
<button type="button" onclick="send('/status')">STATUS</button>
<button type="button" onclick="document.getElementById('cmd').focus()">COMMAND</button>
</div>
<input
id="cmd"
type="text"
placeholder="Example : B120">
<button
class="send"
type="button"
onclick="sendCommand()">
SEND
</button>
<div class="footer">
ESP8266 Web Controller
</div>
</div>
</body>
</html>
)rawliteral";

const char *ssid = "RoboticArm";
const char *password = "12345678";
WiFiServer server(80);
void sendWebPage(WiFiClient &c) {
  c.println("HTTP/1.1 200 OK");
  c.println("Content-Type: text/html");
  c.println("Connection: close");
  c.println();
  c.print(FPSTR(webpage));
}
void sendOK(WiFiClient &c) {
  c.println("HTTP/1.1 200 OK");
  c.println("Content-Type: text/plain");
  c.println("Connection: close");
  c.println();
  c.println("OK");
}
void handleRequest(const String &r) {
  if (r.indexOf("/home") >= 0) Serial.println("HOME");
  else if (r.indexOf("/stop") >= 0) Serial.println("STOP");
  else if (r.indexOf("/save") >= 0) Serial.println("SAVE");
  else if (r.indexOf("/play") >= 0) Serial.println("PLAY");
  else if (r.indexOf("/undo") >= 0) Serial.println("UNDO");
  else if (r.indexOf("/clear") >= 0) Serial.println("CLEAR");
  else if (r.indexOf("/status") >= 0) Serial.println("STATUS");
  else if (r.indexOf("/command?cmd=") >= 0) {
    int s = r.indexOf("cmd=");
    if (s >= 0) {
      s += 4;
      int e = r.indexOf(' ', s);
      Serial.println(r.substring(s, e));
    }
  }
}
void setup() {
  Serial.begin(9600);

  Serial.println();
  Serial.println("===== ESP BOOT =====");
  Serial.print("Reset reason: ");
  Serial.println(ESP.getResetReason());

  WiFi.softAP(ssid, password);
  server.begin();
}
void loop() {
  WiFiClient c = server.available();
  if (!c) return;
  while (!c.available()) delay(1);
  String req = c.readStringUntil('\r');
  c.flush();
  if(req.startsWith("GET / ")) sendWebPage(c);
  else {
    handleRequest(req);
    sendOK(c);
  }
  delay(5);
  c.stop();
}
