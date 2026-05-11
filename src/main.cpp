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

#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <exception>

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

/**
 * Converts a C-style string into an integer.
 * @param text String to convert
 * @param value Converted integer
 * @return true if the conversion was correct, false otherwise
 */
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
    string inputFile = "";
    string outputFile = DEFAULT_OUTPUT_FILE;

    int i = 1;

    while (i < argc && argv[i][0] == '-')
    {
        string option = argv[i];

        if (option == "-K")
        {
            if (i + 1 >= argc)
            {
                showHelp(cerr, "Missing value for -K");
                return 1;
            }

            if (!stringToInt(argv[i + 1], k))
            {
                showHelp(cerr, "Invalid value for -K");
                return 1;
            }

            if (k <= 0)
            {
                showHelp(cerr, "K must be greater than 0");
                return 1;
            }

            i += 2;
        }
        else if (option == "-o")
        {
            if (i + 1 >= argc)
            {
                showHelp(cerr, "Missing value for -o");
                return 1;
            }

            outputFile = argv[i + 1];
            i += 2;
        }
        else
        {
            showHelp(cerr, "Unknown option " + option);
            return 1;
        }
    }

    if (i >= argc)
    {
        showHelp(cerr, "Input file not provided");
        return 1;
    }

    inputFile = argv[i];
    i++;

    if (i < argc)
    {
        showHelp(cerr, "Too many input files");
        return 1;
    }

    try
    {
        DataSet inputDataSet;
        inputDataSet.load(inputFile);

        Clustering clustering;
        clustering.set(inputDataSet.getVectorLocation(), k);
        clustering.run();

        DataSet reducedDataSet = inputDataSet.getReducedDataSet(clustering);
        reducedDataSet.save(outputFile);
    }
    catch (const exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}
