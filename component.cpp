#include "component.hpp"
#include <utility>

void impedance::set_stamp(unsigned int offset)
{
    stmp.write(offset, nodes[0], value);
    stmp.write(nodes[0], offset, 1);
    stmp.write(offset, nodes[1], -value);
    stmp.write(nodes[1], offset, -1);
    stmp.write(offset, offset, -1);
}

void voltage_source::set_stamp(unsigned int offset)
{
    stmp.write(offset, nodes[0], 1);
    stmp.write(nodes[0], offset, 1);
    stmp.write(offset, nodes[1], -1);
    stmp.write(nodes[1], offset, -1);
}

void current_source::set_stamp(unsigned int offset)
{
    // a current source has no stamp (no changes in the network matrix)
}

void opamp::set_stamp(unsigned int offset)
{
    stmp.write(offset, nodes[0], 1);
    stmp.write(offset, nodes[1], -1);
    stmp.write(nodes[2], offset, 1);
}

void voltage_controlled_voltage_source::set_stamp(unsigned int offset)
{
    stmp.write(offset, nodes[0], 1);
    stmp.write(nodes[0], offset, 1);
    stmp.write(offset, nodes[1], -1);
    stmp.write(nodes[1], offset, -1);
    stmp.write(offset, nodes[2], -value);
    stmp.write(offset, nodes[3], value);
}

void current_controlled_voltage_source::set_stamp(unsigned int offset)
{
    stmp.write(offset, nodes[0],     1);
    stmp.write(nodes[0], offset + 1, 1);
    stmp.write(offset, nodes[1],     -1);
    stmp.write(nodes[1], offset + 1, -1);
    stmp.write(offset + 1, nodes[2], 1);
    stmp.write(nodes[2], offset,     1);
    stmp.write(offset + 1, nodes[3], -1);
    stmp.write(nodes[3], offset,     -1);
    stmp.write(offset, offset, -value);
}

void voltage_controlled_current_source::set_stamp(unsigned int offset)
{
    // ++-- (regular)
    // -++- (inverted, what i want!)
    stmp.write(nodes[0], nodes[2], -value);
    stmp.write(nodes[0], nodes[3], +value);
    stmp.write(nodes[1], nodes[2], +value);
    stmp.write(nodes[1], nodes[3], -value);
}

void current_controlled_current_source::set_stamp(unsigned int offset)
{
    stmp.write(nodes[0], offset, value);
    stmp.write(nodes[1], offset, -value);
    stmp.write(offset, nodes[2], 1);
    stmp.write(nodes[2], offset, 1);
    stmp.write(offset, nodes[3], -1);
    stmp.write(nodes[3], offset, -1);
}

std::unique_ptr<component> create_component(char type, std::vector< unsigned int> nodes, GiNaC::ex& value)
{ 
    switch(type)
    {
        case 'R':
            return std::make_unique<resistor>(nodes,value);
        case 'C':
            return std::make_unique<capacitor>(nodes,value);
        case 'L':
            return std::make_unique<inductor>(nodes,value);
        case 'V':
            return std::make_unique<voltage_source>(nodes,value);
        case 'I':
            return std::make_unique<current_source>(nodes,value);
        case 'O':
            return std::make_unique<opamp>(nodes,value);
        case 'E':
            return std::make_unique<voltage_controlled_voltage_source>(nodes,value);
        case 'F':
            return std::make_unique<current_controlled_voltage_source>(nodes,value);
        case 'G':
            return std::make_unique<voltage_controlled_current_source>(nodes,value);
        case 'H':
            return std::make_unique<current_controlled_current_source>(nodes,value);
        default:
            std::cerr << "Unknown component: '" << type << "'\n";
            return NULL;
    }
}

// vim: nowrap
