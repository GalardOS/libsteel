#pragma once 

namespace steel { namespace auxuart {
 
    void initialize();

    void write(char c);
    void write(const char* msg);

} }