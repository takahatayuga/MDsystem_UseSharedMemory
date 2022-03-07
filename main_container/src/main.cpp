/**************************************
*              main.cpp               *
*  (main function for the head node)  *
**************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <chrono>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/circular_buffer.hpp>
using namespace std;

#define FRAME_LENGTH 1920*1080*3

int main(){
      // 空のファイル作成
    FILE *fp;
    const string file_path = "./key.dat";
    fp = fopen(file_path.c_str(), "w");
    fclose(fp);

    // IPC keyの取得
    const int id = 50;
    const key_t key = ftok(file_path.c_str(), id);
    std::cout << "key = " << key << endl;
    if(key == -1){
        cerr << "Failed to acquire key" << endl;
        return EXIT_FAILURE;
    }

    // 共有メモリIDの取得 (50MB)
    const long long size = 0x5000000;
    const long long seg_id = shmget(key, size,
                              IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | 0x1b6);
    // 共有メモリIDの取得 (9MB)
    if(seg_id == -1){
        cerr << "Failed to acquire segment" << endl;
        return EXIT_FAILURE;
    }

    unsigned char* const shared_memory = reinterpret_cast<unsigned char*>(shmat(seg_id, 0, 0));

    // 画像フレームの取得
    cv::VideoCapture video;
    string src = "/home/main_container/src/video.mp4";
    video = cv::VideoCapture(src.c_str());
    if(!video.isOpened()){
        std::cout << "Failed to open video " << src.c_str() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    cv::Mat video_frame;

    int buf_index = 0;
    int count = 0;
    while(count < 100){
        video >> video_frame;
        memcpy(shared_memory + buf_index * FRAME_LENGTH, video_frame.data, FRAME_LENGTH);
        buf_index = (buf_index + 1) % 8;
        count++;
    }

    //qキーが押されるまでループ
    string s;
    int flag = 0;
    while(flag == 0){
        cout << "word: ";
        cin >> s;
        if(s == "q") flag = 1;
    }

    // 共有メモリをプロセスから切り離す
    shmdt(shared_memory);

    // 共有メモリを解放する
    shmctl(seg_id, IPC_RMID, NULL);

    return 0;
}