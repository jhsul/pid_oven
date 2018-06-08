# PID Oven

The goal of this project was to implement a simple PID control loop on an Arduino controller regulating the power to a toaster oven. By turning the oven on and off at varying intervals (I couldn't modify the voltage, just turn it on/off), the controller adjusts the temperature of the oven.

## Control Constants

```C
// PID constants
const double kp = 0.5f;
const double ki = 0.01f;
const double kd = 0.5f;
```
## Testing
To test the controller, I started with the oven at room temperature and set the setpoint to 100 °C. Once the oven became stable there, I changed the setpoint to 125 °C. 

*Note: there is an added delay in the graph because I used a separate temperature probe with less sensitivity to collect this data*

![alt text](graph.jpeg "Oven Data")