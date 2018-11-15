#include "device.h"

namespace modbus4qt
{

Device::Device(int readTimeout, int writeTimeout, QObject *parent)
    : QObject(parent),
      readTimeout_(readTimeout),
      writeTimeout_(writeTimeout_)
{

}

}

