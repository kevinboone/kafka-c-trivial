
/*
 * kafka-c-trivial: a very simple demonstration of the rdkafka library,
 *   which sends a few messages to a Kafka message broker. This example
 *   does not demonstrate authentication or encryption, or any more advanced
 *   features of rdkafka.
 *
 * rdkafka API documentation is here:
 * https://docs.confluent.io/platform/current/clients/librdkafka/html/index.html
 *
 * Source code for the library is here:
 * https://github.com/edenhill/librdkafka
 *
 * Copyright (c)2022 Kevin Boone, GPL v3.0
 */

#include <stdio.h>
#include <string.h>
#include <librdkafka/rdkafka.h>

// Define the URI of the Kafka bootstrap server(s)
#define BOOTSTRAP_SERVERS ":9092"

// Define the name of the topic to which we will send messages
#define TOPIC "my-topic"

/** queue_for_send is wrapper for the rd_kafka_producev() function, that
    queues a single message with a specified (string) key and value for
    sending to the broker. This function does not block -- it will likely
    complete before the message has been sent. We will later use
    rdkafka_flush() to wait for completion of the send. */ 

void queue_for_send (const char *argv0, rd_kafka_t *producer, 
    const char *topic, const char *key, const char *value)
  {
  rd_kafka_resp_err_t err = rd_kafka_producev (producer,
    RD_KAFKA_V_TOPIC (topic),
    RD_KAFKA_V_MSGFLAGS (RD_KAFKA_MSG_F_COPY),
    RD_KAFKA_V_KEY ((void*)key, strlen (key)),
    RD_KAFKA_V_VALUE ((void*)value, strlen (value)),
    RD_KAFKA_V_OPAQUE (NULL),
    RD_KAFKA_V_END);
  if (err)
    {
    fprintf (stderr, "%s: can't send: %s\n", argv0, rd_kafka_err2str (err));
    }
  }

/* 
 * Start here
 */
int main (int argc, char **argv)
  {
  (void)argv; // Silence compiler warning about unused argc and argv
  (void)argc;

  char err[256]; // A buffer for error messages

  // Allocate a new rd_kafka_conf_f object, to store configuration 
  //   values.
  rd_kafka_conf_t *conf = rd_kafka_conf_new();

  /* Some librdkafka functions will log to sterr in addition to any error
     codes they might return. We could install a different log handler using
     rd_kafka_conf_set_log_cb() if required. However, because of the
     built-in logging, it's not always necesssary to handle error messages
     returned by the API functions in this simple application.  */

  rd_kafka_conf_set (conf, "bootstrap.servers", BOOTSTRAP_SERVERS, 
        err, sizeof (err));

  /* Ideally, we should read the configuration settings from a file, 
       allowing the user to set values that the program leaves as defaults. */

  /* rd_kafka_new can create a producer or a consumer. He we create a
       producer. */
  rd_kafka_t *producer = rd_kafka_new (RD_KAFKA_PRODUCER, conf, 
    err, sizeof (err));
  if (producer)
    {
    // Send a couple of messages
    queue_for_send (argv[0], producer, TOPIC, "mykey", "Hello");
    queue_for_send (argv[0], producer, TOPIC, "mykey", "World");

    rd_kafka_flush (producer, 10000); // Wait for 10 seconds for all the
                                      //  messages to be sent

    // Let's see if there's anything left in the producer queue after
    //   10 seconds. In this simple application, we can't do anything
    //   except to report the problem.
    int unsent = rd_kafka_outq_len (producer);
    if (unsent > 0) 
      {
      fprintf (stderr, "%s: %d message(s) were not sent\n", argv[0], unsent); 
      }
  
    // Clean up.
    // We do not have to destroy the conf object, as rd_kafka_new will have
    //   transferred it to the 'ownership' of the producer object. In fact,
    //   destroying it will probably cause a crash.
    rd_kafka_destroy (producer);
    }
  else
    {
    // No producer was allocated -- perhaps out of memory?
    // This should rarely happen, anyway.
    fprintf (stderr, "%s: can't create producer: %s\n", argv[0], err); 
    }
  }

