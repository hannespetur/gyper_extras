#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>

#include <seqan/arg_parse.h>
#include <seqan/basic.h>
#include <seqan/file.h>
#include <seqan/random.h>
#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include <seqan/stream.h>

#include "imgt_xml_wrapper.hpp"

// By default, use the seqan namespace.
using namespace seqan;

/* 
    Struct that keeps all our argument options in one place.
*/
struct args
{
    bool verbose;
    bool icelandic;
    bool features;
    seqan::CharString output;
    seqan::CharString gene; 
    seqan::CharString xml; 

    void printArgs(){ // Print out the struct values.
        if (endPosition (output) == 0) {
            std::cout << "Output to standard output." << std::endl;
        }
        else {
            std::cout << "Output to file: " << output << std::endl;
        }

        if (endPosition (gene) == 0) {
            std::cout << "Using all genes." << std::endl;
        }
        else {
            std::cout << "Using gene: " << gene << std::endl;
        }

        if (endPosition (xml) == 0) {
            std::cout << "Using the hla.xml." << std::endl;
        } else {
            std::cout << "Using xml: " << xml << std::endl;
        }
        
        std::cout << "Verbose mode: " << verbose << std::endl;
        std::cout << "Icelandic alleles only: " << icelandic << std::endl;
        std::cout << "Use features: " << features << std::endl;
    }
};

/*  
    parseCommandLine
*/
ArgumentParser::ParseResult
parseCommandLine(args& AO, ArgumentParser & parser, int argc, char const ** argv )
{
    setVersion(parser, "0.1");
    setDate(parser, "May 2015");

    addUsageLine(parser, "[\\fIOPTIONS\\fP]");
    addDescription(parser, "Descr.");

    addSection(parser, "Main options");
    addOption(parser, ArgParseOption("v", "verbose", "Use verbose mode."));
    addOption(parser, ArgParseOption("i", "icelandic", "Use only alleles that are found commonly in Icelanders."));
    addOption(parser, ArgParseOption("f", "features", "Get features (exon, intron or untranslated) in each entry instead of whole allele."));
    addOption(parser, ArgParseOption("o", "output",
                                     "Specify output folder for FASTA file (default: standard output).",
                                     ArgParseArgument::STRING)
                                    );

    addOption(parser, ArgParseOption("g", "gene",
                                        "Specify a single gene to get data from. Available genes are at least: HLA-A, HLA-B, HLA-C, HLA-DRB, HLA-DQA1, and HLA-DQB1. (default: all).",
                                        ArgParseArgument::STRING));

    addOption(parser, ArgParseOption("x", "xml",
                                     "Specify an XML to read from (default: hla.xml).",
                                     ArgParseArgument::STRING)
                                    );


    ArgumentParser::ParseResult res = parse(parser, argc, argv);

    if (res != ArgumentParser::PARSE_OK)
    {
        return res;
    }

    AO.verbose = isSet(parser, "verbose");
    AO.icelandic = isSet(parser, "icelandic");
    AO.features = isSet(parser, "features");
    seqan::CharString output;

    if (isSet(parser, "output"))
    {
        getOptionValue(output, parser, "output");
    }

    AO.output = output;
    seqan::CharString gene;
    
    if (isSet(parser, "gene"))
    {
        getOptionValue(gene, parser, "gene");
    }

    AO.gene = gene;

    seqan::CharString xml;

    if (isSet(parser, "xml"))
    {
        getOptionValue(xml, parser, "xml");
    }
    else
    {
        xml = "hla.xml";
    }

    AO.xml = xml;

    if (AO.verbose)
    {
        AO.printArgs();
    }

    return ArgumentParser::PARSE_OK;
}


void
writeToFile(std::stringstream& ss, char* file)
{  
    using namespace std;
    string myString = ss.str();  
    ofstream myfile;  
    myfile.open (file);
    myfile << myString;  
    myfile.close();  
}


