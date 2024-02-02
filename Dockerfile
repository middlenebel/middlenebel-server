# Build the app in image ‘builder’ (multi-stage builds)
FROM debian as builder

RUN apt-get update && apt-get install -y \
    make cmake librdkafka-dev libssl-dev libjsoncpp-dev libmysqlcppconn-dev \
    python3-launchpadlib clang clang-tools libz-dev g++

# Define working directory
WORKDIR /app

# Duplicate all necessary files
COPY . .

RUN rm -rf ./cppkafka/build || true

WORKDIR /app/cppkafka/build

RUN cmake /app/cppkafka
RUN make

# Compile Middlenebel
WORKDIR /app
RUN make all

# Use alpine server to deliver the application
# Not good for dinamic libraries FROM alpine:3.4
#FROM scratch
# FROM debian:buster
#FROM ubuntu:22.04
FROM debian

USER 0
RUN mkdir -p /usr/local/middlenebel/plugins
RUN mkdir -p /usr/local/middlenebel/cfgPlugins
RUN mkdir -p /usr/local/middlenebel/scripts/sql
RUN mkdir -p /lib/x86_64-linux-gnu

COPY --from=builder /app/cppkafka/build/src/lib/* /usr/lib/
COPY --from=builder /app/plugins/*.so /usr/local/middlenebel/plugins/
COPY --from=builder /app/cfgPlugins/* /usr/local/middlenebel/plugins/
COPY --from=builder /app/main /usr/local/middlenebel/main
COPY --from=builder /app/*.nebel /usr/local/middlenebel/
COPY --from=builder /app/scripts/*.nebel /usr/local/middlenebel/scripts/
COPY --from=builder /app/scripts/sql/*.sql /usr/local/middlenebel/scripts/sql/

#COPY --from=builder /lib/x86_64-linux-gnu/libjsoncpp.* /lib/x86_64-linux-gnu/
# COPY --from=builder /usr/lib/x86_64-linux-gnu/libmysqlcppconn.* /usr/lib/x86_64-linux-gnu/
# COPY --from=builder /usr/lib/x86_64-linux-gnu/librdkafka.* /usr/lib/x86_64-linux-gnu/

#COPY --from=builder /lib/x86_64-linux-gnu/* /lib/x86_64-linux-gnu/
COPY --from=builder /usr/lib/x86_64-linux-gnu/* /usr/lib/x86_64-linux-gnu/
COPY --from=builder /usr/lib/x86_64-linux-gnu/* /usr/lib/x86_64-linux-gnu/

RUN chmod 775 /usr/local/middlenebel
RUN chmod 775 /usr/local/middlenebel/plugins/*
RUN chmod 775 /usr/local/middlenebel/*
RUN chmod 775 /usr/local/middlenebel/scripts
RUN chmod 776 /usr/local/middlenebel/scripts/*.nebel
RUN chmod 775 /usr/local/middlenebel/scripts/sql
RUN chmod 776 /usr/local/middlenebel/scripts/sql/*.sql

#RUN strip --remove-section=.note.ABI-tag /lib/x86_64-linux-gnu/libjsoncpp.so
#RUN strip --remove-section=.note.ABI-tag /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so
#RUN strip --remove-section=.note.ABI-tag /usr/lib/x86_64-linux-gnu/librdkafka.so

USER $CONTAINER_USER_ID

#ENTRYPOINT ["/usr/local/middlenebel/main"]
#RUN /usr/local/middlenebel/main


