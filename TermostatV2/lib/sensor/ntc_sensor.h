#pragma once
#include <Arduino.h>
#include <vector>

using namespace std;
#define MAX_VECTOR_SIZE 5

#define THRESHOLD 0.5
// #define ENV_THRESHOLD 0.5
// #define FLOOR_THRESHOLD 1

extern TaskHandle_t updateStatusTaskHandle;

class NTC{
    private:
        vector<float>floorReadings;
        vector<float>environmentReadings;
        const double beta {3950.0};
        const double r0 {100000.0};
        const double t0 {273.15 + 25.0};
        const double rx {r0 * exp(-beta/t0)};
        const double R {100000.0};
        double vcc {1.2};
        double vref_esp;

        float lastEnvironmentTemperature;
        float lastFloorTemperature;
        float floorTemperature;
        float environmentTemperature;
        bool floorSensor {false};
        bool measurementError {false};
        int envErrorCount {0};
        int floorErrorCount {0};

    public:
    
        /**
         * @brief Initialize temperature sensor pins.
         * 
         */
        void init();

        /**
         * @brief Verifies if the device has a floor sensor connected.
         * 
         */
        void detectFloorSensor();

        /**
         * @brief Returns if has a floor sensor.
         * 
         * @return true 
         * @return false 
         */
        bool hasFloorSensor();

        /**
         * @brief Return sensor error state
         * 
         * @return true (has error)
         * @return false (has no error)
         */
        bool hasSensorError();

        /**
         * @brief Read the sensor (0 - NTC, 1 - Floor) and calculates temperature.
         * 
         */
        bool calculateTemperature(bool floorSensor);

        /**
         * @brief Return floor temperature.
         * 
         * @return float 
         */
        float getFloorTemp();
        
        /**
         * @brief Return environment temperature.
         * 
         * @return float 
         */
        float getEnvTemp();

};

extern NTC ntc;