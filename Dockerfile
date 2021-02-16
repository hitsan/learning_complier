FROM ubuntu:20.04

RUN apt-get update -y && \
    apt-get install -y tzdata && \
    apt-get install -y g++ build-essential git make autoconf automake gcc make libtool
WORKDIR /home
RUN git clone git://github.com/cpputest/cpputest.git
WORKDIR /home/cpputest/cpputest_build
RUN autoreconf .. -i && ../configure && make && make tdd
ENV CPPUTEST_HOME=/home/cpputest 
RUN make install
WORKDIR /home