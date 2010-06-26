/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "TestFramework.h"
#include "TestChapter.h"
#include "tbe_global.h"
#include "Position.h"

#include <cmath>

// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 4;

class TestPosition : public TestChapter
{
public:
	TestPosition() : TestChapter("Test Position class")	{}
	virtual bool runTests();
};

class TestVector : public TestChapter
{
public:
	TestVector() : TestChapter("Test Vector class")	{}
	virtual bool runTests();
};


bool TestPosition::runTests(void)
{
	{
	// simple construction tests
	Position myPos(23.3, 68.4, 42.1);
	check(myPos.x == 23.3, "X correct \n");
	check(myPos.y == 68.4, "Y correct \n");
	check(myPos.angle == 42.1, "angle correct \n");
	check(myPos.length() == sqrt(23.3*23.3+68.4*68.4), "length is correct \n");

	// test the compare operator
	Position myPos2 = myPos;
	check (myPos == myPos2, "== works correctly\n");
	Position myPos3(23.4, 68.4, 42.1);
	Position myPos4(23.3, 68.5, 42.1);
	Position myPos5(23.3, 68.4, 42.2);
	check ( !(myPos==myPos3), "== finds difference in x\n");
	check ( !(myPos==myPos4), "== finds difference in y\n");
	check ( !(myPos==myPos5), "== finds difference in angle\n");
	}

	// slightly harder - addition with and without angle
	Position myPos1(1.0, 2.0, 0.0);
	Vector   myVect(1.0, 2.0);
	const qreal PI = 3.14159265;

	Position myAdd1 = myPos1+myVect;
	check( myAdd1 == Position(2.0, 4.0, 0), "without angle, addition is easy\n");
	myPos1.angle += 1.0;
	Position myAdd2 = myPos1+myVect;
	check( !(myAdd2==myAdd1), "with angle, addition is different\n");
	myPos1.angle = PI;
	Position myAdd3 = myPos1+myVect;
	check( myAdd3 == Position(0.0, 0.0, PI), "180 degrees means subtract\n");
	myPos1.angle = PI/2.0;
	Position myAdd4 = myPos1+myVect;
	check( myAdd4 == Position(-1.0, 3.0, PI/2.0), "180 degrees means subtract\n");

	// to string conversion
	check( strcmp(ASCII(myAdd1.toString()),"(2,4)@0")       ==0, "toString() works, check 1\n");
	check( strcmp(ASCII(myAdd4.toString()),"(-1,3)@1.5708") ==0, "toString() works, check 2\n");

	return true;
}

bool TestVector::runTests(void)
{
	// I'm too lazy to test construction of Vector
	// But we do need to check compare first
	Vector myV1 (1.0, 0.0);
	Vector myV2 (0.0, 0.1);
	Vector myV1a = myV1;
	check (myV1==myV1a,   "Vector == works correctly\n");
	check (!(myV1==myV2), "Vector == detects mismatch\n");

	// creation from string is used a lot...
	// let's make sure to test the <beep> out of it...
	check ( myV1.fromString("(2,3)"), "fromString succeeds if good string\n");
	check ( myV1 == Vector(2,3),      "and the result is correct, too :-)\n");
	check ( myV2.fromString("(2.000,3.000)"), "fromString still succeeds if good string\n");
	check ( myV1 == myV2,             "and the results remain correct :-)\n");

	check ( !myV1.fromString(""),      "fromString fails if empty string\n");
	check ( !myV1.fromString("lala"),  "fromString fails if bad string\n");
	check ( !myV1.fromString("(1,2b)"),"almost good string is not good enough\n");
	check ( !myV1.fromString("(1.2)"), "almost good string is not good enough\n");
	check ( myV1 == myV2,              "original value still in vector after all the misery\n");

	Vector myV3 = -3 * Vector(2,3);
	check ( myV3 == Vector(-6,-9),     "multiply with constant works\n");
	Vector myV4 = Vector(1,0) * Vector(2,3);
	check ( myV4 == Vector(2,0),       "multiply with scale a works\n");
	Vector myV5 = Vector(0,1) * Vector(2,3);
	check ( myV5 == Vector(0,3),       "multiply with scale a works\n");


	return true;
}


int main(int argc, char *argv[])
{
	TestFramework myFramework(argc, argv);

	myFramework.add( new TestPosition );
	myFramework.add( new TestVector );

	myFramework.run();

	return 0;
}