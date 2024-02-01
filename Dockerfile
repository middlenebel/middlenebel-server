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
RUN cp src/lib/* ../../lib

# Compile Middlenebel
WORKDIR /app
RUN make all

# Use alpine server to deliver the application
FROM alpine:3.4

USER 0
RUN mkdir -p /usr/local/middlenebel/plugins
RUN mkdir -p /usr/local/middlenebel/cfgPlugins
RUN mkdir -p /usr/local/middlenebel/scripts/sql
RUN mkdir -p /lib/x86_64-linux-gnu

COPY --from=builder /app/lib/* /usr/lib/
RUN ln -s /usr/lib/libcppkafka.so.* /usr/lib/libcppkafka.so
COPY --from=builder /app/plugins/*.so /usr/local/middlenebel/plugins/
COPY --from=builder /app/cfgPlugins/* /usr/local/middlenebel/plugins/
COPY --from=builder /app/main /usr/local/middlenebel/main
COPY --from=builder /app/*.nebel /usr/local/middlenebel/
COPY --from=builder /app/scripts/*.nebel /usr/local/middlenebel/scripts/
COPY --from=builder /app/scripts/sql/*.sql /usr/local/middlenebel/scripts/sql/

COPY --from=builder /lib/x86_64-linux-gnu/libjsoncpp.so /lib/x86_64-linux-gnu/libjsoncpp.so
COPY --from=builder /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so
COPY --from=builder /usr/lib/x86_64-linux-gnu/librdkafka.so /usr/lib/x86_64-linux-gnu/librdkafka.so
USER $CONTAINER_USER_ID