std::vector<std::string>
getIcelandicAlleles()
{
    std::vector<std::string> icelandic_alleles;

    /* DQA1 */
    icelandic_alleles.push_back("HLA-DQA1*01:01"); // 101
    icelandic_alleles.push_back("HLA-DQA1*01:02"); // 102
    icelandic_alleles.push_back("HLA-DQA1*01:03"); // 103
    icelandic_alleles.push_back("HLA-DQA1*01:05"); // 105
    icelandic_alleles.push_back("HLA-DQA1*02:01"); // 201
    icelandic_alleles.push_back("HLA-DQA1*03:01"); // 301
    icelandic_alleles.push_back("HLA-DQA1*03:02"); // 302
    icelandic_alleles.push_back("HLA-DQA1*03:03"); // 303
    icelandic_alleles.push_back("HLA-DQA1*04:01"); // 401
    icelandic_alleles.push_back("HLA-DQA1*05:01"); // 501
    icelandic_alleles.push_back("HLA-DQA1*05:05"); // 505
    icelandic_alleles.push_back("HLA-DQA1*06:01"); // 601

    /* DQB1 */
    icelandic_alleles.push_back("HLA-DQB1*02:01"); // 201
    icelandic_alleles.push_back("HLA-DQB1*02:02"); // 202
    icelandic_alleles.push_back("HLA-DQB1*03:01"); // 301
    icelandic_alleles.push_back("HLA-DQB1*03:02"); // 302
    icelandic_alleles.push_back("HLA-DQB1*03:03"); // 303
    icelandic_alleles.push_back("HLA-DQB1*04:02"); // 402
    icelandic_alleles.push_back("HLA-DQB1*05:01"); // 501
    icelandic_alleles.push_back("HLA-DQB1*05:02"); // 502
    icelandic_alleles.push_back("HLA-DQB1*05:03"); // 503
    icelandic_alleles.push_back("HLA-DQB1*06:02"); // 602
    icelandic_alleles.push_back("HLA-DQB1*06:03"); // 603
    icelandic_alleles.push_back("HLA-DQB1*06:04"); // 604
    icelandic_alleles.push_back("HLA-DQB1*06:09"); // 609

    /* DRB1 */
    icelandic_alleles.push_back("HLA-DRB1*01:01"); // 101
    icelandic_alleles.push_back("HLA-DRB1*01:03"); // 103
    icelandic_alleles.push_back("HLA-DRB1*03:01"); // 301
    icelandic_alleles.push_back("HLA-DRB1*04:01"); // 401
    icelandic_alleles.push_back("HLA-DRB1*04:03"); // 403
    icelandic_alleles.push_back("HLA-DRB1*04:04"); // 404
    icelandic_alleles.push_back("HLA-DRB1*04:05"); // 405
    icelandic_alleles.push_back("HLA-DRB1*04:07"); // 407
    icelandic_alleles.push_back("HLA-DRB1*04:08"); // 408
    icelandic_alleles.push_back("HLA-DRB1*07:01"); // 701
    icelandic_alleles.push_back("HLA-DRB1*08:01"); // 801
    icelandic_alleles.push_back("HLA-DRB1*09:01"); // 901
    icelandic_alleles.push_back("HLA-DRB1*10:01"); // 1001
    icelandic_alleles.push_back("HLA-DRB1*11:01"); // 1101
    icelandic_alleles.push_back("HLA-DRB1*11:03"); // 1103
    icelandic_alleles.push_back("HLA-DRB1*11:04"); // 1104
    icelandic_alleles.push_back("HLA-DRB1*12:01"); // 1201
    icelandic_alleles.push_back("HLA-DRB1*13:01"); // 1301
    icelandic_alleles.push_back("HLA-DRB1*13:02"); // 1302
    icelandic_alleles.push_back("HLA-DRB1*13:03"); // 1303
    icelandic_alleles.push_back("HLA-DRB1*14:01"); // 1401
    icelandic_alleles.push_back("HLA-DRB1*15:01"); // 1501
    icelandic_alleles.push_back("HLA-DRB1*16:01"); // 1601

    /* HLAA */
    icelandic_alleles.push_back("HLA-A*01:01"); // 101
    icelandic_alleles.push_back("HLA-A*02:01"); // 201
    icelandic_alleles.push_back("HLA-A*03:01"); // 301
    icelandic_alleles.push_back("HLA-A*11:01"); // 1101
    icelandic_alleles.push_back("HLA-A*23:01"); // 2301
    icelandic_alleles.push_back("HLA-A*24:02"); // 2402
    icelandic_alleles.push_back("HLA-A*25:01"); // 2501
    icelandic_alleles.push_back("HLA-A*26:01"); // 2601
    icelandic_alleles.push_back("HLA-A*29:01"); // 2901
    icelandic_alleles.push_back("HLA-A*30:01"); // 3001
    icelandic_alleles.push_back("HLA-A*31:01"); // 3101
    icelandic_alleles.push_back("HLA-A*32:01"); // 3201
    icelandic_alleles.push_back("HLA-A*33:01"); // 3301
    icelandic_alleles.push_back("HLA-A*68:01"); // 6801

    /* HLAB */
    icelandic_alleles.push_back("HLA-B*07"); // 7
    icelandic_alleles.push_back("HLA-B*08"); // 8
    icelandic_alleles.push_back("HLA-B*13"); // 13
    icelandic_alleles.push_back("HLA-B*14"); // 14
    icelandic_alleles.push_back("HLA-B*15"); // 15
    icelandic_alleles.push_back("HLA-B*18"); // 18
    icelandic_alleles.push_back("HLA-B*27"); // 27
    icelandic_alleles.push_back("HLA-B*35"); // 35
    icelandic_alleles.push_back("HLA-B*37"); // 37
    icelandic_alleles.push_back("HLA-B*38"); // 38
    icelandic_alleles.push_back("HLA-B*39"); // 39
    icelandic_alleles.push_back("HLA-B*40"); // 40
    icelandic_alleles.push_back("HLA-B*44"); // 44
    icelandic_alleles.push_back("HLA-B*45"); // 45
    icelandic_alleles.push_back("HLA-B*49"); // 49
    icelandic_alleles.push_back("HLA-B*51"); // 51
    icelandic_alleles.push_back("HLA-B*52"); // 52
    icelandic_alleles.push_back("HLA-B*53"); // 53
    icelandic_alleles.push_back("HLA-B*55"); // 55
    icelandic_alleles.push_back("HLA-B*56"); // 56
    icelandic_alleles.push_back("HLA-B*57"); // 57
    icelandic_alleles.push_back("HLA-B*58"); // 58

    /* HLAC */
    icelandic_alleles.push_back("HLA-C*01"); // 1
    icelandic_alleles.push_back("HLA-C*02"); // 2
    icelandic_alleles.push_back("HLA-C*03"); // 3
    icelandic_alleles.push_back("HLA-C*04"); // 4
    icelandic_alleles.push_back("HLA-C*05"); // 5
    icelandic_alleles.push_back("HLA-C*06"); // 6
    icelandic_alleles.push_back("HLA-C*07"); // 7
    icelandic_alleles.push_back("HLA-C*08"); // 8
    icelandic_alleles.push_back("HLA-C*12"); // 12
    icelandic_alleles.push_back("HLA-C*14"); // 14
    icelandic_alleles.push_back("HLA-C*15"); // 15
    icelandic_alleles.push_back("HLA-C*16"); // 16
    icelandic_alleles.push_back("HLA-C*17"); // 17

    return icelandic_alleles;
}


