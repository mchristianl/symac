#ifndef NETLIST_HPP
#define NETLIST_HPP

#include <vector>
#include <iterator>
#include <memory>
#include <map>

#include "component.hpp"
#include "map.hpp"
#include "subckt.hpp"

struct network_state
{
    network_state() :
        numnodes(0),
        numsources(0),
        numimpedances(0),
        numopamps(0),
        numvcvs(0),
        numccvs(0),
        numcccs(0),
        current_impedance(1),
        current_source(1),
        current_opamp(1),
        current_vcvs(1),
        current_ccvs(1),
        current_cccs(1)
    { }

    unsigned int full_size() const
    {
        return numnodes       + 
               numsources     + 
               numimpedances  + 
               numopamps      + 
               numvcvs        +
               numccvs * 2    +
               numcccs;
    }

    void reset()
    {
        current_impedance = 1;
        current_source = 1;
        current_opamp = 1;
        current_vcvs = 1;
        current_ccvs = 1;
        current_cccs = 1;
    }

    // network state
    unsigned int numnodes;
    unsigned int numsources;
    unsigned int numimpedances;
    unsigned int numopamps;
    unsigned int numvcvs;
    unsigned int numccvs;
    unsigned int numcccs;

    unsigned int current_impedance;
    unsigned int current_source;
    unsigned int current_opamp;
    unsigned int current_vcvs;
    unsigned int current_ccvs;
    unsigned int current_cccs;   
};

class netlist
{
    public:
        netlist();
        netlist(const std::string& filename);
        
        void component_read_in(const std::string& line);
        
        void add_component(std::unique_ptr<component>&&);
        void read(std::string filename);
        void print_all_components() const;
        unsigned int numbr_terminals(char type);
        unsigned int number_of_nodes() const;
        const std::vector<const component*> get_devices(component_types type) const;
        unsigned int number_of_devices(component_types type) const;
        unsigned int number_of_impedances() const;
        int number_of_voltage_sources() const;
        
        unsigned int full_network_size() const;

        unsigned int get_current_impedance() { return state.current_impedance; }
        unsigned int get_current_voltage_source() { return state.current_source; }
        unsigned int get_current_opamp() { return state.current_opamp; }
        unsigned int get_current_vcvs() { return state.current_vcvs; }
        unsigned int get_current_ccvs() { return state.current_ccvs; }
        unsigned int get_current_cccs() { return state.current_cccs; }

        void increment_current_impedance() { ++state.current_impedance; }
        void increment_current_voltage_source() { ++state.current_source; }
        void increment_current_opamp() { ++state.current_opamp; }
        void increment_current_vcvs() { ++state.current_vcvs; }
        void increment_current_ccvs() { ++state.current_ccvs; }
        void increment_current_cccs() { ++state.current_cccs; }

        const std::vector<std::unique_ptr<component>>& get_components() const
        {
            return components;
        }

        operator bool();
        // OUTPUT MAP
        void add_to_output_map(unsigned int node, std::string snode);
        std::string get_output_node(unsigned int node) const;
        unsigned int get_unode(std::string snode) const;
        // subcircuit
        bool is_subckt_call(const std::string& line);
        void subckt_call(const std::string& line);
        void read_subckt_line(const std::string& line, unsigned int number_subckt);
        void read_subckt_title(std::string& title);
        std::string change_subline_nodes(std::string line, std::vector<std::string> terminals, std::string subckt_name);
        std::string change_subline_terminals(std::string sline, std::vector<std::string> sub_t_names, std::vector<std::string> t_names);
        //matlab export
        void set_matlab_values(std::string v);
        std::vector<std::string> get_values();
        //command - simplification
        bool is_simplification();
        void set_simplification();
        std::map<std::string, unsigned int> get_simplifications();
        void save_simpl_line(const std::string& line);
        void change_simpl_map(std::string, std::string);
        //simplification level
        void set_simpl_level(const std::string&);
        std::string get_simpl_level();
        
    private:
        void reset();
        void update();

        bool valid;
        std::vector<std::unique_ptr<component>> components;
        map nmap;
        std::map<unsigned int, std::string> output_map;
        
        //subcircuit
        std::vector<subcircuit> subckt_vector;
        unsigned int number_subckt;

        network_state state;

        //matlab output
        std::vector<std::string> values;
        
        //command-simplification
        bool simplification ;
        std::vector<std::string> simplify_lines; 
        std::map<std::string, unsigned int> simpl_map;
        //simplification level (english, medium, well-done)
        std::string simpl_level;
};

#endif //NETLIST_HPP
