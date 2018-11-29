#include "netlist_parser.hpp"

#include "component_parser.hpp"
#include "command_parser.hpp"
#include "comment_parser.hpp"
#include "subcircuit_parser.hpp"

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace bp = boost::phoenix;

netlist_parser_type::netlist_parser_type() : 
    netlist_parser_type::base_type(main, "netlist"),
    local(&spice)
{
    using qi::lit;
    using qi::omit;
    using qi::_1;
    using qi::lazy;

    spice = component_parser            | 
            command_parser              |  
            comment_parser              | 
            subcircuit_parser           | 
            subcircuit_instance_parser;

    spectre = component_parser            | 
              command_parser              |  
              comment_parser              | 
              subcircuit_parser           | 
              subcircuit_instance_parser;

    switch_map.add
        ("spice",  &spice)
        ("spectre", &spectre)
        ;
    switch_rule = lit("simulator") >> lit("lang") >> lit("=") >> switch_map;

    main = -(
                omit[switch_rule[bp::ref(local) = _1]] | 
                lazy(*bp::ref(local))
            ) % qi::eol 
        >> qi::eoi
        ;
}

netlist_parser_type netlist_parser;
