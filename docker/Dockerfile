From centos:7

Run yum install -y wget \
    && wget "https://github.com/bazelbuild/bazel/releases/download/0.17.2/bazel-0.17.2-installer-linux-x86_64.sh" \
    && chmod a+x bazel-0.17.2-installer-linux-x86_64.sh \
    && yum install -y unzip zip \
    && yum -y install sudo vim java \
    && ./bazel-0.17.2-installer-linux-x86_64.sh
