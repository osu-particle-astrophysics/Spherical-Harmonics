#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <ctime>
#include <stdlib.h>
using namespace std;

double Pi = 3.14159265359;
int PopMAX = 100;

// Code modified from https://codereview.stackexchange.com/questions/110793/insertion-sort-in-c
void insertionSort(double array[], int length){ 		// Sort array into greatest -> least
    int i,j;
    for (i = 1; i < length; i++) {
        double temp = array[i];
        for (j = i; j > 0 && array[j - 1] < temp; j--) {
            array[j] = array[j - 1];
        }
        array[j] = temp;
    }
}

void CoordTransform(double oldVec[], double rotx, double roty, double rotz, double newVec[]){
// We are converting the old unit vector into a new unit vector using the rotations
	double x = oldVec[0];
	double y = oldVec[1];
	double z = oldVec[2];
// The below was calculated on mathematica using the 3D rotation matrices found here: https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
// We are performing rotx first, then roty, then rotz
	newVec[0] = sin(rotx)*(y*sin(roty)*cos(rotz) + z*sin(rotz)) + cos(rotx)*(z*sin(roty)*cos(rotz) - y*sin(rotz)) + x*cos(roty)*cos(rotz);
	newVec[1] = sin(rotz)*(y*sin(rotx)*sin(roty) + x*cos(roty)) + cos(rotx)*(z*sin(roty)*sin(rotz) + y*cos(rotz)) - z*sin(rotx)*cos(rotz);
	newVec[2] = y*sin(rotx)*cos(roty) + z*cos(rotx)*cos(roty) - x*sin(roty);
}

void CrossProduct(double oldVec[], double newVec[], double crossVec[]){
	double x = oldVec[0];
	double y = oldVec[1];
	double z = oldVec[2];
	
	double a = newVec[0];
	double b = newVec[1];
	double c = newVec[2];	
	
	crossVec[0] = c*y - b*z;
	crossVec[1] = a*z - c*x;
	crossVec[2] = b*x - a*y;
}

double FScore(int numSeg, double rotx[], double roty[], double rotz[]){
// First, we need to convert from rotations to unit vector coordinates. 
// Each unit vector corresponds to the direction that line segment is pointing relative to fixed coordinates
// Initialize the converted array
	double cartesian[numSeg][3];

// The first node is rotated from vertical (0,0,1)
	double newVec[3];
	double oldVec[] = {0,0,1};
	CoordTransform(oldVec, rotx[0], roty[0], rotz[0], newVec);
	for (int i = 0; i < 3; i++){
		cartesian[0][i] = newVec[i];
	}

// Now, we use loops to generate the rest of the coordinates
	for (int i = 1; i < numSeg; i++){
		for (int j = 0; j < 3; j++){									// Get the old vector (the point before point i)
			oldVec[j] = cartesian[i - 1][j];
		}
		cout << endl;
		CoordTransform(oldVec, rotx[i], roty[i], rotz[i], newVec);	// Calculate the new vector based on old vector and rotations
		for (int j = 0; j < 3; j++){
			cartesian[i][j] = newVec[j];								// Add new vectors onto cartesian
		}
	}	
	
	for (int i=0; i < numSeg; i++){
		for (int j=0; j < 3; j++){
			cout << cartesian[i][j] << " ";
		}
		cout << endl;
	}
// With [numSeg] unit vectors, we can calculate [numSeg-1] cross product vectors.
// The magnitude of these vectors in the z direction (arbitrary choice) gives us the fitness score.
	double crossVec[numSeg - 1][3];
	
	for (int i = 0; i < numSeg - 1; i++){
		for (int j = 0; j < 3; j++){
			oldVec[j] = cartesian[i][j];		// Initialize the old vector
	
		}
		for (int j = 0; j < 3; j++){
			newVec[j] = cartesian[i + 1][j];	// Initialize the new vector
		}	
		CrossProduct(oldVec, newVec, crossVec[i]);
	}

// Simply sum the z component of the cross product vectors to get the fitness score
	double fScore = 0;
	
	for (int i = 0; i < numSeg - 1; i++){
		fScore += crossVec[i][2];
	}
	
	return fScore;
}

double RotToCartesian(int numSeg, double rotx[], double roty[], double rotz[], double xcoord[], double ycoord[], double zcoord[]){
// First, we need to convert from rotations to unit vector coordinates. 
// Each unit vector corresponds to the direction that line segment is pointing relative to fixed coordinates
// Initialize the converted array
	double unitVecs[numSeg][3];

// The first node is rotated from vertical (0,0,1)
	double newVec[3];
	double oldVec[] = {0,0,1};
	CoordTransform(oldVec, rotx[0], roty[0], rotz[0], newVec);
	for (int i = 0; i < 3; i++){
		unitVecs[0][i] = newVec[i];
	}
	
// Now, we use loops to generate the rest of the coordinates
	for (int i = 1; i < numSeg; i++){
		for (int j = 0; j < 3; j++){									// Get the old vector (the point before point i)
			oldVec[j] = unitVecs[i - 1][j];
		}
		CoordTransform(oldVec, rotx[i], roty[i], rotz[i], newVec);	// Calculate the new vector based on old vector and rotations
		for (int j = 0; j < 3; j++){
			unitVecs[i][j] = newVec[j];								// Add new vectors onto cartesian
		}
	}
	
// Now, we need to convert the unit vectors into the actual coordinates. The first point is {0,0,0} and the second point is the same as the first unit vector
// Additional coordinates are made by adding the unit vector onto the previous coordinate
	xcoord[0] = 0;	
	ycoord[0] = 0;
	zcoord[0] = 0;
	
	for (int i = 0; i < numSeg; i++){
		xcoord[i+1] = xcoord[i] + unitVecs[i][0];
		ycoord[i+1] = ycoord[i] + unitVecs[i][1];
		zcoord[i+1] = zcoord[i] + unitVecs[i][2];		
	}	
}


int main ()
{
	double rotx[] = {Pi/2,0,0, 0 };
	double roty[] = {0,0,0,0 };
	double rotz[] = {0, Pi/2, Pi/2, Pi/2};
	double xcoord[5];
	double ycoord[5];
	double zcoord[5];
	int numSeg = 4;	

	RotToCartesian(numSeg, rotx, roty, rotz, xcoord, ycoord, zcoord);

	cout << "line = Line [{";
    for (int i = 0; i < numSeg; i++){
    	cout << "{" << xcoord[i] << ", " << ycoord[i] << ", "<< zcoord[i] << "}, ";
	}
	cout << "{" << xcoord[numSeg+1] << ", " << ycoord[numSeg+1] << ", "<< zcoord[numSeg+1] << "}}] ";
	return 0;
}
