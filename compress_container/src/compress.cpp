#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace std;

#define FRAME_LENGTH 1920*1080*3

int main(int argc, char *argv[]){
    const key_t key = atoi(argv[1]); // shared key
    const int seg_id = shmget(key, 0, 0);
    if(seg_id == -1){
        cerr << "Failed to acquire segment" << endl;
        return EXIT_FAILURE;
    }

    unsigned char* const shared_memory = reinterpret_cast<unsigned char*>(shmat(seg_id, 0, 0));

    // 共有メモリの文字を読み取る
    int flag = 0;
    char c;
    cout << "If you want to close, please type 'q'" << endl;
    cout << "If you want to read the shared memory, push enter button." << endl;

    unsigned char frame_data[FRAME_LENGTH];
    int buf_index = 0;

    while(flag == 0){
        cin.get(c);
        if(c == 'q') flag = 1;
        if(c == 'g') {
            memcpy(frame_data + buf_index * FRAME_LENGTH, shared_memory, FRAME_LENGTH);
            cv::Mat video_image(1080, 1920, CV_8UC3, frame_data);
            //cv::imwrite("received_frame.png", received_frame);
            cout << "get frame." << endl;
            buf_index = (buf_index + 1) % 8;
             }
    }

    // 共有メモリをプロセスから切り離す
    shmdt(shared_memory);

    return 0;
}