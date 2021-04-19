#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "mathm.h"
#include "vec3.h"

#include "Math/Vector2.h"

TEST_CASE("Testing Vector2 functionality")
{
	SUBCASE("Construction")
	{
		/// Default constructor
		const Vector2 v0;
		CHECK(v0.x == 0.0f);
		CHECK(v0.y == 0.0f);

		/// Constructor from components
		const Vector2 v1(9.1f, -3.0f);
		CHECK(v1.x == 9.1f);
		CHECK(v1.y == -3.0f);

		/// Constructor from components
		const Vector2 v2(4.3f);
		CHECK(v2.x == 4.3f);
		CHECK(v2.y == 4.3f);

		/// Copy construction
		Vector2 v3(v2);
		CHECK(v2.x == v3.x);
		CHECK(v2.y == v3.y);

		/// Zero all the components of the vector
		v3.Clear();
		CHECK(v0.x == v3.x);
		CHECK(v0.y == v3.y);
	}

	SUBCASE("Mathematical operators")
	{
		const Vector2 v0(2.3f, 3.7f);
		const Vector2 v1(-6.6f, 1.2f);
		const float factor = 3.5f;
		Vector2 v2;

		/// Returns the value of the given vector added to this.
		v2 = v0 + v1;
		CHECK(v2.x == v0.x + v1.x);
		CHECK(v2.y == v0.y + v1.y);

		/// Returns the value of the given vector subtracted from this.
		v2 = v0 - v1;
		CHECK(v2.x == v0.x - v1.x);
		CHECK(v2.y == v0.y - v1.y);

		SUBCASE("Vector and scalar multiplication.")
		{
			/// Returns a copy of this vector scaled the given value.
			v2 = v1 * factor;
			CHECK(v2.x == v1.x * factor);
			CHECK(v2.y == v1.y * factor);

			/// Returns a new vector being the result of a float value multiplied on right hand side with a vector
			v2 = factor * v0;
			CHECK(v2.x == v0.x * factor);
			CHECK(v2.y == v0.y * factor);
		}

		/// Returns a copy of this vector scaled the given value.
		v2 = v0 / factor;
		CHECK(v2.x == v0.x / factor);
		CHECK(v2.y == v0.y / factor);

		SUBCASE("Dot product calculation.")
		{
			/// Calculates and returns the dot product of this vector with the given vector.
			const float dot = v0 * v1;
			CHECK(-10.74f == dot);

			const float calculatedDot = v0.Dot(v1);
			CHECK(dot == calculatedDot);
		}

		/// Adds the given vector to this.
		SUBCASE("Adds the given vector to this.")
		{
			Vector2 v3(3.0f, -4.0f);
			v3 += v0;
			CHECK(v3.x == v0.x + 3.0f);
			CHECK(v3.y == v0.y + -4.0f);
		}

		/// Subtracts the given vector from this
		SUBCASE("Subtracts the given vector from this.")
		{
			Vector2 v3(3.0f, -4.0f);
			v3 -= v1;
			CHECK(v3.x == 3.0f - v1.x);
			CHECK(v3.y == -4.0f - v1.y);
		}

		/// Multiplies this vector by the given scalar
		SUBCASE("Multiplies this vector by the given scalar.")
		{
			Vector2 v3(3.0f, -4.0f);
			v3 *= factor;
			CHECK(v3.x == 3.0f * factor);
			CHECK(v3.y == -4.0f * factor);
		}
	}

	SUBCASE("Logical operators")
	{
		/// Checks if the two vectors have identical components
		const float x = 2.93f;
		const float y = 4.39f;
		Vector2 v0(x, y);
		Vector2 v1(x, y);
		Vector2 v2(y, x);

		CHECK(v0 == v1);
		CHECK(v2 != v0);
	}

	SUBCASE("Length operations")
	{
		Vector2 v0(3.0f, 4.0f);

		/// Gets the magnitude of this vector
		const float vectorLenght = v0.Magnitude();
		CHECK(vectorLenght == 5.0f);

		/// Gets the squared magnitude of this vector
		const float vectorLenghtSq = v0.SquareMagnitude();
		CHECK(vectorLenghtSq == 25.0f);

		/// Limits the size of the vector to the given maximum
		v0.Trim(2.5f);
		CHECK(2.5f == v0.Magnitude());

		/// Returns the normalised version of a vector
		Vector2 v1 = v0;
		Vector2 v2 = v1.Unit();
		CHECK(v1 == v0);
		CHECK(1.0f == v2.Magnitude());
			
		/// Turns a non-zero vector into a vector of unit length
		v0.Normalize();
		CHECK(1.0f == v0.Magnitude());
	}
}

