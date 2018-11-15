#include "component.hpp"

#include <sstream>
#include <map>

#include "symbol.hpp"

std::map<component_types, std::string> type_map {
    { ct_none,                              "NONE" },
    { ct_resistor,                          "R"    },
    { ct_capacitor,                         "C"    },
    { ct_inductor,                          "L"    },
    { ct_voltage_source,                    "V"    },
    { ct_current_source,                    "I"    },
    { ct_opamp,                             "O"    },
    { ct_voltage_controlled_voltage_source, "E"    },
    { ct_current_controlled_voltage_source, "F"    },
    { ct_voltage_controlled_current_source, "G"    },
    { ct_current_controlled_current_source, "H"    },
    { ct_port                             , "P"    }
};

component::component(const component_proxy& p) :
    component(p.name, p.type, p.nodes)
{
    if(p.value.size() > 0 && p.value.find_first_not_of("0123456789.-") == std::string::npos) // is the string a numeric?
    {
        value = std::stod(p.value);
    }
    else
    {
        value = get_symbol(p.value);
    }
}

component::component(const std::string& name, component_types type, const std::vector<std::string>& nodes) :
    name(name), type(type), nodes(nodes)
{   
    switch(type)
    {
        case ct_resistor:
        case ct_capacitor:
        case ct_inductor:
        case ct_voltage_source:
        case ct_current_source:
        case ct_voltage_controlled_voltage_source:
        case ct_current_controlled_current_source:
            mna_size = 1;
            terminals.push_back(".p");
            break;
        case ct_opamp:
            mna_size = 1;
            terminals.push_back(".out");
            break;
        case ct_current_controlled_voltage_source:
            mna_size = 2;
            terminals.push_back(".p");
            terminals.push_back(".cp");
            break;
        case ct_voltage_controlled_current_source:
            mna_size = 0;
            break;
        case ct_port: // TODO
            mna_size = 0;
        case ct_none: // can't happen
            break;
    }
    this->name.insert(0, type_map[type]);
}

std::string component::get_name() const
{
    return name;
}

std::vector<std::string> component::get_terminal_names() const
{
    return terminals;
}

void component::name_prepend(const std::string& prefix)
{
    name = prefix + name;
}

unsigned int component::element_size() const
{
    return mna_size;
}

const component_types& component::get_type() const
{
    return type;
}

const std::vector<std::string>& component::get_nodes() const
{
    return nodes;
}

void component::set_nodes(const std::vector<std::string>& n)
{
    nodes = n;
}

const GiNaC::ex& component::get_value() const
{
    return value;
}

bool component::operator==(component_types ct) const
{
    return type == ct;
}

std::ostream& operator<<(std::ostream& stream, const component& c)
{
    stream << c.name << ' ';
    for(const std::string& n : c.nodes)
    {
        stream << n << ' ';
    }
    stream << c.value;
    return stream;
}

// vim: nowrap
