#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iomanip>
#include "image.h"

using namespace std;

void odds(int r, int c, vector< vector<double> > global);

int main(int argc, char** argv) {

	// Arg 1 = executable
	// Arg 2 = training
	if (argc != 1) {
		cout << "Number of inputs is wrong. Just do ./mp4";
	}

	// STORE ALL THEM DIGITS

	vector< vector<double> > global; // keep an Image for each digit, with probability for each pixel

	for (int a = 0; a < 10; a++) {  // INITIALIZE global
		vector<double> global_digit;
		for (int b = 0; b < 784; b++) {
			global_digit.push_back(0);
		}
		global.push_back(global_digit);
	}

	vector<int> nums; // to store # of examples per digit
	vector<int> testing_nums;

	for (int p = 0; p < 10; p++) { // INITIALIZE nums
		nums.push_back(0);
		testing_nums.push_back(0);
	}
	vector< vector<double> > weight_vector; // 10 by 784, 2D weight vector for multi-class perceptron
	for (int i = 0; i < 10; i++) {
		vector<double>temp;
		for (int j = 0; j < 784; j++) { // INITIALIZE weight_vector
			temp.push_back(0);
		}
		weight_vector.push_back(temp);
	}
	// AMOUNT OF TIMES TO RUN THROUGH TRAINING TO IMPROVE LEARNING
	int epoch = 3;
	double alpha = (double)1000 / (1000+epoch);

	double laplace_k = 0.1; // set laplacian smoothing constants
	int laplace_V = 2;

	vector< Image > training_images; // training image vector (size should be 5000)
	vector< Image > testing_images; // testing image vector (size should be 1000)

	const char* training_image = "trainingimages"; // filenames
	const char* training_label = "traininglabels";
	const char* test_image = "testimages";
	const char* test_label = "testlabels";

	// Storage processing
	ifstream image_file (training_image);
	ifstream label_file (training_label);
	ifstream test_file (test_image);
	ifstream label_test (test_label);

	string char_line, label_line;
	if (image_file.is_open() && label_file.is_open()) {
		while (image_file.good() && label_file.good()) {	

			vector<string> image_string;					
			for (int x = 0; x < 28; x++)					
			{
				getline (image_file, char_line);			
				image_string.push_back(char_line);			
			}
			getline (label_file, label_line);				
			if (label_file.eof()) {							//check for end of label file
				break;
			}
			int label_val = atoi (label_line.c_str());		
			
			Image img = Image(image_string, label_val);		
			training_images.push_back(img);					
		}
		image_file.close();
		label_file.close();
	}

	if (test_file.is_open() && label_test.is_open()) {
		while (test_file.good() && label_test.good()) {	

			vector<string> image_string;					
			for (int x = 0; x < 28; x++)					//loops for the (relative) size of all of the chars
			{
				getline (test_file, char_line);			
				image_string.push_back(char_line);			
			}
			getline (label_test, label_line);				
			if (label_test.eof()) {							//check for end of label file
				break;
			}
			int label_val = atoi (label_line.c_str());		
			
			Image img = Image(image_string, label_val);		
			testing_images.push_back(img);					
		}
		test_file.close();
		label_test.close();
	}

// ========================================= TRAINING & SETUP ===========================================
	//just change shit to 1s and 0s
	for (int i = 0; i < training_images.size(); i++) { // We want P(Fij | class)
		for (int x = 0; x < 28; x++) { 			
			for (int y = 0; y < 28; y++) {
				// 1 and 0 be features
				switch(training_images[i].image_data[x][y]) {
					case '+':
						training_images[i].image_data[x][y] = '1';
						break;
					case '#':
						training_images[i].image_data[x][y] = '1';
						break;
					case ' ':
						training_images[i].image_data[x][y] = '0';
						break;
				}
				// once features done, then check what number it is
				// whatever number, increment the number in global
			}
		}
		nums[training_images[i].real_number]++;
	}

	for (int w = 0; w < epoch; w++) { // how many times we want to run through training
		// PERCEPTRON
		// decrease in weight vec for the one i mispredict, increase weight vec for correct predict

		for (int i = 0; i < training_images.size(); i++) { // go through each training example
			vector<int> temp; // resulting 10 x 1 vector
			for (int j = 0; j < 10; j++) {
				temp.push_back(0);
			}
			// compare with weight vector, do matrix mult: 10 x 784 * 784 * 1
			for (int m = 0; m < 10; m++) {
				for (int n = 0; n < 1; n++) {
					for (int x = 0; x < 28; x++) { // for each pixel
						for (int y = 0; y < 28; y++) {
							if (training_images[i].image_data[x][y] == '1'){
								temp[m] += (weight_vector[m][x*28+y] * 1);
							}
							else {
								temp[m] += (weight_vector[m][x*28+y] * 0);
							}
						}
					}
				}
			}
			int best_index = 0;
			int best_value = -1410065408;
			// find index with highest value in result 10 x 1
			for (int p = 0; p < 10; p++) {
				if (temp[p] > best_value) {
					best_value = temp[p];
					best_index = p;
				}
				// will get index with highest value at end of these iterations
			}

			// UPDATES
			if (best_index == training_images[i].real_number) {
				// correct prediction - we good
				// increment actual digit

			}
			else {
				// incorrect prediction
				// increment actual digit, decrement misprediction digit
				for (int q = 0; q < 28; q++) {
					for (int r = 0; r < 28; r++) {
						if (training_images[i].image_data[q][r] == '1') {
							weight_vector[training_images[i].real_number][q*28+r] += alpha * 1;
							weight_vector[best_index][q*28+r] -= alpha * 1;
						}
						if (training_images[i].image_data[q][r] == '0') {
							weight_vector[training_images[i].real_number][q*28+r] += 0;
							weight_vector[best_index][q*28+r] += 0;
						}
					}
				}

			}

			// if mispredict, decrement mispredict's digit, and increment actual predict's digit
		}
	}

	// ====================================== TESTING ==========================================

	for (int i = 0; i < testing_images.size(); i++) { // for each image
		for (int x = 0; x < 28; x++) { 				    // for each pixel
			for (int y = 0; y < 28; y++) {

				// 1 and 0 be features
				switch(testing_images[i].image_data[x][y]) {
					case '+':
						testing_images[i].image_data[x][y] = '1';
					
						break;
					case '#':
						testing_images[i].image_data[x][y] = '1';
						break;
					case ' ':
						testing_images[i].image_data[x][y] = '0';
						break;
				}
				// once features done, then check what number it is
				// whatever number, increment the number in global
			}
		}
		testing_nums[testing_images[i].real_number]++;
	}

	int accurate = 0;
	int inaccurate = 0;
	vector< vector<double> > confusion_matrix;
	vector<int> test_digits; 
	for (int p = 0; p < 10; p++) {
		test_digits.push_back(0);
	}

	//initialize confusion matrix
	for (int a = 0; a < 10; a++) { // for each digit
		vector<double> temp_vec;
		for (int b = 0; b < 10; b++) { //for each pixel in digit
			temp_vec.push_back(0);
		}
		confusion_matrix.push_back(temp_vec);
	}

	for (int i = 0; i < testing_images.size(); i++) {
		vector<int> temp; // resulting 10 x 1 vector
		for (int j = 0; j < 10; j++) {
			temp.push_back(0);
		}
		// compare with weight vector, do matrix mult: 10 x 784 * 784 * 1
		for (int m = 0; m < 10; m++) {
			for (int n = 0; n < 1; n++) {
				for (int x = 0; x < 28; x++) { // for each pixel
					for (int y = 0; y < 28; y++) {
						if (testing_images[i].image_data[x][y] == '1'){
							temp[m] = temp[m] + (weight_vector[m][x*28+y] * 1);
						}
						else {
							temp[m] = temp[m] + (weight_vector[m][x*28+y] * 0);
						}
					}
				}
			}
		}
		int best_index = 0;
		int best_value = -1000000;
		// find index with highest value in result 10 x 1
		for (int p = 0; p < 10; p++) {
			if (temp[p] > best_value) {
				best_value = temp[p];
				best_index = p;
			}
			// will get index with highest value at end of these iterations
		}

		if (best_index == testing_images[i].real_number) {
			// correct prediction - we good
			test_digits[testing_images[i].real_number]++;
			accurate++;
		}
		else {
			inaccurate++;
			confusion_matrix[testing_images[i].real_number][best_index]++;
		}
	}

	cout << "Total # tests: " << testing_images.size() << endl;
	cout << "Accurate: " << accurate <<endl;
	cout << "Inaccurate: " << inaccurate << endl;
	cout << "Accuracy: " << (double)accurate/testing_images.size()*100 << "%" << endl;

	for (int g = 0; g < 10; g++) {
		cout << "Accuracy for " << g << " is: " << (double)test_digits[g]/testing_nums[g]*100.0 << "%" << endl;
	}
	
	// classification matrix
	//10x10 matrix whose entry in row r and column c 
	// is the percentage of test images from class r that are classified as class c
	// e.g. if 9 mistaken as 7, would add to (9, 7)
	cout << "Confusion Matrix" <<endl;
	for (int i = 0; i < 10; i++) {
		cout << "   ";
		for (int j = 0; j < 10; j++) {
			confusion_matrix[i][j] /= testing_nums[i];
			cout << setprecision(2) << fixed << confusion_matrix[i][j]*100 << "%   ";
		}
		cout << endl;
	}

	/*
	Learning rate decay function; 							1000/1000+epoch
	Bias vs. no bias;										Bias
	Initialization of weights (zeros vs. random); 			initialize 0
	Ordering of training examples (fixed vs. random);		fixed (in chronological order)
	Number of epochs. 										3
	*/
}
