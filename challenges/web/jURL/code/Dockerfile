FROM openjdk:11
RUN apt-get update
RUN apt-get install -y --no-install-recommends socat
WORKDIR /usr/src/app
COPY ./code/ /usr/src/app
RUN javac Main.java
CMD ["./run.sh"]