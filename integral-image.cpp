#include <iostream>
#include <map>
#include <vector>
using namespace std;

#define ROI_UNDEFINED -1

struct ROI {
  int top;
  int left;
  int width;
  int height;
}

class LOT{
  private:
    vector<T> imageArray;
    
  public:
    map<ROI,int> sumROILookup;
   
    LOT(vector<T> image) {
      imageArray = image;
    }
 
    int _calculateHash(ROI region) {
      
    }   
    
    int calculateHash(ROI region) {
      int sum = map[ROI];
      
      if (sum == ROI_UNDEFINED) {
        sum = _calculateHash(region);
        map[ROI] = sum;
      } else {
        return sum;
      } 
    }
    
    
}

int calculateROISum(vector<T> image, ROI section) {
  
}

int main() {

  for (int i = 0; i < 100; i++) {
    
  }  


	cout<<"Hello";
	return 0;
}
