#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup()
{
  Serial.begin(115200);

  // Try to initialize!
  if (!mpu.begin())
  {
    // Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }

  // Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRangeMPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
}

double a_avg_x = 0;
double a_treadmill_x[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
double avg_treadmill_x[5] = {0, 0, 0, 0, 0};
double a_avg2_x = 0;

double a_avg_y = 0;
double a_treadmill_y[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void loop()
{
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Setting bounds on X-acceleration

  if (a.acceleration.x > 2G)
  {
    a.acceleration.x = 2G;
  }

  if (a.acceleration.x < -2G)
  {
    a.acceleration.x = -2G;
  }

  // Setting bounds on Y-acceleration

  if (a.acceleration.y > 2G)
  {
    a.acceleration.y = 2G;
  }

  if (a.acceleration.y < -2G)
  {
    a.acceleration.y = -2G;
  }

  /* Print out the values */
  // Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print('\t');

  // Serial.print("Acceleration Y: ");
  Serial.print(a.acceleration.y);
  Serial.print('\t');

  // for loop in the x-axis, average of 10 accel-x values
  for (int i = 0; i < 9; i++)
  {
    a_treadmill_x[i] = a_treadmill_x[i + 1];
  }
  a_treadmill_x[9] = a.acceleration.x;

  for (int i = 0; i <= 9; i++)
  {
    a_avg_x += a_treadmill_x[i];
  }
  a_avg_x /= 10;

  // taking the average of 5 averaged accel-x (revise; does not work very well)

  for (int i = 0; i < 4; i++)
  {
    avg_treadmill_x[i] = avg_treadmill_x[i + 1];
  }
  avg_treadmill_x[4] = a_avg_x;

  for (int i = 0; i <= 4; i++)
  {
    a_avg2_x += avg_treadmill_x[i];
  }

  a_avg2_x /= 5;

  // for loop in the y-axis

  for (int i = 0; i < 9; i++)
  {
    a_treadmill_y[i] = a_treadmill_y[i + 1];
  }
  a_treadmill_y[9] = a.acceleration.y;

  for (int i = 0; i <= 9; i++)
  {
    a_avg_y += a_treadmill_y[i];
  }
  a_avg_y = a_avg_y / 10;

  // Serial.print("Moving average: ");
  Serial.print(a_avg_x);
  Serial.print('\t'); // needed for plotting multiple variables; must be placed after every variable

  // Serial.print("Average of the averages: ");
  Serial.print(a_avg2_x);
  Serial.print('\t'); // needed for plotting multiple variables; must be placed after every variable

  Serial.print(a_avg_y);
  Serial.print('\t');

  Serial.println("");
  delay(50);
}