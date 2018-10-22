#ifndef COMPONENT_PARSER_HPP
#define COMPONENT_PARSER_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/adapt_adt.hpp>
#include <boost/phoenix/phoenix.hpp>

namespace qi = boost::spirit::qi;

enum component_types
{
    ct_none                              = 0,
    ct_resistor                          = 1 << 0,
    ct_capacitor                         = 1 << 1,
    ct_inductor                          = 1 << 2,
    ct_voltage_source                    = 1 << 3,
    ct_current_source                    = 1 << 4,
    ct_opamp                             = 1 << 5,
    ct_voltage_controlled_voltage_source = 1 << 6,
    ct_current_controlled_voltage_source = 1 << 7,
    ct_voltage_controlled_current_source = 1 << 8,
    ct_current_controlled_current_source = 1 << 9
};

class component
{
    public:
        friend std::ostream& operator<<(std::ostream& stream, const component& c);

        void set_type(char ct)
        {
            type = ct;
        }

        void set_nodes(const std::vector<std::string>& n)
        {
            nodes = n;
        }

        void set_value(const std::string& v)
        {
            value = v;
        }

        char get_type() const
        {
            return type;
        }

        const std::vector<std::string>& get_nodes() const
        {
            return nodes;
        }

        const std::string& get_value() const
        {
            return value;
        }

    private:
        char type;
        std::vector<std::string> nodes;
        std::string value;
};

std::ostream& operator<<(std::ostream& stream, const component& c)
{
    stream << c.type << " (" << c.value << ") { ";
    for(auto n : c.nodes)
    {
        stream << n << ' ';
    }
    stream << '}';
    return stream;
}

BOOST_FUSION_ADAPT_ADT(
    component,
    (char, char, obj.get_type(), obj.set_type(val))
    (const std::vector<std::string>&, const std::vector<std::string>&, obj.get_nodes(), obj.set_nodes(val))
    (std::string, const std::string&, obj.get_value(), obj.set_value(val))
)

int number(char ct)
{
    std::map<char, unsigned int> number_of_terminals {
        { ct_resistor,                          2 },
        { ct_capacitor,                         2 },
        { ct_inductor,                          2 },
        { ct_voltage_source,                    2 },
        { ct_current_source,                    2 },
        { ct_opamp,                             3 },
        { ct_voltage_controlled_voltage_source, 4 },
        { ct_current_controlled_voltage_source, 4 },
        { ct_voltage_controlled_current_source, 4 },
        { ct_current_controlled_current_source, 4 }
    };
    return number_of_terminals[ct];
}

struct component_type_type : qi::symbols<char, char>
{
    component_type_type()
    {
        add
            ("R", 'R')
            ("C", 'C')
            ("L", 'L')
            ("V", 'V')
            ("I", 'I')
            ("O", 'O')
            ("E", 'E')
            ("F", 'F')
            ("G", 'G')
            ("H", 'H')
        ;
    }
} component_type;

struct component_parser_type : public qi::grammar<std::string::iterator, qi::ascii::blank_type, component(), qi::locals<char>>
{
    typedef std::string::iterator Iterator;

    component_parser_type() : component_parser_type::base_type(main, "component")
    {
        using qi::alnum;
        using qi::char_;
        using qi::repeat;
        using qi::ascii::space;
        using qi::_1;
        using qi::_a;
        using qi::_r1;
        using boost::phoenix::bind;

        type      %= component_type;
        terminal  %= +(alnum | char_("-:_!"));
        terminals %= repeat(bind(number, _r1))[terminal];
        value     %= +(char_ - space);
        main      %= type[_a = _1] >> terminals(_a) >> value;
    }

    qi::rule<Iterator, char()> type;
    qi::rule<Iterator, std::string()> terminal;
    qi::rule<Iterator, std::string()> value;
    qi::rule<Iterator, qi::ascii::blank_type, std::vector<std::string>(char)> terminals;
    qi::rule<Iterator, qi::ascii::blank_type, component(), qi::locals<char>> main;
} component_parser;

/*
struct component_parser_type : public qi::grammar<std::string::iterator, qi::ascii::blank_type, component()>
{
    typedef std::string::iterator Iterator;

    component_parser_type() : component_parser_type::base_type(main, "component")
    {
        using qi::alnum;
        using qi::char_;
        using qi::repeat;
        using qi::eol;

        terminal = +(alnum | char_("-:_!"));
        value    = +(char_ - eol);

        two_terminal_device   = char_("RCLVI") >> repeat(2)[terminal] >> value;
        three_terminal_device = char_("O")     >> repeat(3)[terminal] >> value;
        four_terminal_device  = char_("EFGH")  >> repeat(4)[terminal] >> value;

        main = two_terminal_device | 
                three_terminal_device | 
                four_terminal_device;
    }

    qi::rule<Iterator, std::string()> terminal, value;
    qi::rule<Iterator, component()> two_terminal_device, three_terminal_device, four_terminal_device, main;
} component_parser;
*/

#endif // COMPONENT_PARSER_HPP
