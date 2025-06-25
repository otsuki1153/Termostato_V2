#include <ntc_sensor.h>
#include <gpios.h>
#include <utils.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

esp_adc_cal_characteristics_t adc_cal;
esp_adc_cal_value_t adc_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, 1100, &adc_cal);

NTC ntc;

void NTC::init() {
    pinMode(NTC_PIN, INPUT);
    pinMode(FLOOR_SENSOR_PIN, INPUT);
    pinMode(VREF_PIN, INPUT);
    analogSetPinAttenuation(VREF_PIN, ADC_2_5db);
    analogSetPinAttenuation(NTC_PIN, ADC_0db);
    analogSetPinAttenuation(FLOOR_SENSOR_PIN, ADC_0db);

    if (adc_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        debug("Vref eFuse encontrado: ");
        debugln(adc_cal.vref);
    } else if (adc_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        debug("Two Point eFuse encontrado: ");
        debugln(adc_cal.vref);
    } else {
        debug("utilizando Vref padrao: ");
        debugln(adc_cal.vref);
    }
    vref_esp = adc_cal.vref / 1000.0;
    detectFloorSensor();
}

void NTC::detectFloorSensor() {
    int adc = 0;
    for (int i = 0; i < 50; i++) {
        int16_t rawAdc = analogRead(FLOOR_SENSOR_PIN);
        adc += rawAdc;
    }
    adc = adc/50;

    if (adc > 930) {
        this-> floorSensor = true;
        debugln("Floor sensor detected!");
    } else {
        this-> floorSensor = false;
        debugln("Floor sensor not detected!");
    }
}

bool NTC::calculateTemperature(bool floorSensor) {
    uint averageReading = 0;
    for(int j = 0; j < 1000; j++){
        uint avg = 0;
        for(int i = 0; i < 100; i++){
            avg += analogRead(VREF_PIN);
        }
        averageReading += avg/100;
    }
    Serial.printf("\nvref -> %.3f\nAnalogRead: %d\n", vref_esp, (averageReading/1000));
    double tmp_vcc = ((averageReading/1000) / 4095.0) * (1.25*vref_esp);
    if(tmp_vcc > 1.29 || tmp_vcc < 1.1)
        tmp_vcc = 0;
    else this->vcc = tmp_vcc;
    int16_t f_temp;
    float res_avg = 0;
    
    float avg_temp = 0;
    int avg_analog = 0;
    float avg_volt = 0;

    for (int j = 0; j < 50; j++){
        float tempF = 0;
        int tmp_analog = 0;
        float tmp_res = 0;
        float tmp_volt = 0;
        for (int i = 0; i<50; i++){
            f_temp = floorSensor == 0 ? analogRead(NTC_PIN) : analogRead(FLOOR_SENSOR_PIN);
            tmp_analog += f_temp;

            double v = (vref_esp*f_temp)/4095.0;
            double rt = (vcc*R)/v - R;
            double t = beta / log(rt/rx);
            t = t-273.15;
            tempF += t;
        }

        tmp_volt = tmp_volt/50;
        tempF = tempF/50;
        tmp_analog = tmp_analog/50;
        tmp_res = tmp_res/50;
        avg_temp += tempF;
        avg_analog += tmp_analog;
        res_avg += tmp_res;
        avg_volt += tmp_volt;
    }

    avg_temp = avg_temp/50;
    avg_analog = avg_analog/50;
    res_avg = res_avg/50;
    avg_volt = avg_volt/50;


    if ((isnan(avg_temp) || avg_temp <= 0 || avg_temp >= 60) || isinf(avg_temp) || tmp_vcc == 0) {
        floorSensor ? this->floorErrorCount++ : this->envErrorCount++;
        if (this->floorErrorCount >= 5 || envErrorCount >= 5) {
            this->measurementError = true;
        }
        return false;
    } else {
        floorSensor ? this->floorErrorCount = 0 : this->envErrorCount = 0;
        this->measurementError = false;

        float vectorTemp = 0;
        if(floorSensor) {
            if(floorReadings.size() == MAX_VECTOR_SIZE)
                floorReadings.erase(floorReadings.begin());
            floorReadings.push_back(avg_temp);
            
            for(int i = 0; i < floorReadings.size(); i++) {
                // Serial.println(floorReadings[i]);
                vectorTemp += floorReadings[i];
            }

            avg_temp = vectorTemp / floorReadings.size(); 
            avg_temp = avg_temp - (1 + (avg_temp < 19 ? 0.3 : avg_temp < 23 ? 0.6 : 0.8));
            avg_temp = round((vectorTemp / floorReadings.size())*2)/2; 
            if(floorReadings.size() == 1){
                this->floorTemperature = round(avg_temp*2)/2;
            } else {
                if(avg_temp - this->floorTemperature >= 0.5)
                    this->floorTemperature = this->floorTemperature + 0.5;
                else if(avg_temp - this->floorTemperature <= -0.5)
                    this->floorTemperature = this->floorTemperature - 0.5;            
            }

            this->lastFloorTemperature = this->floorTemperature;            
        } else {
            if(environmentReadings.size() == MAX_VECTOR_SIZE)
                environmentReadings.erase(environmentReadings.begin());
            environmentReadings.push_back(avg_temp);

            for(int i = 0; i < environmentReadings.size(); i++) {
                // Serial.print(environmentReadings[i]);
                // Serial.print("  ");
                vectorTemp += environmentReadings[i];
            }

            avg_temp = vectorTemp / environmentReadings.size(); 
            avg_temp = avg_temp - (1 + (avg_temp < 19 ? 0.3 : avg_temp < 23 ? 0.6 : 0.8));
            avg_temp = round(avg_temp*2)/2;
            // avg_temp = avg_temp - avg_temp*0.08; // 0.072
            if(environmentReadings.size() == 1){
                this->environmentTemperature = round(avg_temp*2)/2;
            } else {
                if(avg_temp - this->environmentTemperature >= 0.5)
                    this->environmentTemperature = this->environmentTemperature + 0.5;
                else if(avg_temp - this->environmentTemperature <= -0.5)
                    this->environmentTemperature = this->environmentTemperature - 0.5;   
            }
            
            this->lastEnvironmentTemperature = this->environmentTemperature; 
        }
        printf("\nVCC: %.3f\tTemp: %.2f\n",this->vcc, this->environmentTemperature);
        return true;
    }
}

float NTC::getFloorTemp(){
    return floorTemperature;
}

float NTC::getEnvTemp(){
    return environmentTemperature;
}

bool NTC::hasSensorError(){
    return measurementError;
}

bool NTC::hasFloorSensor(){
    return floorSensor;
}
