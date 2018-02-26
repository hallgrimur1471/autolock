class Car {
  public:
    Car(void);
    void lock();
    void unlock();
    bool isLocked();
    int32_t get_time_since_toggle();
    
  private:
    int32_t _last_toggle_time;
    bool _locked;
};