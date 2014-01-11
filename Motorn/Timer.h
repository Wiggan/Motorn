#pragma once
class Timer {
private:
    static Timer*   mInstance;
    double          mElapsed;
                    Timer(Timer const&);
    Timer&          operator=(Timer const&);
                    Timer();
                    ~Timer();
public:
    static Timer& getInstance();
    double getElapsed();
    void update(const double delta);
};

