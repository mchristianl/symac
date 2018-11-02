#include "netlist.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <ginac/ginac.h>

#include "util.hpp"
#include "symbol.hpp"
#include "parser/netlist_parser.hpp"

void netlist::add_component(const component& c)
{
    components.add_component(c);
}

struct netlist_printer_type : public boost::static_visitor<>
{
    netlist_printer_type(netlist& nlist) :
        nlist(nlist)
    { }

    void operator() (const component& c) const
    {
        nlist.add_component(c);
    }

    void operator() (const std::string& str) const
    {
        std::cout << '"' << str << '"' << '\n';
    }

    netlist& nlist;
};

void netlist::read(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        valid = false;
        //return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string str = buffer.str();
    auto iter = str.begin();
    std::vector<boost::variant<component, std::string>> lines;
    bool r = qi::phrase_parse(iter, str.end(), netlist_parser, qi::ascii::blank, lines);

    netlist_printer_type visitor(*this);

    if (r && iter == str.end())
    {
        std::cout << "parse succeeded\n";
        for(auto e : lines)
        {
            apply_visitor(visitor, e);
        }
    }
    else
    {
        std::cout << "parse failed. Remaining unparsed: " << std::string(iter, str.end()) << '\n';
    }

    valid = true;
}

netlist::operator bool()
{
    return valid;
}

