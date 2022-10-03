#ifndef IFILTER_H
#define IFILTER_H

template <typename T>
class IFilter
{
public:
    virtual T next(T input) = 0;

};

#endif
