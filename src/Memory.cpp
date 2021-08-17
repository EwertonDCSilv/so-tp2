#include "Memory.hpp"

/*
 *  @method {Construtor of class memory}
 *  @param {std::string repositionMethod - Name of reposition method} 
 *  @param {int sizepage  - Size of pages} 
 *  @param {int sizeMemory* params - Size of memory} 
 *  @return {void}
*/
Memory::Memory( std::string repositionMethod, int sizepage, int sizeMemory, int debug){
    this->sizepage = sizepage;
    this->sizeMemory = sizeMemory;
    this->numberPages = sizeMemory/sizepage;
    this->repositionMethod = repositionMethod;
    this->length = 0;
    this->shift = this->calcShift();
    this->debug = debug;

    // Creating a vector of elements
    for(int i =0; i < this->numberPages; i ++){
        this->buffer.push_back( new Elemento(-1, this->shift,-1, this->debug));
    }

    // Case debug 
    if(this->debug == 1){
        std::cout << "DEBUG MEMORY CONSTRUCT: sizepage         " << this->sizepage << "..." << std::endl;
        std::cout << "DEBUG MEMORY CONSTRUCT: sizeMemory       " << this->sizeMemory << "..." << std::endl;
        std::cout << "DEBUG MEMORY CONSTRUCT: numberPages      " << this->numberPages << "..." << std::endl;
        std::cout << "DEBUG MEMORY CONSTRUCT: repositionMethod " << this->repositionMethod << "..." << std::endl;
        std::cout << "DEBUG MEMORY CONSTRUCT: length           " << this->length << "..." << std::endl;
        std::cout << "DEBUG MEMORY CONSTRUCT: shift            " << this->shift << "..." << std::endl;
        std::cout << "DEBUG MEMORY CONSTRUCT: debug            " << this->debug << "..." << std::endl;
        std::cout << "************************************* ..." << std::endl << std::endl;
    }
}

/*
 *  @method {Calculating a shift value to find addres}
 *  @param {void} 
 *  @return {int shift - Value of shift address}
*/
int Memory::calcShift(){
    // ToDo criar metodo para calcular shift
    unsigned shift = 0; 
    unsigned temp = 0;
    
    temp = this->sizepage * 1024;
    while (temp>1) {
        temp = temp >> 1;
        shift++;
    }
    
    // Case debug 
    this->debuging();
    
    return shift;
}

/*
 *  @method {Get length of memory}
 *  @param {void} 
 *  @return {int length - Length of memory}
*/
int Memory::getComprimento(){
    // Case debug 
    this->debuging();

    return this->length;
}

/*
 *  @method {Check if memory is free page}
 *  @param {void} 
 *  @return {bool - True for free, False for full}
*/
bool Memory::isFree(){
    // Case debug 
    this->debuging();
    
    return this->getComprimento() < this->numberPages;
}

/*
 *  @method {Method to add new value in memory}
 *  @param {int index - Index position to insert data in memory} 
 *  @param {long int data - Data to insert in memory} 
 *  @param {int time - Time on insertion} 
 *  @return {bool - True for free, False for full}
*/
void Memory::add(int index, long int data, int time){
    this->buffer[index]->setTime(time);
    this->buffer[index]->setLastAccess(time);
    this->buffer[index]->setData(data);
    this->buffer[index]->setAddres(data >> this->shift);
    
    // Case debug 
    this->debuging();
}

/*
 *  @method {Method to reposition last recent used}
 *  @param {void} 
 *  @return {int index - Index to insert data }
*/
int Memory::lru(int time){
    int index = 0;
    int timeElement = time;

    // Find index to last recent used
    for(int i =0; i < this->numberPages; i++){
        timeElement = this->buffer[i]->getLastAccess();

        // Check if element time is less than current time and update the best time
        if(timeElement > time && timeElement != -1 ){
            index = i;
			time = timeElement;
        }
    }
    // Case debug 
    this->debuging();
    return index;  
}

/*
 *  @method {Method to reposition first-in-first-out}
 *  @param {void} 
 *  @return {int index - Index to insert data }
*/
int Memory::fifo(int time){
    int index = 0;
    int timeElement = time;

    // Find index to first insert
    for(int i =0; i < this->numberPages; i++){
        timeElement = this->buffer[i]->getTime();

        // Check if element time is less than current time and update the best time
        if(timeElement < time && timeElement != -1 ){
            index = i;
			time = timeElement;
        }
    }

    // Case debug 
    this->debuging();
    return index;    
}

/*
 *  @method {Method to reposition random}
 *  @param {void} 
 *  @return {int index - Index to insert data }
*/
int Memory::random(){
    // Case debug 
    this->debuging();

    return rand() % this->numberPages;
}

/*
 *  @method {Method to write in memory}
 *  @param {long int data - Data to insert in memory} 
 *  @param {int time - Time on insertion} 
 *  @return {bool - True on free memory, False on full memory}
*/
bool Memory::write(long int data, int time){
    int index = this->getComprimento();
    bool isNew = true;
    
    if(this->isFree() == false){
        // Case current repostion method to write is lru
        if(this->repositionMethod == "lru" || this->repositionMethod == "LRU")
            index = this->lru(time);

        // Case current repostion method to write is fifo
        else if(this->repositionMethod == "fifo" || this->repositionMethod == "FIFO")
            index = this->fifo(time);    

        // Case current repostion method to write is random
        else if(this->repositionMethod == "random" || this->repositionMethod == "RANDOM")
            index = this->random();
        
        isNew = false;
    }else{
        this->length++;
    }

    // Add data in current method index
    this->add(index, data, time);
    
    // Case debug 
    this->debuging();

    return isNew;
}

/*
 *  @method {Method to write in memory}
 *  @param {long int data - Data to insert in memory} 
 *  @param {int time - Time on insertion} 
 *  @return {bool - True on find, False on misses}
*/
bool Memory::read(long int data, int time){
    long int addres =  data >> this->shift;

    for(int i =0; i < this->numberPages; i++){
        if(this->debug == 1){
            std::cout << "    DEBUG READ: index            " << i           << "..." << std::endl;
            std::cout << "    DEBUG READ: addres            " << addres      << "..." << std::endl;
            std::cout << "    DEBUG READ: addres buffer            " << this->buffer[i]->getAddres() << "..." << std::endl;
            std::cout << "    DEBUG READ: addres buffer            " << this->buffer[i]->getData() << "..." << std::endl;
            std::cout << "************************************* ..." << std::endl << std::endl;
        }

        // Check values
        if(this->buffer[i]->getAddres() == addres){
            this->buffer[i]->setLastAccess(time);
            return true;
        }
    }

    // Case debug 
    this->debuging();

    return false;
}

void Memory::debuging(){
    // Case debug 
    if(this->debug == 1){
        std::cout << "DEBUG MEMORY: sizepage         " << this->sizepage << "..." << std::endl;
        std::cout << "DEBUG MEMORY: sizeMemory       " << this->sizeMemory << "..." << std::endl;
        std::cout << "DEBUG MEMORY: numberPages      " << this->numberPages << "..." << std::endl;
        std::cout << "DEBUG MEMORY: repositionMethod " << this->repositionMethod << "..." << std::endl;
        std::cout << "DEBUG MEMORY: length           " << this->length << "..." << std::endl;
        std::cout << "DEBUG MEMORY: shift            " << this->shift << "..." << std::endl;
        std::cout << "DEBUG MEMORY: debug            " << this->debug << "..." << std::endl;
        std::cout << "************************************* ..." << std::endl << std::endl;
    }
}