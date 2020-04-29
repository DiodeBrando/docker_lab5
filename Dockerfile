FROM centos
RUN yum update -y
RUN yum upgrade -y
RUN yum install -y gcc-c++ 
COPY oneforall.cpp /home/oneforall.cpp
