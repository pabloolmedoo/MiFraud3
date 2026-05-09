/*
 * Metodología de la Programación
 * Curso 2025/2026
 */

/**
 * @file VectorLocation.h
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * 
 * Created on 30 de julio de 2025, 11:27
 */

#include "Location.h"
#include "VectorLocation.h"

/**
 * @brief It builds a VectorLocation object (vector of Location objects)  
 * with a size and capacity equal to the provided value (@p size). 
 * Each element in the vector is initialized with the default Location 
 * constructor.
 * @throw std::out_of_range Throws a std::out_of_range exception if
 * @p size < 0
 * @param size The size for the vector of Location in this object. Input
 * parameter
 */
VectorLocation::VectorLocation(int size);

/**
 * @brief Copy constructor
 * @param orig the VectorLocation object used as source for the copy. 
 * Input parameter
 */
VectorLocation::VectorLocation(const VectorLocation &orig);

/**
 * @brief Destructor
 */
VectorLocation::~VectorLocation(){}

/**
 * @brief Overloading of the assignment operator for VectorLocation class
 * Modifier method
 * @param orig the VectorLocation object used as source for the assignment.
 * Input parameter
 * @return A reference to this object
 */
VectorLocation operator=(VectorLocation orig){}

/**
 * @brief Gets the number of elements in the vector of this object
 * Query method
 * @return The number of elements
 */
int VectorLocation::getSize() const{}

/**
 * @brief Gets the capacity of the vector in this object
 * Query method
 * @return The capacity of the vector in this object
 */
int VectorLocation::getCapacity() const{}

/**
 * @brief Obtains a string with information about this VectorLocation object, 
 * in the following format:
 * - First line, the number of Location objects in this vector, converted to
 * a string (using the to_string(int) C++ function). 
 * - For each Location, a line with its information, converted to
 * a string with the Location::toString() method.
 * 
 * For example, the following is the content of the string for a 
 * VectorLocation object with 4 locations:
4
24.8 14.9 Quadrangle
25.6 14.9 Ivy
26.4 14.9 Cottage
27.3 14.5 Cap & Gown
    * Query method
    * @return string with information about this VectorLocation object
    */
std::string VectorLocation::toString() const{
     std::string output;
    output += std::to_string(_size) + "\n";

    for (int i = 0; i < _size; i++)
    {
        output += _locations[i].toString() + "\n";
    }

    return output;
}

/**
 * @brief Searches the provided Location in the array of locations in this
 * object. If found, it returns the position where it was found. If not,
 * it returns -1. We consider that position 0 is the first location in the 
 * list of locations and this->getSize()-1 the last location.
 * In order to find a location consider only equality in the name field.
 * Query method
 * @param location A Location. Input parameter
 * @return If found, it returns the position where the location 
 * was found. Otherwise it returns -1
 */
int VectorLocation::findLocation(const Location& location) const{
    for (int i = 0; i < _size; i++)
    {
        if (_locations[i].getName() == location.getName())
        {
            //Esto hay que cambiarlo
            return i;
        }
    }
    return -1;
}

/**
 * @brief Returns a VectorLocation object with those locations whose 
 * positions are inside the area determined by the two given Locations. 
 * Query method
 * @param bottomLeft The Location of the bottom left point. Input parameter
 * @param topRight The Location of the top right point. Input parameter
 * @return A VectorLocation with the selected Locations.
 */
VectorLocation VectorLocation::select(const Location & bottomLeft, 
    const Location &topRight) const{
         VectorLocation output;
    for (int i = 0; i < _size; i++)
    {
        if (_locations[i].getX() >= bottomLeft.getX() && _locations[i].getX() <= topRight.getX() &&
            _locations[i].getY() >= bottomLeft.getY() && _locations[i].getY() <= topRight.getY())
        {
            output.append(_locations[i]);
        }
    }
    return output;
    }

/**
 * @brief Removes all the elements in this object, leaving the container 
 * with a size equal to 0. It only needs to set the number of elements 
 * (_size field) to zero.
 * Modifier method
 */
void VectorLocation::clear(){
    for (int i = 0; i < _size; i++)
    {
        _locations[i].set(0, 0, " ");
    }
    _size = 0;
}

/**
 * @brief Gets a const reference to the Location element at the given 
 * position
 * Query method
 * @throw std::out_of_range Throws an std::out_of_range exception if the 
 * given position is not valid.
 * @param pos position in the VectorLocation object. Input parameter
 * @return A const reference to the Location element at the given position
 */
const Location &VectorLocation:: at(int pos) const{
    if (pos < 0 || pos >= _size)
    {
        throw std::out_of_range("at: posición fuera de rango");
    }
    return _locations[pos];
}

