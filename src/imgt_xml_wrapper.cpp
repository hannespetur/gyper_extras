#include <map>
#include <iostream>
#include <cstddef>

#include <string>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "imgt_xml_wrapper.hpp"

#include <rapidxml-1.13/rapidxml.hpp>
#include <rapidxml-1.13/rapidxml_print.hpp>
#include <rapidxml-1.13/rapidxml_utils.hpp>


// Help
char*
deblank(char *str)
{
    char *out = str, *put = str;

    for(; *str != '\0'; ++str)
    {
        if(*str != ' ') *put++ = *str;
    }
    *put = '\0';

    return out;
}


// ID to node tables
std::map<std::string, rapidxml::xml_node<> *>
getIdTable(rapidxml::xml_node<> * my_root_node)
{
    return getIdTable(my_root_node, std::string("All"));
}


std::map<std::string, rapidxml::xml_node<> *>
getIdTable(rapidxml::xml_node<> * my_root_node, std::string from_genes)
{
    /**
     * getIdTable has every id, name and subname as key for all nodes
     */
    std::map<std::string, rapidxml::xml_node<> *> id_table;
    for ( rapidxml::xml_node<> * allele = my_root_node->first_node() ; allele ; allele = allele->next_sibling() ) {
        if (from_genes == std::string("All") || from_genes == std::string (allele->first_node("locus")->first_attribute("locusname")->value())) {
            std::string name(deblank( allele->first_attribute("name")->value() ));
            std::string id(deblank( allele->first_attribute("id")->value() ));

            id_table[name] = allele;
            id_table[id] = allele;

            // while (name.length() > 4) {
            //     if (id_table.count(name) != 0) {
            //         break;
            //     }

            //     id_table[name] = allele;

            //     if (isdigit(name[name.length()-1])) {
            //         name = name.substr(0,name.length()-3);
            //     }
            //     else {
            //         name = name.substr(0,name.length()-4);
            //     }

            //     if(!isdigit(name[name.length()-1])) {
            //         name = name.substr(0,name.length()-1);
            //     }
            // }
        }
    }
    return id_table;
}


std::map<std::string, rapidxml::xml_node<> *>
getAllNodes (rapidxml::xml_node<> * my_root_node)
{
    return getAllNodes (my_root_node, std::string("All"));
}


std::map<std::string, rapidxml::xml_node<> *>
getAllNodes (rapidxml::xml_node<> * my_root_node,
             std::string from_genes
            )
{
    /**
     * getAllNodes has id as a key for all nodes. Good if you want to get all nodes without getting duplicates
     */
    std::map<std::string, rapidxml::xml_node<> *> node_table;
    for ( rapidxml::xml_node<> * allele = my_root_node->first_node() ; allele ; allele = allele->next_sibling() )
    {
        if (from_genes == "All" ||
            from_genes == std::string (allele->first_node("locus")->first_attribute("locusname")->value())
           ) {
            std::string id(deblank( allele->first_attribute("id")->value() ));
            node_table[id] = allele;
        }
    }
    return node_table;
}


// Alleles
rapidxml::xml_node<> *
getAllele (char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table)
{
    std::string tmp(id);
    if ( id_table.count(tmp) == 1 ) {
        return id_table.at(tmp);
    }
    return 0;
}


// Sequences
char*
getSequence (rapidxml::xml_node<> * allele )
{
    return allele->first_node("sequence")->first_node("nucsequence")->value();
}


std::string
getFullSequence (rapidxml::xml_node<> * allele, std::map<std::string, rapidxml::xml_node<> *> const & id_table)
{
    return getFullSequence(getSequenceId(allele), id_table);
}


std::vector<std::string>
getAllFullSequences (rapidxml::xml_node<> * my_root_node, std::map<std::string, rapidxml::xml_node<> *> const & id_table)
{
    std::vector<std::string> sequences;
    sequences.reserve(id_table.size());
    rapidxml::xml_node<> * allele = my_root_node->first_node();

    while (allele) {
        std::string fullSequence (getFullSequence(allele, id_table));

        if (fullSequence != std::string ("Id not found!"))
            sequences.push_back(fullSequence);

        allele = allele->next_sibling();
    }
    
    return sequences;
}


char*
getSequenceId (rapidxml::xml_node<> * allele )
{
    return allele->first_attribute("id")->value();
}


