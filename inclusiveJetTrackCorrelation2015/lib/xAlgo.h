
namespace xAlgo {

	int binarySearch(float key, float* arr, int i_max, int i_min ){
		assert(key< arr[i_max] ||!"overflow!");
		assert(key> arr[i_min] ||!"underflow!");
		int mid = floor(float(i_max +i_min)/2);
		//	cout<<mid<<endl;
		if(mid == i_min ) return mid;
		if( arr[mid]> key) return binarySearch(key, arr, mid, i_min);
		else if( arr[mid] < key) return binarySearch(key, arr, i_max, mid);
		else return mid;
	}
	int binarySearch(float key, std::vector<float> & arr, int i_max, int i_min ){
		if(key<arr.at(i_min)) return -1;
		if(key>arr.at(i_max)) return -1;
		int mid = floor(float(i_max +i_min)/2);
		//	cout<<mid<<endl;
		if(mid == i_min ) return mid;
		if( arr[mid]> key) return binarySearch(key, arr, mid, i_min);
		else if( arr[mid] < key) return binarySearch(key, arr, i_max, mid);
		else return mid;
	}
}

