#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

#include "imgt_xml_wrapper.hpp"

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <rapidxml-1.13/rapidxml.hpp>
#include <rapidxml-1.13/rapidxml_print.hpp>
#include <rapidxml-1.13/rapidxml_utils.hpp>

/*******************************************************************

Hopufully there will be a test case for every method using the dummy.xml file.

Also a test case for every method that is expected to be used by external
applications using the hla.xml file (tested on release 3.20.0).

********************************************************************/


/**
 * Parsing and reading data
 */
TEST_CASE("dummy.xml can be parsed and read", "[dummy.xml]")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  rapidxml::file<> xmlFile(xml);
  rapidxml::xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  rapidxml::xml_node<> * my_root_node = doc.first_node();

  REQUIRE(strcmp(my_root_node->first_attribute("xmlns")->value(), "http://hla.alleles.org/xml") == 0);
}


TEST_CASE("hla.xml can be parsed and read", "[hla.xml]")
{
  using namespace rapidxml;

  char * xml = (char *)"hla.xml";
  rapidxml::file<> xmlFile(xml);
  rapidxml::xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  rapidxml::xml_node<> * my_root_node = doc.first_node();

  REQUIRE(strcmp(my_root_node->first_attribute("xmlns")->value(), "http://hla.alleles.org/xml") == 0);
}


/**
 * getIdTable method creates a hashmap with id, names and subnames to an alleles node, many to 1 relation.
 */
TEST_CASE("getIdTable should work using ids, names and subnames on dummy.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, rapidxml::xml_node<> *> id_table = getIdTable(my_root_node);

  SECTION("getIdTable should return a table with every id exactly once")
  {
    REQUIRE(id_table.size() >= 6);

    REQUIRE(id_table.count("T1") == 1);
    REQUIRE(id_table.count("T2") == 1);
    REQUIRE(id_table.count("T3") == 1);
    REQUIRE(id_table.count("T4") == 1);
    REQUIRE(id_table.count("HLA00499") == 1);
    REQUIRE(id_table.count("HLA06604") == 1);
  }

  SECTION("getIdTable should return the correct allele using every id")
  {
    REQUIRE(strcmp(id_table["T1"]->first_attribute("name")->value(), "TST-A*01:01:01:01") == 0);
    REQUIRE(strcmp(id_table["T2"]->first_attribute("name")->value(), "TST-A*01:01:01:02") == 0);
    REQUIRE(strcmp(id_table["T3"]->first_attribute("name")->value(), "TST-A*01:01:03") == 0);
    REQUIRE(strcmp(id_table["T4"]->first_attribute("name")->value(), "TST-A*01:04") == 0);
    REQUIRE(strcmp(id_table["HLA00499"]->first_attribute("name")->value(), "HLA-DPA1*01:03:01:01") == 0);
    REQUIRE(strcmp(id_table["HLA06604"]->first_attribute("name")->value(), "HLA-DPA1*01:03:01:02") == 0);
  }

  SECTION("getIdTable should return a table with every name exactly once")
  {
    REQUIRE(id_table.count("TST-A*01:01:01:01") == 1);
    REQUIRE(id_table.count("TST-A*01:01:01:02") == 1);
    REQUIRE(id_table.count("TST-A*01:01:03") == 1);
    REQUIRE(id_table.count("TST-A*01:04") == 1);
    REQUIRE(id_table.count("HLA-DPA1*01:03:01:01") == 1);
    REQUIRE(id_table.count("HLA-DPA1*01:03:01:02") == 1);
  }

  SECTION("getIdTable should return the correct allele using every name")
  {
    REQUIRE(strcmp(id_table["TST-A*01:01:01:01"]->first_attribute("name")->value(), "TST-A*01:01:01:01") == 0);
    REQUIRE(strcmp(id_table["TST-A*01:01:01:02"]->first_attribute("name")->value(), "TST-A*01:01:01:02") == 0);
    REQUIRE(strcmp(id_table["TST-A*01:01:03"]->first_attribute("name")->value(), "TST-A*01:01:03") == 0);
    REQUIRE(strcmp(id_table["TST-A*01:04"]->first_attribute("name")->value(), "TST-A*01:04") == 0);
    REQUIRE(strcmp(id_table["HLA-DPA1*01:03:01:01"]->first_attribute("name")->value(), "HLA-DPA1*01:03:01:01") == 0);
    REQUIRE(strcmp(id_table["HLA-DPA1*01:03:01:02"]->first_attribute("name")->value(), "HLA-DPA1*01:03:01:02") == 0);
  }

  // SECTION ("getIdTable should return a table with subnames exactly once")
  // {
  //  REQUIRE (id_table.count("TST-A*01:01:01") == 1);
  //  REQUIRE (id_table.count("TST-A*01:01") == 1);
  //  REQUIRE (id_table.count("TST-A*01") == 1);
  //  REQUIRE (id_table.count("HLA-DPA1*01:03:01") == 1);
  //  REQUIRE (id_table.count("HLA-DPA1*01:03") == 1);
  //  REQUIRE (id_table.count("HLA-DPA1*01") == 1);
  // }

  // SECTION ("getIdTable should return the correct allele using subnames")
  // {
  //  REQUIRE (strcmp (id_table["TST-A*01:01:01"]->first_attribute("name")->value(), "TST-A*01:01:01:01") == 0);
  //  REQUIRE (strcmp (id_table["TST-A*01:01:01"]->first_attribute("name")->value(), "TST-A*01:01:01:01") == 0);
  //  REQUIRE (strcmp (id_table["TST-A*01:01"]->first_attribute("name")->value(), "TST-A*01:01:01:01") == 0);
  //  REQUIRE (strcmp (id_table["TST-A*01"]->first_attribute("name")->value(), "TST-A*01:01:01:01") == 0);
  //  REQUIRE (strcmp (id_table["HLA-DPA1*01:03:01"]->first_attribute("name")->value(), "HLA-DPA1*01:03:01:01") == 0);
  //  REQUIRE (strcmp (id_table["HLA-DPA1*01:03"]->first_attribute("name")->value(), "HLA-DPA1*01:03:01:01") == 0);
  //  REQUIRE (strcmp (id_table["HLA-DPA1*01"]->first_attribute("name")->value(), "HLA-DPA1*01:03:01:01") == 0);
  // }

  SECTION("getIdTable should return a table that doesn't have anything that is not ids, names or subnames")
  {
    REQUIRE(id_table.count("T0") == 0);
    REQUIRE(id_table.count("aslkdjaf") == 0);
    REQUIRE(id_table.count("T1asdfæal") == 0);

    REQUIRE(id_table.count("HLA00001123123") == 0);
    REQUIRE(id_table.count("HLA00112332") == 0);

    REQUIRE(id_table.count("TST-A*01:04:01") == 0);
    REQUIRE(id_table.count("TST-A*01:01:01:01ads") == 0);
    REQUIRE(id_table.count("TST-A*01:01:01:") == 0);
    REQUIRE(id_table.count("TST-A*01:01:0") == 0);
    REQUIRE(id_table.count("TST-A*01:01:") == 0);
    REQUIRE(id_table.count("TST-A*01:0") == 0);
    REQUIRE(id_table.count("TST-A*01:") == 0);
    REQUIRE(id_table.count("TST-A*") == 0);
    REQUIRE(id_table.count("TST-A*0") == 0);
    REQUIRE(id_table.count("TST-A") == 0);
    REQUIRE(id_table.count("TST-") == 0);
    REQUIRE(id_table.count("TST") == 0);
    REQUIRE(id_table.count("TS") == 0);
    REQUIRE(id_table.count("T") == 0);
    REQUIRE(id_table.count("") == 0);
  }
}


TEST_CASE("getIdTable should work using ids, names and subnames on hla.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"hla.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, rapidxml::xml_node<> *> id_table = getIdTable(my_root_node);

  SECTION("getIdTable should return a table with every id exactly once")
  {
    REQUIRE(id_table.count("HLA00001") == 1);
    REQUIRE(id_table.count("HLA02169") == 1);
    REQUIRE(id_table.count("HLA01244") == 1);
    REQUIRE(id_table.count("HLA01971") == 1);
  }

  SECTION("getIdTable should return the correct allele using every id")
  {
    REQUIRE(strcmp(id_table["HLA00001"]->first_attribute("name")->value(), "HLA-A*01:01:01:01") == 0);
    REQUIRE(strcmp(id_table["HLA02169"]->first_attribute("name")->value(), "HLA-A*01:01:01:02N") == 0);
    REQUIRE(strcmp(id_table["HLA01244"]->first_attribute("name")->value(), "HLA-A*01:01:02") == 0);
    REQUIRE(strcmp(id_table["HLA01971"]->first_attribute("name")->value(), "HLA-A*01:01:03") == 0);
  }

  SECTION("getIdTable should return a table with every name exactly once")
  {
    REQUIRE(id_table.count("HLA-A*01:01:01:01") == 1);
    REQUIRE(id_table.count("HLA-A*01:01:01:02N") == 1);
    REQUIRE(id_table.count("HLA-A*01:01:02") == 1);
    REQUIRE(id_table.count("HLA-A*01:01:03") == 1);
  }

  SECTION("getIdTable should return the correct allele using every name")
  {
    REQUIRE(strcmp(id_table["HLA-A*01:01:01:01"]->first_attribute("name")->value(), "HLA-A*01:01:01:01") == 0);
    REQUIRE(strcmp(id_table["HLA-A*01:01:01:02N"]->first_attribute("name")->value(), "HLA-A*01:01:01:02N") == 0);
    REQUIRE(strcmp(id_table["HLA-A*01:01:02"]->first_attribute("name")->value(), "HLA-A*01:01:02") == 0);
    REQUIRE(strcmp(id_table["HLA-A*01:01:03"]->first_attribute("name")->value(), "HLA-A*01:01:03") == 0);
  }

  // SECTION ("getIdTable should return a table with subnames exactly once")
  // {
  //  REQUIRE (id_table.count("HLA-A*01:01:01") == 1);
  //  REQUIRE (id_table.count("HLA-A*01:01") == 1);
  //  REQUIRE (id_table.count("HLA-A*01") == 1);
  // }

  // SECTION ("getIdTable should return the correct allele using subnames")
  // {
  //  REQUIRE (strcmp (id_table["HLA-A*01:01:01"]->first_attribute("name")->value(),"HLA-A*01:01:01:01") == 0);
  //  REQUIRE (strcmp (id_table["HLA-A*01:01"]->first_attribute("name")->value(),"HLA-A*01:01:01:01") == 0);
  //  REQUIRE (strcmp (id_table["HLA-A*01"]->first_attribute("name")->value(),"HLA-A*01:01:01:01") == 0);
  // }

  SECTION("getIdTable should return a table that doesn't have anything that is not ids, names or subnames")
  {
    REQUIRE(id_table.count("T0") == 0);
    REQUIRE(id_table.count("T1asdfæal") == 0);
    REQUIRE(id_table.count("T1") == 0);

    REQUIRE(id_table.count("HLA00001123123") == 0);
    REQUIRE(id_table.count("HLA00112332") == 0);

    REQUIRE(id_table.count("HLA-A*01:01:01:") == 0);
    REQUIRE(id_table.count("HLA-A*01:01:0") == 0);
    REQUIRE(id_table.count("HLA-A*01:01:") == 0);
    REQUIRE(id_table.count("HLA-A*01:0") == 0);
    REQUIRE(id_table.count("HLA-A*01:") == 0);
    REQUIRE(id_table.count("HLA-A*") == 0);
    REQUIRE(id_table.count("HLA-A*0") == 0);
    REQUIRE(id_table.count("HLA-A") == 0);
    REQUIRE(id_table.count("HLA-") == 0);
    REQUIRE(id_table.count("HLA") == 0);
    REQUIRE(id_table.count("HL") == 0);
    REQUIRE(id_table.count("H") == 0);
    REQUIRE(id_table.count("") == 0);
  }
}


TEST_CASE("getIdTable should work when specifying which genes I want on dummy.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  SECTION("it should work with genes that are in the xml")
  {
    std::map<std::string, rapidxml::xml_node<> *> id_table = getIdTable(my_root_node, std::string("test"));

    REQUIRE(id_table.size() >= 4);
    REQUIRE(id_table.count("T1") == 1);
    REQUIRE(id_table.count("T2") == 1);
    REQUIRE(id_table.count("T3") == 1);
    REQUIRE(id_table.count("T4") == 1);
    REQUIRE(id_table.count("HLA00499") == 0);
    REQUIRE(id_table.count("HLA06604") == 0);

    std::map<std::string, rapidxml::xml_node<> *> another_id_table = getIdTable(my_root_node, std::string("HLA-DPA1"));

    REQUIRE(another_id_table.size() >= 2);
    REQUIRE(another_id_table.count("T1") == 0);
    REQUIRE(another_id_table.count("T2") == 0);
    REQUIRE(another_id_table.count("T3") == 0);
    REQUIRE(another_id_table.count("T4") == 0);
    REQUIRE(another_id_table.count("HLA00499") == 1);
    REQUIRE(another_id_table.count("HLA06604") == 1);
  }

  SECTION("it should return an empty hashmap if I pick a non-existent gene in the xml")
  {
    std::map<std::string, rapidxml::xml_node<> *> id_table = getIdTable(my_root_node, std::string("unkown_gene"));

    REQUIRE(id_table.size() == 0);

    REQUIRE(id_table.count("T1") == 0);
    REQUIRE(id_table.count("T2") == 0);
    REQUIRE(id_table.count("T3") == 0);
    REQUIRE(id_table.count("T4") == 0);
    REQUIRE(id_table.count("HLA00499") == 0);
    REQUIRE(id_table.count("HLA06604") == 0);
  }

  std::map<std::string, rapidxml::xml_node<> *> id_table = getIdTable(my_root_node, std::string("test"));

  SECTION("it should work with names")
  {
    REQUIRE(id_table.count("TST-A*01:01:01:01") == 1);
    REQUIRE(id_table.count("TST-A*01:01:01:02") == 1);
    REQUIRE(id_table.count("TST-A*01:01:03") == 1);
    REQUIRE(id_table.count("TST-A*01:04") == 1);
  }

  // SECTION ("it should also work with subnames")
  // {
  //  REQUIRE (id_table.count("TST-A*01:01:01") == 1);
  //  REQUIRE (id_table.count("TST-A*01:01") == 1);
  //  REQUIRE (id_table.count("TST-A*01") == 1);
  // }

  SECTION("it shouldn't work otherwise")
  {
    REQUIRE(id_table.count("TST-A*01:01:01:") == 0);
    REQUIRE(id_table.count("TST-A*01:01:") == 0);
    REQUIRE(id_table.count("TST-A*01:") == 0);
    REQUIRE(id_table.count("TST-A") == 0);
    REQUIRE(id_table.count("TST-") == 0);
    REQUIRE(id_table.count("TST") == 0);
    REQUIRE(id_table.count("TS") == 0);
    REQUIRE(id_table.count("T") == 0);
    REQUIRE(id_table.count("") == 0);
  }
}


