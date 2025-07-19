import processing.serial.*;

int portNumber = 0;

// Configuration constants
const float DEFAULT_CENTER = -9.81;  // Default center value for gravity
const float DEFAULT_SCALE = 20.0;    // Default scale for Y-axis
const int MIN_SCALE = 5;             // Minimum scale value
const int MAX_SCALE = 50;            // Maximum scale value

Serial port;
float[][] values;
float AR[] = {1.0, 1.0};
float ARalpha = 0.005;
float zoom;
byte lf = 10;
float alpha = 0.4;

long[] triggerTime = {0, 0};
long[] prevTriggerTime = {0, 0};
float[] topValue = {0.0, 0.0};
float[] bottomValue = {0.0, 0.0};
float[] amp = {0, 0};
float[] freq = {0, 0};

void setup() {
  size(640, 480);
  println(Serial.list());
  port = new Serial(this, Serial.list()[portNumber], 115200);
  port.bufferUntil(lf);
  values = new float[2][width];
  zoom = 1.0f;
  smooth();
}

// Convert sensor value to screen Y coordinate
int getY(float val) {
    // Constrain value to prevent extreme outliers
    val = constrain(val, DEFAULT_CENTER - DEFAULT_SCALE, DEFAULT_CENTER + DEFAULT_SCALE);
    
    // Calculate screen position:
    // - height/2 is the center of the screen
    // - (val - center) is the offset from the center
    // - (height/scale) is the pixels per unit
    return (int)((height / 2) - (val - DEFAULT_CENTER) * (height / DEFAULT_SCALE));
}


void serialEvent(Serial port) {
  String[] valuesString = split(trim(port.readString()), ",");
  if (valuesString.length == 2) {
    float[] zOnly = new float[2];
    zOnly[0] = float(valuesString[0]);
    zOnly[1] = float(valuesString[1]);
    pushZValue(zOnly);
  }
}

void pushZValue(float[] zValues) {
  for (int sensor = 0; sensor < 2; sensor++) {
    float newValue = zValues[sensor];

    for (int i = 0; i < width - 1; i++) {
      values[sensor][i] = values[sensor][i + 1];
    }
    values[sensor][width - 1] = newValue;

    AR[sensor] = ((1 - ARalpha) * AR[sensor]) + (ARalpha * newValue);

    topValue[sensor] = max(topValue[sensor], newValue);
    bottomValue[sensor] = min(bottomValue[sensor], newValue);

    if ((values[sensor][width - 10] < AR[sensor]) && (newValue > AR[sensor])) {
      processTrigger(sensor);
    }
  }
}

void processTrigger(int sensor) {
  prevTriggerTime[sensor] = triggerTime[sensor];
  triggerTime[sensor] = millis();
  amp[sensor] = ((1 - alpha) * amp[sensor]) + (alpha * (topValue[sensor] - bottomValue[sensor]));

  float freqObs = 1000.0 / (float)(triggerTime[sensor] - prevTriggerTime[sensor]);
  if (freqObs < 20) {
    freq[sensor] = ((1 - alpha) * freq[sensor]) + (alpha * freqObs);
  }

  topValue[sensor] = AR[sensor];
  bottomValue[sensor] = AR[sensor];
}

void drawLines() {
  stroke(255);
  int displayWidth = (int)(width / zoom);
  int k = values[0].length - displayWidth;

  int x0 = 0;
  int y10 = getY(values[0][k]);
  int y20 = getY(values[1][k]);

  for (int i = 1; i < displayWidth; i++) {
    k++;
    int x1 = (int)(i * (width - 1) / (displayWidth - 1));
    int y11 = getY(values[0][k]);
    int y21 = getY(values[1][k]);

    stroke(255, 0, 0);
    line(x0, y10, x1, y11);

    stroke(0, 255, 0);
    line(x0, y20, x1, y21);

    x0 = x1;
    y10 = y11;
    y20 = y21;
  }
}

void drawGrid() {
  stroke(255, 0, 0);
  line(0, getY(AR[0]), width, getY(AR[0]));

  stroke(0, 255, 0);
  line(0, getY(AR[1]), width, getY(AR[1]));
}

void keyReleased() {
  switch (key) {
    case '+':
      zoom *= 2.0f;
      if ((int)(width / zoom) <= 1) zoom /= 2.0f;
      break;
    case '-':
      zoom /= 2.0f;
      if (zoom < 1.0f) zoom *= 2.0f;
      break;
    case 's':
      exit();
      break;
  }
}


void drawInfo() {
  textSize(16);
  fill(255);
  text("s: stop program \n+: zoom in (time)\n-: zoom out (time)", 450, 400);
}

void draw() {
  background(0);
  drawGrid();
  drawLines();
  drawInfo();
}