// References
char*
getReference (rapidxml::xml_node<> * allele )
{
    return allele->first_node("sequence")->first_node("alignmentreference")->first_attribute("alleleid")->value();
}


char*
getReferenceId (rapidxml::xml_node<> * allele, std::map<std::string, rapidxml::xml_node<> *> const & id_table )
{
    return getSequenceId(getAllele(getReference(allele), id_table));
}


char*
getReferenceId (char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table)
{
    return getReferenceId( getAllele(id, id_table ),  id_table);
}


rapidxml::xml_node<> *
getReferenceAllele (rapidxml::xml_node<> * allele, std::map<std::string, rapidxml::xml_node<> *> const & id_table )
{
    return getAllele( getReferenceId(allele, id_table ), id_table );
}


char*
getReferenceSequence (rapidxml::xml_node<> * reference_allele, std::map<std::string, rapidxml::xml_node<> *> const & id_table)
{
    return getSequence(getReference(reference_allele), id_table);
}


// Features
rapidxml::xml_node<> *
getFeature (rapidxml::xml_node<> * allele , char* name)
{
    for (rapidxml::xml_node<> * feature = allele->first_node("sequence")->first_node("feature") ; feature ; feature = feature->next_sibling())
    {
        if ( strcmp(feature->first_attribute("name")->value(), name) == 0 )
        {
            return feature;
        }
    }
    std::cerr << "Feature " << name << " was not found!" << std::endl;
    return allele;
}


int
getFeatureStart (rapidxml::xml_node<> * feature)
{
    return atoi(feature->first_node("SequenceCoordinates")->first_attribute("start")->value())-1;
}


int
getFeatureEnd (rapidxml::xml_node<> * feature)
{
    return atoi(feature->first_node("SequenceCoordinates")->first_attribute("end")->value());
}


bool
exists ( char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table ){
    return getAllele(id, id_table ) != 0;
}


char*
getSequence (char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table )
{
    rapidxml::xml_node<> * allele = getAllele(id, id_table);

    if ( allele )
    {
        return getSequence(allele);
    } else {
        return (char*) "Id not found!";
    }
}


char*
getReference (char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table )
{
    rapidxml::xml_node<> * allele = getAllele(id, id_table );

    if ( allele )
    {
        return getSequence(getReference(allele), id_table);
    }
    else
    {
        return (char*) "Id not found!";
    }
}


bool
hasReference(rapidxml::xml_node<> * allele)
{
    if(allele->first_node("sequence"))
        return true;
    return false;
}

int
getFeatureOrder(rapidxml::xml_node<> * feature)
{
    if (feature)
    {
        return atoi ( feature->first_attribute("order")->value());
    }

    // As long as no allele has more than 9999999 features this is fine
    return 9999999;
}


std::vector<std::string>
getFeatureNames(char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table)
{
    rapidxml::xml_node<> * allele = getAllele(id, id_table);
    std::vector<std::string> all_features;

    if(!exists(id,id_table) || !allele || !hasReference(allele))
    {
        all_features.push_back("Id not found!");
        return all_features;
    }

    rapidxml::xml_node<> * reference_allele = getReferenceAllele(allele, id_table);

    for (rapidxml::xml_node<> * feature = reference_allele->first_node("sequence")->first_node("feature") ; feature ; feature = feature->next_sibling())
    {
        if (strcmp(feature->first_attribute("featuretype")->value(), "Protein") == 0)
            break;

        // // Skip alleles with partial sequences
        // if (feature->first_attribute("status"))
        // {
        //     std::cout << "Found status " << feature->first_attribute("status")->value() << std::endl;

        //     if (strcmp(feature->first_attribute("status")->value(), "Partial") == 0)
        //     {
        //         std::cout << "INFO: Skipping " << feature->first_attribute("name")->value() << std::endl;
        //         break;
        //     }
        // }

        all_features.push_back( feature->first_attribute("name")->value() );
    }

    if (all_features.front() != std::string("5' UTR"))
    {
        all_features.insert(all_features.begin(), std::string("5' UTR"));
    }

    if (all_features.back() != std::string("3' UTR"))
    {
        all_features.push_back(std::string("3' UTR"));
    }

    return all_features;
}


