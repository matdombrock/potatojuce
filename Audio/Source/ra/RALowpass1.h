#pragma once
class RALowpass1
{
public:
    RALowpass1() {};
    ~RALowpass1() {};
    void prepareToPlay(int newSampleRate) {
        sampleRate = newSampleRate;
        prepared = true;
    }
    double process(double sample, double cut)
    {
        if (!prepared) {
            return sample;
        }
        int nyquist = sampleRate * 0.5;

        double filtered = sample;
        double f = cut / nyquist;

        if (!firstPass) {
            filtered = lastFiltered + f * (sample - lastFiltered);
        }
        else
        {
            firstPass = false;
        }

        lastFiltered = filtered;

        return filtered;
    }
private:
    bool prepared = false;
    bool firstPass = true;
    double lastFiltered = 0;
    int sampleRate = 0;
};
