#include <iostream>

using namespace std;

namespace MySpace
{
	float* myarrayinit(int size)
	{
		srand(time(NULL));
		float* farray = new float[size];
		if (farray != nullptr)
		{
			for (size_t i = 0; i < size; i++)
			{
				farray[i] = (float)rand() / (float)RAND_MAX * (1000.0 + 1000.0) - 1000.0;  //(double)rand()/(double) RAND_MAX*(b-a)+a
			}
		}
		return farray;

	}

	void myarray(float* arr, int size)
	{
		for (size_t i = 0; i < size; i++)
		{
			cout << arr[i] << " ";
		}
		cout << endl;

	}
}