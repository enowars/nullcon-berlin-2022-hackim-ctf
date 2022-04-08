FROM ubuntu:20.04

RUN apt update && apt -y upgrade
RUN apt install -y socat
RUN apt install -y gcc libgmp-dev

RUN mkdir /service
WORKDIR /service
RUN mkdir data

COPY sign_plz.c run.sh ./
RUN gcc sign_plz.c -lgmp -o sign_plz

RUN groupadd -g 2000 cryptogroup \
&& useradd -m -u 2001 -g cryptogroup cryptodude

RUN chown -R cryptodude /service
RUN chmod +x sign_plz run.sh
USER cryptodude

CMD ["/service/run.sh"]
