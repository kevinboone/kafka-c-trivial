# kafka-c-trivial

Just about the simplest possible application of the rdkafka C library -- send a message to a Kafka broker. 

## Prerequisites

`librdkafka` development headers and library must be installed.

## Build

Edit `main.c` to indicate -- as a minimum -- the Kafka bootstrap server
URI and the topic to send to. Then compile `main.c`, linking with
`librdkafka`. See `Makefile` for a trivial way to do this.

## Run

Ensure that Kafka is running.

    $ ./kafka-c-trivial

Use, for example, `kafka-console-consumer.sh` to check that the messages
have been sent.

## More information

For more information, see:

https://kevinboone.me/kafka-c-trivial.html

