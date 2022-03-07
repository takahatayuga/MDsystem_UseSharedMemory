# MDsystem_UseSharedMemory

This is a middleware for virtualizing a multi-display system running on a single board computer and running it on a Docker container.


## Environment in use

### Library
(Host PC)
- Docker

(Docker Container)
- OpenCV
- Gstreamer
- FFmpeg
- TurboJPEG

### OS
- CentOS7 (Container uses Centos8)


## Set Up
1. Install the Docker.
2. Clone this repository, and move into the directory.
3. Follow the instructions below to build the container image．

### Head Container

1. Go to main_container directory
2. Build the Docker image.
`docker build --network host --no-cache -t <image name> .`
4. Start the container.
`docker run -it --net=host --ipc=host --privileged --name=main_container <image name> /bin/bash`

### Compress container

1. Go to compress_container directory
2. Build the Docker image.
`docker build --network host --no-cache -t <image name> .`
4. Start the container.
`docker run -it --net=host --ipc=container:<ID of main_container> --privileged --name=compressxxx <image_name> /bin/bash`

### How to run

1. On the Head Container,
- Build the Program.
`g++ -g3 main.cpp -lopencv_core -lopencv_imgproc -lopencv_videoio -fpermissive -o main`
- Run `./main`. 
- Obtain shared memory ID (Shared memory ID is output to the terminal)

2. On the Display Nodes,
- Build the Program.
`g++ -g3 encoder.cpp -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -lturbojpeg -fpermissive -o encoder`
- Run `./encoder <shared memory ID>`. 


### To Do List
1. Design of synchronous containers to control frame passing
2. Implementation of frame passing process to display node