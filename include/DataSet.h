#ifndef DATASET_H
#define DATASET_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "VectorInt.h"
#include "VectorLocation.h"
#include "Clustering.h"

class DataSet {
public:
    DataSet(int nInstances = 0, int nLocations = 0);

    DataSet(const DataSet& orig);

    ~DataSet();

    DataSet& operator=(const DataSet& orig);

    int getNumInstances() const;
    int getNumLocations() const;

    int getValue(int instanceIndex, int locationIndex) const;
    int getLabel(int instanceIndex) const;

    const VectorLocation& getVectorLocation() const;
    const VectorInt& getVectorLabels() const;

    std::string toString() const;

    void setValue(int instanceIndex, int locationIndex, int value);
    void setLabel(int instanceIndex, int label);

    void initInstances(int value = 0);

    void clear();

    void save(const std::string& fileName) const;
    void load(const std::string& fileName);

    DataSet getReducedDataSet(const Clustering& clustering) const;

private:
    static const std::string MAGIC_STRING_T;

    int** values;
    int nInstances;
    int nLocations;

    VectorInt labels;
    VectorLocation locations;

    void allocate(int numInstances, int numLocations);
    void deallocate();
    void copyFrom(const DataSet& orig);

    void checkInstanceIndex(int instanceIndex) const;
    void checkLocationIndex(int locationIndex) const;
};

#endif
