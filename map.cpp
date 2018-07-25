#include "map.hpp"

#include <vector>
#include <sstream>
#include <ostream>

#include "netlist.hpp"

map::map()
{ 
    mappy["0"]=0;
}
unsigned int map::get_map_node(std::string snode)
{
    
    unsigned int inode;
    std::map<std::string, unsigned int>::iterator it =mappy.begin(); 
    
        
        it = mappy.find(snode);
        if(it != mappy.end())
        {
            inode = it -> second;
            
        }
        else if(snode == "0" ||snode  == "GND" || snode == "gnd" || snode == "Gnd")
        {
            inode =0;
        }    
        else
        {
            inode = mappy.size();
            mappy.insert(std::make_pair(snode,inode)).first->second;
        } 
        return inode;
}    
unsigned int map::get_number_nodes() const
{
    unsigned int i = mappy.size();
    i--;
    return i;
}
unsigned int map::find_node(std::string snode) const
{
     unsigned int unode;

     auto it = mappy.find(snode);
     if (it != mappy.end())
     {
         unode = it-> second;
     }
     else
     {
         std::cerr<< "Node not existent!" << std::endl;
     }
     return unode;
}
            
         
         
         
         
         
         
         
         
         
         
         
         
// TODO : put funct in netlist
// unsigned int map::get_max_node()
// {
//     auto result = std::minmax_element(output_nodes.begin(), output_nodes.end());
//     return output_nodes[result.second - output_nodes.begin()];
// }

// std::string map::get_user_node(unsigned int unode)
// {
//     std::map<unsigned int, std::string > reverse;
//     std::string usernode = "Error: Could not find";
//     std::map<std::string, unsigned int>::iterator i;
//     std::map<unsigned int,std::string>::iterator h;
//     for ( i = mappy.begin(); i != mappy.end(); ++i)
//     {
//         reverse[i->second] = i->first;
//     }
//     h = reverse.find(unode);
//     if(h != reverse.end())
//     {
//         usernode= i -> second;
//     }
//     return usernode;
// }
