#include <Stream.h>

/**
 * This is a mock uart class created for testing purposes. It contains fixed sized 
 * buffer to store characters to be returned during consecutive calls to read(). 
 */
class SimulatedUART : public Stream
{
    static const int buffer_len_max = 10;
    char buffer[buffer_len_max];
    int buffer_index = 0;
    int data_len;
public:
    /**
     * Constructor method
     * @param[in] in c-style string to feed internal buffer
     * @param[in] len length of the string 
     */
    SimulatedUART(char *in, int len)
    {
        data_len = (buffer_len_max < len) ? buffer_len_max : len;
        for (int i = 0; i < data_len; i++)
        {
            buffer[i] = in[i];
        }                
    }

    /**
     * Return number of items still available in internal buffer
     */ 
    virtual int available(void) override
    {
        return data_len - buffer_index;
    }

    /**
     * Read next character from internal buffer, if end of data in buffer is reached, return 0
     */ 
    virtual int read(void) override
    {
        if (buffer_index < data_len)
        {
            return buffer[buffer_index++];
        }
        else
        {
            return 0;
        }
    }

    /**
     * Dummy implementation of peek, does nothing
     */
    virtual int peek() override
    {
        return 0;        
    }

    /**
     * Dummy implementation of write, does nothing
     */ 
    virtual size_t write(uint8_t)
    {
        return 0; 
    }
};

