#include "DataSet.h"

using namespace std;

const string DataSet::MAGIC_STRING_T = "MP-FRAUD_DATASET-T-1.0";

void DataSet::allocate(int numInstances, int numLocations)
{
    if (numInstances < 0 || numLocations < 0)
    {
        throw out_of_range("Negative size in DataSet::allocate()");
    }

    nInstances = numInstances;
    nLocations = numLocations;
    values = nullptr;

    if (nInstances > 0 && nLocations > 0)
    {
        values = new int *[nInstances];

        for (int i = 0; i < nInstances; i++)
        {
            values[i] = new int[nLocations];
        }
    }
}

void DataSet::deallocate()
{
    if (values != nullptr)
    {
        for (int i = 0; i < nInstances; i++)
        {
            delete[] values[i];
        }

        delete[] values;
        values = nullptr;
    }

    nInstances = 0;
    nLocations = 0;
}

void DataSet::copyFrom(const DataSet &orig)
{
    allocate(orig.nInstances, orig.nLocations);

    labels = orig.labels;
    locations = orig.locations;

    for (int i = 0; i < nInstances; i++)
    {
        for (int j = 0; j < nLocations; j++)
        {
            values[i][j] = orig.values[i][j];
        }
    }
}

void DataSet::checkInstanceIndex(int instanceIndex) const
{
    if (instanceIndex < 0 || instanceIndex >= nInstances)
    {
        throw out_of_range("Invalid instance index");
    }
}

void DataSet::checkLocationIndex(int locationIndex) const
{
    if (locationIndex < 0 || locationIndex >= nLocations)
    {
        throw out_of_range("Invalid location index");
    }
}

DataSet::DataSet(int nInstances, int nLocations)
    : values(nullptr),
      nInstances(0),
      nLocations(0),
      labels(nInstances),
      locations(nLocations)
{

    allocate(nInstances, nLocations);
    initInstances(0);
    labels.assign(0);
}

DataSet::DataSet(const DataSet &orig)
    : values(nullptr),
      nInstances(0),
      nLocations(0),
      labels(),
      locations()
{

    copyFrom(orig);
}

DataSet::~DataSet()
{
    deallocate();
}

DataSet &DataSet::operator=(const DataSet &orig)
{
    if (this != &orig)
    {
        deallocate();
        copyFrom(orig);
    }

    return *this;
}

int DataSet::getNumInstances() const
{
    return nInstances;
}

int DataSet::getNumLocations() const
{
    return nLocations;
}

int DataSet::getValue(int instanceIndex, int locationIndex) const
{
    checkInstanceIndex(instanceIndex);
    checkLocationIndex(locationIndex);

    return values[instanceIndex][locationIndex];
}

int DataSet::getLabel(int instanceIndex) const
{
    checkInstanceIndex(instanceIndex);

    return labels.at(instanceIndex);
}

const VectorLocation &DataSet::getVectorLocation() const
{
    return locations;
}

const VectorInt &DataSet::getVectorLabels() const
{
    return labels;
}

string DataSet::toString() const
{
    ostringstream os;

    os << nLocations << '\n';

    for (int i = 0; i < nLocations; i++)
    {
        os << locations.at(i).toString() << '\n';
    }

    os << nInstances << '\n';

    for (int i = 0; i < nInstances; i++)
    {
        os << labels.at(i);

        if (i < nInstances - 1)
        {
            os << ' ';
        }
    }

    os << '\n';

    for (int i = 0; i < nInstances; i++)
    {
        for (int j = 0; j < nLocations; j++)
        {
            os << values[i][j];

            if (j < nLocations - 1)
            {
                os << ' ';
            }
        }

        os << '\n';
    }

    return os.str();
}

void DataSet::setValue(int instanceIndex, int locationIndex, int value)
{
    checkInstanceIndex(instanceIndex);
    checkLocationIndex(locationIndex);

    values[instanceIndex][locationIndex] = value;
}

void DataSet::setLabel(int instanceIndex, int label)
{
    checkInstanceIndex(instanceIndex);

    labels.at(instanceIndex) = label;
}

void DataSet::initInstances(int value)
{
    for (int i = 0; i < nInstances; i++)
    {
        for (int j = 0; j < nLocations; j++)
        {
            values[i][j] = value;
        }
    }
}

void DataSet::clear()
{
    deallocate();

    labels.clear();
    locations.clear();
}

void DataSet::save(const string &fileName) const
{
    ofstream output(fileName);

    if (!output)
    {
        throw ios_base::failure("Could not open output file: " + fileName);
    }

    output << MAGIC_STRING_T << '\n';
    output << toString();

    if (!output)
    {
        throw ios_base::failure("Error writing output file: " + fileName);
    }
}

void DataSet::load(const string &fileName)
{
    ifstream input(fileName);

    if (!input)
    {
        clear();
        throw ios_base::failure("Could not open input file: " + fileName);
    }

    try
    {
        string magic;
        getline(input, magic);

        if (magic != MAGIC_STRING_T)
        {
            clear();
            throw invalid_argument("Invalid magic string in dataset file");
        }

        int numLocations;
        input >> numLocations;

        if (!input)
        {
            clear();
            throw ios_base::failure("Error reading number of locations");
        }

        if (numLocations < 0)
        {
            clear();
            throw out_of_range("Negative number of locations");
        }

        DataSet tmp;

        tmp.locations = VectorLocation(numLocations);

        for (int i = 0; i < numLocations; i++)
        {
            Location loc;
            loc.load(input);
            tmp.locations.at(i) = loc;
        }

        int numInstances;
        input >> numInstances;

        if (!input)
        {
            clear();
            throw ios_base::failure("Error reading number of instances");
        }

        if (numInstances < 0)
        {
            clear();
            throw out_of_range("Negative number of instances");
        }

        tmp.deallocate();
        tmp.allocate(numInstances, numLocations);
        tmp.labels = VectorInt(numInstances);

        for (int i = 0; i < numInstances; i++)
        {
            int label;
            input >> label;

            if (!input)
            {
                clear();
                throw ios_base::failure("Error reading labels");
            }

            tmp.labels.at(i) = label;
        }

        for (int i = 0; i < numInstances; i++)
        {
            for (int j = 0; j < numLocations; j++)
            {
                input >> tmp.values[i][j];

                if (!input)
                {
                    clear();
                    throw ios_base::failure("Error reading dataset values");
                }
            }
        }

        *this = tmp;
    }
    catch (...)
    {
        clear();
        throw;
    }
}

DataSet DataSet::getReducedDataSet(const Clustering &clustering) const
{
    if (!clustering.isDone())
    {
        throw invalid_argument("The clustering algorithm has not been run");
    }

    if (clustering.getNumLocations() != nLocations)
    {
        throw invalid_argument("The number of locations in clustering does not match DataSet");
    }

    int k = clustering.getK();

    DataSet reduced(nInstances, k);

    reduced.locations = clustering.getCentroids();
    reduced.labels = labels;
    reduced.initInstances(0);

    for (int col = 0; col < nLocations; col++)
    {
        int clusterNumber = clustering.clusterOf(col);

        for (int row = 0; row < nInstances; row++)
        {
            reduced.values[row][clusterNumber] += values[row][col];
        }
    }

    return reduced;
}