/**
 * getAllNodes creates a hashmap with id to an alleles node, 1 to 1 relation.
 */
TEST_CASE("getAllNodes should work using ids only on dummy.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  SECTION("getAllNodes should return a table with every id exactly once")
  {
    REQUIRE(all_nodes.count("T1") == 1);
    REQUIRE(all_nodes.count("T2") == 1);
    REQUIRE(all_nodes.count("T3") == 1);
    REQUIRE(all_nodes.count("T4") == 1);
  }

  SECTION("getAllNodes should return a pointer to a the correct allele")
  {
    REQUIRE(strcmp(all_nodes["T1"]->first_attribute("id")->value(), "T1") == 0);
    REQUIRE(strcmp(all_nodes["T2"]->first_attribute("id")->value(), "T2") == 0);
    REQUIRE(strcmp(all_nodes["T3"]->first_attribute("id")->value(), "T3") == 0);
    REQUIRE(strcmp(all_nodes["T4"]->first_attribute("id")->value(), "T4") == 0);
  }

  SECTION("getAllNodes should return a table that doesn't have anything that is not ids")
  {
    REQUIRE(all_nodes.count("T0") == 0);
    REQUIRE(all_nodes.count("aslkdjaf") == 0);
    REQUIRE(all_nodes.count("T1asdfæal") == 0);

    REQUIRE(all_nodes.count("HLA00001123123") == 0);
    REQUIRE(all_nodes.count("HLA00112332") == 0);

    // Subnames are not in all_nodes!
    REQUIRE(all_nodes.count("TST-A*01:04:01") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01:01:01ads") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01:01:") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01:0") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01:") == 0);
    REQUIRE(all_nodes.count("TST-A*01:0") == 0);
    REQUIRE(all_nodes.count("TST-A*01:") == 0);
    REQUIRE(all_nodes.count("TST-A*") == 0);
    REQUIRE(all_nodes.count("TST-A*0") == 0);
    REQUIRE(all_nodes.count("TST-A") == 0);
    REQUIRE(all_nodes.count("TST-") == 0);
    REQUIRE(all_nodes.count("TST") == 0);
    REQUIRE(all_nodes.count("TS") == 0);
    REQUIRE(all_nodes.count("T") == 0);
    REQUIRE(all_nodes.count("") == 0);

    // Names are not in all_nodes!
    REQUIRE(all_nodes.count("TST-A*01:01:01:01") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01:01:02") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01:03") == 0);
    REQUIRE(all_nodes.count("TST-A*01:04") == 0);
  }
}

TEST_CASE("getAllNodes should work when specifying which genes I want on dummy.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  SECTION("it should work with genes that are in the xml")
  {
    std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node, std::string("test"));

    REQUIRE(all_nodes.size() >= 4);
    REQUIRE(all_nodes.count("T1") == 1);
    REQUIRE(all_nodes.count("T2") == 1);
    REQUIRE(all_nodes.count("T3") == 1);
    REQUIRE(all_nodes.count("T4") == 1);
    REQUIRE(all_nodes.count("HLA00499") == 0);
    REQUIRE(all_nodes.count("HLA06604") == 0);

    std::map<std::string, rapidxml::xml_node<> *> another_all_nodes = getAllNodes(my_root_node, std::string("HLA-DPA1"));

    REQUIRE(another_all_nodes.size() >= 2);
    REQUIRE(another_all_nodes.count("T1") == 0);
    REQUIRE(another_all_nodes.count("T2") == 0);
    REQUIRE(another_all_nodes.count("T3") == 0);
    REQUIRE(another_all_nodes.count("T4") == 0);
    REQUIRE(another_all_nodes.count("HLA00499") == 1);
    REQUIRE(another_all_nodes.count("HLA06604") == 1);
  }

  SECTION("it should return an empty hashmap if I pick a non-existent gene in the xml")
  {
    std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node, std::string("unkown_gene"));

    REQUIRE(all_nodes.size() == 0);

    REQUIRE(all_nodes.count("T1") == 0);
    REQUIRE(all_nodes.count("T2") == 0);
    REQUIRE(all_nodes.count("T3") == 0);
    REQUIRE(all_nodes.count("T4") == 0);
    REQUIRE(all_nodes.count("HLA00499") == 0);
    REQUIRE(all_nodes.count("HLA06604") == 0);
  }

  SECTION("it shouldn't work otherwise")
  {
    std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node, std::string("test"));

    REQUIRE(all_nodes.count("TST-A*01:01:01:01") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01:01:02") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01:03") == 0);
    REQUIRE(all_nodes.count("TST-A*01:04") == 0);

    REQUIRE(all_nodes.count("TST-A*01:01:01") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01") == 0);
    REQUIRE(all_nodes.count("TST-A*01") == 0);

    REQUIRE(all_nodes.count("TST-A*01:01:01:") == 0);
    REQUIRE(all_nodes.count("TST-A*01:01:") == 0);
    REQUIRE(all_nodes.count("TST-A*01:") == 0);
    REQUIRE(all_nodes.count("TST-A") == 0);
    REQUIRE(all_nodes.count("TST-") == 0);
    REQUIRE(all_nodes.count("TST") == 0);
    REQUIRE(all_nodes.count("TS") == 0);
    REQUIRE(all_nodes.count("T") == 0);
    REQUIRE(all_nodes.count("") == 0);
  }
}


TEST_CASE("getAllNodes should work using ids only on hla.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"hla.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  SECTION("getIdTable should return a table with every id exactly once")
  {
    REQUIRE(all_nodes.count("HLA00001") == 1);
    REQUIRE(all_nodes.count("HLA02169") == 1);
    REQUIRE(all_nodes.count("HLA01244") == 1);
    REQUIRE(all_nodes.count("HLA01971") == 1);
  }

  SECTION("getAllNodes should return a pointer to a the correct allele")
  {
    REQUIRE(strcmp(all_nodes["HLA00001"]->first_attribute("id")->value(), "HLA00001") == 0);
    REQUIRE(strcmp(all_nodes["HLA02169"]->first_attribute("id")->value(), "HLA02169") == 0);
    REQUIRE(strcmp(all_nodes["HLA01244"]->first_attribute("id")->value(), "HLA01244") == 0);
    REQUIRE(strcmp(all_nodes["HLA01971"]->first_attribute("id")->value(), "HLA01971") == 0);
  }

  SECTION("getIdTable should return a table that doesn't have anything that is not ids")
  {
    REQUIRE(all_nodes.count("T0") == 0);
    REQUIRE(all_nodes.count("T1") == 0);
    REQUIRE(all_nodes.count("T2") == 0);
    REQUIRE(all_nodes.count("T3") == 0);
    REQUIRE(all_nodes.count("T4") == 0);
    REQUIRE(all_nodes.count("aslkdjaf") == 0);
    REQUIRE(all_nodes.count("T1asdfæal") == 0);

    REQUIRE(all_nodes.count("HLA00001123123") == 0);
    REQUIRE(all_nodes.count("HLA00112332") == 0);

    // Subnames are not in all_nodes!
    REQUIRE(all_nodes.count("HLA-A*01:04:01") == 0);
    REQUIRE(all_nodes.count("HLA-A*01:01:01:01ads") == 0);
    REQUIRE(all_nodes.count("HLA-A*01:01:01:") == 0);
    REQUIRE(all_nodes.count("HLA-A*01:01:0") == 0);
    REQUIRE(all_nodes.count("HLA-A*01:01:") == 0);
    REQUIRE(all_nodes.count("HLA-A*01:0") == 0);
    REQUIRE(all_nodes.count("HLA-A*01:") == 0);
    REQUIRE(all_nodes.count("HLA-A*") == 0);
    REQUIRE(all_nodes.count("HLA-A*0") == 0);
    REQUIRE(all_nodes.count("HLA-A") == 0);
    REQUIRE(all_nodes.count("HLA-") == 0);
    REQUIRE(all_nodes.count("HLA") == 0);
    REQUIRE(all_nodes.count("HL") == 0);
    REQUIRE(all_nodes.count("H") == 0);
    REQUIRE(all_nodes.count("") == 0);

    // Names are not in all_nodes!
    REQUIRE(all_nodes.count("HLA-A*01:01:01:01") == 0);
    REQUIRE(all_nodes.count("HLA-A*01:01:01:02") == 0);
    REQUIRE(all_nodes.count("HLA-A*01:01:01:02N") == 0);
    REQUIRE(all_nodes.count("HLA-A*01:01:02") == 0);
  }
}


/**
 * getAllele method returns an pointer to a valid allele.
 */
TEST_CASE("getAllele should work using getIdTable and getAllNodes on dummy.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  char * hla_id1 = (char *)"HLA00001";
  char * hla_id2 = (char *)"HLA02169";
  char * hla_id3 = (char *)"HLA01244";
  char * hla_id4 = (char *)"HLA01971";

  SECTION("getAllele should return reference to some allele if it is used with valid ids")
  {
    char * id1 = (char *)"T1";
    char * id2 = (char *)"T2";
    char * id3 = (char *)"T3";
    char * id4 = (char *)"T4";

    REQUIRE(getAllele(id1, id_table) != 0);
    REQUIRE(getAllele(id2, id_table) != 0);
    REQUIRE(getAllele(id3, id_table) != 0);
    REQUIRE(getAllele(id4, id_table) != 0);

    REQUIRE(getAllele(id1, all_nodes) != 0);
    REQUIRE(getAllele(id2, all_nodes) != 0);
    REQUIRE(getAllele(id3, all_nodes) != 0);
    REQUIRE(getAllele(id4, all_nodes) != 0);
  }

  SECTION("getAllele should return reference to some allele with a valid name with id_table, and not all_nodes")
  {
    char * name1 = (char *)"TST-A*01:01:01:01";
    char * name2 = (char *)"TST-A*01:01:01:02";
    char * name3 = (char *)"TST-A*01:01:03";
    char * name4 = (char *)"TST-A*01:04";

    REQUIRE(getAllele(name1, id_table) != 0);
    REQUIRE(getAllele(name2, id_table) != 0);
    REQUIRE(getAllele(name3, id_table) != 0);
    REQUIRE(getAllele(name4, id_table) != 0);

    REQUIRE(getAllele(name1, all_nodes) == 0);
    REQUIRE(getAllele(name2, all_nodes) == 0);
    REQUIRE(getAllele(name3, all_nodes) == 0);
    REQUIRE(getAllele(name4, all_nodes) == 0);
  }

  // SECTION("getAllele should return reference to some allele with a subname with id_table, and not all_nodes")
  // {
  //   char * subname1 = (char *)"TST-A*01:01:01";
  //   char * subname2 = (char *)"TST-A*01:01";
  //   char * subname3 = (char *)"TST-A*01";
  //
  //   REQUIRE(getAllele(subname1, id_table) != 0);
  //   REQUIRE(getAllele(subname2, id_table) != 0);
  //   REQUIRE(getAllele(subname3, id_table) != 0);
  //
  //   REQUIRE(getAllele(subname1, all_nodes) == 0);
  //   REQUIRE(getAllele(subname2, all_nodes) == 0);
  //   REQUIRE(getAllele(subname3, all_nodes) == 0);
  // }

  SECTION("getAllele should return 0 otherwise, with id_table")
  {
    REQUIRE(getAllele((char *)"T0", id_table) == 0);
    REQUIRE(getAllele((char *)"T123", id_table) == 0);
    REQUIRE(getAllele((char *)"asdfpajsdfpdasdf", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:01:01:01", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:04:01", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:01:01ads", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:01:", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:0", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:0", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A*0", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-A", id_table) == 0);
    REQUIRE(getAllele((char *)"TST-", id_table) == 0);
    REQUIRE(getAllele((char *)"TST", id_table) == 0);
    REQUIRE(getAllele((char *)"TS", id_table) == 0);
    REQUIRE(getAllele((char *)"T", id_table) == 0);
    REQUIRE(getAllele((char *)"", id_table) == 0);
  }

  SECTION("getAllele should return 0 otherwise, with id_table")
  {
    REQUIRE(getAllele((char *)"T0", all_nodes) == 0);
    REQUIRE(getAllele((char *)"T123", all_nodes) == 0);
    REQUIRE(getAllele((char *)"asdfpajsdfpdasdf", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:01:01:01", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:04:01", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:01:01ads", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:01:", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:0", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:01:", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:0", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*01:", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A*0", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-A", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST-", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TST", all_nodes) == 0);
    REQUIRE(getAllele((char *)"TS", all_nodes) == 0);
    REQUIRE(getAllele((char *)"T", all_nodes) == 0);
    REQUIRE(getAllele((char *)"", all_nodes) == 0);
  }
}


/**
 * exists method is returns true if the id is in the hashmap.
 */
