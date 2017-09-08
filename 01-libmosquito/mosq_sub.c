#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mosquitto.h>

/* Server connection parameters */
#define MQTT_HOSTNAME "localhost"
#define MQTT_PORT 1883
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "admin"
#define MQTT_TOPIC "home/admin"

static int run = 1;

void handle_signal(int s)
{
    run = 0;
}

void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
    printf("connect: result code = %d\n", result);
}

void message_callback(struct mosquitto *mosq, void *obj,
        const struct mosquitto_message *message)
{
    bool match = 0;
    printf("got message '%.*s' for topic '%s'\n", message->payloadlen,
            (char *) message->payload, message->topic);

    mosquitto_topic_matches_sub(MQTT_TOPIC, message->topic, &match);
    if (match)
        printf("Got message for matching topic\n");
}

int main(int argc, char **argv)
{
    struct  mosquitto *mosq = NULL;
    char    clientid[24];
    int     rc = 0;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    /* Initialize the Mosquitto library */
    mosquitto_lib_init();

    snprintf(clientid, sizeof(clientid), "mosq_sub_%d", getpid());

    /*
     * Create a new Mosquitto runtime instance with a random client ID,
     * and no application-specific callback data.
     */
    mosq = mosquitto_new(clientid, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Can't initialize Mosquitto library\n");
        exit(EXIT_FAILURE);
    }

    mosquitto_connect_callback_set(mosq, connect_callback);
    mosquitto_message_callback_set(mosq, message_callback);

    /* mosquitto_username_pw_set(mosq, MQTT_USERNAME, MQTT_PASSWORD); */

    /* Establish a connection to the MQTT server. Do not use a keep-alive ping */
    int ret = mosquitto_connect(mosq, MQTT_HOSTNAME, MQTT_PORT, 60);
    if (ret) {
        fprintf(stderr, "Can't connect to mosquitto server\n");
        exit(EXIT_FAILURE);
    }

    mosquitto_subscribe(mosq, NULL, MQTT_TOPIC, 0);

    while (run) {
        rc = mosquitto_loop(mosq, -1, 1);
        if (run && rc) {
            fprintf(stderr, "connection error\n");
            sleep(10);
            mosquitto_reconnect(mosq);
        }
    }

    /* Tidy up */
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return rc;
}

