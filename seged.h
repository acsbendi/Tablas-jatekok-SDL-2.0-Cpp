#ifndef SEGED_H_INCLUDED
#define SEGED_H_INCLUDED
#include <string>
#include <sstream>


namespace seged
{
    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream os ;
        os << value ;
        return os.str() ;
    }
}

#endif // SEGED_H_INCLUDED
