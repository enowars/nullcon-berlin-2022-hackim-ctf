FROM ubuntu:latest
WORKDIR /code
RUN apt update
RUN apt install -y socat
ENTRYPOINT socat TCP-LISTEN:1337,reuseaddr,fork EXEC:"./target/release/magic_dragon"