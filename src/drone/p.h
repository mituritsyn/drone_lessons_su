class P{
public:
    float kp = 0;
    P(float kp) : kp(kp) {};
    float update(float error, float dt) {
        return kp * error;
    };
private:
    float prevError = NAN;
};
