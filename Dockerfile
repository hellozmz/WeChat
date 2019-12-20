FROM centos:7

RUN yum install -y wget \
    && wget "https://github.com/bazelbuild/bazel/releases/download/0.17.2/bazel-0.17.2-installer-linux-x86_64.sh" \
    && chmod a+x bazel-0.17.2-installer-linux-x86_64.sh \
    && yum install -y unzip zip \
    && yum -y install sudo vim java \
    && ./bazel-0.17.2-installer-linux-x86_64.sh \
    && yum install -y gcc gcc-c++

RUN mkdir /WeChat /WeChat/logs
COPY ./src/ /WeChat/src/
COPY ./config/ /WeChat/config/
COPY ./third_party/ /WeChat/third_party/
COPY ./util/ /WeChat/util/
COPY ./others_work/ /WeChat/others_work/
COPY ./README.md/ /WeChat
COPY ./WORKSPACE/ /WeChat