int getDigits(std::string allele)
{
    // std::cout << "allele = " << allele << std::endl;
    // std::cout << "find(allele.begin(), allele.end(), '*') - allele.begin() = " << find(allele.begin(), allele.end(), '*') - allele.begin() << std::endl;
    int suffix_size = allele.size() - (find(allele.begin(), allele.end(), '*') - allele.begin());
    if (suffix_size == 3)
        return 2;
    if (suffix_size == 6)
        return 4;
    if (suffix_size == 9)
        return 6;
    if (suffix_size == 12)
        return 8;
    std::cout << "Unkown suffix for allele: " << allele << std::endl;
    return 1;
}

typedef std::map<std::string, rapidxml::xml_node<> *>::iterator it_type;

std::vector<std::string> getSixDigitIcelandicAlleles(std::map<std::string, rapidxml::xml_node<> *> nodes_table)
{
    std::vector<std::string> icelandic_alleles = getIcelandicAlleles();
    std::vector<std::string> six_digit_icelandic_alleles;

    for (std::vector<std::string>::iterator dig_it = icelandic_alleles.begin(); dig_it != icelandic_alleles.end(); ++dig_it)
    {
        int digits = getDigits(*dig_it);
        if (digits == 2)
        {
            for (int four = 0; four < 1000; ++four)
            {
                std::ostringstream new_allele;
                new_allele << *dig_it << ":" << std::setfill('0') << std::setw(2) << four;
                bool found_six_digit = false;

                for (int six = 0; six < 1000; ++six)
                {
                    std::ostringstream new_allele_six;
                    new_allele_six << new_allele.str() << ":" << std::setfill('0') << std::setw(2) << six;
                    std::string new_allele_str = new_allele_six.str();

                    if (nodes_table.count(new_allele_str) == 1)
                    {
                        found_six_digit = true;
                        six_digit_icelandic_alleles.push_back(new_allele_str);
                    }
                }

                if (!found_six_digit)
                {
                    std::string new_allele_str = new_allele.str();
                    if (nodes_table.count(new_allele_str) == 1)
                    {
                        six_digit_icelandic_alleles.push_back(new_allele_str);
                    }
                }
            }
        }
        else if (digits == 4)
        {
            bool found_six_digit = false;
            for (int six = 0; six < 1000; ++six)
            {
                std::ostringstream new_allele_six;
                new_allele_six << *dig_it << ":" << std::setfill('0') << std::setw(2) << six;
                std::string new_allele_str = new_allele_six.str();

                if (nodes_table.count(new_allele_str) == 1)
                {
                    found_six_digit = true;
                    six_digit_icelandic_alleles.push_back(new_allele_str);
                }
            }

            if (!found_six_digit)
            {
                std::string new_allele_str(*dig_it);
                if (nodes_table.count(new_allele_str) == 1)
                {
                    six_digit_icelandic_alleles.push_back(new_allele_str);
                }
            }
        }
        else
        {
            std::cerr << "Digits are unsupported: " << digits << std::endl;
        }
    }
    return six_digit_icelandic_alleles;
}


