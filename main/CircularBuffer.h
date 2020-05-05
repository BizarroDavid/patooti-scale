#ifndef ANTI_CIRCULAR_BUFFER_H
#define ANTI_CIRCULAR_BUFFER_H


// Implements a circular buffer for this project. 
// The buffer maintains a running sum which is convienent for getting a running average. 
// It is a templated class, the type should be a type that can be summed (e.g. int, float)
template <typename T>
class CircularBuffer {
    private:
        unsigned int _bufferSize=0;
        unsigned int _headIndex = -1; 
        unsigned int _tailIndex = 0; 
        T* _buffer; 
        T _bufferSum = 0;         

    public:

    // Creates an uninitialized buffer 
    CircularBuffer(); 

    ~CircularBuffer(); 

    // Create and initilize a buffer of size `bufferSize`
    CircularBuffer(unsigned int bufferSize);

    // Initialize the buffer.
    // bufferSize:  the size of the buffer.
    void Init( unsigned int bufferSize );

    // Push the next value into the buffer
    void Push( T value );

    unsigned int GetBufferSize(); 

    // Get sum of values over the entire buffer.
    T Sum();

};

template <typename T>
CircularBuffer<T>::CircularBuffer(){
}

template <typename T>
CircularBuffer<T>::~CircularBuffer(){
    delete []_buffer;
}

template <typename T>
CircularBuffer<T>::CircularBuffer(unsigned int bufferSize){
    this->Init(bufferSize);
}

template <typename T>
void CircularBuffer<T>::Init( unsigned int bufferSize ){
    _buffer = new T[bufferSize];
    _bufferSize = bufferSize;
    memset(_buffer,0, sizeof(T)*_bufferSize);
    _headIndex=-1;
    _tailIndex=0;    
    _bufferSum=0;
}

template <typename T>
void CircularBuffer<T>::Push( T value ){
    
    _bufferSum -= _buffer[_tailIndex];
    
    _headIndex++;
    if(_headIndex == _bufferSize){
        _headIndex = 0;
    }

    _tailIndex = _headIndex + 1; 
    if(_tailIndex == _bufferSize){
        _tailIndex = 0;
    }

    _buffer[_headIndex] = value;
    _bufferSum += value;

}

template <typename T>
unsigned int CircularBuffer<T>::GetBufferSize(){
    return _bufferSize;
}

template <typename T>
T CircularBuffer<T>::Sum(){
    return _bufferSum;
}




#endif  