rapidxml::xml_node<> *
hasFeature (rapidxml::xml_node<> * allele, std::string feature_name)
{
    for (rapidxml::xml_node<> * feature = allele->first_node("sequence")->first_node("feature") ; feature ; feature = feature->next_sibling())
    {
        if (strcmp(feature->first_attribute("name")->value(), feature_name.c_str() ) == 0)
            return feature;
    }
    return 0;
}


std::string
getFeatureSequence (rapidxml::xml_node<> * allele,
                    rapidxml::xml_node<> * feature
                   )
{
    int start = getFeatureStart(feature);
    int end = getFeatureEnd(feature);

    std::string seq(getSequence(allele));

    return seq.substr(start, end-start);
}


std::string
getFullFeature (char const * id,
                std::map<std::string, rapidxml::xml_node<> *> const & id_table,
                std::string feature_name
               )
{
    rapidxml::xml_node<> * allele = getAllele(id, id_table);

    if ( !exists(id,id_table) || !allele || !hasReference(allele) )
        return "Id not found!";

    rapidxml::xml_node<> * feature = hasFeature(allele, feature_name);
    
    if (feature) {
        return getFeatureSequence(allele, feature);
    }
    else {
        rapidxml::xml_node<> * reference_allele = getReferenceAllele(allele, id_table);
        feature = hasFeature(reference_allele, feature_name);
        
        if (!feature) {
            std::cerr << id << " has no feature named " << feature_name << std::endl;
            return "0";
        }

        return getFeatureSequence(reference_allele, feature);
    }
}


std::string
getFullSequence (char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table)
{
    rapidxml::xml_node<> * allele = getAllele(id, id_table);

    if(!exists(id,id_table) || !allele || !hasReference(allele)) {
        return "Id not found!";
    }

    rapidxml::xml_node<> * reference_allele = getReferenceAllele(allele, id_table);

    if (allele == reference_allele) {
        return getSequence(allele);
    }
    
    std::string seq(getSequence(allele));
    std::string ref(getSequence(reference_allele));

    {
        std::string const translation("Translation");
        rapidxml::xml_node<> * seq_feature = allele->first_node("sequence")->first_node("feature");
        rapidxml::xml_node<> * ref_feature = reference_allele->first_node("sequence")->first_node("feature");

        if (seq_feature->first_attribute("status") and strcmp(seq_feature->first_attribute("status")->value(), "Partial") == 0)
        {
            std::cout << "Skipping " << seq_feature->first_attribute("name")->value() << std::endl;
            return "Id not found!";
        }

        int seq_pos = 0;
        int ref_pos = 0;

        while (seq_feature &&
               std::string(seq_feature->first_attribute("name")->value()) != translation
              )
        {
            if (seq_feature->first_attribute("status") and strcmp(seq_feature->first_attribute("status")->value(), "Partial") == 0)
            {
                std::cout << "Skipping " << seq_feature->first_attribute("name")->value() << std::endl;
                return "Id not found!";
            }

            if (ref_pos == ref.size())
            {
                int seq_start = getFeatureStart(seq_feature);
                int seq_end = getFeatureEnd(seq_feature);

                ref.append(seq.substr(seq_start));
                break;
            }

            int seq_order = getFeatureOrder(seq_feature);
            int ref_order = getFeatureOrder(ref_feature);

            int seq_start = getFeatureStart(seq_feature);
            int seq_end = getFeatureEnd(seq_feature);

            if (seq_order == ref_order)
            {
                int ref_start = getFeatureStart(ref_feature);
                int ref_end = getFeatureEnd(ref_feature);

                ref.replace(ref_pos, ref_end-ref_start, seq, seq_start, seq_end-seq_start);
                seq_pos += seq_end-seq_start;
                ref_pos += seq_end-seq_start;

                seq_feature = seq_feature->next_sibling();
                ref_feature = ref_feature->next_sibling();
            }
            else if (seq_order < ref_order)
            {
                ref.replace(ref_pos, 0, seq, seq_start, seq_end-seq_start);
                ref_pos += seq_end-seq_start;
                seq_feature = seq_feature->next_sibling();
            }
            else 
            {
                int ref_start = getFeatureStart(ref_feature);
                int ref_end = getFeatureEnd(ref_feature);

                ref_pos += ref_end-ref_start;

                ref_feature = ref_feature->next_sibling();
            }
        }
    }
    return ref;
}
