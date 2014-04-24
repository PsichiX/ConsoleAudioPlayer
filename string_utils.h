#include <iostream>
#include <vector>
#include <string>
#include <sstream>

std::vector< std::string >& string_split( const std::string& s, char delim, std::vector< std::string >& elems )
{
    std::stringstream ss( s );
    std::string item;
    while( std::getline( ss, item, delim ) )
        elems.push_back( item );
    return elems;
}


std::vector< std::string > string_split( const std::string& s, char delim )
{
    std::vector< std::string > elems;
    string_split( s, delim, elems );
    return elems;
}

std::string string_join( const std::string& separator, std::vector< std::string >::iterator from, std::vector< std::string >::iterator to )
{
    std::stringstream ss;
    for( std::vector< std::string >::iterator it = from; it != to; )
    {
        ss << *it;
        it++;
        if( it != to )
            ss << separator;
    }
    return ss.str();
}