void
addToOutput(
    std::string & allele_id_str,
    std::map<std::string, rapidxml::xml_node<> *> & nodes_table,
    std::stringstream & output_stream
)
{
    // std::cerr << allele_id_str << std::endl;

    // char* allele_id = &allele_id_str[0u];

    std::vector<std::string> all_features = getFeatureNames(allele_id_str.data(), nodes_table);

    unsigned num_exons = 0;
    unsigned num_introns = 0;

    {
        bool contains_all_complete_features = true;
        rapidxml::xml_node<> * allele = getAllele(allele_id_str.data(), nodes_table);

        if (!allele)
        {
            std::cout << "[main] INFO: Allele not found." << std::endl;
            return;
        }

        for (auto feature_it = all_features.begin() ; feature_it != all_features.end() ; ++feature_it )
        {
            for (rapidxml::xml_node<> * feature = allele->first_node("sequence")->first_node("feature") ; feature ; feature = feature->next_sibling())
            {
                if (feature->first_attribute("status"))
                {
                    // std::cout << "[main] INFO: Status found " << feature->first_attribute("status")->value() << std::endl;

                    if ( strcmp(feature->first_attribute("status")->value(), "Partial") == 0 )
                    {
                        // std::cout << "Throwing name " << allele->first_attribute("name")->value() << std::endl;
                        contains_all_complete_features = false;
                        break;
                    }
                }
                
            }
        }

        if (not contains_all_complete_features)
        {
            return;
        }
    }

    for (std::vector<std::string>::iterator feature_it = all_features.begin() ; feature_it != all_features.end() ; ++feature_it )
    {
        std::string feature_sequence = getFullFeature(allele_id_str.data(), nodes_table, *feature_it);
        std::string feature_id = *feature_it;
        std::string first_char = feature_id.substr(0,1);
        // std::cout << "Feature_it is " << feature_id << " " << feature_id.substr(0,1) << std::endl;
        if (first_char == "E")
        {
            feature_id.replace(1,4,"");
            ++num_exons;
        }
        else if (first_char == "I")
        {
            feature_id.replace(1,6,"");
            ++num_introns;
        }
        else if (first_char == "3")
        {
            feature_id = "P3";
        }
        else if (first_char == "5")
        {
            feature_id = "5P";
        }
        else
        {
            std::cerr << "Unexpected feature '" << feature_id << "'. Ignoring." << std::endl;
            continue;
        }

        // output_stream << ">" << feature_id << " " << allele_id << " (" << feature_sequence.size() << " bps)" << std::endl;
        output_stream << '>' << allele_id_str.data() << '_' << feature_id << std::endl;
        output_stream << getFullFeature(allele_id_str.data(), nodes_table, *feature_it) << std::endl;
    }

    if (num_introns != num_exons - 1)
    {
        std::cerr << "[main] WARNING: Allele " << allele_id_str << " has unexpected number of exons compared to introns." << std::endl; 
    }
}