TEST_CASE("Testing Vector3 functionality")
{
	SUBCASE("Construction")
	{
		/// The default constructor creates a zero vector.
		vec3 v3{};
		CHECK(v3.x == 0.0f);
		CHECK(v3.y == 0.0f);
		CHECK(v3.z == 0.0f);
		/// Creates a vector with the given components
		vec3 v1(9.1f, -3.0f, -5.11f);
		CHECK(v1.x == 9.1f);
		CHECK(v1.y == -3.0f);
		CHECK(v1.z == -5.11f);
		/// Zero all the components of the vector
		v1 = vec3{};
		CHECK(v1.x == 0.0f);
		CHECK(v1.z == 0.0f);
		CHECK(v1.z == 0.0f);
	}

	SUBCASE("Mathematical operators")
	{
		const vec3 v0(2.3f, 3.7f, 1.5f);
		const vec3 v1(-6.6f, 1.2f, 2.5f);
		const float factor = 3.5f;
		vec3 v2;
		
		/// Returns the value of the given vector added to this
		v2 = v0 + v1;
		CHECK(v2.x == v0.x + v1.x);
		CHECK(v2.y == v0.y + v1.y);
		CHECK(v2.z == v0.z + v1.z);
		
		/// Returns the value of the given vector subtracted from this
		v2 = v0 - v1;
		CHECK(v2.x == v0.x - v1.x);
		CHECK(v2.y == v0.y - v1.y);
		CHECK(v2.z == v0.z - v1.z);
		
		/// Returns a copy of this vector scaled the given value
		v2 = v0 * factor;
		CHECK(v2.x == v0.x * factor);
		CHECK(v2.y == v0.y * factor);
		CHECK(v2.z == v0.z * factor);
		
		/// Returns a copy of this vector scaled the inverse of the value
		v2 = v0 * (1/factor);
		CHECK(v2.x == v0.x * (1/factor));
		CHECK(v2.y == v0.y * (1/factor));
		CHECK(v2.z == v0.z * (1/factor));
		
		/// Returns the negative this vector
		v2 = v0 * (-1);
		CHECK(v2.x == v0.x * (-1));
		CHECK(v2.y == v0.y * (-1));
		CHECK(v2.z == v0.z * (-1));
		
		/// Adds the given vector to this
		vec3 temp = v2;
		v2 += v1;
		CHECK(v2.x == temp.x + v1.x);
		CHECK(v2.y == temp.y + v1.y);
		CHECK(v2.z == temp.z + v1.z);
		
		/// Subtracts the given vector from this
		temp = v2;
		v2 -= v1;
		CHECK(v2.x == temp.x - v1.x);
		CHECK(v2.y == temp.y - v1.y);
		CHECK(v2.z == temp.z - v1.z);
		
		/// Multiplies this vector by the given scalar
		temp = v2;
		v2 *= v1;
		CHECK(v2.x == temp.x * v1.x);
		CHECK(v2.y == temp.y * v1.y);
		CHECK(v2.z == temp.z * v1.z);

		SUBCASE("Vector products")
		{
			/// Calculates and returns the dot product of this with the given vector
			temp = v1;
			float fDotProduct = v0.DotWith(temp);
			CHECK(fDotProduct == (v0.x * v1.x + v0.y * v1.y + v0.z * v1.z));
			
			/// Calculates and returns the cross product of this vector with the given vector
			vec3 v3CrossProduct = v0.CrossWith(temp);
			CHECK(v3CrossProduct.x == ((v0.y * v1.z) - (v0.z * v1.y)));
			CHECK(v3CrossProduct.y == ((v0.z * v1.x) - (v0.x * v1.z)));
			CHECK(v3CrossProduct.z == ((v0.x * v1.y) - (v0.y * v1.x)));
		}

		SUBCASE("Length operations")
		{
			///  Gets the squared magnitude of this vector
			float fMagnitude = v0.GetSquaredMagnitude();
			CHECK(fMagnitude == (v0.x * v0.x) + (v0.y * v0.y) + (v0.z * v0.z));
			
			///  Gets the magnitude of this vector
			fMagnitude = v0.GetMagnitude();
			CHECK(fMagnitude == sqrtf((v0.x * v0.x) + (v0.y * v0.y) + (v0.z * v0.z)));
			
			/// Turns a non-zero vector into a vector of unit length
			v2 = { 5.2f, 3.5f,1.3f };
			fMagnitude = v2.GetMagnitude();
			v2.Normalize();
			CHECK(v2.x == 5.2f / fMagnitude);
			CHECK(v2.y == 3.5f / fMagnitude);
			CHECK(v2.z == 1.3f / fMagnitude);
			
			
			/// Returns the normalised version of a vector
			v2 = { 5.2f, 3.5f,1.3f };
			temp = v2.Normalize();
			CHECK(temp.GetMagnitude() == 1.0f);
		}
	}

	SUBCASE("Logical operations")
	{
		vec3 v1{1,1,1};
		vec3 v2{0,0,0};
		/// Checks if the two vectors have identical components
		CHECK((v1 == v2) == false);
		/// Checks if the two vectors have non-identical components
		CHECK((v1 != v2) == true);
	}

	SUBCASE("Vec3 Edge Cases")
	{
		// normalization of a vector with magnitude 0
		// does not normalize vector
		vec3 v1{};
		vec3 v2 = v1;
		v1.Normalize();
		CHECK(v1.x == v2.x);
		CHECK(v1.y == v2.y);
		CHECK(v1.z == v2.z);

		SUBCASE("very big number")
		{
			// constructing a vector with a very big floating point number
			// (1 Trillion)
			float fBigNumber = 10000000000000.0f;
			vec3 v3{ fBigNumber, fBigNumber,fBigNumber };
			CHECK(v3.x == 10000000000000.0f);
			CHECK(v3.y == 10000000000000.0f);
			CHECK(v3.z == 10000000000000.0f);

			// normalizing a vector with a big floating point number
			// point number && checking if the magnitude == 1 (while
			// accounting for floating point inaccuracies
			v3.Normalize();
			float fMagnitude = v3.GetMagnitude();
			CHECK(mathm::CompFloat(fMagnitude, 1.0f) == true);
		}

		SUBCASE("very small number")
		{
			// constructing a vector with a very big floating point number
			float fSmallNumber = mathm::GetMachineEpsilon(1.0f);
			vec3 v3{fSmallNumber, fSmallNumber, fSmallNumber};
			
			CHECK(v3.x == fSmallNumber);
			CHECK(v3.y == fSmallNumber);
			CHECK(v3.z == fSmallNumber);

			// normalizing a vector with the smallest possible floating
			// point number && checking if the magnitude == 1 (while
			// accounting for floating point inaccuracies
			v3.Normalize();
			float fMagnitude = v3.GetMagnitude();
			CHECK(mathm::CompFloat(fMagnitude, 1.0f) == true);
		}
	}
}

