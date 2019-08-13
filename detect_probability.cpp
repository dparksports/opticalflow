#include <iostream>
#include <vector>

using namespace std;

class Timestamp {
public:
    int timestamp;
    float probability;
};

const int NoneTime = -1;

class VideoRange {
public:
    int begintime;
    int endtime; // exclusive ending
};

vector<VideoRange> video_ranges(vector<Timestamp> datapoints, float threshold) {
    vector<VideoRange> found;
    int beginTimestamp = 0;
    int endTimestamp = 0;

    for (int i = 0; i < datapoints.size(); i++) {
        if (datapoints[i].probability > threshold) {

            if (beginTimestamp == 0) {
                beginTimestamp = datapoints[i].timestamp;
            } else {
                if (i + 1 > datapoints.size()) {
                    endTimestamp = NoneTime;
                } else {
                    endTimestamp = datapoints[i+1].timestamp;
                }
            }
        }



        if (beginTimestamp != 0) {
            if (endTimestamp == 0) {
                VideoRange newRange;
                newRange.begintime = beginTimestamp;
                found.push_back(newRange);

                newRange.endtime = NoneTime;
            } else {
                VideoRange newRange;
                newRange.begintime = beginTimestamp;
                found.push_back(newRange);

                newRange.endtime = endTimestamp;
            }
        }
    }

    return found;
}



// datapoints = [(10, 0.1), (13, 0.2), (19, 0.9), (20, 0.8), (30, 0.1)]
// threshold = 0.8

// datapoints = [(10, 0.1), (13, 0.2), (19, 0.9), (20, 0.1), (30, 0.8)]
// # [(19, 20), (30, None)]


// datapoints = [(10, 0.1), (13, 0.2), (19, 0.7), (20, 0.1), (30, 0.7)]
// # []

//    int timestamp;
//    float probability;


Timestamp insertTimestamp(int timestamp, float probability){
    Timestamp stamp;
    stamp.timestamp = timestamp;
    stamp.probability = probability;
    return stamp;
}

//   int begintime;
//   int endtime; // exclusive ending

int main(){
    vector<Timestamp> datapoints;
    datapoints.push_back(insertTimestamp(10,0.1));
    datapoints.push_back(insertTimestamp(13,0.2));
    datapoints.push_back(insertTimestamp(19,0.9));
    datapoints.push_back(insertTimestamp(20,0.8));
    datapoints.push_back(insertTimestamp(30,0.1));

    float threshold = 0.8;
    vector<VideoRange> range = video_ranges(datapoints, threshold);

    for (int i = 0; i < range.size(); i++) {
        cout << "(" << range[i].begintime << "," << range[i].begintime << ") ";
    }

    cout << endl;

    return 0;
}
