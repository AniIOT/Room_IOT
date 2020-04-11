//#include "MQTT_PubSub.h"
enum teMQTTstatus
{
  eMQTTError,
  eMQTTInProgress,
  eMQTTSuccess,
};

teMQTTstatus teMQTTStatus;

teMQTTStatus MQTTStateMachine()
{
  static teMQTTstatus teStatus = eMQTTInProgress;
  return teStatus;
}