int main(int argc, char const ** argv)
{
    ArgumentParser parser("imgt_xml_parser");
    args AO;

    // Parse the command line
    if (parseCommandLine(AO, parser, argc, argv) != ArgumentParser::PARSE_OK)
        return ArgumentParser::PARSE_ERROR;

    if (AO.verbose)
    {
        std::cout << "Starting IMGT XML wrapper..." << std::endl;
    }

    char* xml = toCString(AO.xml);
    // char* xml = (char*)"dummy.xml";
    rapidxml::file<> xmlFile(xml);
    rapidxml::xml_document<char> doc;
    doc.parse<0>(xmlFile.data());
    rapidxml::xml_node<> * my_root_node = doc.first_node();

    std::map<std::string, rapidxml::xml_node<> *> nodes_table;

    bool useStdout = endPosition(AO.output) == 0;
    bool useAllGenes = endPosition(AO.gene) == 0;

    if (useAllGenes)
    {
        nodes_table = getIdTable(my_root_node);
    }
    else
    {
        nodes_table = getIdTable(my_root_node, toCString(AO.gene));
    }

    if (AO.verbose)
    {
        std::cout << "nodes_table.size() = " << nodes_table.size() << std::endl;
    }

    
    std::stringstream output_stream;

    if (AO.icelandic && !AO.features)
    {
        std::vector<std::string> six_digit_icelandic_alleles = getSixDigitIcelandicAlleles(nodes_table);
        std::vector<std::string>::iterator it;

        for (it_type my_iterator = nodes_table.begin(); my_iterator != nodes_table.end(); ++my_iterator)
        {
            it = find (six_digit_icelandic_alleles.begin(), six_digit_icelandic_alleles.end(), my_iterator->first);
            if (it != six_digit_icelandic_alleles.end())
            {
                // Element was found
                output_stream << "> " << my_iterator->first << std::endl;
                output_stream << getFullSequence((char*)&my_iterator->first[0u], nodes_table) << std::endl;
            }
        }
    }
    else if (AO.icelandic && AO.features)
    {
        std::vector<std::string> six_digit_icelandic_alleles = getSixDigitIcelandicAlleles(nodes_table);
        std::vector<std::string>::iterator it;

        for (it_type my_iterator = nodes_table.begin(); my_iterator != nodes_table.end(); ++my_iterator)
        {
            it = find (six_digit_icelandic_alleles.begin(), six_digit_icelandic_alleles.end(), my_iterator->first);
            if (it != six_digit_icelandic_alleles.end())
            {
                // Element was found
                std::string allele_id_str(my_iterator->first);
                addToOutput(allele_id_str, nodes_table, output_stream);
            }
        }
    }
    else if (AO.features)
    {
        std::string prefix("HLA-");
        for (it_type my_iterator = nodes_table.begin(); my_iterator != nodes_table.end(); ++my_iterator)
        {
            std::string allele_id_str(my_iterator->first);
            // std::cout << "TEST: " << allele_id_str << std::endl;
            if (!allele_id_str.compare(0, prefix.size(), prefix))
            {
                addToOutput(allele_id_str, nodes_table, output_stream);
            }
        }
    }
    else
    {
        for (it_type my_iterator = nodes_table.begin() ; my_iterator != nodes_table.end() ; ++my_iterator)
        {
            output_stream << "> " << my_iterator->first << std::endl;
            output_stream << getFullSequence((char*)&my_iterator->first[0u], nodes_table) << std::endl;
        }
    }
    
    if (useStdout)
    {
        std::cout << output_stream.str();
    }
    else
    {
        writeToFile(output_stream, toCString(AO.output));
    }
}
