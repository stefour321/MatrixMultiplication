/*
Author: Stella Fournier
Class: ECE4122 A
Last Date Modified: 10/09/2022

Description:
This is the main code file for the matrix multiplication using openmp. It opens a text file that contains
two matrices and do multiplication on those matrices. Then, output the result matrix to an output file.
*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>
#include "MatrixMultiplication.h"
using namespace std;

// get matrix size from the string (getline) and put it in an array
int *getMatrixSize(string line)
{
    static int _matrixSize[2] = {0, 0};
    size_t pos = line.find(" ");
    _matrixSize[0] = stoi(line.substr(0, pos));
    _matrixSize[1] = stoi(line.substr(pos + 1, line.length()));
    return _matrixSize;
}

// Instantiate vector size
void init_2Dvector(vector<vector<double>> &myVector, int x, int y)
{
    myVector.resize(x);
    for (int i = 0; i < myVector.size(); i++)
    {
        myVector[i].resize(y);
    }
}

int main(int argc, char *argv[])
{
    // Get File Path From Argument
    string filePath;
    if (argc != 1)
    {
        filePath = argv[1];
    }
    else
    {
        cout << "Bad Command Line Input";
        return 0;
    }

    // Opening File
    fstream myfile;
    myfile.open(filePath);

    // Read file
    if (myfile.is_open())
    {
        string myLine;

        // get sizes A
        getline(myfile, myLine);
        int *matrixSizeA = getMatrixSize(myLine);
        int rowA = matrixSizeA[0];
        int colA = matrixSizeA[1];
        vector<vector<double>> A;

        // transfer array data A
        for (int i = 0; i < rowA; i++)
        {
            getline(myfile, myLine);
            vector<double> temp;
            for (int j = 0; j < colA; j++)
            {
                size_t pos = myLine.find(" ");
                string cut = myLine.substr(0, pos);
                temp.push_back(stod(cut));
                myLine = myLine.substr(pos + 1, myLine.length());
            }
            A.push_back(temp);
        }

        // get sizes B
        getline(myfile, myLine);
        int *matrixSizeB = getMatrixSize(myLine);
        int rowB = matrixSizeB[0];
        int colB = matrixSizeB[1];

        vector<vector<double>> B;
        // transfer array data B
        for (int i = 0; i < rowB; i++)
        {
            getline(myfile, myLine);
            vector<double> temp2;
            for (int j = 0; j < colB; j++)
            {
                size_t pos = myLine.find(" ");
                string cut = myLine.substr(0, pos);
                temp2.push_back(stod(cut));
                myLine = myLine.substr(pos + 1, myLine.length());
            }
            B.push_back(temp2);
            // cout<<endl;
        }

        // Check matrix sizes if they're valid for multiplication
        if (colA != rowB)
        {
            cout << "Invalid Matrix Size for Multiplication!" << endl;
            return 0;
        }
        else
        {
            vector<vector<double>> mul;
            // instantiate the result matrix size with known dimensions
            init_2Dvector(mul, rowA, colB);
// openMP for matrix multiplication tripple for loops
#pragma omp parallel for
            for (int i = 0; i < rowA; i++)
            {
                for (int j = 0; j < colB; j++)
                {
                    mul[i][j] = 0;
                    for (int k = 0; k < colA; k++)
                    {
                        mul[i][j] += A[i][k] * B[k][j];
                    }
                }
            }

            // Outputfile
            ofstream outFile;
            outFile.open("MatrixOut.txt");
            outFile << rowA << " " << colB << endl;
            // Output result matrix to outputfile called MatrixOut.txt
            for (int i = 0; i < rowA; i++)
            {
                for (int j = 0; j < colB; j++)
                {
                    if (j == colB - 1)
                    {
                        outFile << mul[i][j];
                    }
                    else
                    {
                        outFile << mul[i][j] << " ";
                    }
                }
                outFile << "\n";
            }

            // close output file
            outFile.close();
        }
    }
    // close input file
    myfile.close();
    return 0;
}