FROM php:apache
RUN apt-get update && apt-get install -y texlive-latex-base

RUN mkdir /tmp/pdf /tmp/compile
RUN chown -R www-data:www-data /tmp/pdf /tmp/compile