FROM debian:stable-slim
RUN apt-get update -y
RUN apt-get install -y --no-install-recommends socat git ca-certificates curl binutils
RUN useradd -m -s /bin/bash cloner
RUN git clone https://github.com/internetwache/GitTools.git /opt/GitTools
RUN mkdir /app
WORKDIR /app
COPY ./code/ /app
RUN chmod ug+x ./run.sh ./task.sh
RUN chown root:root ./run.sh ./task.sh
CMD ["./run.sh"]