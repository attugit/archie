FROM debian:testing

RUN apt-get update
RUN apt-get install -y git python g++-6 clang-3.8
ADD . attugit/archie

