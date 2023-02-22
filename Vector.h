#include <GL/glut.h>
#include "bits/stdc++.h"

using namespace std;

class Vector{
public:
	
	#ifdef EXACTLY
		#define double long double
	#endif

	double x , y , z;
	Vector(){
		x = y = z = 0;
	};
	Vector(double _x , double _y , double _z){
		x = _x , y = _y , z = _z;
	};
	Vector(double _x , double _y){
		x = _x , y = _y , z = 0;
	};		
	Vector operator*(double a){
		return Vector(x*a , y * a , z * a);
	} 
	Vector operator+(Vector ot){
		return Vector(x + ot.x , y + ot.y , z + ot.z);
	} 	
	Vector operator-(Vector ot){
		return (*this) + (ot * (-1));
	} 	
	Vector operator%(Vector ot){
		return Vector(y * ot.z - ot.y * z , z * ot.x - ot.z * x , x * ot.y - y * ot.x);
	} 	
	double abs(){
		return sqrt(x * x + y * y + z * z);
	}
	double sabs(){
		return x * x + y * y + z * z;
	}

	void tonormal(){
		double a = abs();
		x /= a;
		y /= a;
		z /= a;
	}
	friend ostream& operator<<(ostream& a, const Vector b){
		a << "[" << b.x << "," << b.y << ", " << b.z << "]";
		return a;
	}
};


