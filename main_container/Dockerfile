## image install
FROM centos:centos8

# file copy
COPY src/ /home/src/
COPY ffmpeg-2.8.17.tar.gz /home/ffmpeg-2.8.17.tar.gz

WORKDIR /home

##package install
RUN dnf update -y
RUN dnf install -y gcc gcc-c++ make cmake turbojpeg boost boost-devel wget zip unzip 
RUN dnf --enablerepo=powertools install -y turbojpeg-devel
RUN dnf install -y 'dnf-command(config-manager)'
RUN dnf config-manager --set-enabled powertools
RUN dnf install -y https://download.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
RUN dnf localinstall -y --nogpgcheck https://download1.rpmfusion.org/free/el/rpmfusion-free-release-8.noarch.rpm
RUN dnf install -y ffmpeg ffmpeg-devel opencv-devel yasm


##FFmpeg install
RUN tar xvzf ffmpeg-2.8.17.tar.gz
WORKDIR /home/ffmpeg-2.8.17
RUN ./configure --enable-gpl --enable-shared --prefix=/usr --libdir=/usr/lib64 --pkgconfigdir=/usr/lib64/pkgconfig --incdir=/usr/include
RUN make
RUN make install

WORKDIR /home

##openCV install
RUN wget https://github.com/opencv/opencv/archive/3.4.5.zip
RUN unzip 3.4.5.zip -d .
RUN mkdir ./opencv-3.4.5/build
WORKDIR /home/opencv-3.4.5/build
RUN cmake -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D WITH_FFMPEG=OFF -D WITH_GSTREAMER=ON -D WITH_TBB=ON ..
RUN make -j "$(nproc)"
RUN make install

##Gstreamer install
RUN dnf install -y PackageKit-gstreamer-plugin gstreamer1 gstreamer1-devel gstreamer1-libav gstreamer1-plugins-bad-free gstreamer1-plugins-bad-free-devel \
gstreamer1-plugins-bad-freeworld gstreamer1-plugins-base gstreamer1-plugins-base-devel gstreamer1-plugins-good gstreamer1-plugins-good-gtk \
gstreamer1-plugins-ugly gstreamer1-plugins-ugly-free gstreamer1-svt-av1 gstreamer1-svt-hevc gstreamer1-svt-vp9 libnice-gstreamer1

##source build
WORKDIR /home/src
RUN g++ main.cpp -lopencv_core -lopencv_imgproc -lopencv_videoio -fpermissive -o frame_store
