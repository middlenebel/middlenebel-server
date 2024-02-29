# Build the app in image ‘builder’ (multi-stage builds)
FROM debian as builder

RUN apt-get update && apt-get install -y \
    make cmake librdkafka-dev libssl-dev libjsoncpp-dev libmysqlcppconn-dev \
    python3-launchpadlib clang clang-tools libz-dev g++ curl procps

# Define working directory
WORKDIR /app

# Duplicate all necessary files
COPY . .

# Compile cppkafka
WORKDIR /app/third_party/cppkafka/build
RUN rm -rf * || true
RUN cmake /app/third_party/cppkafka
RUN make
RUN cp lib/libcppkafka.so.* /usr/lib
RUN ln -s /usr/lib/libcppkafka.so.* /usr/lib/libcppkafka.so

# Compile cpp-httplib
WORKDIR /app/third_party/cpp-httplib/build
RUN rm -rf * || true
RUN cmake -DCMAKE_BUILD_TYPE=Release /app/third_party/cpp-httplib
RUN cmake --build /app/third_party/cpp-httplib/build --target install

# Compile Middlenebel
WORKDIR /app/src
RUN make all
RUN make buildPlugins 

FROM debian

USER 0
RUN mkdir -p /usr/local/middlenebel/plugins
RUN mkdir -p /usr/local/middlenebel/cfgPlugins
RUN mkdir -p /usr/local/middlenebel/outyamls
RUN mkdir -p /usr/local/middlenebel/scripts/sql
RUN mkdir -p /lib/x86_64-linux-gnu

COPY --from=builder /usr/lib/libcppkafka.so.* /usr/lib/
RUN ln -s /usr/lib/libcppkafka.so.* /usr/lib/libcppkafka.so
COPY --from=builder /app/plugins/*.so /usr/local/middlenebel/plugins/
COPY --from=builder /app/cfgPlugins/* /usr/local/middlenebel/cfgPlugins/
COPY --from=builder /app/src/nebel /usr/local/middlenebel/nebel
COPY --from=builder /app/runnebel.sh /usr/local/middlenebel/runnebel.sh
COPY --from=builder /app/*.nebel /usr/local/middlenebel/
# TODO remove scripts
COPY --from=builder /app/scripts/*.nebel /usr/local/middlenebel/scripts/
COPY --from=builder /app/scripts/sql/*.sql /usr/local/middlenebel/scripts/sql/

COPY --from=builder /usr/lib/x86_64-linux-gnu/* /usr/lib/x86_64-linux-gnu/
COPY --from=builder /usr/lib/x86_64-linux-gnu/* /usr/lib/x86_64-linux-gnu/

RUN chmod 777 /usr/local/middlenebel/
RUN chmod 777 /usr/local/middlenebel/plugins/*
RUN chmod 777 /usr/local/middlenebel/*
RUN chmod 777 /usr/local/middlenebel/scripts
RUN chmod 777 /usr/local/middlenebel/scripts/*.nebel
RUN chmod 777 /usr/local/middlenebel/scripts/sql
RUN chmod 777 /usr/local/middlenebel/scripts/sql/*.sql

#USER $CONTAINER_USER_ID

#ENTRYPOINT ["/usr/local/middlenebel/runnebel.sh"]
CMD ["/usr/local/middlenebel/runnebel.sh"]

# OK: docker run -p 8080:8080 --entrypoint /usr/local/middlenebel/runnebel.sh nebel-docker
#ENTRYPOINT ["/bin/bash"]

