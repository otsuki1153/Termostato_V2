

class TERMOCONTROLL{
    private:

    //=========================================== Variáveis para a temperatura e Relay =========================================================
    float displayTemp = 16.0;
    //-----------------------------------------------------------------------------------------------------------------------------------

    //=================================================== Variáveis para SetPoint ======================================================
    float setPointValue = 0;
    //-----------------------------------------------------------------------------------------------------------------------------------

    //=================================================== Variáveis para Controle do Display ======================================================
    bool power = true;
    //-----------------------------------------------------------------------------------------------------------------------------------


    public:

    /// @brief Função que retorna em float a temperatura do NTC calculada do dispositivo
    /// @return float displayTemp
    float getTemp();

    /// @brief Função que atualiza o valor da temperatura quando colocado um valor float com o valor calculado do NTC do dispositivo
    /// @param temp float 
    void setTemp(float temp);

    /// @brief Função que vai retornar o valor atual do setpoint
    /// @return float setPointValue
    float getSetPoint();

    /// @brief Função que atualiza o valor do setpoint do dispositivo
    /// @param setPoint float 
    void setSetPoint(float setPoint);

    /// @brief Função que retorna o estado de power do dispositivo
    /// @return power (ON/OFF)
    bool getPowerTermo();

    /// @brief Função que atualiza o estado de power do dispositivo
    /// @param power bool
    void setPowerTermo(bool power);

}; extern TERMOCONTROLL termo;