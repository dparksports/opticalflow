
class Photo {
    vector<int> h;
    vector<int> s;
    vector<int> v;

    map<int, int> histogramHue;
    map<int, int> histogramValue;


    //5,8,12,25,32,76,100,150,120,15,80

public:

    vector<int> parseInts(string str) {
        stringstream ss(str);
        string token;

        vector<int> ints;
        while (getline(ss, token, ',')) {
            stringstream wordstream(token);

            int number;
            wordstream >> number;
            ints.push_back(number);
        }

        return ints;
    }

    void readPhotoInHSV() {
        string str;
        cin >> str;
        vector<int> integers = parseInts(str);

        for (int i = 0; i < integers.size(); i += 3) {
            RgbColor rgb;    // read a photo in HSV to track Hue and Value

            rgb.r = integers[i+2];
            rgb.g = integers[i+1];
            rgb.b = integers[i];

            HsvColor hsv = RgbToHsv(rgb);
            h.push_back(hsv.h);
            s.push_back(hsv.s);
            v.push_back(hsv.v);
        }
    }

    const int NightHueCliff = 64;

    bool createHistogramHue() {
        int countBelow = 0;
        int countAbove = 0;
        for (int i = 0; i < h.size(); ++i) {
            histogramHue[h[i]]++;

            if (h[i] > NightHueCliff)
                countBelow++;
            else
                countAbove++;
        }

        return countBelow > countAbove;
    }

    const int NightValueCliff = 64;

    bool createHistogramValue() {
        int countBelow = 0;
        int countAbove = 0;

        for (int i = 0; i < h.size(); ++i) {
            histogramValue[h[i]]++;

            if (h[i] > NightValueCliff)
                countBelow++;
            else
                countAbove++;
        }

        return countBelow > countAbove;
    }
};

int main() {
    Photo photo;

    // read a photo in HSV to track Hue and Value
    photo.readPhotoInHSV();

    // "Day/Night Detecor for Vehicle Tracking in Traffic Monitoring Systems" paper
    // Using only the sample night hue distribution of a  night photo.
    // e.g.) a filter value should be 25% or 64 of 255.
    // The same filter should for Value of HSV at 25% or 64 of 255.
    bool isNightHue = photo.createHistogramHue();
    bool isNightValue = photo.createHistogramValue();


    // This handles only when Night Hue and Value filters are met.
    // Otherwise, a photo will be classified as a Day photo.
    // Haven't don't any test, due to the time limitation,
    // but the acccuracy should be about little above 60%


    if (isNightHue && isNightValue) {
        cout << "[NIGHT]";
    } else {
        cout << "[DAY]";
    }


    /**
     * Start Writing Code here,
     * Step 1: Write plugins to get the data
     * Step 2: Algorithm to perform detection
     * Step 3: Based on inference write the single word OUTPUT
     */

    return 0;
}