TEST_CASE("exists should work using getIdTable on dummy.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  SECTION("exists should return true for exact ids for both id_table and all_nodes")
  {
    char * id1 = (char *)"T1";
    char * id2 = (char *)"T2";
    char * id3 = (char *)"T3";
    char * id4 = (char *)"T4";

    REQUIRE(exists(id1, id_table) == 1);
    REQUIRE(exists(id2, id_table) == 1);
    REQUIRE(exists(id3, id_table) == 1);
    REQUIRE(exists(id4, id_table) == 1);

    REQUIRE(exists(id1, all_nodes) == 1);
    REQUIRE(exists(id2, all_nodes) == 1);
    REQUIRE(exists(id3, all_nodes) == 1);
    REQUIRE(exists(id4, all_nodes) == 1);
  }

  SECTION("exists shold also work for names, but only for id_table")
  {
    char * name1 = (char *)"TST-A*01:01:01:01";
    char * name2 = (char *)"TST-A*01:01:01:02";
    char * name3 = (char *)"TST-A*01:01:03";
    char * name4 = (char *)"TST-A*01:04";

    REQUIRE(getAllele(name1, id_table) != 0);
    REQUIRE(getAllele(name2, id_table) != 0);
    REQUIRE(getAllele(name3, id_table) != 0);
    REQUIRE(getAllele(name4, id_table) != 0);

    REQUIRE(getAllele(name1, all_nodes) == 0);
    REQUIRE(getAllele(name2, all_nodes) == 0);
    REQUIRE(getAllele(name3, all_nodes) == 0);
    REQUIRE(getAllele(name4, all_nodes) == 0);
  }

  // SECTION("exists shold also work for names, but only for id_table")
  // {
  //   char * subname1 = (char *)"TST-A*01:01:01";
  //   char * subname2 = (char *)"TST-A*01:01";
  //   char * subname3 = (char *)"TST-A*01";
  //
  //   REQUIRE(getAllele(subname1, id_table) != 0);
  //   REQUIRE(getAllele(subname2, id_table) != 0);
  //   REQUIRE(getAllele(subname3, id_table) != 0);
  //
  //   REQUIRE(getAllele(subname1, all_nodes) == 0);
  //   REQUIRE(getAllele(subname2, all_nodes) == 0);
  //   REQUIRE(getAllele(subname3, all_nodes) == 0);
  // }

  SECTION("exists should return false for all other ids")
  {
    char * fake_id1 = (char *)"";
    char * fake_id2 = (char *)"T";
    char * fake_id3 = (char *)"T0";
    char * fake_id4 = (char *)"T123";
    char * fake_id5 = (char *)"asdfpajsdfpdasdf";

    REQUIRE(exists(fake_id1, id_table) == 0);
    REQUIRE(exists(fake_id2, id_table) == 0);
    REQUIRE(exists(fake_id3, id_table) == 0);
    REQUIRE(exists(fake_id4, id_table) == 0);
    REQUIRE(exists(fake_id5, id_table) == 0);
  }
}


TEST_CASE("exists should work using getIdTable on hla.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"hla.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, rapidxml::xml_node<> *> id_table = getIdTable(my_root_node);
  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  SECTION("exists should return true for exact ids")
  {
    char * id1 = (char *)"HLA00001";
    char * id2 = (char *)"HLA02169";
    char * id3 = (char *)"HLA01244";
    char * id4 = (char *)"HLA01971";

    REQUIRE(exists(id1, id_table) == 1);
    REQUIRE(exists(id2, id_table) == 1);
    REQUIRE(exists(id3, id_table) == 1);
    REQUIRE(exists(id4, id_table) == 1);
  }

  SECTION("exists should return false for all other ids")
  {
    char * fake_id1 = (char *)"";
    char * fake_id2 = (char *)"T";
    char * fake_id3 = (char *)"T0";
    char * fake_id4 = (char *)"T123";
    char * fake_id5 = (char *)"asdfpajsdfpdasdf";

    REQUIRE(exists(fake_id1, id_table) == 0);
    REQUIRE(exists(fake_id2, id_table) == 0);
    REQUIRE(exists(fake_id3, id_table) == 0);
    REQUIRE(exists(fake_id4, id_table) == 0);
    REQUIRE(exists(fake_id5, id_table) == 0);
  }
}


/**
 * getSequence method returns the sequence of an allele
 */
