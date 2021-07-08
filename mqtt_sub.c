#include <stdio.h>
#include <stdlib.h>

#include <mosquitto.h>

void on_connect(struct mosquitto *mosq, void *obj, int rc) //this fcn is called after the clients con to the broker
{
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "dev/temp", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) //it is called when the message arrives from the broker
{
	printf("%s: %s\n", msg->topic, (char *) msg->payload);
}

int main() {
	int rc, id=12;

	mosquitto_lib_init();

	struct mosquitto *mosq;
	mosq = mosquitto_new("Data_reader", true, &id);
	
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	
	rc = mosquitto_connect(mosq, "192.168.1.153", 1883, 10);
	if(rc) {
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}

	mosquitto_loop_start(mosq);
	printf("Press Enter to quit...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}