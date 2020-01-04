#include <iostream> // For cin/out
#include <fstream> // For file writing
#include <assert.h> // For assertions
#include <iomanip> // For print precision
#include <math.h> // For fabs comparison
#include <chrono> // For time measurement
#include <string>

using namespace std;
using namespace std::chrono;

// Saves all results to a file
void printResult(int degree, double position, long double result, double timeHorner, double timeSimply);

// Returns time in ns. in human-readable units i.e. 1ms, 10sec, 12h
string getReadableTime(double timeNs);

// Polynomial evaluation via horner's algorithm
long double horner(double factors[], double position, int degree) {
	long double value = factors[0];
	
	for (int i = 1; i <= degree; i++) {
		value = factors[i] + (value * position);
	}
		
	return value;
}

long double power(double x, double n) {
	if (n == 0) return 1L;
	long double value = x;
	while(n > 1) {
		value = value * x;
		n--;
	}
	return value;
}

// Polynomial evaluation via classical approach
long double simply(double factors[], double position, int degree) {
	long double value = factors[degree];
	double exp = 0.0;
	
	for (int i = degree - 1; i >= 0; i--) {
		exp++;
		value = (factors[i] * power(position, exp)) + value;
	}
	
	return value;
}

double getRandomDouble() {
	return ((double) rand() / RAND_MAX) * (1.0 - 0.1) + 0.1;	
}

void fulfillWithRandoms(double array[], int length) {
	for (int i = 0; i < length; i++) {
		array[i] = getRandomDouble();
	}
}

void printArray(double array[], int length) {
	cout<< "---" << endl;
	cout<< "[ ";
	for (int i = 0; i < length; i++) {
		cout<< array[i]<<", ";
	}
	cout<< " ]";
	cout<<endl<< "---" <<endl;
}

// Assures correct evaluation and randoms difference
void test() {
	// For the polynomial 2x^2 + x + 1
	double poly[3] = {2.0, 1.0, 1.0};
	
	assert(horner(poly, 1.0, 2) == 4.0);
	assert(simply(poly, 1.0, 2) == 4.0);
	
  // In some cases comparing with Epsilon
	assert(fabs(horner(poly, 0.1, 2) - 1.12) < 1.12 * 0.000001);
	assert(fabs(simply(poly, 0.1, 2) - 1.12) < 1.12 * 0.000001);
	
	// For the polynomial 3x^3 + 2x^2 - x + 1
	double poly2[4] = {3.0, 2.0, -1.0, 1.0};
	
	assert(horner(poly2, 0.5, 3) == 1.375);
	assert(simply(poly2, 0.5, 3) == 1.375);
	
	double rand1 = getRandomDouble();
	double rand2 = getRandomDouble();
	
	assert(rand1 != rand2);
}

void executeWithTimeMeasuring(double factors[], double position, int degree) {
	
	auto startHorner = high_resolution_clock::now();
	long double hornerValue = horner(factors, position, degree);
	auto endHorner = high_resolution_clock::now();
	
	cout<< "Horner's algorithm finished" << endl;
	
	double timeTakenByHorner = duration_cast<nanoseconds>(endHorner - startHorner).count();
	
  double timeTakenBySimply = 0.0;

  // Don't expect to achieve the result in a proper time for big polynomial
  if (degree <= 100000) {
    auto startSimply = high_resolution_clock::now(); 
    long double simplyValue = simply(factors, position, degree);
    auto endSimply = high_resolution_clock::now();
    
    cout<< "Polynomial evaluation finished" << endl;
    
    timeTakenBySimply = duration_cast<nanoseconds>(endSimply - startSimply).count();
  }
	
	printResult(degree, position, hornerValue, timeTakenByHorner, timeTakenBySimply);
}

int main() {
	
	int currentStep = 1; // Number of degree input cycles
	int currentCycle = 1; // Number of algorithm's invoc. with given degree
			
	// Generate seed
	srand(time(NULL));
	
	// Assert alogrithm's correctness for different data
	test();
	
	// Clean the output file
	ofstream resultFile;
	resultFile.open("algorithm_result.txt", ofstream::out | ofstream::trunc);
	resultFile.close();
	
	while (currentStep <= 4) {
		// Select array lenght
		int degree = 0;
		cout<< "Enter polynomial's degree: ";
		cin >> degree;
		
		// Reset cycle
		currentCycle = 1;
		
		while (currentCycle <= 10) {
		
      // Need a space for a free term
			int arrayLength = degree + 1; 
			
			double* factors = new double[arrayLength];
			
			fulfillWithRandoms(factors, arrayLength);
				
			double generatedPosition = getRandomDouble();
			
			executeWithTimeMeasuring(factors, generatedPosition, degree);
			
			currentCycle++;
		}
		
		currentStep++;
	}
}

void printResult(int degree, double position, long double result, double timeHorner, double timeSimply) {
	ofstream resultFile;
  // append to the file
	resultFile.open("algorithm_result.txt", ios_base::app); 

	resultFile<< endl << "---" << endl;
	resultFile<< "For a degree: " << degree << endl;
	resultFile<< "Evaluation for a given x = " << position << endl;
	resultFile<< "Results in: " << result << endl;

	resultFile<< "Time of horner's algoritm: " << getReadableTime(timeHorner) << endl;

	resultFile<< "Time of classic evaluation: " << getReadableTime(timeSimply) << endl;

	resultFile<< "---" << endl;
	resultFile.close();
	cout<< "Generated output" << endl << endl;
}

string getReadableTime(double timeNs) {
  string outputTime = std::to_string(timeNs) + " ns.";
  
  if ((timeNs / 100000) > 1.0) {
      outputTime = std::to_string(timeNs / 100000) + " ms.";
  }

  if (((timeNs / 100000) / 1000) > 1.0) {
      outputTime = std::to_string(((timeNs / 100000) / 1000)) + " sec.";
  }

  if ((((timeNs / 100000) / 1000) / 60) > 1.0) {
      outputTime = std::to_string((((timeNs / 100000) / 1000) / 60)) + " min.";
  }

  return outputTime;
}
