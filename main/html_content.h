#pragma once

const char HTML_TEMPLATE[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="th">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 LED Dashboard</title>
  <style>
    body {
      font-family: sans-serif;
      background: #1e1e2e;
      color: #cdd6f4;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      min-height: 100vh;
      margin: 0;
    }
    h1 { font-size: 1.6rem; margin-bottom: 0.3rem; }
    .card {
      background: #313244;
      border-radius: 16px;
      padding: 2rem 3rem;
      text-align: center;
      box-shadow: 0 4px 20px #0006;
    }
    .led {
      width: 80px;
      height: 80px;
      border-radius: 50%;
      margin: 1rem auto;
      background: %LED_COLOR%;
      box-shadow: 0 0 20px %LED_COLOR%;
      transition: background 0.3s;
    }
    .status { font-size: 1.1rem; margin-bottom: 1.5rem; }
    .btn {
      display: inline-block;
      padding: .7rem 1.6rem;
      border-radius: 8px;
      color: #fff;
      font-size: 1rem;
      text-decoration: none;
      margin: .4rem;
      cursor: pointer;
      border: none;
    }
    .btn-toggle { background: %BTN_COLOR%; }
    .speed-box { margin-top: 1.2rem; }
    label { font-size: .9rem; }
    input[type=range] { width: 200px; margin: .5rem 0; }
    .delay-val { font-weight: bold; color: #89b4fa; }
    .info { margin-top: 1.5rem; font-size: .8rem; color: #888; }
  </style>
  <script>
    setTimeout(() => location.reload(), 2000);
    function setSpeed(v) {
      document.getElementById('dv').innerText = v + ' ms';
      fetch('/speed?delay=' + v);
    }
  </script>
</head>
<body>
<div class="card">
  <h1>ESP32 LED Dashboard</h1>
  <div class="led"></div>
  <div class="status">
    สถานะ: <b>%BLINK_STATUS%</b> | LED: <b>%LED_STATE%</b>
  </div>
  <a class="btn btn-toggle" href="/toggle">%BTN_TEXT%</a>
  <div class="speed-box">
    <label>ความเร็ว: <span id="dv" class="delay-val">%DELAY_VAL% ms</span></label><br>
    <input type="range" min="100" max="3000" step="100"
           value="%DELAY_VAL%"
           onchange="setSpeed(this.value)"
           oninput="document.getElementById('dv').innerText=this.value+' ms'">
    <br><small>100 ms = เร็ว &nbsp;|&nbsp; 3000 ms = ช้า</small>
  </div>
  <div class="info">ESP32 IP: %ESP_IP%</div>
</div>
</body>
</html>
)rawhtml";