TEST_CASE("Testing Matrix33 functionality")
{
	SUBCASE("Construction")
	{
		/// Construct a new matrix from explicit values
		/// Construct a new identity matrix

		/// Creates an identity matrix
		/// Creates a translation matrix
		/// Creates a scale matrix
		/// Creates a uniform scale matrix

		/// Creates a rotation matrix around an arbitrary axis (angle in radians)
		/// Creates a rotation matrix around the x axis (angle in radians)
		/// Creates a rotation matrix around the y axis (angle in radians)
		/// Creates a rotation matrix around the z axis (angle in radians)
	}

	SUBCASE("Mathematical operations")
	{
		/// Matrix addition
		/// Matrix subtraction
		/// Matrix multiplication

		SUBCASE("Inversion")
		{
			/// Get the determinant of this matrix
			/// Inverts this matrix
			/// Transposes this matrix
		}
	}

	/// Get the x axis 
	/// Get the y axis 
	/// Get the z axis 
}

TEST_CASE("Testing Matrix44 functionality")
{
	SUBCASE("Construction")
	{
		/// Construct a new matrix from explicit values
		/// Construct a new identity matrix

		/// Creates an identity matrix
		/// Creates a translation matrix
		/// Creates a scale matrix
		/// Creates a uniform scale matrix

		/// Creates a rotation matrix around an arbitrary axis (angle in radians)
		/// Creates a rotation matrix around the x axis (angle in radians)
		/// Creates a rotation matrix around the y axis (angle in radians)
		/// Creates a rotation matrix around the z axis (angle in radians)
		/// Creates an orthographic projection matrix
		/// Creates a frustum projection matrix
		/// Creates a perspective projection matrix from camera settings
		/// Creates a look at matrix, usually a view matrix  
	}

	SUBCASE("Mathematical operations")
	{
		/// Matrix addition
		/// Matrix subtraction
		/// Matrix multiplication

		SUBCASE("Inversion")
		{
			/// Get the determinant of this matrix
			/// Inverts this matrix
			/// Transposes this matrix
		}
	}

	SUBCASE("Transformation")
	{
		/// Transform the given vector by this matrix.
		/// Transform the direction vector of this matrix
	}

	/// Retrieve translation part of the matrix
	/// Set the translation of the matrix
	/// Get the x orientation axis 
	/// Get the y orientation axis 
	/// Get the z orientation axis 

	/// Sets the orientation of the matrix to the orthogonal basis vector
}
