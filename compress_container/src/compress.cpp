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
extern "C"{
    #include <turbojpeg.h>
}
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

    const tjhandle handle = tjInitCompress();
    if(handle == NULL){
        const std::string err_msg(tjGetErrorStr());
        _ml::caution("Failed to init JPEG encoder", err_msg);
        std::exit(EXIT_FAILURE);
    }

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
            memcpy(shared_memory + buf_index * FRAME_LENGTH, frame_data, FRAME_LENGTH);
            cv::Mat video_image(1080, 1920, CV_8UC3, frame_data);
            unsigned char *jpeg_frame = NULL;
            unsigned long jpeg_size = 0;
            const int tj_stat = tjCompress2(handle,
                                    video_image.data,
                                    video_image.cols,
                                    video_image.cols*COLOR_CHANNEL_NUM,
                                    video_image.rows,
                                    TJPF_RGB,
                                    &jpeg_frame,
                                    &jpeg_size,
                                    TJSAMP_444,
                                    80,
                                    TJFLAG_FASTDCT
            );
            if(tj_stat == JPEG_FAILED){
            const string err_msg(tjGetErrorStr());
            cout << err_msg << endl;
            }else{
            const string jpeg_str(jpeg_frame, jpeg_frame+jpeg_size);
            //this->send_bufs[id]->push(jpeg_str);
            }
            //cv::imwrite("received_frame.png", received_frame);
            cout << "get frame." << endl;
            buf_index = (buf_index + 1) % 8;
        }
    }


    // 共有メモリをプロセスから切り離す
    shmdt(shared_memory);

    return 0;
}