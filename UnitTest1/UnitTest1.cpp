#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab11.2/lab11.2.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
        TEST_METHOD(TestCalculateGrades)
        {
            const int N = 3;
            Student students[N];
            students[0] = { "Smith", 2, COMPUTER_SCIENCE, 5, 4, {5} };  
            students[1] = { "Doe", 2, COMPUTER_SCIENCE, 3, 2, {4} };
            students[2] = { "Brown", 2, COMPUTER_SCIENCE, 4, 3, {3} };
            int count5 = 0, count4 = 0, count3 = 0;

            CalculateGrades(students, N, count5, count4, count3);

            Assert::AreEqual(1, count5);  
            Assert::AreEqual(1, count4);  
            Assert::AreEqual(1, count3);  
        }

        TEST_METHOD(TestCalculatePercentage)
        {
            const int N = 3;
            Student students[N];
            students[0].physics_grade = 5;
            students[0].math_grade = 5;
            students[1].physics_grade = 4;
            students[1].math_grade = 5;
            students[2].physics_grade = 3;
            students[2].math_grade = 3;

            double result = CalculatePercentage(students, N);

            Assert::AreEqual(66.6667, result, 0.0001); 
        }
	};
}