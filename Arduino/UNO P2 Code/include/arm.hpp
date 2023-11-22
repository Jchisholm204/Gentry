#pragma once

class fsr
{
public:
    fsr(int port, double limit = 90);

    /**
     * @brief Get the Reading of the FSR
     * 
     * @returns current analog input reading
     */
    double getReading();

    /**
     * @brief Get if the FSR has exeeded the limit
     * 
     * @return true if limit is exeeded
     * @return false if limit is not exeeded
     */
    bool getState();

    /**
     * @brief Set the FSR reading limit
     * 
     * @param newLimit The new limit
     */
    void setLimit(double newLimit);

    private:
        int dport;
        double dlim;
};
