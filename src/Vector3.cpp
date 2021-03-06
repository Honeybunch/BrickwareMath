#define BRICKWARE_MATH_EXPORTS

#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Vector4.hpp"

using namespace Brickware;
using namespace Math;

/*
	Constructors
*/
Vector3::Vector3()
{
	this->vector[0] = 0;
	this->vector[1] = 0;
	this->vector[2] = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	this->vector[0] = x;
	this->vector[1] = y;
	this->vector[2] = z;
}

Vector3::Vector3(const Vector3& other)
{
	this->vector[0] = other.vector[0];
	this->vector[1] = other.vector[1];
	this->vector[2] = other.vector[2];
}

Vector3::Vector3(Vector4& v4)
{
	float invV4W = 1/v4[3];

	this->vector[0] = v4[0] * invV4W;
	this->vector[1] = v4[1] * invV4W;
	this->vector[2] = v4[2] * invV4W;
}


/*
	Destructor
*/

Vector3::~Vector3()
{
	//if (rawData)
	//	delete[] rawData;
}

/*
	Accessors & Mutators
*/

float Vector3::getX(){ return vector[0]; }
float Vector3::getY(){ return vector[1]; }
float Vector3::getZ(){ return vector[2];}

float Vector3::getMagnitude(){ return sqrt((vector[0] * vector[0]) + (vector[1] * vector[1]) + (vector[2] * vector[2])); }
float* Vector3::getAsArray(){ return vector; }

void Vector3::setX(float x){ Vector3::vector[0] = x; }
void Vector3::setY(float y){ Vector3::vector[1] = y; }
void Vector3::setZ(float z){ Vector3::vector[2] = z; }

/*
	Static Functions
*/

//Cross product between vectors
Vector3 Vector3::Cross(Vector3 v, Vector3 u)
{
	Vector3 output;

	output.vector[0] = v.vector[1] * u.vector[2] - v.vector[2] * u.vector[1];
	output.vector[1] = v.vector[2] * u.vector[0] - v.vector[0] * u.vector[2];
	output.vector[2] = v.vector[0] * u.vector[1] - v.vector[1] * u.vector[0];

	return output;
}

//Dot product between two vectors
float Vector3::Dot(Vector3 v, Vector3 u)
{
	float output;

	output = v * u;

	return output;
}

//Normalize a vector
Vector3 Vector3::Normalize(Vector3 v)
{
	float magnitude = v.getMagnitude();

	if (magnitude == 0)
		magnitude = 1;

	Vector3 normalizedVec(v.vector[0] / magnitude,
					      v.vector[1] / magnitude,
					      v.vector[2] / magnitude);

	return normalizedVec;
}

/*
	Operators
*/

//Scalar operators
Vector3 Vector3::operator*(float s)
{
	return Vector3(vector[0] * s, vector[1] * s, vector[2] * s);
}
Vector3 Vector3::operator/(float s)
{
	return Vector3(vector[0] / s, vector[1] / s, vector[2] / s);
}
Vector3 Vector3::operator-(float s)
{
	return Vector3(vector[0] - s, vector[1] - s, vector[2] - s);
}
Vector3 Vector3::operator+(float s)
{
	return Vector3(vector[0] + s, vector[1] / s, vector[2] + s);
}

Vector3 Vector3::operator*=(float s)
{
	vector[0] *= s;
	vector[1] *= s;
	vector[2] *= s;

	return (*this);
}
Vector3 Vector3::operator/=(float s)
{
	vector[0] /= s;
	vector[1] /= s;
	vector[2] /= s;

	return (*this);
}
Vector3 Vector3::operator-=(float s)
{
	vector[0] -= s;
	vector[1] -= s;
	vector[2] -= s;

	return (*this);
}
Vector3 Vector3::operator+=(float s)
{
	vector[0] += s;
	vector[1] += s;
	vector[2] += s;

	return (*this);
}

bool Vector3::operator>(Vector3 u){ return getMagnitude() > u.getMagnitude(); }
bool Vector3::operator<(Vector3 u){ return getMagnitude() < u.getMagnitude(); }
bool Vector3::operator==(Vector3 u)
{
	if (vector[0] != u[0])
		return false;
	if (vector[1] != u[1])
		return false;
	if (vector[2] != u[2])
		return false;

	return true;
}
bool Vector3::operator!=(Vector3 u)
{
	if (vector[0] != u[0])
		return true;
	if (vector[1] != u[1])
		return true;
	if (vector[2] != u[2])
		return true;

	return false;
}

//Vector operators
float Vector3::operator* (Vector3 u)
{
	return (vector[0] * u.vector[0] + vector[1] * u.vector[1] + vector[2] * u.vector[2]);
}
Vector3 Vector3::operator+(Vector3 u)
{
	return Vector3(vector[0] + u.vector[0], vector[1] + u.vector[1], vector[2] + u.vector[2]);
}
Vector3 Vector3::operator-(Vector3 u)
{
	return Vector3(vector[0] - u.vector[0], vector[1] - u.vector[1], vector[2] - u.vector[2]);
}
Vector3 Vector3::operator+=(Vector3 u)
{
	vector[0] += u[0];
	vector[1] += u[1];
	vector[2] += u[2];

	return (*this);
}
Vector3 Vector3::operator-=(Vector3 u)
{
	vector[0] -= u[0];
	vector[1] -= u[1];
	vector[2] -= u[2];

	return (*this);
}

//Array operators
float& Vector3::operator[](int i)
{
	return vector[i];
}

//Extraction
std::ostream& operator<<(std::ostream& output, Vector3& v)
{
	output << v.getX() << std::endl << v.getY() << std::endl << v.getZ() << std::endl;
	return output;
}

//Insertion
std::istream& operator>>(std::istream& input, Vector3& v)
{
	float x, y, z;
	input >> x >> y >> z;

	v.setX(x);
	v.setY(y);
	v.setZ(z);

	return input;
}

//Casting to Vector4
Vector3::operator Vector4()
{
	return Vector4(vector[0], vector[1], vector[2], 1.0f);
}
