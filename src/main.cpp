/*
 * Metodología de la Programación
 * Curso 2025/2026
 */

/**
 * @file main.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 *
 * Created on 24 de octubre de 2025, 9:27
 */

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

#include "DataSet.h"
#include "Clustering.h"

using namespace std;

/**
 * Shows help about the use of this program in the given output stream
 * @param outputStream The output stream where the help will be shown
 * @param message Additional message to show with the help
 */
void showHelp(ostream &outputStream, const string &message)
{
    outputStream << "ERROR in Fraud3 parameters. " << message << endl;
    outputStream << "Run with the following arguments: " << endl;
    outputStream << "Fraud3 [-K <K>] [-o <outputFile.dts>] <inputFile.dts>" << endl;
    outputStream << endl;
    outputStream << "Parameters: " << endl;
    outputStream << "-K <K>: an integer with the number of clusters to use "
                 << "(5 by default)" << endl;
    outputStream << "-o <outputFile.dts>: name of the output dataset file "
                 << "(tests/output/output.dts by default)" << endl;
    outputStream << "<inputFile.dts>: name of the input dataset file" << endl;
    outputStream << endl;
}

bool stringToInt(const char *text, int &value)
{
    char *end;
    long result;

    errno = 0;
    result = strtol(text, &end, 10);

    if (errno != 0 || *end != '\0' || result < INT_MIN || result > INT_MAX)
    {
        return false;
    }

    value = static_cast<int>(result);
    return true;
}

int main(int argc, char *argv[])
{
    const int DEFAULT_K = 5;
    const string DEFAULT_OUTPUT_FILE = "tests/output/output.dts";

    int k = DEFAULT_K;
    string inputFileName;
    string outputFileName = DEFAULT_OUTPUT_FILE;
    int currentArgument = 1;

    while (currentArgument < argc && argv[currentArgument][0] == '-')
    {
        string option = argv[currentArgument];

        if (option == "-K")
        {
            if (currentArgument + 1 >= argc)
            {
                showHelp(cerr, "Missing value for -K");
                return 1;
            }

            if (!stringToInt(argv[currentArgument + 1], k))
            {
                showHelp(cerr, "Invalid value for -K");
                return 1;
            }

            if (k <= 0)
            {
                showHelp(cerr, "K must be greater than 0");
                return 1;
            }

            currentArgument += 2;
        }
        else if (option == "-o")
        {
            if (currentArgument + 1 >= argc)
            {
                showHelp(cerr, "Missing value for -o");
                return 1;
            }

            outputFileName = argv[currentArgument + 1];
            currentArgument += 2;
        }
        else
        {
            showHelp(cerr, "Unknown option " + option);
            return 1;
        }
    }

    if (currentArgument >= argc)
    {
        showHelp(cerr, "Input file not provided");
        return 1;
    }

    inputFileName = argv[currentArgument];
    currentArgument++;

    if (currentArgument < argc)
    {
        showHelp(cerr, "Too many input files");
        return 1;
    }

    try
    {
        DataSet inputDataSet;
        DataSet outputDataSet;
        Clustering clustering;

        inputDataSet.load(inputFileName);

        clustering.set(inputDataSet.getVectorLocation(), k);
        clustering.run();

        outputDataSet = inputDataSet.getReducedDataSet(clustering);
        outputDataSet.save(outputFileName);
    }
    catch (const exception &error)
    {
        cerr << "ERROR: " << error.what() << endl;
        return 1;
    }

    return 0;
}