/**
 * @brief Gets a reference to the Location element at the given position. 
 * Modifier method
 * @throw std::out_of_range Throws an std::out_of_range exception if the 
 * given position is not valid
 * @param pos position in the VectorLocation object. Input parameter
 * @return A reference to the Location element at the given position.
 */
Location &VectorLocation::at(int pos){
    if (pos < 0 || pos >= _size)
    {
        throw std::out_of_range("at: posición fuera de rango");
    }
    return _locations[pos];
}

/**
 * @brief Appends a copy of the given Location object at the first free 
 * position in the array of Location in this object. The location is only
 * appended to this object if it was not already found in this object or 
 * its name is an empty string.
 * If the dynamic array of Location was full (its capacity was full), this
 * method automatically reallocates a new array with a capacity equal to 
 * the current capacity plus an extra block of size equal to BLOCK_SIZE.
 * Modifier method
 * @param value the new Location object to be appended. Input parameter
 * @return true if the given Location could be inserted in this 
 * VectorLocation object; false otherwise (the location was already found 
 * in this object)
 */
bool VectorLocation::append(const Location& location){
    if (findLocation(location) != -1)
    {
        return false;
    }

    if (_size >= DIM_VECTOR_LOCATIONS)
    {
        throw std::out_of_range("append: el vector esta completo3");
    }

    _locations[_size]=location;
    
    _size++;
    return true;
}

/**
 * @brief Appends to this VectorLocation object, the list of  
 * Location objects contained in the provided VectorLocation object
 * that are not found (using VectorLocation::findLocation(Location)) in 
 * this object. 
 * This method could be implemented with the help of the method 
 * VectorLocation::append(const Location & location), to append to this 
 * object, the Locations of the provided VectorLocation object.
 * Modifier method
 * @param crimeSet A VectorLocation object. Input parameter
 */
void VectorLocation::join(const VectorLocation& locations){
     int resultado;
    for (int i = 0; i < locations.getSize(); i++)
    {
        resultado = findLocation(locations.at(i));
        if (resultado == -1)
        {
            append(locations.at(i));
        }
    }
}

/**
 * Sorts the array of locations in this object by increasing alphabetical 
 * order of the name of its location (a string). 
 * Modifier method
 */
void VectorLocation::sort(){
    Location auxiliar;
    for (int i = 0; i < _size - 1; i++)
    {
        for (int j = i + 1; j < _size; j++)
        {
            if (_locations[i].getName() > _locations[j].getName())
            {
                auxiliar = _locations[i];
                _locations[i] = _locations[j];
                _locations[j] = auxiliar;
            }
        }
    }
}

/**
 * @brief Gets the position in this vector of the Location object nearest to 
 * the provided location
 * Query method
 * @param location A Location object. Input parameter
 * @return the position of the Location object nearest to the provided 
 * location.
 * If returns -1 if this vector is empty
 */
int VectorLocation::nearest(Location location){
    double minimo, distancia;
    int num = 0;
    if (_size <= 0)
    {
        return -1;
    }

    minimo = location.distance(_locations[0]);

    for (int i = 0; i < _size; i++)
    {
        distancia = location.distance(_locations[i]);
        if (distancia < minimo)
        {
            minimo = distancia;
            num = i;
        }
    }
    return num;
}

/**
 * Assigns the provided value to all the elements in this vector
 * Modifier method
 * @param location A Location object. Input parameter
 */
void VectorLocation::assign(Location location){
    for (int i = 0; i < _size; i++)
    {
        _locations[i] = location;
    }
}

/**
 * @brief Reads from the provided input stream the information
 * to fill this VectorLocation object. See files *.loc in the folder 
 * DataSets as examples of this kind of file. 
 * @note This method should remove any Location previously contained in the
 * provided VectorLocation object.
 * @note This operator throws an exception in some error cases (see below). 
 * Before throwing the corresponding exception, this method clears
 * the object (it calls to clear() method) to leave the object in a
 * consistent state.
 * Modifier method
 * @throw std::out_of_range Throws a std::out_of_range exception if the
 * number of Location read from the input stream is negative.
 * @param is Input stream. Input/output parameter
 */
void VectorLocation::load(std::istream is){
    Location data;
    int nlocs;

    is >> nlocs;

    if (nlocs < 0)
    {
        clear();
        throw std::out_of_range("El numero de localizaciones es negativo");
    }
    if (nlocs > getCapacity())
    {
        clear();
        throw std::out_of_range("El numero de ubicaciones es mas grande que la capacidad");
    }

    clear();
    
    _size = nlocs;

    for (int i = 0; i < _size; i++)
    {
        data.load(is);
        _locations[i]=data;
    }
}