TEST_CASE("getSequence should work using dummy.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  SECTION("getSequence should work using ids with id_table and all_nodes ")
  {
    char * id1 = (char *)"T1";
    char * id2 = (char *)"T2";
    char * id3 = (char *)"T3";
    char * id4 = (char *)"T4";

    REQUIRE(getSequence(id1, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getSequence(id2, id_table) == std::string("TTTTA"));
    REQUIRE(getSequence(id3, id_table) == std::string("AAAATGTT"));
    REQUIRE(getSequence(id4, id_table) == std::string("GATTATTAGA"));

    REQUIRE(getSequence(id1, all_nodes) == std::string("GATTATTAGA"));
    REQUIRE(getSequence(id2, all_nodes) == std::string("TTTTA"));
    REQUIRE(getSequence(id3, all_nodes) == std::string("AAAATGTT"));
    REQUIRE(getSequence(id4, all_nodes) == std::string("GATTATTAGA"));
  }

  SECTION("getSequence should work using names with id_table, but not with all_nodes")
  {
    char * name1 = (char *)"TST-A*01:01:01:01";
    char * name2 = (char *)"TST-A*01:01:01:02";
    char * name3 = (char *)"TST-A*01:01:03";
    char * name4 = (char *)"TST-A*01:04";

    REQUIRE(getSequence(name1, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getSequence(name2, id_table) == std::string("TTTTA"));
    REQUIRE(getSequence(name3, id_table) == std::string("AAAATGTT"));
    REQUIRE(getSequence(name4, id_table) == std::string("GATTATTAGA"));

    REQUIRE(strcmp(getSequence(name1, all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getSequence(name2, all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getSequence(name3, all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getSequence(name4, all_nodes), (char *)"Id not found!") == 0);
  }

  // SECTION("getSequence should work using subnames with id_table, but not with all_nodes")
  // {
  //   char * subname1 = (char *)"TST-A*01:01:01";
  //   char * subname2 = (char *)"TST-A*01:01";
  //   char * subname3 = (char *)"TST-A*01";
  //
  //   REQUIRE(getSequence(subname1, id_table) == std::string("GATTATTAGA"));
  //   REQUIRE(getSequence(subname2, id_table) == std::string("GATTATTAGA"));
  //   REQUIRE(getSequence(subname3, id_table) == std::string("GATTATTAGA"));
  //
  //   REQUIRE(strcmp(getSequence(subname1, all_nodes), (char *)"Id not found!") == 0);
  //   REQUIRE(strcmp(getSequence(subname2, all_nodes), (char *)"Id not found!") == 0);
  //   REQUIRE(strcmp(getSequence(subname3, all_nodes), (char *)"Id not found!") == 0);
  // }

  SECTION("getSequence shouldn't work otherwise")
  {
    REQUIRE(strcmp(getSequence((char *) "", id_table), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getSequence((char *) "TST-A", id_table), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getSequence((char *) "asdfa", id_table), (char *)"Id not found!") == 0);

    REQUIRE(strcmp(getSequence((char *) "", all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getSequence((char *) "TST-A", all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getSequence((char *) "asdfa", all_nodes), (char *)"Id not found!") == 0);
  }
}


/**
 * getReference method returns the reference sequence of an allele, if the allele is its own reference it will return itself.
 */
TEST_CASE("getReference should work using dummy.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  SECTION("getReference should work using ids with id_table and all_nodes ")
  {
    char * id1 = (char *)"T1";
    char * id2 = (char *)"T2";
    char * id3 = (char *)"T3";
    char * id4 = (char *)"T4";

    REQUIRE(getReference(id1, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getReference(id2, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getReference(id3, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getReference(id4, id_table) == std::string("AAAATGTT"));

    REQUIRE(getReference(id1, all_nodes) == std::string("GATTATTAGA"));
    REQUIRE(getReference(id2, all_nodes) == std::string("GATTATTAGA"));
    REQUIRE(getReference(id3, all_nodes) == std::string("GATTATTAGA"));
    REQUIRE(getReference(id4, all_nodes) == std::string("AAAATGTT"));
  }

  SECTION("getReference should work using names with id_table, but not with all_nodes")
  {
    char * name1 = (char *)"TST-A*01:01:01:01";
    char * name2 = (char *)"TST-A*01:01:01:02";
    char * name3 = (char *)"TST-A*01:01:03";
    char * name4 = (char *)"TST-A*01:04";

    REQUIRE(getReference(name1, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getReference(name2, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getReference(name3, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getReference(name4, id_table) == std::string("AAAATGTT"));

    REQUIRE(strcmp(getReference(name1, all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getReference(name2, all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getReference(name3, all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getReference(name4, all_nodes), (char *)"Id not found!") == 0);
  }

  // SECTION("getReference should work using subnames with id_table, but not with all_nodes")
  // {
  //   char * subname1 = (char *)"TST-A*01:01:01";
  //   char * subname2 = (char *)"TST-A*01:01";
  //   char * subname3 = (char *)"TST-A*01";
  //
  //   REQUIRE(getReference(subname1, id_table) == std::string("GATTATTAGA"));
  //   REQUIRE(getReference(subname2, id_table) == std::string("GATTATTAGA"));
  //   REQUIRE(getReference(subname3, id_table) == std::string("GATTATTAGA"));
  //
  //   REQUIRE(strcmp(getReference(subname1, all_nodes), (char *)"Id not found!") == 0);
  //   REQUIRE(strcmp(getReference(subname2, all_nodes), (char *)"Id not found!") == 0);
  //   REQUIRE(strcmp(getReference(subname3, all_nodes), (char *)"Id not found!") == 0);
  // }

  SECTION("getReference shouldn't work otherwise")
  {
    REQUIRE(strcmp(getReference((char *) "", id_table), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getReference((char *) "TST-A", id_table), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getReference((char *) "asdfa", id_table), (char *)"Id not found!") == 0);

    REQUIRE(strcmp(getReference((char *) "", all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getReference((char *) "TST-A", all_nodes), (char *)"Id not found!") == 0);
    REQUIRE(strcmp(getReference((char *) "asdfa", all_nodes), (char *)"Id not found!") == 0);
  }
}

/**
 * getFullSequence method returns the sequence of an allele
 */
TEST_CASE("getFullSequence should work using dummy.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  SECTION("getFullSequence should work using ids with id_table and all_nodes ")
  {
    char * id1 = (char *)"T1";
    char * id2 = (char *)"T2";
    char * id3 = (char *)"T3";
    char * id4 = (char *)"T4";
    char * id5 = (char *)"HLA00499";
    char * id6 = (char *)"HLA06604";

    REQUIRE(getFullSequence(id1, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getFullSequence(id2, id_table) == std::string("GTTTATTAGA"));
    REQUIRE(getFullSequence(id3, id_table) == std::string("GATAAAATGTT"));
    REQUIRE(getFullSequence(id4, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getFullSequence(id5, id_table) != std::string("GATTATTAGA"));
    REQUIRE(getFullSequence(id6, id_table) != std::string("GATTATTAGA"));

    REQUIRE(getFullSequence(id1, all_nodes) == std::string("GATTATTAGA"));
    REQUIRE(getFullSequence(id2, all_nodes) == std::string("GTTTATTAGA"));
    REQUIRE(getFullSequence(id3, all_nodes) == std::string("GATAAAATGTT"));
    REQUIRE(getFullSequence(id4, all_nodes) == std::string("GATTATTAGA"));
    REQUIRE(getFullSequence(id5, all_nodes) != std::string("GATTATTAGA"));
    REQUIRE(getFullSequence(id6, all_nodes) != std::string("GATTATTAGA"));
  }

  SECTION("getFullSequence should work when selecting a gene")
  {
    std::map<std::string, xml_node<> *> another_id_table = getIdTable(my_root_node, std::string("test"));

    char * id1 = (char *)"T1";
    char * id2 = (char *)"T2";
    char * id3 = (char *)"T3";
    char * id4 = (char *)"T4";
    char * id5 = (char *)"HLA00499";
    char * id6 = (char *)"HLA06604";

    REQUIRE(getFullSequence(id1, another_id_table) == std::string("GATTATTAGA"));
    REQUIRE(getFullSequence(id2, another_id_table) == std::string("GTTTATTAGA"));
    REQUIRE(getFullSequence(id3, another_id_table) == std::string("GATAAAATGTT"));
    REQUIRE(getFullSequence(id4, another_id_table) == std::string("GATTATTAGA"));
    REQUIRE(getFullSequence(id5, another_id_table) == std::string("Id not found!"));
    REQUIRE(getFullSequence(id6, another_id_table) == std::string("Id not found!"));

    std::map<std::string, xml_node<> *> yet_another_id_table = getIdTable(my_root_node, std::string("HLA-DPA1"));

    REQUIRE(getFullSequence(id1, yet_another_id_table) == std::string("Id not found!"));
    REQUIRE(getFullSequence(id2, yet_another_id_table) == std::string("Id not found!"));
    REQUIRE(getFullSequence(id3, yet_another_id_table) == std::string("Id not found!"));
    REQUIRE(getFullSequence(id4, yet_another_id_table) == std::string("Id not found!"));
    REQUIRE(getFullSequence(id5, yet_another_id_table) != std::string("Id not found!"));
    REQUIRE(getFullSequence(id6, yet_another_id_table) != std::string("Id not found!"));
  }

  SECTION("getFullSequence should work using names with id_table, but not with all_nodes")
  {
    char * name1 = (char *)"TST-A*01:01:01:01";
    char * name2 = (char *)"TST-A*01:01:01:02";
    char * name3 = (char *)"TST-A*01:01:03";
    char * name4 = (char *)"TST-A*01:04";

    REQUIRE(getFullSequence(name1, id_table) == std::string("GATTATTAGA"));
    REQUIRE(getFullSequence(name2, id_table) == std::string("GTTTATTAGA"));
    REQUIRE(getFullSequence(name3, id_table) == std::string("GATAAAATGTT"));
    REQUIRE(getFullSequence(name4, id_table) == std::string("GATTATTAGA"));

    REQUIRE(getFullSequence(name1, all_nodes) == "Id not found!");
    REQUIRE(getFullSequence(name2, all_nodes) == "Id not found!");
    REQUIRE(getFullSequence(name3, all_nodes) == "Id not found!");
    REQUIRE(getFullSequence(name4, all_nodes) == "Id not found!");
  }

  // SECTION("getFullSequence should work using subnames with id_table, but not with all_nodes")
  // {
  //   char * subname1 = (char *)"TST-A*01:01:01";
  //   char * subname2 = (char *)"TST-A*01:01";
  //   char * subname3 = (char *)"TST-A*01";
  //
  //   REQUIRE(getFullSequence(subname1, id_table) == std::string("GATTATTAGA"));
  //   REQUIRE(getFullSequence(subname2, id_table) == std::string("GATTATTAGA"));
  //   REQUIRE(getFullSequence(subname3, id_table) == std::string("GATTATTAGA"));
  //
  //   REQUIRE(getFullSequence(subname1, all_nodes) == "Id not found!");
  //   REQUIRE(getFullSequence(subname2, all_nodes) == "Id not found!");
  //   REQUIRE(getFullSequence(subname3, all_nodes) == "Id not found!");
  // }

  SECTION("getFullSequence shouldn't work otherwise")
  {
    REQUIRE(getFullSequence((char *) "", id_table) == "Id not found!");
    REQUIRE(getFullSequence((char *) "TST-A", id_table) == "Id not found!");
    REQUIRE(getFullSequence((char *) "asdfa", id_table) == "Id not found!");

    REQUIRE(getFullSequence((char *) "", all_nodes) == "Id not found!");
    REQUIRE(getFullSequence((char *) "TST-A", all_nodes) == "Id not found!");
    REQUIRE(getFullSequence((char *) "asdfa", all_nodes) == "Id not found!");
  }
}


TEST_CASE("getFullSequence should work using hla.xml")
{
  using namespace rapidxml;

  char * xml = (char *)"hla.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  SECTION("getFullSequence should work using ids with id_table and all_nodes ")
  {
    char * id1 = (char *)"HLA00001";
    char * id2 = (char *)"HLA06604";

    REQUIRE(getFullSequence(id1, id_table) == std::string("CAGGAGCAGAGGGGTCAGGGCGAAGTCCCAGGGCCCCAGGCGTGGCTCTCAGGGTCTCAGGCCCCGAAGGCGGTGTATGGATTGGGGAGTCCCAGCCTTGGGGATTCCCCAACTCCGCAGTTTCTTTTCTCCCTCTCCCAACCTACGTAGGGTCCTTCATCCTGGATACTCACGACGCGGACCCAGTTCTCACTCCCATTGGGTGTCGGGTTTCCAGAGAAGCCAATCAGTGTCGTCGCGGTCGCTGTTCTAAAGTCCGCACGCACCCACCGGGACTCAGATTCTCCCCAGACGCCGAGGATGGCCGTCATGGCGCCCCGAACCCTCCTCCTGCTACTCTCGGGGGCCCTGGCCCTGACCCAGACCTGGGCGGGTGAGTGCGGGGTCGGGAGGGAAACCGCCTCTGCGGGGAGAAGCAAGGGGCCCTCCTGGCGGGGGCGCAGGACCGGGGGAGCCGCGCCGGGAGGAGGGTCGGGCAGGTCTCAGCCACTGCTCGCCCCCAGGCTCCCACTCCATGAGGTATTTCTTCACATCCGTGTCCCGGCCCGGCCGCGGGGAGCCCCGCTTCATCGCCGTGGGCTACGTGGACGACACGCAGTTCGTGCGGTTCGACAGCGACGCCGCGAGCCAGAAGATGGAGCCGCGGGCGCCGTGGATAGAGCAGGAGGGGCCGGAGTATTGGGACCAGGAGACACGGAATATGAAGGCCCACTCACAGACTGACCGAGCGAACCTGGGGACCCTGCGCGGCTACTACAACCAGAGCGAGGACGGTGAGTGACCCCGGCCCGGGGCGCAGGTCACGACCCCTCATCCCCCACGGACGGGCCAGGTCGCCCACAGTCTCCGGGTCCGAGATCCACCCCGAAGCCGCGGGACTCCGAGACCCTTGTCCCGGGAGAGGCCCAGGCGCCTTTACCCGGTTTCATTTTCAGTTTAGGCCAAAAATCCCCCCGGGTTGGTCGGGGCGGGGCGGGGCTCGGGGGACTGGGCTGACCGCGGGGTCGGGGCCAGGTTCTCACACCATCCAGATAATGTATGGCTGCGACGTGGGGCCGGACGGGCGCTTCCTCCGCGGGTACCGGCAGGACGCCTACGACGGCAAGGATTACATCGCCCTGAACGAGGACCTGCGCTCTTGGACCGCGGCGGACATGGCAGCTCAGATCACCAAGCGCAAGTGGGAGGCGGTCCATGCGGCGGAGCAGCGGAGAGTCTACCTGGAGGGCCGGTGCGTGGACGGGCTCCGCAGATACCTGGAGAACGGGAAGGAGACGCTGCAGCGCACGGGTACCAGGGGCCACGGGGCGCCTCCCTGATCGCCTATAGATCTCCCGGGCTGGCCTCCCACAAGGAGGGGAGACAATTGGGACCAACACTAGAATATCACCCTCCCTCTGGTCCTGAGGGAGAGGAATCCTCCTGGGTTTCCAGATCCTGTACCAGAGAGTGACTCTGAGGTTCCGCCCTGCTCTCTGACACAATTAAGGGATAAAATCTCTGAAGGAGTGACGGGAAGACGATCCCTCGAATACTGATGAGTGGTTCCCTTTGACACCGGCAGCAGCCTTGGGCCCGTGACTTTTCCTCTCAGGCCTTGTTCTCTGCTTCACACTCAATGTGTGTGGGGGTCTGAGTCCAGCACTTCTGAGTCTCTCAGCCTCCACTCAGGTCAGGACCAGAAGTCGCTGTTCCCTTCTCAGGGAATAGAAGATTATCCCAGGTGCCTGTGTCCAGGCTGGTGTCTGGGTTCTGTGCTCTCTTCCCCATCCCGGGTGTCCTGTCCATTCTCAAGATGGCCACATGCGTGCTGGTGGAGTGTCCCATGACAGATGCAAAATGCCTGAATTTTCTGACTCTTCCCGTCAGACCCCCCCAAGACACATATGACCCACCACCCCATCTCTGACCATGAGGCCACCCTGAGGTGCTGGGCCCTGGGCTTCTACCCTGCGGAGATCACACTGACCTGGCAGCGGGATGGGGAGGACCAGACCCAGGACACGGAGCTCGTGGAGACCAGGCCTGCAGGGGATGGAACCTTCCAGAAGTGGGCGGCTGTGGTGGTGCCTTCTGGAGAGGAGCAGAGATACACCTGCCATGTGCAGCATGAGGGTCTGCCCAAGCCCCTCACCCTGAGATGGGGTAAGGAGGGAGATGGGGGTGTCATGTCTCTTAGGGAAAGCAGGAGCCTCTCTGGAGACCTTTAGCAGGGTCAGGGCCCCTCACCTTCCCCTCTTTTCCCAGAGCTGTCTTCCCAGCCCACCATCCCCATCGTGGGCATCATTGCTGGCCTGGTTCTCCTTGGAGCTGTGATCACTGGAGCTGTGGTCGCTGCCGTGATGTGGAGGAGGAAGAGCTCAGGTGGAGAAGGGGTGAAGGGTGGGGTCTGAGATTTCTTGTCTCACTGAGGGTTCCAAGCCCCAGCTAGAAATGTGCCCTGTCTCATTACTGGGAAGCACCTTCCACAATCATGGGCCGACCCAGCCTGGGCCCTGTGTGCCAGCACTTACTCTTTTGTAAAGCACCTGTTAAAATGAAGGACAGATTTATCACCTTGATTACGGCGGTGATGGGACCTGATCCCAGCAGTCACAAGTCACAGGGGAAGGTCCCTGAGGACAGACCTCAGGAGGGCTATTGGTCCAGGACCCACACCTGCTTTCTTCATGTTTCCTGATCCCGCCCTGGGTCTGCAGTCACACATTTCTGGAAACTTCTCTGGGGTCCAAGACTAGGAGGTTCCTCTAGGACCTTAAGGCCCTGGCTCCTTTCTGGTATCTCACAGGACATTTTCTTCCCACAGATAGAAAAGGAGGGAGTTACACTCAGGCTGCAAGTAAGTATGAAGGAGGCTGATGCCTGAGGTCCTTGGGATATTGTGTTTGGGAGCCCATGGGGGAGCTCACCCACCCCACAATTCCTCCTCTAGCCACATCTTCTGTGGGATCTGACCAGGTTCTGTTTTTGTTCTACCCCAGGCAGTGACAGTGCCCAGGGCTCTGATGTGTCTCTCACAGCTTGTAAAGGTGAGAGCTTGGAGGGCCTGATGTGTGTTGGGTGTTGGGTGGAACAGTGGACACAGCTGTGCTATGGGGTTTCTTTGCGTTGGATGTATTGAGCATGCGATGGGCTGTTTAAGGTGTGACCCCTCACTGTGATGGATATGAATTTGTTCATGAATATTTTTTTCTATAGTGTGAGACAGCTGCCTTGTGTGGGACTGAGAGGCAAGAGTTGTTCCTGCCCTTCCCTTTGTGACTTGAAGAACCCTGACTTTGTTTCTGCAAAGGCACCTGCATGTGTCTGTGTTCGTGTAGGCATAATGTGAGGAGGTGGGGAGAGCACCCCACCCCCATGTCCACCATGACCCTCTTCCCACGCTGACCTGTGCTCCCTCTCCAATCATCTTTCCTGTTCCAGAGAGGTGGGGCTGAGGTGTCTCCATCTCTGTCTCAACTTCATGGTGCACTGAGCTGTAACTTCTTCCTTCCCTATTAAAA"));
    REQUIRE(getFullSequence(id2, id_table) == std::string("GGTGGACCTGAAAGAAAGATTAATTATATATTTTATTAGATAGCACATTCATTCTTAATTATCAGCAGAATATTTTTGTAAAATATTGACAACATAACATAAAACATGATTTAAAATATAGTGACCAAAAAACATCAAAGATCATCTGACTGTCTGGGATGGGGGTGGGGCTCGGGAGAAGGAACATAATGGAAAACATTTGCCGTCAGAAGAAGGTGTAACTTATCTTTTTACATCTCTTTCTCTAACTCTGAAAATGAACTGTGAACTGGAGCTCTCTTGACCACGCTGGTACCTAAAATTCTCCCATCTCTTCCCCAGCACCTTCCAGCGTCCTCTTTACCCAGCAACAGAGAATGTCAGCTCTATGATTTCTCTGATAGGTGAATCCCAGCCATGCTGATTCCTCTCCACCCATTTCCAGTGCTAGAGGCCCACAGTTTCAGTCTCATCTGCCTCCACTCGGCCTCAGTTCCTCATCACTGTTCCTGTGCTCACAGTCATCAATTATAGACCCCACAACATGCGCCCTGAAGACAGAATGTTCCATATCAGAGCTGTGATCTTGAGAGCCCTCTCCTTGGCTTTCCTGCTGAGTCTCCGAGGAGCTGGGGCCATCAAGGGTGAGTGCTCAGGAGGACGCAGGAGCGTCGGGGTGAGTGATGGGGTGGTTCACATCAATTGCTGCTTCAGGGATCACAGATTTTAGGGGCTCATTGATCTATCTGGTCCTCATAGTCTATGTTCCCTCTGGCCCTCATAATAATAACAGCAATAACAGCCAGAATTTATGAGACTCCTGCATAGTTTCTTTCCCCATTTACATCTCACAGGAATCTTCAATGAAGATAATATTCCATTCATTTAGAAATTATTCCTTTTATTTAGAAATTATTTTGAAAAAACTGAAGCTCAAAAAGATGAATAAGTTTTCCAAGGTTACACAGCAGATCAACGAGCCAAGTTTGAAGTCCAGACCCAGCTCTGAGGGTCATACACTGCCTTCCCCAGATTCCTGCACACAGTGACCTACTATCAGGGCCCTCCTATCTCTCTGGGATCCCCAGCCTCTATCTTTTGTGGCTGCTTTACAGGAACTCCGAGCTATGGACTCTGCATTAGGAGACGAAGTGCAAAGAGTGTTTCTGTATCCTCCCTCTCTTCTAGGACCCTAGGGCTCTTCCTGGGTCTTTGTGGGTGGTCACAAGCTTTCCTCTCTCAAGACAGCAGGGTTGCATGGTCTTGATAGCCTTGTGATTCGGGTTCTGAGAGATTCAGGACTGCAAGGGAGGCCTAGACTTTTGATAGCTGCAAGGACTCAGCCAGAGATGGACCGTAGTGAATGCTCCTTTTTCCTGTAGCTGAAATCAGGGAGAATGACATCAAGCCTGTGCATGATGCTGTCATTCCAAAATCTAGTGATGGGGAAGGTTAGAATCCATAACGTACAAGATGCACACTGGCTTCAGACAGTTTTATTTAAGATGTGTAGAATAAAGAGGAGGTCAGGCTGGGTAGAACCAGAAGTATCTATTGCCCTGTTCGCGGTCACCTGAGTTATTTCTAATGTTATGTTATAATAAACACCACAATAGGCTTCTCTTCATAGATGCAAATACTTTTTAGTATTCTTGGTAGAAATTCCTAATGAGCTCAGCTGTCTCTTCAGGGCTTCCCTGCCCAGTCTCTTAACATTTAAACATGTCATTTACCTTAAAAACATAAGTGCAAACCAACTGATAAAAAACAACCTTGCCTTCAGTCTGCATCCTGTCCCAGAGACACTTTCTTTGTGTCCTCACACGTGGAGCTAAGCTTCTGACTTGTCTCTGGTACATCCCTGAGGATCCTCTCATCTTGGCCATCAGGAACCTCTACAGAAGGTCAAATTCAGTGGGTTCTTCTCAGTGCCTCTGACTTGAGTTACTAATAACATTTGCACTATAATCCACTTCTTTCTGATGAACTACCCTGTCCTTATTTTTCTCCTGTTTACCTGGATCCTCCTTATCATCTTTTAAACCACCTCTTAACTATCATGTTCTCTCATTATACCCTGAGATCTCGGCAATTCTGATTTTTGGCACTCTTCCTGGAAAATCTTATTTAACCTGCACCTGCCACTAATGACTCTCAGTTCTATGGCCTAAATTCCTCTCCTGAGACCACCCATAATCCACAAATATCTATGTATTATTTCTCCTTAGATGACTTTCAGGTCTTCTAAGTGCAATAGCCCCACAGTAAACTCAGTATCTTCTCCCGGTCAGGCTGTCTTCCCTGAGAGAAGTGGCTTTTGCCCTGTTTTCTGAATGCCTACATTGAAGCCATCTGTTCCCCAGGAAGCCTTCCCTGATGTGCTGTTTGGTCGCATCTTGTGTATACCTACGTATCTGCACTTATCCTTCTGAACCTGCTGTTGTCCTGTCACTTGTGTTTCCTTCTGTGACTTATACGCGTCTGCAGAACAGGACGTATGTATTATTTTTATTTGGGTATTTAGCATCTAACAGTGTTTGACATATAGTAGTCTTTTAATACATATTTTTGTCTGAATGGAAATGATATTTTGAAGAAAAATAATCTGTTCCATAGCTGGCTGATCTTTGGACTGCAGAACTTGTGAAAGTGTTTTTTAAAAAGCATTTTAAAAAGTACAAGGGACATTCATGTATTAAGAAGATGAGTTTCCAATAACTGCTAGAGGACTTTGTGTCTTTTTATTTTACCCTCTTTTTCCTGATGAGTCCTTTGAGTCCTTTAAACTGAGGAGCAAGCTAAGTTTCCTAGTGAAATACCTATAGGATTTGTTTTGTTTAGTTTCAAATACCACTCTTTGCTTGGCCACTTACTGTGTCAGGGAGTCATTCTCAGTGAAAAATAAGACACAGGTCATACCCTCTAGACACTTACAATTACAGTGGCAAGGAGTCATTCTCCTGTCACTGTAAGTGGCCAAGCACAGACTGGGTCCCCACATGTCAGGGCTGAAAACTCACAGGGAAATCTGTGAGTTGGGAGGTGAGAGCAGAAGAGTCCCGTAGTTCCTTCTCACTCTGATGCATTTATCATTCTAAACCCAGACTTTCACATACACATTCATCGTTTTCTTTCATGATAATAGTTGCTTTTATCCTCTTATCTTTGCTAATTCTTACAAACTAATAAAGACTAAGAAACAAAATAAATTAAATCCTACAGGTGTTCCAAACTCAGCAATAATTTCTAGTTGGCCTCTAAAACAAAAATCAAAATATAAATGTAAGAAAAGTTTAGAATGCTTAGTACCTGTGTGATGAAATAATCTGTACACTAAACCCCCAAGTCATGAGTTTACCTATACAACAAACCTGCACATGTACTCCTGAACATAAAATAAATGTTGAAATATTTTTAAAAAGGAAACAAAAGTTTGGAACAAATGCCAAAATAACTGTACTGTACTTTTGAATTTATATGCCCCAAATGAAAAATATTATCAACAAAGCTATACATTCTACAGTTTCATGTTCATAAACTAAGACAGAAACTTTAAAACTGTCAAGAGCCCTAAAATTTGAAGGATATTTTCTTCTTCCTCTCAATTTTGTATTTTTTTCTACCTTTTCTATAATAAGAAAAAGAAAATGTCCATTCCCCCACCCCCATGACTCTAAAAACAATTTTACATCTGTGTCATAGAAAAATTAAGATCTTAATGGGAGAGAAAACCTCTCTACTAGTTCCGCCAGTAGCCGTATGACCTTAGCAAGTTATTAATATGTAACTTCCCTGCATTTCCTTACCTGTAAAATGTATGATATGTATTTGCTTCATAGGGTTATTGTGACAATTCAGCGAGTGAAATATGTAAAGTATTTAGAAGGATGCCTGGCACAAGTAAGTGCTCAACAAATGTTAGCTGTCATTGTTACTATTACTATTGTGTAGGGTCAGGATGCCCAGACTTTCAAAGACCAGGAAGCAGCTTGACTTATCAGTGATAAACTTTTCATTTTGTTCTTTGCTCCTTTCTTTTTATAACTGCTCATCTGCTCTGTATTATTTCCTTTATGGTGTTGCTCCTTCTTCTTCCCCATATGTCCTTCCTTTGACCTCTTACCTTCTTCCTTTTTATATTCATAAGTCTTTATTCATTCTCTAGCTTTGACCACTTGCATATTCAAACTGACATTTTGTCGTGTTTTTCTCTACTGTCTTTATGCAGCGGACCATGTGTCAACTTATGCCGCGTTTGTACAGACGCATAGACCAACAGGGGAGTTTATGTTTGAATTTGATGAAGATGAGATGTTCTATGTGGATCTGGACAAGAAGGAGACCGTCTGGCATCTGGAGGAGTTTGGCCAAGCCTTTTCCTTTGAGGCTCAGGGCGGGCTGGCTAACATTGCTATATTGAACAACAACTTGAATACCTTGATCCAGCGTTCCAACCACACTCAGGCCACCAACGGTACGCCCTATCTTTGCCTCTTCCTCTGTAGCCCAACTGGAAGGGATGAGAGGGCCTCTCTGCCACCCTCAGACTAGGAAGCCTAAGTGCCCCCTGCTGTGTGATCCTCTTCCCCTAGTGGCCATGGGCTGATCCCACTACAGCAAGGGCTTGCATCCTCTCTTCTCAGGAGAGAGAAAGGTGAGCAGAGTGAGGCTGGTCAGTGGTGTGATACCCCTCTCTGTGATTCAGAGCTGCCATAAAATCTAAGGCTGAGGTAGAGGACCACCCTCCCCTAAGAGGTGGAGCCTTTGTGATTCATCCCAGAAGAGGGGCCTAACCTGGTGCTGTCTCCTTCCAGATCCCCCTGAGGTGACCGTGTTTCCCAAGGAGCCTGTGGAGCTGGGCCAGCCCAACACCCTCATCTGCCACATTGACAAGTTCTTCCCACCAGTGCTCAACGTCACGTGGCTGTGCAACGGGGAGCTGGTCACTGAGGGTGTCGCTGAGAGCCTCTTCCTGCCCAGAACAGATTACAGCTTCCACAAGTTCCATTACCTGACCTTTGTGCCCTCAGCAGAGGACTTCTATGACTGCAGGGTGGAGCACTGGGGCTTGGACCAGCCGCTCCTCAAGCACTGGGGTATGCAACTGCTTTTCTCTCCATAATCTCCTGGCATCCTCTATTCCAAAGACCTGGTGTCCTCTGCACCAGCTTTCCGCACTGGCTGGGTCTCAGTCCTCTCCTCGTCCTAACATCCAATTAACTGGTCCATAACCTTCAATTCCCACAACCATCCCAGGCCATCACCACCCTCACTGCACCTCCTGACCCTATCTCTTCATTCTTCCCCCAGAGGCCCAAGAGCCAATCCAGATGCCTGAGACAACGGAGACTGTGCTCTGTGCCCTGGGCCTGGTGCTGGGCCTAGTCGGCATCATCGTGGGCACCGTCCTCATCATAAAGTCTCTGCGTTCTGGCCATGACCCCCGGGCCCAGGGGACCCTGTGAAATACTGTAAAGGTGGGAATGTAAAGAGGAGGCCCTAGGATTTGTAGAATGTAAGGAAGGGAGGAAAAATTCAATCTGATAAGTGTTCATTGATCTTCTAATGGGTTAAAAGCATTCAGCCACATAACAACAACAACACCGATAACTAACTGAGTAGTTAATATGGTCAGGCGCTATTCTGAGGATTTACATTTATTAACTCACTTTATTCTCACACATAGTCTTTGAGGTAGGTACTATTATTTTCACTATTTCACATGAGAGATACTTACATCTTTTTACATACACAGAGACTTTAAGCACTTTGATCAAGTTCCCACAGCTATGAAGTAGTAGGGCTAGCTTCCAATCCAGAAAGTCTGGATCCAAGACTGTTTATCCACTGTCCTATTCACCCTATTTTGTGAAGGAAAAGACCAAGTTCAAATTCTCCAGAGTCCATTGCCAAATAATGGAGTCAGATCTATATTTCTATACATAATTACAACACAGTGTGGTGGGTGCCTGTAACTACTTACTGTCTCTACTTGGACTCATTCCATGGCAATGTTCACACAAAAAATGCCCCTCCAGAGATCTTACAGGTTTCTATTTATCATAACACTCACCATGCTTTATATTTTTATATGTTTTGGGAATTCTCTTAGCATTAGACAGTGAACTTCCATGCAGATGACCACATCTAATTCATTATTATTATTGTTATTCATGCTGGACCTCAGGTACAAAAGGTTAAGAACTTCTCAGTTCATTATATGATCATCATTGGTGCCTCCGAGCTCTCTCTCTCTCCCTTGATTTATTTGGTCCCTTTTATCTCCAGTCCTTACTCCCATATCTAACCTCTTACCCCTACCTCATAGGTAAACATTTTAATGAATTTGATGTTTCCTTTTATTTGCATAGATCCTCTGTAATATGTAGTAGTGTCCAGTGTACATGTATTTTTAATTAACCAAAATGGCATTAAATTATAGATCTAATTTTGTACATCCAGTTTGTTTCTTCCAAATCTTCCATAGTATTTTACTTTATATGTCCATGCATTAGTCCATTTTGCATTGCTATAAAGGAATATCTGAAGTTACCTAATTTACGAAGAAAAGAGCTTTAAATGGCTCACAGATCTGCAGGCTGTACGCGAAACATGGCACTAGCATCTGCTTCTGTTGGGGGATTCTGGAAGCTTTTACTCATGGTGGAAGGCAAGTGGAGCCAGTGCATCACATGGTCATAGAGGGAGAAAGAGACATAGAAAGAGGTGCCAGCCTCTTTTTAACAACCAGGTTTCATGTGCACTAATAGAGTGAGAACTCACTCATTACCCGGAGAGGGGACAAAGCCATTCATGAGGGTCTCCTCCATGATTCAAATACCTCCCACCAGGCCCCACCTGCAACACTGGGGATCAATTTTCAACATGAGACTTGGAAGTGACAAATATCCAAATCATATTAATCCACATATCTACATTGCTCCTGGGATACCTGGATCATTCCTGGTTCTCTACTATTGCAAGCAATGCTTGTATCTCACATGGAACTGCATATACATGTGGGCCTGACCTGCATCCCTGGAATGTATGTATCCTAGAAAGGGGTTGCAGGGTTGCTGGAGATGCAGCTCCTTAATTTGACTAAACACTGCTCATCTTCTCATCAGAATGGCTGTACTCATCTGAACTTCCTTTGTCAGTACTCTAATTGTCCTGCAACTCCTAAATGGACTTCAACACTGGACATTATCCAGTTTTCTAACTTTTGCCAATTTCATGTGCATAAAGAAATATGCTGTTTTATTTTGCATTTCTTTAATTACTAATAATTGGGGCTATAATTAGGACTGATTAGCCACTTGGGGGTTCCTTTTCTATAAATTGCCTGTTCACATTCATTGTCCATTTTTGTACTATGTGCTTCCATCATTTTCTTATTGATTTGCAGGTGATCCTTATATAGTCCTGCTAGTAGTCCCTTGTCAGTTTTAGGCATTGCAAATGTTTTCCTCTAATCTGACTTCTGGCAACTGTCTCCTTGGTTTCCTTTATTGAAGAGAAATCCTTAATATTTTGTAATGAAGTCCATCAACTGTATTTTTGTTTGTGTGTCTTTTTTAAAAGAAGTCTTCCCTATACTGAGATATCAAAGATACTCTTAAAACATCTCCTACAGTTTTAAATTTCACATTTACTACTTTAATTCATCTGGGATTCATCTTTGTGTTTGATGGGGATCATGTTTTATTTTTCTTTATATAATGGGCCAGTGTGTTCCCACAACTACTAAATAGTTCACCTTTTCCCCATAGGTTAGTAGTGTCTCCTTTGCTATACTGAAAGCTCCCATTATAGGTGGGCCTGTGTCTGAGTTCCATCTTGTTCCACTGTTCTGTTTGTCTCTTCTTGTGCCAGTGTCCTAGTATTTTGATTACTATGACATTGTAGTGTGTGTTAGTATCCAGTAGGACAAATTCTTGTTTATTTTTCTTAGTTCACACACATTTATAATTATATCTATAATGATTTGTAACAGAGTGAAGTGAATGTAGAATGTCAGATGTTAAGAGGAAGAATGGAAAAGAGGGCTGGGACTAGGGTGATGTAGGGGATGCACCTGGCTTAGGTGCAAAATTTGGGGGATACCAAAAGAACTCAGTAATAAATCATATTTTAATGAAATATCTTGAAAAGGCAAAATTAATGCAAAGATACATGATTAACAAAACATCCAAAGAGGAGTATTTAACAAAAATGGAGAAGCAGAGAAGCAGAAGAATTAGGAGAATATGCTGTCACATGAGCCAAGGAATTAAAGAATTCAGGAAGGAGGAAGTACTGCTGTCAGATGTTCAACAGAGGTCATTTTAGAAAATTTACCTTGGTTTTTGAAATCCTTTCAAAGAGCAGTATACACAATGTGAGCAAGTATCCTTCGTTCATTGCCGTCATTGATATGGTTTGGATATTTGTCCCTTCCAATTCTCATTCCAGGGTTAAGCTTCTTCTCTGCCCTCAGTAATGTGGCCCTTCCCCTTGTCTGTATATTTTGGAGACATGAAGCATGTGGGATGGCCTCACAGTCAGCTGGGGTTTGAGGGTGAAATTCAATGACTTTCGTGAACTCCTTGGCTCCTATGTGCTCTTCACCTGGAGGACCAGGGCATGTGCAGGGATGACCACCTTCTCCCTGGGACCTGAACAGGGCAGAGAAATGGGAAGCTCGGGTGCAAAGGGAGTGGGGAAGATGGGTCCGGGCTTACAGTACTGAACCCAGGAATGACAATAACTGTGTGTGTTGCTGCAGGTGACAAAATATCTGAACAGAAGAGGACTTAGGAGAGATCTGAACTCCAGCTGCCCTACAAACTCCATCTCAGCTTTTCTTCTCACTTCATGTGAAAACTACTCCAGTGGCTGACTGAATTGCTGACCCTTCAAGCTCTGTCCTTATCCATTACCTCAAAGCAGTCATTCCTTAGTAAAGTTTCCAACAAATAGAAATTAATGACACTTTGGTAGCACTAATATGGAGATTATCCTTTCATTGAGCCTTTTATCCTCTGTTCTCCTTTGAAGAACCCCTCACTGTCACCTTCCCGAGAATACCCTAAGACCAATAAATACTTCAGTATTTCAGAGCGGGGAGACTCTGAGTCATTCTTACTGGAAGTCTAGGACCAGGTCACATGTGAATACTATTTCTTGAAGGTGTGGTTTCAACCTCTGTTGCCGATGTGGTTACTAAAGGTTCTGATCCCACTTGAACGGAAAGGTCTGAGGATATTGATTCAGTCCTGGGTTTTTCCCTAACTACAGGATAGGGTGGGGTAGAGAAAGGATATTTGGGGGAAATTTTACTTGGATGAAGATTTTCTTGGATGTAGTTTGAAGACTGCAGTGTTTGAAGTCTCTGAGGGAAGAGATTTGGTCTGTCTGGATCAAGATTTCAGGCAGATTAGGATTCCATTCACAGCCCCTGAGCTTCCTTCCCAAGGCTGTATTGTAATTATAGCAATATTTCATGGAGGATTTTTCTACATGATAAACTAAGAGCCAAGAAATAAAATTTTTAAAATGCCCTAATTCATTGCAATTTTTACCAGCCATAGTCACTCCATGTGGGAGAACTTAAATCATGATTACCAGAGCTTTCAAAGGTTTGAGAATAGTGATGATTATGAAGAAAAATATCTTATTTGAGCAAGGATTTTGTTTCTTTATGAGTGTTCATTAGATATTACGATGAAAAAAGCATGAAATGGTAAAAATTCAGATAAATATAAAAACATGTTCTCTAGTTTTTTTTAAGTTAAAAAAGGAATTGTTTAAAGTA"));

    REQUIRE(getFullSequence(id1, all_nodes) == std::string("CAGGAGCAGAGGGGTCAGGGCGAAGTCCCAGGGCCCCAGGCGTGGCTCTCAGGGTCTCAGGCCCCGAAGGCGGTGTATGGATTGGGGAGTCCCAGCCTTGGGGATTCCCCAACTCCGCAGTTTCTTTTCTCCCTCTCCCAACCTACGTAGGGTCCTTCATCCTGGATACTCACGACGCGGACCCAGTTCTCACTCCCATTGGGTGTCGGGTTTCCAGAGAAGCCAATCAGTGTCGTCGCGGTCGCTGTTCTAAAGTCCGCACGCACCCACCGGGACTCAGATTCTCCCCAGACGCCGAGGATGGCCGTCATGGCGCCCCGAACCCTCCTCCTGCTACTCTCGGGGGCCCTGGCCCTGACCCAGACCTGGGCGGGTGAGTGCGGGGTCGGGAGGGAAACCGCCTCTGCGGGGAGAAGCAAGGGGCCCTCCTGGCGGGGGCGCAGGACCGGGGGAGCCGCGCCGGGAGGAGGGTCGGGCAGGTCTCAGCCACTGCTCGCCCCCAGGCTCCCACTCCATGAGGTATTTCTTCACATCCGTGTCCCGGCCCGGCCGCGGGGAGCCCCGCTTCATCGCCGTGGGCTACGTGGACGACACGCAGTTCGTGCGGTTCGACAGCGACGCCGCGAGCCAGAAGATGGAGCCGCGGGCGCCGTGGATAGAGCAGGAGGGGCCGGAGTATTGGGACCAGGAGACACGGAATATGAAGGCCCACTCACAGACTGACCGAGCGAACCTGGGGACCCTGCGCGGCTACTACAACCAGAGCGAGGACGGTGAGTGACCCCGGCCCGGGGCGCAGGTCACGACCCCTCATCCCCCACGGACGGGCCAGGTCGCCCACAGTCTCCGGGTCCGAGATCCACCCCGAAGCCGCGGGACTCCGAGACCCTTGTCCCGGGAGAGGCCCAGGCGCCTTTACCCGGTTTCATTTTCAGTTTAGGCCAAAAATCCCCCCGGGTTGGTCGGGGCGGGGCGGGGCTCGGGGGACTGGGCTGACCGCGGGGTCGGGGCCAGGTTCTCACACCATCCAGATAATGTATGGCTGCGACGTGGGGCCGGACGGGCGCTTCCTCCGCGGGTACCGGCAGGACGCCTACGACGGCAAGGATTACATCGCCCTGAACGAGGACCTGCGCTCTTGGACCGCGGCGGACATGGCAGCTCAGATCACCAAGCGCAAGTGGGAGGCGGTCCATGCGGCGGAGCAGCGGAGAGTCTACCTGGAGGGCCGGTGCGTGGACGGGCTCCGCAGATACCTGGAGAACGGGAAGGAGACGCTGCAGCGCACGGGTACCAGGGGCCACGGGGCGCCTCCCTGATCGCCTATAGATCTCCCGGGCTGGCCTCCCACAAGGAGGGGAGACAATTGGGACCAACACTAGAATATCACCCTCCCTCTGGTCCTGAGGGAGAGGAATCCTCCTGGGTTTCCAGATCCTGTACCAGAGAGTGACTCTGAGGTTCCGCCCTGCTCTCTGACACAATTAAGGGATAAAATCTCTGAAGGAGTGACGGGAAGACGATCCCTCGAATACTGATGAGTGGTTCCCTTTGACACCGGCAGCAGCCTTGGGCCCGTGACTTTTCCTCTCAGGCCTTGTTCTCTGCTTCACACTCAATGTGTGTGGGGGTCTGAGTCCAGCACTTCTGAGTCTCTCAGCCTCCACTCAGGTCAGGACCAGAAGTCGCTGTTCCCTTCTCAGGGAATAGAAGATTATCCCAGGTGCCTGTGTCCAGGCTGGTGTCTGGGTTCTGTGCTCTCTTCCCCATCCCGGGTGTCCTGTCCATTCTCAAGATGGCCACATGCGTGCTGGTGGAGTGTCCCATGACAGATGCAAAATGCCTGAATTTTCTGACTCTTCCCGTCAGACCCCCCCAAGACACATATGACCCACCACCCCATCTCTGACCATGAGGCCACCCTGAGGTGCTGGGCCCTGGGCTTCTACCCTGCGGAGATCACACTGACCTGGCAGCGGGATGGGGAGGACCAGACCCAGGACACGGAGCTCGTGGAGACCAGGCCTGCAGGGGATGGAACCTTCCAGAAGTGGGCGGCTGTGGTGGTGCCTTCTGGAGAGGAGCAGAGATACACCTGCCATGTGCAGCATGAGGGTCTGCCCAAGCCCCTCACCCTGAGATGGGGTAAGGAGGGAGATGGGGGTGTCATGTCTCTTAGGGAAAGCAGGAGCCTCTCTGGAGACCTTTAGCAGGGTCAGGGCCCCTCACCTTCCCCTCTTTTCCCAGAGCTGTCTTCCCAGCCCACCATCCCCATCGTGGGCATCATTGCTGGCCTGGTTCTCCTTGGAGCTGTGATCACTGGAGCTGTGGTCGCTGCCGTGATGTGGAGGAGGAAGAGCTCAGGTGGAGAAGGGGTGAAGGGTGGGGTCTGAGATTTCTTGTCTCACTGAGGGTTCCAAGCCCCAGCTAGAAATGTGCCCTGTCTCATTACTGGGAAGCACCTTCCACAATCATGGGCCGACCCAGCCTGGGCCCTGTGTGCCAGCACTTACTCTTTTGTAAAGCACCTGTTAAAATGAAGGACAGATTTATCACCTTGATTACGGCGGTGATGGGACCTGATCCCAGCAGTCACAAGTCACAGGGGAAGGTCCCTGAGGACAGACCTCAGGAGGGCTATTGGTCCAGGACCCACACCTGCTTTCTTCATGTTTCCTGATCCCGCCCTGGGTCTGCAGTCACACATTTCTGGAAACTTCTCTGGGGTCCAAGACTAGGAGGTTCCTCTAGGACCTTAAGGCCCTGGCTCCTTTCTGGTATCTCACAGGACATTTTCTTCCCACAGATAGAAAAGGAGGGAGTTACACTCAGGCTGCAAGTAAGTATGAAGGAGGCTGATGCCTGAGGTCCTTGGGATATTGTGTTTGGGAGCCCATGGGGGAGCTCACCCACCCCACAATTCCTCCTCTAGCCACATCTTCTGTGGGATCTGACCAGGTTCTGTTTTTGTTCTACCCCAGGCAGTGACAGTGCCCAGGGCTCTGATGTGTCTCTCACAGCTTGTAAAGGTGAGAGCTTGGAGGGCCTGATGTGTGTTGGGTGTTGGGTGGAACAGTGGACACAGCTGTGCTATGGGGTTTCTTTGCGTTGGATGTATTGAGCATGCGATGGGCTGTTTAAGGTGTGACCCCTCACTGTGATGGATATGAATTTGTTCATGAATATTTTTTTCTATAGTGTGAGACAGCTGCCTTGTGTGGGACTGAGAGGCAAGAGTTGTTCCTGCCCTTCCCTTTGTGACTTGAAGAACCCTGACTTTGTTTCTGCAAAGGCACCTGCATGTGTCTGTGTTCGTGTAGGCATAATGTGAGGAGGTGGGGAGAGCACCCCACCCCCATGTCCACCATGACCCTCTTCCCACGCTGACCTGTGCTCCCTCTCCAATCATCTTTCCTGTTCCAGAGAGGTGGGGCTGAGGTGTCTCCATCTCTGTCTCAACTTCATGGTGCACTGAGCTGTAACTTCTTCCTTCCCTATTAAAA"));
    REQUIRE(getFullSequence(id2, all_nodes) == std::string("GGTGGACCTGAAAGAAAGATTAATTATATATTTTATTAGATAGCACATTCATTCTTAATTATCAGCAGAATATTTTTGTAAAATATTGACAACATAACATAAAACATGATTTAAAATATAGTGACCAAAAAACATCAAAGATCATCTGACTGTCTGGGATGGGGGTGGGGCTCGGGAGAAGGAACATAATGGAAAACATTTGCCGTCAGAAGAAGGTGTAACTTATCTTTTTACATCTCTTTCTCTAACTCTGAAAATGAACTGTGAACTGGAGCTCTCTTGACCACGCTGGTACCTAAAATTCTCCCATCTCTTCCCCAGCACCTTCCAGCGTCCTCTTTACCCAGCAACAGAGAATGTCAGCTCTATGATTTCTCTGATAGGTGAATCCCAGCCATGCTGATTCCTCTCCACCCATTTCCAGTGCTAGAGGCCCACAGTTTCAGTCTCATCTGCCTCCACTCGGCCTCAGTTCCTCATCACTGTTCCTGTGCTCACAGTCATCAATTATAGACCCCACAACATGCGCCCTGAAGACAGAATGTTCCATATCAGAGCTGTGATCTTGAGAGCCCTCTCCTTGGCTTTCCTGCTGAGTCTCCGAGGAGCTGGGGCCATCAAGGGTGAGTGCTCAGGAGGACGCAGGAGCGTCGGGGTGAGTGATGGGGTGGTTCACATCAATTGCTGCTTCAGGGATCACAGATTTTAGGGGCTCATTGATCTATCTGGTCCTCATAGTCTATGTTCCCTCTGGCCCTCATAATAATAACAGCAATAACAGCCAGAATTTATGAGACTCCTGCATAGTTTCTTTCCCCATTTACATCTCACAGGAATCTTCAATGAAGATAATATTCCATTCATTTAGAAATTATTCCTTTTATTTAGAAATTATTTTGAAAAAACTGAAGCTCAAAAAGATGAATAAGTTTTCCAAGGTTACACAGCAGATCAACGAGCCAAGTTTGAAGTCCAGACCCAGCTCTGAGGGTCATACACTGCCTTCCCCAGATTCCTGCACACAGTGACCTACTATCAGGGCCCTCCTATCTCTCTGGGATCCCCAGCCTCTATCTTTTGTGGCTGCTTTACAGGAACTCCGAGCTATGGACTCTGCATTAGGAGACGAAGTGCAAAGAGTGTTTCTGTATCCTCCCTCTCTTCTAGGACCCTAGGGCTCTTCCTGGGTCTTTGTGGGTGGTCACAAGCTTTCCTCTCTCAAGACAGCAGGGTTGCATGGTCTTGATAGCCTTGTGATTCGGGTTCTGAGAGATTCAGGACTGCAAGGGAGGCCTAGACTTTTGATAGCTGCAAGGACTCAGCCAGAGATGGACCGTAGTGAATGCTCCTTTTTCCTGTAGCTGAAATCAGGGAGAATGACATCAAGCCTGTGCATGATGCTGTCATTCCAAAATCTAGTGATGGGGAAGGTTAGAATCCATAACGTACAAGATGCACACTGGCTTCAGACAGTTTTATTTAAGATGTGTAGAATAAAGAGGAGGTCAGGCTGGGTAGAACCAGAAGTATCTATTGCCCTGTTCGCGGTCACCTGAGTTATTTCTAATGTTATGTTATAATAAACACCACAATAGGCTTCTCTTCATAGATGCAAATACTTTTTAGTATTCTTGGTAGAAATTCCTAATGAGCTCAGCTGTCTCTTCAGGGCTTCCCTGCCCAGTCTCTTAACATTTAAACATGTCATTTACCTTAAAAACATAAGTGCAAACCAACTGATAAAAAACAACCTTGCCTTCAGTCTGCATCCTGTCCCAGAGACACTTTCTTTGTGTCCTCACACGTGGAGCTAAGCTTCTGACTTGTCTCTGGTACATCCCTGAGGATCCTCTCATCTTGGCCATCAGGAACCTCTACAGAAGGTCAAATTCAGTGGGTTCTTCTCAGTGCCTCTGACTTGAGTTACTAATAACATTTGCACTATAATCCACTTCTTTCTGATGAACTACCCTGTCCTTATTTTTCTCCTGTTTACCTGGATCCTCCTTATCATCTTTTAAACCACCTCTTAACTATCATGTTCTCTCATTATACCCTGAGATCTCGGCAATTCTGATTTTTGGCACTCTTCCTGGAAAATCTTATTTAACCTGCACCTGCCACTAATGACTCTCAGTTCTATGGCCTAAATTCCTCTCCTGAGACCACCCATAATCCACAAATATCTATGTATTATTTCTCCTTAGATGACTTTCAGGTCTTCTAAGTGCAATAGCCCCACAGTAAACTCAGTATCTTCTCCCGGTCAGGCTGTCTTCCCTGAGAGAAGTGGCTTTTGCCCTGTTTTCTGAATGCCTACATTGAAGCCATCTGTTCCCCAGGAAGCCTTCCCTGATGTGCTGTTTGGTCGCATCTTGTGTATACCTACGTATCTGCACTTATCCTTCTGAACCTGCTGTTGTCCTGTCACTTGTGTTTCCTTCTGTGACTTATACGCGTCTGCAGAACAGGACGTATGTATTATTTTTATTTGGGTATTTAGCATCTAACAGTGTTTGACATATAGTAGTCTTTTAATACATATTTTTGTCTGAATGGAAATGATATTTTGAAGAAAAATAATCTGTTCCATAGCTGGCTGATCTTTGGACTGCAGAACTTGTGAAAGTGTTTTTTAAAAAGCATTTTAAAAAGTACAAGGGACATTCATGTATTAAGAAGATGAGTTTCCAATAACTGCTAGAGGACTTTGTGTCTTTTTATTTTACCCTCTTTTTCCTGATGAGTCCTTTGAGTCCTTTAAACTGAGGAGCAAGCTAAGTTTCCTAGTGAAATACCTATAGGATTTGTTTTGTTTAGTTTCAAATACCACTCTTTGCTTGGCCACTTACTGTGTCAGGGAGTCATTCTCAGTGAAAAATAAGACACAGGTCATACCCTCTAGACACTTACAATTACAGTGGCAAGGAGTCATTCTCCTGTCACTGTAAGTGGCCAAGCACAGACTGGGTCCCCACATGTCAGGGCTGAAAACTCACAGGGAAATCTGTGAGTTGGGAGGTGAGAGCAGAAGAGTCCCGTAGTTCCTTCTCACTCTGATGCATTTATCATTCTAAACCCAGACTTTCACATACACATTCATCGTTTTCTTTCATGATAATAGTTGCTTTTATCCTCTTATCTTTGCTAATTCTTACAAACTAATAAAGACTAAGAAACAAAATAAATTAAATCCTACAGGTGTTCCAAACTCAGCAATAATTTCTAGTTGGCCTCTAAAACAAAAATCAAAATATAAATGTAAGAAAAGTTTAGAATGCTTAGTACCTGTGTGATGAAATAATCTGTACACTAAACCCCCAAGTCATGAGTTTACCTATACAACAAACCTGCACATGTACTCCTGAACATAAAATAAATGTTGAAATATTTTTAAAAAGGAAACAAAAGTTTGGAACAAATGCCAAAATAACTGTACTGTACTTTTGAATTTATATGCCCCAAATGAAAAATATTATCAACAAAGCTATACATTCTACAGTTTCATGTTCATAAACTAAGACAGAAACTTTAAAACTGTCAAGAGCCCTAAAATTTGAAGGATATTTTCTTCTTCCTCTCAATTTTGTATTTTTTTCTACCTTTTCTATAATAAGAAAAAGAAAATGTCCATTCCCCCACCCCCATGACTCTAAAAACAATTTTACATCTGTGTCATAGAAAAATTAAGATCTTAATGGGAGAGAAAACCTCTCTACTAGTTCCGCCAGTAGCCGTATGACCTTAGCAAGTTATTAATATGTAACTTCCCTGCATTTCCTTACCTGTAAAATGTATGATATGTATTTGCTTCATAGGGTTATTGTGACAATTCAGCGAGTGAAATATGTAAAGTATTTAGAAGGATGCCTGGCACAAGTAAGTGCTCAACAAATGTTAGCTGTCATTGTTACTATTACTATTGTGTAGGGTCAGGATGCCCAGACTTTCAAAGACCAGGAAGCAGCTTGACTTATCAGTGATAAACTTTTCATTTTGTTCTTTGCTCCTTTCTTTTTATAACTGCTCATCTGCTCTGTATTATTTCCTTTATGGTGTTGCTCCTTCTTCTTCCCCATATGTCCTTCCTTTGACCTCTTACCTTCTTCCTTTTTATATTCATAAGTCTTTATTCATTCTCTAGCTTTGACCACTTGCATATTCAAACTGACATTTTGTCGTGTTTTTCTCTACTGTCTTTATGCAGCGGACCATGTGTCAACTTATGCCGCGTTTGTACAGACGCATAGACCAACAGGGGAGTTTATGTTTGAATTTGATGAAGATGAGATGTTCTATGTGGATCTGGACAAGAAGGAGACCGTCTGGCATCTGGAGGAGTTTGGCCAAGCCTTTTCCTTTGAGGCTCAGGGCGGGCTGGCTAACATTGCTATATTGAACAACAACTTGAATACCTTGATCCAGCGTTCCAACCACACTCAGGCCACCAACGGTACGCCCTATCTTTGCCTCTTCCTCTGTAGCCCAACTGGAAGGGATGAGAGGGCCTCTCTGCCACCCTCAGACTAGGAAGCCTAAGTGCCCCCTGCTGTGTGATCCTCTTCCCCTAGTGGCCATGGGCTGATCCCACTACAGCAAGGGCTTGCATCCTCTCTTCTCAGGAGAGAGAAAGGTGAGCAGAGTGAGGCTGGTCAGTGGTGTGATACCCCTCTCTGTGATTCAGAGCTGCCATAAAATCTAAGGCTGAGGTAGAGGACCACCCTCCCCTAAGAGGTGGAGCCTTTGTGATTCATCCCAGAAGAGGGGCCTAACCTGGTGCTGTCTCCTTCCAGATCCCCCTGAGGTGACCGTGTTTCCCAAGGAGCCTGTGGAGCTGGGCCAGCCCAACACCCTCATCTGCCACATTGACAAGTTCTTCCCACCAGTGCTCAACGTCACGTGGCTGTGCAACGGGGAGCTGGTCACTGAGGGTGTCGCTGAGAGCCTCTTCCTGCCCAGAACAGATTACAGCTTCCACAAGTTCCATTACCTGACCTTTGTGCCCTCAGCAGAGGACTTCTATGACTGCAGGGTGGAGCACTGGGGCTTGGACCAGCCGCTCCTCAAGCACTGGGGTATGCAACTGCTTTTCTCTCCATAATCTCCTGGCATCCTCTATTCCAAAGACCTGGTGTCCTCTGCACCAGCTTTCCGCACTGGCTGGGTCTCAGTCCTCTCCTCGTCCTAACATCCAATTAACTGGTCCATAACCTTCAATTCCCACAACCATCCCAGGCCATCACCACCCTCACTGCACCTCCTGACCCTATCTCTTCATTCTTCCCCCAGAGGCCCAAGAGCCAATCCAGATGCCTGAGACAACGGAGACTGTGCTCTGTGCCCTGGGCCTGGTGCTGGGCCTAGTCGGCATCATCGTGGGCACCGTCCTCATCATAAAGTCTCTGCGTTCTGGCCATGACCCCCGGGCCCAGGGGACCCTGTGAAATACTGTAAAGGTGGGAATGTAAAGAGGAGGCCCTAGGATTTGTAGAATGTAAGGAAGGGAGGAAAAATTCAATCTGATAAGTGTTCATTGATCTTCTAATGGGTTAAAAGCATTCAGCCACATAACAACAACAACACCGATAACTAACTGAGTAGTTAATATGGTCAGGCGCTATTCTGAGGATTTACATTTATTAACTCACTTTATTCTCACACATAGTCTTTGAGGTAGGTACTATTATTTTCACTATTTCACATGAGAGATACTTACATCTTTTTACATACACAGAGACTTTAAGCACTTTGATCAAGTTCCCACAGCTATGAAGTAGTAGGGCTAGCTTCCAATCCAGAAAGTCTGGATCCAAGACTGTTTATCCACTGTCCTATTCACCCTATTTTGTGAAGGAAAAGACCAAGTTCAAATTCTCCAGAGTCCATTGCCAAATAATGGAGTCAGATCTATATTTCTATACATAATTACAACACAGTGTGGTGGGTGCCTGTAACTACTTACTGTCTCTACTTGGACTCATTCCATGGCAATGTTCACACAAAAAATGCCCCTCCAGAGATCTTACAGGTTTCTATTTATCATAACACTCACCATGCTTTATATTTTTATATGTTTTGGGAATTCTCTTAGCATTAGACAGTGAACTTCCATGCAGATGACCACATCTAATTCATTATTATTATTGTTATTCATGCTGGACCTCAGGTACAAAAGGTTAAGAACTTCTCAGTTCATTATATGATCATCATTGGTGCCTCCGAGCTCTCTCTCTCTCCCTTGATTTATTTGGTCCCTTTTATCTCCAGTCCTTACTCCCATATCTAACCTCTTACCCCTACCTCATAGGTAAACATTTTAATGAATTTGATGTTTCCTTTTATTTGCATAGATCCTCTGTAATATGTAGTAGTGTCCAGTGTACATGTATTTTTAATTAACCAAAATGGCATTAAATTATAGATCTAATTTTGTACATCCAGTTTGTTTCTTCCAAATCTTCCATAGTATTTTACTTTATATGTCCATGCATTAGTCCATTTTGCATTGCTATAAAGGAATATCTGAAGTTACCTAATTTACGAAGAAAAGAGCTTTAAATGGCTCACAGATCTGCAGGCTGTACGCGAAACATGGCACTAGCATCTGCTTCTGTTGGGGGATTCTGGAAGCTTTTACTCATGGTGGAAGGCAAGTGGAGCCAGTGCATCACATGGTCATAGAGGGAGAAAGAGACATAGAAAGAGGTGCCAGCCTCTTTTTAACAACCAGGTTTCATGTGCACTAATAGAGTGAGAACTCACTCATTACCCGGAGAGGGGACAAAGCCATTCATGAGGGTCTCCTCCATGATTCAAATACCTCCCACCAGGCCCCACCTGCAACACTGGGGATCAATTTTCAACATGAGACTTGGAAGTGACAAATATCCAAATCATATTAATCCACATATCTACATTGCTCCTGGGATACCTGGATCATTCCTGGTTCTCTACTATTGCAAGCAATGCTTGTATCTCACATGGAACTGCATATACATGTGGGCCTGACCTGCATCCCTGGAATGTATGTATCCTAGAAAGGGGTTGCAGGGTTGCTGGAGATGCAGCTCCTTAATTTGACTAAACACTGCTCATCTTCTCATCAGAATGGCTGTACTCATCTGAACTTCCTTTGTCAGTACTCTAATTGTCCTGCAACTCCTAAATGGACTTCAACACTGGACATTATCCAGTTTTCTAACTTTTGCCAATTTCATGTGCATAAAGAAATATGCTGTTTTATTTTGCATTTCTTTAATTACTAATAATTGGGGCTATAATTAGGACTGATTAGCCACTTGGGGGTTCCTTTTCTATAAATTGCCTGTTCACATTCATTGTCCATTTTTGTACTATGTGCTTCCATCATTTTCTTATTGATTTGCAGGTGATCCTTATATAGTCCTGCTAGTAGTCCCTTGTCAGTTTTAGGCATTGCAAATGTTTTCCTCTAATCTGACTTCTGGCAACTGTCTCCTTGGTTTCCTTTATTGAAGAGAAATCCTTAATATTTTGTAATGAAGTCCATCAACTGTATTTTTGTTTGTGTGTCTTTTTTAAAAGAAGTCTTCCCTATACTGAGATATCAAAGATACTCTTAAAACATCTCCTACAGTTTTAAATTTCACATTTACTACTTTAATTCATCTGGGATTCATCTTTGTGTTTGATGGGGATCATGTTTTATTTTTCTTTATATAATGGGCCAGTGTGTTCCCACAACTACTAAATAGTTCACCTTTTCCCCATAGGTTAGTAGTGTCTCCTTTGCTATACTGAAAGCTCCCATTATAGGTGGGCCTGTGTCTGAGTTCCATCTTGTTCCACTGTTCTGTTTGTCTCTTCTTGTGCCAGTGTCCTAGTATTTTGATTACTATGACATTGTAGTGTGTGTTAGTATCCAGTAGGACAAATTCTTGTTTATTTTTCTTAGTTCACACACATTTATAATTATATCTATAATGATTTGTAACAGAGTGAAGTGAATGTAGAATGTCAGATGTTAAGAGGAAGAATGGAAAAGAGGGCTGGGACTAGGGTGATGTAGGGGATGCACCTGGCTTAGGTGCAAAATTTGGGGGATACCAAAAGAACTCAGTAATAAATCATATTTTAATGAAATATCTTGAAAAGGCAAAATTAATGCAAAGATACATGATTAACAAAACATCCAAAGAGGAGTATTTAACAAAAATGGAGAAGCAGAGAAGCAGAAGAATTAGGAGAATATGCTGTCACATGAGCCAAGGAATTAAAGAATTCAGGAAGGAGGAAGTACTGCTGTCAGATGTTCAACAGAGGTCATTTTAGAAAATTTACCTTGGTTTTTGAAATCCTTTCAAAGAGCAGTATACACAATGTGAGCAAGTATCCTTCGTTCATTGCCGTCATTGATATGGTTTGGATATTTGTCCCTTCCAATTCTCATTCCAGGGTTAAGCTTCTTCTCTGCCCTCAGTAATGTGGCCCTTCCCCTTGTCTGTATATTTTGGAGACATGAAGCATGTGGGATGGCCTCACAGTCAGCTGGGGTTTGAGGGTGAAATTCAATGACTTTCGTGAACTCCTTGGCTCCTATGTGCTCTTCACCTGGAGGACCAGGGCATGTGCAGGGATGACCACCTTCTCCCTGGGACCTGAACAGGGCAGAGAAATGGGAAGCTCGGGTGCAAAGGGAGTGGGGAAGATGGGTCCGGGCTTACAGTACTGAACCCAGGAATGACAATAACTGTGTGTGTTGCTGCAGGTGACAAAATATCTGAACAGAAGAGGACTTAGGAGAGATCTGAACTCCAGCTGCCCTACAAACTCCATCTCAGCTTTTCTTCTCACTTCATGTGAAAACTACTCCAGTGGCTGACTGAATTGCTGACCCTTCAAGCTCTGTCCTTATCCATTACCTCAAAGCAGTCATTCCTTAGTAAAGTTTCCAACAAATAGAAATTAATGACACTTTGGTAGCACTAATATGGAGATTATCCTTTCATTGAGCCTTTTATCCTCTGTTCTCCTTTGAAGAACCCCTCACTGTCACCTTCCCGAGAATACCCTAAGACCAATAAATACTTCAGTATTTCAGAGCGGGGAGACTCTGAGTCATTCTTACTGGAAGTCTAGGACCAGGTCACATGTGAATACTATTTCTTGAAGGTGTGGTTTCAACCTCTGTTGCCGATGTGGTTACTAAAGGTTCTGATCCCACTTGAACGGAAAGGTCTGAGGATATTGATTCAGTCCTGGGTTTTTCCCTAACTACAGGATAGGGTGGGGTAGAGAAAGGATATTTGGGGGAAATTTTACTTGGATGAAGATTTTCTTGGATGTAGTTTGAAGACTGCAGTGTTTGAAGTCTCTGAGGGAAGAGATTTGGTCTGTCTGGATCAAGATTTCAGGCAGATTAGGATTCCATTCACAGCCCCTGAGCTTCCTTCCCAAGGCTGTATTGTAATTATAGCAATATTTCATGGAGGATTTTTCTACATGATAAACTAAGAGCCAAGAAATAAAATTTTTAAAATGCCCTAATTCATTGCAATTTTTACCAGCCATAGTCACTCCATGTGGGAGAACTTAAATCATGATTACCAGAGCTTTCAAAGGTTTGAGAATAGTGATGATTATGAAGAAAAATATCTTATTTGAGCAAGGATTTTGTTTCTTTATGAGTGTTCATTAGATATTACGATGAAAAAAGCATGAAATGGTAAAAATTCAGATAAATATAAAAACATGTTCTCTAGTTTTTTTTAAGTTAAAAAAGGAATTGTTTAAAGTA"));
  }

  SECTION("getFullSequence should work using names with id_table, but not with all_nodes")
  {
    char * name1 = (char *)"HLA-DPA1*01:03:01:03";
    char * name2 = (char *)"HLA-A*01:01:01:01";

    REQUIRE(getFullSequence(name1, id_table) != "Id not found!");
    REQUIRE(getFullSequence(name2, id_table) != "Id not found!");

    REQUIRE(getFullSequence(name1, all_nodes) == "Id not found!");
    REQUIRE(getFullSequence(name2, all_nodes) == "Id not found!");
  }

  // SECTION("getFullSequence should work using subnames with id_table, but not with all_nodes")
  // {
  //   char * subname1 = (char *)"HLA-DPA1*01:03:01";
  //   char * subname2 = (char *)"HLA-A*01:01:01";
  //   char * subname3 = (char *)"HLA-DPA1*01";
  //
  //   REQUIRE(getFullSequence(subname1, id_table) != "Id not found!");
  //   REQUIRE(getFullSequence(subname2, id_table) != "Id not found!");
  //   REQUIRE(getFullSequence(subname3, id_table) != "Id not found!");
  //
  //   REQUIRE(getFullSequence(subname1, all_nodes) == "Id not found!");
  //   REQUIRE(getFullSequence(subname2, all_nodes) == "Id not found!");
  //   REQUIRE(getFullSequence(subname3, all_nodes) == "Id not found!");
  // }

  SECTION("getFullSequence shouldn't work otherwise")
  {
    REQUIRE(getFullSequence((char *) "", id_table) == "Id not found!");
    REQUIRE(getFullSequence((char *) "TST-A", id_table) == "Id not found!");
    REQUIRE(getFullSequence((char *) "asdfa", id_table) == "Id not found!");

    REQUIRE(getFullSequence((char *) "", all_nodes) == "Id not found!");
    REQUIRE(getFullSequence((char *) "TST-A", all_nodes) == "Id not found!");
    REQUIRE(getFullSequence((char *) "asdfa", all_nodes) == "Id not found!");
  }
}


/**
 * getAllFullSequences returns all full sequences
 */
TEST_CASE("getAllFullSequences should work with both id_table and all_nodes")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);

  SECTION("getAllFullSequences using id_table")
  {
    std::vector<std::string> results1 = getAllFullSequences(my_root_node, id_table);
    REQUIRE(results1.size() == 6);

    REQUIRE(results1[0] == std::string("GATTATTAGA"));
    REQUIRE(results1[1] == std::string("GTTTATTAGA"));
    REQUIRE(results1[2] == std::string("GATAAAATGTT"));
    REQUIRE(results1[3] == std::string("GATTATTAGA"));
  }

  SECTION("getAllFullSequences using all_nodes")
  {
    std::vector<std::string> results2 = getAllFullSequences(my_root_node, all_nodes);
    REQUIRE(results2.size() == 6);

    REQUIRE(results2[0] == std::string("GATTATTAGA"));
    REQUIRE(results2[1] == std::string("GTTTATTAGA"));
    REQUIRE(results2[2] == std::string("GATAAAATGTT"));
    REQUIRE(results2[3] == std::string("GATTATTAGA"));
  }

  SECTION("getAllFullSequences using specific gene with id_table")
  {
    std::map<std::string, xml_node<> *> my_id_table = getIdTable(my_root_node, std::string("test"));
    REQUIRE(my_id_table.size() >= 4);

    std::vector<std::string> results3 = getAllFullSequences(my_root_node, my_id_table);
    REQUIRE(results3.size() == 4);

    REQUIRE(results3[0] == std::string("GATTATTAGA"));
    REQUIRE(results3[1] == std::string("GTTTATTAGA"));
    REQUIRE(results3[2] == std::string("GATAAAATGTT"));
    REQUIRE(results3[3] == std::string("GATTATTAGA"));
  }

  SECTION("getAllFullSequences using specific gene with all_nodes")
  {
    std::map<std::string, xml_node<> *> my_all_nodes = getAllNodes(my_root_node, std::string("test"));
    REQUIRE(my_all_nodes.size() == 4);

    std::vector<std::string> results4 = getAllFullSequences(my_root_node, my_all_nodes);
    REQUIRE(results4.size() == 4);

    REQUIRE(results4[0] == std::string("GATTATTAGA"));
    REQUIRE(results4[1] == std::string("GTTTATTAGA"));
    REQUIRE(results4[2] == std::string("GATAAAATGTT"));
    REQUIRE(results4[3] == std::string("GATTATTAGA"));
  }
}

TEST_CASE("getFeatureNames should return all feature names")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  // std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);


  SECTION("it should work when the reference is the allele itself")
  {
    std::vector<std::string> all_features = getFeatureNames((char *) "T1", id_table);
    REQUIRE(all_features.size() == 5);

    REQUIRE(all_features[0] == "5' UTR");
    REQUIRE(all_features[1] == "Exon 1");
    REQUIRE(all_features[2] == "Intron 1");
    REQUIRE(all_features[3] == "Exon 2");
    REQUIRE(all_features[4] == "3' UTR");
  }


  SECTION("it should work when the reference is not the allele itself")
  {
    std::vector<std::string> all_features = getFeatureNames((char *) "T2", id_table);
    REQUIRE(all_features.size() == 5);

    REQUIRE(all_features[0] == "5' UTR");
    REQUIRE(all_features[1] == "Exon 1");
    REQUIRE(all_features[2] == "Intron 1");
    REQUIRE(all_features[3] == "Exon 2");
    REQUIRE(all_features[4] == "3' UTR");
  }
}


TEST_CASE("getFeatureNames should return all feature names on real data")
{
  using namespace rapidxml;

  char * xml = (char *)"hla.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  // std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);


  SECTION("it should work when the reference is the allele itself")
  {
    std::vector<std::string> all_features = getFeatureNames((char *) "HLA01409", id_table);
    REQUIRE(all_features.size() == 9);

    REQUIRE(all_features[0] == "5' UTR");
    REQUIRE(all_features[1] == "Exon 1");
    REQUIRE(all_features[2] == "Intron 1");
    REQUIRE(all_features[3] == "Exon 2");
    REQUIRE(all_features[4] == "Intron 2");
    REQUIRE(all_features[5] == "Exon 3");
    REQUIRE(all_features[6] == "Intron 3");
    REQUIRE(all_features[7] == "Exon 4");
    REQUIRE(all_features[8] == "3' UTR");
  }


  SECTION("it should work when the reference is not the allele itself")
  {
    std::vector<std::string> all_features = getFeatureNames((char *) "HLA00601", id_table);
    REQUIRE(all_features.size() == 9);

    REQUIRE(all_features[0] == "5' UTR");
    REQUIRE(all_features[1] == "Exon 1");
    REQUIRE(all_features[2] == "Intron 1");
    REQUIRE(all_features[3] == "Exon 2");
    REQUIRE(all_features[4] == "Intron 2");
    REQUIRE(all_features[5] == "Exon 3");
    REQUIRE(all_features[6] == "Intron 3");
    REQUIRE(all_features[7] == "Exon 4");
    REQUIRE(all_features[8] == "3' UTR");
  }
}

TEST_CASE("getFullFeature should return the sequence of a feature")
{
  using namespace rapidxml;

  char * xml = (char *)"dummy.xml";
  file<> xmlFile(xml);
  xml_document<char> doc;
  doc.parse<0>(xmlFile.data());
  xml_node<> * my_root_node = doc.first_node();

  std::map<std::string, xml_node<> *> id_table = getIdTable(my_root_node);
  // std::map<std::string, rapidxml::xml_node<> *> all_nodes = getAllNodes(my_root_node);


  SECTION("it should work when the reference is the allele itself")
  {
    std::vector<std::string> all_features = getFeatureNames((char *) "T1", id_table);

    REQUIRE(all_features.size() == 5);

    // 5' UTR
    std::string feature_sequence = getFullFeature((char *) "T1", id_table, all_features[0]);
    REQUIRE(feature_sequence == "G");

    // Exon 1
    feature_sequence = getFullFeature((char *) "T1", id_table, all_features[1]);
    REQUIRE(feature_sequence == "AT");

    // Intron 1
    feature_sequence = getFullFeature((char *) "T1", id_table, all_features[2]);
    REQUIRE(feature_sequence == "TA");

    // Exon 2
    feature_sequence = getFullFeature((char *) "T1", id_table, all_features[3]);
    REQUIRE(feature_sequence == "TTA");

    // 3' UTR
    feature_sequence = getFullFeature((char *) "T1", id_table, all_features[4]);
    REQUIRE(feature_sequence == "GA");
  }


  SECTION("it should work when the reference is not the allele itself")
  {
    std::vector<std::string> all_features = getFeatureNames((char *) "T2", id_table);

    REQUIRE(all_features.size() == 5);

    std::string feature_sequence;

    // 5' UTR
    feature_sequence = getFullFeature((char *) "T2", id_table, all_features[0]);
    REQUIRE(feature_sequence == "G");

    // Exon 1
    feature_sequence = getFullFeature((char *) "T2", id_table, all_features[1]);
    REQUIRE(feature_sequence == "TT");

    // Intron 1
    feature_sequence = getFullFeature((char *) "T2", id_table, all_features[2]);
    REQUIRE(feature_sequence == "TA");

    // Exon 2
    feature_sequence = getFullFeature((char *) "T2", id_table, all_features[3]);
    REQUIRE(feature_sequence == "TTA");

    // 3' UTR
    feature_sequence = getFullFeature((char *) "T2", id_table, all_features[4]);
    REQUIRE(feature_sequence == "GA");
  }

  SECTION("One more...")
  {
    std::vector<std::string> all_features = getFeatureNames((char *) "T3", id_table);

    REQUIRE(all_features.size() == 5);

    std::string feature_sequence;

    // 5' UTR
    feature_sequence = getFullFeature((char *) "T3", id_table, all_features[0]);
    REQUIRE(feature_sequence == "G");

    // Exon 1
    feature_sequence = getFullFeature((char *) "T3", id_table, all_features[1]);
    REQUIRE(feature_sequence == "AT");

    // Intron 1
    feature_sequence = getFullFeature((char *) "T3", id_table, all_features[2]);
    REQUIRE(feature_sequence == "AA");

    // Exon 2
    feature_sequence = getFullFeature((char *) "T3", id_table, all_features[3]);
    REQUIRE(feature_sequence == "AAT");

    // 3' UTR
    feature_sequence = getFullFeature((char *) "T3", id_table, all_features[4]);
    REQUIRE(feature_sequence == "GT");
  }
}
