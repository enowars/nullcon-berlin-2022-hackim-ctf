FROM python:3.9-buster

RUN apt-get update && apt-get install -y gcc-multilib wget
RUN groupadd web
RUN useradd web --gid web

COPY . /web1.0
WORKDIR /web1.0

# Pack it up for maximum open-sourceness
RUN chmod +x ./pack.sh && ./pack.sh

# building web1.0
RUN make clean
# RUN make tests
RUN make

#USER web
RUN chmod +x "./start.sh"

# web
EXPOSE 9001

ENTRYPOINT ["./start.sh"]
