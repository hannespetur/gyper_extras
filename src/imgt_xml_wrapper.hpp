#ifndef __ImgtXMLParser_HPP_INCLUDED__
#define __ImgtXMLParser_HPP_INCLUDED__

#include <rapidxml-1.13/rapidxml.hpp>
#include <rapidxml-1.13/rapidxml_print.hpp>
#include <rapidxml-1.13/rapidxml_utils.hpp>

// Help
char* deblank(char *str);

// Hashmaps
std::map<std::string, rapidxml::xml_node<> *> getIdTable (rapidxml::xml_node<> * my_root_node);
std::map<std::string, rapidxml::xml_node<> *> getIdTable (rapidxml::xml_node<> * my_root_node, std::string from_genes);
std::map<std::string, rapidxml::xml_node<> *> getAllNodes (rapidxml::xml_node<> * my_root_node);
std::map<std::string, rapidxml::xml_node<> *> getAllNodes (rapidxml::xml_node<> * my_root_node, std::string from_genes);

// Alleles
rapidxml::xml_node<> * getAllele( char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table);

// Sequences
char* getSequence( rapidxml::xml_node<> * allele );
std::string getFullSequence( rapidxml::xml_node<> * allele , std::map<std::string, rapidxml::xml_node<> *> const & id_table );
char* getSequenceId ( rapidxml::xml_node<> * allele );

// References
char* getReference( rapidxml::xml_node<> * allele );
char* getReferenceId ( rapidxml::xml_node<> * allele, std::map<std::string, rapidxml::xml_node<> *> const & id_table );
char* getReferenceId ( char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table );
rapidxml::xml_node<> * getReferenceAllele( rapidxml::xml_node<> * allele, std::map<std::string, rapidxml::xml_node<> *> const & id_table );

char* getReferenceSequence( rapidxml::xml_node<> * reference_allele, std::map<std::string, rapidxml::xml_node<> *> const & id_table );

// Features
rapidxml::xml_node<> * getFeature( rapidxml::xml_node<> * allele , char* name);
int getFeatureStart(rapidxml::xml_node<> * feature);
int getFeatureEnd(rapidxml::xml_node<> * feature);
int getFeatureOrder(rapidxml::xml_node<> * feature);

// Public
bool exists( char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table );
char* getSequence( char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table );
std::vector<std::string> getAllFullSequences (rapidxml::xml_node<> * my_root_node, std::map<std::string, rapidxml::xml_node<> *> const & id_table);
char* getReference( char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table );
std::string getFullFeature (char const * id, std::map<std::string,
														rapidxml::xml_node<> *> const & id_table,
														std::string feature_name
													 );
std::string getFullSequence(char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table);
std::vector<std::string> getFeatureNames(char const * id, std::map<std::string, rapidxml::xml_node<> *> const & id_table);

#endif