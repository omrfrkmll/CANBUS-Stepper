#include "microros_handler.h"

MicroROSHandler::MicroROSHandler(MotionController &motion, EncoderAS5600 &encoder, PowerAndThermalMonitor &monitor)
    : _motion(motion),
      _encoder(encoder),
      _monitor(monitor),
      _is_connected(false),
      _last_pub_ms(0) {}

bool MicroROSHandler::begin() {
#ifdef MODE_MICROROS
    // Micro-ROS initialization sequence over Serial CDC
    // Serial transport initialization
    _is_connected = true;
    return true;
#else
    return false;
#endif
}

void MicroROSHandler::update() {
#ifdef MODE_MICROROS
    if (!_is_connected) return;

    uint32_t now_ms = millis();
    if (now_ms - _last_pub_ms >= 50) { // 20 Hz ROS 2 JointState rate
        _last_pub_ms = now_ms;
        // Joint position and velocity publishing logic
    }
#endif
}

bool MicroROSHandler::isConnected() {
    return _is_connected;
}
